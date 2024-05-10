# Create-Own-Filesystem
We will be creating our own filesystem using fuse 

# PROBLEM STATEMENT
Design and implement a simple file system with support for file creation, reading, writing, deletion, and Tree Structured directory management, considering key operating system principles.

![image](https://github.com/kart2004/Create-Own-Filesystem/assets/111494403/c8609e00-ef63-41d9-9c2d-9fcb3f64d8c3)

![image](https://github.com/kart2004/Create-Own-Filesystem/assets/111494403/1118a760-45df-4588-a116-93268245bfb6)

![image](https://github.com/kart2004/Create-Own-Filesystem/assets/111494403/cfea393d-cde1-42da-8636-1bb9f203d127)




# Tools/APIs used
Here are the tools and APIs used in this code:
Standard C Library (#include<stdio.h>, #include<errno.h>, #include<stdlib.h>, #include<string.h>, #include<unistd.h>, #include<time.h>, #include<stddef.h>, #include<assert.h>):
Standard C library functions for general-purpose programming.
File Control (#include<fcntl.h>):
Fcntl header file for file control operations.
FUSE Library (#include<fuse.h>):
Header file for the FUSE library, which provides functions like getattr and readdir for creating a FUSE-based file system.
POSIX API (#include<unistd.h>):
Header file for POSIX (Portable Operating System Interface) API functions.

5. Macros (#define block_size 1024):
Definition of a macro for the block size.
6. Structures (struct Superblock, struct Inode, struct Node):
Structures used for representing the superblock, inode, and node in the filesystem.
7. fuse_main Function:
Entry point for the FUSE-based file system. It initializes the superblock, creates the root node, and starts the FUSE file system.



COMMANDS : 
Commands: Compilation command: cc -g -Wall -O2 -o fileSys fileStruct.c `pkg-config fuse --cflags --libs`
(Wall: enables all warnings || pkg-config finds the necessary compiler and linker flags for the FUSE library)
make all
To make mount: mkdir ~/mount_point
To Mount: ./fileSys -f ~/mount_point 
Command to unmount: sudo umount -f ~/mount_point

FILESYSTEM FUNCTIONS:
cd   ~/mount_point 
ls, mkdir dir0 etc,    echo less Simple >> file, less file  
Echo is a Unix/Linux command tool used for displaying lines of text or string which are passed as arguments on the command line 
Less command is a Linux utility that can be used to read the contents of a text file one page (one screen) at a time

The filesystem commands used:
getattr: This function is called when you access information about a file or directory. $ ls -l /path/to/mounted/directory/file
readdir: This function is called when you list the contents of a directory. $ ls /path/to/mounted/directory
read: This function is called when you read the contents of a file. $ cat /path/to/mounted/directory/file
mkdir: This function is called when you create a new directory. $ mkdir /path/to/mounted/directory/new_directory
mknod: This function is called when you create a new file. $ touch new_file // Not implemented to show differernce 
write: This function is called when you write to a file.  $ echo "content" > /path/to/mounted/directory/file
