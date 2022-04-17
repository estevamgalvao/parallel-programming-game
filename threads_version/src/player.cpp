#include "player.h"


Player::Player(int id, int* board, pthread_mutex_t mutex) {
    id_ = id;
    slotss_ = board;
    mutex_ = mutex;
    // key_ = ftok(KEY_PATH, KEY_ID);

    // /* get the shared memory with external key key_. Permissions = ALL. */
    // shm_id_ = shmget(key_, BOARD_SIZE*BOARD_SIZE * sizeof(int), 0666);

    // /* always check system returns. */
    // if(shm_id_ < 0)
    // {
    //   printf("Shared memory doens't exist. I'm player %d\n", id_);
    //   exit(0);
    // }

    // /* get the semaphore with external key key_. Permissions = ALL. */
    // sem_id_ = semget(key_, 1, 0666);
    // /* always check system returns. */
    // if(sem_id_ < 0)
    // {
    //     printf("Semaphore doesn't exist. I'm player %d\n", id_);
    //     exit(0);
    // }

    // /* identifier of this semaphore inside the array of sem */
    // sem_operations_[0].sem_num = 0;
    // /* which operation? subtract 1 (i'm getting busy) */
    // sem_operations_[0].sem_op = -1;
    // /* what should i do if its busy? wait (0) */
    // sem_operations_[0].sem_flg = 0;

    /* attach the shared memory to our atribute */
    // slotss_ = (int*) shmat(shm_id_, NULL, 0);
    /* initialize the private board for consistence checking purposes */
    private_board_ = (int*) calloc(BOARD_SIZE*BOARD_SIZE, sizeof(int));
    
    /* tagging the initial pos on the private board as well */
    if (id_ == 1) {
        private_board_[18] = 1;
        picked_pos_.push_back(std::pair<int, int>(2,2));
    }
    else if (id_ == 2) {
        private_board_[26] = 2;
        picked_pos_.push_back(std::pair<int, int>(3,2));
    }
    pieces_counter_ = 1;
}

Player::~Player() {
    shmdt(slotss_); //largando o ponteiro compartilhado (o pai que dá o free da memoria)
    // free(private_board_);
}


int Player::GetPiecesCounter() {
    return pieces_counter_;
}


void Player::PrintBoard(int which_board) {

    int* board;
    if (!which_board) {
        board = slotss_;
    }
    else{
        board = private_board_;
    }


    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {   
            if (board[i*BOARD_SIZE + j] != 0)
            {  
                std::cout << "[";
                std::cout << board[i*BOARD_SIZE + j];
                std::cout << "] ";
            }
            else {
                printf("[ ] ");
            };
        }
        printf("\n");
    }
    printf("\n");

};

// ele vai ver se eu posso marcar no 0,0 se eu passar 0,0 para ele
int Player::PickAMove(int x, int y, int which_board) {
    int* board;
    int d = 8 * x + y; // distance inside the memory array 1D as if it was 2D
    if (!which_board) {
        // printf("peguei a memória compartilhada\n");
        board = slotss_;
    }
    else{
        // printf("peguei o tabuleiro privado\n");
        board = private_board_;
    }

    /* not border spots */
    if(x > 0 && y > 0 && x < BOARD_SIZE-1 && y < BOARD_SIZE-1) {
        // printf("sou safe interno\n");
        for (int i = -9; i <= -7; i++)
        {   
            if (board[d+i] == 0) {return i;}
        }

        for (int i = -1; i <= 1; i++)
        {
            if (board[d+i] == 0) {return i;}
        }

        for (int i = 7; i <= 9; i++)
        {
            if (board[d+i] == 0) {return i;}
        }
    }

    /* dealing with the 4 corner-spots*/
    else if (x+y == 0) {                     // [0,0]
        // printf("sou um canto [0,0]\n");
        if (board[d+1] == 0) {return 1;}
        if (board[d+8] == 0) {return 8;}
        if (board[d+9] == 0) {return 9;}
    }

    else if (x+y == BOARD_SIZE-1 && y > 0) { // [0,7]
        // printf("sou um canto [0,7]\n");
        if (board[d-1] == 0) {return -1;}
        if (board[d+7] == 0) {return 7;}
        if (board[d+8] == 0) {return 8;}
    }
    
    else if (x+y == BOARD_SIZE-1 && x > 0) { // [7,0]
        // printf("sou um canto [7,0]\n");
        if (board[d-8] == 0) {return -8;}
        if (board[d-7] == 0) {return -7;}
        if (board[d+1] == 0) {return 1;}
    }

    else if (x+y == 2*(BOARD_SIZE-1)) {      // [7,7]
        // printf("sou um canto [7,7]\n");
        if (board[d-9] == 0) {return -9;}
        if (board[d-8] == 0) {return -8;}
        if (board[d-1] == 0) {return -1;}
    }

    /* boundary spots but not corner*/
    else {
            if (x == 0) // [0, 1-6]
            {
                // printf("estou na borda x = 0 mas não sou [0,0] ou [0,7]\n");
                for (int i = -1; i <= 1; i++)
                {
                    // printf("Estou dentro da função PickAMove. Valor de i = %d\n", i);
                    if (board[d+i] == 0) {return i;}
                }

                for (int i = 7; i <= 9; i++)
                {
                    if (board[d+i] == 0) {return i;}
                }
            }

            else if (x == 7) // [7, 1-6]
            {
                // printf("estou na borda x = 7 mas não sou [7,0] ou [7,7]\n");
                for (int i = -9; i <= -7; i++)
                {
                    if (board[d+i] == 0) {return i;}
                }

                for (int i = -1; i <= 1; i++)
                {
                    if (board[d+i] == 0) {return i;}
                }
            }

            else if (y == 0) // [1-6, 0]
            {
                // printf("estou na borda y = 0 mas não sou [0,0] ou [7,0]\n");
                if (board[d-8] == 0) {return -8;}
                if (board[d-7] == 0) {return -7;}
                if (board[d+1] == 0) {return +1;}
                if (board[d+8] == 0) {return +8;}
                if (board[d+9] == 0) {return +9;}
            }
            
            else if (y == 7) // [1-6, 7]
            {
                // printf("estou na borda y = 7 mas não sou [0,7] ou [7,7]\n");
                if (board[d-9] == 0) {return -9;}
                if (board[d-8] == 0) {return -8;}
                if (board[d-1] == 0) {return -1;}
                if (board[d+7] == 0) {return 7;}
                if (board[d+8] == 0) {return 8;}
            }          
    }
    // printf("sou o player %d e não consegui marcar\n", id_);
    // printf("minha última coordenada foi [%d, %d]\n", x, y);
    
    // printf("VETOR DE COORDENADAS MARCADAS - Jogador %d\n", id_);
    
    // for (size_t i = 0; i < picked_pos_.size(); i++)
    // {
    //     printf("[%d, %d] ", picked_pos_[i].first,picked_pos_[i].second);
    // }
    return 0;
};

int Player::MakeMove(int x, int y, int which_board) {
    int d = 8 * x + y; // distance inside the memory array 1D as if it was 2D
    int i = this->PickAMove(x, y, which_board);
    int target_x, target_y;

    // printf("Valor recebido da função PickAMove, i = %d\n", i);

    if(i) {
        switch (i)
        {
        case -9:
            target_x = x - 1;
            target_y = y - 1;
            break;
        case -8:
            target_x = x - 1;
            target_y = y + 0;
            break;
        case -7:
            target_x = x - 1;
            target_y = y + 1;
            break;
        case -1:
            target_x = x + 0;
            target_y = y - 1;
            break;
        case +1:
            target_x = x + 0;
            target_y = y + 1;
            break;
        case +7:
            target_x = x + 1;
            target_y = y - 1;
            break;
        case +8:
            target_x = x + 1;
            target_y = y + 0;
            break;
        case +9:
            target_x = x + 1;
            target_y = y + 1;
            break;
        default:
            printf("Switch case i != [-9..9]. Não era pra entrar aqui. i = %d\n", i);
            break;
        }
        
        slotss_[d+i] = id_;
        private_board_[d+i] = id_;

        picked_pos_.push_back(std::pair<int, int>(target_x, target_y));
        pieces_counter_ += 1;

        return 1;
    }
    return 0;
}


void Player::Play(int which_board) {
    bool flag = true;
    bool flag_append_new_pos;
    int ret_makemove;

    while(flag) {
        // printf("Entrei no while.\n");
        for (auto it = picked_pos_.end() - 1; it != picked_pos_.begin() - 1; it--)
        {
            // printf("COORDENADA ATUAL - PLAYER %d: [%d, %d]\n", id_, it->first, it->second);
            flag_append_new_pos = true;
            // printf("Coordenada atual do vetor: [%d, %d]\n", it->first, it->second);
            pthread_mutex_lock(&mutex_);
            ret_makemove = this->MakeMove(it->first, it->second, which_board);
            pthread_mutex_unlock(&mutex_);

            if (ret_makemove)
            {
                /* everytime that I append a new pos inside the vector, I want 
                to restart the FOR but without getting out of the while, but if
                I run throught the whole vector and couldn't find a free pos
                to play, so then I want to exit the while as well */
                flag_append_new_pos = false;
                sleep(1);
                break;
            }
        }
        if (flag_append_new_pos) { // se a flag_append_new_pos for false, significa q eu sai do for pq quis e não é pra sair o while, é pra varrer o for atualizado de novo 
            flag = false;
        }
    }
    switch (id_)
    {
    case 1:
        slotss_[BOARD_SIZE * BOARD_SIZE] = id_;
        break;
    case 2:
        slotss_[(BOARD_SIZE * BOARD_SIZE) + 1] = id_;
        break;
    default:
        printf("ERROR - Player ID not recognized.\n");
        break;
    }
    
}

bool Player::CheckConsistency() {
    int d;
    
    for (auto it = picked_pos_.end() - 1; it != picked_pos_.begin() - 1; it--)
        {
            /* distance inside the memory array 1D as if it was 2D */
            d = 8 * it->first + it->second;
            if (slotss_[d] != private_board_[d]) {
                return false;
            }
        }
    return true;
}