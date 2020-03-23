
#pragma once
#include <bitset>
using namespace std;


template <size_t BIT_SIZE> int bitsetRead (bitset <BIT_SIZE> inpBits ) {
	if (inpBits[BIT_SIZE - 1] == 1) {
		inpBits = inpBits.flip();
		return  (-1 * inpBits.to_ulong()) - 1;
	} else {
		return inpBits.to_ulong();
	}
} 