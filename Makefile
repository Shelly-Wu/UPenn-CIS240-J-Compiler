all: jc

jc: jc.c hash.o token.o tokenizer.o
	clang -g jc.c hash.o token.o tokenizer.o -o jc

token.o: token.c token.h
	clang -c token.c
	
tokenizer.o: tokenizer.c tokenizer.h
	clang -c tokenizer.c

hash.o: hash.c hash.h
	clang -c hash.c

clean:
	rm -rf *.o

clobber: clean
	rm -rf jc
