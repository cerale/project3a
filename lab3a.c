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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Global Variables
int mount_fd;
int superblock_offset = 1024;
struct ext2_super_block thuper;
__uint32_t block_size;
__uint32_t inode_size;
struct ext2_inode inode;
struct ext2_group_desc group;
struct ext2_dir_entry dir;

void print_superblock_summary() {
    if(pread(mount_fd, &thuper, sizeof(struct ext2_super_block), superblock_offset) < 0){
        fprintf(stderr, "Unable to pread.\nError message: %s\nError number: %d\n", strerror(errno), errno);
        exit(2);
    }
    block_size = EXT2_MIN_BLOCK_SIZE << thuper.s_log_block_size;
    inode_size = thuper.s_inode_size;
    fprintf(stdout, "SUPERBLOCK,%u,%u,%u,%u,%u,%u,%u\n", thuper.s_blocks_count, thuper.s_inodes_count, block_size, inode_size, thuper.s_blocks_per_group, thuper.s_inodes_per_group, thuper.s_first_ino);
    return;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect usage of the program. Please use the program in the following format: ./lab3a FILE_SYSTEM_IMAGE\n");
        exit(1);
    }
    
    mount_fd = open(argv[1], O_RDONLY);
    if (mount_fd < 0) {
        fprintf(stderr, "Unable to mount file system.\nError message: %s\nError number: %d\n", strerror(errno), errno);
        exit(2);
    }

    print_superblock_summary();
    exit(0); //Successful exit
}
