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
        COORD_VECTOR picked_pos_;

        pthread_mutex_t mutex_;

    public:
        Player(int id, int* board, pthread_mutex_t mutex);
        virtual ~Player();

        int GetPiecesCounter();

        int GetSemaphore();
        int ReleaseSemaphore();

        void PrintBoard(int which_board = 0);

        int PickAMove(int x, int j, int which_board = 0);
        int MakeMove(int x, int j, int which_board = 0);
        void Play(int which_board = 0); //inicializar a primeira posição também

        bool CheckConsistency();
};

#endif //PLAYER_H