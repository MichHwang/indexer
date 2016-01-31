#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filereader.h"
#include "tokenizer.h"
#include "hashtable.h"
#include "dtraversal.h"

void readfile(char * file, char * name) {
	char * finput, * filename;
	FILE * f;
	int fsize, fi;

	f = fopen(file, "r");

	if (f == NULL) {
		printf("Error: File Open Failure\n");
		return;
	}
	
	/* fseek sets file position of stream to offset; 0 if success */
	if (fseek(f, 0, SEEK_END) != 0) {
		printf("Error: fseek() Failure\n");
		return;
	}

	/* ftell returns current file position in stream; -1 if failure */
	fsize = ftell(f);
	if (fsize == -1) {
		printf("Error: ftell() Failure\n");
		return;
	}
		
	/* allocates space and initializes finput using fsize */
	finput = (char *) malloc (fsize+1);
	if (finput == NULL) {
		printf("Error: Malloc() Failure\n");
		return;
	}

	if (fseek(f, 0, SEEK_SET) != 0) {
		printf("Error: fseek() Failure\n");
		return;
	}

	/* fread reads data into stream; returns # elements read successfully */
	if (fread(finput, fsize, 1, f) == 0) {
		printf("Error: fread() Failure\n");
	}

	finput[fsize] = '\0';

	/* printf("Size of File: %d\n", fsize); */

	/* Create independent file name variable */
	fi = strlen(file);
	filename = (char *) malloc (fi + 1);
	strcpy(filename, file);
	filename[fi] = '\0';

	tokenize(finput, filename, name); /* tokenize.c */

	fclose(f);
	free(finput);
	return;
}

