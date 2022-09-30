#include "io.h"

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "player.h"

#define BUFFERSIZE 85

/*
    Simple function to read from the stream stdin to a buffer 'buffer'.
    It's safe from input larger than the buffer: its notified that the input is too big, the stream is cleaned and the function returns false.
    Otherwise, the input is written in the buffer and the function returns false.
*/
static bool readInput(char* buffer)
{
    int count = 0;
    while(true) {
        int ch = getchar();
        if(ch == '\n' || ch == EOF) break;
        else if(count > BUFFERSIZE) {
            puts("[System] Input too big! Try again.\n");
            while((ch = getchar()) != '\n' && ch != EOF) ;
            return false;
        } else
            buffer[count++] = ch;
    }
    buffer[count] = '\0';
    return true;
}

/*
    Simple function that returns true if a string can be converted to a number (integer), otherwise false.
    Strings that can be converted to a number are strings with a sequence of numbers together and may have blank characters (space and tabs) in the initial part of the string or after the sequence of numbers.
    If blanck characters appear between the numbers, than it's considered more than one number and thus, false is returned.
*/
static bool canConvertStringToInt(char* str)
{
    bool appearedDigit = false;
    bool isDigit = false;
    while(*str != '\0') {
        if(!isblank(*str)) {
            if(isdigit(*str)) {
                if(appearedDigit && !isDigit) return false;
                if(!appearedDigit) appearedDigit = true;
                isDigit = true;
            } else {
                return false;
            }
        }
        else {
            isDigit = false;
        }
        str++;
    }

    if(appearedDigit)
        return true;
    else
        return false;
}

void prompt_IO(int identifier, ...)
{
    va_list args;
    va_start(args, identifier);

    switch (identifier)
    {
        case ERROR_IO: {
            char* error_message = va_arg(args, char*);
            puts(error_message);
            exit(EXIT_FAILURE);
        }

        case READ_RANDOMIZE_IO:
        {

	    system("clear");
            char buffer[BUFFERSIZE];

            bool *p_randomize = va_arg(args, bool*);

            bool valid = false;
            do {

                // Could read input, properly
                do
                    printf("[System] Write 'random' for a generated setup or 'manual' to choose it.\n[Player] ");
                while (!readInput(buffer));

                // Input verify the restriction
                if(strcmp(buffer, "random") == 0 || strcmp(buffer, "r") == 0) {
                    *p_randomize = true;
                    valid = true;
                }
                else if(strcmp(buffer, "manual") == 0 || strcmp(buffer, "m") == 0) {
                    *p_randomize = false;
                    valid = true;
                }
                // Case input didn't verify
                else
                    printf("[System] Invalid option! Try again.\n\n");

            } while (!valid);
            break;
        }

        case READ_SETUP_IO:
        {
            char buffer[BUFFERSIZE];

            // ----------------------------------------- Read map size ---------------------------------------
            int *p_map_size = va_arg(args, int *);

            bool valid = false;
            do {

                // Could read input, properly
                do
                    printf("[System] Enter a map size between 20 and 40, inclusive:\n[Player] ");
                while(!readInput(buffer));

                // Input is a number
                if(canConvertStringToInt(buffer)) {
                    *p_map_size = atoi(buffer);

                    // Input verify the restriction
                    if(*p_map_size >= 20 && *p_map_size <= 40)
                        valid = true;
                    // Input didn't verify the restriction
                    else
                        printf("[System] Size must be between 20 and 40...\n\n");
                }
                // Input isn't a number
                else
                    printf("[System] Insert a number!\n\n");

            } while(!valid);

            // ------------------------------------- Number of pieces per type --------------------------------
            valid = false;

            int* p_nr_per_piece = va_arg(args, int(*));

            // Number of pieces the game can handle
            int nrBoatsLeft = (*p_map_size * *p_map_size) / 25;

            bool limitReached = false;

            printf("[System] Choose how many pieces you want for a specific type.\n[System] Since the map is %d by %d, is possible to add %d pieces.\n", *p_map_size, *p_map_size, nrBoatsLeft);

            for(int i = 0; i < 5; i++) {
                // Get the corresponding identifier.
                char type = getType_Utils(i);

                do {
                    // Could read input, properly
                    do
                        printf("[System] Can be added %d more pieces. Number of boats of type %c:\n[Player] ", nrBoatsLeft, type);
                    while(!readInput(buffer));

                    // Input is a number
                    if(canConvertStringToInt(buffer)) {
                        p_nr_per_piece[i] = atoi(buffer);

                        // Number introduced is bigger than the number of boats left (can even be negative, because overfollow)
                        if(p_nr_per_piece[i] > nrBoatsLeft || p_nr_per_piece[i] < 0)
                            printf("[System] It's possible to add only more %d pieces. Try adding less.\n", nrBoatsLeft);

                        // Valid number
                        else {
                            nrBoatsLeft -= p_nr_per_piece[i];

                            // Limit max of pieces reached and were other types left yet.
                            if(nrBoatsLeft == 0 && i + 1 != 5) {
                                printf("[System] Limit max of pieces reached. The number of pieces for the remaining types (");
                                for(int j = i + 1; j < 5; j++) {
                                    printf("type %c", getType_Utils(j));
                                    if(j + 1 != 5)
                                        printf(", ");
                                    // Use the loop, also, to set the other types to zero.
                                    p_nr_per_piece[j] = 0;
                                }
                                printf(") will be setted to zero...\n");

                                limitReached = true;
                                break;
                            }

                            valid = true;
                        }
                    }
                    else
                        // Input isn't a number
                        printf("[System] Insert a number!\n\n");

                } while(!valid);

                // In the the case of reaching the max number of pieces, before asking for all types.
                // The reading can stop. The other types are set to 0, already.
                if(limitReached)
                    break;
            }

            // ------------------------------------- Number of pieces per type --------------------------------
            valid = false;

            int *p_player_attacking = va_arg(args, int *);

            do {

                // Could read input, properly
                do
                    printf("[System] Choose the first player attacking. Enter 1 for Player1, 2 for Player2:\n[Player] ");
                while(!readInput(buffer));

                // Input is a number
                if(canConvertStringToInt(buffer)) {
                    *p_player_attacking = atoi(buffer);

                    // Input verify the restriction
                    if(*p_player_attacking == 1 || *p_player_attacking == 2)
                        valid = true;
                    // Input didn't verify the restriction
                    else
                        printf("[System] Choose '1' or '2'. Try again!\n\n");
                }
                // Input isn't a number
                else
                    printf("[System] Insert a number!\n\n");

            }while(!valid);

            //Normalize. Internally, players 1 and 2 are 0 and 1, respectively.
            *p_player_attacking -= 1;

            printf("[System] Configuring...\n");
            break;
        }

        case CONFIRM_SETUP_IO:
        {
            char buffer[BUFFERSIZE];

            int map_size = va_arg(args, int);
            int* p_nr_per_piece = va_arg(args, int(*));
            int first_player_attacking = va_arg(args, int);
            bool* p_confirmed = va_arg(args, bool*);
            int text_option = va_arg(args, int);

            system("clear");
            printf("[System] The size of the maps will be equal to %d.\n", map_size);
            printf("[System] Both players will have %d pieces of type I, %d of type P, %d of type T, %d of type X and %d of type Z.\n", p_nr_per_piece[0], p_nr_per_piece[1], p_nr_per_piece[2], p_nr_per_piece[3], p_nr_per_piece[4]);
            printf("[System] The first player attacking is the player %d.\n", first_player_attacking + 1);

            bool valid = false;
            do {
                // Could read input, properly
                do {
                    printf("[System] Do you confirm it? Write yes, if you do or write 'no' to ");
                    switch(text_option) {
                        case false: printf("choose another.\n[Player] "); break;
                        case true: printf("generate another.\n[Player] "); break;
                    }
                } while(!readInput(buffer));

                // Verify input
                if(strcmp(buffer, "yes") == 0 || strcmp(buffer, "y") == 0) {
                    *p_confirmed = true;
                    valid = true;
                }
                else if(strcmp(buffer, "no") == 0 || strcmp(buffer, "n") == 0) {
                    *p_confirmed = false;
                    valid = true;
                }
                // Case input didn't match options
                else
                    printf("[System] Invalid option! Try again.\n\n");
            } while(!valid);
            system("clear");
            break;
        }

        case ATTACK_COORDINATES_IO:
        {
            char buffer[BUFFERSIZE];

            int id_player = va_arg(args, int);
            // Normalize the id of player to 1 or 2
            id_player++;

            int* p_x = va_arg(args, int*);
            int* p_y = va_arg(args, int*);


            bool valid = false;
            do {
                // Could read input, properly
                do
                    printf("[System] Introduce the coordinates x and the coordinate y of the attack, within a space between.\n[Player%d] ", id_player);
                while(!readInput(buffer));

                // There are two tokens and they are numbers
                char* str = strtok(buffer, " ");
                if(str != NULL && canConvertStringToInt(str)) {
                    *p_x = atoi(str);
                    str = strtok(NULL, " ");

                    if(str != NULL && canConvertStringToInt(str)) {
                        *p_y = atoi(str);
                        valid = true;

                        // More than 2 tokens introduced
                        str = strtok(NULL, " ");
                        if(str != NULL)
                            valid = false;
                    }
                }

                if(!valid)
                    // Input not two numbers, separated by empty spaces.
                    printf("[System] You must introduce two numbers.\n");

            } while(!valid);

            // Normalize. Internally, the coordinates of the map go from 0 to (game->size - 1).
            *p_x -= 1;
            *p_y -= 1;

            break;
        }

        case ATTACK_RESULT_IO:
        {
            int attack_result = va_arg(args, int);

            switch(attack_result) {
                case -1: puts("[System] Attacked outside the map!"); break;
                case  0: puts("[System] MISS!!!"); break;
                case  1: puts("[System] HITTED A PIECE OF TYPE I!!!"); break;
                case  2: puts("[System] HITTED A PIECE OF TYPE P!!!"); break;
                case  3: puts("[System] HITTED A PIECE OF TYPE T!!!"); break;
                case  4: puts("[System] HITTED A PIECE OF TYPE X!!!"); break;
                case  5: puts("[System] HITTED A PIECE OF TYPE Z!!!"); break;
                case  6: puts("[System] Attacked an piece already hitted!!!"); break;
            }
            break;
        }

        case READ_PIECE_IO:
        {
            char buffer[BUFFERSIZE];

            int id_player = va_arg(args, int);
            // Normalize the id of player.
            id_player++;

            int nr_piece = va_arg(args, int);
            int type = va_arg(args, int);

            int* p_x = va_arg(args, int*);
            int* p_y = va_arg(args, int*);
            int* p_r = va_arg(args, int*);

            bool valid = false;
            printf("[System] Player%d adding piece number %d of type %c.\n", id_player, nr_piece, type);

            // Read the coordinates of the piece
            do {

              // Could read input, properly
              do
              printf("[System] Introduce the coordinates: \n[Player%d] ", id_player);
              while(!readInput(buffer));

              // There are two tokens and they are numbers
              char* str = strtok(buffer, " ");
              if(str != NULL && canConvertStringToInt(str)) {
                *p_x = atoi(str);
                str = strtok(NULL, " ");
                if(str != NULL && canConvertStringToInt(str)) {
                  *p_y = atoi(str);
                  valid = true;

                  // More than 2 tokens introduced
                  str = strtok(NULL, " ");
                  if(str != NULL)
                  valid = false;
                }
              }
              // Input not two numbers, separated by empty spaces.
              if(!valid)
              printf("[System] You must introduce two number, separated by empty spaces between them.\n\n");

            } while(!valid);

            // Read the degrees of the rotation
            valid = false;
            do {

              // Could read input, properly
              do
              printf("[System] Introduce the degree of the rotation:\n[Player%d] ", id_player);
              while(!readInput(buffer));

              // Input is a number
              if(canConvertStringToInt(buffer)) {
                *p_r = atoi(buffer);

                // It's a valid degree
                if(*p_r == 0 || *p_r == 90 || *p_r == 180 || *p_r == 270 || *p_r == 360)
                valid = true;
                else
                printf("[System] Invalid degree. Insere a degree between 0 and 360, multiple of 90!\n");
              }
              // Input isn't a number
              else
              printf("[System] The degree of the rotation must be a number! Try again.\n");

            } while(!valid);

            /*
            Set 360 to 0, since is the same rotation, in the terms how the game is displayed.
            If latter the rotation 360 has some sense, that is, is different from not rotation at all, just need to remove this line.
            */
            if(*p_r == 360) *p_r = 0;


            //Normalize. Internally, the coordinates of the map go from 0 to (game->size - 1).
            *p_x -= 1;
            *p_y -= 1;

            break;
        }

        // Prints the result of adding the piece
        case RESULT_ADDING_PIECE_IO:
        {
            int result_adding_piece = va_arg(args, int);
            switch(result_adding_piece) {
                case 0:  puts("[System] Piece added.\n"); break;
                case 1:  puts("[System] Cannot add piece: piece must be inside the map!\n"); break;
                case 2:  puts("[System] Cannot add piece: piece sobreposition!\n"); break;
            }
            break;
        }

        // Print the map of pieces. '.' for the see, 'B' for the pieces and 'X' for a piece destructed.
        case PIECES_MAP_IO:
        {
            char buffer[BUFFERSIZE];

            Player* player = va_arg(args, Player*);

            int id_player = va_arg(args, int);
            // Normalize the id.
            id_player++;

            printf("[System] All pieces added.\n[System] Map of player %d.\n", id_player);

            short p = 0;
            for(int x = 0; x < player->map->size; x++) {
                for(int y = 0; y < player->map->size; y++) {
                    switch(getPieceStatus_Player(player, x, y)) {
                        // Player doesn't have a piece in that position
                        case 0: buffer[p++] = '.'; break;
                        // Player has a piece in that position and isn't destroyed
                        //case 1: buffer[p++] = player->map->cells[x][y]->piece->type; break;
                        case 1: buffer[p++] = getPieceType_Player(player, x, y); break;
                        // Player has a piece in that position and is destroyed
                        case 2: buffer[p++] = 'X'; break;
                    }
                    buffer[p++] = ' ';
                }
                buffer[--p] = '\0';
                puts(buffer);
                p = 0;
            }

            printf("[System] Press enter...\n[Player%d] ", id_player);
            while(!readInput(buffer));
            system("clear");
            break;
        }

        case SHOTS_MAP_IO:
        {
            char buffer[BUFFERSIZE];

            Player* player = va_arg(args, Player*);

            int id_player = va_arg(args, int);
            // Normalize the id.
            id_player++;

            printf("[System] Attack map of the player %d.\n", id_player);

            short p = 0;
            for(int x = 0; x < player->map->size; x++) {
                for(int y = 0; y < player->map->size; y++) {
                    switch(getShotStatus_Player(player, x, y)) {
                        // Case of no shot
                        case 0: buffer[p++] = '.'; break;
                        // Case of a missed shot
                        case 1: buffer[p++] = 'M'; break;
                        // Case of a shot on a piece of type I
                        case 2: buffer[p++] = 'I'; break;
                        // Case of a shot on a piece of type P
                        case 3: buffer[p++] = 'P'; break;
                        // Case of a shot on a piece of type T
                        case 4: buffer[p++] = 'T'; break;
                        // Case of a shot on a piece of type X
                        case 5: buffer[p++] = 'X'; break;
                        // Case of a shot on a piece of type Z
                        case 6: buffer[p++] = 'Z'; break;
                    }
                    buffer[p++] = ' ';
                }

                buffer[--p] = '\0';
                puts(buffer);
                p = 0;
            }
            puts("");
            break;
        }

        case GAME_OVER_IO:
        {
             // Id of the player that won
            int id_player = va_arg(args, int);
            // Normalize the id of player.
            id_player++;

            printf("[System] Player%d won!\n\n", id_player);
            break;
        }

        case PLAY_AGAIN_IO:
        {
            char buffer[BUFFERSIZE];

            // Pointer to the address where it's gonna be written the result of playing again.
            bool* play_again = va_arg(args, bool*);

            puts("[System] Thank you for playing!");

            bool valid = false;
            do {
                // Could read the input, properly
                do
                    printf("[System] Wanna play again? Write 'again' for playing again, or 'quit' to leave.\n[Player] ");
                while(!readInput(buffer));

                // Input verify the restriction
                if(strcmp(buffer, "again") == 0 || strcmp(buffer, "a") == 0) {
                    valid = true;
                    *play_again = true;
                    system("clear");
                }
                else if(strcmp(buffer, "quit") == 0 || strcmp(buffer, "q") == 0) {
                    valid = true;
                    *play_again = false;
                    puts("[System] Hope you had fun!");
                }
                else
                    puts("[System] Unknown command. Try again!\n");
            } while(!valid);

            break;
        }

        // Invalid identifier
        default:
        {
            prompt_IO(ERROR_IO, "io.c, prompt_IO(): invalid identifier");
            break;
        }
    }

    va_end(args);
}
