# Create-Own-Filesystem
We will be creating our own filesystem using fuse 

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
