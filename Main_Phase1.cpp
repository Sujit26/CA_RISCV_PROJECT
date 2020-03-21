#include<iostream>
#include<fstream>
#include<sstream>
#include<string.h>
#include<bitset>
#include "R_Type.h"
#include "IType.h"
#include "SBType.h"
#include "SType.h"
#include "UJType.h"
#include "UType.h"

using namespace std;

void findLabels(string, vector<string>& , vector<int>&);


int main()
{	

	IType iTypeInsObj;
	RType rTypeInsObj;
	SBType sbTypeInsObj;
        SType sTypeInsObj;
	UJType ujTypeInsObj;
	UType uTypeInsObj;

	vector<string> labelNames;

	vector<int> labelLineNumber;

	int insType;

	string inputFileName = "input1.txt";
	string outputFileName = "machineCode.txt";
	sring basicCodeFileName = "basicCode.txt";

	string dir = "./instructions/";

	//change name accordingly
	iTypeInsObj.intialise(dir+"IType.txt");
	rTypeInsObj.initialise(dir + "RType.txt");
	iTypeInsObj.initialise(dir + "IType.txt");
	sbTypeInsObj.initialise(dir + "SBType.txt");
        sTypeInsObj.initialise(dir + "SType.txt");
	ujTypeInsObj.initialise(dir + "UJType.txt");
	uTypeInsObj.initialise(dir + "UType.txt");

	ifstream iFile(inputFileName.c_str(), ios :: in);
	ofstream oFile(outputFileName.c_str());
	ofstream oFile2(basicCodeFileName.c_str());

	findLabels(inputFileName, labelNames, labelLineNumber);

	if(!iFile.is_open()) 
		cout<<"Error in reading input file";
	else
	{
		int lineNo =0;
		bitset<32>machineCode;
		string line;

		while(getline(iFile,line))
		{

			size_t found = line.find(':');

			if(found!=string::npos)
				continue;

			lineNo++;

			//replacing sp with x2
			for(int i=1;i<line.size()-2;i++)
			{
				if(line[i]=='s'&& line[i+1]=='p' && (line[i-1]==' '|| line[i-1]==' ,' || line[i-1]=='(')&&line[i+2]==' '||line[i+2]==','||line[i+2]==')'||line[i+2]=='\n')
				{
					line[i]='x';
					line[i+1]='2';
				}
			}

			//replacing label values
			for(int i=0;i<labelNames.size();i++)
			{
				size_t found = line.find(labelNames[i]);

				if(found!= string::npos)
				{
					string newline;
					for(int j=0;j<found;j++)
					{
						newline.push_back(line[j]);
					}
					int offset = labelLineNumber[i]-lineNo-i;
					ostringstream numStr;
					numStr<<offset;

					strig intStr = numStr.str();

					line = newline + intStr;
				}
			}

			if(iTypeInsOBJ.isPresent(line))
			{
				machineCode = iTypeInsObj.decode(line);
				insType = 2;
			}
			else if(rTypeInsObj.isPresent(line)) 
			{
				machineCode = rTypeInsObj.decode(line);
				insType = 1;
			}
			else if(sbTypeInsObj.isPresent(line)) 
			{
				machineCode = sbTypeInsObj.decode(line);
				insType = 3;
			}
			else if(sTypeInsObj.isPresent(line)) 
			{
				machineCode = sTypeInsObj.decode(line);
				insType = 4;
			}
			else if(ujTypeInsObj.isPresent(line)) 
			{
				machineCode = ujTypeInsObj.decode(line);
				insType = 5;
			}
			else if(uTypeInsObj.isPresent(line)) 
			{
				machineCode = uTypeInsObj.decode(line);
				insType = 6;
			}
			else
			{
				cout<<"ERROR !! Instruction not identified :"<<line<<endl;
				machineCode = bitset<32>(0);
				insType = -1;
			}
			//machine code needs to be converted in hexa;
			//lineNo need to be converted in address of instruction
			oFile <<lineNo<<" "<< machineCode <<" "<< endl;
			oFile2 <<lineNo<<" "<< line << endl;
		}
		
		oFile<<lineNo+1<<" 0 0"<<endl;//ending 
			
	}
	iFile.close();
	oFile.close();
	oFile2.close();

}

void findLabels(string inputFileName, vector<string> &labelNames , vector<int> &labelLineNumber)
{
	ifstrem ifile(inputFileName.c_str());
	if(iFile.is_open())
	{
		int lineNo = 0;
		string line;
		while(getline(iFile,line))
		{
			lineNo++;
			size_t found = line.find(':');
			if(found!=string::npos)
			{
				string labelName;

				int i =0;
				while(!isalnum(line[i])) 
					i++;
				while(line[i]!=':' && i<line.size()) 
					labelName.push_back(line[i++]);
				
				labelNames.push_back(labelName);
				labelLineNumber.push_back(lineNo);
			}
		}
	}
	else
	{
		cout<<"Error!! Opening File in findLabels\n";
	}
	iFile.close();
}
