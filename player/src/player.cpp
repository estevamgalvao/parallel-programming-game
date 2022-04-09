#include "player.h"


Player::Player(int id) {
    id_ = id;

    key_ = ftok(KEY_PATH, KEY_ID);
    shm_id_ = shmget(key_, BOARD_SIZE*BOARD_SIZE * sizeof(int), 0666 | IPC_CREAT); 
    //0666 soma das permisões (permisão pra tudo)
    //IPC CREAT se não tem memória compartilhada com essa chave, cria
    
    slotss_ = (int*) shmat(shm_id_, NULL, 0);

    private_board_ = (int*) calloc(BOARD_SIZE*BOARD_SIZE, sizeof(int));
    private_board_[5] = 1;
    private_board_[60] = 2;

    picked_pos_.push_back(std::pair<int, int>(0,5));
}

Player::~Player() {
    shmdt(slotss_); //largando o ponteiro compartilhado (o pai que dá o free da memoria)
    // free(private_board_);
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

bool Player::VerifyMove(int x, int y, int which_board) {
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

    if (board[d] != 0)
    {
        // printf("Posição já ocupada.\n");
        return false;
    }
    
    /* not border spots */
    if(x > 0 && y > 0 && x < BOARD_SIZE-1 && y < BOARD_SIZE-1) {
        // printf("sou safe interno\n");
        for (int i = -9; i <= -7; i++)
        {
            if (board[d+i] == id_) {return true;}
        }

        for (int i = -1; i <= 1; i++)
        {
            if (board[d+i] == id_) {return true;}
        }

        for (int i = 7; i <= 9; i++)
        {
            if (board[d+i] == id_) {return true;}
        }
    }

    /* dealing with the 4 corner-spots*/
    else if (x+y == 0) {                     // [0,0]
        // printf("sou um canto [0,0]\n");
        if (board[d+1] == id_) {return true;}
        if (board[d+8] == id_) {return true;}
        if (board[d+9] == id_) {return true;}
    }

    else if (x+y == BOARD_SIZE-1 && y > 0) { // [0,7]
        // printf("sou um canto [0,7]\n");
        if (board[d-1] == id_) {return true;}
        if (board[d+7] == id_) {return true;}
        if (board[d+8] == id_) {return true;}
    }
    
    else if (x+y == BOARD_SIZE-1 && x > 0) { // [7,0]
        // printf("sou um canto [7,0]\n");
        if (board[d-8] == id_) {return true;}
        if (board[d-7] == id_) {return true;}
        if (board[d+1] == id_) {return true;}
    }

    else if (x+y == 2*(BOARD_SIZE-1)) {      // [7,7]
        // printf("sou um canto [7,7]\n");
        if (board[d-9] == id_) {return true;}
        if (board[d-8] == id_) {return true;}
        if (board[d-1] == id_) {return true;}
    }

    /* boundary spots but not corner*/
    else {
            if (x == 0) // [0, 1-6]
            {
                // printf("estou na borda x = 0 mas não sou [0,0] ou [0,7]\n");
                for (int i = -1; i <= 1; i++)
                {
                    if (board[d+i] == id_) {return true;}
                }

                for (int i = 7; i <= 9; i++)
                {
                    if (board[d+i] == id_) {return true;}
                }
            }

            else if (x == 7) // [7, 1-6]
            {
                // printf("estou na borda x = 7 mas não sou [7,0] ou [7,7]\n");
                for (int i = -9; i <= -7; i++)
                {
                    if (board[d+i] == id_) {return true;}
                }

                for (int i = -1; i <= 1; i++)
                {
                    if (board[d+i] == id_) {return true;}
                }
            }

            else if (y == 0) // [1-6, 0]
            {
                // printf("estou na borda y = 0 mas não sou [0,0] ou [7,0]\n");
                if (board[d-8] == id_) {return true;}
                if (board[d-7] == id_) {return true;}
                if (board[d+1] == id_) {return true;}
                if (board[d+8] == id_) {return true;}
                if (board[d+9] == id_) {return true;}
            }
            
            else if (y == 7) // [1-6, 7]
            {
                // printf("estou na borda y = 7 mas não sou [0,7] ou [7,7]\n");
                if (board[d-9] == id_) {return true;}
                if (board[d-8] == id_) {return true;}
                if (board[d-1] == id_) {return true;}
                if (board[d+7] == id_) {return true;}
                if (board[d+8] == id_) {return true;}
            }          
    }
    printf("sou o player %d e não consegui marcar\n", id_);
    return false;
};

int Player::MakeMove(int x, int y, int which_board) {
    // int* board;
    int d = 8 * x + y; // distance inside the memory array 1D as if it was 2D
    int i = this->PickAMove(x, y, which_board);
    int target_x, target_y;


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
            printf("deu bigode %d\n", i);
            break;
        }
    slotss_[d+i] = id_;
    private_board_[d+i] = id_;
    picked_pos_.push_back(std::pair<int, int>(target_x, target_y));
    return 1;
    }
    else {
        printf("verdadeiro bigode %d\n", i);
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

    if (board[d] != 0)
    {
        // printf("Posição já ocupada.\n");
        return false;
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
    return 0;
};


void Player::Play() {
    bool flag = true;

    while(flag) {

        for (auto it = picked_pos_.end(); it != picked_pos_.begin(); it--)
        {
            if (this->MakeMove(it->first, it->second))
            {
                this->PrintBoard(1);
                printf("Marquei.\n");
                sleep(2);
                break;
            }
            else {
                flag = false;
            }
        }
    }
    
}
