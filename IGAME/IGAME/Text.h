#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "Utility.h"

class Text {
public:
	Text() = default;
	Text(int positionX, int positionY, int width, int height, Color color, const char* text,int fontSize,SDL_Renderer *renderer) {
		res = "0";
		mFont = TTF_OpenFont("font/slkscr.ttf", fontSize);
		SDL_Color fontColor;
		switch (color)
		{
		case RED:
			fontColor = { 255,0,0};
			break;
		case GREEN:
			fontColor = { 0,255,0 };
			break;
		case BLUE:
			fontColor = { 0,0,255 };
			break;
		case WHITE:
			fontColor = { 255,255,255 };
		default:
			break;
		}
		SDL_Surface *textSurface = TTF_RenderText_Solid(mFont, text, fontColor);
		mTextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_FreeSurface(textSurface);

		mTextRect.h = height;
		mTextRect.w = width;
		mTextRect.x = positionX;
		mTextRect.y = positionY;
		mStart = SDL_GetTicks();
	}

	~Text(){
		SDL_DestroyTexture(mTextTexture);
	}

	void update() {
		mEnd = SDL_GetTicks();
		mTextRect.y -= 3;
	}
	void updateNoMove() {
		mTextRect.w = 45 * res.length();
	}

	bool isAlive() {
		return (mEnd - mStart) > mTime;
	}

	void render(SDL_Renderer *renderer) {
		SDL_RenderCopy(renderer, mTextTexture, NULL, &mTextRect);
	}

	void renderSpecial(SDL_Renderer *renderer) {
		SDL_RenderCopy(renderer, mTextTexture, NULL, &mTextRect);
		SDL_DestroyTexture(mTextTexture);
	}

	void renderWithRotation(SDL_Renderer *renderer, double angle) {
		SDL_RenderCopyEx(renderer, mTextTexture, NULL, &mTextRect, angle, NULL, SDL_FLIP_NONE);
	}

	SDL_Rect GetRect()const {
		return mTextRect;
	}

	void SetText(std::string text, SDL_Color fontColor,SDL_Renderer *renderer) {
		res = text;
		SDL_Surface *textSurface = TTF_RenderText_Solid(mFont, text.c_str(), fontColor);
		mTextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_FreeSurface(textSurface);
	}


private:
	std::string res;
	const Uint32 mTime = 1500;
	Uint32 mStart;
	Uint32 mEnd;
	SDL_Rect mTextRect;
	TTF_Font *mFont;
	SDL_Texture *mTextTexture;
};