#include "game.h"

#include "io.h"
#include "utils.h"
#include <time.h>
#include <stdlib.h>

// Function to generate a random degree to the rotation
static int randomDegree()
{
    int aux = rand() % 4;
    switch(aux) {
        case 0: return 0;
        case 1: return 90;
        case 2: return 180;
        case 3: return 270;
    }

    // unreachable statement (Since, if it gets to the default case, the execution is aborted). Just to shutdown warning.
    return 0;
}

// Function used to generate the map size, number of pieces per type and the first player attacking
static void generateSetup(int* p_map_size, int nr_per_piece[5], int* p_player_attacking)
{
    // Generate map size
    *p_map_size = ((rand() % 20) + 1) + 20;

    // Generate number of piece per type
    int nrBarcosLeft = (*p_map_size * *p_map_size) / 25; 
    for(int i = 4; i >= 0; i--) {
        // Make sure every type has, at least, one piece
        nr_per_piece[i] = (rand() % (nrBarcosLeft - i)) + 1; 
        nrBarcosLeft -= nr_per_piece[i];
    }

    // Generate the first player attacking
    *p_player_attacking = (rand() % 2 == 0) ? 0 : 1;
}


static void chooseGame(Game* game)
{
    // Read the setup
    bool confirmed = false;
    int map_size;
    int nr_per_piece[5];
    do {
        prompt_IO(READ_SETUP_IO, &map_size, nr_per_piece, &game->player_attacking);
        prompt_IO(CONFIRM_SETUP_IO, map_size, nr_per_piece, game->player_attacking, &confirmed, 0);
    } while(!confirmed);

    // Alloc and read the map of the players
    for(int id_player = 0; id_player < 2; id_player++) {

        game->players[id_player] = new_Player(map_size);

        for(int piece_type = 0; piece_type < 5; piece_type++) {
            char type = getType_Utils(piece_type);
            
            for(int piece_number = 1; piece_number <= nr_per_piece[piece_type]; piece_number++) {

                Piece* piece = new_Piece();
                int px, py, degree_of_rotation;

                int resultAddingPiece;
                do {
                    prompt_IO(READ_PIECE_IO, id_player, piece_number, type, &px, &py, &degree_of_rotation);
                    update_Piece(piece, type, px, py, degree_of_rotation);
                    resultAddingPiece = addPiece_Player(game->players[id_player], piece);
                    prompt_IO(RESULT_ADDING_PIECE_IO, resultAddingPiece);
                } while(resultAddingPiece != 0);
            }
        }
        prompt_IO(PIECES_MAP_IO, game->players[id_player], id_player);
    }
}

static void randomGame(Game* game)
{
    // Get a new seed, based on current time of the system
    srand(time(NULL));

    bool confirmed = false;
    int map_size;
    int nr_per_piece[5];
    do {
        generateSetup(&map_size, nr_per_piece, &game->player_attacking);
        prompt_IO(CONFIRM_SETUP_IO, map_size, nr_per_piece, game->player_attacking, &confirmed, 1);
    } while(!confirmed);
        
    for(int p = 0; p < 2; p++) {
        game->players[p] = new_Player(map_size);

        for(int i = 0; i < 5; i++) {
            char type = getType_Utils(i);

            for(int j = 0; j < nr_per_piece[i]; j++) {
                
                Piece* piece = new_Piece();
                int px, py, degree_of_rotation;
                
                do {
                    // This variables px and py correspond to the center of the piece, so a valid piece, must have this values, inside the map.
                    px = rand() % map_size;
                    py = rand() % map_size;
                    
                    // Random degree of rotation
                    degree_of_rotation = randomDegree();
                    
                    // Update the piece
                    update_Piece(piece, type, px, py, degree_of_rotation);
                
                    // Tries to add the piece to the map till the piece can be (when the return is 0 that means that the piece could and was attactched to the map, so we break the loop)
                } while(addPiece_Player(game->players[p], piece) != 0); 
            }
        }
        // Print the map of pieces
        prompt_IO(PIECES_MAP_IO, game->players[p], p);
    }
}

// Simple function to change turn, that is, if player attacking is 0, now it should be 1 and vice-versa.
static void changeTurn(Game* game) 
{
    game->player_attacking = (game->player_attacking + 1) % 2;
}

// Deallocs the game and all the resources needed
static void free_Game(Game* game)
{
    free_Player(game->players[0]);
    free_Player(game->players[1]);
    free(game);
}


Game* init_Game()
{
    // Alloc, dynamically, the game
    Game* game = (Game*) malloc(sizeof(Game));
    if(game == NULL)
        prompt_IO(ERROR_IO, "game.c, new_Game(): malloc failed");   

    // Prompt to ask if the game will be random generated or choosen manual
    bool randomize;
    prompt_IO(READ_RANDOMIZE_IO, &randomize);

    switch(randomize) {
        // Manual choose
        case false: chooseGame(game); break;
        // Random generation
        case true: randomGame(game); break;
    }

    return game;
}

void playTurn_Game(Game* game) 
{   
    // Get the (x,y) coordinates of the attack
    int x, y;
    prompt_IO(ATTACK_COORDINATES_IO, PLAYER_ATTACKING, &x, &y);
    
    // Attack the player and get the result of the attack
    int attack_result = registerAttack_Player(game->players[PLAYER_UNDER_ATTACK], x, y);

    // Register the attack on the player attacking
    registerShot_Player(game->players[PLAYER_ATTACKING], x, y, attack_result);
    
    // Print the result of the attack
    prompt_IO(ATTACK_RESULT_IO, attack_result);
    // Print the shots map of the player attacking
    prompt_IO(SHOTS_MAP_IO, game->players[PLAYER_ATTACKING]);
    
    // Change turns
    changeTurn(game);
}

bool over_Game(Game* game)
{
    /* 
        We only need to see if the last player that suffer the attack still has hp, 
        but since we've made a call to switch the turns, that player is now the player attacking,
        and if he lost, the player that won is now the player under the attack
    */
    if(game->players[PLAYER_ATTACKING]->hp == 0) {
        prompt_IO(GAME_OVER_IO, PLAYER_UNDER_ATTACK);
        return true;
    }
    return false;
}

bool exit_Game(Game* game)
{
    // At this point, the game is finished, so we can free the game and all resources needed
    free_Game(game);

    // Prompt to play again. The variable play_again if is setted to true, then is to play again, if is setted to false, then isn't to play again.
    bool play_again;
    prompt_IO(PLAY_AGAIN_IO, &play_again);
    return !play_again;
}
