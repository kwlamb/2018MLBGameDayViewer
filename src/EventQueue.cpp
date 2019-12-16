#include "EventQueue.h"

#include "UIManager.h"

#include <SDL2/SDL.h>

//Event handler
static SDL_Event windowEvent;

namespace EventQueue
{
	Status Process(UIManager& uiManager)
	{
		if (SDL_PollEvent(&windowEvent) == 0)
		{
			// No pending events
			return Status::ReadyToProcess;
		}

		if (windowEvent.type == SDL_QUIT)
		{
			// The user wants to quit the application.
			return Status::Shutdown;
		}

		if (windowEvent.type == SDL_KEYDOWN)
		{
			//Select surfaces based on key press
			switch (windowEvent.key.keysym.sym)
			{
			case SDLK_LEFT:
				uiManager.DecrementCurrentlySelectedGameIndex();
				break;

			case SDLK_RIGHT:
				uiManager.IncrementCurrentlySelectedGameIndex();
				break;

			default:
				break;
			}
		}

		return Status::ReadyToProcess;
	}
}