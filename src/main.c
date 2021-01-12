#include <stdio.h>
#include "bwt.h"
#include <assert.h>

int main (int argc, const char *argv[]){
    assert(argc == 2);
    printf("Transformed string is: %s\n", createBwt(argv[1]));
}