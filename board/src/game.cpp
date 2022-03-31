#include "board.h"

int main(int argc, char const *argv[])
{
    /* code */
    Board tabuleiro;
    
    tabuleiro.PrintBoard();
    tabuleiro.MakeMove(7, 0, 1);
    tabuleiro.MakeMove(7, 0, 2);
    tabuleiro.PrintBoard();
    tabuleiro.MakeMove(7, 7, 2);
    tabuleiro.PrintBoard();

    

    return 0;
}
