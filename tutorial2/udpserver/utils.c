#include <stdio.h>
#include <stdlib.h>

void check(int arg, char *error_msg){
    if (arg < 0){
        fprintf(stderr, "%s\n", error_msg);
        exit(EXIT_FAILURE);
    }    
}