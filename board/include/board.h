#ifndef BOARD_H
#define BOARD_H

#include "definitions.h"


class Board {
    private:
        std::vector<std::array<int, 3>> ocuppied_slots_;
        std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> slots_;

    public:
        Board();
        virtual ~Board() {};
        void PrintBoard();
        bool VerifyMove(std::tuple<int,int,int> move);
        void MakeMove(std::tuple<int,int,int> move);

};


#endif //BOARD_H