#ifndef BOARD_H
#define BOARD_H

#include "definitions.h"


class Board {
    private:
        std::vector<std::tuple<int, int, int>> ocuppied_slots_;

    public:
        virtual ~Board() {};
        void PrintBoard();
        bool VerifyMove(std::tuple<int,int,int> move);
        void MakeMove(std::tuple<int,int,int> move);

};


#endif //BOARD_H