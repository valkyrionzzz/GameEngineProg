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
#include "MouseHandler.h"
#include "coin.h"

using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//texture pointer
SDL_Texture* texture;
SDL_Rect sourceRectangle;
SDL_Rect destinationRectangle;


int main(int argc, char **argv)
{
	//initiate SDL with the subsystems you want to use ie SDL_INIT_VIDEO
	//we're initaliasing all of them (sound, input, video etc)
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL Fail initialised!!!\n";
		return -1; //failed, dont continue rest of main code
	}
	else
	{
		cout << "SDL initialised success!!!\n";
	}

	//NEED TO INIT SDL_Image
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









	
	//LOAD UP WHATEVER ASSETS HERE AFTER INIT

	//background
	SDL_Surface* titleBackground = SDL_LoadBMP("assets/title.bmp");
	SDL_Texture* titleBackgroundTexture = SDL_CreateTextureFromSurface(renderer, titleBackground);
	SDL_FreeSurface(titleBackground);

	//lets use pass by reference to work the width and height of our texture in pixels
	SDL_QueryTexture(titleBackgroundTexture, NULL, NULL, &sourceRectangle.w, &sourceRectangle.h);
	cout << "texture size is : " << sourceRectangle.w << "x" << sourceRectangle.h << endl;

	sourceRectangle.x = 0;
	sourceRectangle.y = 0;
	destinationRectangle.x = 0;
	destinationRectangle.y = 0;
	destinationRectangle.w = sourceRectangle.w; //copy the width of our texture
	destinationRectangle.h = sourceRectangle.h; //copy the height of our texture







	//Zelda Run
	SDL_Texture* runSpriteSheet = IMG_LoadTexture(renderer, "assets/zeldarunSmall.png");
	//this time, lets make a source rectangle copying just 1 frame in size at a time
	SDL_Rect runClipRect = { 0, 0 ,102 ,100 }; //x:0, y:0, w: 32, h:32
	SDL_Rect runDestRect = { 0, 0 ,102 ,100 }; //x y, get image to bottom right of screen. w/h match clip/frame size

	SDL_Surface* runSurface = IMG_Load("assets/zeldarunSmall.png");

	//params: surface, 1/0 to set or unset color key, rgb value to make colour key. mapRGB just does its best to find closest match to a surfaces pixel format e.g 8bit, 16bit, 32bit and what colours are in that set
	SDL_SetColorKey(runSurface, 1, SDL_MapRGB(runSurface->format, 128, 128, 255));

	//now convert to texture
	SDL_Texture* runSpriteSheetWithNoBG = SDL_CreateTextureFromSurface(renderer, runSurface);
	//free the surface 
	SDL_FreeSurface(runSurface);

	//Animation Objects
	Animation zeldarun(runSpriteSheet, renderer, 5, 100, 100, 0.1);//50ms per frame


	//Coins
	SDL_Texture* coins = IMG_LoadTexture(renderer, "assets/coin.png");
	SDL_Rect coinClipRect = { 0, 0 ,102 ,100 }; //x:0, y:0, w: 32, h:32
	SDL_Rect coinDestRect = { 0, 0 ,102 ,100 }; //x y, get image to bottom right of screen. w/h match clip/frame size

	SDL_Surface* coinsSurface = IMG_Load("assets/coin.png");
	SDL_SetColorKey(coinsSurface, 1, SDL_MapRGB(coinsSurface->format, 128, 128, 255));
	SDL_Texture* coinSpriteSheetWithNoBG = SDL_CreateTextureFromSurface(renderer, coinsSurface);

	SDL_FreeSurface(coinsSurface);

	Animation coinCoining(coins, renderer, 14, 36.999999, 100, 0.3);//50ms per frame




	//setup time stuff
	Uint32 lastUpdate = SDL_GetTicks(); //set last update to current time (milliseconds to reach this bit of code)



										//list of game entities
	list<Entity*> entities;
	//build hero
	Hero* hero = new Hero();
	hero->setAnimation(&zeldarun);
	hero->setRenderer(renderer);


	//build coin
	coin* Coin = new coin();
	Coin->setAnimation(&coinCoining);
	Coin->setRenderer(renderer);
	coin* Coin2 = new coin();
	Coin2->setAnimation(&coinCoining);
	Coin2->setRenderer(renderer);
	coin* Coin3 = new coin();
	Coin3->setAnimation(&coinCoining);
	Coin3->setRenderer(renderer);
	coin* Coin4 = new coin();
	Coin4->setAnimation(&coinCoining);
	Coin4->setRenderer(renderer);
	coin* Coin5 = new coin();
	Coin5->setAnimation(&coinCoining);
	Coin5->setRenderer(renderer);
	coin* Coin6 = new coin();
	Coin6->setAnimation(&coinCoining);
	Coin6->setRenderer(renderer);
	
	//build vector to represent starting position for hero
	Vector heroStartPos(200, 460);
	hero->setPosition(heroStartPos);


	//coin starting pos
	Vector coinStartPos(800, 480);
	Coin->setPosition(coinStartPos);
	Vector coinStartPos2(700, 480);
	Coin2->setPosition(coinStartPos2);
	Vector coinStartPos3(600, 480);
	Coin3->setPosition(coinStartPos3);
	Vector coinStartPos4(500, 480);
	Coin4->setPosition(coinStartPos4);
	Vector coinStartPos5(400, 480);
	Coin5->setPosition(coinStartPos5);
	Vector coinStartPos6(300, 480);
	Coin6->setPosition(coinStartPos6);

	//CREATE INPUT HANDLERS
	KeyboardHandler keyboardHandler;
	keyboardHandler.hero = hero; //let it reference our hero

	//MouseHandler mouseHandler;
	//mouseHandler.hero = hero;

	//add our entities to the list
	entities.push_back(Coin);
	entities.push_back(Coin2);
	entities.push_back(Coin3);
	entities.push_back(Coin4);
	entities.push_back(Coin5);
	entities.push_back(Coin6);
	entities.push_back(hero);
		

	//LOAD UP OUR FONT
	TTF_Font* fontTitle = TTF_OpenFont("assets/Mario-Kart-DS.ttf", 50); //params: font file, font size
	TTF_Font* fontEnter = TTF_OpenFont("assets/Mario-Kart-DS.ttf", 25); //params: font file, font size
	SDL_Color titleColor = { 34,139,34, 0 };
	SDL_Color enterColor = { 255,40,0, 0 };

	//title
	SDL_Surface* textSurface = TTF_RenderText_Blended(fontTitle, "Zelda Chronicles", titleColor);
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


	bool loop = true;
	while (loop) {

		//difference is current time running minus the last update time
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		//convert into deltaTime (decimal)
		float DT = timeDiff / 1000.0; //e.g 200ms now = 0.2 for DT
									  //now update lastUpdate to the current time so the next loop calculates correctly
		lastUpdate = SDL_GetTicks();

		//set drawing colour for the renderer, numbers are: RGBA(alpha, transparency). All values between 0-255
		SDL_SetRenderDrawColor(renderer, 255, 0, 168, 255);
		//clear screen with current draw colour
		SDL_RenderClear(renderer);

		//menu background todo  & ui
		//set drawing colour to a colour
		//SDL_SetRenderDrawColor(renderer, 0, 228, 255, 255);
		//SDL_Rect rect = { 100, 50, 300, 240 };
		//render rectangle to screen (renderer and a rectangle for positioning)
		//SDL_RenderFillRect(renderer, &rect);

		//DRAW OUR 2D IMAGES HERE
		SDL_RenderCopy(renderer, titleBackgroundTexture, &sourceRectangle, &destinationRectangle);
						
		/*
		anim2.draw(50, 100, 4.0f); //4.0f is scale, stretch by 4x
		anim3.draw(50, 250, true); //flip image horizontally???
		*/

		//DEAL WITH USER INPUT
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



		//loop through and update and draw all entities
		for (list<Entity*>::iterator eIt = entities.begin(); eIt != entities.end(); eIt++) {
			(*eIt)->update(DT);
			(*eIt)->draw();
		}

		//DRAW TEXT ONTOP
		//params: renderer(drawing object), texture to render, srcRectangle(null means use full texture, using an sdl_rect will choose a clip of texture), destination sdl_rect(where to draw, if NULL, fill entire window) 
		SDL_RenderCopy(renderer, textTexture, NULL, &textDestination);
		SDL_RenderCopy(renderer, enterTexture, NULL, &enterDestination);

		//get renderer to output to the window
		SDL_RenderPresent(renderer);

		//sdl_ticks checks how many milliseconds since we started running our game
		/*if (SDL_GetTicks() > 5000)
		{
		loop = false;
		}*/
	}
	//SDL_Delay pauses game for x many milliseconds
	//SDL_Delay(5000); //5000ms = 5secs

	//clean up any game objects
	delete hero;
	delete Coin;
	delete Coin2;
	delete Coin3;
	delete Coin4;
	delete Coin5;
	delete Coin6;

	//cleanup font
	TTF_CloseFont(fontTitle);
	TTF_CloseFont(fontEnter);
	//clean up textures
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(enterTexture);
	SDL_DestroyTexture(runSpriteSheet);
	SDL_DestroyTexture(runSpriteSheetWithNoBG);
	SDL_DestroyTexture(coins);
	SDL_DestroyTexture(titleBackgroundTexture);

	//clean up renderer and window properly (aka clean up dynamic memory)
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//shut down subsystems and cleanup any setup it did earlier in sdl_init
	SDL_Quit();

	system("pause");

	return 0;
}
