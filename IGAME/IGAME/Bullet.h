#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include "Utility.h"

class Bullet {
public:
	Bullet() = default;
	~Bullet() {
		SDL_DestroyTexture(mTexture);
	}
	Bullet(int positionX, int positionY, Color color,SDL_Renderer *renderer, int speed,double radian) {
		mColor = color;
		SDL_Surface *mSurface = NULL;
		switch (color)
		{
		case GREEN:
			mSurface = IMG_Load("assets/08.png");
			break;
		case BLUE:
			mSurface = IMG_Load("assets/01.png");
			break;
		case RED:
			mSurface = IMG_Load("assets/02.png");
			break;
		default:
			break;
		}
		mTexture = SDL_CreateTextureFromSurface(renderer, mSurface);
		SDL_FreeSurface(mSurface);
		SDL_QueryTexture(mTexture, NULL, NULL, &mSource.w, &mSource.h);

		mAlive = true;
		mRadian = radian;
		mSpeed = speed;
		mSource.x = 0;
		mSource.y = 0;

		mDest.x = positionX;
		mDest.y = positionY;
		mDest.h = 30;
		mDest.w = 30;

		mPoint.x = mDest.w / 2;
		mPoint.y = mDest.h / 2;
	}

	void update() {
		mDest.x += mSpeed*cos(mRadian);
		mDest.y += mSpeed*sin(mRadian);
	}
	void render(SDL_Renderer* render,double angle) {
		SDL_RenderCopyEx(render, mTexture, &mSource, &mDest, angle, &mPoint, SDL_FLIP_NONE);
	}

	SDL_Rect getRectangle(){
		return mDest;
	}

	SDL_Point GetPosition(){
		SDL_Point point;
		point.x = mDest.x;
		point.y = mDest.y;

		return point;
	}

	void SetAlive(bool alive) {
		mAlive = alive;
	}

	bool GetAlive()const {
		return mAlive;
	}

	void SetSpeed(int speed) {
		mSpeed = speed;
	}

	Color GetColor()const {
		return mColor;
	}

private:
	Color mColor;
	bool mAlive;
	int mSpeed;
	double mRadian;
	SDL_Point mPoint;
	SDL_Rect mSource;
	SDL_Rect mDest;
	SDL_Texture *mTexture;
};