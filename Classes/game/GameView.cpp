#include "GameView.h"
#include "GameOverView.h"
#include "MainView.h"
#include "Enemy.h"
#include "SimpleAudioEngine.h"
#include "../util/GlobalSingleton.h"

#include "../util/PlatformInterface.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "GKHWrapperCpp.h"
#endif

using namespace CocosDenshion;

#define GENERATE_TIME sqrt(1.0*s_GlobalSingleton->GAME_MODEL)



USING_NS_CC;
using namespace std;

enum 
{
	REPLAY_BUTTON = 1,
	SHARE_BUTTON = 2,
};
typedef enum{
	BackGroundTag = 1000,
	BackGroundTag_Near = 1001,
	BackGroundTag_Far = 1002,
}BackGroundTagType;

#define MAX_GENERATE_ENEMY 6

GameView::GameView()
{
}

GameView::~GameView()
{
	_enemies->release();
}

CCScene* GameView::scene()
{
	CCScene *scene = CCScene::create();
	GameView *layer = GameView::create();

	scene->addChild(layer);


	return scene;
}

bool GameView::init()
{
	//show ad

    PlatformInterface::showAdView(2);


	CCSize s = CCDirector::sharedDirector()->getVisibleSize();

	if ( !CCLayerColor::initWithColor(ccc4(125,125,125,125),s.width,s.height))
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	CCSprite *bg = CCSprite::create("bg.jpg");
	bg->setAnchorPoint(CCPointZero);
	this->addChild(bg,-1);

	bg->setScale(s_GlobalSingleton->scale);

    _isGameStart = false;
    
	_time_count = 1;
	_time = 0;
	_gameScore = 0;
	_forAnimateScore = 0;

	_theHero = Hero::initHeroWithGameView(this);;

	_enemies = CCArray::create();
	_enemies->retain();

	this->setTouchEnabled(true);

	//SimpleAudioEngine::sharedEngine()->playBackgroundMusic("GameBgMusic.mp3", true);
    
   // PlatformInterface::hideADView(1);//hide ad
  
	this->initScore();

	return true;
}
void GameView::gameOver()
{
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();

    PlatformInterface::showAdView(1);

	this->stopGame();
    
    /////score
    string score_type;
    string score_board;
    if(s_GlobalSingleton->GAME_MODEL == GAME_EASY)
    {
        score_board = "BlackWhiteWorld_Easy";
        score_type = "score_easy";
    }
    else if(s_GlobalSingleton->GAME_MODEL == GAME_HARD)
    {
        score_board = "BlackWhiteWorld_Hard";
        score_type = "score_hard";
    }
    else if(s_GlobalSingleton->GAME_MODEL == GAME_TERRIBLE)
    {
        score_board = "BlackWhiteWorld_Terrible";
        score_type = "score_terrible";
    }
    else if(s_GlobalSingleton->GAME_MODEL == GAME_ET)
    {
        score_board = "BlackWhiteWorld_NoneHuman";
        score_type = "score_et";
    }

    
	int highestScore = CCUserDefault::sharedUserDefault()->getIntegerForKey(score_type.c_str());
	if(highestScore < _gameScore)
	{
		CCUserDefault::sharedUserDefault()->setIntegerForKey(score_type.c_str(), _gameScore);//save highest score
		CCUserDefault::sharedUserDefault()->flush();
		highestScore = _gameScore;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		///////game center///////
		GKHWrapperCpp gkh;
		gkh.submitScoreToCatagory(highestScore, score_board.c_str());
#endif
        
	}
    
    s_GlobalSingleton->current_score = _gameScore;
    s_GlobalSingleton->best_score = highestScore;
    
	if(s_GlobalSingleton->language == kLanguageChinese)
	{
		int n = (CCRANDOM_0_1()*7+1);
		if(n>7)n=7;
		char buf[10] = {0};
		sprintf(buf,"%d",n);
		string s = "cn/info_";
		s = s + buf + ".png";

		CCSprite *sp = CCSprite::create(s.c_str());//GameOverView::getScaledSprite(s.c_str());
        sp->setContentSize(sp->boundingBox().size);
		sp->setPosition(ccp(size.width/2,size.height/2));
		addChild(sp,222222,222222);
		sp->setScale(0.001);

		CCSequence* showAction = CCSequence::create(
			CCSpawn::create(CCRotateBy::create(1.0f,360),
			CCScaleTo::create(1.0f,size.width*0.75/sp->getContentSize().width),
			NULL),
			CCFadeIn::create(1.0f),
			CCFadeOut::create(0.5f),
			CCFadeIn::create(1.0f),
			CCDelayTime::create(1.0f),
			CCCallFunc::create(this,callfunc_selector(GameView::goToGameOver)),
			NULL);
		sp->runAction(showAction);
	}
	else
	{
		CCDirector::sharedDirector()->replaceScene(
			CCTransitionFade::create(1, GameOverView::scene()));
		//SimpleAudioEngine::sharedEngine()->playEffect("Win.wav");//
	}
}
void GameView::goToGameOver()
{
   // this->getChildByTag(222222)->setPosition(<#const cocos2d::CCPoint &position#>);
	CCDirector::sharedDirector()->replaceScene(
		CCTransitionFade::create(1, GameOverView::scene()));
	//SimpleAudioEngine::sharedEngine()->playEffect("Win.wav");//
}
void GameView::menuClickCallback(CCObject* pSender)
{
	CCNode *node = (CCNode *)pSender;
	int tag = node->getTag();
	switch (tag)
	{
	case REPLAY_BUTTON:
		SimpleAudioEngine::sharedEngine()->stopAllEffects();//stop effect
		CCDirector::sharedDirector()->replaceScene(
			CCTransitionFade::create(1, GameView::scene()));
		break;
	case SHARE_BUTTON:
        {
            int sns;
            if(CCApplication::sharedApplication()->getCurrentLanguage() == kLanguageChinese)
            {
                /*
                sns = CCRANDOM_0_1()*3;
                if(sns == 3)
                {
                    sns = 0;
                }
                 */
                sns = 0;
            }
            else
            {
                sns = CCRANDOM_0_1()*2 + 3;
                if (sns == 5)
                {
                    sns = 4;
                }
            }
            
           // PlatformInterface::share(sns);
        }
        
		//share
		break;
	}
}
void GameView::backToMainView(float dt)
{
	this->unschedule(schedule_selector(GameView::backToMainView));
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, MainView::scene()));
}

void GameView::update(float dt)
{
	/*
	_time += dt;
	CCLOG("---->%.1f",_time);
	if((int)_time != 0 &&(int)_time % 60 == 0)
	{
		_time_count++;
		if(_time_count > MAX_GENERATE)
		{
			_time_count = MAX_GENERATE;
		}
		this->unschedule(schedule_selector(GameView::enemyGenerate));

		this->schedule(schedule_selector(GameView::enemyGenerate), 1.0f/(GENERATE_TIME*sqrt(_time_count)));
	}
	*/
}
void GameView::enemyGenerate(float dt)
{
	//

	Enemy *enemy = Enemy::initEnemyWithGameView(this);
	enemy->start();
	_enemies->addObject(enemy);
    
}
void GameView::ccTouchesBegan(CCSet *pTouches,CCEvent *pEvent)
{
	if(!_isGameStart)
	{
		//start game
		this->start();
        
		//touch 
		CCSetIterator iter = pTouches->begin();//
		for (; iter != pTouches->end(); iter++)
		{
			CCTouch* pTouch = (CCTouch*)(*iter);
			CCPoint pos = pTouch->getLocation();
			_theHero->touchDown(pos);
		}


		//SimpleAudioEngine::sharedEngine()->playEffect("Begin.wav");//
	}
	else
	{
		//touch 
		CCSetIterator iter = pTouches->begin();//
		for (; iter != pTouches->end(); iter++)
		{
			CCTouch* pTouch = (CCTouch*)(*iter);
			CCPoint pos = pTouch->getLocation();
			_theHero->touchDown(pos);
            break;
		}
	}
}
void GameView::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	//touch 
	CCSetIterator iter = pTouches->begin();//
	for (; iter != pTouches->end(); iter++)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);
		CCPoint pos = pTouch->getLocation();
        CCPoint pp =  _theHero->getHero()->sp->getPosition();
        //CCLog("move:[%.1f,%.1f],[%.1f,%.1f]",pos.x,pos.y,pp.x,pp.y);
		_theHero->touchMove(pos);
        break;
	}
}
void GameView::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	//touch 
	CCSetIterator iter = pTouches->begin();
	for (; iter != pTouches->end(); iter++)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);
		CCPoint pos = pTouch->getLocation();
		_theHero->touchEnd(pos);
        break;
	}
}
void GameView::start()
{
    _isGameStart = true;
    _theHero->start();
    this->setTouchEnabled(true);
    CCObject *pObject = NULL;
    
    CCARRAY_FOREACH(_enemies, pObject)
    {
        //
        Enemy *enemy = (Enemy*)pObject;
        
        enemy->start();
    }
	this->schedule(schedule_selector(GameView::enemyGenerate), .5f/(GENERATE_TIME*sqrt(_time_count)));
	//
	this->scheduleUpdate();
}

void GameView::stopGame()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();//stop effect

	_isGameStart = false;
	this->setTouchEnabled(false);

	this->unscheduleUpdate();

	_theHero->stop();
	//stop all enemy update
	CCObject *pObject = NULL;
	CCARRAY_FOREACH(_enemies,pObject)
	{
		//
		Enemy *enemy = (Enemy *)pObject;
		enemy->stop();
	}

	this->unschedule(schedule_selector(GameView::enemyGenerate));

}
void GameView::resumeGame()
{
	this->setTouchEnabled(true);
	start();
}

void GameView::updateScore(CCNode *pTarget,void *data)
{

	CCSprite *label = (CCSprite *)data;
	if(!label) return;

	_forAnimateScore += ((CCNumber*)label->getChildByTag(1))->num;

	label->removeAllChildrenWithCleanup(true);



	//CCLabelTTF *l = (CCLabelTTF*)this->getChildByTag(1);
	this->removeChildByTag(1);
	//CCSprite *bg = (CCSprite*)this->getChildByTag(2);
	///
	CCSize s = CCDirector::sharedDirector()->getVisibleSize();

	CCSprite* l = GameOverView::createNumLabel(_forAnimateScore);//CCLabelTTF::create("0", FONT_NAME, 48*s_GlobalSingleton->scale);
	l->setAnchorPoint(ccp(0,0));
	addChild(l, 1001,1);
	l->setPosition( ccp(s.width/2 - l->getContentSize().width/2, s.height - 50*s_GlobalSingleton->scale) );
	/*
	char buf[1024] = {0};
	sprintf(buf,"%d",_forAnimateScore);
	string s = "Score : ";
	s = s + buf;
	l->setString(s.c_str());
	*/

	//
	//bg->setScaleX((l->boundingBox().size.width + 20)/bg->getContentSize().width);
}
void GameView::setGameScore(int score)
{
	CCSize s = CCDirector::sharedDirector()->getVisibleSize();

	char buf[1024] = {0};
	sprintf(buf,"%d",score);

	_gameScore += score;

	CCSprite *scoreBord = (CCSprite*)this->getChildByTag(2);

	CCPoint pos = _theHero->getHero()->sp->getPosition();
	CCSprite *scoreLabel = GameOverView::createNumLabel(score);//CCLabelTTF::create(buf, FONT_NAME, 48*s_GlobalSingleton->scale);

	addChild(scoreLabel,10000);
	scoreLabel->setPosition(pos);
	
	CCPoint toPos = ccp(s.width/2,s.height - 50*s_GlobalSingleton->scale/2);

	CCAction *sqAction = CCSequence::create(
		CCMoveTo::create(1.0f,toPos),
		CCFadeOut::create(0.2f),
		CCCallFuncND::create(this, callfuncND_selector(GameView::updateScore),(void *)scoreLabel),
		NULL);

	scoreLabel->runAction(sqAction);

}
int GameView::getGameScore()
{
	return _gameScore;
}


void GameView::initScore()
{
	CCSize s = CCDirector::sharedDirector()->getVisibleSize();

	CCSprite* label1 = GameOverView::createNumLabel(0);//CCLabelTTF::create("0", FONT_NAME, 48*s_GlobalSingleton->scale);
	addChild(label1, 1001,1);
	label1->setAnchorPoint(ccp(0,0));
	label1->setPosition( ccp(s.width/2 - label1->getContentSize().width/2, s.height - 50*s_GlobalSingleton->scale) );
    
    CCSize size = label1->boundingBox().size;
    CCSprite *bg = CCSprite::create("scoreBg.png");
    bg->setScaleY((size.height + 20*s_GlobalSingleton->scale)/bg->getContentSize().height*0.2);
    bg->setPosition(ccp(s.width/2,s.height - 80));
    addChild(bg,1000,2);    
}
