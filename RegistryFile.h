#pragma once
#include "Functions.h"
#include<bitset>
#include<vector>

#define REG_WIDTH 32
using namespace std;

class Registry_File
{
	private:
		vector< bitset <REG_WIDTH>> registers;

	public:
		Registry_File()
		{
			for(int i=0;i<REG_WIDTH;i++)
			{
				registers.push_back(0);
			}

			registers[2] = 200000;
		}

		void writeInt(int index,int value)
		{
			if(index!=0)
				registers[index] = value;
		}

		void readInt(int index)
		{
			return bitsetRead(registers[index]);
		}

		void writeBits(int index,bitset<REG_WIDTH> value)
		{
			if(index!=0)
				registers[index]=value;
		}

		bitset<REG_WIDTH>readBits(int index)
		{
			return registers[index];
		}

		void print()
		{
			cout<<"\t\t\tRegistrty File \n";
			for(int i=0;i<8;i++)
			{
				cout<<"Reg"<<i<<" "<<bitsetRead(registers[i])<<"\t";
				cout<<"Reg"<<i+8<<" "<<bitsetRead(registers[i+8])<<"\t";
				cout<<"Reg"<<i+16<<" "<<bitsetRead(registers[i+16])<<"\t";
				cout<<"Reg"<<i+24<<" "<<bitsetRead(registers[i+24])<<"\t";
			}

			cout<<"\t\t\t"<<endl;

		}
};
