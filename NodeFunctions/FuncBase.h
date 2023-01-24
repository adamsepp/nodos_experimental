#pragma once

#include <thread>
#include <string>
#include <atomic>

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
};
