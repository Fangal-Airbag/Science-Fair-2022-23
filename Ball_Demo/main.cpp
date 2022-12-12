#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "SerialClass.h"

int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
            x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
            x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
            x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
            x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

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
	TTF_Init();

	SDL_Window* window = SDL_CreateWindow("Ball Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_FULLSCREEN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Event e;

    SDL_Rect border1 = { 0, 0, 400, 300 };
    SDL_Rect border2 = { 490, 60, 440, 360 };
    SDL_Rect border3 = { 0, 420, 930, 300 };
    SDL_Rect border4 = { 1030, 0, 130, 620 };
    SDL_Rect border5 = { 1160, 0, 120, 420 };
    SDL_Rect goal = { 1160, 300, 120, 120 };

    double xPos = 45, yPos = 720 / 2;

    bool goUp = false;
    bool goDown = false;
    bool goLeft = false;
    bool goRight = false;

    bool isBeat = false;

    int s = 0, ms = 0;

    char gx[256] = "", gy[256] = "", gz[256] = "";

    char timer[1000] = "";

    int x = 0, y = 0, z = 0;

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
            if (goRight) xPos += 2.9f;
            if (goLeft) xPos -= 2.9f;
            if (goUp) yPos -= 2.9f;
            if (goDown) yPos += 2.9f;

            // collisions are hard :P
            if ((yPos <= 325) && (xPos <= 425)) yPos += 2.9f;
            if ((yPos >= 395) && (xPos <= 464)) yPos -= 2.9f;
            if (xPos <= 24) xPos += 2.9f;
            if (xPos >= 1255) xPos -= 2.9f;
            if (yPos <= 23) yPos += 2.9f;
            if ((yPos <= 325) && (xPos <= 425)) xPos += 2.9f;
            if ((yPos >= 33) && (xPos >= 463) && (xPos <= 905)) xPos -= 2.9f;
            if ((yPos >= 35) && (xPos >= 463) && (xPos <= 905)) yPos -= 2.9f;
            if (yPos >= 695) yPos -= 2.9f;
            if ((xPos >= 955) && (xPos >= 1005) && (xPos <= 1035) && (yPos <= 625)) xPos -= 2.9f;
            if ((yPos >= 33) && (xPos >= 950) && (xPos <= 955)) xPos += 2.9f;
            if ((xPos >= 1005) && (xPos <= 1125) && (yPos <= 644)) yPos += 2.9f;
            if ((xPos >= 1178) && (xPos <= 1182) && (yPos <= 644)) xPos += 2.9f;
            if ((xPos >= 1178) && (yPos <= 325)) yPos += 2.9f;

            if ((xPos >= 1178) && (yPos <= 445)) isBeat = true;

            ms++;
            SDL_Delay(1);

            if (ms >= 45) {
                s++;
                ms = 0;
            }

            sprintf_s(timer, "%d.%02ds", s, ms);

            SP->WriteData("d", 1);
            int rgx = SP->ReadData(gx, 400);

            SP->WriteData("e", 1);
            int rgy = SP->ReadData(gy, 400);

            SP->WriteData("f", 1);
            int rgz = SP->ReadData(gz, 400);

            gx[rgx] = 0;
            gy[rgy] = 0;
            gz[rgz] = 0;

            std::stringstream gxValue, gyValue, gzValue;
            gxValue << gx;
            gyValue << gy;
            gzValue << gz;

            int intGxValue, intGyValue, intGzValue;
            gxValue >> intGxValue;
            gyValue >> intGyValue;
            gzValue >> intGzValue;

            x += intGyValue / 5500;
            y += intGzValue / 5500;
            z += intGxValue / 5500;

            if (z <= -156185) goLeft = true;
            else goLeft = false;

            if (z >= -156175) goRight = true;
            else goRight = false;

            if (x <= -312375) goDown = true;
            else goDown = false;

            if (x >= -312355) goUp = true;
            else goUp = false;

            /* Background */
            SDL_SetRenderDrawColor(renderer, 205, 205, 205, 255);
            SDL_RenderClear(renderer);

            /* Borders */
            SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
            SDL_RenderFillRect(renderer, &border1);
            SDL_RenderFillRect(renderer, &border2);
            SDL_RenderFillRect(renderer, &border3);
            SDL_RenderFillRect(renderer, &border4);
            SDL_RenderFillRect(renderer, &border5);

            /* Goal */
            SDL_SetRenderDrawColor(renderer, 150, 150, 0, 255);
            SDL_RenderFillRect(renderer, &goal);

            /* Timer */
            Draw_Font(renderer, timer, 10, 10, 200, 70, 300, { 245, 245, 245 });

            /* Ball */
            SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
            SDL_RenderFillCircle(renderer, xPos, yPos, 25);
        }
        else {
            /* Win Screen */
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            Draw_Font(renderer, timer, 550, 300, 200, 70, 300, { 245, 245, 245 });
        }
        
        /* Render */
        SDL_RenderPresent(renderer);
	}

	TTF_Quit();
	SDL_Quit();

	return 0;
}