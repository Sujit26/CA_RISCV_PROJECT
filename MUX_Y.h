#include <iostream>
#include "InterStateBuffers.h"
using namespace std;

class MUX_Y{
    
    public:
    int MUX_Y_SELECT; // select line from control signal. | 1 for RZ | 2 for memory register| 3 for return address |

    int output(InterStateBuffers &ibf){
        if(MUX_Y_SELECT == 1){
            int RZvalue =  ibf.RZ.readInt();
            return RZvalue;
        }
        else if(MUX_Y_SELECT == 2){
            return ibf.mem_register;
        }
        else if(MUX_Y_SELECT == 3){
						if(!ibf.enablePipe)
	            return ibf.return_address;
						else 
							return ibf.returnAddM;
        }
        else{
            cout<<" the MUX_Y_SELECT IS SET WRONG!"<<endl;
        }
    }
};
