#pragma once

#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<bitset>
#include<sstream>

class IType
{
	private:
		vector <string> instructions;
		vector <string> opcode;
		vector <string> funct3;
		//For extracting all the integers that is the value of destination register, source register and Immediate 
		vector<int> extract(string str)
		{	//immediate/rs1/funct3/rd/opcode
			//12/5/3/5/7
				vector<int> result;
				int sum=0;
				for(int i=0;i<str.size();i++)
				{
					sum=0;
					int negative = 0;
					int positive = 0;
					if(str[i]=='-') //For Negative Number
					{
						negative = 1;
						while(i<str.size()&&isdigit(str[i]))
						{
							int p = str[i]-'0';
							sum = (sum*10)+p;
							i=i+1;
						}
						sum=sum*(-1);
					}
					else//For positive Number
					{
						if(i<str.size()&&isdigit(str[i]))
						positive = 1;
						while(i<str.size()&&isdigit(str[i]))
						{
							int p = str[i]-'0';
							sum = (sum*10)+p;
							i=i+1;
						}
					}
					if(positive==1||negative==1)//If found any number then push it into the result
					{
						result.push_back(sum);
					}
				}
			return result;
		}

	public:
	
		void intialise(string filename)
		{
			ifstream ifile(filename.c_str());
			string line;
			while(getline(ifile,line))//Getting all the instructions
			{
				stringstream ss(line);
				string token;
				ss>>token;
				instructions.push_back(token);
				ss>>token;
				opcode.push_back(token);
				ss>>token;
				funct3.push_back(token);
			}
		}

		bool isPresent(string command) 
		{
			stringstream ss(command);
			string ins1;
			ss >> ins1;
			vector <string> :: iterator it = find(instructions.begin(),instructions.end(),ins1);
			if(it == instructions.end())
			return false;
			else
			return true;
  		 }

		bitset<32> decode(string instruction)
		{
			bitset<32>Machine_code;
			bitset<12>immediate;
			bitset<5>rd;
			bitset<5>rs1;

			vector<int>result1 = extract(instruction);
			
			stringstream ins(instruction);
			string instruct,fuct3_1,opcode_1;

			ins>>instruct;
			//Finding our instruction in the file 
			int index = find(instructions.begin(),instructions.end(),instruct)-instrutions.begin();
			//extarcting opcode and funct3 of our instruction 
			opcode_1 = opcode[index];
			funct3_1 = funct3[index];

			rd = result1[0];

			if(instruct[0]=='l'||instruct[0]=='j')
			{
				immediate = result1[1];
				rs1=result1[2];
			}
			else
			{
				immediate = result1[2];
				rs1=result1[1];
			}

			for(int i=0;i<7;i++)
				Machine_code[i] = (opcode_1[opcode_1.size()-1-i] == '0') ? 0 : 1;

			for(int i=0;i<5;i++)
				Machine_code[i+7] = rd[i];

			for(int i=0;i<3;i++)
				Machine_code[i+12]=(funct3_1[funct3_1.size()-i-1] == '0') ? 0 : 1;

			for(int i=0;i<5;i++)
				Machine_code[i+15]=rs1[i];

			for(int i=0;i<12;i++)
				Machine_code[i+20]=immediate[i];

			return Machine_code;
		}
}
