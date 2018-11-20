/*
NAME: Alejandra Cervantes,Ryan Miyahara
EMAIL: alecer@ucla.edu,rmiyahara144@gmail.com
ID: 104844623,804585999
*/
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//Global Variables

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect usage of the program. Please use the program in the following format: ./lab3a FILE_SYSTEM_IMAGE\n");
        exit(1);
    }
    if (debug) debug_print(0);

    exit(0); //Successful exit
}
