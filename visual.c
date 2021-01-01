#include "defs.h"



void ShowSqAtBySide(const int side, const S_BOARD *pos) {
		
	int rank = 0;
	int file = 0;
	int sq = 0;
	
	printf("\n\nSquares attacked by:%c\n",SideChar[side]);
	for(rank = RANK_8; rank >= RANK_1; --rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = FR2SQ(file,rank);
			if(SqAttacked(sq,side,pos)==TRUE) {
				printf("X");
			} else {
				printf("-");
			}
				
		}
		printf("\n");
	}  
    printf("\n\n");

}
