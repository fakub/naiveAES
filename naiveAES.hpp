#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
#include <stdexcept>

#include "tables.hpp"

#pragma once

using namespace std;

// hex conversion tools
inline byte hex2val(const byte c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	throw runtime_error("invalid hex code.");
}

inline void hex2nbytes(const char * hex, byte * bytes, const byte n = 16) {
	for (byte i=0; i<n; i++)
		bytes[i] = 16 * hex2val(hex[2*i]) + hex2val(hex[2*i+1]);
}

inline void byte2hex(const byte a, byte * b) {
	snprintf((char *) b, 2+1, "%02X", a);
}

inline void nbytes2hex(const byte * bytes, byte * hex, const byte n = 16) {
	for (byte i=0; i<n; i++)
		byte2hex(bytes[i], hex + 2*i);
}

// printing tools
inline void printnbytes(const byte * val, const byte n, const string delim = " ") {
	for (int i=0; i<n-1; i++) printf("%02X%s", val[i], delim.c_str());
	printf("%02X\n", val[n-1]);
}

inline void print128bits(const byte * val, const string delim = " ") {printnbytes(val, 16, delim);}

inline void printState(const byte * a) {
	for (byte i=0; i<4; i++) printf("%02X %02X %02X %02X\n", a[i], a[i+4], a[i+8], a[i+12]);
}

// XOR tools
inline void xornbytes(const byte * a, const byte * b, byte * c, byte n) {
	for (byte i=0; i<n; i++)
		c[i] = a[i] ^ b[i];
}

// swap
inline void swap(byte & a, byte & b) {
	a ^= b;
	b ^= a;
	a ^= b;
}

// transpose state matrix
inline void transpose(byte * a) {
	swap(a[1], a[4]);
	swap(a[2], a[8]);
	swap(a[3], a[12]);
	swap(a[6], a[9]);
	swap(a[7], a[13]);
	swap(a[11], a[14]);
}

// naive AES
void naiveAES(const byte * key, const byte * pt, byte * ct);
void sbox(const byte * in, byte * out);
void shiftRows(byte * in, byte * out);
void mixColumns(byte * a);
void addRoundKey(byte * a, const byte * expkey, const byte round);

inline void mixSingleColumn(byte * r) {
	byte a[4];
	byte b[4];
	byte c;
	byte h;
	for(c=0; c<4; c++) {
		a[c] = r[c];
		h = (byte)((signed char)r[c] >> 7);
		b[c] = r[c] << 1;
		b[c] ^= 0x1B & h;
	}
	r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
	r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
	r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3];
	r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0];
}

inline void ksCore(byte * t, byte i) {
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

inline void keySchedule(const byte * key, byte * expkey) {
	memcpy(expkey, key, 16);
	byte i = 1;
	byte ekend = 16;
	byte t[4];
	
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
