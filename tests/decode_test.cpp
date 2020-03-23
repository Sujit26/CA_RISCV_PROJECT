#include"../Decode.h"
#include"../ALU.h"
#include<bits/stdc++.h>

using namespace std;

int main(){
    InterStateBuffers buf;
    Registry_File regFile;
    Decode dec;
    dec.initialise();
    string instStr = "00000000010000010000000100010011";

    bitset<32> inst(instStr);

    buf.IR.writeBitset(inst);
    buf.insType = 2;

    dec.decoder(buf, regFile);

    buf.RA.writeInt(10);
    buf.RB.writeInt(20);
    
    
   
     //write ALU operation here for example : "add" ,"sub" , "xor" etc.
	ALU bufect;
	bufect.compute(buf);
	int result = buf.RZ.readInt(); // result is the value stored in RZ
	int state = bufect.state; // shows the state variable (result of comparison (bool for blt etc.))
	cout<<" the current RZ = "<<result<<" and state : "<<state<<endl; 

    return 0;
}