#ifndef PLAYER_H
#define PLAYER_H

#include "definitions.h"
#include "board.h"

class Player {
    private:
        Board private_board_;

    public:
        virtual ~Player() {};
        void PickMove();

};

#endif //PLAYER_H