#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "cocos2d.h"
USING_NS_CC;

class Enemy:public cocos2d::Ref
{
public:
	static Enemy* getInstance();
	Sprite* createNewEnemy(int score);
	Sprite* createBoss();
	void enmey_Moving(float planex,float planey);
	void deleteEnemy(Sprite* e);
	int getEnemySize();
	Sprite* getAEnemy(int i);
private:
	Enemy();
	Vector<Sprite*> enemys;
	static Enemy* enemy;
};

#endif // !_ENEMY_H_

