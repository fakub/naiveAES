#include "types.hpp"
#include "AESSubroutines.hpp"

void naiveAES(const byte * key, const byte * pt, byte * ct) {
	// alloc memory for intermediate results and key expansion
	byte im[16];
	byte expkey[176];
	
	// copy input
	for (byte i=0; i<16; i++) ct[i] = pt[i];
	// run key expansion
	keySchedule(key, expkey);
	
	// ===   A E S   B E G I N   =================================================
	/// addRoundKey
	addRoundKey(ct, expkey, 0);
	/// 9 times do
	for (byte round=0; round<9; round++) {
		/// subBytes
		sbox(ct, im);
		/// shiftRows
		shiftRows(im, ct);
		/// mixColumns
		mixColumns(ct);
		/// addRoundKey
		addRoundKey(ct, expkey, round+1);
	/// end
	}
	/// subBytes
	sbox(ct, im);
	/// shiftRows
	shiftRows(im, ct);
	/// addRoundKey
	addRoundKey(ct, expkey, 10);
	// ===   A E S   E N D   =====================================================
}

