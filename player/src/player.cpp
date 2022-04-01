#include "player.h"


Player::Player(int id) {
    key_ = ftok(KEY_PATH, KEY_ID);

    shm_id_ = shmget(key_, BOARD_SIZE*BOARD_SIZE * sizeof(int), 0666 | IPC_CREAT); 
    //0666 soma das permisões (permisão pra tudo)
    //IPC CREAT se não tem memória compartilhada com essa chave, cria
    slotss_ = (int*) shmat(shm_id_, NULL, 0);

    id_ = id;

}

Player::~Player() {
    shmdt(slotss_); //largando o ponteiro compartilhado (o pai que dá o free da memoria)
}


void Player::PrintBoard() {
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {   
            if (slotss_[i*BOARD_SIZE + j] != 0)
            {  
                std::cout << "[";
                std::cout << slotss_[i*BOARD_SIZE + j];
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