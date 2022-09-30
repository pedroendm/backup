/*
    io.h
    Module that handles all of the IO activity of the game.
*/

#ifndef IO_H
#define IO_H

/* 
    This function realizes the IO needed.
    The IO needed is identified through an IDENTIFIER (enumerated and documented below).
    For some identifiers, more arguments are needed. 
    This arguments vary and they are needed to get the information to print or where to read.
 */

void prompt_IO(int identifier, ...);

// List of the identifiers to the function prompt_IO
enum IDENTIFIER {
    /* 
        ERROR_IO: prints the string received and abort execution.     
        Parameters: char* (address of the string to print)
     */
    ERROR_IO,

    /*
        READ_RANDOMIZE_IO: IO to read the variable randomize, used to decide if the setup of the game will be choosen or random generated.
        Parameters: bool* (address of the variable where it's gonna be written the choosen option: false for "manual choose" and true for "random generated")
    */
    READ_RANDOMIZE_IO,

    /*
        READ_SETUP_IO: IO to read the setup.
        Parameters: int* (address of the variable that holds the the map size), int* (array to write the number of pieces per type (order of types: I, P, T, X, Z)) and int* (address of the variable that holds the first player attacking)
    */
    READ_SETUP_IO,

    /*
        CONFIRM_SETUP_IO: IO to confirm the setup.
        Parameters: int (map size), int* (array with the numbers of pieces per type), int (first player attacking) 
                    and *bool (to change output text, false for the case of a choosen setup and true in case of a random setup)
    */
    CONFIRM_SETUP_IO,

    /*
        ATTACK_COORDINATES_IO: IO to read the attack coordinates
        Parameters: int (id player attacking), int* (address of the variable that holds the x coordinate of the attack) and int* (address of the variable that holds the y coordinate of the attack)
    */
    ATTACK_COORDINATES_IO,

    /*
        ATTACK_RESULT_IO: IO to print the result of the attack, that is, the fail or sucess of the attack.
        Parameters: int (the result of the attack)
    */
    ATTACK_RESULT_IO,

    /*
        READ_PIECE_IO: IO to add a piece.
        Parameters: int (id player), int (number of the piece), int (type of the piece), 
                    int* (address of the variable that holds the x coordinate of the piece) and int* (address of the variable that holds the y coordinate of the piece) and
                    int* (address of the variable that holds the degree of the rotation of the piece) 
    */
    READ_PIECE_IO,

    /*
        RESULT_ADDING_PIECE_IO: IO to print the result of adding the piece, that is, feedback the player if the piece could be added or not.
        Parameters: int (the result of adding the piece)
    */
    RESULT_ADDING_PIECE_IO, 
    
    /*
        PIECES_MAP_IO: IO used to print the pieces of a player.
        Parameters: Player* (the player to print the pieces) and int (the player's id)
    */
    PIECES_MAP_IO,

    /*
        SHOTS_MAP_IO: IO used to print the shots of a player
        Parameters: Player* (the player to print the shots) and int (the player's id)
    */
    SHOTS_MAP_IO,

    /*
        GAME_OVER_IO: IO to print who won the game.
        Parameters: int (the player's id that won) 
    */
    GAME_OVER_IO,

    /*
        PLAY_AGAIN_IO: IO to ask if the players want to play again.
        Parameters: bool* (address of the variable where it's gonna be written if it's to play again (true) or no (false))
    */
    PLAY_AGAIN_IO
};

#endif