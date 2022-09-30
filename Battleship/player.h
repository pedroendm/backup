/*
  player.h
  Representation of a player.

  The piece has a hp and a map.
  The hp corresponds to the number of (x,y) coordinates in the map that have a piece not destroyed.
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"

// Definition of the player
typedef struct Player 
{
    int hp;
    Map* map;
} Player;

// Alocs a new player and his map of map_size * map_size. Also, his hp is setted to 0.
Player* new_Player(int map_size);

/*
    Tries to add a piece to the map of the player.
    If the piece can be added, the piece is added, the hp increases by 5 (all pieces have "size" of 5) and the function returns 'true'.
    Otherwise, the piece isn't added, and the function returns 'false'.
*/
int addPiece_Player(Player* player, Piece* piece);

// Returns the shot status of the cell of the map of the player on position (x,y) in the map. 
int getShotStatus_Player(Player* player, int x, int y);

// Returns the piece status of the cell of the map of the player on position (x,y) in the map. 
int getPieceStatus_Player(Player* player, int x, int y);

// Returns the type of the piece the player on position (x,y) in the map
// For efficient reasons, it's not safe, it doesn't verify if the piece exists. 
// If doesn't get verified, because in the program, when we call this function we had always verify if the piece existed, before.
char getPieceType_Player(Player* player, int x, int y);

/* 
    Attacks a player on coordinate (x,y) and returns a code based on the sucess of the attack:
    If the return value is -1, than the attack went outside the map.
    If the return value is 0, than was attacked a cell without piece.
    If the return value is 1, than there was a piece of type I, not hitted, that is, was a hit shot.
    If the return value is 2, than there was a piece of type P, not hitted, that is, was a hit shot.
    If the return value is 3, than there was a piece of type T, not hitted, that is, was a hit shot.
    If the return value is 4, than there was a piece of type X, not hitted, that is, was a hit shot.
    If the return value is 5, than there was a piece of type Z, not hitted, that is, was a hit shot.
    If the return value is 6, than there was a piece, but was already destroyed.

    If the attack was a hit shot, then the player's hp decrease by one.
 */
int registerAttack_Player(Player* player, int x, int y);

// Marks the cell of the player with the shot made
void registerShot_Player(Player*, int x, int y, int attack_result);

// Frees the player and all the resources allocated in it
void free_Player(Player*);


#endif