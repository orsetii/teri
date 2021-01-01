#include "defs.h"


#include "defs.h"
#define FEN1 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define FEN2 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN4 "rnbqkbnr/2p3p1/p4p2/1p1pp2p/1PP1P1P1/B4P1P/P2P4/RN1QKBNR b KQkq - 0 7"
#define FEN_QUEEN_1v1 "8/3q1p2/8/5P2/4Q3/8/8/8 w - - 0 2 "
#define PAWNMOVESW "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define PAWNMOVESB "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1"
#define KNIGHTSKINGS "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"
#define ROOKS "6k1/8/5r2/8/1nR5/5N2/8/6K1 b - - 0 1"
#define QUEENS "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 w - - 0 1 "
#define BISHOPS "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 b - - 0 1 "
#define CASTLE1 "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1"
#define CASTLE2 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
// TODO move these into test file or something



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


	parseFen(CASTLE2, board);
	printBoard(board);

	S_MOVELIST list[1];

	GenerateAllMoves(board, list);

	PrintMoveList(list);

	//assert(CheckBoard(board));



	return 0;
}
