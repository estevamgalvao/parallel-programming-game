#include "definitions.h"
#include "player.h"
#include "board.h"

int* slotss_ = (int*) calloc((BOARD_SIZE*BOARD_SIZE) + 2, sizeof(int)); 

void* player_function(void* id);
// void* player_function(void* id);

typedef struct {
    int id;
    int num;
} DadosPlayer;

int main()
{   

    /* initial positions hard coded - i can change that after if i've time*/
    slotss_[18] = 1;
    slotss_[26] = 2;
    pthread_t thread_player[2];
    int player_id[2] = {1,2};
    
    // = (DadosPlayer*)malloc(sizeof(DadosPlayer));
  
    int iret1, iret2;

    // player_id[0] = 1;
    // player_id[0] = 2;

    iret1 = pthread_create(&(thread_player[0]), NULL, player_function, &player_id[0]);
    iret2 = pthread_create(&(thread_player[1]), NULL, player_function, &player_id[1]);

    pthread_join(thread_player[0], NULL);
    pthread_join(thread_player[1], NULL);
    
    printf("Thread 1 returns: %d\n", iret1);
    printf("Thread 2 returns: %d\n", iret2);
    exit(0);
};


void* player_function(void* id) {
    
    Player jogador(1, slotss_);
    int id_jogador = *((int*) id);
    printf("Sou a thread %ld\n", pthread_self());
    printf("PEÇAS: %d e ID JOGADOR %d\n", jogador.GetPiecesCounter(), id_jogador);
    // tabuleiro.PrintBoard();
    // exit(0);
    pthread_exit(0);
}

void* task2(void* id) {
    int id_jogador = *((int*) id);
    Player jogador(1, slotss_);
    // printf("Olá.\n");
    printf("PEÇAS: %d e ID JOGADOR %d\n", jogador.GetPiecesCounter(), id_jogador);
    printf("Sou a thread %ld\n", pthread_self());
    // tabuleiro.PrintBoard();
    // exit(0);
    pthread_exit(0);

}