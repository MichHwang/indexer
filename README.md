# indexer

About Inverted Indexer

Input: a file or directory (of files and/or sub-directories)
Output: a sorted JSON file describing an inverted index (maps keywords to their files) with recorded frequencies of a keyword in each file

-----------------------------------------------------------------------

Overview

dtraversal --> filereader --> tokenizer --> hashtable

- dtraversal: 
	- recieves command-line arguments: ./index outfile.txt directory
	- recursively traverses directory structure, passing file paths to functions in filereader
		- identifies the single parameter as a file or directory
		- calls itself if a directory is encountered in order to traverse files or directories that reside inside
		- otherwise, calls void readfile (char * file, char * name)
	- calls function to print inverted index into specified output file

- readfile: accepts absolute path of a file, opens and loads contents to be tokenized by tokenizer

- tokenizer: tokenizes file input passed from readfile
	- identifies alphanumeric tokens
	- calls function to insert tokens into a hashtable

- hashtable: 
	- contains structs node (to create word linked list) and file (to create file linked list)
	- each word node points to a file list of file nodes
	- file nodes record number of occurrences of that word in their file and file's absolute path
	- contains function to print inverted index

-----------------------------------------------------------------------

Testing Requirements / Test Plan

- recursive directory traversal: 
	- visit all files
	- get accurate paths/file names

- tokenizer:
	- keywords sent to lowercase
	- accurate parsing (only alphanumeric keywords accepted - tokens start with a letter, whitespace & punctuation not included)

- hash table / inverted index organization:
	- accurate number of occurrences of a keyword for each file
	- file list ordered by descending number of occurrences for a keyword
	- for same number of occurrences for a keyword, file names (not paths) listed alphanumerically
	- keywords listed alphanumerically

- special cases:
	- files with same name (but in different directories)

-----------------------------------------------------------------------

Test 1: ./index outfile.txt miscellaneous

Expected Output:

{"list" : [
	"a" : [
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/movie.txt" : 2},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/music.txt" : 2},
	]},
	"and" : [
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/movies/music.txt" : 3},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/movie.txt" : 1},
	]},
	"cinderella" : [
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/movie.txt" : 1},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/movies/music.txt" : 1},
	]},
	"courage" : [
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/movie.txt" : 1},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/music.txt" : 1},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/movies/music.txt" : 1},
	]},
	"dream" : [
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/music.txt" : 3},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/movie.txt" : 1},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/movies/music.txt" : 1},
	]},
	"ella" : [
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/movie.txt" : 1},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/movies/music.txt" : 1},
	]},
	"kindness" : [
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/movies/music.txt" : 2},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/movie.txt" : 1},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/music.txt" : 1},
	]},
	"me" : [
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/movie.txt" : 1},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/music.txt" : 1},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/movies/music.txt" : 1},
	]},
	"soundtrack" : [
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/movie.txt" : 1},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/movies/music.txt" : 1},
	]},
	"strong" : [
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/movies/music.txt" : 2},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/music.txt" : 2},
		{"/.autofs/ilab/ilab_users/mh812/pa4/pa4/miscellaneous/music/movie.txt" : 1},
	]},
]}

-----------------------------------------------------------------------

Test 2: ./index outfile.txt directory
