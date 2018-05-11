#include "coin.h"


coin::coin()
{
}


coin::~coin()
{
}

void coin::setAnimation(Animation* animation) {
	this->animation = animation;
}

void coin::update(float dt) {
	animation->update(dt);
}

void coin::draw() {
	if (animation != NULL) {
		animation->draw(pos.x, pos.y, true);
	}
}