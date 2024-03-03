#define FUSE_USE_VERSION 30
#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

struct File {
    char name[256];
    char content[256];
};

struct Node {
    char name[256];
    struct Node* child[64];
    struct File* file[2];
    int idx;
};

typedef struct Node* NODE;
typedef struct File* F;

NODE root;

void add_dir(const char *dir_name) {
    root->idx++;
    if (root->idx >= 64) {
        printf("Insufficient space\n");
        exit(0);
    }
    
    NODE temp = (NODE)malloc(sizeof(struct Node));
    temp->idx = -1;
    strcpy(temp->name, dir_name);

    for (int i = 0; i < 64; ++i) {
        temp->child[i] = NULL;
    }
    temp->file[0] = NULL;temp->file[1] = NULL;
    root->child[root->idx] = temp;
}


int is_dir(const char *path) {
    path++; // Remove "/" in path
    NODE ptr = (NODE)malloc(sizeof(struct Node));
    for (int i = 0; i < root->idx; ++i) {
        ptr = root->child[i];
        if (strcmp(path, ptr->name) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_file(const char *filename) {
    F temp = (F)malloc(sizeof(struct File));
    strcpy(temp->name, filename);
    strcpy(temp->content, "");
    NODE ptr = (NODE)malloc(sizeof(struct Node));
    int i;
    for (i = 0; i < root->idx; ++i) {
        ptr = root->child[i];
        if (ptr->idx < 1) {
            ptr->idx++;
            ptr->file[ptr->idx] = temp;
            break;
        }
    }
    if (i >= root->idx)
        printf("Insufficient space\n");
 
}

int is_file(const char *path) {
    path++; // Eliminating "/" in the path
    NODE ptr = (NODE)malloc(sizeof(struct Node));
    for (int i = 0; i < root->idx; ++i) {
        ptr = root->child[i];
        if (ptr->idx >= 0) {
            if (strcmp(path, ptr->file[0]->name) == 0) {
                return 1;
            }
        }
        if (ptr->idx == 1) {
            if (strcmp(path, ptr->file[1]->name) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

char* get_file_directory(const char *path) {
    path++; // Eliminating "/" in the path
    NODE ptr = (NODE)malloc(sizeof(struct Node));
    for (int i = 0; i < root->idx; ++i) {
        ptr = root->child[i];
        if (ptr->idx >= 0) {
            if (strcmp(path, ptr->file[0]->name) == 0) {
                return ptr->name;
            }
        }
        if (ptr->idx == 1) {
            if (strcmp(path, ptr->file[1]->name) == 0) {
                return ptr->name;
            }
        }
    }
    free(ptr);
    return NULL;
}

void write_to_file(const char *path, const char *new_content) {
    path++; // Eliminating "/" in the path
    NODE ptr = (NODE)malloc(sizeof(struct Node));
    int i;
    for (i = 0; i < root->idx; ++i) {
        ptr = root->child[i];
        if (ptr->idx >= 0) {
            if (strcmp(path, ptr->file[0]->name) == 0)
                strcpy(ptr->file[0]->content, new_content);
        }
        if (ptr->idx == 1) {
            if (strcmp(path, ptr->file[1]->name) == 0)
                strcpy(ptr->file[1]->content, new_content);
        }
    }
    if (i == root->idx)
        printf("File not found\n");
    free(ptr);
}

static int do_getattr(const char *path, struct stat *st) {
    st->st_uid = getuid(); // the user who mounted the filesystem
    st->st_gid = getgid(); // The group is the same as the group of the user
    st->st_atime = time(NULL); // last "a"ccess is right now
    st->st_mtime = time(NULL); // last "m"odification is right now

    if (strcmp(path, "/") == 0 || is_dir(path) == 1) {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2;
    } else if (is_file(path) == 1) {
        st->st_mode = S_IFREG | 0644;
        st->st_nlink = 1;
        st->st_size = 1024;
    } else {
        return -ENOENT;
    }

    return 0;
}

static int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    filler(buffer, ".", NULL, 0); // Current Directory
    filler(buffer, "..", NULL, 0); // Parent Directory

    if (strcmp(path, "/") == 0) // If the user is trying to show the files/directories of the root directory
    {
        NODE ptr = (NODE)malloc(sizeof(struct Node));
        for (int i = 0; i < root->idx; ++i) {
            ptr = root->child[i];
            filler(buffer, ptr->name, NULL, 0);
        }
        for (int i = 0; i < root->idx; ++i) {
            ptr = root->child[i];
            for (int j = 0; j < 2; ++j) {
                if (ptr->idx >= j)
                    filler(buffer, ptr->file[j]->name, NULL, 0);
            }
        }
        free(ptr);
    }
    return 0;
}

static int do_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi) {
    char *content = NULL; // Initialize content to NULL
    path++; // Eliminating "/" in the path
    NODE ptr = (NODE)malloc(sizeof(struct Node));
    for (int i = 0; i < root->idx; ++i) {
        ptr = root->child[i];
        for (int j = 0; j < 2; ++j) {
            if (ptr->idx >= j) {
                if (strcmp(path, ptr->file[j]->name) == 0) {
                    content = ptr->file[j]->content;
                    break;
                }
            }
        }
    }
    free(ptr);

    if (content == NULL)
        return -1; // File not found

    size_t content_length = strlen(content);
    if (offset < content_length) {
        if (offset + size > content_length) {
            size = content_length - offset;
        }
        memcpy(buffer, content + offset, size);
        return size;
    } else {
        return 0; // Offset is beyond the end of the file
    }
}

static int do_mkdir(const char *path, mode_t mode) {
    path++;
    printf("Creating directory: %s\n", path);
    add_dir(path);

    return 0;
}

static int do_mknod(const char *path, mode_t mode, dev_t rdev) {
    path++;
    add_file(path);

    return 0;
}

static int do_write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info) {
    write_to_file(path, buffer);

    return size;
}

static struct fuse_operations operations = {
    .getattr = do_getattr,
    .readdir = do_readdir,
    .read = do_read,
    .mkdir = do_mkdir,
    .mknod = do_mknod,
    .write = do_write,
};

int main(int argc, char *argv[]) {
    root = (NODE)malloc(sizeof(struct Node));
    root->idx = -1;
    return fuse_main(argc, argv, &operations, NULL);
}

