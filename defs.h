#ifndef DEFS_H
#define DEFS_H

#include "stdlib.h"

//#define DEBUG

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - Failed",#n); \
printf("On %s ",__DATE__); \
printf("At %s ",__TIME__); \
printf("In File %s ",__FILE__); \
printf("At Line %d\n",__LINE__); \
exit(1);}
#endif

typedef unsigned long long U64;

// Name and version of current program
#define NAME "teri 0.1"
// Total amount of squares on the 'board'
#define BRD_SQ_NUM 120 

// Max number of moves we would expect in a game (this is 2048 half-moves).
#define MAXGAMEMOVES 2048
// Max number of moves we would expect in a given position.
#define MAXPOSITIONMOVES 256

// The first part is the description of pieces on the board
// with numbers being the number of consecutive empty pieces in a row
// followed by either 'w' or 'b' for which side to move,
// then castling permissions
// then notation for an En Passant Square if available, if not it is simply '-'
// Then fifty move count
// Then total moves (a move being counted as both black and white have moved)

// enumerated constants of pieces
// 'w' meaning white, 'b' meaning black, followed by a piece letter
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };
// enumerated constants of 'files' aka x axis positions down the board A-H
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE }; 
// enumerated constants of 'ranks' aka y axis positions (what row is it in) 1-8
enum { RANK_1, RANK_2,RANK_3,RANK_4,RANK_5,RANK_6,RANK_7,RANK_8,RANK_NONE };
// Colours of what piece
enum { WHITE, BLACK, BOTH };

enum {
  A1 = 21, B1, C1, D1, E1, F1, G1, H1,
  A2 = 31, B2, C2, D2, E2, F2, G2, H2,
  A3 = 41, B3, C3, D3, E3, F3, G3, H3,
  A4 = 51, B4, C4, D4, E4, F4, G4, H4,
  A5 = 61, B5, C5, D5, E5, F5, G5, H5,
  A6 = 71, B6, C6, D6, E6, F6, G6, H6,
  A7 = 81, B7, C7, D7, E7, F7, G7, H7,
  A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
};

enum { FALSE, TRUE };

enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

typedef struct {
	int move;
	int score;
} S_MOVE;

typedef struct {
	S_MOVE moves[MAXPOSITIONMOVES];
	int count;
} S_MOVELIST;

// Information needed to undo a move
typedef struct {

	// A move, stored as an integer.
	int move;
	// Castle Permission, before that move was played.
	int castlePerm;
	// En Passant Square before move was played.
	int enPas;
	// Fifty Move Count Status
	int fiftyMove;
	// Position Key, to define board state in positions
	U64 posKey;

} S_UNDO;

// S_BOARD is the structure defining the board
typedef struct {

	// Define an array of ints, each value corresponding to a square
	// The int value will define the 'state' of the square.
	int pieces[BRD_SQ_NUM];

	// The 64 defines one bit, for each (valid)square.
	// We define three of these, one for each colour - BLACK, WHITE or BOTH.
	// note that BOTH defines the pawn positions for pawns of both sides, so should always equal '~(WHITE | BLACK)' (thats C for NAND)
	U64 pawns[3];
	
	// kingSq holds the square that the kings are on, for white and black, at index 0 and 1 of this array.
	int KingSq[2];

	// side holds the current side to move
	int side;
	
	// En Passant square, if one is active. If not, this will be 'OFF_BOARD'
	int enPas;

	// The counter for fifty moves, we are counting in plys or half moves, so this will have to equal 100, not 50.
	int fiftyMove;

	// The ply, how many half-moves into the current search.
	int ply;

	// In the total game so far, how many half-moves have been played.
	int hisPly;

	int castlePerm;

	// a unique key generated for each position.
	U64 posKey;

	// This is a key used to define how many pieces are on the board of a type. There are 12 possible pieces, as we are including the black and white variants.
	int pceNum[13];

	// The 3 below arrays all have 3 ints, for white, black and for both.
	// Big Pieces are any piece that is NOT a pawn.
	int bigPce[2];
	// Major Pieces are Rooks and Queens and Kings
	int majPce[2];
	// Minor Pieces are Bishops and Knights
	int minPce[2];

	// Holds the material score, for white and black.
	int material[2];

	// Now we should be able to undo the entire way back through each half move.
	S_UNDO history[MAXGAMEMOVES];
	
	// Piece List
	// 13 pieces, of which you can have at MOST 10 of, on the board (would be done via pawn promotions)
	int pList[13][10];

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
 * 0000 0000 0000 0000 0000 0111 1111
 *                           ^^^ ^^^^                     
 * 	         	    From Square
 *
 * The least significant 7 bits store the 'From' square.
 *
 * To Get: 0x7F
 *
 *
 * 0000 0000 0000 0011 1111 1--- ----
 *                  ^^ ^^^^ ^
 *                 To Square
 *
 * The next least significant 7 bits store the 'To' Square
 *
 * To Get: >> 7, 0x7F
 *                
 *
 * 0000 0000 0011 11-- ---- ---- ----
 *             ^^ ^^
 *        What Piece, if any, was captured. 
 * 	  We have 12 possible pieces, so we can store all of that in 4 bits.
 *
 * To Get: >> 14, 0xF
 *
 * 0000 0000 01-- ---- ---- ---- ----
 *	      ^
 *	Was the move an En Passant Capture or not?
 *
 * To Get: 0x40000pce
 *
 *
 * 0000 0000 1-- ---- ---- ---- ----
 *           ^
 *      Was the move a pawn start?
 *      If so, this bit should be set.
 *
 * To Get: 0x80000
 *
 *
 * 0000 1111 ---- ---- ---- ---- ----
 *      ^^^^ 
 *    If a pawn promoted, what piece did it promote to?
 *    If nothing promoted, all bits are set to 0.
 *
 *    4 bits correspond to: Bishop - Rook - Queen - Knight
 *
 * To Get: >> 20, 0xF
 *
 *
 * 0001 ---- ---- ---- ---- ---- ----
 *    ^
 * Was the move a castling move?
 * If so, this bit should be set.
 *
 * The last three bits should never be set.
 *
 * To Get: 0x1000000
 *
 *
 *
 *
 */


#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m)>>7) & 0x7F)
#define CAPTURED(m) (((m)>>14) & 0xF)
#define PROMOTED(m) (((m)>>20) & 0xF)

#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000

#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000


/* MACROS */

#define FR2SQ(f,r) ( (21 + (f) ) + ( (r) * 10 ) ) 
#define SQ64(sq120) (Sq120ToSq64[(sq120)])
#define SQ120(sq64) (Sq64ToSq120[(sq64)])
#define POP(b) PopBit(b)
#define CNT(b) CountBits(b)
#define CLRBIT(bb,sq) ((bb) &= ClearMask[(sq)])
#define SETBIT(bb,sq) ((bb) |= SetMask[(sq)])

#define IsBQ(p) (PieceBishopQueen[(p)])
#define IsRQ(p) (PieceRookQueen[(p)])
#define IsKn(p) (PieceKnight[(p)])
#define IsKi(p) (PieceKing[(p)])

/* GLOBALS */

extern int Sq120ToSq64[BRD_SQ_NUM];
extern int Sq64ToSq120[64];
extern U64 SetMask[64];
extern U64 ClearMask[64];
extern U64 PieceKeys[13][120];
extern U64 SideKey;
extern U64 CastleKeys[16];
extern char PceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];

extern int PieceBig[13];
extern int PieceMaj[13];
extern int PieceMin[13];
extern int PieceVal[13];
extern int PieceCol[13];
extern int PiecePawn[13];

extern int FilesBrd[BRD_SQ_NUM];
extern int RanksBrd[BRD_SQ_NUM];

extern int PieceKnight[13];
extern int PieceKing[13];
extern int PieceRookQueen[13];
extern int PieceBishopQueen[13];
extern int PieceSlides[13];

/* FUNCTIONS */

// init.c
extern void AllInit();

// bitboards.c
extern void PrintBitBoard(U64 bb);
extern int PopBit(U64 *bb);
extern int CountBits(U64 b);

// hashkeys.c
extern U64 GeneratePosKey(const S_BOARD *pos);

// board.c
extern void ResetBoard(S_BOARD *pos);
extern int ParseFen(char *fen, S_BOARD *pos);
extern void PrintBoard(const S_BOARD *pos);
extern void UpdateListsMaterial(S_BOARD *pos);
extern int CheckBoard(const S_BOARD *pos);

// attack.c
extern int SqAttacked(const int sq, const int side, const S_BOARD *pos);

// io.c
extern char *PrMove(const int move);
extern char *PrSq(const int sq);
extern void PrintMoveList(const S_MOVELIST *list);

//validate.c
extern int SqOnBoard(const int sq);
extern int SideValid(const int side);
extern int FileRankValid(const int fr);
extern int PieceValidEmpty(const int pce);
extern int PieceValid(const int pce);

// movegen.c
extern void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list);

// makemove.c
extern int MakeMove(S_BOARD *pos, int move);
extern void TakeMove(S_BOARD *pos);

// perft.c 
extern void PerftTest(int depth, S_BOARD *pos);
#endif















