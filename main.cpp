#include <SDL.h>
#include "GraphicsManager.h"

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2
	SDL_Window *window = SDL_CreateWindow(
		"Butt",									  // window title
		SDL_WINDOWPOS_UNDEFINED,				  // initial x pos
		SDL_WINDOWPOS_UNDEFINED,				  // initial y pos
		300,									  // width in pixels
		200,									  // height, in pixels
		SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS // flags
	);

	// SDL_SetWindowOpacity(window, 0.0); // Make the window completely transparent

	// Check whether the window was successfully created
	if (window == NULL)
	{
		printf("Window created fail: %s\n", SDL_GetError());
		return 1;
	}

	// Create a renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Enable transparency
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Create an instance of GraphicsManager
	GraphicsManager gfxMan;
	SDL_Texture *iconTex = gfxMan.loadTexture("icon.png", renderer);
	if (iconTex == NULL)
	{
		printf("Failed to load texture: %s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Main loop flag
	bool quit = false;
	SDL_Event e;

	while (!quit)
	{
		// Handle events on queque
		while (SDL_PollEvent(&e) != 0)
		{
			// If user requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		// Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		// Render texture to screen
		SDL_RenderCopy(renderer, iconTex, NULL, NULL);

		// Update screen
		SDL_RenderPresent(renderer);
	}

	// Free loaded image
	SDL_DestroyTexture(iconTex);

	// The window is open
	// SDL_Delay(3000); // Pause execution for 3 seconds

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}
