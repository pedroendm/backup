#include "player.h"

#include "io.h"
#include <stdlib.h>

Player* new_Player(int map_size) {
    Player* player = (Player*) malloc(sizeof(Player));
    
    // Case malloc failed, print that malloc failed and abort execution 
    if(player == NULL)
        prompt_IO(ERROR_IO, "player.c, new_Player(): malloc failed");

    // Set the hp of the player to 0
    player->hp = 0;
    // And alloc his map
    player->map = new_Map(map_size);

    return player;
}

int addPiece_Player(Player* player, Piece* piece)
{
    // (Tries to) add a piece to the map
    int resultAddPiece = addPiece_Map(player->map, piece);
    
    // If the result is 0, than that means that the piece was added
    if(resultAddPiece == 0)
        //So if was added, increase the hp by 5 (every piece is "size" 5)
        player->hp += 5;
    return resultAddPiece;
}

int getPieceStatus_Player(Player* player, int x, int y)
{
    return getPieceStatus_Map(player->map, x, y);
}

int getShotStatus_Player(Player* player, int x, int y)
{
    return getShotStatus_Map(player->map, x, y);
}

char getPieceType_Player(Player* player, int x, int y)
{
    return getPieceType_Map(player->map, x, y);
}

int registerAttack_Player(Player* player, int x, int y)
{
   // Register the attack on the map
   int result = registerAttack_Map(player->map, x, y);

   // If the attack result is between 1 and 5, inclusive, then some piece was hitted sucessfully, so we can decrease the hp of the player by 1.
   if(result >= 1 && result <= 5) player->hp -= 1;

   return result; 
}

void registerShot_Player(Player* player, int x, int y, int attack_result)
{
    switch(attack_result) {
        // Attack went outside the map. Nothing to register.
        case -1: break;
        
        // Missed attack. 
        case 0: registerShot_Map(player->map, x, y, 1); break;
        
        // Attack on piece I with sucess
        case 1: registerShot_Map(player->map, x, y, 2); break;

        // Attack on piece P with sucess        
        case 2: registerShot_Map(player->map, x, y, 3); break;

        // Attack on piece T with sucess
        case 3: registerShot_Map(player->map, x, y, 4); break;

        // Attack on piece X with sucess
        case 4: registerShot_Map(player->map, x, y, 5); break;

        // Attack on piece Z with sucess
        case 5: registerShot_Map(player->map, x, y, 6); break;

        // Attack on piece already hitted. Nothing to register.
        case 6: break;
    }
}

void free_Player(Player* player) 
{
    free_Map(player->map);
    free(player);
}