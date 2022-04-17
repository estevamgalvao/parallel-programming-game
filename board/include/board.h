#ifndef BOARD_H
#define BOARD_H

#include "definitions.h"

/* class designed to be the board itself and manage all actions related to 
each possible game's slot */
class Board {
    private:
        int *slotss_;
        int shm_id_, sem_id_;

        struct sembuf sem_operations_[1];

        key_t key_;

    public:
        Board();
        virtual ~Board();

        int GetSemaphore(int num_sem);
        int ReleaseSemaphore(int num_sem);

        int* GetSlotss();
        void PrintBoard();
        
        bool VerifyMove(int x, int y, int player_id);
        void MakeMove(int x, int y, int player_id);

};


#endif //BOARD_H