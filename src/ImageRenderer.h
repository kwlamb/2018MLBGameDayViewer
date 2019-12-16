#pragma once

#include <string>

struct SDL_Rect;
struct SDL_Renderer;
struct SDL_Texture;

class ImageRenderer
{
public:
	ImageRenderer(SDL_Renderer* sdlRenderer);
	~ImageRenderer();

	bool Init();

	SDL_Texture* LoadTexture(const std::string& imageFilePath);
	bool RenderImage(
		SDL_Texture* texture,
		SDL_Rect* destinationRect);

private:
	SDL_Renderer* m_SDL_Renderer;

	void Terminate();
};