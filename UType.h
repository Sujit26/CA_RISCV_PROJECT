#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<bitset>  
#include<algorithm>
#include<ctype.h>   // This header file takes the int equivalent of one character as parameter and return an int 

using namespace std;

class UType{

	//instruction <imm[31:12]><rd[11:7]><opcode[6:0]>
private:
	vector <string> instructions;
	vector <string> opcode;

public:
	// initialise the vectors with their respective values from the input file
	void initialise (string filename);
	//checks if given command is present in the list of U Type instructions
	bool isPresent(string command);
	//converts string instruction to 32 bit machine code
	bitset <32> decode (string instruction);

};

void UType::initialise (string filename) {
	ifstream ifile(filename.c_str());
	string instName, instOpcode;
	while(ifile >> instName >> instOpcode){
		instructions.push_back(instName);
		opcode.push_back(instOpcode);
	}

}

bool UType::isPresent (string inst){
	int i=0;
	string instName;
	while(!isalnum(inst[i])) i++;	//isalnum checks whether inst[i] is digit,lower or uppercase letter
	while(isalpha(inst[i])){	//isalpha checks whether inst[i] is an alphabet
		instName.push_back(inst[i]);
		i++;
	}
	vector <string> :: iterator itr = find(instructions.begin(),instructions.end(),instName);
	if(itr == instructions.end())	return false;
	return true;
	
}

bitset <32> UType::decode (string instruction){
	bitset <32> machineCode;
	string instName, rdName, immStr;

	//Splitting instruction into various parts, ignoring whitespaces and commas
	int rdNum=0, immValue=0, i=0;
	while(!isalnum(instruction[i]))	i++;
	while(isalnum(instruction[i])){
		instName.push_back(instruction[i]);
		i++;
	}
	while(!isalnum(instruction[i]))	i++;
	while(isalnum(instruction[i])){
		rdName.push_back(instruction[i]);
		i++;
	}
	while(!isalnum(instruction[i]))	i++;
	while(isalnum(instruction[i])){
		immStr.push_back(instruction[i]);
		i++;
	}
	// index for opcode
	int index = find(instructions.begin(),instructions.end(),instName) - instructions.begin();
	for(int i=0;i<7;i++)
		machineCode[i] = opcode[index][6-i]-'0';

	//Finding register number
	if(rdName.size()==2)
		rdNum = rdName[1]-'0';
	else
		rdNum = 10*(rdName[1]-'0') + (rdName[2]-'0');

	bitset<5> rd(rdNum);   //to convert rdNum to bits

	//Generating immediate value
	if(immStr[0]=='0' && immStr[1]=='x'){	//Hexadecimal input
		for(i=2;i<immStr.size();i++){
			if(immStr[i]>='a' && immStr[i]<='f')
				immValue = immValue*16 + (immStr[i]-'a' + 10);
			else if(immStr[i]>='A' && immStr[i]<='F')
				immValue = immValue*16 + (immStr[i]-'A' + 10);
			else
				immValue = immValue*16 + (immStr[i]-'0');

		}

	}
	else{	//Decimal input
		for(i=0;i<immStr.size();i++)
			immValue = immValue*10 + (immStr[i] - '0');
	}
	if(immValue<0 || immValue>1048575){
		for(int i=0;i<32;i++)
				machineCode[i]=-1;
			return machineCode;
	}
	bitset <20> imm(immValue);  //to convert immValue to bits

	//Generating machine code
	for(int i=0;i<5;i++)
		machineCode[7+i] = rd[i];
	for(int i=0;i<20;i++)
		machineCode[12+i] = imm[i];

	return machineCode;	

}
