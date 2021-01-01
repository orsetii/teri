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


void AddQuietMove (const S_BOARD *pos, int move, S_MOVELIST *list) {

	list->moves[list->count].move = move;
	// TODO change this value for move scoring
	list->moves[list->count].score = 0;
	list->count++;
}

void AddCaptureMove (const S_BOARD *pos, int move, S_MOVELIST *list) {

	list->moves[list->count].move = move;
	// TODO change this value for move scoring
	list->moves[list->count].score = 0;
	list->count++;
}

void AddEnPassantMove (const S_BOARD *pos, int move, S_MOVELIST *list) {

	list->moves[list->count].move = move;
	// TODO change this value for move scoring
	list->moves[list->count].score = 0;
	list->count++;
}

void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list) {

	list->count = 0;

}

