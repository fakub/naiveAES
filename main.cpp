#include <iostream>
#include <cstring>
#include <stdexcept>

#include "types.hpp"
#include "hexTools.hpp"
#include "printTools.hpp"
#include "naiveAES.hpp"

int main(const int argc, const char ** argv) {
	try {
		// check input length
		if (argc < 2)
			throw runtime_error("Plaintext required (32 hex chars).");
		if (strlen(argv[1]) != 32)
			throw runtime_error("Input length must be 32 hex chars.");
		
		// alloc and read input
		byte key[16];
		byte pt[16];
		hex2nbytes("2b7e151628aed2a6abf7158809cf4f3c", key);
		hex2nbytes(argv[1], pt);
		
		// alloc output and run naiveAES
		byte ct[16];
		naiveAES(key, pt, ct);
		
		// print output with empty delimiter
		print128bits(ct, "");
	}
	catch(const runtime_error & re) {
		cerr << "Error: " << re.what() << endl;
	}
	catch(...) {
		cerr << "Error: unspecified." << endl;
	}
	return 0;
}