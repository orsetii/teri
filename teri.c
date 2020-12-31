#include <stdio.h>
#include "defs.h"


// In the u64 interpretation of the board (bitBoard), we can easily define a bit at a position by taking for example, we want to define square A2 as 1. We know A2 is the 8th piece, so we can do 1 >> 8, and we have set A2 to 1.



int main(int argc, char** argv) {

	u64 test_bitboard = 0ULL;
	test_bitboard |= (1ULL << SQ264(A2));
	printBitBoard(test_bitboard);
	return 0;
}
