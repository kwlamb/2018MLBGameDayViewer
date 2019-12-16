#pragma once

class UIManager;

namespace EventQueue
{
	enum class Status
	{
		ReadyToProcess,
		Shutdown,
	};

	Status Process(UIManager& uiManager);
}
