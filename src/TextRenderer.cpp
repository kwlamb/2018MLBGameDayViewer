#include "TextRenderer.h"

#include <iostream>

TextRenderer::TextRenderer(SDL_Renderer* sdlRenderer)
	: m_SDL_Renderer(sdlRenderer)
	, m_font(NULL)
	, m_fontFileName("fonts/calibri.ttf")
	, m_fontSize(20)
{}

TextRenderer::~TextRenderer()
{
	Terminate();
}

bool TextRenderer::Init()
{
	if (TTF_Init() == -1)
	{
		std::cout << "TTF could not initialize! TTF Error:" << TTF_GetError() << std::endl;
		return false;
	}

	//Open the font
	m_font = 
		TTF_OpenFont(
			m_fontFileName.c_str(),
			m_fontSize);
	
	if (m_font == NULL)
	{
		std::cout << "TTF could not open the font (" << m_fontFileName.c_str() << ")! TTF Error: " << TTF_GetError() << std::endl;
		return false;
	}

	return true;
}

void TextRenderer::Terminate()
{
	if (m_font != NULL)
	{
		TTF_CloseFont(m_font);
		m_font = NULL;
	}

	TTF_Quit();
}

bool TextRenderer::RenderText(
	SDL_Color color,
	const std::string& text,
	SDL_Rect* destinationRect,
	unsigned int wrapLength/* = 0*/)
{
	SDL_Surface* surface;

	if (wrapLength > 0)
	{
		surface =
			TTF_RenderText_Blended_Wrapped(
				m_font, text.c_str(),
				color,
				wrapLength);
	}
	else
	{
		surface =
			TTF_RenderText_Blended(
				m_font,
				text.c_str(),
				color);
	}

	if (surface == NULL)
	{
		std::cout << "Render text surface could not be created! TTF Error : " << TTF_GetError() << std::endl;
		return false;
	}

	SDL_Texture* texture = 
		SDL_CreateTextureFromSurface(
			m_SDL_Renderer,
			surface);
	if (texture == NULL)
	{
		std::cout << "Render text texture could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	int renderCopyReturnValue =
		SDL_RenderCopy(
			m_SDL_Renderer,
			texture,
			NULL,
			destinationRect);

	if (renderCopyReturnValue != 0)
	{
		std::cout << "Render text render copy failed! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
	}

	if (surface != NULL)
	{
		SDL_FreeSurface(surface);
		surface = NULL;
	}

	return true;
}

bool TextRenderer::GetTextRenderDimensions(
	const std::string& text,
	int& textWidth, 
	int& textHeight)
{
	if (TTF_SizeText(m_font, text.c_str(), &textWidth, &textHeight))
	{
		std::cout << "Unable to get the size of the text '" << text.c_str() << "'! TTF Error : " << TTF_GetError() << std::endl;
		return false;
	}

	return true;
}