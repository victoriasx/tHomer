#include <SDL.h>
#include "GraphicsManager.h"
#include <algorithm>
#include <CoreGraphics/CoreGraphics.h>

// Constants for physics simulation
const float SPRING_CONSTANT = 0.15f;
const float DAMPING_FACTOR = 0.9f;

// Function to get the global mouse position (absolute coordinates on the screen)
void GetGlobalMouseState(int *x, int *y)
{
	CGEventRef event = CGEventCreate(NULL);
	CGPoint location = CGEventGetLocation(event);
	*x = (int)location.x;
	*y = (int)location.y;
	CFRelease(event);
}

// Main function to run the program
int main(int argc, char **argv)
{
	// Initialize SDL2 and check for errors
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
    	printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    	SDL_Quit();
    	return 1;
	}

	SDL_Window *window = SDL_CreateWindow(
		"Butt",									  // window title
		SDL_WINDOWPOS_UNDEFINED,				  // initial x pos
		SDL_WINDOWPOS_UNDEFINED,				  // initial y pos
		150,									  // width in pixels
		150,									  // height, in pixels
		SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP // flags
	);

	// Set window transparency
	// SDL_SetWindowOpacity(window, 0.75f);

	// Check whether the window was successfully created
	if (window == NULL)
	{
		printf("Window created fail: %s\n", SDL_GetError());
		IMG_Quit();
        SDL_Quit();
		return 1;
	}

	 // Hide the mouse cursor
    SDL_ShowCursor(SDL_DISABLE);

	// Create a renderer and check for errors
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); 	// Enable transparency

	GraphicsManager gfxMan; // Create an instance of GraphicsManager
	// Load the homer texture and check for errors
	SDL_Texture *iconTex = gfxMan.loadTexture("../assets/homer.png", renderer);
	if (iconTex == NULL)
	{
		printf("Failed to load homer texture: %s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	// Load the donut texture and check for errors
	SDL_Texture *donutTex = gfxMan.loadTexture("../assets/donut.png", renderer);
	if (donutTex == NULL)
	{
		printf("Failed to load donut texture: %s\n", SDL_GetError());
		SDL_DestroyTexture(iconTex);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	// Homer texture
	int texWidth, texHeight;
	SDL_QueryTexture(iconTex, NULL, NULL, &texWidth, &texHeight);
	texWidth = texWidth/2;
	texHeight = texHeight/2;

	// Donut texture
	int donutWidth, donutHeight;
	SDL_QueryTexture(donutTex, NULL, NULL, &donutWidth, &donutHeight);
	donutWidth = donutWidth/2;
	donutHeight = donutHeight/2;

	// Define the eyes initial pos as a percentage of the window size
	float leftEyeXPercent = 32.0f / 150.0f;
	float leftEyeYPercent = 52.0f / 150.0f;
	float rightEyeXPercent = 56.0f / 150.0f;
	float rightEyeYPercent = 58.0f / 150.0f;
	float eyeRadiusPercent = 5.0f / 150.0f;

	// Initialize physics variables
	float donutPosX = 150.0f; // Start at the center of the screen
	float donutPosY = 150.0f;
	float velX = 0.0f;
	float velY = 0.0f;

	// Handle dragging window
	bool dragging = false;
	int dragOffsetX, dragOffsetY;

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
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					int mouseX, mouseY;
					GetGlobalMouseState(&mouseX, &mouseY);
					int winX, winY;
					SDL_GetWindowPosition(window, &winX, &winY);

					dragging = true;
					dragOffsetX = mouseX - winX;
					dragOffsetY = mouseY - winY;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					dragging = false;
				}
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				if (dragging)
				{
					int mouseX, mouseY;
					GetGlobalMouseState(&mouseX, &mouseY);
					SDL_SetWindowPosition(window, mouseX - dragOffsetX, mouseY - dragOffsetY);
				}
			}
			else if(e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_q)
				{
					quit = true;
				}
			}
		}

		// Get mouse position
		int mouseX, mouseY;
		// SDL_GetMouseState(&mouseX, &mouseY);
		GetGlobalMouseState(&mouseX, &mouseY);

		// Adjust the mouse pos to be relative to the window
		int winX, winY;
		SDL_GetWindowPosition(window, &winX, &winY);
		mouseX -= winX;
		mouseY -= winY;

		// Get the current window size
		int winWidth, winHeight;
		SDL_GetWindowSize(window, &winWidth, &winHeight);

		// Scale the textures based on the window size
		float scaleX = (float)winWidth / 150.0f;
		float scaleY = (float)winHeight / 150.0f;

		int scaledTexWidth = (int)(texWidth * scaleX);
		int scaledTexHeight = (int)(texHeight * scaleY);
		int scaleddonutWidth = (int)(donutWidth * scaleX);
		int scaleddonutHeight = (int)(donutHeight * scaleY);


		/* ------------------------- Calcute donut position ------------------------- */

		// Calculate the desired position for the donut (centered on the mouse cursor)
        float desiredX = mouseX - scaleddonutWidth / 2;
        float desiredY = mouseY - scaleddonutHeight / 2; // Modify Y position based on cursor

		// Calculate the force (spring force towards the desired pos)
		float forceX = (desiredX - donutPosX) * SPRING_CONSTANT;
		float forceY = (desiredY - donutPosY) * SPRING_CONSTANT;

		// Update velocity with damping
        velX = (velX + forceX) * DAMPING_FACTOR;
        velY = (velY + forceY) * DAMPING_FACTOR;

		// Update pos
		donutPosX += velX;
		donutPosY += velY;
		
		// Calculate destination rectangle for rendering the donut
        SDL_Rect donutRect = { (int)donutPosX, (int)donutPosY, scaleddonutWidth, scaleddonutHeight };


		/* ------------------------- Calculate eyes position ------------------------ */

		// Calculate the scaled eye positions
		int leftEyeX = (int)(leftEyeXPercent * winWidth);
		int leftEyeY = (int)(leftEyeYPercent * winHeight);
		int rightEyeX = (int)(rightEyeXPercent * winWidth);
		int rightEyeY = (int)(rightEyeYPercent * winHeight);
		int eyeRadius = (int)(eyeRadiusPercent * winWidth);

		// Calculate the new pos for the left eye
		int newLeftEyeX = leftEyeX + (mouseX - leftEyeX) * 0.1;
		int newLeftEyeY = leftEyeY + (mouseY - leftEyeY) * 0.1;

		// Calculate the new pos for the right eye
		int newRightEyeX = rightEyeX + (mouseX - rightEyeX) * 0.1;
		int newRightEyeY = rightEyeY + (mouseY - rightEyeY) * 0.1;


		// Ensure the left eye pupil stays within the left eye area (bounding box)
		int eyeRadiusForCalc = eyeRadius * 2;
		
		// Define the bouding box of the left eye area
		int leftEyeMinX = leftEyeX - eyeRadiusForCalc + eyeRadius;
		int leftEyeMaxX = leftEyeX + eyeRadiusForCalc - eyeRadius;
		int leftEyeMinY = leftEyeY - eyeRadiusForCalc;
		int leftEyeMaxY = leftEyeY + eyeRadiusForCalc - eyeRadius;

		newLeftEyeX = std::max(leftEyeMinX, std::min(newLeftEyeX, leftEyeMaxX));		
		newLeftEyeY = std::max(leftEyeMinY, std::min(newLeftEyeY, leftEyeMaxY));	
		SDL_Rect leftEyeRect = {winWidth / 2 - scaledTexWidth / 2 + newLeftEyeX, winHeight / 2 - scaledTexHeight / 2 + newLeftEyeY, eyeRadius, eyeRadius};

		// Define the bouding box of the right eye area
		int rightEyeMinX = rightEyeX - eyeRadiusForCalc + eyeRadius;
		int rightEyeMaxX = rightEyeX + eyeRadiusForCalc - eyeRadius;
		int rightEyeMinY = rightEyeY - eyeRadiusForCalc;
		int rightEyeMaxY = rightEyeY + eyeRadiusForCalc - eyeRadius;

		newRightEyeX = std::max(rightEyeMinX, std::min(newRightEyeX, rightEyeMaxX));
		newRightEyeY = std::max(rightEyeMinY, std::min(newRightEyeY, rightEyeMaxY));
		SDL_Rect rightEyeRect = {winWidth / 2 - scaledTexWidth / 2 + newRightEyeX, winHeight / 2 - scaledTexHeight / 2 + newRightEyeY, eyeRadius, eyeRadius};


		/* ----------------------------- Render Content ----------------------------- */

		// Clear screen
		SDL_SetRenderDrawColor(renderer, 254, 249, 231, 255);
		SDL_RenderClear(renderer);

		// Render the static part of Homer at the center
        SDL_Rect centerRect = {winWidth / 2 - scaledTexWidth / 2, winHeight / 2 - scaledTexHeight / 2, scaledTexWidth, scaledTexHeight};
        SDL_RenderCopy(renderer, iconTex, NULL, &centerRect);

		// Render the eyes at the new calculated pos
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black for the eyes

		// Render the left eye
		SDL_RenderFillRect(renderer, &leftEyeRect);

		// Render the right eye
		SDL_RenderFillRect(renderer, &rightEyeRect);

		// Render the moving portion of the texture at the calculated position
        SDL_RenderCopy(renderer, donutTex, NULL, &donutRect);

		// Update screen
		SDL_RenderPresent(renderer);
	}

	// Free loaded image
	SDL_DestroyTexture(iconTex);
	SDL_DestroyTexture(donutTex);

	// The window is open
	// SDL_Delay(3000); // Pause execution for 3 seconds
	SDL_DestroyRenderer(renderer);

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	IMG_Quit();
	SDL_Quit();
	return 0;
}
