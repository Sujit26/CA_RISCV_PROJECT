#pragma once
#include "RegistryFile.h"
#include "InterStateBuffers.h"

class RegUpdate {
public:
	void update (InterStateBuffers & buffer, Registry_File & Reg,int destination ) { // 
		Reg.writeInt( destination, buffer.RY.readInt() );	
	}
};
