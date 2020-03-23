#pragma once
#include <iostream>
#include <bitset>
#include <string>
#include <deque> 
#define registerWidth 32
using namespace std;

class Register{
	private:
		int value;
	public:
		Register(){
			value = 0;
		}
		
		void reset(){
			value = 0;
		}
		
		void writeInt(int x){
			value = x;
		}

		void writeBitset(bitset <registerWidth> x){
			value = x.to_ulong();
		}
		
		int readInt(){
			return value;
		}
		
		bitset<registerWidth> readBitset(){
			bitset <registerWidth> x(value);	
			return x;
		}
};

class InterStateBuffers{
	public:
		Register RA, RB, RX, RY, RM, RZ, RMD;
		int PC;
		int mem_register, return_address;
		Register IR;
		int pc_offset;
		int insType;
		string ALU_OP;
		bool isjalr, isMem;

		bool enableCache;
		// For stalling
		bool stall;
		int numStall;
		int totalCycles;

		int hazard_type; /* 0 No Branch , 1 Jal ,  2 Jalr ,3 branch */
		int branch_address_def;
		int branch_address;

		int insTypeD,insTypeE,insTypeM,insTypeW;
		int isjalrD,isjalrE,isjalrM,isjalrW;
		int wblocD,wblocE,wblocM,wblocW;
		int isMemD,isMemE,isMemM,isMemW;
		int returnAddD,returnAddE,returnAddM,returnAddW;

		bool taken;
		int mispredNumber;
		bool isMispred;
		int nextPC;

		int pWrite;     // holds the write register value for the previous instruction
    int ppWrite;    // the instruction before that
    string pInst;      // instruction
    string ppInst;     //

		int coldmiss;
		int hitcount;
		int accesscount;
		int conflict_misses_data;
		int cold_misses_data;
		int capacity_misses_data;
		int accesses_data;
		int hits_data;

		//Write back location:- stores register number for writeback, 
		//-1 for SB type where no write back occurs.
		int write_back_location;


		//inserting knobs as bool types, specification given with each knob
		//take in the value at runtime or hardcode it for development purposes

		bool enablePipe ;	// E/D pipelining
					// This would be used by the control unit
		bool enableDF ;	// E/D data forwarding
					// Used by decode, execute, memory units
		bool printRegFile ;	// E/D printing register file values after each cycle
					// Used by control
		bool printISB;	// E/D printing pipleline registers after each cycle
					// Control
		bool printISBspecific;	// E/D printing for a specific instruction, handle later
					// Control, decode(maybe)
		int conflict_misses;
		int cold_misses;
		int capacity_misses;

		InterStateBuffers(){
			PC = 1;
			return_address = 1;
			mem_register = 0;
			pc_offset = 0;
			isjalr = false;
			isMem = false;
			write_back_location = -1;

			enablePipe = true;
			enableDF = true; 
			printRegFile = false;
			printISB = false;
			printISBspecific = false;
			stall  = false;
			hazard_type = 0;

			wblocW = -1;
			wblocM = -1;
			wblocE = -1;
			wblocD = -1;
	
			returnAddW = -1;
			returnAddM = -1;
			returnAddE = -1;
			returnAddD = -1;	

			insTypeW = -1;
			insTypeM = -1;
			insTypeE = -1;
			insTypeD = -1;

			isjalrW = 0;
			isjalrM = 0;
			isjalrE = 0;
			isjalrD = 0;

			isMemW = 0;
			isMemM = 0;
			isMemE = 0;
			isMemD = 0;

			taken = true;
			mispredNumber = 0;
			isMispred = false;
			nextPC = 0;

			enableCache = true;

			pWrite = 0;
			ppWrite = 0;
			numStall = 0;
			totalCycles = 0;
			
			coldmiss = 0;
			hitcount = 0;
			accesscount = 0;
			cold_misses_data = 0;
			capacity_misses_data =  0;
			conflict_misses_data = 0;
			accesses_data = 0;
			hits_data = 0;
		}

		void resetAll(){
			RA.reset();
			RB.reset();
			RX.reset();
			RY.reset();
			RM.reset();
			RZ.reset();
			mem_register = 0;
			pc_offset = 0;
		}

		void printAll(){
			cout<<"********** Inter State Buffer Values ***********\n";
			cout<<"\tRA\t:\t"<<RA.readInt()<<endl;
			cout<<"\tRB\t:\t"<<RB.readInt()<<endl;
			cout<<"\tRM\t:\t"<<RM.readInt()<<endl;
			cout<<"\tRZ\t:\t"<<RZ.readInt()<<endl;
			cout<<"\tRY\t:\t"<<RY.readInt()<<endl;
			cout<<"*************************************************\n";
		}
};
	

