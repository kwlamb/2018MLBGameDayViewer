#include "RenderableImage.h"

#include "ImageRenderer.h"

#include <SDL2_Image/SDL_image.h>

#include <iostream>

RenderableImage::RenderableImage(
	const std::string& imageFilePath,
	ImageRenderer* imageRenderer)
	: m_imageFilePath(imageFilePath)
	, m_imageRenderer(imageRenderer)
	, m_imageTexture(NULL)
{}

RenderableImage::~RenderableImage()
{
	Terminate();
}

bool RenderableImage::Init()
{
	m_imageTexture = m_imageRenderer->LoadTexture(m_imageFilePath.c_str());
	if (m_imageTexture == NULL)
	{
		std::cout << "Failed to load texture image for '" << m_imageFilePath.c_str() << "'!" << std::endl;
		return false;
	}

	return true;
}

void RenderableImage::Terminate()
{
	if (m_imageTexture != NULL)
	{
		SDL_DestroyTexture(m_imageTexture);
		m_imageTexture = NULL;
	}
}

bool RenderableImage::Render(SDL_Rect* destinationRect)
{
	SDL_assert(m_imageTexture);

	return m_imageRenderer->RenderImage(m_imageTexture, destinationRect);
}