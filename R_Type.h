#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

class RType
{
	private:
	vector <string> instructions;  
	vector <string> opcode;
	int error = 0;
    vector <string> funct3;
    vector <string> funct7;

     // instruction : 0->31 
     // |opcode (7) | rd (5) | funct3 | rs1(5) | rs2 (5) | funct7 |


    //Extraction of integers from instruction for Machine code generation
    vector<int> intextraction(string a) 
    {
    	vector<int> final;//Containing all the numeric values of parameters
    	int sum,currentint;
    	for(int index=0 ; index < a.size() ; index++)
    	{
    		sum = 0;
    		int flag =0;
		//handling Error
    		while(index < a.size() && isdigit(a[index]))
    		{
    			currentint =a[index] - '0';
			if(a[index-1]!='x')
				error = -1;
    			sum = sum* 10 + currentint;
    			index++;
    			flag=1;
    		}
    		if(flag=1)
    		{
    			final.push_back(sum);
    		}
    	}
    	return final;
    }


	public:
		void initialise(string parameter)
		{
			ifstream myfile(parameter.c_str());
			string line;

			while(getline(myfile,line))
			{
				stringstream ss(line);
				string stemp;
				ss >> stemp;
				instructions.push_back(stemp);
				ss >> stemp;
				opcode.push_back(stemp);
				ss >> stemp;
				funct3.push_back(stemp);
				ss >> stemp;
				funct7.push_back(stemp);
			}
		}

		//Checking the list of instructions and checking whether its R type or not

		bool ispresent(string line)
		{
			stringstream ss(line);
			string ins;
			ss>>ins;
			int flag=0;
			for(auto i=instructions.begin() ;i<instructions.end() ; i++ )
			{
				if(*i==ins)
				{
					flag=1;
					break;
				}
			}
			if(flag)
				return true;	
			else 
				return false;
		}

		bitset<32> decode(string instruction)
		{
			bitset<32> Machinecode;
			stringstream ss(instruction);
			string action;
			ss >> action;
			vector<int> parameter=intextraction(instruction);
			if(error==-1)
			{
				for(int i=0;i<32;i++)
					Machinecode[i]=-1;
				return Machinecode;
			}
			int index=find(instructions.begin(),instructions.end(),action)-instructions.begin();
			string opcodestr,funct3str,funct7str;
			opcodestr=opcode[index];
			funct3str=funct3[index];
			funct7str=funct7[index];

			bitset<5> rd(parameter[0]);
			bitset<5> rs1(parameter[1]);
			bitset<5> rs2(parameter[2]);

			for(int i=0;i<7;i++)
			{
				Machinecode[i] = (opcodestr[opcodestr.size()-1-i] == '0')?0:1;
			}

				for(int i=0;i<5;i++)
			{
				Machinecode[i+7] = rd[i];
			}
			for(int i=0;i<3;i++)
			{
				Machinecode[i+12] = (funct3str[funct3str.size()-1-i] == '0')?0:1;
			}
				for(int i=0;i<5;i++)
			{
				Machinecode[i+15] = rs1[i];
			}
				for(int i=0;i<5;i++)
			{
				Machinecode[i+20] = rs2[i];
			}
			for(int i=0;i<7;i++)
			{
				Machinecode[i+25] = (funct7str[funct7str.size()-1-i] == '0')?0:1;
			}
			
			return Machinecode;
		}
};
