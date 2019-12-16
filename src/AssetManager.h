#pragma once

#include <map>

#include <iostream>

class ImageRenderer;
class RenderableImage;

class AssetManager
{
public:
	AssetManager(ImageRenderer* imageRenderer);
	~AssetManager();

	bool Init();

	RenderableImage* GetBackgroundRenderableImage();
	RenderableImage* GetGameRenderableImage(const std::string& imageFilePath);
	bool AddGameImage(const std::string& imageFilePath);

private:
	ImageRenderer* m_imageRenderer;
	RenderableImage* m_backgroundImage;
	std::map<std::string, RenderableImage*> m_gameImages;

	void Terminate();
	bool InitRenderableImage(
		RenderableImage** renderableImage,
		const std::string& imageFilePath);
};

