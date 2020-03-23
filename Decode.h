#pragma once
#include "InterStateBuffers.h"
#include "RegistryFile.h"
#include<iostream>
#include<fstream>
#include <sstream>
#include<string.h>
#include<bitset>

using namespace std;

class Decode{
    private:
    vector <string> instructionName, aluString, relevantstr;

    public:
    // Declaring Variables
    bitset<7> opcode;
    bitset<3> func3;
    bitset<7> func7;
    bitset<12> imm1;
    bitset<20> imm2;
    bitset<5> rs1;
    bitset<5> rs2;
    bitset<5> rd;
    int locA, locB, locC;
    bool hasFunc3 = true;
    bool hasFunc7 = true;
    bool stallA = false;
    bool stallB = false;
    bool stallC = false;



    // initialising function
    void initialise() {
        ifstream ifile("ALU.txt");
        string line,temp;
        
        while(getline(ifile,line))
        {
            stringstream ss(line);
            ss >> temp; // temp now has the instruction name ex : add, sub etc,
            instructionName.push_back(temp);
            ss >> temp;
            aluString.push_back(temp); // ALU INSTRUCTION given to the ALU guy
            ss >> temp ; // temp now has the concatenated string. we'll use this to map to name of instruction
            relevantstr.push_back(temp);
        }

        ifile.close();
    }

    //actual decoder
    void decoder(InterStateBuffers &ibs, Registry_File &regFile){
        // initialising actual valies of immediate
        int immVal1;
        int immVal2;

        //initialising class variables
        func3 = -1;
        func7 = -1;
        imm1 = -1;
        imm2 = -1;
        rs1 = 0;
        rs2 = 0;
        rd = 0;

		int insType;
		if(!ibs.enablePipe) insType = ibs.insType;
		else insType = ibs.insTypeD;
        bitset<32> IR(ibs.IR.readInt());


        if(insType == 0){
            opcode = 51;
            func3 = 0;
            func7 = 0;
            rs1 = 0;
            rs2 = 0;
            rd = 0;
            ibs.ALU_OP = "add";
            hasFunc3 = true;
            hasFunc7 = true;
        }
        if(insType == 1){
            // RType | opcode (7) | rd (5) | funct3 | rs1(5) | rs2 (5) | funct7 |
            for(int i=0;i<7;i++){
                opcode[i] = IR[i];
            }
            for(int i=0; i<5; i++){
                rd[i] = IR[7+i];
            }
            for(int i=0; i<3; i++){
                func3[i] = IR[12+i];
            }
            for(int i=0; i<5; i++){
                rs1[i] = IR[15+i];
            }
            for(int i=0; i<5; i++){
                rs2[i] = IR[20+i];
            }
            for(int i=0; i<7; i++){
                func7[i] = IR[25+i];
            }
						
			hasFunc3 = true;
    		hasFunc7 = true;

            ibs.write_back_location = rd.to_ulong();
        } 
        if(insType == 2){
            // IType 0->31 | opcode (7) | rd (5) | funct3 | rs1(5) | immediate(12) |
            for(int i=0;i<7;i++){
                opcode[i] = IR[i];
            }
            for(int i=0; i<5; i++){
                rd[i] = IR[7+i];
            }
            for(int i=0; i<3; i++){
                func3[i] = IR[12+i];
            }
            for(int i=0; i<5; i++){
                rs1[i] = IR[15+i];
            }
            for(int i=0; i<12; i++){
                imm1[i] = IR[20+i];
            }
            hasFunc7 = false;
						hasFunc3 = true;

            ibs.write_back_location = rd.to_ulong();
        }
        if(insType == 3){
            // SBType imm[12] | imm [10:5] | rs2 | rs1 | funct3 | imm[4:1] | imm[11] | opcode
            for(int i=0;i<7;i++){
                opcode[i] = IR[i];
            }
            imm1[10] = IR[7];

            for(int i=0;i<4;i++){
                imm1[i] = IR[8+i];
            }
            for(int i=0;i<3;i++){
                func3[i] = IR[12+i];
            }
            for(int i=0;i<5;i++){
                rs1[i] = IR[15+i];
            }
            for(int i=0;i<5;i++){
                rs2[i] = IR[20+i];
            }
            for(int i=0;i<6;i++){
                imm1[i+4] = IR[25+i];
            }
            imm1[11] = IR[31];
            hasFunc7 = false;
			hasFunc3 = true;
	
            ibs.write_back_location = -1;
        }
        if(insType == 4){
            // SType immediate (7) | rs2 (5) | rs1 (5) | func3 | immediate (5) | opcode (7) |
            // rs1 replaced by rd to symbolize reading on that register, rs2 replaced by rs1 to leave room for writing
            for(int i=0;i<7;i++){
                opcode[i] = IR[i];
            }
            for(int i=0;i<5; i++){
                imm1[i] = IR[7+i];
            }
            for(int i=0; i<3; i++){
                func3[i] = IR[12+i];
            }
            for(int i=0; i<5; i++){
                rd[i] = IR[15+i];
            }
            for(int i=0; i<5; i++){
                rs1[i] = IR[20+i];
            }
            for(int i=0; i<7; i++){
                imm1[i+5] = IR[25+i];
            }
            hasFunc7 = false;
						hasFunc3 = true;

            ibs.write_back_location = -1 ;
        }
        if(insType == 5){
            // UJType imm[20][10:1][11][19:12] | rd[11:7] | opcode[6:0]	
            for(int i=0; i<7;i++){
                opcode[i] = IR[i];
            }
            for(int i=0; i<5; i++){
                rd[i] = IR[7+i];
            }
            for(int i=0; i<8; i++){
                imm2[11+i] = IR[12+i];
            }
            imm2[10] = IR[20];
            for(int i=0; i<10; i++){
                imm2[i] = IR[21+i];
            }
            imm2[19] = IR[31];
            hasFunc7 = false;
            hasFunc3 = false;

            ibs.write_back_location = rd.to_ulong();
        }
        if(insType == 6){
            // UType imm[31:12] | rd[11:7] | opcode[6:0]
            for(int i=0;i<7;i++){
                opcode[i] = IR[i];
            }
            for(int i=0;i<5;i++){
                rd[i] = IR[i+7];
            }
            for(int i=0;i<20;i++){
                imm2[i] = IR[12+i];
            }
            hasFunc7 = false;
            hasFunc3 = false;

            ibs.write_back_location = rd.to_ulong();
        }

        // Also add the same to the register files once made
        locA = rs1.to_ulong();
        locB = rs2.to_ulong();
        locC = rd.to_ulong();

        //Concatenated opcode func3 and func7 and checked for ALU_OP
        string relStr;
        relStr = opcode.to_string();
        if(hasFunc3){
            relStr = relStr + func3.to_string(); 
            if(hasFunc7){
                relStr = relStr + func7.to_string();
            }
            else{
                relStr = relStr + "-1";
            }
        }
        else{
            relStr = relStr + "-1";
        }

				
        //Register file object will be passed and values will be read
        // MUX B Implementation
        //Uncomment the following lines once the register file has been created and update the names.

        //Feeding buffer RA


        if(locA == ibs.pWrite && ibs.pWrite!=0 && ibs.enablePipe == true){
            // if pipelining and data forwarding is true
            if(ibs.enableDF == true){
                if(ibs.pInst == "lw" || ibs.pInst == "lb" || ibs.pInst == "lh"){
                    stallA = true;
                }
                else{
                    stallA = false;
                    ibs.RA.writeInt(ibs.RZ.readInt());
                }
            }
            // if only pipelining is true
            else{
                // ibs.stall the pipeline
                stallA = true;
            }
        }
        else if(locA == ibs.ppWrite && ibs.ppWrite != 0 && ibs.enablePipe == true){
            if(ibs.enableDF == true){
                // for general instruction, no load exceptions are here
                stallA = false;
                ibs.RA.writeInt(ibs.RY.readInt());
            }
            // if only pipelining is true
            else{
                // ibs.stall the pipeline
                stallA = true;
            }
        }
        else{
            stallA = false;
            ibs.RA.writeInt(regFile.readInt(locA));
        }


        //Feeding in RB
        //Negations have been added, in case a new function is made replace it here.

        if(insType == 1 || insType ==3){
            if(locB == ibs.pWrite && ibs.pWrite !=0 && ibs.enablePipe == true){
                if(ibs.enableDF == true){
                    if(ibs.pInst == "lw" || ibs.pInst == "lb" || ibs.pInst == "lh"){
                        stallB = true;
                    }
                    else{
                        stallB = false;
                        ibs.RB.writeInt(ibs.RZ.readInt());
                    }
                }
                // if only pipelining is true
                else{
                    // ibs.stall
                    stallB = true; 
                }
            }
            else if(locB == ibs.ppWrite && ibs.ppWrite != 0 && ibs.enablePipe == true){
                if(ibs.enableDF == true){
                    stallB = false;
                    ibs.RB.writeInt(ibs.RY.readInt());
                }
                else{
                    stallB = true;
                }
            }
            else{
                stallB = false;
                ibs.RB.writeInt(regFile.readInt(locB));
            }
        }
        else if(insType == 2 || insType == 4){
						if(imm1[11] == 0){
                immVal1 = imm1.to_ulong();
            }
            else{
                immVal1 = imm1.flip().to_ulong();
                immVal1++;
                immVal1 = immVal1*-1;
            }
            ibs.RB.writeInt(immVal1);
        }
        else if(insType == 6){
            if(imm2[19] == 0){
                immVal2 = imm2.to_ulong();
            }
            else{
                immVal2 = imm2.flip().to_ulong();
                immVal2++;
                immVal2 = immVal2*-1;
            }
            ibs.RB.writeInt(immVal2);
        }

        if(insType==3){
            if(imm1[11] == 0){
                immVal1 = imm1.to_ulong();
            }
            else{
                immVal1 = imm1.flip().to_ulong();
                immVal1++;
                immVal1 = immVal1*-1;
            }
            ibs.pc_offset = immVal1;
        }
        else if(insType==5){
            if(imm2[19] == 0){
                immVal2 = imm2.to_ulong();
            }
            else{
                immVal2 = imm2.flip().to_ulong();
                immVal2++;
                immVal2 = immVal2*-1;
            }
            ibs.pc_offset = immVal2;
        }
        else{
            ibs.pc_offset = 0;
        }

        ibs.RM = ibs.RMD;

        if(insType == 4){
            if(locC == ibs.pWrite && ibs.pWrite !=0 && ibs.enablePipe == true){
                if(ibs.enableDF == true){
                    if(ibs.pInst == "lw" || ibs.pInst == "lb" || ibs.pInst == "lh"){
                        stallC = true;
                    }
                    else{
                        stallC = false;
                        ibs.RMD.writeInt(ibs.RZ.readInt());
                    }
                }
                else{
                    //ibs.stall
                    stallC = true;
                }
            }
            else if(locC == ibs.ppWrite && ibs.ppWrite !=0 && ibs.enablePipe == true){
                if(ibs.enableDF == true){
                    stallC = false;
                    ibs.RMD.writeInt(ibs.RY.readInt());
                }
                else{
                    //ibs.stall
                    stallC = true;
                }
            }
            else{
                stallC = false;
                ibs.RMD.writeInt(regFile.readInt(locC));
            }
        }
        if(!ibs.enablePipe){
            ibs.RM = ibs.RMD;
        }

        if(ibs.hazard_type == 2){
            int ra = ibs.RA.readInt();
            int rb = ibs.RB.readInt();
            ibs.branch_address = ra + rb;
        }

        //Branch prediction checking
        bool state;
        if(insType == 3){
            int ra = ibs.RA.readInt();
            int rb = ibs.RB.readInt();
            int rau;
            int rbu;
            if(ra >= 0){
                rau = ra;
            }
            else{
                rau = INT_MAX -ra;
            }
            if(rb >= 0){
                rbu = rb;
            }
            else{
                rbu = INT_MAX -rb;
            }

            if(relStr == "1100011000-1"){
                // beq instruction
                state = (ra == rb) ? 1 : 0;
            }
            if(relStr == "1100011001-1"){
                // bne instruction
                state = (ra != rb) ? 1 : 0;
            }
            if(relStr == "1100011100-1"){
                // blt instruction
                state = (ra < rb) ? 1 : 0;
            }
            if(relStr == "1100011101-1"){
                // bge instruction
                state = (ra >= rb) ? 1 : 0;
            }
            if(relStr == "1100011110-1"){
                // bltu instruction
                state = (rau < rbu) ? 1 : 0;
            }
            if(relStr == "1100011111-1"){
                // bgeu instruction
                state = (rau >= rbu) ? 1 : 0;
            }

            if(ibs.taken == true && state == false){
                ibs.mispredNumber++;
                ibs.isMispred = true;
                // Implement flush logic
                // Put ba_def in PC
                ibs.nextPC = ibs.branch_address_def;
            }
            else if(ibs.taken == false && state == true){
                ibs.mispredNumber++;
                ibs.isMispred = true;
                // Implement flush logic
                // Put ba in PC
                ibs.nextPC = ibs.branch_address;
            }
            else{
                //Sab sahi hai bero
                ibs.isMispred = false;
            }

        }

				if((ibs.hazard_type == 1 || ibs.hazard_type == 2) && ibs.enablePipe){
					// locC value save karni hai ie register number
					regFile.writeInt(locC, ibs.returnAddD);

				}

        //Updated ALU_OP

        if(stallA || stallB || stallC){
            ibs.stall = true;
            ibs.numStall++;
        }
        else{
            ibs.stall = false;
        }

        for(int i=0;i<instructionName.size(); i++){
					
            if(relevantstr[i] == relStr){
                ibs.ALU_OP = aluString[i];
                if(instructionName[i]== "jalr"){
                    ibs.isjalr = true;
                }
                else{
                    ibs.isjalr = false;
                }

                ibs.ppInst = ibs.pInst;
                ibs.pInst = instructionName[i];

                if(instructionName[i] == "lb" || instructionName[i] == "lw" || instructionName[i] == "lh" || instructionName[i] == "ld" || instructionName[i] == "lbu" || instructionName[i] == "lhu" ||instructionName[i] == "lwu" ||  ibs.insType == 4){
                            ibs.isMem = true;
                    }
                    else{
                            ibs.isMem = false;									
                }
            }
						
        }

        
	
        // Updating the previous write registers
        // if ibs.stall is activated, feed ibs.pWrite with 0 or insType == 4 ??


        ibs.ppWrite = ibs.pWrite;
        if(insType == 4 || ibs.stall == true){
            ibs.pWrite = 0;
        }
        else{
            ibs.pWrite = locC;
        }
        
    }
};
