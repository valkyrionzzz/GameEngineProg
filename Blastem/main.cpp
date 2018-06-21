#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "Animation.h"
#include "Entity.h"
#include "Hero.h"
#include <list>
#include "InputHandler.h"
#include "KeyboardHandler.h"
#include "coin.h"
#include <SDL_mixer.h>
#include "SoundManager.h"
#include "Spikes.h"
#include "Wall.h"
#include "Platform.h"
#include "Platform2.h"

#include <stdlib.h>
#include <time.h> 

using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//texture pointer
SDL_Texture* texture;
SDL_Rect sourceRectangle;
SDL_Rect destinationRectangle;

int main(int argc, char **argv)
{
	//check level
	bool lvl2 = false;
	bool lvl3 = false;

	//initaliasing sound, input, video etc
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL Fail initialised!!!\n";
		return -1;
	}
	else
	{
		cout << "SDL initialised success!!!\n";
	}

	//INIT SDL_Image
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
		cout << "sdl image did not load: " << IMG_GetError() << endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	//create window, params are: window title, window pos x, pos y, width, height, window flags
	window = SDL_CreateWindow("BLASTEM!!!", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 1024, 614, SDL_WINDOW_SHOWN /*| SDL_WINDOW_FULLSCREEN*/);

	if (window != NULL){
		cout << "Window created!" << endl;
	}
	else
	{
		cout << "Failed to create window!" << endl;
		return -1;
	}

	//create renderer to help draw stuff to the screen
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer != NULL)
	{
		cout << "Renderer created!" << endl;
	}
	else
	{
		cout << "Renderer FAILED!" << endl;
		return -1;
	}

	//INIT sdl ttf
	if (TTF_Init() != 0)
	{
		//if failed, complain about it
		cout << "SDL TTF FAILED!" << endl;
		system("pause");
		SDL_Quit();
		return -1;
	}

	//INIT MIXER
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		cout << "mixer didn't initialise" << endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	//------------------------------------------------AUDIO-----------------------------------------------------

	Mix_Music* music = Mix_LoadMUS("assets/fairyTheme.ogg");
	if (music == NULL) {
		cout << "Music failed to load!!!" << endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	//assets/songOfStorms.ogg--Unused Asset - to use later

	Mix_PlayMusic(music, -1);

	SoundManager::soundManager.loadSound("heroDeath", "assets/deathSound.ogg");
	SoundManager::soundManager.loadSound("coinSound", "assets/coin.ogg");
	
	//------------------------------------------------LOAD ASSETS-----------------------------------------------------

	//-----LOAD BACKGROUND-----------------------------------------------------

	//load background
	SDL_Surface* titleBackground = SDL_LoadBMP("assets/title.bmp");
	SDL_Texture* titleBackgroundTexture = SDL_CreateTextureFromSurface(renderer, titleBackground);
	SDL_FreeSurface(titleBackground);

	SDL_QueryTexture(titleBackgroundTexture, NULL, NULL, &sourceRectangle.w, &sourceRectangle.h);
	cout << "texture size is : " << sourceRectangle.w << "x" << sourceRectangle.h << endl;

	sourceRectangle.x = 0;
	sourceRectangle.y = 0;
	destinationRectangle.x = 0;
	destinationRectangle.y = 0;
	destinationRectangle.w = sourceRectangle.w; //copy the width of our texture
	destinationRectangle.h = sourceRectangle.h; //copy the height of our texture

	//-----LOAD ANIMATIONS-----------------------------------------------------
	
	//-----Zelda Run-------------------
	SDL_Texture* runSpriteSheet = IMG_LoadTexture(renderer, "assets/zeldarunSmall.png");
	//this time, lets make a source rectangle copying just 1 frame in size at a time
	SDL_Rect runClipRect = { 0, 0 ,102 ,100 }; //x:0, y:0, w: 32, h:32
	SDL_Rect runDestRect = { 0, 0 ,102 ,100 }; //x y, get image to bottom right of screen. w/h match clip/frame size

	SDL_Surface* runSurface = IMG_Load("assets/zeldarunSmall.png");

	//params: surface, 1/0 to set or unset color key, rgb value to make colour key. mapRGB just does its best to find closest match to a surfaces pixel format e.g 8bit, 16bit, 32bit and what colours are in that set
	SDL_SetColorKey(runSurface, 1, SDL_MapRGB(runSurface->format, 128, 128, 255));

	//now convert to texture
	SDL_Texture* runSpriteSheetWithNoBG = SDL_CreateTextureFromSurface(renderer, runSurface);

	SDL_FreeSurface(runSurface);

	//Animation Objects
	Animation zeldarun(runSpriteSheet, renderer, 5, 100, 100, 0.1);//50ms per frame
	
	//------Coins--------------------
	SDL_Texture* coins = IMG_LoadTexture(renderer, "assets/coin.png");
	SDL_Rect coinClipRect = { 0, 0 ,10 ,10 }; //x:0, y:0, w: 32, h:32
	SDL_Rect coinDestRect = { 0, 0 ,10 ,10 }; //x y, get image to bottom right of screen. w/h match clip/frame size

	SDL_Surface* coinsSurface = IMG_Load("assets/coin.png");
	SDL_SetColorKey(coinsSurface, 1, SDL_MapRGB(coinsSurface->format, 128, 128, 255));
	SDL_Texture* coinSpriteSheetWithNoBG = SDL_CreateTextureFromSurface(renderer, coinsSurface);

	SDL_FreeSurface(coinsSurface);

	Animation coinCoining(coins, renderer, 14, 35, 30, 0.3);//50ms per frame

	//------Spike-------------------
	SDL_Texture* spikes = IMG_LoadTexture(renderer, "assets/spike.png");
	SDL_Rect spikesClipRect = { 0, 0 ,50 ,50 }; //x:0, y:0, w: 32, h:32
	SDL_Rect spikesDestRect = { 0, 0 ,50 ,50 }; //x y, get image to bottom right of screen. w/h match clip/frame size

	SDL_Surface* spikesSurface = IMG_Load("assets/spike.png");
	SDL_SetColorKey(spikesSurface, 1, SDL_MapRGB(spikesSurface->format, 128, 128, 255));
	SDL_Texture* spikesSpriteSheetWithNoBG = SDL_CreateTextureFromSurface(renderer, spikesSurface);

	SDL_FreeSurface(spikesSurface);

	Animation spikesSpiking(spikes, renderer, 1, 50, 50, 0.3);//50ms per frame
	
	//------Shoveller----------------
	SDL_Texture* shovel = IMG_LoadTexture(renderer, "assets/shovelknight.png");
	SDL_Rect shovelClipRect = { 0, 0 ,100 ,100 }; //x:0, y:0, w: 32, h:32
	SDL_Rect shovelDestRect = { 0, 0 ,100 ,100 }; //x y, get image to bottom right of screen. w/h match clip/frame size

	SDL_Surface* shovelSurface = IMG_Load("assets/spike.png");
	SDL_SetColorKey(shovelSurface, 1, SDL_MapRGB(shovelSurface->format, 128, 128, 255));
	SDL_Texture* shovelSpriteSheetWithNoBG = SDL_CreateTextureFromSurface(renderer, shovelSurface);

	SDL_FreeSurface(shovelSurface);

	Animation shovelShoveling(shovel, renderer, 1, 100, 100, 0.3);//50ms per frame
	
	//-----Platformer----------------
	SDL_Texture* platform = IMG_LoadTexture(renderer, "assets/platform.png");
	SDL_Rect platformlClipRect = { 0, 0 ,100 ,65 }; //x:0, y:0, w: 32, h:32
	SDL_Rect platformDestRect = { 0, 0 ,100 ,65 }; //x y, get image to bottom right of screen. w/h match clip/frame size

	SDL_Surface* platformSurface = IMG_Load("assets/platform.png");
	SDL_SetColorKey(platformSurface, 1, SDL_MapRGB(platformSurface->format, 128, 128, 255));
	SDL_Texture* platformSpriteSheetWithNoBG = SDL_CreateTextureFromSurface(renderer, platformSurface);

	SDL_FreeSurface(platformSurface);

	Animation platPlatforming(platform, renderer, 1, 100, 65, 0.3);//50ms per frame

	//-----------------------------------------------------------------------------------------------------

	//setup time stuff
	Uint32 lastUpdate = SDL_GetTicks(); //set last update to current time (milliseconds to reach this bit of code)

	//------------------------------------------------BUILD GAME OBJECTS-----------------------------------------------------

	//list of game entities
	list<Entity*> entities;
	Entity::entities = &entities;

	//------build hero---------------------------
	Hero* hero = new Hero();
	hero->setAnimation(&zeldarun);
	hero->setRenderer(renderer);

	//build vector to represent starting position for hero
	Vector heroStartPos(415, 460);
	hero->setPosition(heroStartPos);

	//------Make spikes---------------------------
	Spikes *spike1 = new Spikes();
	spike1->setAnimation(&spikesSpiking);
	spike1->setRenderer(renderer);

	spike1->pos.x = 10;
	spike1->pos.y = 440;

	entities.push_back(spike1);

	//------make shoveller---------------------------
	Wall *wall1 = new Wall();
	wall1->setAnimation(&shovelShoveling);
	wall1->setRenderer(renderer);

	wall1->pos.x = 900;
	wall1->pos.y = 450;

	entities.push_back(wall1);

	//------Make platform---------------------------
	Platform *plat1 = new Platform();
	plat1->setAnimation(&platPlatforming);
	plat1->setRenderer(renderer);

	plat1->pos.x = 10;
	plat1->pos.y = 300;

	entities.push_back(plat1);

	//plat2--
	Platform *plat2 = new Platform();
	plat2->setAnimation(&platPlatforming);
	plat2->setRenderer(renderer);

	plat2->pos.x = 445;
	plat2->pos.y = 480;

	entities.push_back(plat2);

	//-------build coin---------------------------
	for (int i(0); i < 5; i++){
		//START FUNC
			coin* ctemp = new coin();
			ctemp->setAnimation(&coinCoining);
			ctemp->setRenderer(renderer);
			//set position

			srand(time(NULL)+i);
			int roll = rand() % 5 + 1;
			if (roll == 1){
				Vector coinStartPos(200, 480);
				ctemp->setPosition(coinStartPos);
			}
			else if (roll == 2){
				Vector coinStartPos(600, 480);
				ctemp->setPosition(coinStartPos);
			}
			else if (roll == 3){
				Vector coinStartPos(400, 480);
				ctemp->setPosition(coinStartPos);
			}
			else if (roll == 4){
				Vector coinStartPos(800, 510);
				ctemp->setPosition(coinStartPos);
			}
			else{
				Vector coinStartPos(600, 510);
				ctemp->setPosition(coinStartPos);
			}
			entities.push_back(ctemp);
		//add to list of entities
		//END FUNC
	}
	//coin* Coin = new coin();
	//Coin->setAnimation(&coinCoining);
	//Coin->setRenderer(renderer);
	//coin* Coin2 = new coin();
	//Coin2->setAnimation(&coinCoining);
	//Coin2->setRenderer(renderer);
	//coin* Coin3 = new coin();
	//Coin3->setAnimation(&coinCoining);
	//Coin3->setRenderer(renderer);
	//coin* Coin4 = new coin();
	//Coin4->setAnimation(&coinCoining);
	//Coin4->setRenderer(renderer);
	//coin* Coin5 = new coin();
	//Coin5->setAnimation(&coinCoining);
	//Coin5->setRenderer(renderer);
	//coin* Coin6 = new coin();
	//Coin6->setAnimation(&coinCoining);
	//Coin6->setRenderer(renderer);

	//coin starting pos
	//Vector coinStartPos(800, 480);
	//Coin->setPosition(coinStartPos);
	//Vector coinStartPos2(700, 480);
	//Coin2->setPosition(coinStartPos2);
	//Vector coinStartPos3(600, 480);
	//Coin3->setPosition(coinStartPos3);
	//Vector coinStartPos4(500, 480);
	//Coin4->setPosition(coinStartPos4);
	//Vector coinStartPos5(400, 480);
	//Coin5->setPosition(coinStartPos5);
	//Vector coinStartPos6(300, 480);
	//Coin6->setPosition(coinStartPos6);

	//-----------------------------------------------------------------------------------------------------

	//CREATE INPUT HANDLERS
	KeyboardHandler keyboardHandler;
	keyboardHandler.hero = hero; //let it reference our hero

	//MouseHandler mouseHandler;		//implement later
	//mouseHandler.hero = hero;			//implement later

	//--------------------------------------------------ADD ENTITIES TO LIST---------------------------------------------------

	//add our entities to the list
	//entities.push_back(Coin);
	//entities.push_back(Coin2);
	//entities.push_back(Coin3);
	//entities.push_back(Coin4);
	//entities.push_back(Coin5);
	//entities.push_back(Coin6);
	entities.push_back(hero);
		
	//--------------------------------------------------FONT/TITLE---------------------------------------------------

	//LOAD UP OUR FONT
	TTF_Font* fontTitle = TTF_OpenFont("assets/Mario-Kart-DS.ttf", 50); //params: font file, font size
	TTF_Font* fontEnter = TTF_OpenFont("assets/Mario-Kart-DS.ttf", 25); //params: font file, font size
	SDL_Color titleColor = { 34,139,34, 0 };
	SDL_Color enterColor = { 255,40,0, 0 };

	//title
	SDL_Surface* textSurface = TTF_RenderText_Blended(fontTitle, "Legend of Link", titleColor);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);

	//press enter
	SDL_Surface* enterSurface = TTF_RenderText_Blended(fontEnter, "Insert Coin", enterColor);
	SDL_Texture* enterTexture = SDL_CreateTextureFromSurface(renderer, enterSurface);
	SDL_FreeSurface(enterSurface);
	
	//title location
	SDL_Rect textDestination;
	textDestination.x = 175;
	textDestination.y = 75;
	SDL_QueryTexture(textTexture, NULL, NULL, &textDestination.w, &textDestination.h);

	//enter location
	SDL_Rect enterDestination;
	enterDestination.x = 390;
	enterDestination.y = 350;
	SDL_QueryTexture(enterTexture, NULL, NULL, &enterDestination.w, &enterDestination.h);

	//--------------------------------------------MAIN GAME LOOP---------------------------------------------------------

	bool loop = true;
	while (loop) {
		//DIFFICULTY
		if (hero->coins > 20 && lvl2 == false) {
			spike1->velocity.x = spike1->velocity.x * 2 ;
			lvl2 = true;
		}
		if (hero->coins > 40 && lvl3 == false) {
			spike1->velocity.x = spike1->velocity.x * 2;
			lvl3 = true;
		}

		//difference is current time running minus the last update time
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		//convert into deltaTime (decimal)
		float DT = timeDiff / 1000.0; //e.g 200ms now = 0.2 for DT
									  //now update lastUpdate to the current time so the next loop calculates correctly
		lastUpdate = SDL_GetTicks();

		//----------------------------------------------RENDERER--------------------------

		//set drawing colour for the renderer, numbers are: RGBA(alpha, transparency). All values between 0-255
		SDL_SetRenderDrawColor(renderer, 255, 0, 168, 255);
		//clear screen with current draw colour
		SDL_RenderClear(renderer);
		
		//DRAW OUR 2D IMAGES HERE
		SDL_RenderCopy(renderer, titleBackgroundTexture, &sourceRectangle, &destinationRectangle);
						
		/*
		anim2.draw(50, 100, 4.0f); //4.0f is scale, stretch by 4x	//code to manipualte image
		anim3.draw(50, 250, true); //flip image horizontally???
		*/

		//----------------------------------------------DEAL WITH USER INPUT----------------------

		//we check what kind of user input events have happened since our last check
		SDL_Event e;
		//loops through all events and temporarily stores event details in an SDL_Event object
		while (SDL_PollEvent(&e))
		{
			//check if user has clicked on the close window button
			if (e.type == SDL_QUIT) {
				//exit our loop,
				loop = false;
			}
			//check if user has 'pressed' a button(not held)
			if (e.type == SDL_KEYDOWN) {
				//see if ESC key was pressed
				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					//exit loop
					loop = false;
				}

			}			
				keyboardHandler.update(&e);
		}

		//----------------------------------------------DRAW ALL ENTITIES / POOF PLACE--------------------------

		//loop through and update and draw all entities
		for (list<Entity*>::iterator eIt = entities.begin(); eIt != entities.end(); ) {
			(*eIt)->update(DT);
			(*eIt)->draw();
			if ((*eIt)->type == "coin" && (*eIt)->poof == true)
				{
					SoundManager::soundManager.playSound("coinSound");
					delete (*eIt);
					eIt = entities.erase(eIt);
					int heroCoins = hero->coins;
					heroCoins++;
					hero->coins = heroCoins;
					
					//turn this into function

					coin* ctemp = new coin();
					ctemp->setAnimation(&coinCoining);
					ctemp->setRenderer(renderer);
					//set position

					srand(time(NULL));
					int roll = rand() % 5 + 1;
					if (roll == 1){
						Vector coinStartPos(800, 480);
						ctemp->setPosition(coinStartPos);
					}
					else if (roll == 2){
						Vector coinStartPos(600, 480);
						ctemp->setPosition(coinStartPos);
					}
					else if (roll == 3){
						Vector coinStartPos(400, 480);
						ctemp->setPosition(coinStartPos);
					}
					else if (roll == 4){
						Vector coinStartPos(800, 510);
						ctemp->setPosition(coinStartPos);
					}
					else{
						Vector coinStartPos(600, 510);
						ctemp->setPosition(coinStartPos);
					}
					entities.push_back(ctemp);
				}
			else{
				eIt++;
			}
			/*
				 

			if ((*eIt)->type == "plat2")					// y axis plat (implement later)
			{
				hero->velocity.y = (*eIt)->velocity.y;
			}*/
		}

		//----------------------------------------------DISPLAY HERO HP--------------------------				//TODO SHOW AS HEARTS <3
		int heroHP(0);

		heroHP = hero->hp;

		string printString("");
		printString = "Heroes Hp    " + to_string(heroHP);

		const char * finalPrintString = printString.c_str();

		SDL_Color Black = { 0, 0, 0, 255 };
		SDL_Surface* coordSurface = TTF_RenderText_Blended(fontEnter, finalPrintString, Black);
		SDL_Texture* coordTexture = SDL_CreateTextureFromSurface(renderer, coordSurface);
		SDL_FreeSurface(coordSurface);

		SDL_Rect coordTextDestination;
		coordTextDestination.x = 0;
		coordTextDestination.y = 150;

		SDL_QueryTexture(coordTexture, NULL, NULL, &coordTextDestination.w, &coordTextDestination.h);
		SDL_RenderCopy(renderer, coordTexture, NULL, &coordTextDestination);

		//----------------------------------------------DISPLAY Coins--------------------------
		int heroCoins(0);

		heroCoins = hero->coins;

		string printString2("");
		printString2 = "Coins    " + to_string(heroCoins);

		const char * finalPrintString2 = printString2.c_str();

		SDL_Surface* coordSurface2 = TTF_RenderText_Blended(fontEnter, finalPrintString2, Black);
		SDL_Texture* coordTexture2 = SDL_CreateTextureFromSurface(renderer, coordSurface2);
		SDL_FreeSurface(coordSurface2);

		SDL_Rect coordTextDestination2;
		coordTextDestination2.x = 0;
		coordTextDestination2.y = 190;
		
		SDL_QueryTexture(coordTexture2, NULL, NULL, &coordTextDestination2.w, &coordTextDestination2.h);
		SDL_RenderCopy(renderer, coordTexture2, NULL, &coordTextDestination2);


		//----------------------------------------------DRAW TEXT ONTOP--------------------------
		//params: renderer(drawing object), texture to render, srcRectangle(null means use full texture, using an sdl_rect will choose a clip of texture), destination sdl_rect(where to draw, if NULL, fill entire window) 
		SDL_RenderCopy(renderer, textTexture, NULL, &textDestination);
		SDL_RenderCopy(renderer, enterTexture, NULL, &enterDestination);

		//get renderer to output to the window
		SDL_RenderPresent(renderer);

		//sdl_ticks checks how many milliseconds since we started running our game					//use this for time relate codes
		/*if (SDL_GetTicks() > 5000)
		{
		loop = false;
		}*/
	}
	//SDL_Delay pauses game for x many milliseconds													//use this code for pausing
	//SDL_Delay(5000); //5000ms = 5secs

	//--------------------------------------------DELETE GAME OBJECTS---------------------------------------------------------

	for (list<Entity*>::iterator eIt = entities.begin(); eIt != entities.end();eIt++) {
			delete (*eIt);
			//entities.erase(eIt);
	}	

	entities.clear();

	//stop music from playing
	Mix_PauseMusic();
	//delete song memory
	Mix_FreeMusic(music);
	//cleanup font
	TTF_CloseFont(fontTitle);
	TTF_CloseFont(fontEnter);
	//clean up textures
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(enterTexture);
	SDL_DestroyTexture(runSpriteSheet);
	SDL_DestroyTexture(runSpriteSheetWithNoBG);
	SDL_DestroyTexture(coins);
	SDL_DestroyTexture(coinSpriteSheetWithNoBG);
	SDL_DestroyTexture(spikes);
	SDL_DestroyTexture(spikesSpriteSheetWithNoBG);
	SDL_DestroyTexture(shovel);
	SDL_DestroyTexture(shovelSpriteSheetWithNoBG);
	SDL_DestroyTexture(titleBackgroundTexture);
	SDL_DestroyTexture(platformSpriteSheetWithNoBG);
	SDL_DestroyTexture(platform);

	//clean up renderer and window properly (aka clean up dynamic memory)
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//shut down subsystems and cleanup any setup it did earlier in sdl_init
	SDL_Quit();

	system("pause");

	return 0;
}
