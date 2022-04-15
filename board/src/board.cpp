#include "board.h"
#include "definitions.h"


Board::Board() {
    key_ = ftok(KEY_PATH, KEY_ID);

    /* create the semaphore with external key key_ if it doesn't already 
    exists. Permissions = ALL. */
    shm_id_ = shmget(key_, ((BOARD_SIZE*BOARD_SIZE) + 1) * sizeof(int), 0666 | IPC_CREAT); 
    
    /* always check system returns. */
    if(shm_id_ < 0)
    {
      fprintf(stderr, "Unable to obtain shared memory.\n");
      exit(0);
    }
    
    slotss_ = (int*) shmat(shm_id_, NULL, 0);

    for (size_t i = 0; i < (BOARD_SIZE*BOARD_SIZE) + 1; i++)
    {
        slotss_[i] = 0;
    }

    /* dealing with semaphore */
    union semun {
        int val;
        struct semid_ds *buf;
        ushort * array;
    } argument;
    
    argument.val = 1;
  
    /* create the semaphore with external key key_ if it doesn't already 
    exists. Permissions = ALL. */

    sem_id_ = semget(key_, 2, 0666 | IPC_CREAT);

    /* always check system returns. */
    if(sem_id_ < 0)
    {
        fprintf(stderr, "Unable to obtain semaphore.\n");
        exit(0);
    }

    /* what we actually get is an array of semaphores. The second 
     argument to semget() was the array dimension - in our case
     1. */
  
    /* set the value of the number 0 semaphore in semaphore array
     # id to the value 0. */

    if( semctl(sem_id_, 0, SETVAL, argument) < 0)
    {
        fprintf( stderr, "Cannot set semaphore value.\n");
    }
    else
    {
        fprintf(stderr, "Semaphore %d initialized.\n", key_);
    }

    /* initial positions hard coded - i can change that after if i've time*/
    slotss_[18] = 1;
    slotss_[26] = 2;
};

Board::~Board() {
    shmdt(slotss_);
    shmctl(shm_id_, IPC_RMID, NULL);
}

int* Board::GetSlotss() {
    return slotss_;
}

void Board::PrintBoard() {
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {   
            if (slotss_[i*BOARD_SIZE + j] != 0)
            {  
                std::cout << "[";
                std::cout << slotss_[i*BOARD_SIZE + j];
                std::cout << "] ";
            }
            else {
                printf("[ ] ");
            };
        }
        printf("\n");
    }
    printf("\n");
};