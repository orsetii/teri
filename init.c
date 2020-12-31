#include "defs.h"
#include <string.h>
#include <stdlib.h>


#define RAND_64 (	(u64)rand() + \
			(u64)rand() << 15 + \
			(u64)rand() << 30 + \
			(u64)rand() << 45 + \
			((u64)rand() & 0xf) << 60	)

u64 SetMask[64];
u64 ClearMask[64];

u64 PieceKeys[13][120];
u64 SideKey;
u64 CastleKeys[16];

void InitHashKeys() {

	for (int i = 0; i < 13; ++i) {
		for (int j = 0; j < 120; ++j) {
			PieceKeys[i][j] = RAND_64;
		}
	}

	SideKey = RAND_64;

	for (int i = 0; i < 16; ++i) {
		CastleKeys[i] = RAND_64;
	}



}



void InitBitMasks() {
	memset(SetMask, 0, sizeof(u64)*64);
	memset(ClearMask, 0, sizeof(u64)*64);

	int i;

	for (i = 0; i < 64; i++) {
		SetMask[i] |= (1ULL << i);
		ClearMask[i]  = ~SetMask[i];
	}
}



void teri_init() {
	InitBitMasks();
	InitHashKeys();
}