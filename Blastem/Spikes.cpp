#include "Spikes.h"

Spikes::Spikes()
{
	bumpIntoSolids = true;
	solid = true;
	collisionBox.w = 32;
	collisionBox.h = 32;
	dmg = 1000;
	knockbackX = 0;
	knockbackY = 250;

	velocity.x = 100;
	type = "environment";
}


Spikes::~Spikes()
{
}

void Spikes::setAnimation(Animation* animation) {
	this->animation = animation;
}
void Spikes::update(float dt)
{
	updateCollisionBox();

	if (pos.x < 10)
		velocity.x = 200;
	if (pos.x > 800)
		velocity.x = -200;

	updateCollisions(dt);

	updateMovement(dt);
}

void Spikes::draw()
{
	if (animation != NULL && poof == false) {
		animation->draw(pos.x, pos.y, true);
	}
}
