#include "board.h"
#include "definitions.h"


Board::Board() {
    slots_ = {};
    slots_[0][0] = 1;
    slots_[0][6] = 1;
    slots_[7][1] = 2;
};


void Board::PrintBoard() {
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {   
            if (slots_[i][j] != 0)
            {  
                std::cout << "[";
                std::cout << slots_[i][j];
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