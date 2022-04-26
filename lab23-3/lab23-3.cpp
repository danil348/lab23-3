#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include "TextureManager.h"
#include <Windows.h>
#include "TextManager.h"

using namespace std;

SDL_Renderer* TextureManager::renderer = nullptr;

class GameObgect
{
public:
	void setPosX_Y(int screenW, int screenH, int tile_w, int tile_h);
	void setMainTexture(SDL_Texture* value);
	void setSecondTexture(SDL_Texture* value);
	void setSrcDest_W_H(int src_w, int src_h, int dest_w, int dest_h);
	void setSrcDest_X_Y(int src_x, int src_y, int dest_x, int dest_y);
	void setType(int type);

	void chengeMainTexture();

	SDL_Texture* getMainTexture();
	SDL_Texture* getSecondTexture();
	int getScore();

	SDL_Rect src, dest;
	int posX, posY;
	bool isOpen = false;
	int score;
private:
	SDL_Texture* maintx;
	SDL_Texture* second;
	int type;
};

bool intersection(GameObgect gameObgect, int x, int y);

struct Key
{
	int mouseX;
	int mouseY;
};

bool hasTile(int number, GameObgect* gameObgect, int gameObgectCount) {
	for (int i = 0; i < gameObgectCount; i++) {
		if (i != number && gameObgect[i].posX == gameObgect[number].posX && gameObgect[i].posY == gameObgect[number].posY) {
			return 1;
		}
	}
	return 0;
}


int main(int argc, char** argv)
{
	setlocale(LC_ALL, "RU");
	srand(time(0));

	TextureManager textureManager;
	TextManager textManager;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL Error: %S", SDL_GetError());
	}
	else if (IMG_Init(IMG_INIT_PNG) == 0) {
		printf("IMG Error: %S", SDL_GetError());
	}
	else {
		printf("Initialised!\n");
		SDL_Window* window = SDL_CreateWindow("bushes", 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, SDL_WINDOW_FULLSCREEN_DESKTOP, 1);
		if (window) {
			printf("Window created!\n");
		}
		SDL_SetWindowFullscreen(window, 1);
		textureManager.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (textureManager.renderer) {
			SDL_SetRenderDrawColor(textureManager.renderer, 255, 255, 255, 255);
			printf("Renderer created!\n");
		}

	}

	int screenW, screenH;
	SDL_GetRendererOutputSize(textureManager.renderer, &screenW, &screenH);

	SDL_Rect src, dest;
	src = { 0,0,860,695 };
	dest = { 0,0,screenW, screenH };
	SDL_Texture* fon;
	fon = TextureManager::LoadTexture("assets/fon.png");
	Key key;
	SDL_Event event;
	GameObgect* bushes;
	int bushesCount = screenW / 100 * screenH / 100 / 5.5;
	int bushesCountNotOpen = bushesCount;
	bushes = new GameObgect[bushesCount];
	int type;
	int score = 0;
	for (int i = 0; i < bushesCount; i++) {
		type = rand() % 11+1;
		if (type == 1) {
			bushes[i].setType(1);
			bushes[i].setSecondTexture(TextureManager::LoadTexture("assets/bon.png"));
		}
		else if (type > 1 && type < 9) {
			bushes[i].setType(2);
			bushes[i].setSecondTexture(TextureManager::LoadTexture("assets/grib.png"));
		}
		else {
			bushes[i].setType(3);
			bushes[i].setSecondTexture(TextureManager::LoadTexture("assets/lov.png"));
		}
		do
		{
			bushes[i].setPosX_Y(screenW, screenH, 100, 100);
		} while (hasTile(i, bushes, bushesCount) == true);
		bushes[i].setMainTexture(TextureManager::LoadTexture("assets/bush.png"));
		bushes[i].setSrcDest_W_H(60, 60, 100, 100);
	}

	const int FPS = 180;
	const int frameDelay = 1000 / FPS;
	int frameStart;
	int frameTime;

	while (true)
	{
		SDL_GetMouseState(&key.mouseX, &key.mouseY);
		SDL_PollEvent(&event);

		frameStart = clock();

		
		SDL_SetRenderDrawColor(textureManager.renderer, 123, 123, 123, 0);
		SDL_RenderClear(textureManager.renderer);
		if (bushesCountNotOpen > 0) {
			TextureManager::Drow(fon, src, dest);
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				for (int i = 0; i < bushesCount; i++) {
					if (bushes[i].isOpen == false && intersection(bushes[i], key.mouseX, key.mouseY) == true) {
						score += bushes[i].getScore();
						if (score < 0) {
							score = 0;
						}
						bushesCountNotOpen--;
						bushes[i].isOpen = true;
					}
				}
			}

			for (int i = 0; i < bushesCount; i++) {
				if (bushes[i].isOpen == false) {
					TextureManager::Drow(bushes[i].getMainTexture(), bushes[i].src, bushes[i].dest);
				}
				else {
					TextureManager::Drow(bushes[i].getSecondTexture(), bushes[i].src, bushes[i].dest);
				}
			}
		}
		else {
			textManager.Drow(textureManager.renderer, u8"вы набрали " + to_string(score) + u8" очков", 43 * 20, 72, screenW / 2 - 43 * 20 / 2,
				screenH / 2 + 80, 255, 255, 255);
		}

		SDL_RenderPresent(textureManager.renderer);

		frameTime = clock() - frameStart;
		while (frameTime < frameDelay)
		{
			frameTime = clock() - frameStart;
		}
	}

	return 0;
}


bool intersection(GameObgect gameObgect, int x, int y) {
	if (x > gameObgect.dest.x && x< gameObgect.dest.x + gameObgect.dest.w &&
		y > gameObgect.dest.y && y < gameObgect.dest.y + gameObgect.dest.h) {
		return 1;
	}
	return 0;
}


void GameObgect::setPosX_Y(int screenW, int screenH, int tile_w, int tile_h)
{
	posX = rand() % (screenW / tile_w - 2) + 1;
	posY = rand() % (screenH / tile_h - 1) + 1;
	src.x = posX * tile_w;
	src.y = posY * tile_h;
	dest.x = posX * tile_w;
	dest.y = posY * tile_h;
}

void GameObgect::setMainTexture(SDL_Texture* value)
{
	maintx = value;
}

void GameObgect::setSecondTexture(SDL_Texture* value)
{
	second = value;
}

void GameObgect::setSrcDest_W_H(int src_w, int src_h, int dest_w, int dest_h)
{
	src.w = src_w;
	src.h = src_h;
	dest.w = dest_w;
	dest.h = dest_h;
}

void GameObgect::setSrcDest_X_Y(int src_x, int src_y, int dest_x, int dest_y)
{
	src.x = src_x;
	src.y = src_y;
	dest.x = dest_x;
	dest.y = dest_y;
}

void GameObgect::setType(int type)
{
	this->type = type;
	if (type == 1) {
		score = rand() % 10 + 1;
	}
	else if (type == 2) {
		score = 1;
	}
	else {
		score = -1;
	}
}

SDL_Texture* GameObgect::getMainTexture()
{
	return maintx;
}

SDL_Texture* GameObgect::getSecondTexture()
{
	return second;
}

int GameObgect::getScore()
{
	return score;
}

void GameObgect::chengeMainTexture()
{
	maintx = second;
}