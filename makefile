
CFLAGS=-g -o teri
SOURCE_FILES=teri.c bitboards.c init.c hashkeys.c board.c data.c attack.c io.c visual.c movegen.c

all:
	gcc $(SOURCE_FILES) $(CFLAGS) 
