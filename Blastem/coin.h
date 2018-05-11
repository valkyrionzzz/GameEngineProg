#pragma once
#include "Entity.h"
#include "Animation.h"
class coin :
	public Entity
{
private:
	Animation * animation = NULL;
public:
	coin();
	~coin();

	void setAnimation(Animation* animation);

	virtual void update(float dt);
	virtual void draw();
};

