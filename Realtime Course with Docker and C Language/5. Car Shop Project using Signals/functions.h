


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


    char separator[] = "=";

    while(fgets(line, sizeof(line), file) != NULL){

        char *str = strtok(line, separator);
        strncpy(label, str, sizeof(label));
        str = strtok(NULL, separator);


        if (strcmp(label, "NUMBER_OF_CARS") == 0){
            NUMBER_OF_CARS = atoi(str);
        }



    }

    fclose(file);
}

#endif