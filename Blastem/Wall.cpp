#include "Wall.h"

Wall::Wall()
{
	bumpIntoSolids = false;
	solid = true;
	collisionBox.w = 32;
	collisionBox.h = 32;
	dmg = 10;
	knockbackX = 50;
	knockbackY = 50;
	type = "environment";
}


Wall::~Wall()
{
}
void Wall::setAnimation(Animation* animation) {
	this->animation = animation;}

void Wall::update(float dt)
{
	updateCollisionBox();
}

void Wall::draw()
{
	if (animation != NULL && poof == false) {
		animation->draw(pos.x, pos.y, true);
	}
}
