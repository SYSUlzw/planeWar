#ifndef __GAME_H__
#define __GAME_H__
#include "floatItem.h"
#include "PlaneLayer.h"
#include "Enemy.h"
#include "cocos2d.h"
USING_NS_CC;
class game : public cocos2d::Layer
{
public:
	static Scene* createScene();
	static game* create(PhysicsWorld* world);
	bool init(PhysicsWorld* world);
	// operation
	void testTouchEvent();
	void testContact(float tt);
	void testKeyboardEvent();
	void single_click(float tt);
	// hero
	void skill(Ref* pSender);
	void shoot(Ref* pSender);
	// judge
	void preloadMusic();
	void menuCloseCallback(cocos2d::Ref* pSender);
	void Win_Fail(float tt);
	bool onContactBegan(PhysicsContact& contact);
	void update_score();
	// enemy
	void enemy_moving(float tt);
	void new_enemy(float tt);
	// bullet
	void bullet_delete(float tt);
	// stone
	void new_stone(float tt);
	void stone_moving(float tt);
	// floatItem
	void item_move(float tt);
	void item_disappear(float tt);
	void judge_remove(float tt);
	void invin_end(float tt);
	void speedup_end(float tt);
	void getBuff(float tt);
	// boss
	void crateboss(float tt);
	void boss_shoot(float tt);

	//CREATE_FUNC(game);
private:
	Size visibleSize;
	Sprite *boss_;
	CCProgressTimer *left;

	Vector<Sprite*> stones;
	Vector<Sprite*> boss_buls;
	
	int boss_power;
	int Score;
	int boss;
	int last_size;
	Rect rect;
	
	Label* label_score;

	EventDispatcher* dispatcher;
	FloatItem* myItems;
	PlaneLayer* myPlane;
	Enemy* myEnemy;

	Vec2 touch_pos;
	bool clicked;

	Speed *speed_;
	bool is_invin;
	bool is_speedup;
	float angle;
	int Power;

	// bullet
	Vector<Sprite*> bullets;
	int bullet_power;
};

#endif
