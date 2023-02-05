#pragma once

#include <thread>
#include <string>
#include <atomic>
#include <vector>

class FuncBase abstract
{
public:
	enum State {
		idle, running, done, error
	};
	
	FuncBase();
	~FuncBase();
	
	virtual std::string getType() { return "FuncBase"; };
	std::string getName() { return strName; };
	bool setName(std::string strName) { this->strName = strName; return true; };
	State getState() { return state; };
	const char* getStateStr() {
		switch (state) {
		case idle: return "idle"; break;
		case running: return "running"; break;
		case done: return "done"; break;
		case error: return "error"; break;
		default: return "unkown"; break;
		}
	}
	bool getResetLinkUsed() { return resetLinkUsed; }
	bool setResetLinkUsed(bool resetLinkUsed) { this->resetLinkUsed = resetLinkUsed;  return true; }

	bool clearFunctionLinks() { functionsStarter.clear(); functionsResetter.clear(); functionsToStart.clear(); functionsToReset.clear(); return true; }
	bool addFunctionStarter(std::shared_ptr<FuncBase> functionStarter) { functionsStarter.push_back(functionStarter); return true; }
	std::vector<std::shared_ptr<FuncBase>> getFunctionsStarter() { return functionsStarter; }
	bool addFunctionResetter(std::shared_ptr<FuncBase> functionResetter) { functionsResetter.push_back(functionResetter); return true; }
	std::vector<std::shared_ptr<FuncBase>> getFunctionsResetter() { return functionsResetter; }
	bool addFunctionToStart(std::shared_ptr<FuncBase> functionToStart) { functionsToStart.push_back(functionToStart); return true; }
	std::vector<std::shared_ptr<FuncBase>> getFunctionsToStart() { return functionsToStart; }
	bool addFunctionToReset(std::shared_ptr<FuncBase> functionToReset) { functionsToReset.push_back(functionToReset); return true; }
	std::vector<std::shared_ptr<FuncBase>> getFunctionsToReset() { return functionsToReset; }

	void init();
	bool start();
	bool reset();
	void CyclicWorkerThread();

protected:

	std::string strName = "";
	State state = idle;
	std::string strError = "";

private:

	virtual bool execute();
	std::thread workerThread;
	std::atomic<bool> workerThreadRunning = false;
	std::atomic<bool> startExecuteCmd = false;
	std::atomic<bool> resetLinkUsed = false;

	std::vector<std::shared_ptr<FuncBase>> functionsStarter;
	std::vector<std::shared_ptr<FuncBase>> functionsResetter;
	std::vector<std::shared_ptr<FuncBase>> functionsToStart;
	std::vector<std::shared_ptr<FuncBase>> functionsToReset;
};
