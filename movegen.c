#include "defs.h"

/*
 * Move Generation
 *
 * We take in a board, and a list.
 *
 * We loop all pieces:
 *
 * 	If it is a slider piece, we loop each direction, adding each move.
 * 		AddMove(list->moves[list->count] = move; list->count++;
 *
 */

/* defines for move generator */

// MOVE takes in (f)rom as the square moving from,
// (t)o as the square moving to
// (ca)ptured as what, if any, piece was captured
// (pro) as the piece to promote to, if possible
// (fl)ags as the various flags for the move
#define MOVE(f,t,ca,pro,fl)  ( (f) | ((t) << 7) | ( (ca) << 14 ) | ( (pro) << 20) | (fl))

int loopSlidePiece[8] = {
	wB, wR, wQ, 0, bB, bR, bQ, 0
};

int loopSlideIndex[2] = { 0, 4 };

int loopNonSlidePiece[6] = {
	wN, wK, 0, bN, bK, 0
};

int loopNonSlideIndex[2] = { 0, 3 };


// PieceDir Defines what directions/square offsets a given piece can move.
int PieceDir[13][8] = {
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 }
};
// This defines how many times to loop through PieceDir for each piece.
int NumDir[13] = {
	0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 7
};


static void AddQuietMove (const S_BOARD *pos, int move, S_MOVELIST *list) {

	list->moves[list->count].move = move;
	// TODO change this value for move scoring
	list->moves[list->count].score = 0;
	list->count++;
}

static void AddCaptureMove (const S_BOARD *pos, int move, S_MOVELIST *list) {

	list->moves[list->count].move = move;
	// TODO change this value for move scoring
	list->moves[list->count].score = 0;
	list->count++;
}

static void AddEnPassantMove (const S_BOARD *pos, int move, S_MOVELIST *list) {

	list->moves[list->count].move = move;
	// TODO change this value for move scoring
	list->moves[list->count].score = 0;
	list->count++;
}

// Could change below to be colour-agnostic via intermediate value for each piece 
static void AddWhitePawnCapMove( const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *list ) { 
	if(RanksBrd[from] == RANK_7) {
		
		assert(!SQOFFBOARD(from));
		assert(!SQOFFBOARD(to));
		AddCaptureMove(pos, MOVE(from,to,cap,wQ,0), list);
		AddCaptureMove(pos, MOVE(from,to,cap,wR,0), list);
		AddCaptureMove(pos, MOVE(from,to,cap,wB,0), list);
		AddCaptureMove(pos, MOVE(from,to,cap,wN,0), list);
	} else {
		AddCaptureMove(pos, MOVE(from,to,cap,EMPTY,0), list);
	}
}

static void AddWhitePawnMove( const S_BOARD *pos, const int from, const int to, S_MOVELIST *list ) {
	if(RanksBrd[from] == RANK_7) {
		assert(!SQOFFBOARD(from));
		assert(!SQOFFBOARD(to));
		AddQuietMove(pos, MOVE(from,to,EMPTY,wQ,0), list);
		AddQuietMove(pos, MOVE(from,to,EMPTY,wR,0), list);
		AddQuietMove(pos, MOVE(from,to,EMPTY,wB,0), list);
		AddQuietMove(pos, MOVE(from,to,EMPTY,wN,0), list);
	} else {
		AddQuietMove(pos, MOVE(from,to,EMPTY,EMPTY,0), list);
	}
}

static void AddBlackPawnCapMove( const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *list ) { 
	if(RanksBrd[from] == RANK_2) {
		assert(!SQOFFBOARD(from));
		assert(!SQOFFBOARD(to));
		AddCaptureMove(pos, MOVE(from,to,cap,bQ,0), list);
		AddCaptureMove(pos, MOVE(from,to,cap,bR,0), list);
		AddCaptureMove(pos, MOVE(from,to,cap,bB,0), list);
		AddCaptureMove(pos, MOVE(from,to,cap,bN,0), list);
	} else {
		AddCaptureMove(pos, MOVE(from,to,cap,EMPTY,0), list);
	}
}

static void AddBlackPawnMove( const S_BOARD *pos, const int from, const int to, S_MOVELIST *list ) {
	if(RanksBrd[from] == RANK_2) {
		assert(!SQOFFBOARD(from));
		assert(!SQOFFBOARD(to));
		AddQuietMove(pos, MOVE(from,to,EMPTY,bQ,0), list);
		AddQuietMove(pos, MOVE(from,to,EMPTY,bR,0), list);
		AddQuietMove(pos, MOVE(from,to,EMPTY,bB,0), list);
		AddQuietMove(pos, MOVE(from,to,EMPTY,bN,0), list);
	} else {
		AddQuietMove(pos, MOVE(from,to,EMPTY,EMPTY,0), list);
	}
}

// TODO refactor all above functions into one, parameters for the functions done already below.
// we should be able to add empty fields, as they will be empty if not specified in the MOVE macro.
// TODO!
/*
void AddPawnMove( const S_BOARD *pos, const int from, const int to, int side, int cap, S_MOVELIST *list ) {
	if(RanksBrd[from] == RANK_7) {
		AddQuietMove(pos, MOVE(from,to,EMPTY,wQ,0), list);
		AddQuietMove(pos, MOVE(from,to,EMPTY,wR,0), list);
		AddQuietMove(pos, MOVE(from,to,EMPTY,wB,0), list);
		AddQuietMove(pos, MOVE(from,to,EMPTY,wN,0), list);
	} else {
		AddQuietMove(pos, MOVE(from,to,EMPTY,EMPTY,0), list);
	}
}
*/



void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list) {

	assert(CheckBoard(pos));


	list->count = 0;

	int side = pos-> side;
	int sq;

	int dir = 0;
	int pieceIndex = 0;
	int index = 0;
	int piece = 0;



	if (side == WHITE) {

		for (int pieceNum = 0; pieceNum < pos->pieceNum[wP]; ++pieceNum) {
			
			// Get the square that white pawn number i is on, into the sq variable.
			sq = pos->pieceList[wP][pieceNum];
			assert(!SQOFFBOARD(sq));

			if (pos->pieces[sq+10] == EMPTY) {
				AddWhitePawnMove(pos, sq, sq+10, list);
				// Check if double pawn move from 2nd rank is available, if so, add that to list.
				// we don't have to check if the square in front is empty per current if block
				if (RanksBrd[sq] == RANK_2 && pos->pieces[sq+20] == EMPTY) {
					AddQuietMove(pos, MOVE(sq, sq+20, EMPTY, EMPTY, MFLAGPS), list);
				}
			}

			if(!SQOFFBOARD(sq+9) && PieceCol[pos->pieces[sq+9]] == BLACK) {
				AddWhitePawnCapMove(pos, sq, sq+9, pos->pieces[sq+9], list);
			}

			if(!SQOFFBOARD(sq+11) && PieceCol[pos->pieces[sq+11]] == BLACK) {
				AddWhitePawnCapMove(pos, sq, sq+11, pos->pieces[sq+11], list);
			}
			if(sq+9 == pos->enPas) {
				AddCaptureMove(pos, MOVE(sq, sq+9, EMPTY, EMPTY, MFLAGEP), list);
			}

			if(sq+11 == pos->enPas) {
				AddCaptureMove(pos, MOVE(sq, sq+11, EMPTY, EMPTY, MFLAGEP), list);
			}
		}

		// Note: the castling of moving the rook is done later in MakeMove()
		//
		// If White Castle King Side Available
		if (pos->castlePerm & WKCA) {
			if (pos->pieces[F1] == EMPTY && pos->pieces[G1] == EMPTY) {
				if (!SqAttacked(E1, BLACK, pos) && !SqAttacked(F1, BLACK, pos) ) {
					AddQuietMove(pos, MOVE(E1, G1, EMPTY, EMPTY, MFLAGCA), list);
				}
			}
		}
		//
		// If White Castle Queen Side Available
		if (pos->castlePerm & WQCA) {
			if (pos->pieces[D1] == EMPTY && pos->pieces[C1] == EMPTY && pos->pieces[B1] == EMPTY ) {
				if (!SqAttacked(E1, BLACK, pos) && !SqAttacked(D1, BLACK, pos) ) {
					AddQuietMove(pos, MOVE(E1, C1, EMPTY, EMPTY, MFLAGCA), list);
				}
			}
		}

	} else {
		for (int pieceNum = 0; pieceNum < pos->pieceNum[bP]; ++pieceNum) {
			
			// Get the square that white pawn number i is on, into the sq variable.
			sq = pos->pieceList[bP][pieceNum];
			assert(!SQOFFBOARD(sq));

			if (pos->pieces[sq-10] == EMPTY) {
				AddBlackPawnMove(pos, sq, sq-10, list);
				// Check if double pawn move from 2nd rank is available, if so, add that to list.
				// we don't have to check if the square in front is empty per current if block
				if (RanksBrd[sq] == RANK_7 && pos->pieces[sq-20] == EMPTY) {
					AddQuietMove(pos, MOVE(sq, sq-20, EMPTY, EMPTY, MFLAGPS), list);
				}
			}

			if(!SQOFFBOARD(sq-9) && PieceCol[pos->pieces[sq-9]] == WHITE) {
				AddBlackPawnCapMove(pos, sq, sq-9, pos->pieces[sq-9], list);
			}

			if(!SQOFFBOARD(sq-11) && PieceCol[pos->pieces[sq-11]] == WHITE) {
				AddBlackPawnCapMove(pos, sq, sq-11, pos->pieces[sq-11], list);
			}
			if(sq-9 == pos->enPas) {
				AddCaptureMove(pos, MOVE(sq, sq-9, EMPTY, EMPTY, MFLAGEP), list);
			}

			if(sq-11 == pos->enPas) {
				AddCaptureMove(pos, MOVE(sq, sq-11, EMPTY, EMPTY, MFLAGEP), list);
			}
		}

		// If Black Castle King Side Available
		if (pos->castlePerm & BKCA) {
			if (pos->pieces[F8] == EMPTY && pos->pieces[G8] == EMPTY) {
				if (!SqAttacked(E8, WHITE, pos) && !SqAttacked(F8, WHITE, pos) ) {
					AddQuietMove(pos, MOVE(E8, G8, EMPTY, EMPTY, MFLAGCA), list);
				}
			}
		}
		//
		// If White Castle Queen Side Available
		if (pos->castlePerm & BQCA) {
			if (pos->pieces[D8] == EMPTY && pos->pieces[C8] == EMPTY && pos->pieces[B8] == EMPTY ) {
				if (!SqAttacked(E8, WHITE, pos) && !SqAttacked(D8, WHITE, pos) ) {
					AddQuietMove(pos, MOVE(E8, C8, EMPTY, EMPTY, MFLAGCA), list);
				}
			}
		}

	}
	

	/* Loop for slide pieces */

	pieceIndex = loopSlideIndex[side];
	piece = loopSlidePiece[pieceIndex++];
	while (piece != 0) {
		assert(PieceValid(piece));
		printf("sliders pceIndex:%d pce:%d\n",pieceIndex,piece);
		for (int pieceNum = 0; pieceNum < pos->pieceNum[piece]; ++pieceNum) {
			sq = pos->pieceList[piece][pieceNum];
			assert(!SQOFFBOARD(sq));
			printf("Piece:%c on %s\n", PieceChar[piece], PrintSq(sq));
	// We have now removed 
		
			for( int i = 0; i < NumDir[piece]; ++i) {
				dir = PieceDir[piece][i];
				int t_sq = sq + dir;

				while (!SQOFFBOARD(t_sq)) {




					if (pos->pieces[t_sq] != EMPTY) {
						if (PieceCol[pos->pieces[t_sq]] == (side ^ 1)) {
							AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
						}
						break;
					}
					AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);

					t_sq += dir;
				}

			}
		}
		piece = loopSlidePiece[pieceIndex++];
	}

	/* Loop for non-slide pieces */

	pieceIndex = loopNonSlideIndex[side];
	piece = loopNonSlidePiece[pieceIndex++];
	while (piece != 0) {
		assert(PieceValid(piece));
		printf("non sliders pceIndex:%d pce:%d\n",pieceIndex,piece);
		for (int pieceNum = 0; pieceNum < pos->pieceNum[piece]; ++pieceNum) {
			sq = pos->pieceList[piece][pieceNum];
			assert(!SQOFFBOARD(sq));
			printf("Piece:%c on %s\n", PieceChar[piece], PrintSq(sq));
		
			for( int i = 0; i < NumDir[piece]; ++i) {
				dir = PieceDir[piece][i];
				int t_sq = sq + dir;

				if (SQOFFBOARD(t_sq)) continue;

				if (pos->pieces[t_sq] != EMPTY) {
					if (PieceCol[pos->pieces[t_sq]] == (side ^ 1)) {
						AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
					}
					continue;
				}
				AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);

			}
		}
		piece = loopNonSlidePiece[pieceIndex++];
	}


}

