#include "AssetManager.h"

#include "RenderableImage.h"

#include <iostream>

AssetManager::AssetManager(ImageRenderer* imageRenderer)
	: m_imageRenderer(imageRenderer)
	, m_backgroundImage(NULL)
{}

AssetManager::~AssetManager()
{
	Terminate();
}

bool AssetManager::Init()
{
	if (!InitRenderableImage(&m_backgroundImage, "images/MLBBackground.jpg"))
	{
		return false;
	}

	return true;
}

void AssetManager::Terminate()
{
	if (m_backgroundImage != NULL)
	{
		delete m_backgroundImage;
		m_backgroundImage = NULL;
	}

	// Delete the game images
	for(std::map<std::string, RenderableImage*>::iterator it = m_gameImages.begin(); it != m_gameImages.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
}

RenderableImage* AssetManager::GetBackgroundRenderableImage()
{
	return m_backgroundImage;
}

bool AssetManager::AddGameImage(const std::string& imageFilePath)
{
	RenderableImage* newRenderableImage = NULL;

	if (!InitRenderableImage(&newRenderableImage, imageFilePath))
	{
		return false;
	}

	m_gameImages[imageFilePath] = newRenderableImage;

	return true;
}

RenderableImage* AssetManager::GetGameRenderableImage(const std::string& imageFilePath)
{
	return m_gameImages[imageFilePath];
}

bool AssetManager::InitRenderableImage(
	RenderableImage** renderableImage,
	const std::string& imageFilePath)
{
	*renderableImage = new RenderableImage(imageFilePath, m_imageRenderer);
	if (*renderableImage == NULL)
	{
		std::cout << "Failed to instantiate renderable image for '" << imageFilePath.c_str() << "'!" << std::endl;
		return false;
	}

	if (!(*renderableImage)->Init())
	{
		return false;
	}

	return true;
}