#ifndef _PLANELAYER_H_
#define _PLANELAYER_H_
#include "cocos2d.h"
USING_NS_CC;
class PlaneLayer : public cocos2d::Ref
{
public:
	static PlaneLayer* getInstance();
	void createPlane(Speed* &speed_);
	Sprite* getPlane();
	float getAngle();
	int getPower();
	void getHurt();
	void getCure();

	void movePlane(float touchX,float touchY,Speed* &speed);

	void speedup_begin();
	void speedup_end();

	void invin_begin();
	void invin_end();
	bool getInvin();

	//more
	void getContact(); // 用来判断当前的碰撞会给自身造成怎么样的影响
private:
	PlaneLayer();
	Sprite* plane;
	float angle;
	int power;
	bool is_invin;
	bool is_speedup;
	static PlaneLayer* planelayer;
};

#endif // !_PLANELAYER_H_

