#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <ctype.h>

using namespace std;

class UJType{
		//instuction imm[20][10:1][11][19:12] | rd[11:7] | opcode[6:0]	
	private:
		vector <string> instructions;
    vector <string> opcode;
	
	public:
		// initialise the vectors with their respective values from the input file.
    void initialise (string filename);
		// checks if given command is present in the list of S Type instructions.
    bool isPresent(string command);
		// Converts string instruction to 32 bit machine code
		bitset <32> decode (string instruction);
};

void UJType::initialise (string filename) {
	ifstream ifile(filename.c_str());
  string instName,instOpcode;
  while(ifile >> instName >> instOpcode) {
    instructions.push_back(instName);
    opcode.push_back(instOpcode);
  }
}

bool UJType::isPresent(string inst){
	int i=0;
	string instName;
	while(!isalnum(inst[i])) i++;
	while(isalpha(inst[i])){
	 	instName.push_back(inst[i]);
		i++;
	}
	vector <string> :: iterator itr = find(instructions.begin(),instructions.end(),instName);
	if(itr == instructions.end())	return false;
	return true;
}

bitset <32> UJType::decode (string instruction){
	bitset <32> machineCode;
	string instName, rdName, labelOffset;
	int offset = 0;
	bool isNegative = false;

	// Splitting instruction into various parts, ignoring whitespaces and commas
	int rdNum,i=0;
	while(!isalnum(instruction[i])) i++;
	while(isalnum(instruction[i])){
	 	instName.push_back(instruction[i]);
		i++;
	}
	while(!isalnum(instruction[i])) i++;
	while(isalnum(instruction[i])){
	 	rdName.push_back(instruction[i]);
		i++;
	}
	while(!isdigit(instruction[i])){
		if(	instruction[i] == '-') isNegative = true;
		i++;
	}
	while(isdigit(instruction[i])){
	 	labelOffset.push_back(instruction[i]);
		i++;
	}

	// index for opcode
	int index = find(instructions.begin(),instructions.end(),instName) - instructions.begin();

	//Finding register number
	if(rdName.size()==2)
		rdNum = rdName[1]-'0';
	else
		rdNum = 10*(rdName[1]-'0') + (rdName[2]-'0');
	bitset <5> rd(rdNum);

	//Finding offset
	for(i=0; labelOffset[i] != '\0'; i++)
		offset = offset*10 + (labelOffset[i] - '0');
	if(isNegative)
		offset *= -1;
	bitset <20> imm(offset);

	//Generating machine code
	for(int i=0; i<7; i++)
		machineCode[i]=opcode[index][6-i]-'0';
	for(int i=0; i<5; i++)
		machineCode[7+i] = rd[i];
	for(int i=0; i<8; i++)
		machineCode[12+i] = imm[i+11];
	machineCode[20] = imm[10];
	machineCode[31] = imm[19];
	for(int i=0; i<11; i++)
		machineCode[21+i] = imm[i];
	return machineCode;
}
