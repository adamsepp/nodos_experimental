#include "FuncStartStop.h"

FuncStartStop::FuncStartStop(std::string strName)
{
	this->strName = strName;
	init();
}

FuncStartStop::~FuncStartStop()
{
}

bool FuncStartStop::execute()
{
	strError = "";
	// nothing to do here... (yet)
	return(1);
}
