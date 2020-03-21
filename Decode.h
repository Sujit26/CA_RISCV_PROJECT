#include "RegistryFile.h"

using namespace std;

class Decode{
    private:

    vector <string> instructionName, aluString, relevantstr;

    public:

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
    }
    void decoder(){

    }
};