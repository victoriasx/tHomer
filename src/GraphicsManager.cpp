#include "GraphicsManager.h"

GraphicsManager::GraphicsManager()
{
}

GraphicsManager::~GraphicsManager()
{
}

SDL_Texture *GraphicsManager::loadTexture(const char *filePath, SDL_Renderer *renderer)
{
	printf("Loading image from path: %s\n", filePath);
	// Ensure the output is flushed to the terminal
	fflush(stdout);

	SDL_Surface *surf = IMG_Load(filePath);
	if (!surf)
	{
		printf("Failed to load image: %s\n", IMG_GetError());
		return nullptr;
	}

	// SDL_SetColorKey( surf , SDL_SRCCOLORKEY, SDL_MapRGB( screen->format, 255, 0, 255) );


	// Eable color key for transparency
	// SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0, 0, 0));
	// Uint32 colorKey = SDL_MapRGB(surf->format, 255, 0, 255);
	Uint32 colorKey = SDL_MapRGB(surf->format, 255, 0, 255);
	SDL_SetColorKey(surf, SDL_TRUE, colorKey);

	// Print surface format
    printf("Surface format: %s\n", SDL_GetPixelFormatName(surf->format->format));
    fflush(stdout);

	// Create a texture from the surface
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf); // Free the surface
	if (!tex)
	{
		printf("Failed to create texture: %s\n", SDL_GetError());
	}

	printf("Successfully loaded texture: %s\n", filePath);
	fflush(stdout);
	return tex;
}
