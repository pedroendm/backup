#include "game.h"

int main(void) 
{  
    Game* game;
    do {
        game = init_Game();
        do
            playTurn_Game(game);
        while(!over_Game(game));
    } while(!exit_Game(game));
    return 0;
}
