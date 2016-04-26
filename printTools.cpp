#include <string>

#include "types.hpp"

void printnbytes(const byte * val, const byte n, const string delim = " ") {
	for (byte i=0; i<n-1; i++) printf("%02x%s", val[i], delim.c_str());
	printf("%02x\n", val[n-1]);
}

void print128bits(const byte * val, const string delim = " ") {printnbytes(val, 16, delim);}
