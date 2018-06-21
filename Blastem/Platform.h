#pragma once
#include "Entity.h"
#include "Animation.h"
class Platform :
	public Entity
{
private:
	Animation * animation = NULL;
public:
	Platform();
	~Platform();

	void setAnimation(Animation* animation);

	void update(float dt);
	void draw();
};

