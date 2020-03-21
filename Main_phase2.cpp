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
#include"ALU.h"
#include"IAG.h"
#include"Decode.h"
#include"Fetch.h"
#include"RegistryFile.h"
#include"RegUpdate.h"
#include"MemoryAccess.h"
#include"InterStateBuffers.h"

using namespace std;

void findLabels(string, vector<string>& , vector<int>&);
void writeBack(InterStateBuffers &, RegUpdate &, Registry_File &);
void print(int i, InterStateBuffers &, Registry_File &);
void printSummary(InterStateBuffers &);


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
  
  Registry_File rFile;
	Fetch fetch;
	Decode decode;
	RegUpdate regUpdate;
	ALU alu;
	IAG iag;
  

	cout<<" Show register value after every cycle ? (y/n)  ";
	char c;
	cin>>c;
	if(c=='y'||c=='Y') 
		isb.printRegFile = true;
	else 
		isb.printRegFile = false;
		
	isb.printISB = false;
	cout<<" Run step by step ? (y/n)  ";
	cin>>c;
	if(c=='y'||c=='Y') 
		runStepByStep = true;
	else 
		runStepByStep = false;

		int i = 0;
		char k;
		while(1)
		{
			i++;
			if(runStepByStep)
			{
				k = 'n';
				while(k != 'r' && k != 'R')
				{
					cout<<"\n RUN CYCLE NUMBER : "<<i<<" ? "<<" ( enter 'r' to run & 'e' to terminate) "<<endl;
					cin>>k;
				}
			}
			
			if(isb.IR.readInt() == 0 || k == 'e' || k == 'E')
				break;

			decode.decoder(isb,rFile);
			alu.compute(isb);
			memory(isb, memAccess, muxy,cache);
			writeBack(isb, regUpdate, rFile);
			iag.step(isb,alu);
			isb.resetAll();

			writeBack(isb, regUpdate, rFile);

		}

		isb.totalCycles = i-1;
		cout<<"\n\n---------------- Code executed succesfully ----------------\n\n"<<endl;
		cout<<" Final register values :\n";	
		rFile.print();
		cout<<" Summary :\n";
		printSummary(isb);

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

void writeBack(InterStateBuffers &isb, RegUpdate &regUpdate, Registry_File &rFile)
{
	if(!isb.enablePipe)
	{
		if(isb.write_back_location != -1)
		{
			regUpdate.update(isb,rFile, isb.write_back_location);
		}
	}
	else
	{
		if(isb.wblocW != -1)
		{
			regUpdate.update(isb,rFile, isb.wblocW);
		}
	}
}
void writeBack(InterStateBuffers &isb, RegUpdate &regUpdate, Registry_File &rFile)
{
		if(isb.write_back_location != -1)
		{
			regUpdate.update(isb,rFile, isb.write_back_location);
		}

}
void print(int i, InterStateBuffers &isb, Registry_File &rFile)
{
	cout<<"===== < Cycle "<<i<<" > ====="<<endl;
	if(isb.printRegFile)
	   rFile.print();
	if(isb.printISB) 
		isb.printAll();
	cout<<endl;
}

void printSummary(InterStateBuffers &isb)
{
	cout<<" Total Cycles \t\t:\t"<<isb.totalCycles<<endl;
}
