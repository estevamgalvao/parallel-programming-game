#include "definitions.h"
#include "player.h"


int main(int argc, char const *argv[])
{   
    if (argc < 2) {
        printf("Missing argument player ID.\n");
        return -1;
    }

    int player_id = atoi(argv[1]);
    // int player_id = 1;
    
    Player player(player_id);

    printf("Jogador %d entrou no jogo.\n", player_id);

    player.Play();
    
    printf("\nParei de jogar. Marquei %d peças.\n", player.GetPiecesCounter());

    // player.~Player();
    return player.GetPiecesCounter();
}
