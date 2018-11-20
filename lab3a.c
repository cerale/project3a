/*
NAME: Alejandra Cervantes,Ryan Miyahara
EMAIL: alecer@ucla.edu,rmiyahara144@gmail.com
ID: 104844623,804585999
*/
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

//Global Variables
bool debug = false;

void debug_print(int mes) {
    switch(mes) {
        case 0:
            printf("Debug mode activated!\n");
            break;
        default:
            printf("You shouldn't get here!\n");
    }
    return;
}

int main(int argc, char** argv) {
    //Variable setup section
    struct option flags [] = { //Sets up the 2 optional flags
        {"debug", no_argument, NULL, 'd'}
    }; //Option data structure referenced here: https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
    int curr_param; //Contains the parameter that is currently being analyzed

    //Set params
    while ((curr_param = getopt_long(argc, argv, "p:s:l:d", flags, NULL)) != -1) {
        switch(curr_param) {
            case 'd':
                debug = true;
                break;
            default:
                fprintf(stderr, "Incorrect usage, please use this program in the following format: ./lab3a\n");
                exit(1);
        }
    }
    if (debug) debug_print(0);
}