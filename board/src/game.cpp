#include "board.h"

int main(int argc, char const *argv[])
{
    /* code */
    Board tabuleiro;
    pid_t p_id1, p_id2;
    int p_status1, p_status2;

    tabuleiro.PrintBoard();
    
    // tabuleiro.MakeMove(7, 0, 1);

    char* player_id1[] = {PLAYER_EXEC_PATH, "1", NULL};
    char* player_id2[] = {PLAYER_EXEC_PATH, "2", NULL};
    
    p_id1 = fork();

    if (p_id1 == 0)
    {
        execvp(PLAYER_EXEC_PATH, player_id1);
    }

    p_id2 = fork();

    if (p_id2 == 0)
    {
        execvp(PLAYER_EXEC_PATH, player_id2);
    }



    // deixar printando sem parar o tabuleiro dando clear e print
    

    // printf("waitpid");
    waitpid(p_id1, &p_status1, 0); // é pretendido que seja blocante
    waitpid(p_id2, &p_status2, 0); // é pretendido que seja blocante

    tabuleiro.~Board();
    return 0;
}
