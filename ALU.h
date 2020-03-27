#pragma once
#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
#include"InterStateBuffers.h"


using namespace std;



class ALU {
	private:
	int RA,RB;
	unsigned RAU,RBU;
    public:
    bool state;
    int result;

    void compute(InterStateBuffers &object) 
    {
        string ins = object.ALU_OP;
		RA = object.RA.readInt(); // reading both values before hand
		RB = object.RB.readInt();
		RAU = object.RA.readInt(); // reading both values before hand
		RBU = object.RB.readInt();

		if(ins == "add"){
			result = RA + RB;
			object.RZ.writeInt(result);
			cout <<"\t\t"<<result <<"="<< RA <<"+"<< RB<<endl;
		}
		else if(ins == "sub"){
			result = RA - RB;
			cout <<"\t\t"<<result <<"="<< RA <<"-"<< RB<<endl;
			object.RZ.writeInt(result);
		}
		else if(ins == "mul"){
			result = RA * RB;
			cout <<"\t\t"<<result <<"="<< RA <<"*"<< RB<<endl;
			object.RZ.writeInt(result);
		}
		else if(ins == "div"){
			result = RA / RB;
			cout <<"\t\t"<<result <<"="<< RA <<"/"<< RB<<endl;
			object.RZ.writeInt(result);
		}
		else if(ins == "rem"){
			result = RA % RB;
			cout <<"\t\t"<<result <<"="<< RA <<"%"<< RB<<endl;
			object.RZ.writeInt(result);
		}
		else if(ins == "and"){
			result = RA & RB;
			cout <<"\t\t"<<result <<"="<< RA <<" and "<< RB<<endl;
			object.RZ.writeInt(result);
		}
		else if(ins == "or"){
			result = RA | RB;
			cout <<"\t\t"<<result <<"="<< RA <<" or "<< RB<<endl;
			object.RZ.writeInt(result);
		}
		else if(ins == "xor"){
			result = RA ^ RB;
			cout <<"\t\t"<<result <<"="<< RA <<" xor "<< RB<<endl;
			object.RZ.writeInt(result);
		}
		else if(ins == "sll"){
			result = RA << RB ;
			cout <<"\t\t"<<result <<"="<< RA <<" << "<< RB<<endl;
			object.RZ.writeInt(result);
		}
		else if(ins == "srl"){
			result = RA >> RB;
			cout <<"\t\t"<<result <<"="<< RA <<" >> "<< RB<<endl;
			object.RZ.writeInt(result);
		}

		else if(ins == "blt"){
			state = (RA < RB) ? 1 : 0;
			cout<<"\t\t"<<state<<" = "<<RA<< " < "<<RB<<endl;
		}
		else if(ins == "ble"){
			state = (RA <= RB) ? 1 : 0;
			cout<<"\t\t"<<state<<" = "<<RA<< " <= "<<RB<<endl;
		}
		else if(ins == "bgt"){
			state = (RA > RB) ? 1 : 0;
			cout<<"\t\t"<<state<<" = "<<RA<< " > "<<RB<<endl;
		}
		else if(ins == "bge"){
			state = (RA >= RB) ? 1 : 0;
			cout<<"\t\t"<<state<<" = "<<RA<< " >= "<<RB<<endl;
		}
		else if(ins == "bltu"){
			cout<<"\t\t"<<state<<" = "<<RA<< " < "<<RB<<endl;
			state = (RAU < RBU) ? 1 : 0;
		}
		else if(ins == "beq"){
			
			state = RA == RB ? 1 : 0;
			cout<<"\t\t"<<state<<" = "<<RA<< " = "<<RB<<endl;
		}
	    	else if(ins == "bne"){
				
			state = RA != RB ? 1 : 0;
			cout<<"\t\t"<<state<<" = "<<RA<< " != "<<RB<<endl;
		}
		else if(ins == "bgtu"){
			state = (RAU > RBU) ? 1 : 0;
			cout<<"\t\t"<<state<<" = "<<RA<< " > "<<RB<<endl;
		}
		else if (ins == "sra"){ //arithmetic shift
			int shift = RB;
			bitset <32> finalresult = RA;
			int size = finalresult.size();
			int MSB = finalresult[size - 1];
			finalresult >> shift;
			for(int index = size - shift; index < size ; index++ )
			{
				finalresult[index] = MSB;
			}
			cout<<"operation:->sra, Value = "<<finalresult.to_ulong()<<endl;
			object.RZ.writeInt(finalresult.to_ulong());
		}
		else{
			cout<<"Sorry instruction "<<ins<< " not found ! "<<endl;
		}	
    }
};
