#ifndef __WIN_H__
#define __WIN_H__
#include "game.h"
#include "cocos2d.h"
USING_NS_CC;
class Win : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();

	virtual bool init();
	void restart(Ref* pSender);
	void exit_game(cocos2d::Ref* pSender);

	CREATE_FUNC(Win);
private:
	
};

#endif // __Win_H__
