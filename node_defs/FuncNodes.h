#ifndef FUNCNODEREGISTRY_H
#define FUNCNODEREGISTRY_H

#include <plano_api.h>
#include <internal/imgui_stdlib.h> // For 3-arg text box

#include "FuncWait.h"
#include "FuncStartStop.h"

using plano::types::PinType;
namespace node_defs
{
	namespace FuncWait_
	{
		void Initialize(std::shared_ptr<FuncBase>* function)
		{
			*function = std::make_shared<FuncWait>("NodeFuncWait"); // TODO: better naming? -> or later adjustable by user???
		}
		void DrawAndEdit(std::shared_ptr<FuncBase> function)
		{
			if(function != nullptr)
				if (function->getType() == "FuncWait") {
					ImGui::Text("State = %s", function->getStateStr());
					std::shared_ptr<FuncWait> functionWait = std::dynamic_pointer_cast<FuncWait>(function);
					if (function->getState() == FuncBase::State::running)
						ImGui::Text("Current time %ims", (int)functionWait->getCurrentTimeDiff());
					ImGui::SetNextItemWidth(100); int waitWait = functionWait->getWaitTime();
					if (ImGui::DragInt("Wait time", &waitWait, 1, 0, 100000, "%i ms"))
						functionWait->setWaitTime(waitWait);
				}
				else
				{
					ImGui::Text("ERROR: Wrong function assigned");
				}
			else
				ImGui::Text("ERROR: No function assigned");
		}
		plano::api::NodeDescription ConstructDefinition(void)
		{
			plano::api::NodeDescription node;
			node.Type = "Wait";
			node.Color = ImColor(128, 195, 248);
			node.Inputs.push_back(plano::api::PinDescription("", PinType::Flow));
			node.Outputs.push_back(plano::api::PinDescription("", PinType::Flow));
			node.InitializeDefaultProperties = Initialize;
			node.DrawAndEditProperties = DrawAndEdit;
			return node;
		}
	}

	namespace FuncStartStop_
	{
		void Initialize(std::shared_ptr<FuncBase>* function)
		{
			*function = std::make_shared<FuncStartStop>("NodeFuncStartStop"); // TODO: better naming? -> or later adjustable by user???
		}
		void DrawAndEdit(std::shared_ptr<FuncBase> function)
		{
			if (function != nullptr)
				if (function->getType() == "FuncStartStop") {
					// ImGui::Text("State = %s", function->getStateStr());
					std::shared_ptr<FuncStartStop> functionStartStop = std::dynamic_pointer_cast<FuncStartStop>(function);
					ImGui::SetNextItemWidth(100); bool started = functionStartStop->getStarted();
					if (!started) {
						if (ImGui::Button("Start")) functionStartStop->setStarted(true);
					}
					else {
						if (ImGui::Button("Stop")) functionStartStop->setStarted(false);
					}
				}
				else
				{
					ImGui::Text("ERROR: Wrong function assigned");
				}
			else
				ImGui::Text("ERROR: No function assigned");
		}
		plano::api::NodeDescription ConstructDefinition(void)
		{
			plano::api::NodeDescription node;
			node.Type = "Start/Stop";
			node.Color = ImColor(128, 195, 248);
			node.Outputs.push_back(plano::api::PinDescription("", PinType::Flow));
			node.InitializeDefaultProperties = Initialize;
			node.DrawAndEditProperties = DrawAndEdit;
			return node;
		}
	}
}
#endif // FUNCNODEREGISTRY
