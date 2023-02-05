#ifndef FUNCNODEREGISTRY_H
#define FUNCNODEREGISTRY_H

#include <plano_api.h>
#include <internal/imgui_stdlib.h> // For 3-arg text box

#include "FuncWait.h"
#include "FuncStartStop.h"

#define ITEM_COLOR_IDLE    (ImVec4)ImColor(128, 128, 128, 255)
#define ITEM_COLOR_STARTUP (ImVec4)ImColor(255, 255, 100, 255)
#define ITEM_COLOR_RUNNING (ImVec4)ImColor(70, 150, 70, 255)
#define ITEM_COLOR_ERROR   (ImVec4)ImColor(255, 100, 100, 255)

using plano::types::PinType;
namespace node_defs
{
	ImColor getStateColor(FuncBase::State state) {
		switch (state) {
		case FuncBase::State::idle: return ITEM_COLOR_IDLE; break;
		case FuncBase::State::running: return ITEM_COLOR_STARTUP; break;
		case FuncBase::State::done: return ITEM_COLOR_RUNNING; break;
		case FuncBase::State::error: return ITEM_COLOR_ERROR; break;
		default: return ITEM_COLOR_IDLE; break;
		}
	}

	namespace FuncWait_
	{
		void Initialize(std::shared_ptr<FuncBase>* function)
		{
			*function = std::make_shared<FuncWait>(""); // user can edit name later in DrawSideEdit()...
		}
		void DrawAndEdit(std::shared_ptr<FuncBase> function, ImColor* nodeColor)
		{
			if (function != nullptr)
				if (function->getType() == "FuncWait") {
					*nodeColor = getStateColor(function->getState());
					ImGui::Text("State = %s", function->getStateStr());
					std::shared_ptr<FuncWait> functionWait = std::dynamic_pointer_cast<FuncWait>(function);
					if (function->getState() == FuncBase::State::running)
						ImGui::Text("Current time %ims", (int)functionWait->getCurrentTimeDiff());
					ImGui::SetNextItemWidth(100); int waitWait = functionWait->getWaitTime();
					if (ImGui::DragInt("Wait time", &waitWait, 100, 0, 100000, "%i ms"))
						functionWait->setWaitTime(waitWait);
				}
				else
				{
					ImGui::TextColored(ITEM_COLOR_ERROR, "ERROR: Wrong function assigned!");
					*nodeColor = ITEM_COLOR_ERROR;
				}
			else
			{
				ImGui::TextColored(ITEM_COLOR_ERROR, "ERROR: No function assigned");
				*nodeColor = ITEM_COLOR_ERROR;
			}
		}
		void DrawSideEdit(std::shared_ptr<FuncBase> function)
		{
			if (function != nullptr) {
				ImGui::SetNextItemWidth(120.0f);
				std::string name = function->getName();
				if (ImGui::InputText("Display Name", &name))
					function->setName(name);

				ImGui::Text("TODO:");
				ImGui::Text(" additional settins for node if needed");
			}
			else
				ImGui::TextColored(ITEM_COLOR_ERROR, "ERROR: No function assigned");
		}
		plano::api::NodeDescription ConstructDefinition(void)
		{
			plano::api::NodeDescription node;
			node.Type = "Wait";
			node.Inputs.push_back(plano::api::PinDescription("Start", plano::types::PinType::Flow));
			node.Inputs.push_back(plano::api::PinDescription("Reset", plano::types::PinType::FlowReset));
			node.Outputs.push_back(plano::api::PinDescription("", plano::types::PinType::Flow));
			node.InitializeDefaultProperties = Initialize;
			node.DrawAndEditProperties = DrawAndEdit;
			node.DrawSideEditProperties = DrawSideEdit;
			return node;
		}
	}

	namespace FuncStartStop_
	{
		void Initialize(std::shared_ptr<FuncBase>* function)
		{
			*function = std::make_shared<FuncStartStop>(""); // user can edit name later in DrawSideEdit()...
		}
		void DrawAndEdit(std::shared_ptr<FuncBase> function, ImColor* nodeColor)
		{
			if (function != nullptr)
				if (function->getType() == "FuncStartStop") {
					*nodeColor = getStateColor(function->getState());
					// ImGui::Text("State = %s", function->getStateStr());
					std::shared_ptr<FuncStartStop> functionStartStop = std::dynamic_pointer_cast<FuncStartStop>(function);
					ImGui::SetNextItemWidth(100); bool started = (functionStartStop->getState() == FuncBase::done);
					if (!started) {
						if (ImGui::Button("Start")) functionStartStop->start();
					}
					else if (ImGui::Button("Stop")) functionStartStop->reset();
				}
				else
				{
					ImGui::TextColored(ITEM_COLOR_ERROR, "ERROR: Wrong function assigned!");
					*nodeColor = ITEM_COLOR_ERROR;
				}
			else
			{
				ImGui::TextColored(ITEM_COLOR_ERROR, "ERROR: No function assigned");
				*nodeColor = ITEM_COLOR_ERROR;
			}
		}
		void DrawSideEdit(std::shared_ptr<FuncBase> function)
		{
			if (function != nullptr) {
				ImGui::SetNextItemWidth(120.0f);
				std::string name = function->getName();
				if (ImGui::InputText("Display Name", &name))
					function->setName(name);

				ImGui::Text("TODO:");
				ImGui::Text(" additional settins for node if needed");
			}
			else
				ImGui::Text("ERROR: No function assigned");
		}
		plano::api::NodeDescription ConstructDefinition(void)
		{
			plano::api::NodeDescription node;
			node.Type = "Start/Stop";
			node.Outputs.push_back(plano::api::PinDescription("", plano::types::PinType::Flow));
			node.InitializeDefaultProperties = Initialize;
			node.DrawAndEditProperties = DrawAndEdit;
			node.DrawSideEditProperties = DrawSideEdit;
			return node;
		}
	}
}
#endif // FUNCNODEREGISTRY
