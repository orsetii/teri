// Define guard so we don't have redefinitions or any mess.
#ifndef DEFS_H
#define DEFS_H

typedef unsigned long long u64;

// Name and version of current program
#define NAME "teri 0.1"
// Total amount of squares on the 'board'
#define BRD_SQ_NUM 120 

// Max number of moves we would expect in a game (this is 2048 half-moves).
#define MAXGAMEMOVES 2048


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
// Also has the 'NO_SQ' for invalid squares.
// This provides an int value for each square.
enum {
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2, 
	A3 = 41, B3, C3, D3, E3, F3, G3, H3, 
	A4 = 51, B4, C4, D4, E4, F4, G4, H4, 
	A5 = 61, B5, C5, D5, E5, F5, G5, H5, 
	A6 = 71, B6, C6, D6, E6, F6, G6, H6, 
	A7 = 81, B7, C7, D7, E7, F7, G7, H7, 
	A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ
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
	
	// En Passant square, if one is active. If not, this will be 'NO_SQ'
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

	// This is a key used to define what piece is what. There are 12 possible pieces, as we are including the black and white variants.
	int pieceNum[13];

	// The 3 below arrays all have 3 ints, for white, black and for both.
	// Big Pieces are any piece that is NOT a pawn.
	int bigPieces[3];
	// Major Pieces are Rooks and Queens
	int MajPieces[3];
	// Minor Pieces are Bishops and Knights
	int MinPieces[3];
	


	

} S_BOARD;
 
 
 

 
 
 
 
 
 



#endif
