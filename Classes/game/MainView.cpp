#include "MainView.h"
#include "../util/GlobalSingleton.h"
#include "GameView.h"
#include "PlatformInterface.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "GKHWrapperCpp.h"
#endif

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;




USING_NS_CC;
using namespace std;

CCScene* MainView::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainView *layer = MainView::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainView::init()
{

	this->setKeypadEnabled(true);
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(125,125,125,125)) )
    {
        return false;
    }
    
    m_bHasClicked = false;

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	
	CCSprite *bg = CCSprite::create("bg.jpg");

	float sx = visibleSize.width/bg->getContentSize().width;
	float sy = visibleSize.height/bg->getContentSize().height;

	s_GlobalSingleton->scale = sx>sy?sx:sy;
	bg->setScale(s_GlobalSingleton->scale);
	bg->setAnchorPoint(CCPointZero);
	this->addChild(bg,-1);

	srand((unsigned int)(time(0)));

    s_GlobalSingleton->language = CCApplication::sharedApplication()->getCurrentLanguage();///
    
	string res_path = "";
	if(s_GlobalSingleton->language == kLanguageChinese)
	{
		res_path = res_path + "cn/";
	}
	else
	{
		res_path = res_path + "en/";
	}

	string easy = res_path + "easy.png";
	CCMenuItemImage *pEasy = CCMenuItemImage::create(
		easy.c_str(),
		easy.c_str(),
		NULL,
		this,
		menu_selector(MainView::menuClickCallback));

	string hard = res_path + "hard.png";
	CCMenuItemImage *pHard = CCMenuItemImage::create(
		hard.c_str(),
		hard.c_str(),
		NULL,
		this,
		menu_selector(MainView::menuClickCallback));
	string terrible = res_path + "terrible.png";
	CCMenuItemImage *pTerrible = CCMenuItemImage::create(
		terrible.c_str(),
		terrible.c_str(),
		NULL,
		this,
		menu_selector(MainView::menuClickCallback));

	string et = res_path + "et.png";
	CCMenuItemImage *pET = CCMenuItemImage::create(
		et.c_str(),
		et.c_str(),
		NULL,
		this,
		menu_selector(MainView::menuClickCallback));

	string rank = res_path + "rank.png";
	CCMenuItemImage *pRank = CCMenuItemImage::create(
		rank.c_str(),
		rank.c_str(),
		NULL,
		this,
		menu_selector(MainView::menuClickCallback));

	float scale = visibleSize.width*5/8/pEasy->getContentSize().width;
	pEasy->setScale(scale);
	pHard->setScale(scale);
	pTerrible->setScale(scale);
	pET->setScale(scale);
	pRank->setScale(scale);

	CCMenu *pMenu = CCMenu::create(pEasy,pHard,pTerrible,pET,pRank,NULL);


	//设置TAG、锚点、位置
	pEasy->setTag(GAME_EASY);
	pHard->setTag(GAME_HARD);
	pTerrible->setTag(GAME_TERRIBLE);
	pET->setTag(GAME_ET);
	pRank->setTag(GAME_ET+1);

	pMenu->alignItemsVerticallyWithPadding(scale*pEasy->getContentSize().height/10);

	this->addChild(pMenu);
	pMenu->setPosition(visibleSize.width/2,visibleSize.height/2);//to be fixed

	return true;
}

void MainView::keyBackClicked(void)
{
	if(m_bHasClicked)
	{
		CCDirector::sharedDirector()->end();
	}
	else
	{
		this->scheduleOnce(schedule_selector(MainView::resetBackClick), 3.0f);
		m_bHasClicked = true;
		PlatformInterface::showToast("再点击一次退出游戏",3);
	}
}
void MainView::keyMenuClicked(void)
{

}
void MainView::resetBackClick(float dt)
{
	m_bHasClicked = false;
}
void MainView::menuClickCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    GKHWrapperCpp gkh;
#endif
	CCNode *node = (CCNode *)pSender;
	int tag = node->getTag();
	switch (tag)
	{
	case GAME_EASY:
	case GAME_HARD:
	case GAME_TERRIBLE:
    case GAME_ET:
		s_GlobalSingleton->GAME_MODEL = (GameModel)tag;
		CCDirector::sharedDirector()->replaceScene(
			CCTransitionFade::create(1, GameView::scene()));
		break;
	case GAME_ET+1:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        gkh.showLeaderBoard();
#endif
		
        break;
	default:
		break;
	}
}
