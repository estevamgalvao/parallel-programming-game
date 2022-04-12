#ifndef BOARD_H
#define BOARD_H

#include "definitions.h"

/* class designed to be the board itself and manage all actions related to 
each possible game's slot */
class Board {
    private:
        std::array<std::array<int, BOARD_SIZE + 2>, BOARD_SIZE + 2> slots_;
        int *slotss_;
        int shm_id_, sem_id_;
        key_t key_;


    public:
        Board();
        virtual ~Board();
        void PrintBoard();
        bool VerifyMove(int x, int y, int player_id);
        void MakeMove(int x, int y, int player_id);

};


#endif //BOARD_H