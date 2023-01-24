#pragma once

#include "FuncBase.h"

class FuncStartStop : public FuncBase
{
public:

	FuncStartStop(std::string strName);
	~FuncStartStop();
	
	std::string getType() { return "FuncStartStop"; };

	int getStarted() { return started; };
	bool setStarted(bool started) { this->started = started; state = started ? done : idle; return true; };

private:

	bool execute();
	bool started = false;
};
