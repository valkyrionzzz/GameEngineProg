#pragma once
#include "Entity.h"
#include "Animation.h"
#include "SoundManager.h"
#include <string>
class Hero :
	public Entity
{
private:
	Animation* animation = NULL;
	bool faceRight = true;
	std::string state = "";//might not need
	bool deathSound;
	float gravity;
public:
	Hero();
	~Hero();

	bool idle = false;
	bool onFloor = false;
	int coins = 0;

	void setAnimation(Animation* animation);
	void applyGravity(float dt);

	//overriding
	virtual void update(float dt);
	virtual void draw();
};

