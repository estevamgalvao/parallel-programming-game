#ifndef PLAYER_H
#define PLAYER_H

#include "definitions.h"
// #include "board.h"

class Player {
    private:
        int id_;
        // Board private_board_;

    public:
        virtual ~Player() {};
        Player(int id);
        void PickMove();

};

#endif //PLAYER_H