#include "naiveAES.hpp"

void naiveAES(const byte * key, const byte * pt, byte * ct) {
	// alloc memory for intermediate results and key expansion
	byte im[16];
	byte expkey[176];
	
	// copy input
	for (byte i=0; i<16; i++) ct[i] = pt[i];
	// run key expansion
	keySchedule(key, expkey);
	
	// ===   A E S   B E G I N   =========================================
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
	// ===   A E S   E N D   =============================================
}

void sbox(const byte * in, byte * out) {
	for (byte i=0; i<16; i++)
		out[i] = sboxTable[in[i]];
}

void shiftRows(byte * in, byte * out) {
	// fuj
	transpose(in);
	for (byte i=0; i<16; i++)
		out[i] = in[(i&0xc)|((i+((i&0xc)>>2))&0x3)];
	transpose(out);
}

void mixColumns(byte * a) {
	for (byte i=0; i<4; i++)
		mixSingleColumn(a + 4*i);
}

void addRoundKey(byte * a, const byte * expkey, const byte round) {
	for (byte i=0; i<16; i++)
		a[i] ^= expkey[16*round + i];
}
