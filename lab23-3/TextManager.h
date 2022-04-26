#pragma once
#include <iostream>
#include <string>
#include "SDL_ttf.h"

using namespace std;

class TextManager
{
public:
	TextManager();
	~TextManager();
	void Drow(SDL_Renderer* renderer, string text, int dest_w, int dest_h, int dest_x, int dest_y,
		int colorR, int colorG, int colorB);
private:
	TTF_Font* my_font;
	SDL_Surface* textSurface;
	SDL_Color fore_color;
	SDL_Texture* texture;
	SDL_Rect rect;
};

