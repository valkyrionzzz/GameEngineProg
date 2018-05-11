#pragma once
#include "Entity.h"
#include "Animation.h"
#include <string>
class Hero :
	public Entity
{
private:
	Animation* animation = NULL;
	bool faceRight = true;
	std::string state = "";
public:
	Hero();
	~Hero();
	
	void setAnimation(Animation* animation);

	//overriding
	virtual void update(float dt);
	virtual void draw();
};

