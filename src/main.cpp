#include "EventQueue.h"
#include "GameDayInfo.h"
#include "RenderEngine.h"
#include "UIManager.h"

#include <iostream>

const std::string helpArgumentShortForm = "-h";
const std::string helpArgumentLongForm = "--help";

static void DisplayHelpText();

int main(int argc, char* argv[])
{
	// Default to this date.
	std::string targetDate = "2018-06-10";

	// Check if we have an argument.
	if (argc > 1)
	{
		// Check if the argument is the help argument
		if ((helpArgumentShortForm == argv[1]) || (helpArgumentLongForm == argv[1]))
		{
			DisplayHelpText();
			return EXIT_SUCCESS;
		}
		else
		{
			// If the argument is not the help argument,
			// we treat the argument as the target date.
			targetDate  = "2018-";
			targetDate += argv[1];
		}
	}

	// Create the Game Day info
	GameDayInfo gameDayInfo(targetDate);
	if (!gameDayInfo.Init())
	{
		return EXIT_FAILURE;
	}

	// Create the UI Manager
	UIManager uiManager(&gameDayInfo);

	// Create the render engine
	RenderEngine renderEngine(&uiManager);
	if (!renderEngine.Init())
	{
		return EXIT_FAILURE;
	}

	// Main loop
	while (true)
	{
		if (EventQueue::Process(uiManager) == EventQueue::Status::Shutdown)
		{
			break;
		}

		if (!renderEngine.Update())
		{
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

static void DisplayHelpText()
{
	std::cout << std::endl;
	std::cout << "2018 MLB Game Day Viewer" << std::endl;
	std::cout << std::endl;
	std::cout << "Usage :" << std::endl;
	std::cout << "\t2018MLBGameDayViewer" << std::endl;
	std::cout << "\t2018MLBGameDayViewer [mm-dd]" << std::endl;
	std::cout << "\t2018 MLBGameDayViewer -h | --help" << std::endl;
	std::cout << std::endl;
	std::cout << "Options :" << std::endl;
	std::cout << "\t-h --help   Show this screen." << std::endl;
	std::cout << "\tmm-dd  Target date[default: 06-10]." << std::endl;
}