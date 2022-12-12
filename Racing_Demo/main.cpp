#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "SerialClass.h"

void Draw_Font(SDL_Renderer* renderer, const char* str, int x, int y, int width, int height, int size, SDL_Color color) {
	TTF_Font* font = TTF_OpenFont("font.ttf", size);

	SDL_Surface* message_surf = TTF_RenderText_Blended(font, str, color);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, message_surf);
	SDL_Rect Message_rect = { x, y, width, height };
	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

	SDL_DestroyTexture(Message);
	SDL_FreeSurface(message_surf);
	TTF_CloseFont(font);
}

int main(int argc, char** argv)
{
	std::cout << "Hello World!" << std::endl;
	
	bool isRunning = true;

	std::string portChoice = "\\\\.\\";

	std::cout << "Input port: ";
	std::getline(std::cin, portChoice);

	const char* cPortChoice = portChoice.c_str();

	Serial* SP = new Serial(cPortChoice);
	if (SP->IsConnected()) std::cout << "We're connected!" << std::endl;
	
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	SDL_Window* window = SDL_CreateWindow("Racing Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_FULLSCREEN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Event e;

	SDL_Surface* sCar = IMG_Load("car.png");
	SDL_Texture* tCar = SDL_CreateTextureFromSurface(renderer, sCar);
	SDL_FreeSurface(sCar);

	bool isBeat = false;
	
	bool goForward = false;

	double xPos = 10;
	double vel = 0;
	double speed = 0;

	char ax[256] = "", ay[256] = "", az[256] = "";

	char speedometer[1000] = "";

	SDL_ShowCursor(SDL_DISABLE);

	while (isRunning) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT: {
					isRunning = false;
				} break;
				case SDL_KEYDOWN: {
					if (e.key.keysym.sym == SDLK_1) isRunning = false;
				} break;
			}
		}

		if (!isBeat) {
			SDL_Rect road = { 0, 200, 1280, 350 };
			SDL_Rect pav1 = { 20, 350, 100, 50 };
			SDL_Rect pav2 = { 270, 350, 100, 50 };
			SDL_Rect pav3 = { 520, 350, 100, 50 };
			SDL_Rect pav4 = { 770, 350, 100, 50 };
			SDL_Rect pav5 = { 1020, 350, 100, 50 };
			SDL_Rect goal = { 1180, 200, 100, 350 };
			SDL_Rect car = { xPos, 355, 70, 40 };

			SP->WriteData("a", 1);
			int agx = SP->ReadData(ax, 400);

			SP->WriteData("b", 1);
			int agy = SP->ReadData(ay, 400);

			SP->WriteData("c", 1);
			int agz = SP->ReadData(az, 400);

			ax[agx] = 0;
			ay[agy] = 0;
			az[agz] = 0;

			std::stringstream axValue, ayValue, azValue;
			axValue << ax;
			ayValue << ay;
			azValue << az;

			int intAxValue, intAyValue, intAzValue;
			axValue >> intAxValue;
			ayValue >> intAyValue;
			azValue >> intAzValue;

			if (intAxValue >= 6000) {
				goForward = true;
				vel = 2.5f;
				speed = 42.3f;
			}
			else {
				goForward = false;
				speed = 0;
			}

			if (intAxValue >= 10000) {
				goForward = true;
				vel = 5;
				speed = 80.23f;
			}

			if (intAxValue >= 15000) {
				goForward = true;
				vel = 7.5;
				speed = 90;
			}

			if (intAxValue >= 20000) {
				goForward = true;
				vel = 10;
				speed = 136;
			}

			if (intAxValue >= 30000) {
				goForward = true;
				vel = 20;
				speed = 223;
			}

			if (goForward) xPos += vel;

			sprintf_s(speedometer, "%.2f km/h", speed);

			if (xPos >= 1110) isBeat = true;

			/* Background */
			SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
			SDL_RenderClear(renderer);

			/* Road */
			SDL_SetRenderDrawColor(renderer, 88, 88, 88, 255);
			SDL_RenderFillRect(renderer, &road);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &road);

			/* Road Pavements */
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderFillRect(renderer, &pav1);
			SDL_RenderFillRect(renderer, &pav2);
			SDL_RenderFillRect(renderer, &pav3);
			SDL_RenderFillRect(renderer, &pav4);
			SDL_RenderFillRect(renderer, &pav5);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &pav1);
			SDL_RenderDrawRect(renderer, &pav2);
			SDL_RenderDrawRect(renderer, &pav3);
			SDL_RenderDrawRect(renderer, &pav4);
			SDL_RenderDrawRect(renderer, &pav5);

			/* Goal */
			SDL_SetRenderDrawColor(renderer, 150, 150, 0, 255);
			SDL_RenderFillRect(renderer, &goal);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &goal);

			Draw_Font(renderer, speedometer, 10, 10, 150, 70, 100, { 255, 255, 255 });

			/* Car */
			SDL_RenderCopy(renderer, tCar, NULL, &car);
		}
		else {
			/* Win screen */
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			Draw_Font(renderer, "You won! Wasn't sure what else to put here...", 350, 300, 600, 70, 50, { 245, 245, 245 });
		}
		
		/* Render */
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(tCar);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}