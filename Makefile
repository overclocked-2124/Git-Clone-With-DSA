CC = gcc
CFLAGS = -Wall -g

all: git_dsa

git_dsa: src/main.o src/git_dsa.o
	$(CC) $(CFLAGS) -o git_dsa src/main.o src/git_dsa.o

src/main.o: src/main.c src/git_dsa.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/git_dsa.o: src/git_dsa.c src/git_dsa.h
	$(CC) $(CFLAGS) -c src/git_dsa.c -o src/git_dsa.o

clean:
	rm -f src/*.o git_dsa
