#include <stdio.h>
#include "defs.h"
#include <assert.h>



static int chessNotationToSQ120(char* square) {

	if (*square == '-') return NO_SQ;
	

	int rank = NO_SQ;
	int file = NO_SQ;
	
	
	file = square[0] - 'a';
	rank = square[1] - '1';


	assert(rank >= RANK_1 && rank <= RANK_8);
	assert(file >= FILE_A && file <= FILE_H);
	return FR2SQ(file, rank);
}


// CheckBoard takes in a board struct and does many checks to ensure that the entire struct matches the board state
// and everything is in order.
// It does this by attempting to replicate some fields using other fields, which, in a correct configuration should always check out.
int CheckBoard(const S_BOARD *pos) {


	int t_pceNum[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int t_BigPieces[2] = { 0, 0 };
	int t_MajPieces[2] = { 0, 0 };
	int t_MinPieces[2] = { 0, 0 };
	int t_material[2] = { 0, 0 };
	
	int sq64,t_piece,t_pce_num,sq120,colour,pcount;
	
	u64 t_pawns[3] = {0ULL, 0ULL, 0ULL};
	
	t_pawns[WHITE] = pos->pawns[WHITE];
	t_pawns[BLACK] = pos->pawns[BLACK];
	t_pawns[BOTH] = pos->pawns[BOTH];
	
	// Loop through all pieces.
	for(t_piece = wP; t_piece <= bK; ++t_piece) {
		// Loop through each instance of each piece.
		for(t_pce_num = 0; t_pce_num < pos->pieceNum[t_piece]; ++t_pce_num) {
			sq120 = pos->pieceList[t_piece][t_pce_num];
			assert(pos->pieces[sq120] == t_piece);
		}	
	}	
	
	for(sq64 = 0; sq64 < 64; ++sq64) {
		sq120 = SQ120(sq64);
		t_piece = pos->pieces[sq120];
		t_pceNum[t_piece]++;
		colour = PieceCol[t_piece];
		if( PieceBig[t_piece] == TRUE) t_BigPieces[colour]++;
		if( PieceMin[t_piece] == TRUE) t_MinPieces[colour]++;
		if( PieceMaj[t_piece] == TRUE) t_MajPieces[colour]++;
		
		t_material[colour] += PieceVal[t_piece];
	}
	
	for(t_piece = wP; t_piece <= bK; ++t_piece) {
		assert(t_pceNum[t_piece]==pos->pieceNum[t_piece]);	
	}

	// check bitboards count
	pcount = CNT(t_pawns[WHITE]);
	assert(pcount == pos->pieceNum[wP]);
	pcount = CNT(t_pawns[BLACK]);
	assert(pcount == pos->pieceNum[bP]);
	pcount = CNT(t_pawns[BOTH]);
	assert(pcount == (pos->pieceNum[bP] + pos->pieceNum[wP]));
	
	// check bitboards squares
	while(t_pawns[WHITE]) {
		sq64 = POP(&t_pawns[WHITE]);
		assert(pos->pieces[SQ120(sq64)] == wP);
	}
	
	while(t_pawns[BLACK]) {
		sq64 = POP(&t_pawns[BLACK]);
		assert(pos->pieces[SQ120(sq64)] == bP);
	}
	
	while(t_pawns[BOTH]) {
		sq64 = POP(&t_pawns[BOTH]);
		assert( (pos->pieces[SQ120(sq64)] == bP) || (pos->pieces[SQ120(sq64)] == wP) );
	}


	// Check Values are the same.
	assert(t_material[WHITE]==pos->material[WHITE] && t_material[BLACK]==pos->material[BLACK]);
	//printf("t_MinPieces[WHITE]: %d : %d\n", *t_MinPieces, *pos->MinPieces);
	//assert(t_MinPieces[WHITE]==pos->MinPieces[WHITE] && t_MinPieces[BLACK]==pos->MinPieces[BLACK]);
	assert(t_MajPieces[WHITE]==pos->MajPieces[WHITE] && t_MajPieces[BLACK]==pos->MajPieces[BLACK]);
	assert(t_BigPieces[WHITE]==pos->BigPieces[WHITE] && t_BigPieces[BLACK]==pos->BigPieces[BLACK]);	
	
	// Check that side is either white or black.
	assert(pos->side==WHITE || pos->side==BLACK);

	// Check that key is correct.
	assert(generatePositionKey(pos)==pos->positionKey);
	
	// Check that En Passant is a valid square OR not a square.
	assert(pos->enPas==NO_SQ || ( RanksBrd[pos->enPas]==RANK_6 && pos->side == WHITE)
		 || ( RanksBrd[pos->enPas]==RANK_3 && pos->side == BLACK));
	
	assert(pos->pieces[pos->kingSq[WHITE]] == wK);
	assert(pos->pieces[pos->kingSq[BLACK]] == bK);
		 
	return TRUE;
}





void UpdateListsMaterial(S_BOARD *pos) {

	int piece, sq, i, colour;

	for (i = 0; i < BRD_SQ_NUM; ++i) {
		sq = i;
		piece = pos->pieces[i];
		if (piece != OFF_BOARD && piece != EMPTY) {
			// Grab the colour of this piece.
			colour = PieceCol[piece]; 
			// If this piece is a big piece, increment the bigPieces count for the colour it is.
			if (PieceBig[piece] == TRUE) pos->BigPieces[colour]++;
			// If this piece is a minor piece, increment the MinPieces count for the colour it is.
			if (PieceMin[piece] == TRUE) pos->MinPieces[colour]++;
			// If this piece is a major piece, increment the MajPieces count for the colour it is.
			if (PieceMaj[piece] == TRUE) pos->MajPieces[colour]++;

			// Add the value of this piece to it's colour's value.
			pos->material[colour] += PieceVal[piece];

			// Piece List
			// index into the pieceList for the piece, then you select which piece to change via
			// using the previous total as an index, which is stored in pos->pieceNum[piece].
			// We then change that piece to be defined as being in 'sq'.
			pos->pieceList[piece][pos->pieceNum[piece]] = sq;
			pos->pieceNum[piece]++;

			if (piece == wK) pos->kingSq[WHITE] = sq;
			if (piece == bK) pos->kingSq[BLACK] = sq;

			if (piece == wP) {
				SETBIT(pos->pawns[WHITE], SQ264(sq));
				SETBIT(pos->pawns[BOTH], SQ264(sq));
			} else if (piece == bP) {
				SETBIT(pos->pawns[BLACK], SQ264(sq));
				SETBIT(pos->pawns[BOTH], SQ264(sq));
			}
		}
	}
}











int parseFen(char* fen, S_BOARD *pos) {


	assert(fen != NULL);
	assert(pos != NULL);

	int rank = RANK_8;
	int file =  FILE_A;
	int piece = 0;
	int i = 0;

	char* char_ptr = fen;

	resetBoard(pos);

	while ((rank >= RANK_1) && *char_ptr) {
		int count = 1;
		switch (*char_ptr) {
			case 'p': piece = bP; break;
			case 'r': piece = bR; break;
			case 'n': piece = bN; break;
			case 'b': piece = bB; break;
			case 'k': piece = bK; break;
			case 'q': piece = bQ; break;

			case 'P': piece = wP; break;
			case 'R': piece = wR; break;
			case 'N': piece = wN; break;
			case 'B': piece = wB; break;
			case 'K': piece = wK; break;
			case 'Q': piece = wQ; break;

			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				  piece = EMPTY;
				  // Here we simply work in ASCII values and doing maths on those char values.
				  // We take the ASCII value of the char at location *char_ptr subtract it by
				  // '0' or 0x30 and take the difference into the 'count' variable.
				  count = *char_ptr - '0';
				  break;
			case '/':
			case ' ':
				  // Decrement rank (we start at RANK_8) to continue looping through for each square.
				  rank--;
				  // Reset file to A.
				  file = FILE_A;
				  // Increment char ptr to next char
				  char_ptr++;
				  continue;
			default:
				  // If we get here, error in parsing this char occurred.
				  printf("Error in Parsing FEN String: %s at char: %c\n", fen, *char_ptr);
				  return -1;

			}

		
		// For each square in the rank
		// iterate through each file
		// if 'piece' is defined,  
		for (i = 0; i < count; i++) {
			if (piece != EMPTY) {
				pos->pieces[FR2SQ(file, rank)] = piece;
			}
			file++;
		}
		char_ptr++;
	}


	// Check we are at correct char, as it must be either white or black to move.
	assert(*char_ptr == 'w' || *char_ptr == 'b');
	
	// If not white, set side to play as black and vice versa.
	pos->side = (*char_ptr == 'w') ? WHITE : BLACK;

	// Increment char pointer to point at the first char of the castling permission chars.
	char_ptr += 2;

	// Iterate through castling permissions, break out on spaces.
	for (int i = 0; i < 4; i++) {
		if (*char_ptr == ' ') break;
		switch(*char_ptr) {
			case 'K': pos->castlePerm |= WKCA; break;
			case 'Q': pos->castlePerm |= WQCA; break;
			case 'k': pos->castlePerm |= BKCA; break;
			case 'q': pos->castlePerm |= BQCA; break;
			default: break;
		}
		char_ptr++;
	}
	char_ptr++;

	// Set En Passant Square.
	pos->enPas = chessNotationToSQ120(char_ptr);	

	char_ptr += 2;
	pos->fiftyMovCnt = atoi(char_ptr);

	char_ptr += 2;
	pos->ply = atoi(char_ptr) * 2;
	pos->positionKey = generatePositionKey(pos);

	UpdateListsMaterial(pos);

	return 0;

}










void resetBoard(S_BOARD *pos) {

	// Reset each square, with all squares init'd to OFF_BOARD
	// Valid Squares are then re-assigned to EMPTY
	for (int i = 0; i < BRD_SQ_NUM; ++i) {
		pos->pieces[i] = OFF_BOARD;
		// If the square is a valid square in the hardcoded 120 sq board
		// then it is counted as such via declaring it EMPTY.
		if (board_120[i] != -1) pos->pieces[i] = EMPTY;
		// Reset Piece Numbers
		// We do one loop for WHITE, for BLACK and for BOTH.
		if (i < 2) {
			pos->BigPieces[i] = 0;
			pos->MajPieces[i] = 0;
			pos->MinPieces[i] = 0;
			pos->material[i] = 0;
			pos->pawns[i]  = 0ULL;
		}
		if (i < 13) {
			pos->pieceNum[i] = 0;
		}

	}

	pos->kingSq[WHITE] = pos->kingSq[BLACK] = NO_SQ;


	// When cleared, we set the board to both. This allows us to check further board state, before beginning the game.
	pos->side = BOTH;

	// Set En Passant Squares to N/A
	pos->enPas = NO_SQ;

	// Reset the Fifty Move Counter to 0.
	pos->fiftyMovCnt = 0;

	pos->ply = 0;
	pos->hist_ply = 0;

	pos->castlePerm = 0;

	pos->positionKey = 0ULL;


	for (int i = 0; i < MAXGAMEMOVES; ++i) {
		pos->history[i].move = 0;
		pos->history[i].castlePerm = 0;
		pos->history[i].enPas = NO_SQ;
		pos->history[i].fiftyMovCnt = 0;
		pos->history[i].positionKey = 0ULL;

	}


	// TODO make function to init pieceList


}




void printBoard(const S_BOARD *pos) {


	int sq, file, rank, piece;

	printf("\nGame board:\n");


	// TODO print chess unicode chars
	// Loop through ranks, starting from RANK 8, as this is standard to have rank 1 at the bottom.
	for (rank = RANK_8; rank >= RANK_1; rank--) {
		// Print ranks on the side of board.
		// Add 1 as we are 1-indexed for ranks in chess.
		printf("%d ", rank+1);
		for (file = FILE_A; file <= FILE_H; file++) {
			sq = FR2SQ(file, rank);
			piece = pos->pieces[sq];
			printf("%3c", PieceChar[piece]);
		}
		printf("\n");
	}
	printf("\n  ");

	// TODO: hardcode this shit later.
	for (file = FILE_A; file <= FILE_H; file++) {
		printf("%3c", 'a'+file);
	}

	printf("\n");

	printf("side: %c\n", SideChar[pos->side]);


	// TODO make sq120 -> chess notation  function, and use it here.
	printf("En Passant Square: %d\n", pos->enPas);

	printf("castle: %c%c%c%c\n", 
			pos->castlePerm & WKCA ? 'K' : '-',
			pos->castlePerm & WQCA ? 'Q' : '-',
			pos->castlePerm & BKCA ? 'k' : '-',
			pos->castlePerm & BQCA ? 'q' : '-'
	      		);

	
	printf("PosKey: %11X\n", pos->positionKey);









}
