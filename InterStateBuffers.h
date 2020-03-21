#pragma once
#include<iostream>
#include<bitset>
#include<string>
#include<deque>

#define registerWidth 32

using namespace std;

class Register
{
	private:
		int value;
	public:
		Register()
		{
			value = 0;
		}
		void reset()
		{
			value = 0;
		}
		void writeInt(int x)
		{
			value = x;
		}
		void writeBitset(bitset<registerWidth> x)
		{
			value = x.to_ulong();
		}
		int readInt()
		{
			return value;
		}
		bitset<registerWidth> readBitset()
		{
			bitset<registerWidth> x(value);
			return x;
		}
};

class InterStateBuffers
{
	public:
		Register RA, RB, RX, RY, RM, RZ, RMD;
		int PC;
		int mem_register, return_address;
		Register IR;
		int pc_offset;
		int insType;
		string ALU_OP;
		bool isjalr , isMem;

		int write_back_location;//it will hold -1 for write in SB type instruction as there occurs no wirte back in this

		bool printRegFile;

		InterStateBuffers()
		{
			PC = 1;
			return_address = 1;
			mem_register = 0;
			pc_offset  = 0;
			isjalr = false;
			isMem = false;

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