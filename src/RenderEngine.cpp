#include "RenderEngine.h"

#include "AssetManager.h"
#include "ImageRenderer.h"
#include "TextRenderer.h"
#include "RenderableImage.h"
#include "UIManager.h"

#include <SDL.h>
#include <SDL2_Image/SDL_image.h>

#include <iostream>

static SDL_Color s_colorWhite = { 255, 255, 255 };

RenderEngine::RenderEngine(UIManager* uiManager)
	: m_uiManager(uiManager)
	, m_mainWindow(NULL)
	, m_SDL_Renderer(NULL)
	, m_textRenderer(NULL)
	, m_imageRenderer(NULL)
	, MainWindowTitle("DSSTest")
	, MainWindowWidth(1920)
	, MainWindowHeight(1080)
	, GameImageWidth(MainWindowWidth / 10)
	, GameImageHeight(MainWindowHeight / 10)
	, SelectedGameImageWidth((int)(GameImageWidth * 1.5))
	, SelectedGameImageHeight((int)(GameImageHeight * 1.5))
	, SelectedGameX((MainWindowWidth/2) - (SelectedGameImageWidth/2))
	, SelectedGameY((unsigned int)(MainWindowHeight*.33))
	, GameHeaderTextHeight(GameImageHeight/4)
	, GameFooterTextHeight(GameHeaderTextHeight * 15)
	, PixelWidthBetweenGames(50)
	, PixelWidthBetweenSelectedGameAndHeaderText(10)
	, PixelWidthBetweenSelectedGameAndFooterText(10)
{}

RenderEngine::~RenderEngine()
{
	Terminate();
}

bool RenderEngine::Init()
{
	// Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL could not initialize! SDL Errror:" << SDL_GetError() << std::endl;
		return false;
	}

	// Init the main window
	m_mainWindow =
		SDL_CreateWindow(
			"DSS Test",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			MainWindowWidth,
			MainWindowHeight,
			SDL_WINDOW_ALLOW_HIGHDPI);

	if (m_mainWindow == NULL)
	{
		std::cout << "Could not create window: " << SDL_GetError() << std::endl;
		return false;
	}

	//Create renderer for window
	m_SDL_Renderer =
		SDL_CreateRenderer(
			m_mainWindow,
			-1,
			SDL_RENDERER_ACCELERATED);
	if (m_SDL_Renderer == NULL)
	{
		std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	m_textRenderer = new TextRenderer(m_SDL_Renderer);
	if (!m_textRenderer->Init())
	{
		return false;
	}

	m_imageRenderer = new ImageRenderer(m_SDL_Renderer);
	if (!m_imageRenderer->Init())
	{
		return false;
	}

	m_assetManager = new AssetManager(m_imageRenderer);
	if (!m_assetManager->Init())
	{
		return false;
	}

	InitGameAssets();

	InitGameRects();

	return true;
}

void RenderEngine::Terminate()
{
	if (m_assetManager != NULL)
	{
		delete m_assetManager;
		m_assetManager = NULL;
	}

	if (m_imageRenderer != NULL)
	{
		delete m_imageRenderer;
		m_imageRenderer = NULL;
	}

	if (m_textRenderer != NULL)
	{
		delete m_textRenderer;
		m_textRenderer = NULL;
	}

	//Destroy window
	if (m_SDL_Renderer != NULL)
	{
		SDL_DestroyRenderer(m_SDL_Renderer);
		m_SDL_Renderer = NULL;
	}

	if (m_mainWindow != NULL)
	{
		SDL_DestroyWindow(m_mainWindow);
		m_mainWindow = NULL;
	}

	//Quit SDL subsystems
	SDL_Quit();
}

bool RenderEngine::InitGameAssets()
{
	if (!m_uiManager->AreThereGamesToDisplay())
	{
		return true;
	}

	// Add all of the images for all of the games to the asset manager.
	for (unsigned int i = 0; i < m_uiManager->GetTotalNumberOfGamesToDisplay(); ++i)
	{
		const std::string& gameImageFilePath = m_uiManager->GetGameImageFilePath(i);
		m_assetManager->AddGameImage(gameImageFilePath);
	}

	return true;
}

bool RenderEngine::InitGameRects()
{
	// This rect will always be needed.
	m_selectedGameRect =
	{
		SelectedGameX,
		SelectedGameY,
		SelectedGameImageWidth,
		SelectedGameImageHeight
	};

	// This rect will always be needed (unless there is no game data).
	m_selectedGameOutlineRect =
	{
		m_selectedGameRect.x - 3,
		m_selectedGameRect.y - 3,
		m_selectedGameRect.w + 6,
		m_selectedGameRect.h + 6
	};

	m_defaultSelectedGameHeaderTextRect =
	{
		SelectedGameX,
		SelectedGameY - GameHeaderTextHeight - PixelWidthBetweenSelectedGameAndHeaderText,
		SelectedGameImageWidth,
		GameHeaderTextHeight
	};

	m_defaultSelectedGameFooterTextRect =
	{
		SelectedGameX,
		SelectedGameY + SelectedGameImageHeight + PixelWidthBetweenSelectedGameAndFooterText,
		SelectedGameImageWidth,
		GameFooterTextHeight
	};

	m_firstUnselectedLeadingGameRect =
	{
		SelectedGameX - GameImageWidth - PixelWidthBetweenGames,
		SelectedGameY + (SelectedGameImageHeight/2) - (GameImageHeight/2),
		GameImageWidth,
		GameImageHeight
	};

	m_firstUnslectedTrailingGameRect =
	{
		SelectedGameX + SelectedGameImageWidth + PixelWidthBetweenGames,
		SelectedGameY + (SelectedGameImageHeight / 2) - (GameImageHeight / 2),
		GameImageWidth,
		GameImageHeight
	};

	return true;
}

bool RenderEngine::Update()
{
	ClearScreen();

	// Always draw the background.
	m_assetManager->GetBackgroundRenderableImage()->Render(NULL);

	if (m_uiManager->AreThereGamesToDisplay())
	{
		RenderGames();
	}
	else
	{
		m_textRenderer->RenderText(s_colorWhite, "No Game Data", &m_selectedGameRect);
	}

	//Update screen
	SDL_RenderPresent(m_SDL_Renderer);

	return true;
}

void RenderEngine::ClearScreen()
{
	SDL_SetRenderDrawColor(
		m_SDL_Renderer,
		0xFF,
		0xFF,
		0xFF,
		0xFF);
	SDL_RenderClear(m_SDL_Renderer);

}

SDL_Rect RenderEngine::GetSelectedGameHeaderTextRect(const std::string currentlySelectedGameHeaderText)
{
	int textWidth;
	int textHeight;
	
	if (!m_textRenderer->GetTextRenderDimensions(currentlySelectedGameHeaderText, textWidth, textHeight))
	{
		return m_defaultSelectedGameHeaderTextRect;
	}

	// Adjust the text so that it is centered horizontally around the midpoint of the selected game image.
	int defaultRectXMidPoint = m_defaultSelectedGameHeaderTextRect.x + (m_defaultSelectedGameHeaderTextRect.w / 2);
	int adjustedRectX = defaultRectXMidPoint - (textWidth / 2);

	// Adjust the text so that there is a gap between the bottom of the text and the selected game image.
	int adjustedRectY = m_selectedGameRect.y - textHeight - PixelWidthBetweenSelectedGameAndHeaderText;
	
	SDL_Rect selectedGameHeaderTextRect =
	{
		adjustedRectX,
		adjustedRectY,
		textWidth,
		textHeight
	};

	return selectedGameHeaderTextRect;
}

SDL_Rect RenderEngine::GetSelectedGameFooterTextRect(const std::string currentlySelectedGameFooterText)
{
	int textWidth;
	int textHeight;

	if (!m_textRenderer->GetTextRenderDimensions(currentlySelectedGameFooterText, textWidth, textHeight))
	{
		return m_defaultSelectedGameFooterTextRect;
	}

	// The footer text will be wrapped, so we need to extend the rect height so that it can display all of the text.
	// Adjust the text so that it is tall enough to display all of the text 
	int numOfLinesRequired = textWidth / m_defaultSelectedGameFooterTextRect.w;
	if (textWidth % m_defaultSelectedGameFooterTextRect.w)
	{
		numOfLinesRequired++;
	}

	// Each line needs enough space to display the text plus a few pixels of space between the lines.
	// This magic number represents the number that is required to make the wrapped text look correct.
	// There is some issue with the wrapped text functionality that makes the text look squished without this magic number.
	// Obviously, I would like to have a better understanding of what is going on, but for now, I am going to
	// use this magic number to get the wrapped footer text to look good.
	const int MagicNumber = 6;
	int requiredHeight = numOfLinesRequired * (textHeight + MagicNumber);

	SDL_Rect selectedGameHeaderTextRect =
	{
		m_defaultSelectedGameFooterTextRect.x,
		m_defaultSelectedGameFooterTextRect.y,
		m_defaultSelectedGameFooterTextRect.w,
		requiredHeight
	};

	return selectedGameHeaderTextRect;
}

void RenderEngine::RenderGames()
{
	RenderSelectedGame();

	RenderNonSelectedGames();

#ifdef _DEBUG
	RenderDebugLines();
#endif
}

void RenderEngine::RenderSelectedGame()
{
	// It will always appear in the middle of our UI.
	unsigned int currentlySelectedGameIndex = m_uiManager->GetCurrentlySelectedGameIndex();

	// Render the image
	const std::string& currentlySelectedGameImage = m_uiManager->GetGameImageFilePath(currentlySelectedGameIndex);
	m_assetManager->GetGameRenderableImage(currentlySelectedGameImage)->Render(&m_selectedGameRect);

	// Render the header text
	const std::string& currentlySelectedGameHeaderText = m_uiManager->GetGameHeaderText(currentlySelectedGameIndex);
	SDL_Rect selectedGameHeaderTextRect = GetSelectedGameHeaderTextRect(currentlySelectedGameHeaderText);
	m_textRenderer->RenderText(
		s_colorWhite,
		currentlySelectedGameHeaderText,
		&selectedGameHeaderTextRect);

	// Render the footer text
	const std::string& currentlySelectedGameFooterText = m_uiManager->GetGameFooterText(currentlySelectedGameIndex);
	SDL_Rect selectedGameFooterTextRect = GetSelectedGameFooterTextRect(currentlySelectedGameFooterText);
	m_textRenderer->RenderText(
		s_colorWhite,
		currentlySelectedGameFooterText,
		&selectedGameFooterTextRect,
		SelectedGameImageWidth);

	// Render the selection outline.
	SDL_SetRenderDrawColor(
		m_SDL_Renderer,
		0xFF,
		0xFF,
		0xFF,
		0xFF);
	SDL_RenderDrawRect(
		m_SDL_Renderer,
		&m_selectedGameOutlineRect);
}

void RenderEngine::RenderNonSelectedGames()
{
	unsigned int currentlySelectedGameIndex = m_uiManager->GetCurrentlySelectedGameIndex();

	// First, let's display any games to the right of the selected game.
	for (unsigned int i = currentlySelectedGameIndex + 1; i < m_uiManager->GetTotalNumberOfGamesToDisplay(); ++i)
	{
		SDL_Rect unselectedGameRect = m_firstUnslectedTrailingGameRect;
		int gameIndexDiff = i - currentlySelectedGameIndex;
		const std::string& unselectedGameImage = m_uiManager->GetGameImageFilePath(i);
		unselectedGameRect.x += GameImageWidth * (gameIndexDiff - 1);
		unselectedGameRect.x += PixelWidthBetweenGames * (gameIndexDiff - 1);

		if (unselectedGameImage.empty())
		{
			m_textRenderer->RenderText(
				s_colorWhite,
				"NO IMAGE AVAILABLE",
				&unselectedGameRect);
		}
		else
		{
			m_assetManager->GetGameRenderableImage(unselectedGameImage)->Render(&unselectedGameRect);
		}
	}

	// Then, let's display any games to the left of the selected game.
	for (int i = currentlySelectedGameIndex - 1; i >= 0; --i)
	{
		SDL_Rect unselectedGameRect = m_firstUnselectedLeadingGameRect;
		int gameIndexDiff = currentlySelectedGameIndex - i;
		const std::string& unselectedGameImage = m_uiManager->GetGameImageFilePath(i);
		unselectedGameRect.x -= GameImageWidth * (gameIndexDiff - 1);
		unselectedGameRect.x -= PixelWidthBetweenGames * (gameIndexDiff - 1);

		m_assetManager->GetGameRenderableImage(unselectedGameImage)->Render(&unselectedGameRect);
	}
}

#ifdef _DEBUG
void RenderEngine::RenderDebugLines()
{
	// This just draws colored lines along the middle of the x and y axis.

	//Draw blue horizontal line
	SDL_SetRenderDrawColor(
		m_SDL_Renderer,
		0x00,
		0x00,
		0xFF,
		0xFF);
	SDL_RenderDrawLine(
		m_SDL_Renderer,
		0,
		MainWindowHeight / 2,
		MainWindowWidth,
		MainWindowHeight / 2);

	//Draw yellow horizontal line
	SDL_SetRenderDrawColor(
		m_SDL_Renderer,
		0xFF,
		0xFF,
		0x00,
		0xFF);
	SDL_RenderDrawLine(
		m_SDL_Renderer,
		MainWindowWidth / 2,
		0,
		MainWindowWidth / 2,
		MainWindowHeight);
}
#endif