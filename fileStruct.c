#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>//posix api
#include<time.h>
#include<fcntl.h>//File control
#include<stddef.h> //Null and ofset
#include<assert.h>
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
typedef struct Node* NODE;

void initialize_superblock(){
	spblock.total_blocks = 0;
	spblock.total_inodes = 0;
}

NODE create_node(const char* name, int is_dir){
	NODE new_node = (NODE)malloc(sizeof(struct Node));
	strcpy(new_node -> name, name);
	new_node->inode = (struct Inode*)malloc(sizeof( struct Inode));
	new_node->children = NULL;
	new_node->next = NULL;
	new_node->is_dir = is_dir;
	new_node->permissions = 0777;
	new_node->user_id = getuid();
	new_node->group_id = getgid();
	new_node->a_time = new_node->m_time = new_node->c_time = new_node->b_time = time(NULL);
	return new_node;
}

void destroy_node(NODE node){
	free(node->inode);
	free(node);
}

static int mygetattr(const char *path, struct stat *stbuf){
	NODE node = NULL;
	if(node == NULL) return -ENOENT;
	stbuf->st_mode = node->permissions | (node->is_dir ? S_IFDIR : S_IFREG);
	stbuf->st_uid = node->user_id;
        stbuf->st_gid = node->group_id;
        stbuf->st_atime = node->a_time;
        stbuf->st_mtime = node->m_time;
        stbuf->st_ctime = node->c_time;
        stbuf->st_size = node->is_dir ? 0 : node->inode->size;
        stbuf->st_blocks = node->is_dir ? 0 : node->inode->blocks;
	return 0;
}

static int myreaddir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	NODE dir_node = NULL;
	if(dir_node == NULL || !dir_node->is_dir)
		return -ENOENT;
	 struct Node* child = dir_node->children;
	    while (child != NULL) {
		filler(buffer, child->name, NULL, 0);
		child = child->next;
	    }
    return 0;
}

static struct fuse_operations operations = {
	.getattr = mygetattr,
	//.readdir = myreaddir,
};
int main(int argc, char *argv[]){
	int ret = 0;
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	initialize_superblock();
	NODE root = create_node("/", 1);
	ret =  fuse_main(args.argc, args.argv, &operations);
	fuse_opt_free_args(&args);
	return ret;
}
