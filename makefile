
CFLAGS=-g -o teri
SOURCE_FILES=teri.c bitboards.c init.c hashkeys.c board.c data.c

all:
	gcc $(SOURCE_FILES) $(CFLAGS) 
