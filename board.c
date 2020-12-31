#include <stdio.h>
#include "defs.h"


void resetBoard(S_BOARD *pos) {

	// Reset each square, with all squares init'd to OFF_BOARD
	// Valid Squares are then re-assigned to EMPTY
	for (int i = 0; i < BRD_SQ_NUM; ++i) {
		pos->pieces[i] = OFF_BOARD;
		if (i < 64) {
			pos->pieces[board_120[board_64[i]]] = EMPTY;
		}
		// Reset Piece Numbers
		// We do one loop for WHITE, for BLACK and for BOTH.
		if (i < 3) {
			pos->bigPieces[i] = 0;
			pos->MajPieces[i] = 0;
			pos->MinPieces[i] = 0;
			pos->pawns[i]  = 0ULL;
		}
		if (i < 13) {
			pos->pieceNum[i] = 0;
		}

	}

	pos->kingSq[WHITE] = pos->kingSq[1] = NO_SQ;

	pos->side = BOTH;
	pos->enPas = NO_SQ;
	pos->fiftyMovCnt = 0;

	pos->ply = 0;
	pos->hist_ply = 0;

	pos->castlePerm = 0;

	pos->positionKey = 0ULL;

	// TODO make function to init pieceList


}


