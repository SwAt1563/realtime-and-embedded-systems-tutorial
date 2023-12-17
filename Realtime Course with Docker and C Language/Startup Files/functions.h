#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include "header.h"
#include "constants.h"


void readFile(char* filename){
    char line[200];
    char label[50];

    FILE *file;

    file = fopen(filename, "r");

    if (file == NULL){
        perror("The file not exist\n");
        exit(-2);
    }

    #ifdef EQUAL
    char separator[] = "=";
    //#elifdef DOUBLE_DOT
    //    char separator[] = ":";
    #else
        char separator[] = ".";
    #endif

    while(fgets(line, sizeof(line), file) != NULL){

        char *str = strtok(line, separator);
        strncpy(label, str, sizeof(label));
        str = strtok(NULL, separator);


        if (strcmp(label, "RANDOM_NUMBER") == 0){
            RANDOM_NUMBER = atoi(str);
        }else if(strcmp(label, "RANDOM_PRICE") == 0){
            RANDOM_PRICE = atof(str);
        }else if(strcmp(label, "RANDOM_NAME") == 0){
            strncpy(RANDOM_NAME, str, sizeof(RANDOM_NAME));
        }



    }

    fclose(file);
}

#endif