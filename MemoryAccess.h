#pragma once 
#include <bitset>
#include <map>
#include <fstream>
#include <sstream>
#include "InterStateBuffers.h"

using namespace std;
class  MemoryAccess {
private:
	
	map <int , int > MEM;
	
public:
	
	void writeMem(InterStateBuffers &isb) { 
		//TODO : Divide the original into 4 Bytes and Write
		MEM[isb.RZ.readInt()] = isb.RM.readInt();
		cout << "Wrote " << isb.RM.readInt() << " at " << isb.RZ.readInt() <<endl;
	}

	void writeMem(int address, int value){
		MEM[address] = value;
		cout<<" wrote " << value <<" at "<<address<<endl;
	}
	
	void readMem (InterStateBuffers &ibf) {
		ibf.mem_register = MEM[ibf.RZ.readInt()];
		cout << "Read " << ibf.mem_register << " from " << ibf.RZ.readInt() <<endl;
	}

	int readMem (int value) {
		return MEM[value];
	}

	void Test () {
		cout << "Printing Mem" <<endl;
		for (auto elem : MEM) {
			cout << elem.first << " " <<elem.second<< endl;
		}
	}
}; 

