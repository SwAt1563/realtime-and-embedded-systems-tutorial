#!/bin/sh


gcc -g main.c -o main -lpthread -lrt
gcc -g timer.c -o timer  -lpthread -lrt
gcc -g generator.c -o generator  -lpthread -lrt
gcc -g person.c -o person -lpthread -lrt
gcc -g door.c -o door  -lpthread -lrt
gcc -g security.c -o security  -lpthread -lrt
gcc -g teller.c -o teller  -lpthread -lrt
gcc -g drawer.c -o drawer -lglut -lGLU -lGL -lm -lpthread -lrt

./main


