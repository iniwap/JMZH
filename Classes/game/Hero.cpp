#include "Hero.h"
#include "GameView.h"
#include "Enemy.h"
#include "SimpleAudioEngine.h"
#include "../util/GlobalSingleton.h"

#define DEAD_SPEED_FACTOR (1*sqrt(1.0*s_GlobalSingleton->GAME_MODEL))
#define LIVE_SPEED_FACTOR (0.4*sqrt(1.0*s_GlobalSingleton->GAME_MODEL))
#define MIN_LIVE_R  (20*sqrt(1.0*s_GlobalSingleton->GAME_MODEL))
#define AREA_RATIO  (960*640)
#define SQRT_2 1.414

#define SCORE_FACTOR 0.001

using namespace CocosDenshion;

using namespace std;


Hero::Hero(void)
{
	//_heroSprite = NULL;
}
Hero::~Hero(void)
{
}
Hero* Hero::initHeroWithGameView(GameView* game)
{
	Hero *pRet = new Hero();
	if (pRet && pRet->initHero(game))
	{
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}
bool Hero::initHero(GameView* gameView)
{
	bool bRet = false;
	do 
	{
		CCSize s = gameView->getContentSize();

		_theGameView = gameView;

		_hero = new HeroAttr();
		
		_hero->aliveTime = 0.0f;//just set eq r
		_hero->isAlive = false;

		mWarningMinSize = CCSprite::create("black.png");
		mWarningMinSize->setColor(ccBLACK);
		mWarningMinSize->setVisible(false);
		mWarningMinSize->setScale(MIN_LIVE_R*4/mWarningMinSize->getContentSize().width);

		_hero->sp = CCSprite::create("hero.png");
		
		_ori_r = _hero->sp->getContentSize().width/2;

		_hero->color = ccc3(255,255,255);
		_hero->sp->setColor(_hero->color);
		_hero->sp->setScale(200.0f*(s.width/640)/_hero->sp->getContentSize().width);
		_hero->pos = ccp(s.width/2,100.0f*(s.width/640));
		_hero->sp->setPosition(_hero->pos);
		mWarningMinSize->setPosition(_hero->pos);
		//add hero streak
		mHeroStreak = CCMotionStreak::create(1, 1, 100, ccWHITE, "heroTail.png" );
		mHeroStreak->setPosition( _hero->pos ); 

		/*
		CCRepeatForever *repeat = CCRepeatForever::create( CCRotateBy::create(1.0f, 360) );
		_hero->sp->runAction(repeat);
		*/
		this->addChild(mWarningMinSize);
		this->addChild(mHeroStreak);
		this->addChild(_hero->sp);

		_theGameView->addChild(this,HERO_ZORDER);

		isAction = false;

		bRet = true;
	} while (0);

	return bRet;
}
bool Hero::checkIfCollide(CCPoint pos1,CCPoint pos2,float r1,float r2)
{
	if(pow(pos1.x - pos2.x,2) + pow(pos1.y - pos2.y,2) <= pow(r1+r2,2))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Hero::update(float dt)
{
	if(isAction) return;
	CCSize s = CCDirector::sharedDirector()->getVisibleSize();
	//////////////////////////////////////////////////////////////////////////
	float _hero_r = _hero->sp->boundingBox().size.width/2;
	if(_hero->isAlive)
	{
		//increase the alive time
		_hero->aliveTime += dt;
		_hero_r *= (1 + LIVE_SPEED_FACTOR*dt);
		if(_hero_r*2 <= s.width)
		{
			_hero->sp->setScale(_hero_r/_ori_r);
		}
		else
		{
			_hero_r = s.width/2;
			_hero->pos.x = s.width/2;
		}
	}
	else
	{
		//decrease the hero size if zero,die
		if(_theGameView->getIsGameStart())
		{
			_hero->aliveTime -= dt;
			if(_hero->aliveTime < 0)
			{
				_hero->aliveTime = 0;
			}

			_hero_r *= (1 - DEAD_SPEED_FACTOR*dt);
			if( _hero_r <= MIN_LIVE_R)
			{
				//dead
				gameOver();
			}
			else
			{
				if(_hero_r <= 2*MIN_LIVE_R)
				{
					mWarningMinSize->setVisible(true);
				}
				else
				{
					mWarningMinSize->setVisible(false);
				}
				_hero->sp->setScale( _hero_r/_ori_r);

			}
		}
	}

	////////////////////////////////Collide//////////////////////////////////////

	CCObject *pObject = NULL;
	CCARRAY_FOREACH_REVERSE(_theGameView->getTheEnemies(), pObject)
	{
		Enemy *enemy = (Enemy*)pObject;
		float enemy_r = enemy->getEnemy()->sp->boundingBox().size.width/2;
		if(checkIfCollide(_hero->pos,enemy->getEnemy()->pos,_hero_r,enemy_r))
		{
			//
			if(enemy->getEnemy()->isWhite)
			{
				//
				float tmp;
				tmp = _hero_r;
	
				///////+score r
				this->calculateScore(enemy_r);

				_hero_r += enemy_r*_theGameView->getTimeCT()/MAX_GENERATE;

				runScoreAction(_hero_r/_ori_r,enemy_r);

				//remove
				enemy->getEnemy()->sp->removeFromParentAndCleanup(true);
				enemy->removeFromParentAndCleanup(true);
				_theGameView->getTheEnemies()->removeObject(pObject);

			}
			else
			{
				//dead game over
				gameOver();
			}
		}
	}

	//score
	//this->calculateScore(_hero->aliveTime);

}

void Hero::runScoreAction(float scale,float speed)
{
	CCSize s = CCDirector::sharedDirector()->getVisibleSize();
	int tmp = 0;
	if(speed < s.width/32)
	{
		tmp = 1;
	}
	else if(speed < s.width/16 && speed > s.width/32)
	{
		tmp = 2;
	}
	else if(speed < s.width/8 && speed > s.width/16)
	{
		tmp = 3;
	}
	else if(speed < s.width/4 && speed > s.width/8)
	{
		tmp = 4;
	}
	else if(speed < s.width/2 && speed > s.width/4)
	{
		tmp = 5;
	}
	else if(speed < s.width && speed > s.width/2)
	{
		tmp = 6;
	}
	else
	{
		tmp = 7;
	}
	isAction = true;

	CCSequence* showAction = CCSequence::create(
		CCSpawn::create(CCRotateBy::create(0.3f,90*tmp),
		CCScaleTo::create(0.3f,scale),
		NULL),
		CCCallFunc::create(this,callfunc_selector(Hero::enableScale)),
		NULL);
	_hero->sp->runAction(showAction);
}
void Hero::enableScale()
{
	isAction = false;
}
void Hero::calculateScore(float plus)
{
	float score = 0.0f;
	float _hero_r = _hero->sp->boundingBox().size.width/2;

	score += pow(_hero_r * plus,1.1);

	_theGameView->setGameScore(int(score * SCORE_FACTOR));
}
void Hero::heroDead()
{

}
void Hero::gameOver()
{
	CCObject *pObject = NULL;
	CCARRAY_FOREACH_REVERSE(_theGameView->getTheEnemies(), pObject)
	{
		Enemy *enemy = (Enemy*)pObject;
		enemy->getEnemy()->sp->setOpacity(50);
	}
	_hero->sp->setOpacity(50);
	_theGameView->gameOver();
}
void Hero::start()
{
	this->scheduleUpdate();//frame call
}
void Hero::stop()
{
	this->unscheduleUpdate();//frame call

}
void Hero::draw()
{
	
}
void Hero::touchDown(CCPoint pos)
{
	float _hero_r = _hero->sp->boundingBox().size.width/2;

	CCRect touchRect = CCRectMake(pos.x,pos.y,2*_hero_r,2*_hero_r);
	CCRect heroRect = CCRectMake(_hero->pos.x,_hero->pos.y,2*_hero_r,2* _hero_r);

	if(touchRect.intersectsRect(heroRect))
	{
		_hero->isAlive = true;
		_hero->pos = pos;
		fixHeroPos();
		_hero->sp->setPosition(_hero->pos);
		mWarningMinSize->setPosition(_hero->pos);
	}

}
void Hero::fixHeroPos()
{
	CCSize s = CCDirector::sharedDirector()->getVisibleSize();
	float _hero_r = _hero->sp->boundingBox().size.width/2;

	if(_hero->pos.x < _hero_r)
	{
		_hero->pos.x = _hero_r;
	}
	if(_hero->pos.y < _hero_r)
	{
		_hero->pos.y = _hero_r;
	}
	if(_hero->pos.x > s.width - _hero_r)
	{
		_hero->pos.x = s.width - _hero_r;
	}
	if(_hero->pos.y > s.height - _hero_r)
	{
		_hero->pos.y = s.height - _hero_r;
	}
}
void Hero::touchMove(CCPoint pos)
{
	if(_hero->isAlive)
	{
		_hero->pos = pos;
		fixHeroPos();
		_hero->sp->setPosition(_hero->pos);
		mHeroStreak->setPosition(_hero->pos);
		mWarningMinSize->setPosition(_hero->pos);
	}
}
void Hero::touchEnd(CCPoint pos)
{
	if(_hero->isAlive)
	{
		_hero->isAlive = false;
		_hero->pos = pos;
		fixHeroPos();
		_hero->sp->setPosition(_hero->pos);
		mWarningMinSize->setPosition(_hero->pos);
	}
}
