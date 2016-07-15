#include "PlaneLayer.h"
USING_NS_CC;

PlaneLayer* PlaneLayer::planelayer = NULL;
PlaneLayer::PlaneLayer() {}

PlaneLayer* PlaneLayer::getInstance() {
	if ( planelayer == NULL) {
		planelayer = new PlaneLayer();
	}
	return planelayer;
}

void PlaneLayer::createPlane(Speed* &speed_){
	// 相关参数的初始化
	Size visibleSize = Director::getInstance()->getVisibleSize();
	angle = 0;
	is_speedup = false;
	is_invin = false;
	power = 3;
	// 创建对象
	plane = Sprite::create("player.png"); 
	plane->setPosition(Director::getInstance()->getVisibleSize().width / 2, 
		Director::getInstance()->getVisibleSize().height / 2);
	plane->setPhysicsBody(PhysicsBody::createBox(plane->getContentSize()));
	Vec2 pos = plane->getPhysicsBody()->getPosition();
	plane->setScale(0.3);
	plane->getPhysicsBody()->setGroup(-1);
	plane->getPhysicsBody()->setCategoryBitmask(0xFFFFFFFF);
	plane->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
	plane->getPhysicsBody()->setCollisionBitmask(0xFFFFFFFF);
	plane->setTag(1);
	auto move = MoveTo::create(4, ccp(visibleSize.width / 2, visibleSize.height / 2));
	speed_ = Speed::create(move, 3);
	plane->runAction(speed_);
}

Sprite* PlaneLayer::getPlane() {
	return plane;
}

void PlaneLayer::movePlane(float touchX,float touchY,Speed* &speed_) {
	float x = touchX - plane->getPosition().x;
	float y = touchY - plane->getPosition().y;
	angle = atan2f(x, y) * 180 / M_PI;
	plane->setRotation(angle);
	plane->stopAction(speed_);
	auto move = MoveTo::create((sqrt((pow(x, 2) + pow(y, 2))) / 100 + 0.5), ccp(touchX, touchY));
	if (!is_speedup)
		speed_ = Speed::create(move, 3);
	else
		speed_ = Speed::create(move, 5);
	plane->runAction(speed_);
}

void PlaneLayer::speedup_begin() {
	is_speedup = true;
}

void PlaneLayer::speedup_end() {
	is_speedup = false;
}

void PlaneLayer::invin_begin() {
	is_invin = true;
}

void PlaneLayer::invin_end() {
	is_invin = false;
}

bool PlaneLayer::getInvin() {
	return is_invin;
}

void PlaneLayer::getContact() {

}

float PlaneLayer::getAngle() {
	return angle;
}


int PlaneLayer::getPower() {
	return power;
}

void PlaneLayer::getHurt() {
	power--;
}

void PlaneLayer::getCure() {
	power++;
}



