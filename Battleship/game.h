/*
    game.h
    Representation of the state of the game.
 */

#ifndef GAME_H
#define GAME_H

#include "player.h"

// Definition of the game
typedef struct Game
{
    // Stores the of the player that is attacking
    int player_attacking;
    // The two players
    Player* players[2];
} Game;

//  Build the game: prompting the configurations and allocating all resources needed
Game* init_Game();

/*
    The player attacking chooses an (x,y) to attack the other player. 
    In the end, they switch, so the player that suffer the attack is now the next player attacking
    and the player attacking is now the next player suffering the next attack.
*/
void playTurn_Game(Game*);

//   Returns true if the game ended, ie, one of the players has all pieces destroyed, false otherwise.
bool over_Game(Game*);

/*
    Deallocs the game and all the resources allocated by the game.
    Returns 'false' if it's to play again, 'true' otherwise.
*/
bool exit_Game(Game*);

#endif