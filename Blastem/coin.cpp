#include "coin.h"


coin::coin()
{
	bumpIntoSolids = true;
	solid = true;
	collisionBox.w = 1;
	collisionBox.h = 1;
	dmg = 0;

	type = "coin";
}


coin::~coin()
{
}

void coin::setAnimation(Animation* animation) {
	this->animation = animation;
}

void coin::update(float dt) {

	updateCollisionBox();
	updateMovement(dt);
	animation->update(dt);
}

void coin::draw() {
	if (animation != NULL && poof == false) {
		animation->draw(pos.x, pos.y, true);
	}
}