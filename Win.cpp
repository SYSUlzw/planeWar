#include "Win.h"
#include "game.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;

Scene* Win::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Win::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}
bool Win::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *bg = Sprite::create("background.jpg");
	float scale_x = visibleSize.width / bg->getContentSize().width;
	float scale_y = visibleSize.height / bg->getContentSize().height;
	bg->setScaleX(scale_x);
	bg->setScaleY(scale_y);
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg, 0);

	auto label = Label::createWithSystemFont("游戏获胜", "Microsoft Yahei", 30);

	// position the label on the center of the screen
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);

	auto label_restart = Label::createWithSystemFont("重新开始", "Microsoft Yahei", 30);
	auto restartItem = MenuItemLabel::create(label_restart, CC_CALLBACK_1(Win::restart, this));
	restartItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(restartItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto exit_label = Label::createWithSystemFont("退出游戏", "Microsoft Yahei", 30);
	auto exitItem = MenuItemLabel::create(exit_label, CC_CALLBACK_1(Win::exit_game, this));
	exitItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - label->getContentSize().height));

	auto menu_ = Menu::create(exitItem, NULL);
	menu_->setPosition(Vec2::ZERO);
	this->addChild(menu_, 1);

	return true;
}
void Win::restart(Ref* pSender) {
	auto game_scene = game::createScene();
	Director::getInstance()->replaceScene(game_scene);
}
void Win::exit_game(Ref* pSender) {
	/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif
	*/
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}