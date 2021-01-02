int MakeMove(S_BOARD *pos, int move) {

	assert(CheckBoard(pos));
	
	int from = FROMSQ(move);
    int to = TOSQ(move);
    int side = pos->side;
	
	assert(!SQOFFBOARD(from));
    assert(!SQOFFBOARD(to));
    assert(SideValid(side));
    assert(PieceValid(pos->pieces[from]));
	
	pos->history[pos->hist_ply].posKey = pos->posKey;
	
	if(move & MFLAGEP) {
        if(side == WHITE) {
            ClearPiece(to-10,pos);
        } else {
            ClearPiece(to+10,pos);
        }
    } else if (move & MFLAGCA) {
        switch(to) {
            case C1:
                MovePiece(A1, D1, pos);
			break;
            case C8:
                MovePiece(A8, D8, pos);
			break;
            case G1:
                MovePiece(H1, F1, pos);
			break;
            case G8:
                MovePiece(H8, F8, pos);
			break;
            default: assert(FALSE); break;
        }
    }	
	
	if(pos->enPas != NO_SQ) HASH_EP;
    HASH_CA;
	
	pos->history[pos->hist_ply].move = move;
    pos->history[pos->hist_ply].fiftyMovCnt = pos->fiftyMovCnt;
    pos->history[pos->hist_ply].enPas = pos->enPas;
    pos->history[pos->hist_ply].castlePerm = pos->castlePerm;

    pos->castlePerm &= CastlePerm[from];
    pos->castlePerm &= CastlePerm[to];
    pos->enPas = NO_SQ;

	HASH_CA;
	
	int captured = CAPTURED(move);
    pos->fiftyMovCnt++;
	
	if(captured != EMPTY) {
        assert(PieceValid(captured));
        ClearPiece(to, pos);
        pos->fiftyMovCnt = 0;
    }
	
	pos->hist_ply++;
	pos->ply++;
	
	if(PiecePawn[pos->pieces[from]]) {
        pos->fiftyMovCnt = 0;
        if(move & MFLAGPS) {
            if(side==WHITE) {
                pos->enPas=from+10;
                assert(RanksBrd[pos->enPas]==RANK_3);
            } else {
                pos->enPas=from-10;
                assert(RanksBrd[pos->enPas]==RANK_6);
            }
            HASH_EP;
        }
    }
        
	MovePiece(from, to, pos);
	
	int prPce = PROMOTED(move);
    if(prPce != EMPTY)   {
        assert(PieceValid(prPce) && !PiecePawn[prPce]);
        ClearPiece(to, pos);
        AddPiece(to, pos, prPce);
    }
	
	if(PieceKing[pos->pieces[to]]) {
        pos->kingSq[pos->side] = to;
    }
	
	pos->side ^= 1;
    HASH_SIDE;

    assert(CheckBoard(pos));
	
		
	if(SqAttacked(pos->KingSq[side],pos->side,pos))  {
		TakeMove(pos);
		return FALSE;
    	}
	
	return TRUE;
	
}
