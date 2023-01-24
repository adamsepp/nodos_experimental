#include "FlowWorker.h"

FlowWorker::FlowWorker(const plano::types::ContextData* context)
{
	assert(context);
	assert(context->contextMtx);
	this->context = context;
	if (!workerThreadRunning)
		workerThread = std::thread(&FlowWorker::CyclicWorkerThread, this);
}

FlowWorker::~FlowWorker()
{
	if (workerThread.joinable())
	{
		workerThreadRunning = false;
		workerThread.join();
	}
}

void FlowWorker::CyclicWorkerThread()
{
	workerThreadRunning = true;

	while (workerThreadRunning) {
		
		{ // lock mutex
			std::lock_guard<std::mutex> lock(*context->contextMtx); // lock context...
		
			// TODO: this needs to be abstracted... Or better: other methods to not have to search all the time...
			for (auto node : context->s_Nodes) {
				bool shouldStart = false;
				bool shouldNotStart = false;
				std::vector<ax::NodeEditor::LinkId> linkIDs;
				if (node.function != nullptr) {
					if ((node.function->getState() == FuncBase::idle)
					|| (node.function->getState() == FuncBase::done)) {
						for (auto inputLink : node.Inputs) {
							if (inputLink.Type == plano::types::PinType::Flow) {
								for (auto& link : context->s_Links) {
									if (link.EndPinID == inputLink.ID)
										for (auto nodeLinked : context->s_Nodes) // search for ancestor
											for (auto outputLink : nodeLinked.Outputs) {
												if (outputLink.Type == plano::types::PinType::Flow)
													if (link.StartPinID == outputLink.ID)
														if (nodeLinked.function != nullptr)
															if (nodeLinked.function->getState() == FuncBase::done) {
																if (node.function->getState() == FuncBase::idle) {
																	shouldStart = true;
																	linkIDs.push_back(link.ID); // collect for drawing flow
																}
															}
															else
															{
																if (node.function->getState() == FuncBase::done)
																	node.function->reset();
																shouldNotStart = true;
															}
															
															
											}
								}
							}
						}
						if (shouldStart && !shouldNotStart) {
							for (auto linkID : linkIDs)
								ax::NodeEditor::Flow(linkID); // draw all link flows which are responsible for starting this node
							node.function->start();
						}
					}
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1)); // reduce CPU usage...
	}
}
