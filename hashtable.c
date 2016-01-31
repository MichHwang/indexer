#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filereader.h"
#include "tokenizer.h"
#include "hashtable.h"
#include "dtraversal.h"

struct file {
	char * fname; /* file path */
	char * name; /* file name */
	int occ;
	struct file * next;
};

struct node {
	char * word;
	struct file * fhead;
	struct node * next;
};

struct word {
	char * word;
	struct word * next;
};

struct node * table[5000]; /* word list head */
struct word * head;

void initialize() {
	int i;
	for (i = 0; i < 5000; i++) {
		table[i] = NULL;
	}
}

int hashFunction (char * word) {
	int index, size, sum;
	char c;
	
	size = index = sum = 0;
	size = strlen(word);

	for (c = 0; c < size; c++) {
		/* implicit cast - sum ASCII value of each char */
		sum += word[c];
	}

	index = sum%5000;
	return index;
}

void recordKeyword(char * word) {
	struct word * temp, * curr, * prev;
	prev = NULL;
	curr = head;

	while (curr != NULL) {
		if (strcmp(curr->word, word) == 0) {
			return;
		} else if (strcmp(curr->word, word) > 0) {
			break;
		} 
		prev = curr;
		curr = curr->next;
	}
	
	temp = (struct word *) malloc (sizeof(struct word));
	temp->word = word;
	if (prev == NULL) {
		temp->next = head;
		head = temp;
	} else {
		prev->next = temp;
		temp->next = curr;
	}
	return;
}

/* 
 * Given specified output file name, printTable() will print the contents of 
 * the hashtable into that file in JSON format.
 */
void printTable(char * outfile) {
	int i, idx;
	struct node * lptr;
	struct file * fptr;	
	FILE * out;
	struct word * curr;

	/* printf("PRINTING TO %s\n", outfile); */
	out = fopen(outfile, "w");
	fprintf(out, "{\"list\" : [\n");

	curr = head;
	while (curr != NULL) {
		idx = hashFunction(curr->word);	
		lptr = table[idx];
		while (lptr != NULL) {
			if (strcmp(curr->word, lptr->word) == 0) {
				fprintf(out, "\t\"%s\" : [\n", lptr->word);
				fptr = lptr->fhead;
				while (fptr != NULL) {
					fprintf(out, "\t\t{\"%s\" : %d},\n", fptr->fname, fptr->occ);
					fptr = fptr->next; /* get next file */
				}
				fprintf(out, "\t]},\n");
			}
			lptr = lptr->next;
		}
		curr = curr->next;
	}
	fprintf(out, "]}\n");
}

/* inserts word into the hashtable given word, file path, and name */
void insertWord (char * fname, char * word, char * nme) {
	int index;
	struct node *lptr, *ltemp;
	struct file *fptr, *ftemp, *fprev, *prev, *curr;
	char * filename, *name;

	name = (char *) malloc (strlen(nme) + 1);
	strcpy(name, nme);
	name[strlen(nme)] = '\0';

	recordKeyword(word);
	index = hashFunction(word);

	lptr = table[index];

	/* word list is empty */
	if (lptr == NULL) {
		/* allocate space and initialize new word in word list */
		ltemp = (struct node *) malloc (sizeof(struct node));
		ltemp->word = word;
		ltemp->next = NULL;

		/* allocate space and initialize new file in file list */
		ltemp->fhead = (struct file *) malloc (sizeof(struct file));
		ltemp->fhead->fname = fname;
		ltemp->fhead->name = name;
		ltemp->fhead->occ = 1;
		ltemp->fhead->next = NULL;

		/* insert first in word list */
		if (table[index] == NULL) {
			table[index] = ltemp;
		}
		return;
	}

	/* word list not empty - search for word in word list */
	while (lptr != NULL) {
		if (strcmp(lptr->word, word) == 0) {
			break;
		}
		lptr = lptr->next;
	}		

	/* word is not in list - add to front */
	if (lptr == NULL) {
		/* allocate space and initialize new word in word list */
		ltemp = (struct node *) malloc (sizeof(struct node));
		ltemp->word = word;
		ltemp->next = NULL;

		/* allocate space and initialize new file in file list */
		ltemp->fhead = (struct file *) malloc (sizeof(struct file));
		ltemp->fhead->fname = fname;
		ltemp->fhead->name = name;
		ltemp->fhead->next = NULL;
		ltemp->fhead->occ = 1;
		
		ltemp->next = table[index];
		table[index] = ltemp;

		return;
	}

	/* word already exists in word list, check if in file list */
	/* assume here that file list cannot be empty - at least one file node */
	fptr = lptr->fhead;
	fprev = NULL;

	while (fptr != NULL) {
		if (strcmp(fptr->fname, fname) == 0) {
			/* compare path names */
			fptr->occ++;

			/* file node is in front */
			if (fprev == NULL) {
				return;

			/* curr node has greater occ than prev */
			} else if (fprev->occ < fptr->occ) {
				/* printf("Removing file node from list\n"); */
				/* printf("%d < %d\n", fprev->occ, fptr->occ); */
				fprev->next = fptr->next;

				curr = lptr->fhead;
				prev = NULL;

				/* iterate through file list checking occ number */
				while (curr != NULL) {
					/* need to insert file node before curr */
					if (fptr->occ > curr->occ) {
						if (prev == NULL) {
							/* printf("Inserting at file head\n"); */
							fptr->next = lptr->fhead;
							lptr->fhead= fptr;
						} else {
							/* printf("Reinserting in middle\n"); */
							prev->next = fptr;
							fptr->next = curr;
						}
						/* printf("Inserted %s\n", word); */

						return;
					/* break at beginning of file nodes with same occ */
					} else if (fptr->occ == curr->occ) {
						break;
					}
					prev = curr;
					curr = curr->next;
				}
	
				/* organize within same occ number, alphanum by file name not path! */
				while (curr != NULL) {
					/* check if reached end of file nodes with same occ */
					if (fptr->occ != curr->occ) {
						prev->next = fptr;
						fptr->next = curr;
						/* printf("Inserted %s\n", word); */
						return;
					}
					if (strcmp(fptr->name, curr->name) <= 0) {
						/* fptr occ is the greatest in file list */
						if (prev == NULL) {
							fptr->next = lptr->fhead;
							lptr->fhead = fptr;
						} else {
							prev->next = fptr;
							fptr->next = curr;
						}
						/* printf("Inserted %s\n", word); */
						return;
					} 
					prev = curr;
					curr = curr->next;
				}

			/* curr node has equal occ as prev, but not in alphanum order */
			} else if (fprev->occ == fptr->occ && strcmp(name, fprev->name) < 0) {
				/* printf("Removing file node from list\n"); */
				fprev->next = fptr->next;

				curr = lptr->fhead;
				prev = NULL;

				/* iterate through file list checking occ number */
				while (curr != NULL) {
					/* need to insert file node before curr */
					if (fptr->occ > curr->occ) {
						if (prev == NULL) {
							fptr->next = lptr->fhead;
							lptr->fhead= fptr;
						} else {
							prev->next = fptr;
							fptr->next = curr;
						}
						return;
					/* break at beginning of file nodes with same occ */
					} else if (fptr->occ == curr->occ) {
						break;
					}
					prev = curr;
					curr = curr->next;
				}
	
				/* organize within same occ number */
				while (curr != NULL) {
					/* check if reached end of file nodes with same occ */
					if (fptr->occ != curr->occ) {
						prev->next = fptr;
						fptr->next = curr;
						return;
					}
					if (strcmp(fptr->name, curr->name) <= 0) {
						/* fptr occ is the greatest in file list */
						if (prev == NULL) {
							fptr->next = lptr->fhead;
							lptr->fhead = fptr;
						} else {
							prev->next = fptr;
							fptr->next = curr;
						}
						return;
					}
					prev = curr;
					curr = curr->next;
				}

			}
			return;
		} else {
			fprev = fptr;
			fptr = fptr->next;
		}

	}
		
	/* this is first occurrence for file, insert at end */

	/* allocates space for new file node */
	ftemp = (struct file *) malloc (sizeof(struct file));
	ftemp->fname = fname;
	ftemp->name = name;
	ftemp->occ = 1;
	ftemp->next = NULL;

	if (fprev->occ == 1 && strcmp(name, fprev->name) < 0) {

		curr = lptr->fhead;
		prev = NULL;

		/* iterate through file list checking occ number */
		while (curr != NULL) {
			/* need to insert file node before curr */
			if (ftemp->occ > curr->occ) {
				if (prev == NULL) {
					ftemp->next = lptr->fhead;
					lptr->fhead= ftemp;
				} else {
					prev->next = ftemp;
					ftemp->next = curr;
				}
				return;
			/* break at beginning of file nodes with same occ */
			} else if (ftemp->occ == curr->occ) {
				break;
			}
			prev = curr;
			curr = curr->next;
		}
	
		/* organize within same occ number */
		while (curr != NULL) {
			/* check if reached end of file nodes with same occ */
			if (ftemp->occ != curr->occ) {
				prev->next = ftemp;
				ftemp->next = curr;
				return;
			}
			if (strcmp(ftemp->name, curr->name) <= 0) {
				/* fptr occ is the greatest in file list */
				if (prev == NULL) {
					ftemp->next = lptr->fhead;
					lptr->fhead = ftemp;
				} else {
					prev->next = ftemp;
					ftemp->next = curr;
				}
				return;
			}
			prev = curr;
			curr = curr->next;
		}

	} else {
		/* appends new file node to end of file list */
		fprev->next = ftemp;
	}

	return;
}
