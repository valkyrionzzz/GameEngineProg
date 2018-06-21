#include "KeyboardHandler.h"


KeyboardHandler::KeyboardHandler()
{
}


KeyboardHandler::~KeyboardHandler()
{
}

void KeyboardHandler::update(SDL_Event* event) {
	//button presses for our hero
	if (event->type == SDL_KEYDOWN) {
		//reset hero position if press R
		if (event->key.keysym.scancode == SDL_SCANCODE_R) {
			hero->pos.x = 400;
			hero->pos.y = 400;
		}
		if (event->key.keysym.scancode == SDL_SCANCODE_UP) {
			//jump
			if (hero->velocity.y == 0)
			{
				hero->velocity.y = -230;
				cout << "jump triggered ";

			}
			else {
				cout << "velocity y = " << hero->velocity.y << endl;
			}
		}
		//TODO add attacks or other actions here for button presses
	}

	//check if a button is held down
	//first get state of all keyboard buttons (1 pressed, 0 not pressed)
	const Uint8 *keystates = SDL_GetKeyboardState(NULL); //you can pass it a smaller array, or null to return all keys
														 //down
	if (keystates[SDL_SCANCODE_DOWN])
		hero->velocity.y = 180;
	//left
	if (keystates[SDL_SCANCODE_LEFT])
		hero->velocity.x = -180;
	//right
	if (keystates[SDL_SCANCODE_RIGHT])
		hero->velocity.x = 180;
	//check if none are held down
	if (!keystates[SDL_SCANCODE_UP] && !keystates[SDL_SCANCODE_DOWN])
	{
		//then stop moving
		//hero->velocity.y = 0;
	}
	if (!keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT])
	{
		hero->velocity.x = 0;
	}
	if (!keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_UP] && !keystates[SDL_SCANCODE_DOWN] && !keystates[SDL_SCANCODE_RIGHT])
		hero->idle = true;
	else
		hero->idle = false;


}