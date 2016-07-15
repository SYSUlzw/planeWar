#include "Enemy.h"

#pragma execution_character_set("utf-8")
USING_NS_CC;

Enemy* Enemy::enemy = NULL;
Enemy::Enemy() {}

Enemy* Enemy::getInstance() {
	if (enemy == NULL) {
		enemy = new Enemy();
	}
	return enemy;
}

Sprite* Enemy::createNewEnemy(int score) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	srand((unsigned int)time(0));
	/*
	int pos_x = rand() % (int)visibleSize.width;
	pos_x = pos_x < 32 ? pos_x + visibleSize.width / 2 : pos_x;
	int pos_y = rand() % (int)visibleSize.height;
	pos_y = pos_y < 32 ? pos_y + visibleSize.height / 2 : pos_y;
	if (pos_y > 64 && pos_y < visibleSize.height - 64) pos_y = 32;
	*/
	int pos_x = rand() % (int)visibleSize.width;
	int pos_y = rand() % (int)visibleSize.height;
	int sym = (pos_x + pos_y) % 4;
	auto enemy = Sprite::create("enemy.png");
	enemy->setPhysicsBody(PhysicsBody::createBox(enemy->getContentSize()));
	enemy->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
	if (score <= 100 || score >= 300) enemy->setTag(10);
	else if (score <= 200) enemy->setTag(9);
	else enemy->setTag(8);
	enemy->setScale(float(rand() % 4 + 3) / 10);
	if (sym == 0) enemy->setPosition(0, pos_y);
	else if (sym == 1) enemy->setPosition(visibleSize.width, pos_y);
	else if (sym == 2) enemy->setPosition(pos_x, 0);
	else enemy->setPosition(pos_x, visibleSize.height);
	enemys.pushBack(enemy);
	return enemy;
}


void Enemy::enmey_Moving(float planex, float planey) {
	for (int i = 0; i < enemys.size(); i++) {
		auto enemy = enemys.at(i);
		float x = planex - enemy->getPositionX();
		float y = planey - enemy->getPositionY();
		float angle_ = atan2f(x, y) * 180 / M_PI;
		enemy->setRotation(angle_ - 180);
		enemy->stopAllActions();
		auto move = MoveTo::create(5, ccp(planex,planey));
		enemy->runAction(move);
	}
}

Sprite* Enemy::createBoss() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	srand((unsigned int)time(0));
	int pos_x = rand() % (int)visibleSize.width;
	int pos_y = rand() % (int)visibleSize.height;
	int sym = (pos_x + pos_y) % 4;
	auto boss_ = Sprite::create("boss.png");
	boss_->setPhysicsBody(PhysicsBody::createBox(boss_->getContentSize()));
	boss_->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
	boss_->setTag(50);
	boss_->setScale(1.0);
	if (sym == 0) boss_->setPosition(0, pos_y);
	else if (sym == 1) boss_->setPosition(visibleSize.width, pos_y);
	else if (sym == 2) boss_->setPosition(pos_x, 0);
	else boss_->setPosition(pos_x, visibleSize.height);
	enemys.pushBack(boss_);
	return boss_;
}

void Enemy::deleteEnemy(Sprite* e) {
	enemys.eraseObject(e);
}

int Enemy::getEnemySize() {
	return enemys.size();
}

Sprite* Enemy::getAEnemy(int i) {
	return enemys.at(i);
}