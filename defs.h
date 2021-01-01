// Define guard so we don't have redefinitions or any mess.
#ifndef DEFS_H
#define DEFS_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


typedef unsigned long long u64;

// Name and version of current program
#define NAME "teri 0.1"
// Total amount of squares on the 'board'
#define BRD_SQ_NUM 120 

// Max number of moves we would expect in a game (this is 2048 half-moves).
#define MAXGAMEMOVES 2048

// The first part is the description of pieces on the board
// with numbers being the number of consecutive empty pieces in a row
// followed by either 'w' or 'b' for which side to move,
// then castling permissions
// then notation for an En Passant Square if available, if not it is simply '-'
// Then fifty move count
// Then total moves (a move being counted as both black and white have moved)
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"




// enumerated constants of pieces
// 'w' meaning white, 'b' meaning black, followed by a piece letter
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };
// enumerated constants of 'files' aka x axis positions down the board A-H
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE }; 
// enumerated constants of 'ranks' aka y axis positions (what row is it in) 1-8
enum { RANK_1, RANK_2,RANK_3,RANK_4,RANK_5,RANK_6,RANK_7,RANK_8,RANK_NONE };
// Colours of what piece
enum { WHITE, BLACK, BOTH };

// constants for all 64 valid squares.
// Also has the 'OFF_BOARD' for invalid squares.
// This provides an int value for each square.
enum {
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2, 
	A3 = 41, B3, C3, D3, E3, F3, G3, H3, 
	A4 = 51, B4, C4, D4, E4, F4, G4, H4, 
	A5 = 61, B5, C5, D5, E5, F5, G5, H5, 
	A6 = 71, B6, C6, D6, E6, F6, G6, H6, 
	A7 = 81, B7, C7, D7, E7, F7, G7, H7, 
	A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFF_BOARD = -1
};

enum { FALSE, TRUE };
 
// This defines castling permissions
// KCA means 'King Castling' and QCA means 'Queen Castling'
//
// Permissions are defined as 4 bits.
// 0 0 0 0
// Each Bit is represented as below!
//
// You will not be able to castle if rook or king is moved (i think)
// So, we can simply just NOT a castling perm in that scenario.
enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };


typedef struct {

	// The 'move' int stores all the information we need for a move.
	int move;
	int score;





} S_MOVE;










// Information needed to undo a move
typedef struct {

	// A move, stored as an integer.
	int move;
	// Castle Permission, before that move was played.
	int castlePerm;
	// En Passant Square before move was played.
	int enPas;
	// Fifty Move Count Status
	int fiftyMovCnt;
	// Position Key, to define board state in positions
	u64 positionKey;

} S_UNDO;

// S_BOARD is the structure defining the board
typedef struct {

	// Define an array of ints, each value corresponding to a square
	// The int value will define the 'state' of the square.
	int pieces[BRD_SQ_NUM];

	// The 64 defines one bit, for each (valid)square.
	// We define three of these, one for each colour - BLACK, WHITE or BOTH.
	// note that BOTH defines the pawn positions for pawns of both sides, so should always equal '~(WHITE | BLACK)' (thats C for NAND)
	u64 pawns[3];
	
	// kingSq holds the square that the kings are on, for white and black, at index 0 and 1 of this array.
	int kingSq[2];

	// side holds the current side to move
	int side;
	
	// En Passant square, if one is active. If not, this will be 'OFF_BOARD'
	int enPas;

	// The counter for fifty moves, we are counting in plys or half moves, so this will have to equal 100, not 50.
	int fiftyMovCnt;

	// The ply, how many half-moves into the current search.
	int ply;

	// In the total game so far, how many half-moves have been played.
	int hist_ply;

	int castlePerm;

	// a unique key generated for each position.
	u64 positionKey;

	// This is a key used to define how many pieces are on the board of a type. There are 12 possible pieces, as we are including the black and white variants.
	int pieceNum[13];

	// The 3 below arrays all have 3 ints, for white, black and for both.
	// Big Pieces are any piece that is NOT a pawn.
	int BigPieces[2];
	// Major Pieces are Rooks and Queens and Kings
	int MajPieces[2];
	// Minor Pieces are Bishops and Knights
	int MinPieces[2];

	// Holds the material score, for white and black.
	int material[2];

	// Now we should be able to undo the entire way back through each half move.
	S_UNDO history[MAXGAMEMOVES];
	
	// Piece List
	// 13 pieces, of which you can have at MOST 10 of, on the board (would be done via pawn promotions)
	int pieceList[13][10];

	// If we wanted to put the first White Knight on E1, we would do:
	// pieceList[wN][0] = E1;


	

} S_BOARD;

/* GAME MOVE */

/*
 * We store the move in a binary representation
 * The next four bits, store what a pawn was promoted to, if a pawn was promoted. If there wasn't a promotion,
 * these will all be set to 0.
 * The last bit will store whether the move was a castling move or not.
 *
 * 0000 0000 0000 0000 0000 0000
 *                      ^^^ ^^^^                     
 * 	         	 From Square
 *
 * The least significant 7 bits store the 'From' square.
 *
 *
 * 0000 0000 0000 0000 0--- ----
 *             ^^ ^^^^ ^
 *                 To Square
 *
 * The next least significant 7 bits store the 'To' Square
 *                
 *
 * 0000 0000 00-- ---- ---- ----
 *        ^^ ^^
 *        What Piece, if any, was captured. 
 * 	  We have 12 possible pieces, so we can store all of that in 4 bits.
 *
 * 0000 00-- ---- ---- ---- ----
 *	 ^
 *	Was the move an En Passant Capture or not?
 *
 *
 *
 *
 *
 * 0000 0--- ---- ---- ---- ----
 *  ^^^ ^
 *    If a pawn promoted, what piece did it promote to?
 *    If nothing promoted, all bits are set to 0.
 *
 *    4 bits correspond to: Bishop - Rook - Queen - Knight
 *
 * 0--- ---- ---- ---- ---- ----
 * ^
 * Was the move a castling move?
 * If so, this bit should be set.
 *
 *
 *
 *
 */

/* MACROS */

// This macro, when given the File (F) and Rank (R) number, returns the 120-array-based index.
#define FR2SQ(f, r) ( (21 + (f) ) + ( (r) * 10) )
#define SQ264(n) (board_120[n])
#define SQ120(n) (board_64[(n)])


#define POP(b) PopBit(b)
#define CNT(b) CountBits(b)
#define CLRBIT(bb, sq) ((bb) &= ClearMask[(sq)])
#define SETBIT(bb, sq) ((bb) |= SetMask[(sq)])

// The below 4 macros allow us to easily ask if a piece is of X type.
#define IsBQ(p) (PieceBishopQueen[(p)])
#define IsRQ(p) (PieceRookQueen[(p)])
#define IsKn(p) (PieceKnight[(p)])
#define IsKi(p) (PieceKing[(p)])


/* GLOBALS */

// board_120 contains square coords(kinda) for all 120 squares. All '-1' squares are invalid squares.
static const int board_120[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
	-1,  0,  1,  2,  3,  4,  5,  6,  7, -1, 
	-1,  8,  9, 10, 11, 12, 13, 14, 15, -1, 
	-1, 16, 17, 18, 19, 20, 21, 22, 23, -1, 
	-1, 24, 25, 26, 27, 28, 29, 30, 31, -1, 
	-1, 32, 33, 34, 35, 36, 37, 38, 39, -1, 
	-1, 40, 41, 42, 43, 44, 45, 46, 47, -1, 
	-1, 48, 49, 50, 51, 52, 53, 54, 55, -1, 
	-1, 56, 57, 58, 59, 60, 61, 62, 63, -1, 
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};


// board_64 contains the 64-square based array, with only the valid squares; containing ints ranging from 21-98.
static const int board_64[] = {
	21, 22, 23, 24, 25, 26, 27, 28, 
	31, 32, 33, 34, 35, 36, 37, 38, 
	41, 42, 43, 44, 45, 46, 47, 48, 
	51, 52, 53, 54, 55, 56, 57, 58, 
	61, 62, 63, 64, 65, 66, 67, 68, 
	71, 72, 73, 74, 75, 76, 77, 78, 
	81, 82, 83, 84, 85, 86, 87, 88, 
	91, 92, 93, 94, 95, 96, 97, 98,
};

extern u64 SetMask[64];
extern u64 ClearMask[64];

extern u64 PieceKeys[13][120];
extern u64 SideKey;
extern u64 CastleKeys[16];
 
extern char PceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];

extern int PieceBig[13];
// Major Pieces are Rooks and Queens
extern int PieceMaj[13];
extern int PieceMin[13];
extern int PieceVal[13];
extern int PieceCol[13];

extern int FilesBrd[BRD_SQ_NUM];
extern int RanksBrd[BRD_SQ_NUM];

// Is a piece an X? These arrays are defined to help with answering these questions.
extern int PieceKnight[13];
extern int PieceKing[13];
extern int PieceRookQueen[13];
extern int PieceBishopQueen[13];


/* FUNCTIONS */

// bitboards.c
void printBitBoard(u64 board);
int PopBit(u64 *bb);
int CountBits(u64 board);
u64 xorFR(u64 to_pop, u64 board);

// init.c
extern void teri_init();
 
// hashkeys.c
extern u64 generatePositionKey(const S_BOARD *pos);
 
// board.c
extern void resetBoard(S_BOARD *pos);
extern int parseFen(char* fen, S_BOARD *pos);
extern void printBoard(const S_BOARD *pos);
extern void UpdateListsMaterial(S_BOARD *pos);
extern int CheckBoard(const S_BOARD *pos);

// attack.c
extern int SqAttacked(const int sq, const int side, const S_BOARD *pos);

// visual.c
extern void ShowSqAtBySide(const int side, const S_BOARD *pos);



#endif
