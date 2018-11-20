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
#include "ext2_fs.h"
#include <unistd.h>

//Global Variables
int mount_fd;

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect usage of the program. Please use the program in the following format: ./lab3a FILE_SYSTEM_IMAGE\n");
        exit(1);
    }
    
    mount_fd = read(argv[1], O_RDONLY);
    if (mount_fd < 0) {
        fprintf(stderr, "Unable to mount file system.\nError message: %s\nError number: %d\n", strerror(errno), errno);
        exit(2);
    }

    exit(0); //Successful exit
}
