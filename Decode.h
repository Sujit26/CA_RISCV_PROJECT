#pragma once
#include <bits/stdc++.h>
#include "RegistryFile.h"
#include "InterStateBuffers.h"
#include "RegistryFile.h"

using namespace std;

class Decode{
    private:

    vector <string> instructionName, aluString, relevantstr;

    public:

    
    void decoder(){

    }
};

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
    int pWrite;     // holds the write register value for the previous instruction
    int ppWrite;    // the instruction before that


    // initialising function
    void initialise() {
        ifstream ifile("ALU.txt");
        string line,temp;

        while(getline(ifile,line)){
            stringstream ss(line);
            
            ss >> temp ; //temp contain name (intitials of instructions)
            instructionName.push_back(temp);
            
            ss >> temp; // string corresponding to alu thing
            aluString.push_back(temp);

            ss >> temp;
            relevantstr.push_back(temp);
        }

        ifstream.close()
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

        int insType = ibs.insType;
        bitset<32> IR(ibs.IR.readInt());


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

				
        //Register file object will be passed and values will be read
        // MUX B Implementation
        //Uncomment the following lines once the register file has been created and update the names.

        //Feeding buffer RA
        
        ibs.RA.writeInt(regFile.readInt(locA));
    


        //Feeding in RB
        //Negations have been added, in case a new function is made replace it here.

        
        
        if(insType == 2 || insType == 4){
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

        ibs.RM.writeInt(regFile.readInt(locC));


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

        //Updated ALU_OP
        for(int i=0;i<instructionName.size(); i++){
					
            if(relevantstr[i] == relStr){
                ibs.ALU_OP = aluString[i];
                if(instructionName[i]== "jalr"){
                    ibs.isjalr = true;
                }
                else{
                    ibs.isjalr = false;
                }

                if(instructionName[i] == "lb" || instructionName[i] == "lw" || instructionName[i] == "lh" || instructionName[i] == "ld" || instructionName[i] == "lbu" || instructionName[i] == "lhu" ||instructionName[i] == "lwu" ||  ibs.insType == 4){
                            ibs.isMem = true;
                    }
                    else{
                            ibs.isMem = false;									
                }
            }

						
        }
        
    }
};