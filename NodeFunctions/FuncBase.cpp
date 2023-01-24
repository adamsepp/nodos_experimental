#include "FuncBase.h"

FuncBase::FuncBase()
{
}

FuncBase::~FuncBase()
{
	if (workerThread.joinable())
	{
		workerThreadRunning = false;
		workerThread.join();
	}
}

void FuncBase::init()
{
	if (!workerThreadRunning)
		workerThread = std::thread(&FuncBase::CyclicWorkerThread, this);
}

bool FuncBase::start()
{
	if (startExecuteCmd)
	{
		strError = getType() + " \"" + getName() + "\": start not possible because function is still busy!";
		return 0;
	}
	strError.clear();
	startExecuteCmd = true;
	return(true);
}

bool FuncBase::reset()
{
	if (startExecuteCmd)
	{
		strError = getType() + " \"" + getName() + "\": reset not possible because function is still busy!";
		return 0;
	}
	strError.clear();

	// TODO: clear here all results in derived classes

	state = idle;
	return(true);
}

void FuncBase::CyclicWorkerThread()
{
	workerThreadRunning = true;

	while (workerThreadRunning) {
		if (startExecuteCmd)
		{
			state = running;
			if (!execute()) state = error;

			startExecuteCmd = false;
		}
		else
		{
			// TODO: set here the state correctly according to the current input state
			//state = idle;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); // reduce CPU usage...
	}
}

bool FuncBase::execute()
{
	strError = "";
	// do some stuff here... --> this is the base class, so nothing will happen here...
	return(1);
}
