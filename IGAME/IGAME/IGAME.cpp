#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_syswm.h>
#include <SFML\Audio\SoundBuffer.hpp>
#include <SFML\Audio\Sound.hpp>
#include "Bullet.h"
#include "block.h"
#include "Particle.h"
#include "Text.h"

SDL_Window *g_window;
SDL_Renderer *g_renderer;

SDL_Rect CreateCustomRect(int x, int y, int w, int h);
void mainmenu();
void gameplay();

const int DELAY_TIME = 1000.0f / 60.f;

bool loop = true;
int ResX, ResY;
int HP = 10;
int maxHP = 10;
const Uint32 shield = 2500;
const Uint32 energy = 100;
const Uint32 cooldown = 2000;
const Uint32 cooldown2 = 3500;
const Uint32 cooldown3 = 10000;

State state = State::MAINMENU;
GameType gameType = GameType::NOPE;

int main(int argc, char* args[]) {
	srand(time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	g_window = SDL_CreateWindow("Angle Shooter Remake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 720, SDL_WINDOW_FULLSCREEN_DESKTOP);
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);
	if (TTF_Init() < 0) {
		printf("failed to init ttf\n");
	}
	if (Mix_Init(MIX_INIT_MP3) <= 0) {
		printf("failed to init mp3 loader\n");
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 4, 1024) < 0) {
		printf("Init audio failure");
	}
	ResX = GetSystemMetrics(SM_CXSCREEN);
	ResY = GetSystemMetrics(SM_CYSCREEN);

	while (loop) {
		switch (state)
		{
		case MAINMENU:
			mainmenu();
			break;
		case GAME:
			gameplay();
			break;
		case NONE:
			break;
		default:
			break;
		}
	}
	
	SDL_DestroyWindow(g_window);
	SDL_DestroyRenderer(g_renderer);
	SDL_Quit();

	printf("THANK YOU FOR PLAYING MY GAME ^^\n");

	return 0;
}

void mainmenu() {
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("sound/uiClick.wav")) {
		printf("Can't load file\n");
	}
	sf::Sound sound;
	sound.setBuffer(buffer);

	Mix_Music *music = Mix_LoadMUS("sound/Menu.mp3");
	bool localLoop = true;
	bool showPopDiff = false;
	Text *titleText = new Text(ResX / 2 - 700 / 2, ResY / 2 - 80, 700, 150, Color::WHITE, "ANGLE SHOOTER", 70, g_renderer);
	Text *subTitleText = new Text(ResX / 2 - 300 / 2 + 350, ResY / 2, 300, 75, Color::GREEN, "REMAKE", 70, g_renderer);
	Text *startText = new Text(25, ResY - 100, 250, 45, Color::WHITE, "PRESS SPACE TO PLAY", 20, g_renderer);
	Text *endText = new Text(25, ResY - 50, 250, 45, Color::WHITE, "PRESS ESCAPE TO END", 70, g_renderer);

	Text *difficult = new Text(ResX / 2-500/2, ResY / 2-200, 500, 100, Color::WHITE, "-DIFFICULTY-", 55, g_renderer);
	Text *normal = new Text(ResX / 2-400/2, ResY / 2-50, 400, 85, Color::WHITE, "[1]NORMAL", 35, g_renderer);
	Text *hidden = new Text(ResX / 2-400/2, ResY / 2+75, 400, 85, Color::WHITE, "[2]HIDDEN", 35, g_renderer);
	Text *info1 = new Text(ResX / 2 - 450 / 2, ResY / 2 + 200, 450, 45, Color::WHITE, "PRESS KEY NUMBER TO CHOOSE DIFFICULTY", 35, g_renderer);
	Text *info2 = new Text(ResX / 2 - 400 / 2, ResY / 2 + 250, 400, 50, Color::WHITE, "PRESS BACKSPACE TO GO MAIN MENU", 35, g_renderer);
	SDL_Surface *backgroundImgSurface = IMG_Load("assets/bg_01.png");
	SDL_Texture *backgroundImgTexture = SDL_CreateTextureFromSurface(g_renderer, backgroundImgSurface);
	SDL_FreeSurface(backgroundImgSurface);

	SDL_Rect bgImgRect1;
	SDL_Rect bgImgRect2;
	SDL_Rect bgImgRect3;

	Uint32 frametime, frameStart;

	Mix_PlayMusic(music, 100);

	SDL_Rect background;
	background.h = ResY;
	background.w = ResX;
	background.x = 0;
	background.y = 0;

	bgImgRect1.x = 0;
	bgImgRect2.x = -ResX;
	bgImgRect3.x = -ResX * 2;
	bgImgRect1.y = bgImgRect2.y = bgImgRect3.y = 0;
	SDL_QueryTexture(backgroundImgTexture, NULL, NULL, &bgImgRect1.w, &bgImgRect1.h);
	SDL_QueryTexture(backgroundImgTexture, NULL, NULL, &bgImgRect2.w, &bgImgRect2.h);
	SDL_QueryTexture(backgroundImgTexture, NULL, NULL, &bgImgRect3.w, &bgImgRect3.h);
	bgImgRect1.h = bgImgRect2.h = bgImgRect3.h = ResY;
	bgImgRect1.w = bgImgRect2.w = bgImgRect3.w = ResX;

	while (localLoop) {
		frameStart = SDL_GetTicks();
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_SPACE) {
					sound.play();
					showPopDiff = true;
				}
				if (event.key.keysym.sym == SDLK_ESCAPE &&!showPopDiff) {
					sound.play();
					localLoop = false;
					loop = false;
					state = State::NONE;
				}
				if (event.key.keysym.sym == SDLK_1 &&showPopDiff) {
					gameType = GameType::NORMAL;
					state = State::GAME;
					localLoop = false;
				}
				if (event.key.keysym.sym == SDLK_2&&showPopDiff) {
					gameType = GameType::HIDDEN;
					state = State::GAME;
					localLoop = false;
				}
				if (event.key.keysym.sym == SDLK_BACKSPACE) {
					showPopDiff = false;
				}
			default:
				break;
			}
		}
		bgImgRect1.x++;
		bgImgRect2.x++;
		bgImgRect3.x++;

		if (bgImgRect1.x > ResX) {
			bgImgRect1.x = -ResX * 2 + 10;
		}
		if (bgImgRect2.x > ResX) {
			bgImgRect2.x = -ResX * 2 + 10;
		}
		if (bgImgRect3.x > ResX) {
			bgImgRect3.x = -ResX * 2 + 10;
		}

		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
		SDL_RenderClear(g_renderer);
		SDL_RenderCopy(g_renderer, backgroundImgTexture, NULL, &bgImgRect1);
		SDL_RenderCopy(g_renderer, backgroundImgTexture, NULL, &bgImgRect2);
		SDL_RenderCopy(g_renderer, backgroundImgTexture, NULL, &bgImgRect3);
		titleText->render(g_renderer);
		subTitleText->renderWithRotation(g_renderer, -10);
		startText->render(g_renderer);
		endText->render(g_renderer);

		if (showPopDiff) {
			SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 175);
			SDL_RenderFillRect(g_renderer, &background);
			difficult->render(g_renderer);
			normal->render(g_renderer);
			hidden->render(g_renderer);
			info1->render(g_renderer);
			info2->render(g_renderer);
		}

		SDL_RenderPresent(g_renderer);

		frametime = SDL_GetTicks() - frameStart;

		if (frametime < DELAY_TIME) {
			SDL_Delay((int)(DELAY_TIME - frametime));
		}
	}
	delete normal;
	delete hidden;
	delete info1;
	delete info2;
	delete endText;
	delete startText;
	delete subTitleText;
	delete titleText;
	delete difficult;

	difficult = nullptr;
	normal = nullptr;
	hidden = nullptr;
	info1 = nullptr;
	info2 = nullptr;
	endText = nullptr;
	startText = nullptr;
	subTitleText = nullptr;
	titleText = nullptr;
	SDL_DestroyTexture(backgroundImgTexture);
	Mix_FreeMusic(music);
}

void gameplay() {
	HP = 10;
	bool ableToShoot = true;
	bool isShieldActive = true;
	Uint32 startRechargeEnergy = 0;
	Uint32 startRechargeShield = 0;
	int energyTempValue;
	int hpTempValue = 107;
	int shieldTempValue = 107;

	sf::SoundBuffer buffer;
	sf::SoundBuffer soundBlockBuffer;
	sf::SoundBuffer soundChangeWeaponBuffer;
	sf::SoundBuffer soundSkill1Effect;
	sf::SoundBuffer soundSkill2Effect;
	sf::SoundBuffer soundSkill3Effect;

	if (!buffer.loadFromFile("sound/blaster.wav")) {
		printf("can't load file\n");
	}
	if (!soundBlockBuffer.loadFromFile("sound/texturedImpact.wav")) {
		printf("can't load file\n");
	}
	if (!soundChangeWeaponBuffer.loadFromFile("sound/changeWeapon.wav")) {
		printf("can't load file\n");
	}
	if (!soundSkill1Effect.loadFromFile("sound/dimension.wav")) {
		printf("can't load file\n");
	}
	if (!soundSkill2Effect.loadFromFile("sound/skill2.wav")) {
		printf("can't load file\n");
	}
	if (!soundSkill3Effect.loadFromFile("sound/skill3.wav")) {
		printf("can't load file\n");
	}

	sf::Sound sound;
	sound.setBuffer(buffer);
	sf::Sound soundBlockBlast;
	soundBlockBlast.setBuffer(soundBlockBuffer);
	sf::Sound changeWeapon;
	changeWeapon.setBuffer(soundChangeWeaponBuffer);
	sf::Sound skillEffect1;
	skillEffect1.setBuffer(soundSkill1Effect);
	sf::Sound skillEffect2;
	skillEffect2.setBuffer(soundSkill2Effect);
	sf::Sound skillEffect3;
	skillEffect3.setBuffer(soundSkill3Effect);

	Mix_Music *backThemeSound = Mix_LoadMUS("sound/keith.mp3");

	std::vector<Bullet*>bullets;
	std::vector<Block*>blocks;
	std::vector<Particle*>particles;
	std::vector<Text*>texts;

	SDL_Surface *guiSurface = IMG_Load("assets/uibar.png");
	SDL_Texture *guiTexture = SDL_CreateTextureFromSurface(g_renderer, guiSurface);
	guiSurface = IMG_Load("assets/bulletChooseBar.png");
	SDL_Texture *guiBulletTexture = SDL_CreateTextureFromSurface(g_renderer, guiSurface);
	guiSurface = IMG_Load("assets/01.png");
	SDL_Texture *blueBullet = SDL_CreateTextureFromSurface(g_renderer, guiSurface);
	guiSurface = IMG_Load("assets/02.png");
	SDL_Texture *redBullet = SDL_CreateTextureFromSurface(g_renderer, guiSurface);
	guiSurface = IMG_Load("assets/08.png");
	SDL_Texture *greenBullet = SDL_CreateTextureFromSurface(g_renderer, guiSurface);
	guiSurface = IMG_Load("assets/hpBar.png");
	SDL_Texture *hpBar = SDL_CreateTextureFromSurface(g_renderer, guiSurface);
	guiSurface = IMG_Load("assets/energyBar.png");
	SDL_Texture *energyBar = SDL_CreateTextureFromSurface(g_renderer, guiSurface);
	guiSurface = IMG_Load("assets/bulletBar.png");
	SDL_Texture *bulletBar = SDL_CreateTextureFromSurface(g_renderer, guiSurface);
	guiSurface = IMG_Load("assets/spr_shield.png");
	SDL_Texture *shieldEffect = SDL_CreateTextureFromSurface(g_renderer, guiSurface);
	SDL_FreeSurface(guiSurface);

	SDL_Surface *effectSkill = IMG_Load("assets/spell.png");
	SDL_Texture *effectSkillTexture = SDL_CreateTextureFromSurface(g_renderer, effectSkill);
	SDL_Surface *skillBorder = IMG_Load("assets/border.png");
	SDL_Texture *skillBorderTexture = SDL_CreateTextureFromSurface(g_renderer, skillBorder);
	SDL_FreeSurface(effectSkill);
	SDL_FreeSurface(skillBorder);

	SDL_Rect borderSkillSrcRect;
	SDL_Rect borderSkillDestRect;
	SDL_Rect borderSkill2DestRect;
	SDL_Rect borderSkill3DestRect;
	SDL_Rect skillSrcRect;
	SDL_Rect skillDestRect;
	SDL_Rect Skill2SrcRect;
	SDL_Rect Skill2DestRect;
	SDL_Rect Skill3SrcRect;
	SDL_Rect skill3DestRect;
	SDL_Rect skillIconSrcRect;
	SDL_Rect skillIconDestRect;
	SDL_Rect skill2IconSrcRect;
	SDL_Rect skill2IconDestRect;
	SDL_Rect skill3IconSrcRect;
	SDL_Rect skill3IconDestRect;

	skillIconSrcRect.h = skillIconSrcRect.w = skill2IconSrcRect.h = skill2IconSrcRect.w = skill3IconSrcRect.h = skill3IconSrcRect.w = 64;
	skillIconSrcRect.x = 64 * 2;
	skillIconSrcRect.y = 64 * 2;
	skill2IconSrcRect.y = 64 * 6;
	skill2IconSrcRect.x = 64 * 2;
	skill3IconSrcRect.x = 64 * 1;
	skill3IconSrcRect.y = 64 * 4;
	skillIconDestRect.h = skill2IconDestRect.h = skill3IconDestRect.h = 45;
	skillIconDestRect.w = skill2IconDestRect.w = skill3IconDestRect.w = 45;
	borderSkillSrcRect.h = borderSkillSrcRect.w = 64;
	borderSkillSrcRect.x = 64 * 5;
	borderSkillSrcRect.y = 0;
	borderSkillDestRect.h = borderSkill2DestRect.h = borderSkill3DestRect.h = 54;
	borderSkillDestRect.w = borderSkill2DestRect.w = borderSkill3DestRect.w = 54;
	borderSkillDestRect.x = 500;
	borderSkill2DestRect.x = 500 + 75;
	borderSkill3DestRect.x = 500 + 150;
	borderSkillDestRect.y = borderSkill2DestRect.y = borderSkill3DestRect.y = ResY - 95;

	skillIconDestRect.x = borderSkillDestRect.x + 5;
	skillIconDestRect.y = borderSkillDestRect.y + 5;
	skill2IconDestRect.x = borderSkill2DestRect.x + 5;
	skill2IconDestRect.y = borderSkill2DestRect.y + 5;
	skill3IconDestRect.x = borderSkill3DestRect.x + 5;
	skill3IconDestRect.y = borderSkill3DestRect.y + 5;

	skillSrcRect.h = 64;
	skillSrcRect.w = 64;
	skillSrcRect.x = 64 * 2;
	skillSrcRect.y = 64 * 2;
	skillDestRect.w = 64 * 8;
	skillDestRect.h = 64 * 8;
	skillDestRect.x = 400;
	skillDestRect.y = 200;

	Skill2SrcRect.w = Skill2SrcRect.h = Skill3SrcRect.w = Skill3SrcRect.h = 64;
	Skill2SrcRect.x = Skill3SrcRect.x = 0;
	Skill2SrcRect.y = 64 * 6;
	Skill3SrcRect.y = 64 * 4;

	skill3DestRect.h = 64 * 4;
	skill3DestRect.w = 64 * 4;
	Skill2DestRect.h = 64 * 5;
	Skill2DestRect.w = 64 * 5;

	int skillCurrentFrame = 0;
	int skill2CurrentFrame = 0;
	int skill3CurrentFrame = 0;

	SDL_Rect hpBarRect;
	SDL_Rect shieldBarRect;
	SDL_Rect energyBarRect;

	SDL_Texture *currentBullet = blueBullet;
	SDL_Rect guiRect;
	SDL_Rect bulletGuiRect;
	SDL_Rect redBulletGuiRect;
	SDL_Rect blueBulletGuiRect;
	SDL_Rect greenBulletGuiRect;
	SDL_Rect currentBulletGuiRect;
	SDL_Rect shieldEffectRect;
	SDL_Rect shieldEffectCollider;

	redBulletGuiRect.h = blueBulletGuiRect.h = greenBulletGuiRect.h = currentBulletGuiRect.h = 121;
	redBulletGuiRect.w = blueBulletGuiRect.w = greenBulletGuiRect.w = currentBulletGuiRect.w = 125;

	SDL_QueryTexture(guiTexture, NULL, NULL, &guiRect.w, &guiRect.h);
	SDL_QueryTexture(guiTexture, NULL, NULL, &bulletGuiRect.w, &bulletGuiRect.h);
	SDL_QueryTexture(hpBar, NULL, NULL, &hpBarRect.w, &hpBarRect.h);
	SDL_QueryTexture(energyBar, NULL, NULL, &energyBarRect.w, &energyBarRect.h);
	SDL_QueryTexture(bulletBar, NULL, NULL, &shieldBarRect.w, &shieldBarRect.h);
	SDL_QueryTexture(shieldEffect, NULL, NULL, &shieldEffectRect.w, &shieldEffectRect.h);
	shieldEffectRect.h = 256;
	shieldEffectRect.w = 256;
	shieldEffectCollider.h = 175;
	shieldEffectCollider.w = 175;


	bulletGuiRect.x = 50 + guiRect.w;
	bulletGuiRect.y = ResY - 100;
	guiRect.x = 35;
	guiRect.y = ResY - 100;

	hpBarRect.x = guiRect.x + 75;
	hpBarRect.y = guiRect.y + 7;
	shieldBarRect.x = guiRect.x + 74;
	shieldBarRect.y = guiRect.y + 27;
	energyBarRect.x = guiRect.x + 74;
	energyBarRect.y = guiRect.y + 46;

	currentBulletGuiRect.x = guiRect.x - 25;
	currentBulletGuiRect.y = guiRect.y - 27;

	blueBulletGuiRect.x = bulletGuiRect.x - 14;
	blueBulletGuiRect.y = bulletGuiRect.y - 28;

	redBulletGuiRect.x = bulletGuiRect.x + 41;
	redBulletGuiRect.y = bulletGuiRect.y - 28;

	greenBulletGuiRect.x = bulletGuiRect.x + 96;
	greenBulletGuiRect.y = bulletGuiRect.y - 28;

	Text *controlA = new Text(bulletGuiRect.x + 33, bulletGuiRect.y + 60, 25, 25, Color::WHITE, "A", 20, g_renderer);
	Text *controlS = new Text(bulletGuiRect.x + 87, bulletGuiRect.y + 60, 25, 25, Color::WHITE, "S", 20, g_renderer);
	Text *controlD = new Text(bulletGuiRect.x + 143, bulletGuiRect.y + 60, 25, 25, Color::WHITE, "D", 20, g_renderer);
	Text *controlQ = new Text(borderSkillDestRect.x + 15, borderSkillDestRect.y + 55, 25, 25, Color::WHITE, "Q", 20, g_renderer);
	Text *controlW = new Text(borderSkillDestRect.x + 90, borderSkillDestRect.y + 55, 25, 25, Color::WHITE, "W", 20, g_renderer);
	Text *controlE = new Text(borderSkillDestRect.x + 165, borderSkillDestRect.y + 55, 25, 25, Color::WHITE, "E", 20, g_renderer);
	Text *guiScore = new Text(50, 50, 45, 50, Color::WHITE, "0", 25, g_renderer);
	Text *timeText = new Text(705, ResY - 95, 125, 50, Color::WHITE, "TIME :", 25, g_renderer);
	Text *time = new Text(835, ResY - 95, 45, 50, Color::WHITE, "0", 25, g_renderer);


	SDL_Surface *backgroundImgSurface = IMG_Load("assets/bg_01.png");
	SDL_Texture *backgroundImgTexture = SDL_CreateTextureFromSurface(g_renderer, backgroundImgSurface);
	SDL_FreeSurface(backgroundImgSurface);

	SDL_Rect bgImgRect1;
	SDL_Rect bgImgRect2;
	SDL_Rect bgImgRect3;

	bgImgRect1.x = 0;
	bgImgRect2.x = -ResX;
	bgImgRect3.x = -ResX * 2;
	bgImgRect1.y = bgImgRect2.y = bgImgRect3.y = 0;
	SDL_QueryTexture(backgroundImgTexture, NULL, NULL, &bgImgRect1.w, &bgImgRect1.h);
	SDL_QueryTexture(backgroundImgTexture, NULL, NULL, &bgImgRect2.w, &bgImgRect2.h);
	SDL_QueryTexture(backgroundImgTexture, NULL, NULL, &bgImgRect3.w, &bgImgRect3.h);
	bgImgRect1.h = bgImgRect2.h = bgImgRect3.h = ResY;
	bgImgRect1.w = bgImgRect2.w = bgImgRect3.w = ResX;

	SDL_Surface *surface = IMG_Load("assets/triangle1.png");
	SDL_Texture *g_texture = SDL_CreateTextureFromSurface(g_renderer, surface);
	SDL_FreeSurface(surface);
	SDL_Rect rect;


	SDL_Surface *asr = SDL_LoadBMP("assets/red.bmp");
	SDL_Texture *particleRed = SDL_CreateTextureFromSurface(g_renderer, asr);
	SDL_Surface *asb = SDL_LoadBMP("assets/blue.bmp");
	SDL_Texture *particleBlue = SDL_CreateTextureFromSurface(g_renderer, asb);
	SDL_Surface *asg = SDL_LoadBMP("assets/green.bmp");
	SDL_Texture *particleGreen = SDL_CreateTextureFromSurface(g_renderer, asg);
	SDL_FreeSurface(asr);
	SDL_FreeSurface(asg);
	SDL_FreeSurface(asb);

	SDL_QueryTexture(g_texture, NULL, NULL, &rect.w, &rect.h);

	rect.x = 0;
	rect.y = 0;

	SDL_Rect dstRect;
	dstRect.w = rect.w;
	dstRect.h = rect.h;
	dstRect.w = 100;
	dstRect.h = 100;
	dstRect.x = (ResX / 2) - (dstRect.w / 2);
	dstRect.y = (ResY / 2) - (dstRect.h / 2);

	SDL_Point point;
	SDL_Rect boundMainObject;
	boundMainObject.h = 25;
	boundMainObject.w = 25;
	boundMainObject.x = dstRect.x + 40;
	boundMainObject.y = dstRect.y + 40;
	shieldEffectRect.x = dstRect.x - 75;
	shieldEffectRect.y = dstRect.y - 75;
	shieldEffectCollider.x = shieldEffectRect.x + 40;
	shieldEffectCollider.y = shieldEffectRect.y + 40;

	point.x = dstRect.w / 2;
	point.y = dstRect.h / 2;

	SDL_Rect visibleZone;
	visibleZone.h = ResY - 350;
	visibleZone.w = ResX - 500;
	visibleZone.x = ResX / 2 - visibleZone.w / 2;
	visibleZone.y = ResY / 2 - visibleZone.h / 2;

	Uint32 frameStart, frametime;

	int mouseX = 0, mouseY = 0;

	Uint32 timeStrt = 0;
	Uint32 timeElapse = 0;
	Uint32 timeTextAnimated = 0;

	Uint32 skillAnimatedTime = 0;
	Uint32 skill2AnimatedTime = 0;
	Uint32 skill3AnimatedTime = 0;
	Uint32 skillEffectTime = 0;
	Uint32 skill3EffectTime = 0;
	Uint32 skillIconEffect = 0;
	Uint32 skill2IconEffect = 0;
	Uint32 skill3IconEffect = 0;
	Uint32 skillCooldown = 0;
	Uint32 skill2Cooldown = 0;
	Uint32 skill3Cooldown = 0;

	Uint32 increaseHard = SDL_GetTicks();
	int speedSpawnBlock = 1500;
	bool effectActivated = false;
	bool effect3Activated = false;

	Color shootColor = Color::BLUE;

	int centerX = 0;
	int centerY = 0;
	int random = 0;
	double radian = 0.0;
	double angle = 0.0;

	int positionX = 0, positionY = 0, cosX = 0, sinY = 0;
	double aradian = 0;

	Mix_PlayMusic(backThemeSound, 90);

	int currentEffectIconFrame = 0;
	int currentEffect2IconFrame = 0;
	int currentEffect3IconFrame = 0;

	int score = 0;
	int time_t = 0;

	timeTextAnimated = SDL_GetTicks();

	bool useSkill = false;
	bool useSkill2 = false;
	bool useSkill3 = false;
	bool animationSkillActive = false;
	bool animationSkill2Active = false;
	bool animationSkill3Active = false;
	bool gameplay = true;
	while (gameplay) {
		if (SDL_GetTicks() - timeTextAnimated > 1000) {
			time_t++;
			timeTextAnimated = SDL_GetTicks();
		}
		if (HP <= 0) {
			gameplay = false;
			state = State::MAINMENU;
		}
		frameStart = SDL_GetTicks();
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&mouseX, &mouseY);
				centerX = mouseX - dstRect.x - point.x;
				centerY = mouseY - dstRect.y - point.y;

				radian = atan2(centerY, centerX);

				angle = 180.000 * radian / 3.141592;

				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_a) {
					changeWeapon.play();
					currentBullet = blueBullet;
					shootColor = Color::BLUE;
				}
				if (event.key.keysym.sym == SDLK_s) {
					changeWeapon.play();
					currentBullet = redBullet;
					shootColor = Color::RED;
				}
				if (event.key.keysym.sym == SDLK_d) {
					changeWeapon.play();
					currentBullet = greenBullet;
					shootColor = Color::GREEN;
				}
				if (event.key.keysym.sym == SDLK_q && !useSkill) {
					score += 15;
					skillEffect1.play();
					skillCooldown = SDL_GetTicks();
					skillAnimatedTime = SDL_GetTicks();
					skillCurrentFrame = 0;
					animationSkillActive = true;
					useSkill = true;
					effectActivated = true;
					skillDestRect.x = mouseX - skillDestRect.w / 2;
					skillDestRect.y = mouseY - skillDestRect.h / 2;
				}
				if (event.key.keysym.sym == SDLK_w && !useSkill2) {
					score += 35;
					skillEffect2.play();
					skill2Cooldown = SDL_GetTicks();
					skill2AnimatedTime = SDL_GetTicks();
					skill2CurrentFrame = 0;
					useSkill2 = true;
					animationSkill2Active = true;
					Skill2DestRect.x = mouseX - Skill2DestRect.w/2;
					Skill2DestRect.y = mouseY - Skill2DestRect.h/2;
				}
				if (event.key.keysym.sym == SDLK_e && !useSkill3) {
					score += 55;
					skillEffect3.play();
					skill3Cooldown = SDL_GetTicks();
					skill3AnimatedTime = SDL_GetTicks();
					skill3EffectTime = SDL_GetTicks();
					effect3Activated = true;
					useSkill3 = true;
					skill3CurrentFrame = 0;
					animationSkill3Active = true;
					skill3DestRect.x = dstRect.x - skill3DestRect.w / 2 + 50;
					skill3DestRect.y = dstRect.y - skill3DestRect.h / 2 + 50;
				}
				if (event.key.keysym.sym == SDLK_SPACE && ableToShoot) {
					score++;
					startRechargeEnergy = SDL_GetTicks();
					energyTempValue = 107;
					ableToShoot = false;
					sound.play();
					if (shootColor == Color::GREEN) {
						Bullet *bullet = new Bullet(dstRect.x + point.x, dstRect.y + point.y, Color::GREEN, g_renderer, 10, radian);
						bullets.push_back(bullet);
					}
					if (shootColor == Color::BLUE) {
						Bullet *bullet = new Bullet(dstRect.x + point.x, dstRect.y + point.y, Color::BLUE, g_renderer, 10, radian);
						bullets.push_back(bullet);
					}
					if (shootColor == Color::RED) {
						Bullet *bullet = new Bullet(dstRect.x + point.x, dstRect.y + point.y, Color::RED, g_renderer, 10, radian);
						bullets.push_back(bullet);
					}

					for (std::vector<Block*>::iterator pBlock = blocks.begin(); pBlock != blocks.end(); pBlock++) {
						(*pBlock)->SetWrongColorShoot(false);
					}
				}
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					state = State::MAINMENU;
					gameplay = false;
				}
				break;
			case SDL_QUIT:
				gameplay = false;
				break;
			}
		}
		if (SDL_GetTicks() - increaseHard > 10000 && speedSpawnBlock >= 500) {
			increaseHard = SDL_GetTicks();
			speedSpawnBlock -= 100;
		}
		timeElapse = SDL_GetTicks();
		if (timeElapse > timeStrt + speedSpawnBlock) {
			timeStrt = timeElapse;
			random = rand() % 4;
			Block *block = nullptr;
			switch (random)
			{
				//TOP
			case 0:
				positionX = rand() % ResX;
				positionY = -200;
				cosX = dstRect.x - positionX;
				sinY = dstRect.y - positionY;
				aradian = atan2(sinY, cosX);
				block = new Block(positionX, positionY, aradian, Color::BLUE, g_renderer, dstRect);
				blocks.push_back(block);
				break;
				//LEFT
			case 1:
				positionX = -200;
				positionY = rand() % ResY;
				cosX = dstRect.x + point.x * 2 - positionX + 30;
				sinY = dstRect.y + point.y * 2 - positionY + 30;
				aradian = atan2(sinY, cosX);
				block = new Block(positionX, positionY, aradian, Color::GREEN, g_renderer, dstRect);
				blocks.push_back(block);
				break;
				//DOWN
			case 2:
				positionX = rand() % ResX;
				positionY = ResY + 200;
				cosX = dstRect.x + point.x * 2 - positionX + 30;
				sinY = dstRect.y + point.y * 2 - positionY + 30;
				aradian = atan2(sinY, cosX);
				block = new Block(positionX, positionY, aradian, Color::RED, g_renderer, dstRect);
				blocks.push_back(block);
				break;
				//RIGHT
			case 3:
				positionX = ResX + 200;
				positionY = rand() % ResY;
				cosX = dstRect.x + point.x * 2 - positionX + 30;
				sinY = dstRect.y + point.y * 2 - positionY + 30;
				aradian = atan2(sinY, cosX);
				block = new Block(positionX, positionY, aradian, Color::BLUE, g_renderer, dstRect);
				blocks.push_back(block);
				break;
			default:
				break;
			}
		}

		hpBarRect.w = hpTempValue*HP / maxHP;

		if (gameType == GameType::HIDDEN) {
			for (std::vector<Block*>::iterator itr = blocks.begin(); itr != blocks.end(); itr++) {
				if ((*itr)->enterAreaRect(&visibleZone)) {
					(*itr)->SetVisible(false);
				}
			}
		}


		if (effect3Activated) {
			for (std::vector<Bullet*>::iterator itr = bullets.begin(); itr != bullets.end(); itr++) {
				(*itr)->SetSpeed(50);
			}
			if (SDL_GetTicks() - skill3EffectTime > 7500) {
				effect3Activated = false;
			}
		}

		guiScore->updateNoMove();
		time->updateNoMove();
		guiScore->SetText(std::to_string(score), { 255,255,255 }, g_renderer);
		time->SetText(std::to_string(time_t), { 255,255,255 }, g_renderer);

		if (energy < SDL_GetTicks() - startRechargeEnergy) {
			ableToShoot = true;
		}
		if (shield < SDL_GetTicks() - startRechargeShield) {
			isShieldActive = true;
		}

		if (!isShieldActive) {
			int temp = shieldTempValue*(SDL_GetTicks() - startRechargeShield) / (shield);
			shieldBarRect.w = temp;
		}

		if (!ableToShoot) {
			int temp = energyTempValue*(SDL_GetTicks() - startRechargeEnergy) / (energy);
			energyBarRect.w = temp;
		}

		for (std::vector<Block*>::iterator blk = blocks.begin(); blk != blocks.end();)
		{
			if (isShieldActive && (*blk)->enterAreaRect(&shieldEffectCollider)) {
				isShieldActive = false;
				startRechargeShield = SDL_GetTicks();
				soundBlockBlast.play();
				if ((*blk)->GetColor() == Color::BLUE) {
					Particle *particle = new Particle(g_renderer, particleBlue);
					particle->SetEmitter((*blk)->getPosition().x + 30, (*blk)->getPosition().y + 30);
					particles.push_back(particle);
				}
				if ((*blk)->GetColor() == Color::GREEN) {
					Particle *particle = new Particle(g_renderer, particleGreen);
					particle->SetEmitter((*blk)->getPosition().x + 30, (*blk)->getPosition().y + 30);
					particles.push_back(particle);
				}
				if ((*blk)->GetColor() == Color::RED) {
					Particle *particle = new Particle(g_renderer, particleRed);
					particle->SetEmitter((*blk)->getPosition().x + 30, (*blk)->getPosition().y + 30);
					particles.push_back(particle);
				}
				(*blk)->~Block();
				delete *blk;
				blk = blocks.erase(blk);
				continue;
			}
			if ((*blk)->enterAreaRect(&boundMainObject) && !isShieldActive) {
				HP--;
				soundBlockBlast.play();
				if ((*blk)->GetColor() == Color::BLUE) {
					Particle *particle = new Particle(g_renderer, particleBlue);
					particle->SetEmitter((*blk)->getPosition().x + 30, (*blk)->getPosition().y + 30);
					particles.push_back(particle);
				}
				if ((*blk)->GetColor() == Color::GREEN) {
					Particle *particle = new Particle(g_renderer, particleGreen);
					particle->SetEmitter((*blk)->getPosition().x + 30, (*blk)->getPosition().y + 30);
					particles.push_back(particle);
				}
				if ((*blk)->GetColor() == Color::RED) {
					Particle *particle = new Particle(g_renderer, particleRed);
					particle->SetEmitter((*blk)->getPosition().x + 30, (*blk)->getPosition().y + 30);
					particles.push_back(particle);
				}
				(*blk)->~Block();
				delete *blk;
				blk = blocks.erase(blk);
				continue;
			}
			blk++;
		}

		for (std::vector<Particle*>::iterator itr = particles.begin(); itr != particles.end(); itr++) {
			(*itr)->update();
		}

		for (std::vector<Bullet*>::iterator itr = bullets.begin(); itr != bullets.end(); ++itr) {
			(*itr)->update();
		}

		for (std::vector<Block*>::iterator itr = blocks.begin(); itr != blocks.end(); ++itr) {
			(*itr)->Update();
		}

		if (effectActivated) {
			effectActivated = false;
			skillEffectTime = SDL_GetTicks();
			for (std::vector<Block*>::iterator itr = blocks.begin(); itr != blocks.end(); ++itr) {
				if ((*itr)->enterAreaRect(&skillDestRect)) {
					(*itr)->SetStopped(true);
				}
			}
		}
		if (SDL_GetTicks() - skillEffectTime > 1500) {
			for (std::vector<Block*>::iterator itr = blocks.begin(); itr != blocks.end(); ++itr) {
				if ((*itr)->enterAreaRect(&skillDestRect)) {
					(*itr)->SetStopped(false);
				}
			}
		}
		
		if (animationSkill2Active) {
			for (std::vector<Block*>::iterator blk = blocks.begin(); blk != blocks.end();) {
				if ((*blk)->enterAreaRect(&Skill2DestRect)) {
					score += 100;
					soundBlockBlast.play();
					if ((*blk)->GetColor() == Color::BLUE) {
						Particle *particle = new Particle(g_renderer, particleBlue);
						particle->SetEmitter((*blk)->getPosition().x + 30, (*blk)->getPosition().y + 30);
						particles.push_back(particle);
					}
					if ((*blk)->GetColor() == Color::GREEN) {
						Particle *particle = new Particle(g_renderer, particleGreen);
						particle->SetEmitter((*blk)->getPosition().x + 30, (*blk)->getPosition().y + 30);
						particles.push_back(particle);
					}
					if ((*blk)->GetColor() == Color::RED) {
						Particle *particle = new Particle(g_renderer, particleRed);
						particle->SetEmitter((*blk)->getPosition().x + 30, (*blk)->getPosition().y + 30);
						particles.push_back(particle);
					}
					(*blk)->~Block();
					delete *blk;
					blk = blocks.erase(blk);
					continue;
				}
				blk++;
			}
		}

		for (std::vector<Bullet*>::iterator itr = bullets.begin(); itr != bullets.end();) {
			if ((*itr)->GetPosition().x > ResX || (*itr)->GetPosition().y > ResY || (*itr)->GetPosition().x < 0 || (*itr)->GetPosition().y < 0) {
				(*itr)->~Bullet();
				delete *itr;
				itr = bullets.erase(itr);
				continue;
			}
			for (std::vector<Block*>::iterator blk = blocks.begin(); blk != blocks.end();) {
				if ((*blk)->enterAreaRect(&(*itr)->getRectangle()) && (*itr)->GetColor() == (*blk)->GetColor()) {
					score += 100;
					soundBlockBlast.play();
					if ((*blk)->GetColor() == Color::BLUE) {
						Particle *particle = new Particle(g_renderer, particleBlue);
						particle->SetEmitter((*blk)->getPosition().x + 30, (*blk)->getPosition().y + 30);
						particles.push_back(particle);
					}
					if ((*blk)->GetColor() == Color::GREEN) {
						Particle *particle = new Particle(g_renderer, particleGreen);
						particle->SetEmitter((*blk)->getPosition().x + 30, (*blk)->getPosition().y + 30);
						particles.push_back(particle);
					}
					if ((*blk)->GetColor() == Color::RED) {
						Particle *particle = new Particle(g_renderer, particleRed);
						particle->SetEmitter((*blk)->getPosition().x + 30, (*blk)->getPosition().y + 30);
						particles.push_back(particle);
					}
					Text *pText = new Text((*blk)->getPosition().x, (*blk)->getPosition().y, 50, 20, (*blk)->GetColor(), textEffect[rand() % 3], 20, g_renderer);
					texts.push_back(pText);
					(*blk)->~Block();
					delete *blk;
					blk = blocks.erase(blk);
					(*itr)->SetAlive(false);
					continue;
				}
				if ((*blk)->enterAreaRect(&(*itr)->getRectangle()) && (*itr)->GetColor() != (*blk)->GetColor() && !(*blk)->GetWrongColorShoot()) {
					(*blk)->SetWrongColorShoot(true);
					Text *pText = new Text((*blk)->getPosition().x, (*blk)->getPosition().y, 50, 20, (*blk)->GetColor(), "Miss", 20, g_renderer);
					texts.push_back(pText);
				}
				blk++;
			}
			itr++;
		}


		for (std::vector<Text*>::iterator pText = texts.begin(); pText != texts.end(); pText++) {
			(*pText)->update();
		}
		for (std::vector<Text*>::iterator pText = texts.begin(); pText != texts.end();) {
			if ((*pText)->isAlive()) {
				delete *pText;
				pText = texts.erase(pText);
				continue;
			}
			pText++;
		}

		for (std::vector<Bullet*>::iterator pItr = bullets.begin(); pItr != bullets.end();) {
			if (!(*pItr)->GetAlive()) {
				(*pItr)->~Bullet();
				delete *pItr;
				pItr = bullets.erase(pItr);
				continue;
			}
			pItr++;
		}

		for (std::vector<Particle*>::iterator pParticle = particles.begin(); pParticle != particles.end();) {
			if ((*pParticle)->Destroy()) {
				delete *pParticle;
				pParticle = particles.erase(pParticle);
				continue;
			}
			pParticle++;
		}

		if (useSkill) {
			if (SDL_GetTicks() - skillCooldown > cooldown) {
				useSkill = false;
			}
		}

		if (useSkill2) {
			if (SDL_GetTicks() - skill2Cooldown > cooldown2) {
				useSkill2 = false;
			}
		}
		
		if (useSkill3) {
			if (SDL_GetTicks() - skill3Cooldown > cooldown3) {
				useSkill3 = false;
			}
		}

		//Render
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
		SDL_RenderClear(g_renderer);
		bgImgRect1.x++;
		bgImgRect2.x++;
		bgImgRect3.x++;

		if (bgImgRect1.x > ResX) {
			bgImgRect1.x = -ResX * 2 + 10;
		}
		if (bgImgRect2.x > ResX) {
			bgImgRect2.x = -ResX * 2 + 10;
		}
		if (bgImgRect3.x > ResX) {
			bgImgRect3.x = -ResX * 2 + 10;
		}
		SDL_RenderCopy(g_renderer, backgroundImgTexture, NULL, &bgImgRect1);
		SDL_RenderCopy(g_renderer, backgroundImgTexture, NULL, &bgImgRect2);
		SDL_RenderCopy(g_renderer, backgroundImgTexture, NULL, &bgImgRect3);
		for (std::vector<Particle*>::iterator pItr = particles.begin(); pItr != particles.end(); pItr++) {
			(*pItr)->draw(g_renderer);
		}

		for (std::vector<Text*>::iterator pText = texts.begin(); pText != texts.end(); pText++) {
			(*pText)->render(g_renderer);
		}

		if (!bullets.empty()) {
			for (const auto& itr : bullets) {
				itr->render(g_renderer, angle);
			}
		}
		for (std::vector<Block*>::iterator itr = blocks.begin(); itr != blocks.end(); ++itr) {
			if ((*itr)->isVisible()) {
				(*itr)->render(g_renderer);
			}
		}
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 255, 255);
		SDL_RenderCopyEx(g_renderer, g_texture, &rect, &dstRect, angle + 90, &point, SDL_FLIP_NONE);

		

		if (!isShieldActive) {
			SDL_SetTextureBlendMode(shieldEffect, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(shieldEffect, (255 * (SDL_GetTicks() - startRechargeShield) / shield));
		}
		else {
			SDL_SetTextureBlendMode(shieldEffect, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(shieldEffect, 255);
		}

		if (!useSkill) {
			if (SDL_GetTicks() - skillIconEffect > 1000 / 9) {
				skillIconSrcRect.x = 64 * currentEffectIconFrame;
				skillIconEffect = SDL_GetTicks();
				currentEffectIconFrame++;
				if (currentEffectIconFrame > 10) {
					currentEffectIconFrame = 0;
				}
			}
		}
		if (useSkill) {
			skillIconSrcRect.x = 64 * 2;
		}
		if (!useSkill2) {
			if (SDL_GetTicks() - skill2IconEffect > 1000 / 9) {
				skill2IconSrcRect.x = 64 * currentEffect2IconFrame;
				skill2IconEffect = SDL_GetTicks();
				currentEffect2IconFrame++;
				if (currentEffect2IconFrame > 10) {
					currentEffect2IconFrame = 0;
				}
			}
		}
		if (useSkill2) {
			skill2IconSrcRect.x = 64 * 2;
		}
		if (!useSkill3) {
			if (SDL_GetTicks() - skill3IconEffect > 1000 / 9) {
				skill3IconSrcRect.x = 64 * currentEffect3IconFrame;
				skill3IconEffect = SDL_GetTicks();
				currentEffect3IconFrame++;
				if (currentEffect3IconFrame > 10) {
					currentEffect3IconFrame = 0;
				}
			}
		}
		if (useSkill3) {
			skill3IconSrcRect.x = 64 * 2;
		}
		
		controlA->render(g_renderer);
		controlD->render(g_renderer);
		controlS->render(g_renderer);
		controlQ->render(g_renderer);
		controlW->render(g_renderer);
		controlE->render(g_renderer);

		if (animationSkill3Active) {
			Skill3SrcRect.x = 64 * skill3CurrentFrame;
			SDL_RenderCopy(g_renderer, effectSkillTexture, &Skill3SrcRect, &skill3DestRect);
			if (SDL_GetTicks() - skill3AnimatedTime > 1000 / 11) {
				skill3AnimatedTime = SDL_GetTicks();
				skill3CurrentFrame++;
			}
			if (skill2CurrentFrame >= 11) {
				animationSkill2Active = false;
			}
		}

		if (animationSkill2Active) {
			Skill2SrcRect.x = 64 * skill2CurrentFrame;
			SDL_RenderCopy(g_renderer, effectSkillTexture, &Skill2SrcRect, &Skill2DestRect);
			if (SDL_GetTicks() - skill2AnimatedTime > 1000 / 8) {
				skill2AnimatedTime = SDL_GetTicks();
				skill2CurrentFrame++;
			}
			if (skill2CurrentFrame >= 11) {
				animationSkill2Active = false;
			}
		}

		if (animationSkillActive) {
			skillSrcRect.x = 64 * skillCurrentFrame;
			SDL_RenderCopy(g_renderer, effectSkillTexture, &skillSrcRect, &skillDestRect);
			if (SDL_GetTicks() - skillAnimatedTime > 1000 / 11) {
				skillAnimatedTime = SDL_GetTicks();
				skillCurrentFrame++;
			}
			if (skillCurrentFrame >= 11)
				animationSkillActive = false;
		}
		SDL_RenderCopy(g_renderer, shieldEffect, NULL, &shieldEffectRect);
		SDL_RenderCopy(g_renderer, skillBorderTexture, &borderSkillSrcRect, &borderSkillDestRect);
		SDL_RenderCopy(g_renderer, skillBorderTexture, &borderSkillSrcRect, &borderSkill2DestRect);
		SDL_RenderCopy(g_renderer, skillBorderTexture, &borderSkillSrcRect, &borderSkill3DestRect);

		SDL_RenderCopy(g_renderer, guiTexture, NULL, &guiRect);
		SDL_RenderCopy(g_renderer, guiBulletTexture, NULL, &bulletGuiRect);
		SDL_RenderCopy(g_renderer, currentBullet, NULL, &currentBulletGuiRect);
		SDL_RenderCopy(g_renderer, blueBullet, NULL, &blueBulletGuiRect);
		SDL_RenderCopy(g_renderer, redBullet, NULL, &redBulletGuiRect);
		SDL_RenderCopy(g_renderer, greenBullet, NULL, &greenBulletGuiRect);
		SDL_RenderCopy(g_renderer, hpBar, NULL, &hpBarRect);
		SDL_RenderCopy(g_renderer, bulletBar, NULL, &shieldBarRect);
		SDL_RenderCopy(g_renderer, energyBar, NULL, &energyBarRect);

		SDL_RenderCopy(g_renderer, effectSkillTexture, &skillIconSrcRect, &skillIconDestRect);
		SDL_RenderCopy(g_renderer, effectSkillTexture, &skill2IconSrcRect, &skill2IconDestRect);
		SDL_RenderCopy(g_renderer, effectSkillTexture, &skill3IconSrcRect, &skill3IconDestRect);

		guiScore->renderSpecial(g_renderer);
		timeText->render(g_renderer);
		time->renderSpecial(g_renderer);

		SDL_RenderPresent(g_renderer);
		frametime = SDL_GetTicks() - frameStart;

		if (frametime < DELAY_TIME) {
			SDL_Delay((int)(DELAY_TIME - frametime));
		}
	}
	Mix_FreeMusic(backThemeSound);

	SDL_DestroyTexture(g_texture);
	SDL_DestroyTexture(blueBullet);
	SDL_DestroyTexture(greenBullet);
	SDL_DestroyTexture(redBullet);
	SDL_DestroyTexture(currentBullet);
	SDL_DestroyTexture(particleBlue);
	SDL_DestroyTexture(particleRed);
	SDL_DestroyTexture(particleGreen);
	SDL_DestroyTexture(guiTexture);
	SDL_DestroyTexture(redBullet);
	SDL_DestroyTexture(greenBullet);
	SDL_DestroyTexture(blueBullet);
	SDL_DestroyTexture(hpBar);
	SDL_DestroyTexture(energyBar);
	SDL_DestroyTexture(bulletBar);
	SDL_DestroyTexture(guiBulletTexture);
	SDL_DestroyTexture(shieldEffect);
	SDL_DestroyTexture(backgroundImgTexture);
	SDL_DestroyTexture(effectSkillTexture);
	
	for (std::vector<Bullet*>::iterator pBullet = bullets.begin(); pBullet != bullets.end(); pBullet++) {
		delete (*pBullet);
	}
	for (std::vector<Block*>::iterator pBlock = blocks.begin(); pBlock != blocks.end(); pBlock++) {
		delete (*pBlock);
	}
	for (std::vector<Particle*>::iterator pParticle = particles.begin(); pParticle != particles.end(); pParticle++) {
		delete (*pParticle);
	}
	for (std::vector<Text*>::iterator pText = texts.begin(); pText != texts.end(); pText++) {
		delete *pText;
	}

	bullets.clear();
	particles.clear();
	blocks.clear();
	texts.clear();

	delete controlA;
	delete controlD;
	delete controlS;
	delete controlQ;
	delete controlW;
	delete controlE;
	delete guiScore;
	delete time;
	delete timeText;

	controlA = nullptr;
	controlD = nullptr;
	controlS = nullptr; 
	controlW = nullptr;
	controlQ = nullptr;
	controlE = nullptr;
	guiScore = nullptr;
	time = nullptr;
	timeText = nullptr;

	std::string textScore = std::to_string(score);
	Text *gameOver = new Text(ResX / 2 - (500 / 2), (ResY / 2 - (75 / 2)) - 200, 500, 75, Color::WHITE, "-GAME OVER-", 50, g_renderer);
	Text *scoreLost = new Text(ResX / 2 - (500 / 2)-100, (ResY / 2 - (75 / 2)) - 50, 500, 75, Color::WHITE, "SCORE>>", 50, g_renderer);
	Text *ScoreLostText = new Text(ResX / 2 - (500 / 2) + 400, (ResY / 2 - (75 / 2)) - 50, 45, 75, Color::WHITE, textScore.c_str(), 50, g_renderer);
	Text *timeLost = new Text(ResX / 2 - (500 / 2) - 100, (ResY / 2 - (75 / 2)) + 50, 490, 75, Color::WHITE, "TIME    >>", 50, g_renderer);
	Text *timeLostText = new Text(ResX / 2 - (500 / 2) + 400, (ResY / 2 - (75 / 2)) + 50, 45, 75, Color::WHITE, "0", 50, g_renderer);
	Text *pressEnterToBack = new Text(ResX / 2 - (750 / 2), (ResY / 2 - (75 / 2)) + 150, 750, 75, Color::WHITE, "-PRESS ENTER TO GO MAIN MENU-", 50, g_renderer);
	ScoreLostText->SetText(textScore, { 255,255,255 }, g_renderer);
	ScoreLostText->updateNoMove();
	timeLostText->SetText(std::to_string(time_t), { 255,255,255 }, g_renderer);
	timeLostText->updateNoMove();

	SDL_Rect background;
	background.h = ResY;
	background.w = ResX;
	background.x = 0;
	background.y = 0;
	

	SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 128);
	SDL_RenderFillRect(g_renderer, &background);
	gameOver->render(g_renderer);
	scoreLost->render(g_renderer);
	ScoreLostText->render(g_renderer);
	timeLost->render(g_renderer);
	timeLostText->render(g_renderer);
	pressEnterToBack->render(g_renderer);
	SDL_RenderPresent(g_renderer);

	int localLoop = true;
	while (localLoop) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == 0x0D) {
					localLoop = false;
				}
				break;
			default:
				break;
			}
		}
		
	}
	delete gameOver;
	delete scoreLost;
	delete ScoreLostText;
	delete timeLost;
	delete timeLostText;
	delete pressEnterToBack;

	gameOver = nullptr;
	scoreLost = nullptr;
	ScoreLostText = nullptr;
	timeLost = nullptr;
	timeLostText = nullptr;
	pressEnterToBack = nullptr;
}

SDL_Rect CreateCustomRect(int x, int y, int w, int h) {
	SDL_Rect rect;
	rect.h = h;
	rect.w = w;
	rect.x = x;
	rect.y = y;

	return rect;
}