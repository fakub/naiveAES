#pragma once

// supporting routines
void ksCore(byte * t, byte i);
void mixSingleColumn(byte * r);

// AES subroutines
void sbox(const byte * in, byte * out);
void shiftRows(byte * in, byte * out);
void mixColumns(byte * a);
void addRoundKey(byte * a, const byte * expkey, const byte round);

void keySchedule(const byte * key, byte * expkey);
