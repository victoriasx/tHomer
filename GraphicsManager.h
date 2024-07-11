#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include <SDL.h>
#include <SDL_image.h>

class GraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();
	SDL_Texture *loadTexture(const char *filePath, SDL_Renderer *renderer);
};

#endif
