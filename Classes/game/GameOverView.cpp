#include "MainView.h"
#include "../util/GlobalSingleton.h"
#include "GameOverView.h"
#include "PlatformInterface.h"
#include "GameView.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;


USING_NS_CC;
using namespace std;

CCScene* GameOverView::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameOverView *layer = GameOverView::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverView::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(125,125,125,125)) )
    {
        return false;
    }
    PlatformInterface::showAdView(3);
    
    //show cha ping ad

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	CCSprite *bg = CCSprite::create("bg.jpg");//getScaledSprite("bg.jpg");//CCSprite::create("bg.jpg");
    bg->setScale(s_GlobalSingleton->scale);
	bg->setAnchorPoint(CCPointZero);
	this->addChild(bg,-1);

   
	
	srand((unsigned int)(time(0)));

	string res_path = "";
    int offset_x = 0;
	if(s_GlobalSingleton->language == kLanguageChinese)
	{
		res_path = res_path + "cn/";
	}
	else
	{
        offset_x = getScaledValue(10);
		res_path = res_path + "en/";
	}

	m_title = 0;
	string path;
    //////connect img
    path = res_path + "connect.png";
    CCSprite *connect = getScaledSprite(path.c_str());//CCSprite::create(path.c_str());
    addChild(connect,222,222);
    connect->setAnchorPoint(ccp(0,0));
    connect->setColor(ccc3(0,0,0));
    connect->setPosition(ccp(visibleSize.width/2 - connect->getContentSize().width/2 +offset_x,
		visibleSize.height - connect->getContentSize().height - getScaledValue(15)));

	//s_GlobalSingleton->GAME_MODEL
	char buf[10] = {0};
	sprintf(buf,"%d",s_GlobalSingleton->GAME_MODEL);
	string s = "level_";
	s = s + buf + ".png";
	path = res_path + s;
	CCSprite *gameOver = getScaledSprite(path.c_str());//CCSprite::create(path.c_str());
    addChild(gameOver,222,222);
	gameOver->setAnchorPoint(ccp(0,0));
	gameOver->setColor(ccc3(0,0,0));
    gameOver->setPosition(ccp(visibleSize.width/2 + connect->getContentSize().width/2 + getScaledValue(15)+offset_x,
		visibleSize.height - gameOver->getContentSize().height - getScaledValue(15)));

    path = res_path + "title.png";
	CCSprite *gameOver1 = getScaledSprite(path.c_str());//CCSprite::create(path.c_str());
    addChild(gameOver1,222,222);
	gameOver1->setAnchorPoint(ccp(0,0));
	gameOver1->setColor(ccc3(0,0,0));
    gameOver1->setPosition(ccp(visibleSize.width/2 - gameOver1->getContentSize().width - connect->getContentSize().width/2 - getScaledValue(15)+offset_x,
		visibleSize.height - gameOver1->getContentSize().height - getScaledValue(15)));
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    float yOver = visibleSize.height - gameOver->getContentSize().height - getScaledValue(45);

	path = "";
	path = res_path + "scoreBg.png";
    CCSprite *scoreBg = getScaledSprite(path.c_str());//CCSprite::create(path.c_str());
	scoreBg->setAnchorPoint(ccp(0,0));
    scoreBg->setPosition(ccp(visibleSize.width/2 - scoreBg->getContentSize().width/2,
		yOver - scoreBg->getContentSize().height));
    addChild(scoreBg);

    

    CCSprite *label_score = createNumLabel(s_GlobalSingleton->current_score);
	label_score->setAnchorPoint(ccp(0,0));
    label_score->setPosition(ccp(visibleSize.width/2 - label_score->getContentSize().width/2,
		scoreBg->getPositionY() + getScaledValue(50) + scoreBg->getContentSize().height/2));
  
	CCSprite *label_best = createNumLabel(s_GlobalSingleton->best_score);
	label_best->setAnchorPoint(ccp(0,0));
    label_best->setPosition(ccp(visibleSize.width/2 - label_best->getContentSize().width/2,
		scoreBg->getPositionY() - getScaledValue(130)  + scoreBg->getContentSize().height/2 ));
    
    addChild(label_score);
    addChild(label_best);
    
	yOver = visibleSize.height - gameOver->getContentSize().height  - scoreBg->getContentSize().height - getScaledValue(60);
	//menu
	//
	path = res_path + "share.png";
	CCSprite *sp = getScaledSprite(path.c_str());
	CCMenuItemSprite *pShare = CCMenuItemSprite::create(
		sp,
		sp,
		NULL,
		this,
		menu_selector(GameOverView::menuClickCallback));

	path = res_path + "back.png";
	CCSprite *sp1 = getScaledSprite(path.c_str());
	CCMenuItemSprite *pBack = CCMenuItemSprite::create(
		sp1,
		sp1,
		NULL,
		this,
		menu_selector(GameOverView::menuClickCallback));

	path = res_path + "again.png";
	CCSprite *sp2 = getScaledSprite(path.c_str());
	CCMenuItemSprite *pAgain = CCMenuItemSprite::create(
		sp2,
		sp2,
		NULL,
		this,
		menu_selector(GameOverView::menuClickCallback));

	CCMenu *pMenu = CCMenu::create(pShare,pBack,pAgain,NULL);

	//
	pShare->setTag(1);
	pShare->setPosition(ccp(visibleSize.width/2 - pShare->getContentSize().width/2 - getScaledValue(20),
		yOver - pAgain->getContentSize().height - pShare->getContentSize().height/2));

	pBack->setTag(2);
	pBack->setPosition(ccp(visibleSize.width/2 + pBack->getContentSize().width/2 + getScaledValue(20),
		yOver - pAgain->getContentSize().height - pBack->getContentSize().height/2));

	pAgain->setTag(3);
	pAgain->setPosition(ccp(visibleSize.width/2,
		yOver - pAgain->getContentSize().height/2));
	
	pMenu->setAnchorPoint(ccp(0,0));
    pMenu->setPosition(ccp(0,0));
	this->addChild(pMenu);
	if(s_GlobalSingleton->language == kLanguageChinese)
	{
		//this->schedule(schedule_selector(GameOverView::switchTitle), 3.0f);
	}
	return true;
}
void GameOverView::switchTitle(float dt)
{
	m_title = !m_title;
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	this->getChildByTag(222)->removeFromParent();

	string path;
	if(!m_title)
	{
		char buf[10] = {0};
		sprintf(buf,"%d",s_GlobalSingleton->GAME_MODEL);
		string s = "level_";
		s = s + buf + ".png";
		path = "cn/" + s;
	}
	else
	{
		path = "cn/title.png";
	}

	CCSprite *gameOver = getScaledSprite("cn/title.png");//CCSprite::create(path.c_str());
    addChild(gameOver,222,222);
	gameOver->setAnchorPoint(ccp(0,0));
	gameOver->setColor(ccc3(0,0,0));
    gameOver->setPosition(ccp(visibleSize.width/2 - gameOver->getContentSize().width/2,
		visibleSize.height - gameOver->getContentSize().height - getScaledValue(15)));

    if(m_title)
    {
    	gameOver->setScale(0.5f);
    }
}
CCSprite *GameOverView::getScaledSprite(const char* filename)
{
	CCSize s = CCDirector::sharedDirector()->getVisibleSize();

	CCSprite *sp = CCSprite::create(filename);

	sp->setScale(s.height/960.0f);

	sp->setContentSize(sp->boundingBox().size);

	return sp;
}
float GameOverView::getScaledValue(float value)
{
	CCSize s = CCDirector::sharedDirector()->getVisibleSize();
	return (s.height/960.0f)*value;
}

void GameOverView::menuClickCallback(CCObject* pSender)
{
	CCNode *node = (CCNode *)pSender;
	int tag = node->getTag();
	switch (tag)
	{
	case 1:
            int sns;
            if(CCApplication::sharedApplication()->getCurrentLanguage() == kLanguageChinese)
            {
                 sns = CCRANDOM_0_1()*2;
                 if(sns == 2)
                 {
                     sns = 0;
                 }
            }
            else
            {
                sns = CCRANDOM_0_1()*2 + 5;
                if (sns == 7)
                {
                    sns = 5;
                }
            }
            
           PlatformInterface::share(sns,s_GlobalSingleton->GAME_MODEL,s_GlobalSingleton->current_score);
        //SHARE
		break;
	case 2:
        PlatformInterface::hideADView(1);
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, MainView::scene()));
        break;
	case 3:
        PlatformInterface::hideADView(1);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, GameView::scene()));
		break;
	}
}
CCSprite *GameOverView::createNumLabel(int num)
{
	CCSprite *pContainer = CCSprite::create();

	char buf[1024] = {0};
	sprintf(buf,"%d",num);
	string num_str = buf;
	int len = num_str.length();
	int width = 0;
	for(int i = 0;i < len ;i++)
	{
		string path = "num/" + num_str.substr(i,1) + ".png";
		CCSprite *sp = getScaledSprite(path.c_str());//CCSprite::create(path.c_str());
		sp->setPosition(ccp(width,getScaledValue(20)));
		pContainer->addChild(sp);

		width += sp->getContentSize().width + getScaledValue(2);
	}
	pContainer->setContentSize(CCSizeMake(width,getScaledValue(40)));


	CCNumber *ccnumber = CCNumber::create();
	ccnumber->num = num;
	ccnumber->setTag(1);

	pContainer->addChild(ccnumber);

	return pContainer;
}
