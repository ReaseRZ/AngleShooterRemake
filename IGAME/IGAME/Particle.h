#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define MAX_PARTICLE 50

class Dot{
public:
	double mX, mY;
	double vecX, vecY;
	Dot() = default;
	void emitLocation(int x, int y, double radian) {
		mX = x;
		mY = y;
		vecX = std::cos(radian);
		vecY = std::sin(radian);
	}

	void update() {
		vecX /= 1.0075;
		vecY /= 1.0075;
		mX += vecX;
		mY += vecY;
	}
};

class Particle{
public:
	Particle(SDL_Renderer* renderer, SDL_Texture *texture) {
		mTexture = texture;

		mSource.x = 0;
		mSource.y = 0;
		SDL_QueryTexture(mTexture, NULL, NULL, &mSource.w, &mSource.h);

		mDest.h = mSource.h;
		mDest.w = mSource.w;

		for (int i = 0; i < MAX_PARTICLE; i++) {
			dot[i] = new Dot();
		}
	}
	~Particle() {
		for (int i = 0; i < MAX_PARTICLE; i++) {
			delete dot[i];
		}
	}
	Dot *dot[MAX_PARTICLE];

	void SetEmitter(int x, int y) {
		for (int i = 0; i < MAX_PARTICLE;i++) {
			double angle = (std::rand() % 360) * 3.14f / 180.f;
			dot[i]->emitLocation(x, y, angle);
		}
	}

	bool Destroy() {
		for (int i = 0; i < MAX_PARTICLE; i++) {
			if ((dot[i]->vecX<-0.1 || dot[i]->vecX>0.1) && dot[i]->vecY<-0.1 || dot[i]->vecY>0.1) {
				return false;
			}
		}
		return true;
	}

	void update() {
		for (int i = 0; i < MAX_PARTICLE; i++) {
			dot[i]->update();
		}
	}

	void draw(SDL_Renderer *renderer) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		for (int i = 0; i < MAX_PARTICLE; i++) {
			mDest.x = dot[i]->mX;
			mDest.y = dot[i]->mY;
			SDL_RenderCopy(renderer, mTexture, &mSource, &mDest);
		}
	}

private:
	SDL_Rect mSource;
	SDL_Rect mDest;
	SDL_Texture *mTexture;
};