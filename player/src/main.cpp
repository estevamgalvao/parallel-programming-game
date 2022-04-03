#include "definitions.h"
#include "player.h"


int main(int argc, char const *argv[])
{   
    if (argc < 2) {
        printf("Missing argument player ID.\n");
        return -1;
    }

    int player_id = atoi(argv[1]);

    Player player(player_id);

    std::cout << "entrei no jogo ";    
    std::cout << player_id << std::endl;

    player.PrintBoard();
    player.PrintBoard(1);


    player.~Player();
    return 0;
}
