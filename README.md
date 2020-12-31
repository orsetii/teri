# Board Representation

Board is represented by an array of integers, with a different value depending on if it is an empty, square, if it has a piece on it (a different number for what piece) or a number for the square being off the board.

moves are represented by an algebriac notation.

a-h on the x axis.

1-8 on the y axis.

Instead of representing the board as an 8x8 board, we need to know if the piece has run off the board or not.

So we make our array with a set of border squares, with set value like `OFF_BOARD` so we know its invalid square.

The board is a 10x12, as we need to account for knights moving 2 squares deep off-board. We dont need extra rows on the sides of the array as it will loop around to an invalid square on the next row, on the other side of the board.

So, the board starts at tile 21, with each row having invalid squares at *x*0 and *x*9; until we hit tile 98, of which is the 64th valid square!

![pic of board](https://i.imgur.com/JzCLfMQ.png)

# Accounting for Rules

**Fifty Moves Rule** - This the rule that if we haven't had a capture or a pawn move for 50 moves, the game is a draw. (We will need a counter variable for this) 

**3x Rep Rule** - If there is a threefold repetition, the game is ruled a draw. (We will need some kind of key, defining a unique value, to detect this)

**En Passant** - Check if any En Passant captures are avaialable (need something to track this).

**Side to Move** - Track which side to move.

**Ply** - Amount of half moves made in a game (if one side has made a move, that is one ply. If two sides have a made a move (counted as a *move*) then we are two plys deep.) (need to track this value aswell).




