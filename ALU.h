#pragma once

#include<iostream>
#include<bitset>
#include<fstream>
#include<string>
#include"InterStateBuffers.h"

using namespace std;

class ALU {
	private :
		int RA,RB;
		unsigned RAU,RBU;
	public :
		bool state;
		int result;

		void compute(InterStateBuffers &object){

			string ins = object.ALU_OP;
			RA = object.RA.readInt();
			RB = object.RB.readInt();
			RAU = object.RA.raedInt();
			RBU = object.RB.readInt();

			if(ins == "add"){
				result = RA + RB;
				object.RZ.writeInt(result);
			}
			else if(ins == "sub"){
				result = RA - RB;
				object.RZ.writeInt(result);
			}
			else if(ins == "mul"){
				result = RA * RB;
				object.RZ.writeInt(result);
			}
			else if(ins == "div"){
				result = RA / RB;
				object.RZ.writeInt(result);
			}
			else if(ins == "rem"){
				result = RA % RB;
				object.RZ.writeInt(result);
			}
			else if(ins == "and"){
				result = RA & RB;
				object.RZ.writeInt(result);
			}
			else if(ins == "or"){
				result = RA | RB;
				object.RZ.writeInt(result);
			}
			else if(ins == "xor"){
				result = RA ^ RB;
				object.RZ.writeInt(result);
			}
			else if(ins == "sll"){
				result = RA << RB;
				object.RZ.writeInt(result);
			}
			else if(ins == "srl"){
				result = RA >> RB;
				object.RZ.writeInt(result);
			}
			else if(ins == "blt"){
				state = (RA < RB) ? 1 : 0;
			}
			else if(ins == "ble"){
				state = (RA <= RB) ? 1 : 0;
			}
			else if(ins == "bgt"){
				state = (RA > RB) ? 1 : 0;
			}
			else if(ins == "bge"){
				state = (RA >= RB) ? 1 : 0;
			}
			else if(ins == "bltu"){
				state = (RAU < RBU) ? 1 : 0;
			}
			else if(ins == "beq"){
				state = (RA == RB) ? 1 : 0;
			}
			else if(ins == "bne"){
				state = (RA != RB) ? 1 : 0;
			}
			else if(ins == "bgtu"){
				state = (RAU > RBU) ? 1 : 0;
			}
			else if(ins == "sra"){	//arithmetic shift right
				int shift = RB;
				bitset <32> finalresult = RA;
				int size = finalresult.size();
				int MSB = finalresult[size - 1];  //sign bit
				finalresult >> shift;
				for(int index = size - shift; index < size ; index++){
					fianlresult[index] = MSB;
				}
				object.RZ.writeInt(finalresult.to_ulong());
 
			}
			else {
				cout<<"Sorry instruction "<<ins<<" not found !!"<<endl;
			}
		}
}

