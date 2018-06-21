#include "Platform.h"

Platform::Platform()
{
	bumpIntoSolids = false;
	solid = true;
	collisionBox.w = 32;
	collisionBox.h = 32;
	dmg = 0;
	knockbackX = 50;
	knockbackY = 100;
	type = "environment";

	velocity.x = 20;
}


Platform::~Platform()
{
}

void Platform::setAnimation(Animation* animation) {
	this->animation = animation;
}

void Platform::update(float dt)
{
	updateCollisionBox();

	if (pos.x < 10)
		velocity.x = 200;
	if (pos.x > 1000)
		velocity.x = -200;
	
	updateCollisions(dt);

	updateMovement(dt);
}

void Platform::draw()
{
	if (animation != NULL && poof == false) {
		animation->draw(pos.x, pos.y, true);
	}
}
