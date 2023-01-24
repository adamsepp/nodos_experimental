#pragma once

#include <thread>
#include <string>
#include <mutex>
#include <atomic>

#include <internal/internal.h>

class FlowWorker
{
public:
	
	FlowWorker(const plano::types::ContextData* context);
	~FlowWorker();
	
	void CyclicWorkerThread();

private:
	
	const plano::types::ContextData* context;
	
	std::thread workerThread;
	std::atomic<bool> workerThreadRunning = false;
	std::atomic<bool> startExecuteCmd = false;
};
