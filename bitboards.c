#include "stdio.h"
#include "defs.h"

void printBitBoard(u64 board) {
	u64 shift = 1ULL;
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

}
