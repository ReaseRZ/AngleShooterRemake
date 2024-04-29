#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Utility.h"

class Block {
public:
	Block() = default;
	Block(const Block&) = delete;

	Block(int positionX, int positionY, double radian, Color color,SDL_Renderer *renderer, SDL_Rect rect) {
		mVisible = true;
		mGetWrongColorShoot = false;
		mStopped = false;
		mColor = color;
		angle = 0;
		mPointRect = rect;
		mRadian = radian;
		mRect.x = 0;
		mRect.y = 0;
		SDL_Surface *mSurface = NULL;
		switch (color)
		{
		case GREEN:
			mSurface = IMG_Load("assets/rect3.png");
			break;
		case BLUE:
			mSurface = IMG_Load("assets/rect1.png");
			break;
		case RED:
			mSurface = IMG_Load("assets/rect2.png");
			break;
		default:
			break;
		}
		mTexture = SDL_CreateTextureFromSurface(renderer, mSurface);
		SDL_FreeSurface(mSurface);

		SDL_QueryTexture(mTexture, NULL, NULL, &mRect.w, &mRect.h);

		mDestRect.h = 60;
		mDestRect.w = 60;

		mDestRect.x = positionX;
		mDestRect.y = positionY;
	}
	~Block() {
		SDL_DestroyTexture(mTexture);
	}

	SDL_Point getPosition() {
		SDL_Point position;
		position.x = mDestRect.x;
		position.y = mDestRect.y;

		return position;
	}

	SDL_bool enterAreaRect(SDL_Rect *rect) {
		return SDL_HasIntersection(&mDestRect, rect);
	}

	void render(SDL_Renderer *renderer) {
		SDL_RenderCopyEx(renderer, mTexture, &mRect, &mDestRect, angle, NULL, SDL_FLIP_NONE);
	}

	void Update() {
		angle += 10;
		if (!mStopped) {
			int cosX = mPointRect.x + 25 - mDestRect.x;
			int sinY = mPointRect.y + 25 - mDestRect.y;
			mRadian = atan2(sinY, cosX);
			mDestRect.x += 1.5 * (cos(mRadian));
			mDestRect.y += 1.5 * (sin(mRadian));
		}
	}

	Color GetColor()const {
		return mColor;
	}

	void SetWrongColorShoot(bool value) {
		mGetWrongColorShoot = value;
	}

	void SetStopped(bool value) {
		mStopped = value;
	}

	void SetVisible(bool value) {
		mVisible = value;
	}

	bool isVisible()const {
		return mVisible;
	}

	bool GetWrongColorShoot()const {
		return mGetWrongColorShoot;
	}

private:
	bool mVisible;
	bool mStopped;
	bool mGetWrongColorShoot;
	double angle;
	double mRadian;
	Color mColor;
	SDL_Rect mPointRect;
	SDL_Rect mRect;
	SDL_Rect mDestRect;
	SDL_Texture *mTexture;
};