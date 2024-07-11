#include "GraphicsManager.h"

GraphicsManager::GraphicsManager()
{
}

GraphicsManager::~GraphicsManager()
{
}

SDL_Texture *GraphicsManager::loadTexture(const char *filePath, SDL_Renderer *renderer)
{
	SDL_Surface *surf = IMG_Load(filePath);
	if (!surf)
	{
		printf("Failed to load image: %s\n", IMG_GetError());
		return nullptr;
	}

	// Eable color key for transparency
	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0, 0, 0));
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	if (!tex)
	{
		printf("Failed to create texture: %s\n", SDL_GetError());
	}
	return tex;
}
