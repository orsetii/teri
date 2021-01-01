#include "defs.h"


// Knight Direction defines offset from a square that can be attacked by a knight, should it be on a square on these offsets.
const int KnDir[8] = { -8, -19, -21, -12, 8, 19, 21, 12 };

// Rook Direction defines minimum and maximum lines that a square can be attacked by a rook, either via x or y axis files/ranks.
const int RkDir[4] = { -1, -10, 1, 10 };

// Bishop Direction defines minimum and maximum lines that a square can be attacked by a bishop
const int BiDir[4] = { -9, -11, 11, 9 };

// King Direction defines the movement available to a king piece, being able to move one square in any direction.
const int KiDir[8] = { -1, -10, 1, 10, -9, -11, 11, 9 };

// SqAttacked defines if a square is attacked, and if so, what by and how.
// sq defines the square in question
// side defines the side to move
// pos is a pointer to the boardstate
int SqAttacked(const int sq, const int side, const S_BOARD *pos) {

	assert(SqOnBoard(sq));
	assert(SideValid(side));
	assert(CheckBoard(pos));
	

	int piece,t_sq,dir;
	
	// Checking for Pawn Attacks
	if(side == WHITE) {
		if(pos->pieces[sq-11] == wP || pos->pieces[sq-9] == wP) {
			return TRUE;
		}
	} else {
		if(pos->pieces[sq+11] == bP || pos->pieces[sq+9] == bP) {
			return TRUE;
		}	
	}

	// Checking for Knight Attacks
	for (int i = 0; i < 8; ++i) {
		// Check squares that, if containing a knight would be attacking this square.
		piece = pos->pieces[sq+KnDir[i]];
		// Then, check that square if it has a knight on it.
		// And that the piece is of the colour that is attacking / to move.
		if (IsKn(piece) && PieceCol[piece] == side) {
			return TRUE;
		}

	}

	// Checking For Rooks & Queen Attacks
	// These pieces are 'sliding pieces' meaning
	// they can move the entire length of the board if there is nothing in their way
	// So, we need to keep traversing in each available direction until either we hit
	// a piece (be it of the same colour or opposite) or an OFF_BOARD square.
	for (int i = 0; i < 4; ++i) {
		// Get a direction (this is looped through for each direction)
		dir = RkDir[i];
		// Create a temporary square, at the original square + the direction(the direction always
		// being 1 move in some direction)
		t_sq = sq + dir;
		// Get the piece at our temp square (t_sq)
		piece = pos->pieces[t_sq];
		// Loop until we go off the board or loop is exited manually.
		while (piece != OFF_BOARD) {
			if (piece != EMPTY) {
				// Same as the knights, we ask 
				// Is it of the right type?
				// Is it of the attacking colour?
				// If yes, we return TRUE
				if(IsRQ(piece) && PieceCol[piece] == side) {
					return TRUE;
				}
				break;
			}
			t_sq += dir;
			piece = pos->pieces[t_sq];

		}
	}

	// Checking For Bishop & Queen Attacks
	// These pieces are 'sliding pieces' meaning
	// they can move the entire length of the board if there is nothing in their way
	// So, we need to keep traversing in each available direction until either we hit
	// a piece (be it of the same colour or opposite) or an OFF_BOARD square.
	for (int i = 0; i < 4; ++i) {
		// Get a direction (this is looped through for each direction)
		dir = BiDir[i];
		// Create a temporary square, at the original square + the direction(the direction always
		// being 1 move in some direction)
		t_sq = sq + dir;
		// Get the piece at our temp square (t_sq)
		piece = pos->pieces[t_sq];
		// Loop until we go off the board or loop is exited manually.
		while (piece != OFF_BOARD) {
			if (piece != EMPTY) {
				// Same as the knights, we ask 
				// Is it of the right type?
				// Is it of the attacking colour?
				// If yes, we return TRUE
				if(IsBQ(piece) && PieceCol[piece] == side) {
					return TRUE;
				}
				break;
			}
			t_sq += dir;
			piece = pos->pieces[t_sq];

		}
	}

	// Checking For King Attacks
	for (int i = 0; i < 8; ++i) {
		piece = pos->pieces[sq + KiDir[i]];
		if (IsKi(piece) && PieceCol[piece] == side) {
			return TRUE;
		}
	}

	// If we get here and haven't returned yet, it is not attacked
	// So we return false.
	return FALSE;
	
}




	
	
