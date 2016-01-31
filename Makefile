index: dtraversal.o filereader.o tokenizer.o hashtable.o
	gcc -o index dtraversal.o filereader.o tokenizer.o hashtable.o
dtraversal.o: dtraversal.c dtraversal.h
	gcc -c dtraversal.c 
filereader.o: filereader.c filereader.h
	gcc -c filereader.c 
tokenizer.o: tokenizer.c tokenizer.h
	gcc -c tokenizer.c 
hashtable.o: hashtable.c hashtable.h
	gcc -c hashtable.c 
clean:
	rm -f index dtraversal.o filereader.o tokenizer.o hashtable.o