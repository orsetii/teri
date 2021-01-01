#include "defs.h"


// In the u64 interpretation of the board (bitBoard), we can easily define a bit at a position by taking for example, we want to define square A2 as 1. We know A2 is the 8th piece, so we can do 1 >> 8, and we have set A2 to 1.



#define FEN1 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define FEN2 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN4 "rnbqkbnr/2p3p1/p4p2/1p1pp2p/1PP1P1P1/B4P1P/P2P4/RN1QKBNR b KQkq - 0 7"
#define FEN_QUEEN_1v1 "8/3q1p2/8/5P2/4Q3/8/8/8 w - - 0 2 "



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


void test_FENS(S_BOARD *board) {


	parseFen(FEN1, board);
	printBoard(board);

	parseFen(FEN2, board);
	printBoard(board);

	parseFen(FEN3, board);
	printBoard(board);

}

int main(int argc, char** argv) {

	teri_init();

	S_BOARD board[1];


	parseFen(FEN_QUEEN_1v1 , board);
	printBoard(board);

	printf("\n\nWhite Attacking:\n");
	ShowSqAtBySide(WHITE,board);
	
	printf("\n\nBlack Attacking:\n");
	ShowSqAtBySide(BLACK,board);

	assert(CheckBoard(board));



	return 0;
}
