#include <stdio.h>
#include "bwt.h"

int main (int argc, const char *argv[]){
    const char *str = "banana";
    const char *ret = createBwt(str);
    puts(ret);
}