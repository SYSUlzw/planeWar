#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "game.h"
#include "cocos2d.h"
USING_NS_CC;
class HelloWorld : public cocos2d::Layer
{
public:

    static cocos2d::Scene* createScene();

    virtual bool init();

	void preloadMusic();
	void playBgm();

	void beign_game(cocos2d::Ref* pSender);
    void exit_game(cocos2d::Ref* pSender);

    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
