#include"game.h"
#include"Fail.h"
#include"Win.h"
#include "floatItem.h"
#include "SimpleAudioEngine.h"
#include "PlaneLayer.h"
#include "Enemy.h"
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>
USING_NS_CC;
using namespace CocosDenshion;
using namespace std;
using namespace cocos2d;

Scene* game::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2::ZERO);
	auto layer = game::create(scene->getPhysicsWorld());
	scene->addChild(layer);
	// return the scene
	return scene;
}

game* game::create(PhysicsWorld* world)
{
	game* pRet = new game();
	if (pRet && pRet->init(world))
	{
		return pRet;
	}
	pRet = NULL;
	return NULL;
}

bool game::init(PhysicsWorld* world)
{
	if (!Layer::init())
	{
		return false;
	}
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.wav", true);

	clicked = false;
	Power = 3;
	Score = 0;
	angle = 0;
	boss_power = 20; // xz
	boss = 0;
	bullet_power = 1;
	last_size = 0;
	is_invin = false;
	is_speedup = false;
	speed_ = NULL;

	myItems = FloatItem::getInstance();
	
	myPlane = PlaneLayer::getInstance();
    myPlane->createPlane(speed_);
	auto newplane = myPlane->getPlane();
	this->addChild(newplane, 2);

	myEnemy = Enemy::getInstance();


	dispatcher = Director::getInstance()->getEventDispatcher();
	visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	label_score = Label::createWithSystemFont("Your Points: 0", "Microsoft Yahei", 30);
	label_score->setPosition(visibleSize.width / 2, visibleSize.height - 50);
	this->addChild(label_score, 1);

	auto *game_bg = Sprite::create("background.jpg");
	float scale_x = visibleSize.width / game_bg->getContentSize().width;
	float scale_y = visibleSize.height / game_bg->getContentSize().height;
	game_bg->setScaleX(scale_x);
	game_bg->setScaleY(scale_y);
	game_bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(game_bg, 0);

	testTouchEvent();
	testKeyboardEvent();
	testContact(myPlane->getAngle());
	this->schedule(schedule_selector(game::new_enemy), 3.0f);
	this->schedule(schedule_selector(game::crateboss), 1.0f);
	this->schedule(schedule_selector(game::boss_shoot), 3.0f);
	this->schedule(schedule_selector(game::new_stone), 8.0f); 
	this->schedule(schedule_selector(game::bullet_delete), 0.00001f);
	this->schedule(schedule_selector(game::enemy_moving), 0.1f);
	this->schedule(schedule_selector(game::Win_Fail), 0.5f);
	this->schedule(schedule_selector(game::item_move), 5.0f); 
	this->schedule(schedule_selector(game::judge_remove), 0.1f);
	this->schedule(schedule_selector(game::getBuff), 0.1f);

	auto closeItem = MenuItemImage::create(
		"button.png",
		"button.png",
		CC_CALLBACK_1(game::skill, this));

	auto shootItem = MenuItemFont::create("Shoot",
		CC_CALLBACK_1(game::shoot, this));
	shootItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2 + 100));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	left = CCProgressTimer::create(CCSprite::create("blood.png"));
	left->setType(kCCProgressTimerTypeBar);
	left->setMidpoint(ccp(0, 0));
	left->setBarChangeRate(ccp(1, 0));
	left->setPosition(left->getContentSize().width - 100, visibleSize.height - left->getContentSize().height);
	left->setPercentage(100.0);
	left->setScale(0.8);
	this->addChild(left, 1);


	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->addChild(shootItem);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	
	return true;
}

void game::testTouchEvent()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch* touch, Event* event){
		touch_pos = touch->getLocation();
		scheduleOnce(schedule_selector(game::single_click), 0.2f);
		return true;
	};
	listener->onTouchMoved = [&](Touch* touch, Event* event){};
	listener->onTouchEnded = [&](Touch* touch, Event* event){};
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void game::testKeyboardEvent() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode code, Event* event) {
		if (code == cocos2d::EventKeyboard::KeyCode::KEY_SPACE) {
			shoot(this);
		}
	};
	dispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void game::single_click(float tt) {
	
	myPlane->movePlane(touch_pos.x,touch_pos.y,speed_);

}

void game::item_move(float tt) {
	myItems->moveItem();
}

void game::update_score() {
	std::string score = "Your Points: ";
	score += to_string(Score);
	label_score->setString(score);
}

void game::shoot(Ref* pSender) {
	auto bullet = Sprite::create("bullet.png");
	bullet->setPhysicsBody(PhysicsBody::createBox(bullet->getContentSize()));
	bullet->getPhysicsBody()->setGroup(-1);
	bullet->getPhysicsBody()->setCategoryBitmask(0xFFFFFFFF);
	bullet->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
	bullet->getPhysicsBody()->setCollisionBitmask(0xFFFFFFFF);
	bullet->setTag(20);
	float x = myPlane->getPlane()->getPositionX();
	float y = myPlane->getPlane()->getPositionY();
	bullet->setPosition(x, y);
	bullet->setScale(0.5);
	this->addChild(bullet, 1);
	bullets.pushBack(bullet);
	SimpleAudioEngine::getInstance()->playEffect("music/bulletvoice.wav");

	bullet->setRotation(myPlane->getAngle());
	float end_x;
	float end_y;
	float len_x;
	float len_y;
	float angle_;
	if (myPlane->getAngle() == 0) {
		end_x = x;
		end_y = visibleSize.height;
	} else if (myPlane->getAngle() == 90) {
		end_x = visibleSize.width;
		end_y = y;
	} else if (myPlane->getAngle() == 180) {
		end_x = x;
		end_y = 0;
	} else if (myPlane->getAngle() == -90) {
		end_x = 0;
		end_y = y;
	} else if (myPlane->getAngle() > 0 && myPlane->getAngle() < 90) {
		len_y = visibleSize.height - y;
		len_x = visibleSize.width - x;
		angle_ = atan2f(len_x, len_y) * 180 / M_PI;
		if (myPlane->getAngle() >= angle_) {
			end_x = visibleSize.width;
			end_y = tan((90 - myPlane->getAngle()) * M_PI / 180) * (end_x - x) + y;
		} else {
			end_y = visibleSize.height;
			end_x = tan(myPlane->getAngle() * M_PI / 180) * (end_y - y) + x;
		}
	} else if (myPlane->getAngle() > 90 && myPlane->getAngle() < 180) {
		len_x = visibleSize.width - x;
		len_y = -y;
		angle_ = atan2f(len_x, len_y) * 180 / M_PI;
		if (myPlane->getAngle() <= angle_) {
			end_x = visibleSize.width;
			end_y = y - tan(abs(myPlane->getAngle() - 90) * M_PI / 180) * (end_x - x);
		} else {
			end_y = 0;
			end_x = tan(abs(180 - myPlane->getAngle()) * M_PI / 180) * y + x;
		}
	} else if (myPlane->getAngle() < 0 && myPlane->getAngle() > -90) {
		len_x = -x;
		len_y = visibleSize.height - y;
		angle_ = atan2f(len_x, len_y) * 180 / M_PI;
		if (myPlane->getAngle() >= angle_) {
			end_y = visibleSize.height;
			end_x = x - tan(abs(myPlane->getAngle()) * M_PI / 180) * (end_y - y);
		} else {
			end_x = 0;
			end_y = tan(abs(-90 - myPlane->getAngle()) * M_PI / 180) * x + y;
		}
	} else if (myPlane->getAngle() < -90 && myPlane->getAngle() > -180) {
		len_x = -x;
		len_y = -y;
		angle_ = atan2f(len_x, len_y) * 180 / M_PI;
		if (myPlane->getAngle() >= angle_) {
			end_x = 0;
			end_y = y - tan(abs(myPlane->getAngle() + 90) * M_PI / 180) * x;
		} else {
			end_y = 0;
			end_x = x - tan(abs(myPlane->getAngle() + 180) * M_PI / 180) * y;
		}
	}
	auto bulletMove = MoveTo::create(5, ccp(end_x, end_y));
	auto bullet_speed = Speed::create(bulletMove, 3);
	bullet->runAction(bullet_speed);
}

void game::boss_shoot(float tt) {
	if (boss == 1) {

	}
}

void game::new_enemy(float tt) {
	//srand((unsigned int)time(0));
	/*
	int pos_x = rand() % (int)visibleSize.width;
	pos_x = pos_x < 32 ? pos_x + visibleSize.width / 2 : pos_x;
	int pos_y = rand() % (int)visibleSize.height;
	pos_y = pos_y < 32 ? pos_y + visibleSize.height / 2 : pos_y;
	if (pos_y > 64 && pos_y < visibleSize.height - 64) pos_y = 32;
	*/
	//int pos_x = rand() % (int)visibleSize.width;
	//int pos_y = rand() % (int)visibleSize.height;
	//int sym = (pos_x + pos_y) % 4;
	//auto enemy = Sprite::create("enemy.png");
	//enemy->setPhysicsBody(PhysicsBody::createBox(enemy->getContentSize()));
	//enemy->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
	//if (Score <= 100 || Score >= 300) enemy->setTag(10);
	///else if (Score <= 200) enemy->setTag(9);
	//else enemy->setTag(8);
	//enemy->setScale(float(rand() % 4 + 3) / 10);
	//if (sym == 0) enemy->setPosition(0, pos_y);
	//else if (sym == 1) enemy->setPosition(visibleSize.width, pos_y);
	//else if (sym == 2) enemy->setPosition(pos_x, 0);
	//else enemy->setPosition(pos_x, visibleSize.height);
	//enemys.pushBack(enemy);
	//this->addChild(enemy, 1);

	//xz 
	auto Aenemy = myEnemy->createNewEnemy(Score);
	this->addChild(Aenemy,1);
}

void game::new_stone(float tt) {
	srand((unsigned int)time(0));
	int pos_x = rand() % (int)visibleSize.width;
	int pos_y = rand() % (int)visibleSize.height;
	int sym = (pos_x + pos_y) % 4;
	auto stone = Sprite::create("stone.png");
	stone->setPhysicsBody(PhysicsBody::createBox(stone->getContentSize()));
	stone->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
	stone->setTag(60);
	stone->setScale(0.5);
	float targetX, targetY;
	float stoneX = stone->getContentSize().width, stoneY = stone->getContentSize().height;
	if (sym == 0) {
		stone->setPosition(0, pos_y);
		pos_x = 0;
		targetX = visibleSize.width + stoneX;
		targetY = visibleSize.height - pos_y;
	}
	else if (sym == 1) {
		stone->setPosition(visibleSize.width, pos_y);
		pos_x = visibleSize.width;
		targetX = -stoneX;
		targetY = visibleSize.height - pos_y;
	}
	else if (sym == 2) {
		stone->setPosition(pos_x, 0);
		pos_y = 0;
		targetX = visibleSize.width - pos_x;
		targetY = visibleSize.height + stoneY;
	}
	else {
		stone->setPosition(pos_x, visibleSize.height);
		pos_y = visibleSize.height;
		targetX = visibleSize.width - pos_x;
		targetY = -stoneY;
	}
	stones.pushBack(stone);
	this->addChild(stone, 1);

	auto stoneMove = MoveTo::create(10, ccp(targetX, targetY));
	auto stone_speed = Speed::create(stoneMove, 2.5);
	stone->runAction(stone_speed);
}

void game::crateboss(float tt) {
	if (Score == 100 && boss == 0) {
		boss_ = myEnemy->createBoss();
		this->addChild(boss_,1);
		boss = 1;
	}
}

void game::enemy_moving(float tt) {

	//for (int i = 0; i < enemys.size(); i++) {
		//auto enemy = enemys.at(i);
		//float x = myPlane->getPlane()->getPosition().x - enemy->getPositionX();
		//float y = myPlane->getPlane()->getPosition().y - enemy->getPositionY();
		//float angle_ = atan2f(x, y) * 180 / M_PI;
		//enemy->setRotation(angle_ - 180);
		//enemy->stopAllActions();
		//auto move = MoveTo::create(5, myPlane->getPlane()->getPosition());
		//enemy->runAction(move);
	//}
	// xz
	myEnemy->enmey_Moving(myPlane->getPlane()->getPosition().x, myPlane->getPlane()->getPosition().y);
}

void game::testContact(float tt) {
	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(game::onContactBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void game::judge_remove(float tt) {
	if (last_size != myItems->getSize()) {
		if (last_size == 0 || (last_size > myItems->getSize()))
			scheduleOnce(schedule_selector(game::item_disappear), 10.0f);
		last_size = myItems->getSize();
	}
}
void game::speedup_end(float tt) {
	//is_speedup = false;
	myPlane->speedup_end();
}
void game::invin_end(float tt) {
	//is_invin = false;
	myPlane->invin_end();
}
bool game::onContactBegan(PhysicsContact& contact)
{
	srand((unsigned int)time(0));
	Sprite *spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	Sprite *spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
	// ֻ�����������鶼��Ϊ�յ�ʱ��ִ�����µ�����
	if (!(spriteA != NULL && spriteB != NULL)) return true;

	int tagA = contact.getShapeA()->getBody()->getNode()->getTag();
	int tagB = contact.getShapeB()->getBody()->getNode()->getTag();
	ParticleExplosion *explosions = ParticleExplosion::create();
	/*
	tag=20���ӵ���tag=10��9��8��enemy�������ӵ�����tagΪ10��enemyʱ��������enemy
	�ӵ���ײ�л�������һ���������ӷ�
	*/
	if ((tagA == 10 && tagB == 20) || (tagA == 20 && tagB == 10)) {
		//auto label = Label::createWithSystemFont("ss", "Microsoft Yahei", 30);
		//label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - label->getContentSize().height / 2));
		//this->addChild(label, 1);
		contact.getShapeA()->getBody()->getNode()->removeFromParent();
		contact.getShapeB()->getBody()->getNode()->removeFromParent();
		Score += 10;
		update_score();
		if (tagA == 10) {
			explosions->setPosition(spriteA->getPosition());
			//enemys.eraseObject(spriteA);
			// xz
			myEnemy->deleteEnemy(spriteA);
		} else {
			explosions->setPosition(spriteB->getPosition());
			//enemys.eraseObject(spriteB);
			//xz
			myEnemy->deleteEnemy(spriteB);
		}
		int posbility = rand() % 100;
		if (posbility <= 50) {
			int type = rand() % 3;
			auto newItem = myItems->createItem(type);
			newItem->setScale(2);
			auto body = PhysicsBody::createCircle(newItem->getContentSize().width / 2);
			newItem->setPosition(explosions->getPosition());
			float x = newItem->getPosition().x;
			float y = newItem->getPosition().y;
		    //newItem->setPhysicsBody(body);
			//newItem->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
			this->addChild(newItem, 1);		
		}
		SimpleAudioEngine::getInstance()->playEffect("music/boom.wav");
		this->addChild(explosions, 1);
	}
	/*
	�ӵ�����tagΪ8��9��enmey���ӵ���ʧ��enemy��tag++��
	�ӵ���ײ�л���������һ���������ӵ���ʧ���л���һ����
	*/
	else if ((tagA < 10 && tagB == 20) || (tagA == 20 && tagB < 10)) {
		//auto label = Label::createWithSystemFont("1111", "Microsoft Yahei", 30);
		//label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - label->getContentSize().height / 2));
		//this->addChild(label, 1);
		if (tagB == 20) {
			contact.getShapeB()->getBody()->getNode()->removeFromParent();
			explosions->setPosition(spriteA->getPosition());
			tagA = tagA + bullet_power;
			spriteA->setTag(tagA);
		}
		else {
			contact.getShapeA()->getBody()->getNode()->removeFromParent();
			explosions->setPosition(spriteB->getPosition());
			tagB = tagB + bullet_power;
			spriteB->setTag(tagB);
		}
		SimpleAudioEngine::getInstance()->playEffect("music/boom.wav");
		this->addChild(explosions, 1);
	}
	/*
	tag = 1�����ң�tag=10��9��8��enemy
	������ײ�л����л������������������٣����ӷ�
	*/
	else if ((tagA == 1 && tagB >= 7 && tagB <= 10) || (tagA >= 7 && tagA <= 10 && tagB == 1)) {
		//auto label = Label::createWithSystemFont("fff", "Microsoft Yahei", 30);
		//label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - label->getContentSize().height / 2));
		//this->addChild(label, 1);
		if (tagA == 1) {
			explosions->setPosition(spriteB->getPosition());
			spriteB->removeFromParent();
			//enemys.eraseObject(spriteB);
			//xz
			myEnemy->deleteEnemy(spriteA);

			spriteA->stopAction(speed_);
		} else {
			explosions->setPosition(spriteA->getPosition());
			spriteA->removeFromParent();
			//enemys.eraseObject(spriteA);

			//xz
			myEnemy->deleteEnemy(spriteA);

			spriteB->stopAction(speed_);
		}
		this->addChild(explosions, 1);
		CCActionInterval * blink = CCBlink::create(1.0f, 6);
		myPlane->getPlane()->runAction(blink);
		if (!myPlane->getInvin()) {
			myPlane->getHurt();
			left->setPercentage(myPlane->getPower() * 33);
		}
	}
	/*
	����bossײ�Լ�,�Լ���Ѫ
	*/
	else if ((tagA == 1 && tagB == 50) || (tagA == 50 && tagB == 1)) {
		CCActionInterval * blink = CCBlink::create(1.0f, 6);
		myPlane->getPlane()->runAction(blink);
		if (!myPlane->getInvin()) {
			myPlane->getHurt();
			left->setPercentage(myPlane->getPower() * 33);
		}
	}
	/*
	boss��ײ�ӵ�
	*/
	else if ((tagA == 20 && tagB == 50) || (tagA == 50 && tagB == 20)) {
		if (boss_power == 0) {
			contact.getShapeA()->getBody()->getNode()->removeFromParent();
			contact.getShapeB()->getBody()->getNode()->removeFromParent();
			Score += 50;
			update_score();
			if (tagA == 10) {
				explosions->setPosition(spriteA->getPosition());
				//enemys.eraseObject(spriteA);
				// xz
				myEnemy->deleteEnemy(spriteA);
			}
			else {
				explosions->setPosition(spriteB->getPosition());
				//enemys.eraseObject(spriteB);
				// xz
				myEnemy->deleteEnemy(spriteA);
			}
			SimpleAudioEngine::getInstance()->playEffect("music/boom.wav");
			this->addChild(explosions, 1);
			boss = -1;
		}
		else {
			if (tagB == 20) {
				contact.getShapeB()->getBody()->getNode()->removeFromParent();
				explosions->setPosition(spriteA->getPosition());
				boss_power -= bullet_power;
			}
			else {
				contact.getShapeA()->getBody()->getNode()->removeFromParent();
				explosions->setPosition(spriteB->getPosition());
				boss_power -= bullet_power;
			}
			SimpleAudioEngine::getInstance()->playEffect("music/boom.wav");
			this->addChild(explosions, 1);
		}
	}
	/*
	�ӵ���ײ��ʯ���ӵ���ʧ����ʯ����ʧ
	*/
	else if ((tagA == 20 && tagB == 60) || (tagA == 60 && tagB == 20)) {
		if (tagB == 20) {
			contact.getShapeB()->getBody()->getNode()->removeFromParent();
			explosions->setPosition(spriteA->getPosition());
			//boss_power -= bullet_power;
		}
		else {
			contact.getShapeA()->getBody()->getNode()->removeFromParent();
			explosions->setPosition(spriteB->getPosition());
			//boss_power -= bullet_power;
		}
		SimpleAudioEngine::getInstance()->playEffect("music/boom.wav");
		this->addChild(explosions, 1);
	}
	/*
	������ײ��ʯ����ʯ��ʧ�����Ҽ���
	*/
	else if ((tagA == 60 && tagB == 1) || (tagA == 1 && tagB == 60)) {
		if (tagA == 1) {
			explosions->setPosition(spriteB->getPosition());
			spriteB->removeFromParent();
			stones.eraseObject(spriteB);
			spriteA->stopAction(speed_);
		}
		else {
			explosions->setPosition(spriteA->getPosition());
			spriteA->removeFromParent();
			stones.eraseObject(spriteA);
			spriteB->stopAction(speed_);
		}
		this->addChild(explosions, 1);
		CCActionInterval * blink = CCBlink::create(1.0f, 6);
		myPlane->getPlane()->runAction(blink);
		if (!myPlane->getInvin()) {
			myPlane->getHurt();
			left->setPercentage(myPlane->getPower() * 33);
		}
	}
	//�������ߵ��ж�
	else if ((tagA == 100 && tagB == 1) || (tagA == 1 && tagB == 100)) {
		if (myPlane->getPower() < 3) {
			myPlane->getCure();
			left->setPercentage(myPlane->getPower() * 33);
		}
		if (tagA == 100)
			myItems->remove_contactItem(spriteA);
		else
			myItems->remove_contactItem(spriteB);
	}
	else if ((tagA == 101 && tagB == 1) || (tagA == 1 && tagB == 101)) {
		myPlane->invin_begin();
		scheduleOnce(schedule_selector(game::invin_end), 5.0f);
		if (tagA == 101)
			myItems->remove_contactItem(spriteA);
		else
			myItems->remove_contactItem(spriteB);
	}
	else if ((tagA == 102 && tagB == 1) || (tagA == 1 && tagB == 102)) {
		myPlane->speedup_begin();
		scheduleOnce(schedule_selector(game::speedup_end), 5.0f);
		if (tagA == 102)
			myItems->remove_contactItem(spriteA);
		else
			myItems->remove_contactItem(spriteB);
	}
	/*
	��ʯ��ײ�л���˫˫����
	*/
	else if ((tagA == 60 && tagB >= 8 && tagB <= 10) || (tagA >= 7 && tagA <= 10 && tagB == 60)) {
		contact.getShapeA()->getBody()->getNode()->removeFromParent();
		contact.getShapeB()->getBody()->getNode()->removeFromParent();
		if (tagA == 60) {
			explosions->setPosition(spriteB->getPosition());
			//enemys.eraseObject(spriteB);
			//xz
			myEnemy->deleteEnemy(spriteA);
		}
		else {
			explosions->setPosition(spriteA->getPosition());
			//enemys.eraseObject(spriteA);
			//xz
			myEnemy->deleteEnemy(spriteA);
		}
		SimpleAudioEngine::getInstance()->playEffect("music/boom.wav");
		this->addChild(explosions, 1);
	}
	else if ((tagA == 50 && tagB == 60) || (tagA == 60 && tagB == 50)) {
		if (tagA == 50) {
			explosions->setPosition(spriteB->getPosition());
			spriteB->removeFromParent();
			stones.eraseObject(spriteB);
			//spriteA->stopAction(speed_); xz
		}
		else {
			explosions->setPosition(spriteA->getPosition());
			spriteA->removeFromParent();
			stones.eraseObject(spriteA);
			//spriteB->stopAction(speed_);xz
		}
		this->addChild(explosions, 1);
		//CCActionInterval * blink = CCBlink::create(1.0f, 6);
		//boss_->runAction(blink);
		boss_power -= 5;
		// ��ʯֱ��ײ��boss������
		if (boss_power < 0) {
			boss = -1;
		}
	}
	
	return true;
}

void game::getBuff(float tt) {
	float planeX = myPlane->getPlane()->getPosition().x;
	float planeY = myPlane->getPlane()->getPosition().y;
	float R = myPlane->getPlane()->getContentSize().height*0.3;
	int type = myItems->is_getbuff(planeX, planeY,R);
	switch (type)
	{
	case 1:
		if (myPlane->getPower() < 3) {
			myPlane->getCure();
			left->setPercentage(myPlane->getPower() * 33);
		}
		break;
	case 2:
		myPlane->invin_begin();
		scheduleOnce(schedule_selector(game::invin_end), 5.0f);
		break;
	case 3:
		myPlane->speedup_begin();
		scheduleOnce(schedule_selector(game::speedup_end), 5.0f);
		break;
	default:
		break;
	}
}

void game::Win_Fail(float tt) {
	if (boss == -1) {
		SimpleAudioEngine::getInstance()->playEffect("music/victory.wav");
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		auto Win_scene = Win::createScene();
		Director::getInstance()->replaceScene(Win_scene);
	}
	if (myPlane->getPower() == 0) {
		SimpleAudioEngine::getInstance()->playEffect("music/defeat.wav");
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		ParticleExplosion *explosions = ParticleExplosion::create();
		explosions->setPosition(myPlane->getPlane()->getPosition());
		this->addChild(explosions, 1);
		auto Fail_scene = Fail::createScene();
		Director::getInstance()->replaceScene(Fail_scene);
	}
}

void game::item_disappear(float tt) {
	myItems->removeItem();
}

void game::bullet_delete(float tt) {
	for (int i = 0; i < bullets.size(); i++) {
		auto bullet_ = bullets.at(i);
		if (bullet_->getPositionX() <= 0 || bullet_->getPositionX() >= visibleSize.width || bullet_->getPositionY() <= 0 || bullet_->getPositionY() >= visibleSize.height) {
			bullet_->removeFromParent();
			bullets.eraseObject(bullet_);
		}
	}
}

void game::skill(Ref* pSender) {
	for (int i = 0; i < myEnemy->getEnemySize(); i++) {
		auto enemy = myEnemy->getAEnemy(i);
		ParticleExplosion *explosions = ParticleExplosion::create();
		explosions->setPosition(enemy->getPosition());
		this->addChild(explosions, 1);
		SimpleAudioEngine::getInstance()->playEffect("music/boom.wav");
		enemy->removeFromParent();
		myEnemy->deleteEnemy(enemy);
	}
}

void game::preloadMusic() {
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/boom.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/bulletvoice.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/defeat.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/victory.wav");
}

