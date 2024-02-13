#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>//posix api
#include<time.h>
#include<fuse.h>
/*header file for the FUSE library. Common FUSE functions include getattr (get file attributes), readdir (read directory contents), open (open a file), read (read data from a file), write (write data to a file), and more.*/

#define block_size 1024

struct Superblock{
	int total_blocks;
	int total_inodes;
}spblock;

struct Inode {
	int inodeNumber;
	int blocks;
	int size;
}inode;

struct Node {
	char name[256];
	struct Inode* inode;
	struct Node* children;
	struct Node* next;
	int is_dir;
	mode_t permissions;
	uid_t user_id;
	gid_t group_id;
	time_t a_time;
    	time_t m_time;
    	time_t c_time;
    	time_t b_time;
};

void initialize_superblock(){
	spblock.total_blocks = 0;
	spblock.total_inodes = 0;
}
