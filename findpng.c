#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>   /* for printf().  man 3 printf */
#include <stdlib.h>  /* for exit().    man 3 exit   */
#include <string.h>  /* for strcat().  man strcat   */
#include <stdbool.h>
#include <arpa/inet.h> ////remove once pnginfo is done
#include "lab_png.h" /* for is_png function */

// declare the function printPNGPath
void printPNGPath(char *current_path, bool *success_ptr);

int main(int argc, char *argv[]) {
    // ensure that the directory was passed as a command line argument
    if (argc == 1) {
        fprintf(stderr, "Usage: %s <directory name>\n", argv[0]);
        exit(1);
    }

    // flag to determine whether any PNGs were found 
    bool success = false;
    bool *success_ptr = &success;

    // print out the relative paths of all of the PNG files in the passed in directory 
    printPNGPath(argv[1], success_ptr);

    if (success == false) {
        printf("%s\n", "findpng: No PNG file found");
    }

    return 0;
}

// printPNGPath will print out the relative path name of the PNGs in currentPath 
void printPNGPath(char *current_path, bool *success_ptr) {
    DIR *p_dir;
    struct dirent *p_dirent;
    char str[64];

    if ((p_dir = opendir(current_path)) == NULL) {
        sprintf(str, "opendir(%s)", current_path);
        perror(str);
        exit(2);
    }

    while ((p_dirent = readdir(p_dir)) != NULL) {
        char str_path[256 + sizeof(current_path)];
        struct stat buf;
        U32 sig_buf[2];

        // use entire relative pathname using string concatination 
        sprintf(str_path, "%s/%s", current_path, p_dirent->d_name);
        if (lstat(str_path, &buf) < 0) {
            perror("lstat error");
            continue;
        }  

        // if the file type is a regular file... 
        if (S_ISREG(buf.st_mode))  { 
            // check if the file is a PNG 
            FILE *f = fopen(str_path, "rb");
            // read from file to memory
            fread(sig_buf, 8, 1, f);
            
            // if the file is a PNG, print the full path
            if (is_png(sig_buf, 8)) {
                printf("%s\n", str_path);
                // set flag to know that a PNG was found
                *success_ptr = true;
            }

            fclose(f);
        }

        // if the file type is a directory...
        else if (S_ISDIR(buf.st_mode)) {
             // ensure we are not in the current working directory (.) or the parent directory (..) by comparing the relative pathname
             // if we were to recursively call on the current working or parent directories, we create an infinite loop
            if (strcmp(p_dirent->d_name, ".") != 0 && strcmp(p_dirent->d_name, "..") != 0 )
            {
                // recursively call printPNGPath, passing in the directory 
                printPNGPath(str_path, success_ptr);
            }
        } 
        
        // otherwise, the file is of an unknown type
        else {
            printf("%s\n", "Unknown File Type!");
        }
    } 

    // successfully close the directory
    if ( closedir(p_dir) != 0 ) {
        perror("closedir");
        exit(3);
    }
}