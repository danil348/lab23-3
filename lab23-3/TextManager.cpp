#include "TextManager.h"

TextManager::TextManager()
{
	TTF_Init();
	my_font = TTF_OpenFont("3.ttf", 72);
}

TextManager::~TextManager()
{
	TTF_CloseFont(my_font);
}

void TextManager::Drow(SDL_Renderer* renderer, string text, int dest_w, int dest_h, int dest_x, int dest_y,
	int colorR, int colorG, int colorB)
{
	fore_color.r = colorR;
	fore_color.g = colorG;
	fore_color.b = colorB;
	textSurface = TTF_RenderUTF8_Blended_Wrapped(my_font, text.c_str(), fore_color, 1000);
	texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	rect = { dest_x, dest_y, dest_w, dest_h };
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(texture);
}
