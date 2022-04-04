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

    std::cout << "entrei no jogo ";    
    std::cout << player_id << std::endl;

    player.PrintBoard();
    // std::cout << player.VerifyMove(0,2,1) << std::endl; // não está entrando nessa função.
    player.MakeMove(0,2);
    player.MakeMove(1,1);
    player.PrintBoard();

    player.~Player();
    return 0;
}
