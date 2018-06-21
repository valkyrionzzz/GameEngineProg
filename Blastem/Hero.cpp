#include "Hero.h"


Hero::Hero()
{
	bumpIntoSolids = true;
	solid = true;
	collisionBox.w = 75;
	collisionBox.h = 75;
	hp = 100;
	idle = false;
	canTakeDmg = true;
	gravity = 200;
	deathSound = false;
	coins = 0;
	//knockbackX = 100;
	//knockbackY = 250;
}


Hero::~Hero()
{
}

void Hero::setAnimation(Animation* animation){
	this->animation = animation;
}

//overriding
void Hero::update(float dt){
	//face direction based on velocity.x value
	if (velocity.x >= 0){
		faceRight = true;
	}
	if (velocity.x < 0){
		faceRight = false;
	}

	//check if idle //might not need
	if (velocity.x == 0) {
		this->state = "idle";
	}

	if (hp <= 0) {
		system("cls");
		cout << "dead" << endl;
		if (deathSound == false) {
			SoundManager::soundManager.playSound("heroDeath");
			deathSound = true;
			velocity.y = 200;
			bumpIntoSolids = false;
		}
	}

	if (hp > 0) {
		applyGravity(dt);
	}
	updateCollisions(dt);

	updateMovement(dt);

	animation->update(dt);
}
void Hero::draw(){
	if (animation != NULL){
		if (faceRight)
			animation->draw(pos.x, pos.y);
		else
			animation->draw(pos.x, pos.y, true);
	}
}
void Hero::applyGravity(float dt) {
	if (pos.y < 440) {
		velocity.y += gravity * dt;
	}
	if (pos.y > 450) {
		pos.y = 450;
		velocity.y = 0;
	}
}