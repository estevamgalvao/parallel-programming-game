#include "definitions.h"
#include "player.h"
#include "board.h"

int* slotss_ = (int*) calloc((BOARD_SIZE*BOARD_SIZE) + 2, sizeof(int)); 
pthread_mutex_t mutex[2];

void* player_function(void* id);

int main()
{
    /* initializing the board */
    Board board_obj(slotss_);
    /* initial positions hard coded - i can change that after if i've time*/
    slotss_[18] = 1;
    slotss_[26] = 2;

    pthread_t thread_player[2];


    int player_id[2] = {1,2};

    int tc_ret1, tc_ret2;

    tc_ret1 = pthread_create(&(thread_player[0]), NULL, player_function, &player_id[0]);
    tc_ret2 = pthread_create(&(thread_player[1]), NULL, player_function, &player_id[1]);

    if (tc_ret1 != 0 || tc_ret2 != 0) {
        printf("Threads weren't created properly. Exiting.\n");
        exit(0);
    }
    else{
        printf("Jogadores entraram no jogo.\n");
    }

    pthread_mutex_lock(&mutex[1]);
    while (true)
    {
        board_obj.PrintBoard();
        // sleep(1);
        if (board_obj.GetSlotss()[BOARD_SIZE*BOARD_SIZE] != 0 
        && board_obj.GetSlotss()[(BOARD_SIZE*BOARD_SIZE) + 1] != 0) {
            break;
        }
    }
    pthread_mutex_unlock(&mutex[1]);

    pthread_join(thread_player[0], NULL);
    pthread_join(thread_player[1], NULL);

    if (slotss_[BOARD_SIZE*BOARD_SIZE] > slotss_[BOARD_SIZE*BOARD_SIZE + 1]) {
        printf("\nAcabou o jogo. O jogador 1 é o campeão.\n");
    }
    else if (slotss_[BOARD_SIZE*BOARD_SIZE] < slotss_[BOARD_SIZE*BOARD_SIZE + 1]) {
        printf("\nAcabou o jogo. O jogador 2 é o campeão.\n");
    }
    else {
        printf("\nAcabou o jogo. Os jogadores empataram.\n");
    }

    exit(0);
};


void* player_function(void* id) {
    sleep(1);
    int player_id = *((int*) id);
    Player player(player_id, slotss_, &mutex[0]);

    // printf("---- Sou a thread %ld ----\n", pthread_self());

    // printf("Jogador %d entrou no jogo.\n", player_id);

    player.Play();
    pthread_mutex_lock(&mutex[1]);
    if(player.CheckConsistency()) {
        printf("Jogador %d possui as mesmas peças no tabuleiro público e privado.\n", player_id);
    }
    else {
        printf("ERRO! Jogador %d possui tabuleiros inconsistentes.\n", player_id);
    }
    
    printf("Jogador %d parou de jogar. Marcou %d peças.\n", player_id, player.GetPiecesCounter());
    pthread_mutex_unlock(&mutex[1]);
    pthread_exit(0);
}
