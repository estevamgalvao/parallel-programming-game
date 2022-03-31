#ifndef BOARD_H
#define BOARD_H

#include "definitions.h"


class Board {
    private:
        // std::vector<std::array<int, 3>> ocuppied_slots_;
        std::array<std::array<int, BOARD_SIZE + 2>, BOARD_SIZE + 2> slots_;

    public:
        Board();
        virtual ~Board() {};
        void PrintBoard();
        bool VerifyMove(int x, int y, int player_id);
        void MakeMove(int x, int y, int player_id);

};


#endif //BOARD_H