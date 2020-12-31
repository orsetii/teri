#include "stdio.h"
#include "defs.h"


const int BitTable[64] = {
  63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
  51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
  26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
  58, 20, 37, 17, 36, 8
};

int PopBit(u64 *bb) {
  u64 b = *bb ^ (*bb - 1);
  unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
  *bb &= (*bb - 1);
  return BitTable[(fold * 0x783a9b23) >> 26];
}



void printBitBoard(u64 board) {
	u64 shift = 1ULL;
	printf("Total Bits: %d\n", countBits(board));
	for (int rank = RANK_8; rank >= RANK_1; --rank) {
		for (int file = FILE_A; file <= FILE_H; ++file) {
			int sq = FR2SQ(file, rank); // 120 based
			int sq64 = SQ264(sq);

			// If the bit at the square is 1, print 'X'
			if ((shift << sq64) & board) {
				printf("X ");
			} else {
				printf("- ");
			}
		}
	printf("\n");
	}

	printf("\n");

}

int countBits(u64 board) {
	int total;
	for (total = 0; board; ++total, board &= board - 1);
	return total;
}

// deletes a given square's bit.
u64 clearBit(u64 to_pop, u64 board) {
	return board ^= (1ULL << SQ264(to_pop));	
}


/* Generate a unique blocker board, given an index (0..2^bits) and the blocker mask 
 * for the piece/square. Each index will give a unique blocker board. */
static u64 gen_blockerboard (int index, u64 blockermask) 
{
    /* Start with a blockerboard identical to the mask. */
    u64 blockerboard = blockermask;

    /* Loop through the blockermask to find the indices of all set bits. */
    int8_t bitindex = 0;
    for (int8_t i=0; i<64; i++) {
        /* Check if the i'th bit is set in the mask (and thus a potential blocker). */
        if ( blockermask & (1ULL<<i) ) {
            /* Clear the i'th bit in the blockerboard if it's clear in the index at bitindex. */
            if ( !(index & (1<<bitindex)) ) {
                blockerboard &= ~(1ULL<<i); //Clear the bit.
            }
            /* Increment the bit index in the 0-4096 index, so each bit in index will correspond 
             * to each set bit in blockermask. */
            bitindex++;
        }
    }
    return blockerboard;
}

