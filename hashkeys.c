#include "defs.h"
#include <assert.h>


// Position keys aka hash key

// generatePositionKey generates a position key for the given board position
// It takes a pointer to the board state.
u64 generatePositionKey(const S_BOARD *pos) {
	
	int sq = 0;
	u64 finalKey = 0;
	int piece = EMPTY;
	
	// Iterate through squares, looking for pieces
	for (sq = 0; sq < BRD_SQ_NUM; ++sq) {
		piece = pos->pieces[sq];
		if (piece != OFF_BOARD && piece != EMPTY) {
			assert(piece >= wP && piece <= bK);
			finalKey ^= PieceKeys[piece][sq];
		}
	}

	if (pos->side == WHITE) {
		finalKey ^= SideKey;
	}


	if (pos->enPas != OFF_BOARD) {
		assert(pos->enPas >= 0 && pos->enPas < BRD_SQ_NUM);
		finalKey ^= PieceKeys[EMPTY][pos->enPas];
	}

	assert(pos->castlePerm >= 0 && pos->castlePerm <= 15);

	finalKey ^= CastleKeys[pos->castlePerm];

	return finalKey;
		

}
