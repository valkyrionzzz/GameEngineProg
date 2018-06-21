#pragma once
#include "Entity.h"
#include "Animation.h"
class Wall :
	public Entity
{
private:
	Animation * animation = NULL;
public:
	Wall();
	~Wall();

	void setAnimation(Animation* animation);

	void update(float dt);
	void draw();
};

