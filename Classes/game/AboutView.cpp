#include "MainView.h"
#include "AboutView.h"

USING_NS_CC;

CCScene* AboutView::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	AboutView *layer = AboutView::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool AboutView::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	CC_UNUSED CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CC_UNUSED CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	return true;
}


void AboutView::menuClickCallback(CCObject* pSender)
{
	CCNode *node = (CCNode *)pSender;
	int tag = node->getTag();
}
