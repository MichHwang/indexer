#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <errno.h>

#include "filereader.h"
#include "tokenizer.h"
#include "hashtable.h"
#include "dtraversal.h"

/*
 * Recursively traverses through directory and subdirectory structures. Records and appends 
 * file names to create the files' absolute paths. Sends file and path names to readfile().
 */
void recurse(char * obj, char * name) {
	struct stat buffer;
	DIR * dir;
	struct dirent * curr;
    	char path[PATH_MAX + 1];
	char * file;
	
	realpath(obj, path);

	/* get status of object */
	if (stat(obj, &buffer) != 0) {
		printf("Error: %d\n", errno);
		exit(1);
	}

	/* identify file or directory */
	if (S_ISREG (buffer.st_mode)) {
		/* this is a base case */
		/* printf("FILE: %s\n", obj); */
		readfile(path, name);
		return;
	} else if (S_ISDIR (buffer.st_mode)) {
		/* printf("\nDIRECTORY: %s\n", obj); */
		if ((dir = opendir(obj)) == NULL) {
			printf("Error: Cannot Open Directory\n");
			exit(1);
		}

		while ((curr = readdir(dir)) != NULL) {
	      	  	if (!strcmp (curr->d_name, ".") || !strcmp (curr->d_name, "..")) {
	            		continue;
			}

			file = curr->d_name;
			realpath(obj, path);
			strcat(path, "/");
			strcat(path, file);
			/* printf("\nINSIDE %s has %s\n", obj, file); */
			recurse(path, file);
		}
		/* printf("\nClosing directory\n"); */
		closedir(dir);
	} else {
		printf("Error: Must input either file or directory\n");
		exit(1);
	}

}

int main(int argc, char ** argv) {

	/* improper number of inputs */
	if (argc < 3) {
		printf("Error: Require 3 Arguments\n");
		exit(1);
	}

	/* this code assumes the output file is correct */
	initialize();
	recurse(argv[2], argv[2]);

	printf("\nOutput written to %s\n", argv[1]);
	printTable(argv[1]);
	exit(0);
}
