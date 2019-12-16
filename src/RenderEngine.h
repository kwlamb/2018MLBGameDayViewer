#pragma once

#include <string>

#include <SDL2/SDL_rect.h>

class AssetManager;
class ImageRenderer;
class TextRenderer;
class UIManager;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;

class RenderEngine
{
public:
	RenderEngine(UIManager* uiManager);
	~RenderEngine();

	bool Init();
	bool Update();

private:
	UIManager* m_uiManager;

	//The window we'll be rendering to
	SDL_Window* m_mainWindow;
	
	//The window renderer
	SDL_Renderer* m_SDL_Renderer;

	TextRenderer* m_textRenderer;
	ImageRenderer* m_imageRenderer;

	AssetManager* m_assetManager;

	const std::string MainWindowTitle;
	const int MainWindowWidth;
	const int MainWindowHeight;
	const int GameImageWidth;
	const int GameImageHeight;
	const int SelectedGameImageWidth;
	const int SelectedGameImageHeight;
	const int SelectedGameX;
	const int SelectedGameY;
	const int GameHeaderTextHeight;
	const int GameFooterTextHeight;
	const int PixelWidthBetweenGames;
	const int PixelWidthBetweenSelectedGameAndHeaderText;
	const int PixelWidthBetweenSelectedGameAndFooterText;

	SDL_Rect m_selectedGameRect;
	SDL_Rect m_selectedGameOutlineRect;
	SDL_Rect m_defaultSelectedGameHeaderTextRect;
	SDL_Rect m_defaultSelectedGameFooterTextRect;
	SDL_Rect m_firstUnselectedLeadingGameRect;
	SDL_Rect m_firstUnslectedTrailingGameRect;

	void Terminate();
	bool InitGameAssets();
	bool InitGameRects();
	void ClearScreen();
	SDL_Rect GetSelectedGameHeaderTextRect(const std::string currentlySelectedGameHeaderText);
	SDL_Rect GetSelectedGameFooterTextRect(const std::string currentlySelectedGameFooterText);
	void RenderGames();
	void RenderSelectedGame();
	void RenderNonSelectedGames();

#ifdef _DEBUG
	void RenderDebugLines();
#endif
};