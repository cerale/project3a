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
#include <time.h>

//Global Variables
int mount_fd;
int superblock_offset = 1024;
struct ext2_super_block thuper;
__uint32_t block_count;
__uint32_t block_size;
__uint32_t inode_count;
__uint32_t inode_size;
struct ext2_group_desc groupie;
struct ext2_inode inode;
struct ext2_dir_entry dir;

void print_error_and_exit(char *msg){
  fprintf(stderr, "ERROR: %s\nErrno reports: %s\n Error number: %d", msg, strerror(errno), errno);
  exit(2);
}

char * format_time(){
  
  return "TIME";
}
void superblock_summary() {
    if (pread(mount_fd, &thuper, sizeof(struct ext2_super_block), superblock_offset) < 0)
      print_error_and_exit("Unable to pread for superblock.");

    block_count = thuper.s_blocks_count;
    inode_count = thuper.s_inodes_count;
    block_size = EXT2_MIN_BLOCK_SIZE << thuper.s_log_block_size;
    inode_size = thuper.s_inode_size;
    fprintf(stdout, "SUPERBLOCK,%u,%u,%u,%u,%u,%u,%u\n", block_count, inode_count, block_size, inode_size, thuper.s_blocks_per_group, thuper.s_inodes_per_group, thuper.s_first_ino);
    return;
}

void group_summary() {
    if (pread(mount_fd, &groupie, sizeof(struct ext2_group_desc), superblock_offset + sizeof(struct ext2_super_block)) < 0)
      print_error_and_exit("Unable to pread for group."); 

    fprintf(stdout, "GROUP,0,%u,%u,%u,%u,%u,%u,%u\n", block_count, inode_count, groupie.bg_free_blocks_count, groupie.bg_free_inodes_count, groupie.bg_block_bitmap, groupie.bg_inode_bitmap, groupie.bg_inode_table);
    return;
}

void free_check(__uint32_t bitmap, __uint32_t count, char xdlmao) {
    __uint32_t i;
    for (i = 0; i < count; i++) {
        unsigned char offset = (i >> 3) & 255;
        unsigned char buffer;
        if (pread(mount_fd, &buffer, sizeof(unsigned char), bitmap * block_size + offset) < 0)
            print_error_and_exit("Unable to pread for free check.");

        bool not_free = ((buffer >> (i & 7)) & 1);
        if (!(not_free))
            fprintf(stdout, "%cFREE,%d\n", xdlmao, i + 1);
    }
    return;
}

void inode_summary(__uint32_t count){
    __uint32_t i;
    for (i = 0; i < count; i++){
    unsigned char offset = (i >> 3) & 255;
    unsigned char buffer;
    if (pread(mount_fd, &buffer, sizeof(unsigned char), groupie.bg_inode_bitmap * block_size + offset) < 0)
      print_error_and_exit("Unable to pread inode summary."); 

    bool not_free = ((buffer >> (i & 7)) & 1); 
    if (!(not_free))
      (void) i; 

    if (pread(mount_fd, &inode, inode_size, 1024 + (groupie.bg_inode_table - 1) * block_size 
	      + i * sizeof(struct ext2_inode)) < 0)
      print_error_and_exit("Unable to pread inode summary 2."); 
    
    char file_type = '?'; 
    if (inode.i_mode == 0 || inode.i_links_count == 0) continue; 
    if ((inode.i_mode & 0xF000) == 0xA000) file_type = 's';
    else if ((inode.i_mode & 0xF000) == 0x8000) file_type = 'f';
    else if ((inode.i_mode & 0xF000) == 0x4000){ 
      file_type = 'd';
      // Do directory stuff here
    }

    fprintf(stdout, "INODE,%d,%c,%o,%u,%u,%u,%s,%s,%s,%d,%d", 
	    i+1, file_type, (inode.i_mode & 0xFFF), inode.i_uid, inode.i_gid, 
	    inode.i_links_count, format_time(inode.i_ctime), format_time(inode.i_mtime),
	    format_time(inode.i_atime), inode.i_size, inode.i_blocks);
    
    if (!(file_type == 's' && inode.i_size < 60)) {
        __uint32_t j;
        for (j = 0; j < 15; j++)
            fprintf(stdout, ",%u", inode.i_block[j]);
    }
    else fprintf(stdout, ",%u", inode.i_block[0]);
    fprintf(stdout, "\n");

    if (!(file_type == 's' && inode.i_size < 60)){
      // Do indirect stuff here
    }
  }
    return; 
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect usage of the program. Please use the program in the following format: ./lab3a FILE_SYSTEM_IMAGE\n");
        exit(1);
    }
    
    mount_fd = open(argv[1], O_RDONLY);
    if (mount_fd < 0)
      print_error_and_exit("Unable to mount file system.");
      
    superblock_summary();
    group_summary();
    free_check(groupie.bg_block_bitmap, block_count, 'B');
    free_check(groupie.bg_inode_bitmap, inode_count, 'I');
    inode_summary(inode_count);
    exit(0); //Successful exit
}
