#include <cstring>

#include "types.hpp"
#include "tables.hpp"
#include "stateTools.hpp"


// supporting routines =========================================================

void mixSingleColumn(byte * r) {
	byte a[4];
	byte b[4];
	byte h;
	// do magic
	for(byte c=0; c<4; c++) {
		a[c] = r[c];
		h = static_cast<byte>(static_cast<char>(r[c]) >> 7);   // major C++ hack
		b[c] = r[c] << 1;
		b[c] ^= 0x1B & h;
	}
	r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
	r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
	r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3];
	r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0];
}

void ksCore(byte * t, const byte i) {
	// rotate 1 byte to the left
	swap(t[0], t[1]);
	swap(t[1], t[2]);
	swap(t[2], t[3]);
	// apply sbox
	for (byte j=0; j<4; j++)
		t[j] = sboxTable[t[j]];
	// XOR the first byte with rcon(i)
	t[0] ^= rconTable[i];
}


// AES subroutines =============================================================

void sbox(const byte * in, byte * out) {
	for (byte i=0; i<16; i++)
		out[i] = sboxTable[in[i]];
}

void shiftRows(byte * in, byte * out) {
	for (byte i=0; i<16; i++)
		out[i] = in[(i + ((i&0b11) << 2)) & 0xf];
}

void mixColumns(byte * a) {
	for (byte i=0; i<4; i++)
		mixSingleColumn(a + (i<<2));
}

void addRoundKey(byte * a, const byte * expkey, const byte round) {
	for (byte i=0; i<16; i++)
		a[i] ^= expkey[(round<<4) + i];
}

void keySchedule(const byte * key, byte * expkey) {
	memcpy(expkey, key, 16);
	byte i(1);
	byte ekend(16);
	byte t[4];
	// do magic
	while (ekend < 176) {
		memcpy(t, expkey + (ekend-4), 4);
		ksCore(t, i++);
		xornbytes(t, expkey + (ekend-16), expkey + ekend, 4);
		ekend += 4;
		for (byte j=0; j<3; j++) {
			memcpy(t, expkey + (ekend-4), 4);
			xornbytes(t, expkey + (ekend-16), expkey + ekend, 4);
			ekend += 4;
		}
	}
}
