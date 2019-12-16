#include "ImageRenderer.h"

#include <SDL2_Image/SDL_image.h>

#include <iostream>

ImageRenderer::ImageRenderer(SDL_Renderer* sdlRenderer)
	: m_SDL_Renderer(sdlRenderer)
{}

ImageRenderer::~ImageRenderer()
{
	Terminate();
}

bool ImageRenderer::Init()
{
	if (!(IMG_Init(IMG_INIT_JPG) && IMG_INIT_JPG))
	{
		std::cout << "Failed to initialize the image renderer: " << IMG_GetError() << std::endl;
		return false;
	}

	return true;
}

void ImageRenderer::Terminate()
{
	IMG_Quit();
}

SDL_Texture* ImageRenderer::LoadTexture(const std::string& imageFilePath)
{
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(imageFilePath.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << "Unable to load image " << imageFilePath.c_str() << "! SDL_image Error: '" << IMG_GetError() << "'" << std::endl;
		return NULL;
	}

	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(m_SDL_Renderer, loadedSurface);
	if (newTexture == NULL)
	{
		std::cout << "Unable to create texture from " << imageFilePath.c_str() << "! SDL Error: " << SDL_GetError() << std::endl;
		return NULL;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	return newTexture;
}

bool ImageRenderer::RenderImage(
	SDL_Texture* texture,
	SDL_Rect* destinationRect)
{
	int renderCopyReturnValue =
		SDL_RenderCopy(
			m_SDL_Renderer,
			texture,
			NULL,
			destinationRect);
	if (renderCopyReturnValue != 0)
	{
		std::cout << "SDL_RenderCopy failed! SDL Error :" << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}