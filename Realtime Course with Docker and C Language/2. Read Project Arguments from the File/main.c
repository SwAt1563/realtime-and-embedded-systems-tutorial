
#include "header.h"
#include "constants.h"
#include "functions.h"



int main(){

    readFile("arguments.txt");

    printf("value: %s\n", THE_LEADER_NAME);

    return 0;
}