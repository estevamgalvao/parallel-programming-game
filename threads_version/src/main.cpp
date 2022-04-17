#include "definitions.h"
#include "player.h"
#include "board.h"

int* slotss_ = (int*) calloc((BOARD_SIZE*BOARD_SIZE) + 2, sizeof(int)); 
pthread_mutex_t mutex;

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

    int iret1, iret2;

    iret1 = pthread_create(&(thread_player[0]), NULL, player_function, &player_id[0]);
    iret2 = pthread_create(&(thread_player[1]), NULL, player_function, &player_id[1]);

    while (int i = 40 > 0)
    {
        board_obj.PrintBoard();
        sleep(1);
        i--;
        if (board_obj.GetSlotss()[BOARD_SIZE*BOARD_SIZE] != 0 
        && board_obj.GetSlotss()[(BOARD_SIZE*BOARD_SIZE) + 1] != 0) {
            printf("Alguém venceu.\n");
            break;
        }
    }
    
    printf("Acabou o jogo.\n");

    pthread_join(thread_player[0], NULL);
    pthread_join(thread_player[1], NULL);

    exit(0);
};


void* player_function(void* id) {
    int player_id = *((int*) id);
    Player player(player_id, slotss_, mutex);

    printf("---- Sou a thread %ld ----\n", pthread_self());
    printf("Jogador %d entrou no jogo.\n", player_id);


    player.Play();
    if(player.CheckConsistency()) {
        printf("Jogador %d possui as mesmas peças no tabuleiro público e privado.\n", player_id);
    }
    
    printf("Jogador %d parou de jogar. Marcou %d peças.\n", player_id, player.GetPiecesCounter());

    pthread_exit(0);
}
