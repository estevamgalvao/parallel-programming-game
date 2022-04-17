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
    
    player.GetSemaphore(1);
    if(player.CheckConsistency()) {
        printf("Jogador %d possui as mesmas peças no tabuleiro público e privado.\n", player_id);
    }
    
    printf("Jogador %d parou de jogar. Marcou %d peças.\n", player_id, player.GetPiecesCounter());
    player.ReleaseSemaphore(1);

    return player.GetPiecesCounter();
}
