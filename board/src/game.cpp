#include "board.h"

int main()
{
    Board tabuleiro;
    pid_t p_id1, p_id2;
    int p_status1, p_status2;

    tabuleiro.PrintBoard();

    char* player_id1[] = {PLAYER_EXEC_PATH, "1", NULL};
    char* player_id2[] = {PLAYER_EXEC_PATH, "2", NULL};
    
    /* forking the new processes and when they're done, executing the new exe files at PLAYER_EXEC_PATH */
    p_id1 = fork();
    if (p_id1 == 0) {execvp(PLAYER_EXEC_PATH, player_id1);}

    p_id2 = fork();
    if (p_id2 == 0) {execvp(PLAYER_EXEC_PATH, player_id2);}

    while (true)
    {
        tabuleiro.PrintBoard();
        // sleep(1);
        if (tabuleiro.GetSlotss()[BOARD_SIZE*BOARD_SIZE] != 0 
        && tabuleiro.GetSlotss()[(BOARD_SIZE*BOARD_SIZE) + 1] != 0) {
            break;
        }
    }

    /* waiting the return from the child processes */
    waitpid(p_id1, &p_status1, 0);
    waitpid(p_id2, &p_status2, 0);

    if (tabuleiro.GetSlotss()[BOARD_SIZE*BOARD_SIZE] > tabuleiro.GetSlotss()[BOARD_SIZE*BOARD_SIZE + 1]) {
        printf("\nAcabou o jogo. O jogador 1 é o campeão.\n");
    }
    else if (tabuleiro.GetSlotss()[BOARD_SIZE*BOARD_SIZE] < tabuleiro.GetSlotss()[BOARD_SIZE*BOARD_SIZE + 1]) {
        printf("\nAcabou o jogo. O jogador 2 é o campeão.\n");
    }
    else {
        printf("\nAcabou o jogo. Os jogadores empataram.\n");
    }

    printf("Acabou o jogo.\n");

    tabuleiro.~Board();
    return 0;
}
