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

    while (int i = 40 > 0)
    {
        tabuleiro.PrintBoard();
        sleep(1);
        i--;
        if (tabuleiro.GetSlotss()[BOARD_SIZE*BOARD_SIZE] != 0 
        && tabuleiro.GetSlotss()[(BOARD_SIZE*BOARD_SIZE) + 1] != 0) {
            printf("Alguém venceu.\n");
            break;
        }
    }
    
    printf("Acabou o jogo.\n");

    /* waiting the return from the child processes */
    waitpid(p_id1, &p_status1, 0);
    waitpid(p_id2, &p_status2, 0);

    tabuleiro.~Board();
    return 0;
}
