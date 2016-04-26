#include "types.hpp"

void xornbytes(const byte * a, const byte * b, byte * c, byte n) {
	for (byte i=0; i<n; i++)
		c[i] = a[i] ^ b[i];
}

void swap(byte & a, byte & b) {
	a ^= b;
	b ^= a;
	a ^= b;
}

