#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "filereader.h"
#include "tokenizer.h"
#include "hashtable.h"
#include "dtraversal.h"

/* 
 * Creates new, lowercase token identified from specified start and
 * tklen index. Sends token back to tokenize() function.
 */
char * reset(char * string, int start, int tklen) {
	char * token;
	int i;

	token = (char *) malloc(tklen + 1);
	if (token == NULL) {
		printf("Error: Tokenizer Malloc Failure\n");
		exit(0);
	}
	strncpy(token, string+start, tklen);
	token[tklen] = '\0';

	for (i = 0; i < tklen; i++) {
		token[i] = tolower(token[i]);
	}
	return token;
}


/*
 * This function will take finput as parameter and traverse through, 
 * sending each token encountered into the hashtable function to be 
 * inserted. Tokens are alphanumeric characters distinguished start
 * to end by any character that is not alphanumeric.
 */
void tokenize(char * finput, char *fname, char * name) {

	char * string, *token;
	char c;
	int size, i, start, tklen, valid;
	/* variable valid to ensure that token begins with a letter */

	string = finput;
	start = tklen = valid = 0;
	size = strlen(string);
	if (size == 0) {
		printf("Error: Invalid String\n");
		exit(0);
	}

	/* printf("finput: %s\n", finput); */

	for (i = 0; i < size; i++) {
		c = string[i];
		/* current char is delimiter */
		if (!isalnum(c)) {
			start = i + 1;
			continue;
		} else if (isdigit(c) && valid != 1) {
			start = i + 1;
			continue;
		}
		valid = 1;
		/* check at next char end of finput */
		if (i + 1 < size) {
			if (!isalnum(string[i+1])) {
				tklen = i - start + 1;
				token = reset(string, start, tklen);
				insertWord(fname, token, name); /* hashtable.c */
				start = i + 1; /* move pointer along */
				valid = 0; /* reset valid */
			}
		/* end of finput reached */
		} else {
			tklen = i - start + 1;
			token = reset(string, start, tklen);
			valid = 0; /* reset valid */
			insertWord(fname, token, name); /* hashtable.c */
		}

	}



}