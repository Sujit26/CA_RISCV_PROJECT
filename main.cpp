#include<iostream>
#include<fstream>
#include<sstream>
#include<string.h>
#include<bitset>
#include"RType.h"
#include"IType.h"
#include"SBType.h"
#include"SType.h"
#include"UJType.h"
#include"UType.h"
#include"ALU.h"
#include"IAG.h"
#include"Decode.h"
#include"Fetch.h"
#include"MUX_Y.h"
#include"RegistryFile.h"
#include"RegUpdate.h"
#include"MemoryAccess.h"
#include"InterStateBuffers.h"
#include"Assembler.h"
#include"CacheMemory.h"

using namespace std;

void findLabels(string,vector<string>&,vector<int>&);
void memory(InterStateBuffers &,MemoryAccess & ,MUX_Y &,Cache &);
void writeBack(InterStateBuffers &, RegUpdate &, Registry_File &);
void print(int i, InterStateBuffers &, Registry_File &);
void updateISB(InterStateBuffers &);
void updateAfterDecoder(InterStateBuffers &);
void updateIfStall(InterStateBuffers &);
void printSummary(InterStateBuffers &);

int main(){

	RType rTypeInsObj;
	IType iTypeInsObj;
	SBType sbTypeInsObj;
  	SType sTypeInsObj;
	UJType ujTypeInsObj;
	UType uTypeInsObj;
	InterStateBuffers isb;

	vector<string> labelNames;
	vector<int> labelLineNumber;

	int insType;
	string inputFileName = "input1.txt";
	string outputFileName = "machineCode.txt";
	string basicCodeFileName = "basicCode.txt";

	// Directory path to source instruction text files
	string dir = "./instructions/";

	rTypeInsObj.initialise(dir + "RType.txt");
	iTypeInsObj.initialise(dir + "IType.txt");
	sbTypeInsObj.initialise(dir + "SBType.txt");
  sTypeInsObj.initialise(dir + "SType.txt");
	ujTypeInsObj.initialise(dir + "UJType.txt");
	uTypeInsObj.initialise(dir + "UType.txt");

	MemoryAccess memAccess;

	assembler_initiate(memAccess);
	cout<<"check1\n";
	ifstream iFile(inputFileName.c_str(), ios :: in);cout<<"check2\n";
	ofstream oFile(outputFileName.c_str());cout<<"check3\n";
	ofstream oFile2(basicCodeFileName.c_str());

	cout<<"\n::::::::::  RISC V SIMULATOR :::::::::::::\n\n"<<endl;
	
	//Find All Labels in the input file
	findLabels(inputFileName,labelNames,labelLineNumber);	

	if(!iFile.is_open()) cout<<"Error in reading input file";
	else{

		int lineNo=0;
		bitset<32> machineCode;
		string line;
		while(getline(iFile,line)) {
			// Skip Label descriptions
			size_t found = line.find(':');			
			if(found != string::npos) continue;

			lineNo++;
			
			// replacing sp with x2
			for(int i=1;i<(line.size()-2);i++){
				if(line[i]=='s' &&line[i+1]=='p'&&(line[i-1]==' '||line[i-1]==','||line[i-1]=='(')&&(line[i+2]==' ' ||line[i+2]==','||line[i+2]==')'||line[i+2]=='\n')){
					line[i]='x';
					line[i+1]='2';
				}
			}

			// replacing labels
			for(int i=0;i<labelNames.size();i++){
				size_t found = line.find(labelNames[i]);
				if(found != string::npos){
					string newline;
					for(int j=0;j<found;j++) newline.push_back(line[j]);
					int offset = labelLineNumber[i] - lineNo -i;
				//	offset *= 4; // Multiplying offset by 4
					ostringstream numStr;
					numStr << offset;
					string intStr = numStr.str();
					line = newline + intStr;
				}
			}
		
			if(rTypeInsObj.isPresent(line)) {
				machineCode = rTypeInsObj.decode(line);
				insType = 1;
			}
			else if(iTypeInsObj.isPresent(line)) {
				machineCode = iTypeInsObj.decode(line);
				insType = 2;
			}
			else if(sbTypeInsObj.isPresent(line)) {
				machineCode = sbTypeInsObj.decode(line);
				insType = 3;
			}
			else if(sTypeInsObj.isPresent(line)) {
				machineCode = sTypeInsObj.decode(line);
				insType = 4;
			}
			else if(ujTypeInsObj.isPresent(line)) {
				machineCode = ujTypeInsObj.decode(line);
				insType = 5;
			}
			else if(uTypeInsObj.isPresent(line)) {
				machineCode = uTypeInsObj.decode(line);
				insType = 6;
			}
			else {
				cout<<"ERROR !! Instuction not identified : "<<line<<endl;
				machineCode = bitset<32>(0);
				insType = -1;
			}
			oFile <<lineNo<<" "<< machineCode <<" "<< insType << endl;
			oFile2 <<lineNo<<" "<< line << endl;
		}
		oFile<<lineNo+1<<" 0 0"<<endl;
	}
	iFile.close();
	oFile.close();
	oFile2.close();

	char cacheChoice = 'n';
	// cout<<"Enable cache ? (y/n) : ";
	// cin>>cacheChoice;
	int cs=512,bs=4;
	if(cacheChoice=='y'||cacheChoice=='Y'){
		isb.enableCache = true;
		cout<<" Enter cache size in Bytes (recommended size - 512 B) : ";
		cin>>cs;
		cout<<" Enter cache block size in Bytes (recommended size - 4 B) : ";
		cin>>bs;
	}
	else isb.enableCache = true;

	Registry_File rFile;
	Fetch fetch;
	MUX_Y muxy;
	Decode decode;
	RegUpdate regUpdate;
	ALU alu;
	IAG iag;
	Cache cache(cs,bs,1,1);

	decode.initialise();
		
	// cout<<">>> Modes <<<"<<endl;
	// cout<<"  1) Without Pipelining\n  2) With Pipelining but no data forwarding\n";
	// cout<<"  3) With Pipelining and data forwarding\n\n";
	// cout<<" Enter your choice : ";

	int ch;
	bool runStepByStep = false;
	// cin>>ch;
	ch = 1;
	if(ch == 1){
		isb.enablePipe = false;
		cout<<" Show register value after every cycle ? (y/n)  ";
		char c;
		cin>>c;
		if(c=='y'||c=='Y') isb.printRegFile = true;
		else isb.printRegFile = false;
		isb.printISB = false;
		cout<<" Run step by step ? (y/n)  ";
		cin>>c;
		if(c=='y'||c=='Y') runStepByStep = true;
		else runStepByStep = false;		
	}
	else if(ch == 2){
		isb.enablePipe = true;
		isb.enableDF = false;
		char c;
		cout<<" Show register value after every cycle ? (y/n)  ";
		cin>>c;
		if(c=='y'||c=='Y') isb.printRegFile = true;
		else isb.printRegFile = false;
		cout<<" Show inter state buffer values after every cycle ? (y/n)  ";
		cin>>c;
		if(c=='y'||c=='Y') isb.printISB = true;
		else isb.printISB = false;
	}
	else if(ch == 3) {
		isb.enablePipe = true;
		isb.enableDF = true;
		char c;
		cout<<" Show register value after every cycle ? (y/n)  ";
		cin>>c;
		if(c=='y'||c=='Y') isb.printRegFile = true;
		else isb.printRegFile = false;
		cout<<" Show inter state buffer values after every cycle ? (y/n)  ";
		cin>>c;
		if(c=='y'||c=='Y') isb.printISB = true;
		else isb.printISB = false;
	}
	else{ cout<<" invalid choice !\n"; return 0;}

// If pipeline is disabled
	if(!isb.enablePipe){
		int i = 0;
		char k;
		while(1){
			i++;
			if(runStepByStep){
				k = 'n';
				while(k != 'r' && k != 'R'){
					cout<<"\n RUN CYCLE NUMBER : "<<i<<" ? "<<" ( enter 'r' to run & 'e' to terminate) "<<endl;
					cin>>k;
				}
			}
			fetch.get(isb,rFile);
			if(isb.IR.readInt() == 0 || k == 'e' || k == 'E')
				break;

			decode.decoder(isb,rFile);
			alu.compute(isb);
			memory(isb, memAccess, muxy,cache);
			writeBack(isb, regUpdate, rFile);
			iag.step(isb,alu);
			isb.resetAll();

			if(isb.printRegFile) print(i,isb,rFile);
		}
		isb.totalCycles = i-1;
		cout<<"\n\n---------------- Code executed succesfully ----------------\n\n"<<endl;
		cout<<" Final register values :\n";	
		rFile.print();
		// cout<<" Summary :\n";
		// printSummary(isb);
	}

// If pipeline is enabled with data forwarding
	if(isb.enablePipe && isb.enableDF){
		bool end = false;
		int i = 0,j=0;
		while(1){
			i++;
			
			isb.isMispred = false;
			if(end)
				j++;
			if(j >= 4)
				break;
			
			if(i==1){
				if(!end){
					fetch.get(isb,rFile);
					if(!isb.hazard_type) iag.update(isb);
					else iag.jumpPC(isb,isb.branch_address);
				}
				updateISB(isb);
			}
			else if(i==2) {
				decode.decoder(isb,rFile);
				if(isb.stall){
					updateIfStall(isb);
					continue;
				}
				updateAfterDecoder(isb);
				if(isb.isMispred) iag.jumpPC(isb,isb.nextPC);
				if(!end){
					if(isb.hazard_type == 2){
						iag.jumpPC(isb, isb.branch_address);
					}
					fetch.get(isb,rFile);
					updateISB(isb);
					if(!isb.hazard_type) iag.update(isb);
					else iag.jumpPC(isb,isb.branch_address);
				}
				if(end)	updateISB(isb);
			}
			else if(i==3) {
				if(!isb.stall) alu.compute(isb);
				decode.decoder(isb,rFile);
				if(isb.stall){
					updateIfStall(isb);
					continue;
				}
				updateAfterDecoder(isb);
				if(isb.isMispred) iag.jumpPC(isb,isb.nextPC);
				if(!end){
					if(isb.hazard_type == 2){
						iag.jumpPC(isb, isb.branch_address);
					}
					fetch.get(isb,rFile);
					updateISB(isb);
					if(!isb.hazard_type) iag.update(isb);
					else iag.jumpPC(isb,isb.branch_address);
				}
				if(end)	updateISB(isb);
			}
			else if(i==4) {
				memory(isb, memAccess, muxy,cache);
				if(!isb.stall) alu.compute(isb);
				decode.decoder(isb,rFile);
				if(isb.stall){
					updateIfStall(isb);
					continue;
				}
				updateAfterDecoder(isb);
				if(isb.isMispred) iag.jumpPC(isb,isb.nextPC);
				if(!end){
					if(isb.hazard_type == 2){
						iag.jumpPC(isb, isb.branch_address);
					}
					fetch.get(isb,rFile);
					updateISB(isb);
					if(!isb.hazard_type) iag.update(isb);
					else iag.jumpPC(isb,isb.branch_address);
				}
				if(end)	updateISB(isb);
			}
			else{
				writeBack(isb, regUpdate, rFile);
				memory(isb, memAccess, muxy,cache);
				if(!isb.stall) alu.compute(isb);
				decode.decoder(isb,rFile);
				if(isb.stall){
					updateIfStall(isb);
					continue;
				}
				updateAfterDecoder(isb);
				if(isb.isMispred) iag.jumpPC(isb,isb.nextPC);
				if(!end){
					if(isb.hazard_type == 2){
						iag.jumpPC(isb, isb.branch_address);
					}
					fetch.get(isb,rFile);
					updateISB(isb);
					if(!isb.hazard_type) iag.update(isb);
					else iag.jumpPC(isb,isb.branch_address);
				}
				if(end)	updateISB(isb);
			}
			if(isb.IR.readInt() == 0 )
				end = true;
			if(isb.printRegFile || isb.printISB) print(i,isb,rFile);
		}
		isb.totalCycles = i-1;
		cout<<"\n\n---------------- Code executed succesfully ----------------\n\n"<<endl;
		cout<<" Final register values :\n";	
		rFile.print();
		cout<<" Summary :\n";
		printSummary(isb);
	}

	// If pipeline is enabled without data forwarding
	if(isb.enablePipe && !isb.enableDF){
		bool end = false, skipExecute = false;
		int i = 0,j=0;
		while(1){
			i++;
			isb.isMispred = false;
			if(end)
				j++;
			if(j >= 4)
				break;
			
			if(i==1){
				if(!end){
					fetch.get(isb,rFile);
					if(!isb.hazard_type) iag.update(isb);
					else iag.jumpPC(isb,isb.branch_address);
				}
				updateISB(isb);
			}
			else if(i==2) {
				decode.decoder(isb,rFile);
				if(isb.stall){
					updateIfStall(isb);
					continue;
				}
				updateAfterDecoder(isb);
				if(isb.isMispred) iag.jumpPC(isb,isb.nextPC);
				if(!end){
					if(isb.hazard_type == 2){
						iag.jumpPC(isb, isb.branch_address);
					}
					fetch.get(isb,rFile);
					updateISB(isb);
					if(!isb.hazard_type) iag.update(isb);
					else iag.jumpPC(isb,isb.branch_address);
				}
				if(end)	updateISB(isb);
			}
			else if(i==3) {
				if(!isb.stall) alu.compute(isb);
				decode.decoder(isb,rFile);
				if(isb.stall){
					updateIfStall(isb);
					continue;
				}
				updateAfterDecoder(isb);
				if(isb.isMispred) iag.jumpPC(isb,isb.nextPC);
				if(!end){
					if(isb.hazard_type == 2){
						iag.jumpPC(isb, isb.branch_address);
					}
					fetch.get(isb,rFile);
					updateISB(isb);
					if(!isb.hazard_type) iag.update(isb);
					else iag.jumpPC(isb,isb.branch_address);
				}
				if(end)	updateISB(isb);
			}
			else if(i==4) {
				memory(isb, memAccess, muxy, cache);
				if(!isb.stall) alu.compute(isb);
				decode.decoder(isb,rFile);
				if(isb.stall){
					updateIfStall(isb);
					continue;
				}
				updateAfterDecoder(isb);
				if(isb.isMispred) iag.jumpPC(isb,isb.nextPC);
				if(!end){
					if(isb.hazard_type == 2){
						iag.jumpPC(isb, isb.branch_address);
					}
					fetch.get(isb,rFile);
					updateISB(isb);
					if(!isb.hazard_type) iag.update(isb);
					else iag.jumpPC(isb,isb.branch_address);
				}
				if(end)	updateISB(isb);
			}
			else{
				writeBack(isb, regUpdate, rFile);
				memory(isb, memAccess, muxy, cache);
				if(!isb.stall) alu.compute(isb);
				decode.decoder(isb,rFile);
				if(isb.stall){
					updateIfStall(isb);
					continue;
				}
				updateAfterDecoder(isb);
				if(isb.isMispred) iag.jumpPC(isb,isb.nextPC);
				if(!end){
					if(isb.hazard_type == 2){
						iag.jumpPC(isb, isb.branch_address);
					}
					fetch.get(isb,rFile);
					updateISB(isb);
					if(!isb.hazard_type) iag.update(isb);
					else iag.jumpPC(isb,isb.branch_address);
				}
				if(end)	updateISB(isb);
			}
			if(isb.IR.readInt() == 0 )
				end = true;
			if(isb.printRegFile || isb.printISB) print(i,isb,rFile);
		}
		isb.totalCycles = i-1;
		cout<<"\n\n---------------- Code executed succesfully ----------------\n\n"<<endl;
		cout<<" Final register values :\n";	
		rFile.print();
		cout<<" Summary :\n";
		printSummary(isb);
	}
	return 0;
}

void findLabels(string inputFileName, vector<string> &labelNames, vector<int> &labelLineNumber){
	ifstream iFile(inputFileName.c_str());
	if(iFile.is_open()){
		int lineNo = 0;	
		string line;
		while(getline(iFile,line)) {
			lineNo++;
			size_t found = line.find(':');
			if(found != string::npos){
				string labelName;
				int i=0;
				while(!isalnum(line[i])) i++;
				while(line[i]!=':' && i<line.size()) labelName.push_back(line[i++]);
				labelNames.push_back(labelName);
				labelLineNumber.push_back(lineNo);
			}
		}
	}
	iFile.close();
}

void memory(InterStateBuffers &isb,MemoryAccess &memAccess ,MUX_Y &muxy,Cache &cache){
	if(!isb.enablePipe){
			if(isb.isMem == true){
				if(isb.insType == 4){
					if(isb.enableCache) cache.WriteCache(memAccess,isb,1);
					else memAccess.writeWord(isb);
					muxy.MUX_Y_SELECT = 1;
				}
				else {
					if(isb.enableCache) cache.ReadCache(memAccess,isb,1);
					else memAccess.readWord(isb);
					muxy.MUX_Y_SELECT = 2; // for getting register val from memory
				}
		}
		else if(isb.isjalr == true || isb.insType == 5){
			muxy.MUX_Y_SELECT = 3;
		}
		else
			muxy.MUX_Y_SELECT = 1;
		isb.RY.writeInt(muxy.output(isb));
	}
	else{
		if(isb.isMemM == true){
				if(isb.insTypeM == 4){
					if(isb.enableCache) cache.WriteCache(memAccess,isb,1);
					else memAccess.writeWord(isb);
					muxy.MUX_Y_SELECT = 1;
				}
				else {
					if(isb.enableCache) cache.ReadCache(memAccess,isb,1);
					else memAccess.readWord(isb);
					muxy.MUX_Y_SELECT = 2; // for getting register val from memory
				}
		}
		else if(isb.isjalrM == true || isb.insTypeM == 5){
			muxy.MUX_Y_SELECT = 3;
		}
		else
			muxy.MUX_Y_SELECT = 1;
		isb.RY.writeInt(muxy.output(isb));
	}
}

void writeBack(InterStateBuffers &isb, RegUpdate &regUpdate, Registry_File &rFile){
	if(!isb.enablePipe){
		if(isb.write_back_location != -1){
			regUpdate.update(isb,rFile, isb.write_back_location);
		}
	}
	else{
		if(isb.wblocW != -1){
			regUpdate.update(isb,rFile, isb.wblocW);
		}
	}
}

void updateISB(InterStateBuffers &isb){
	isb.wblocW = isb.wblocM;
	isb.wblocM = isb.wblocE;
	isb.wblocE = isb.wblocD;
	
	isb.returnAddW = isb.returnAddM;
	isb.returnAddM = isb.returnAddE;
	isb.returnAddE = isb.returnAddD;
	isb.returnAddD = isb.return_address;

	isb.insTypeW = isb.insTypeM;
	isb.insTypeM = isb.insTypeE;
	isb.insTypeE = isb.insTypeD;
	isb.insTypeD = isb.insType;

	isb.isjalrW = isb.isjalrM;
	isb.isjalrM = isb.isjalrE;
	isb.isjalrE = isb.isjalrD;

	isb.isMemW = isb.isMemM;
	isb.isMemM = isb.isMemE;
	isb.isMemE = isb.isMemD;
}

void print(int i, InterStateBuffers &isb, Registry_File &rFile){
	cout<<"===== < Cycle "<<i<<" > ====="<<endl;
	if(isb.printRegFile) rFile.print();
	if(isb.printISB) isb.printAll();
	cout<<endl;
}

void printSummary(InterStateBuffers &isb){
	// cout<<" Total Cycles \t\t:\t"<<isb.totalCycles<<endl;
	// cout<<" Total Stalls \t\t:\t"<<isb.numStall<<endl;
	// cout<<" Total Misprediction \t:\t"<<isb.mispredNumber<<endl;
	// cout<<" Total Instruction Cache access count \t:\t"<<isb.accesscount<<endl;
	// cout<<" Total Instruction Cache hit count \t:\t"<<isb.hitcount<<endl;
	// cout<<" Total Instruction Cache cold miss \t:\t"<<isb.coldmiss<<endl;
	// cout<<" Total Data Cache access count \t:\t"<<isb.accesses_data<<endl;
	// cout<<" Total Data Cache hit count \t:\t"<<isb.hits_data<<endl;
	// cout<<" Total Data Cache cold miss \t:\t"<<isb.cold_misses_data<<endl;
	// cout<<" Total Data Cache conflict miss \t:\t"<<isb.conflict_misses_data<<endl;
	// cout<<" Total Data Cache capacity miss \t:\t"<<isb.capacity_misses_data<<endl;
}

void updateAfterDecoder(InterStateBuffers &isb){
	isb.wblocD = isb.write_back_location;
	isb.isjalrD = isb.isjalr;
	isb.isMemD = isb.isMem;
}

void updateIfStall(InterStateBuffers &isb){
	isb.wblocW = isb.wblocM;
	isb.wblocM = isb.wblocE;
	isb.wblocE = -1;		 
	isb.insTypeW = isb.insTypeM;
	isb.insTypeM = isb.insTypeE;
	isb.returnAddW = isb.returnAddM;
	isb.returnAddM = isb.returnAddE;
	isb.isjalrW = isb.isjalrM;
	isb.isjalrM = isb.isjalrE;
	isb.isMemW = isb.isMemM;
	isb.isMemM = isb.isMemE;
}
