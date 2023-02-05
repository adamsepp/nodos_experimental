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
	resetLinkUsed = false;
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
	state = idle;

	// TODO: clear here all results in derived classes

	// check here if there is any function to be resetted more down the road
	// -> any function to start which has now own reset link should be directly enherit the reset here...
	for (auto functionToStart : functionsToStart) {
		if ((functionToStart->getState() == done) && !functionToStart->getFunctionsResetter().size())
			functionToStart->reset();
	}

	// directly start the function here again, if all input links are still ok
	// TODO: in a very special case it could happen that 2 reset functions are about to reset the same note at the same time -> in this case we need some mutexing...
	if (functionsStarter.size()) {
		bool shouldRestart = true;
		for (auto functionStarter : functionsStarter)
			if (functionStarter->getState() == idle) {
				shouldRestart = false; break;
			}
		if (shouldRestart)
			start();
	}

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
			if (state == running) {
				state = done;

				// check here if there is any function to be started
				for (auto functionToStart : functionsToStart) {
					bool shouldStart = true;
					if (functionToStart->getState() != done) {
						for (auto functionStarter : functionToStart->getFunctionsStarter())
							if (functionStarter->getState() != done) {
								shouldStart = false; break;
							}
						if (shouldStart)
							functionToStart->start();
					}
				}

				// check here if there is any function to be resetted
				for (auto functionToReset : functionsToReset) {
					bool shouldReset = true;
					if (functionToReset->getState() == done) {
						for (auto functionResetter : functionToReset->getFunctionsResetter())
							if (functionResetter->getState() != done) {
								shouldReset = false; break;
							}
						if (shouldReset)
							functionToReset->reset();
					}
				}
			}
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
