#pragma once

#include "FuncBase.h"

class FuncWait : public FuncBase
{
public:

	FuncWait(std::string strName);
	~FuncWait();
	
	std::string getType() { return "FuncWait"; };

	int getWaitTime() { return waitTime; };
	bool setWaitTime(int waitTime) { if (waitTime >= 0) { this->waitTime = waitTime; return true; } else return false; };

	double getCurrentTimeDiff() { return currentTimeDiff; };

private:

	bool execute();
	int waitTime = 2000; // in [ms]

	double currentTimeDiff = 0;
};
