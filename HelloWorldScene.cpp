#include "HelloWorldScene.h"
#include "game.h"
#include "SimpleAudioEngine.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;
using namespace std;
using namespace CocosDenshion;
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	preloadMusic();
	playBgm();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *bg = Sprite::create("background.jpg");
	float scale_x = visibleSize.width / bg->getContentSize().width;
	float scale_y = visibleSize.height / bg->getContentSize().height;
	bg->setScaleX(scale_x);
	bg->setScaleY(scale_y);
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg, 0);

	auto begin_label = Label::createWithSystemFont("¿ªÊ¼ÓÎÏ·", "Microsoft Yahei", 30);
	auto beginItem = MenuItemLabel::create(begin_label, CC_CALLBACK_1(HelloWorld::beign_game, this));
	beginItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + beginItem->getContentSize().height * 2));

	auto menu = Menu::create(beginItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


	auto exit_label = Label::createWithSystemFont("ÍË³öÓÎÏ·", "Microsoft Yahei", 30);
	auto exitItem = MenuItemLabel::create(exit_label, CC_CALLBACK_1(HelloWorld::exit_game, this));
	exitItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + exitItem->getContentSize().height));

	auto menu_ = Menu::create(exitItem, NULL);
	menu_->setPosition(Vec2::ZERO);
	this->addChild(menu_, 1);


	auto label = Label::createWithSystemFont("²Ù×Ý·ÉÐÐÆ÷ÏûÃðËùÓÐÍþÐ²ÄãµÄµÐÈË£¡µ¥»÷\nÊó±ê×ó¼ü¿ØÖÆ·ÉÐÐÆ÷ÒÆ¶¯£¬¿Õ¸ñ¼ü»ò°´Å¥\nÍùÆä³¯Ïò·¢ÉäÅÚµ¯¹¥»÷µÐ»ú¡£½ô¼±Çé¿öÊ±£¬\n¿Éµã»÷ÆÁÄ»ÓÒÏÂ·½µÄ°´Å¥¼¤»îÃØ¼¼ÏûÃðÒ»\n¶¨ÊýÁ¿µÄµÐ»ú¡£×£ÄãºÃÔË£¬Ö¸»Ó¹Ù£¡", "Microsoft Yahei", 30);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - label->getContentSize().height / 2));
	this->addChild(label, 1);
	

    return true;
}


void HelloWorld::beign_game(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	auto game_scene = game::createScene();
	Director::getInstance()->replaceScene(game_scene);
}
void HelloWorld::exit_game(Ref* pSender) {
	/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
return;
#endif
*/

	Director::getInstance()->end();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
	#endif
}
void HelloWorld::preloadMusic() {
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.wav");
}
void HelloWorld::playBgm()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.wav", true);
}
