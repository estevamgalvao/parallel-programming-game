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

    // int x, y, i = 0;
    // retorno = semctl(player.sem_id_, 0, SETVAL, sem_cons);
    // if (retorno == -1) {
    //     perror("ERRO NO CTL");
    // }
    // printf("...\n");

    player.Play();
    
    // player.PrintBoard();
    // // std::cout << player.VerifyMove(0,2,1) << std::endl; // não está entrando nessa função.
    // player.MakeMove(0,2);
    // player.MakeMove(1,1);
    // player.PrintBoard();

    // você parou finalizando a função de makemove verificando qual tabuleiro
    // - falta colocar em 2 terminais
    // - condição de vitória
    // - pedir ao usuário valores
    // - tratar condição de corrida

    player.~Player();
    return 0;
}
