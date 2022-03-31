#include "board.h"
#include "definitions.h"


Board::Board() {
    // ocuppied_slots_.push_back()
};


void Board::PrintBoard() {
    for (size_t i = 1; i < BOARD_SIZE; i++)
    {
        printf("[ ]\n");
        for (size_t j = 1; j < BOARD_SIZE; j++)
        {
            printf("[ ] ");
        }
    }
    
};