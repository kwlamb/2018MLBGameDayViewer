#pragma once

#include <SDL2_TTF/SDL_ttf.h>

#include <string>

struct SDL_Renderer;

class TextRenderer
{
public:
	TextRenderer(SDL_Renderer* sdlRenderer);
	~TextRenderer();

	bool Init();
	bool RenderText(
		SDL_Color color,
		const std::string& text,
		SDL_Rect* destinationRect,
		unsigned int wrapLength = 0);

	bool GetTextRenderDimensions(
		const std::string& text,
		int& textWidth, 
		int& textHeight);

private:
	SDL_Renderer* m_SDL_Renderer;
	TTF_Font* m_font;
	const std::string m_fontFileName;
	int m_fontSize;

	void Terminate();
};

