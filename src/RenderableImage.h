#pragma once

#include <string>

class ImageRenderer;
struct SDL_Rect;
struct SDL_Texture;

class RenderableImage
{
public:
	RenderableImage(
		const std::string& imageFilePath,
		ImageRenderer* imageRenderer);
	~RenderableImage();

	bool Init();
	
	bool Render(SDL_Rect* destinationRect);

private:
	std::string m_imageFilePath;
	ImageRenderer* m_imageRenderer;
	SDL_Texture* m_imageTexture;

	void Terminate();
};