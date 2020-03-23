#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>

using namespace std;

class SType{
private:
    vector <string> instructions;
    int error = 0;
// instruction : [ immediate (7) ][ rs2 (5)][rs1 (5)][func3][immediate (5)][opcode (7)]
    vector <string> opcode;
    vector <string> funct3;
    // extract all integers from givenn isntrustion(str)
    // for eg sw x3 0(x7)   extracted 3 0 7 
    vector<int> extractint(string str){
        vector <int> result;
        int sum,currentint;
        int counter = 0;
        for(int strIndex = 0 ; strIndex < str.size() ; strIndex++) {
            
            sum = 0;
            bool intfound = 0;

            while(strIndex < str.size() && isdigit(str[strIndex])) {
                if(counter==2)
                {
                    if(isalpha(str[strIndex]))
                        error = -1;
                }
                currentint = str[strIndex] - '0';
                sum = sum*10 + currentint;
                strIndex++;
                intfound = 1;
            }
            
            if(intfound){
                counter = counter +1;
                result.push_back(sum);}
        }

        return result;
    }
public:
    // initialise the vectors with their respective values from the input file.
    void initialise (string filename) {
        ifstream ifile(filename.c_str());
        string line;
        while (getline(ifile,line)) {
            stringstream ss(line);
            string token; // temp string named as token
            
            ss >> token;
            instructions.push_back(token); // putting instrucion name in our database
            
            ss >> token;
            opcode.push_back(token); // putting opcode to  respective instrucion name in our database
            
            ss >> token;
            funct3.push_back(token); // putting function3 to  respective instrucion name in our database
        }
    }

    // tell weather given command is present in our database or not
    bool isPresent(string command)
    {   
        stringstream ss(command);
        string ins;
        ss >> ins;
        vector <string> :: iterator it = find(instructions.begin(),instructions.end(),ins);
        if(it == instructions.end())
        return false;
        else
        return true;
    }
    
    bitset <32> decode (string instruction) {
        bitset <32> MC;
        stringstream ss(instruction); //helpful for tokenizing space separated strings.
        vector <int> parameters = extractint(instruction); // extracted all register names, offsets etc.
        
        if(parameters.size()!=3||error==-1)
        {
            for(int i=0;i<32;i++)
                MC[i]=-1;
            
                error = 0;
                return MC;
        }
        string action;
        ss >> action;
        int index = find(instructions.begin(),instructions.end(),action) - instructions.begin();
        string opcodestr,funct3str;
        
        opcodestr = opcode[index];
        funct3str = funct3[index];
        bitset <12> immediate(parameters[2]); // loading offset
        bitset <5> rs1(parameters[1]),rs2(parameters[0]);
        if(parameters[2]<-2048||parameters[2]>2047 || parameters[0]<0 || parameters[0]>31 || parameters[1]<0 || parameters[1]>31)
			{
				for(int i=0;i<32;i++)
					MC[i]=-1;
				return MC;
			}
        for(int i=0;i<7;i++){
            if(opcodestr[opcodestr.size()-1-i] == '0')
                MC[i] = 0;
            else 
                MC[i] = 1; //copying opcode string to the opcode field
        }
        for(int i=0;i<5;i++)
            MC[i+7] = immediate[i];

         for(int i = 0; i<3; i++)
            MC[i+12] = (funct3str[funct3str.size()-i-1] == '0') ? 0 : 1;
        
        for(int i=0;i<5;i++)
            MC[i+15] = rs1[i];
        
        for(int i=0; i<5 ; i++)
            MC[i+20] = rs2[i];
        
        for(int i=0;i<7;i++)
            MC[i+25] = immediate[i+5];

        return MC;

    }
};
