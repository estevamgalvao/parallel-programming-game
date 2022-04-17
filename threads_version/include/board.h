#ifndef BOARD_H
#define BOARD_H

#include "definitions.h"

/* class designed to be the board itself and manage all actions related to 
each possible game's slot */
class Board {
    private:
        int *slotss_;
        int shm_id_, sem_id_;
        key_t key_;

    public:
        Board(int* board);
        virtual ~Board();
        int* GetSlotss();
        void PrintBoard();
        bool VerifyMove(int x, int y, int player_id);
        void MakeMove(int x, int y, int player_id);

};


#endif //BOARD_H