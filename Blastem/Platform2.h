#pragma once
#include "Entity.h"
#include "Animation.h"
class Platform2 :
	public Entity
{
private:
	Animation * animation = NULL;
public:
	Platform2();
	~Platform2();

	void setAnimation(Animation* animation);

	void update(float dt);
	void draw();
};

