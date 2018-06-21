#pragma once
#include "Entity.h"
#include "Animation.h"
class Spikes :
	public Entity
{
private:
	Animation * animation = NULL;
public:
	Spikes();
	~Spikes();

	void setAnimation(Animation* animation);

	void update(float dt);
	void draw();
};

