#ifndef PLAYER_H
#define PLAYER_H

#include "definitions.h"
#include "board.h"

class Player {
    private:
        int id_;
        int pieces_counter_;
        int* slotss_;
        int* private_board_;
        /* Atributes intended to establish the shared memory link */
        int shm_id_;
        key_t key_;

    public:
        Player(int id);
        virtual ~Player();
        bool VerifyMove(int x, int j, int which_board = 0);
        void PrintBoard(int which_board = 0);

};

#endif //PLAYER_H