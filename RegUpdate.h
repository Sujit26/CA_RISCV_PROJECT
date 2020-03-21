#pragma once
#include "RegistryFile.h"
#include "InterStateBuffers.h"

class RegUpdate
{
	public:
		void update(InterStateBuffers & buffer, Registry_file & Reg, int destination)
		{
			Reg.wrteInt(destination, buffer.RY.readInt());
		}
};