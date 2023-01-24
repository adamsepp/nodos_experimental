#include "FuncWait.h"

FuncWait::FuncWait(std::string strName)
{
	this->strName = strName;
	init();
}

FuncWait::~FuncWait()
{
}

bool FuncWait::execute()
{
	strError = "";
	std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
	currentTimeDiff = 0;
	while (currentTimeDiff < waitTime)
	{
		currentTimeDiff = (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count();
	}

	state = done;
	return(1);
}
