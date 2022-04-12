#include "board.h"
#include "definitions.h"


Board::Board() {
    key_ = ftok(KEY_PATH, KEY_ID);

    shm_id_ = shmget(key_, ((BOARD_SIZE*BOARD_SIZE) + 2) * sizeof(int), 0666 | IPC_CREAT); 
    //0666 soma das permisões (permisão pra tudo)
    //IPC CREAT se não tem memória compartilhada com essa chave, cria
    slotss_ = (int*) shmat(shm_id_, NULL, 0);

    for (size_t i = 0; i < BOARD_SIZE*BOARD_SIZE; i++)
    {
        slotss_[i] = 0;
    }

    /* semáforo */    
    sem_id_ = semget(key_, 2, 0666 | IPC_CREAT); 


    /* initial positions hard coded - i can change that after if i've time*/
    slotss_[3] = 1;
    slotss_[60] = 2;
    
    slotss_[(BOARD_SIZE*BOARD_SIZE)] = 3;
    slotss_[(BOARD_SIZE*BOARD_SIZE) + 1] = 60;
};

Board::~Board() {
    shmdt(slotss_);
    shmctl(shm_id_, IPC_RMID, NULL);
}


void Board::PrintBoard() {
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


bool Board::VerifyMove(int x, int y, int player_id) {
    x += 1;
    y += 1;
    if (slots_[x][y] != 0)
    {   
        printf("Posição já ocupada.\n");
        return false;
    }
    else
    {
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {   
                if (slots_[x+i][y+j] == player_id)
                {
                    // printf("Posição permitida.\n");
                    return true;
                }
                // std::cout << "Pixel: ";
                // std::cout << slots_[x+i][y+j] << std::endl;
            }
        }
    }
    // printf("Posição inválida.\n");
    return false;
}

void Board::MakeMove(int x, int y, int player_id) {
    
    if (VerifyMove(x, y, player_id))
    {   
        // printf("Escrevendo. %d %d\n", x, y);
        this->slots_[x+1][y+1] = player_id;
    }
}