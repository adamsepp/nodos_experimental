#pragma once

#include "FuncBase.h"

class FuncStartStop : public FuncBase
{
public:

	FuncStartStop(std::string strName);
	~FuncStartStop();
	
	std::string getType() { return "FuncStartStop"; };

private:

	bool execute();
};
