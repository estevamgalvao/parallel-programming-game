#ifndef PLAYER_H
#define PLAYER_H

#include "definitions.h"
// #include "board.h"

class Player {
    private:
        int id_;
        // Board private_board_;
        int *slotss_;
        int shm_id_;
        key_t key_;

    public:
        Player(int id);
        virtual ~Player();
        void PickMove();

        void PrintBoard();

};

#endif //PLAYER_H