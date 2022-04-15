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
        COORD_VECTOR blocked_pos_;

        struct sembuf sem_operations_[1];

        /* Atributes intended to establish the shared memory link */
        int shm_id_, sem_id_;
        key_t key_;

    public:
        Player(int id);
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