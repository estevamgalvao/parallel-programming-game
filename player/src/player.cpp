#include "player.h"


Player::Player(int id) {
    id_ = id;

    key_ = ftok(KEY_PATH, KEY_ID);

    //0666 soma das permisões (permisão pra tudo)
    //IPC CREAT se não tem memória compartilhada com essa chave, cria
    shm_id_ = shmget(key_, BOARD_SIZE*BOARD_SIZE * sizeof(int), 0666); 
    /* Always check system returns. */
    if(shm_id_ < 0)
    {
      printf("Shared memory doens't exist. I'm player %d\n", id_);
      exit(0);
    }

    sem_id_ = semget(key_, 1, 0666);
    /* Always check system returns. */
    if(sem_id_ < 0)
    {
        printf("Semaphore doesn't exist. I'm player %d\n", id_);
        exit(0);
    }

    /* identifier of this semaphore inside the array of sem */
    sem_operations_[0].sem_num = 0;
    /* which operation? subtract 1 (i'm getting busy) */
    sem_operations_[0].sem_op = -1;
    /* what should i do if its busy? wait (0) */
    sem_operations_[0].sem_flg = 0;

    /* inicializando com memória */
    slotss_ = (int*) shmat(shm_id_, NULL, 0);
    private_board_ = (int*) calloc(BOARD_SIZE*BOARD_SIZE, sizeof(int));
    
    /* marcando primeiras posições na memória privada */
    if (id_ == 1) {
        private_board_[3] = 1;
        picked_pos_.push_back(std::pair<int, int>(0,3));
    }
    else if (id_ == 2) {
        private_board_[60] = 2;
        picked_pos_.push_back(std::pair<int, int>(7,4));
    }
    pieces_counter_ = 0;
}

Player::~Player() {
    shmdt(slotss_); //largando o ponteiro compartilhado (o pai que dá o free da memoria)
    // free(private_board_);
}


int Player::GetSemaphore() {
    /* which operation? subtract 1 (i'm getting busy) */
    sem_operations_[0].sem_op = -1;
    int ret_op = semop(sem_id_, sem_operations_, 1);

    if (ret_op < 0) {
        printf("Algo inesperado aconteceu. Não aguardei o semáforo. Sou o player %d\n", id_);
    }

    return ret_op;
}

int Player::ReleaseSemaphore() {
    /* which operation? sum 1 (i'm ceasing to be busy) */
    sem_operations_[0].sem_op = 1;
    int ret_op = semop(sem_id_, sem_operations_, 1);

    if (ret_op < 0) {
        printf("Algo inesperado aconteceu no ReleaseSemaphore. Sou o player %d\n", id_);
    }
    
    return ret_op;
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

    // if (board[d] != 0)
    // {
    //     // printf("Posição já ocupada.\n");
    //     return false;
    // }
    
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
    printf("sou o player %d e não consegui marcar\n", id_);
    printf("minha última coordenada foi [%d, %d]\n", x, y);
    
    printf("VETOR DE COORDENADAS MARCADAS - Jogador %d\n", id_);
    
    for (size_t i = 0; i < picked_pos_.size(); i++)
    {
        printf("[%d, %d] ", picked_pos_[i].first,picked_pos_[i].second);
    }

    printf("\n");

    return 0;
};

int Player::MakeMove(int x, int y, int which_board) {
    // int* board;
    int d = 8 * x + y; // distance inside the memory array 1D as if it was 2D
    //pega semáforo
    int i = this->PickAMove(x, y, which_board);
    //solta semáforo lá embaixo
    int target_x, target_y;

    printf("Valor recebido da função PickAMove, i = %d\n", i);

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

        return 1;
    }
    else {
        // printf("Erro na função MakeMove, vou retonar 0. i = %d\n", i);
    }
    // if (!which_board) {
    //     printf("peguei a memória compartilhada\n");
    // // board = slotss_;
    //     slotss_[d] = id_;
    //     return 1;
    // }
    // else {
    // // printf("peguei o tabuleiro privado\n");
    // // board = private_board_;
    //     private_board_[d] = id_;
    //     return 2;
    // }        
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
            printf("COORDENADA ATUAL - PLAYER %d: [%d, %d]\n", id_, it->first, it->second);
            flag_append_new_pos = true;
            // printf("Coordenada atual do vetor: [%d, %d]\n", it->first, it->second);
            this->GetSemaphore();
            ret_makemove = this->MakeMove(it->first, it->second, which_board);
            this->ReleaseSemaphore();

            if (ret_makemove)
            {
                // this->PrintBoard(1);
                // printf("Marquei.\n");
                flag_append_new_pos = false; // quero verificar se eu saí do loop por q marquei no tabuleiro e atualizei o vetor
                sleep(1);
                break;
            }
            else {
                printf("\n\nMAKE A MOVE Retornou 0. Não entrei no IF JOGADOR %d\n\n", id_);
            }
        }
        // sleep(4);
        printf("Break for -> while occoreu.\n");
        if (flag_append_new_pos) { // se a flag_append_new_pos for false, significa q eu sai do for pq quis e não é pra sair o while, é pra varrer o for atualizado de novo 
            flag = false;
        }
    }

    printf("\nParei de jogar. Não achei nenhuma posição válida no vetor.\n");
    
}
