#include <bits/stdc++.h>
#include "../RegistryFile.h"

using namespace std;


int main (){
	bitset <32> r(5); 
	Registry_File te;
	te.writeInt(12, 16);
	te.writeBits(12,r );
	cout << te.readInt(12) <<endl;
	cout << te.readBits(12) <<endl;

}