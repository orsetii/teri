#include "defs.h"


#include "defs.h"
#define FEN1 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define FEN2 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN4 "rnbqkbnr/2p3p1/p4p2/1p1pp2p/1PP1P1P1/B4P1P/P2P4/RN1QKBNR b KQkq - 0 7"
#define FEN_QUEEN_1v1 "8/3q1p2/8/5P2/4Q3/8/8/8 w - - 0 2 "





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


	int move = 0;
	int from = A2; int to = H7;
	int cap = wR; int prom = bK;
	
	move = ( ( from ) | ( to << 7 ) | ( cap << 14 ) | ( prom << 20) );
			
	printf("from:%d to:%d cap:%d prom:%d\n",
		FROMSQ(move),TOSQ(move),CAPTURED(move),
		PROMOTED(move));
	
	printf("Algebraic from:%s\n",PrintSq(from));
	printf("Algebraic to:%s\n",PrintSq(to));
	printf("Algebraic move:%s\n",PrintMove(move));
	
	//assert(CheckBoard(board));



	return 0;
}
