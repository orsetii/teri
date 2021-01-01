#include "defs.h"

// Hashing Macros

#define HASH_PCE(pce,sq) (pos->positionKey ^= (PieceKeys[(pce)][(sq)]))
#define HASH_CA (pos->posKey ^= (CastleKeys[(pos->castlePerm)]))
#define HASH_SIDE (pos->positionKey ^= (SideKey))
#define HASH_EP (pos->positionKey ^= (PieceKeys[EMPTY][(pos->enPas)]))


// 15 = 1111
// ca_perm &= 3 -> 0011
// So we can update castling permission for each side via bitwise operations.
const int CastlePerm[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

static void ClearPiece(const int sq, S_BOARD *pos) {

	assert(!SQOFFBOARD(sq));

	int piece = pos->pieces[sq];

	assert(PieceValid(piece));

	int colour = PieceCol[piece];
	int i = 0;
	int t_pieceNum = -1;


	HASH_PCE(piece, sq);

	pos->pieces[sq] = EMPTY;
	pos->material[colour] -= PieceVal[piece];

	if (PieceBig[piece]) {
		pos->BigPieces[colour]--;
		if (PieceMaj[piece]) {
			pos->MajPieces[colour]--;
		} else {
			pos->MinPieces[colour]--;
		}
	} else {
		CLRBIT(pos->pawns[colour], SQ264(sq));
		CLRBIT(pos->pawns[BOTH], SQ264(sq));
	}

	/* 
	 * pos->pieceNum[wP] == 5 Looping from 0 to 4
	 * pos->pieceList[piece][0] == sq0
	 * pos->pieceList[piece][0] == sq1
	 * pos->pieceList[piece][0] == sq2
	 * ...
	 *
	 */
	for (i = 0; i < pos->pieceNum[piece]; ++i) {
		if (pos->pieceList[piece][i] == sq) {
			t_pieceNum = i;
			break;
		}
	}

	assert(t_pieceNum != -1);

	pos->pieceNum[piece]--;
	pos->pieceList[piece][t_pieceNum] = pos->pieceList[piece][pos->pieceNum[piece]];
	// Following our example
	// pos->pieceList[wP][3] =  pos->pieceList[wP][4] = sq4

	// It removes the entry containing the square entered in the 'sq' param
	// Then re-adjusts the pieceList for that piece type

}
