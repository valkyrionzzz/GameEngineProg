#include "Platform2.h"
#include "Platform.h"

Platform2::Platform2()
{
	bumpIntoSolids = false;
	solid = true;
	collisionBox.w = 32;
	collisionBox.h = 32;
	dmg = 0;
	knockbackX = 50;
	knockbackY = 0;
	type = "plat2";

	velocity.y = -20;
}


Platform2::~Platform2()
{
}

void Platform2::setAnimation(Animation* animation) {
	this->animation = animation;
}

void Platform2::update(float dt)
{
	updateCollisionBox();

	if (pos.y < 10)
		velocity.y = 200;
	if (pos.y > 600)
		velocity.y = -200;

	updateCollisions(dt);

	updateMovement(dt);
}

void Platform2::draw()
{
	if (animation != NULL && poof == false) {
		animation->draw(pos.x, pos.y, true);
	}
}
