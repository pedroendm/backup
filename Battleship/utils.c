#include "utils.h"

#include "io.h"

char getType_Utils(int type)
{
    switch(type) {
        case 0: return 'I'; 
        case 1: return 'P'; 
        case 2: return 'T'; 
        case 3: return 'X'; 
        case 4: return 'Z';
        default: prompt_IO(ERROR_IO, "utils.c, getType_Utils(): invalid type"); break;
    }
    
    // unreachable statement (Since, if it gets to the default case, the execution is aborted). Just to shutdown warning.
    return 0;
}

int abs(int n) {
    if(n < 0) return -n;
    return n;
}