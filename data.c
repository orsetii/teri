#include "defs.h"

// Char for each piece, with a '.' representing EMPTY.
char PceChar[] = ".PNBRQKpnbrqk";
// Char for which side, with '-' representing BOTH.
char SideChar[] = "wb-";
// Char for each rank, 1-8.
char RankChar[] = "12345678";
// Char for each file, a-h.
char FileChar[] = "abcdefgh";

// Big Pieces are non-pawn pieces.
//		     EMPTY  wP     wN    wB    wR    wQ    wK    bP     bN    bB    bR    bQ    bK
int PieceBig[13] = { FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE };

// Major Pieces are Rooks and Queens and Kings
int PieceMaj[13] = { FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE };

// Minor Pieces are Bishops and Knights
int PieceMin[13] = { FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE };

// PieceVal is the value of each Piece.
// A Pawn is usually worth 1 point, a bishop/knight 3 points, a rook 5 points and a queen 9 points.
// We are using values as a 1/100th of a pawn. Therefore, our usually 3 points is 325.
int PieceVal[13] = { 0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000 };

int PieceCol[13] = { BOTH, 
	WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 
	BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK };


int PieceKnight[13] = { FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE };

int PieceKing[13] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE };

int PieceRookQueen[13] = { FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE };

int PieceBishopQueen[13] = { FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE };

