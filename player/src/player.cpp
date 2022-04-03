#include "player.h"


Player::Player(int id) {
    id_ = id;

    key_ = ftok(KEY_PATH, KEY_ID);
    shm_id_ = shmget(key_, BOARD_SIZE*BOARD_SIZE * sizeof(int), 0666 | IPC_CREAT); 
    //0666 soma das permisões (permisão pra tudo)
    //IPC CREAT se não tem memória compartilhada com essa chave, cria
    
    slotss_ = (int*) shmat(shm_id_, NULL, 0);

    private_board_ = (int*) calloc(BOARD_SIZE*BOARD_SIZE, sizeof(int));
    private_board_[8] = 18;

}

Player::~Player() {
    shmdt(slotss_); //largando o ponteiro compartilhado (o pai que dá o free da memoria)
    free(private_board_);
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