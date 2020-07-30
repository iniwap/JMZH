#include "Enemy.h"
#include "GameView.h"
#include "Hero.h"
#include "../util/GlobalSingleton.h"


using namespace std;

static CCSize viewSize;

#define DEAD_POS 160
#define V_FACTOR (160*3*s_GlobalSingleton->GAME_MODEL)
#define A_FACTOR (100*3*s_GlobalSingleton->GAME_MODEL)
#define R_FACTOR  50//(50*sqrt(1.0*s_GlobalSingleton->GAME_MODEL))

Enemy::Enemy(void)
{

}


Enemy::~Enemy(void)
{

}

Enemy* Enemy::initEnemyWithGameView(GameView* gameView)
{
	Enemy *pRet = new Enemy();
	if (pRet && pRet->initEnemy(gameView))
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
bool Enemy::initEnemy(GameView* gameView)
{
	bool bRet = false;
	do 
	{
		CCSize s = gameView->getContentSize();

		viewSize = CCDirector::sharedDirector()->getVisibleSize();

		_theGameView = gameView;

		_enemy = new EnemyAttr();

		{
			_enemy->ax = (-0.5 + CCRANDOM_0_1())*A_FACTOR/2*(viewSize.width*viewSize.height/(960*640));
			_enemy->ay = CCRANDOM_0_1()*A_FACTOR*(viewSize.width*viewSize.height/(960*640));
			_enemy->vx = fabs(_enemy->ax)/_enemy->ax*V_FACTOR/4*(viewSize.width*viewSize.height/(960*640));
			_enemy->vy = V_FACTOR*(viewSize.width*viewSize.height/(960*640));
			if(CCRANDOM_0_1() <=0.5f)
			{
				_enemy->isWhite = false;
				_enemy->color = ccc3(0,0,0);
				_enemy->sp = CCSprite::create("black.png");
			}
			else
			{
				_enemy->isWhite = true;
				_enemy->color = ccc3(255,255,255);
				_enemy->sp = CCSprite::create("white.png");
			}
			//random pos
			_enemy->pos = CCPointMake(CCRANDOM_0_1()*viewSize.width,viewSize.height + 100);

			//random r according to hero size
			float _enemy_r = CCRANDOM_0_1()*R_FACTOR*(s.width*s.height/(960*640)) + R_FACTOR*(s.width*s.height/(960*640))/2;

			
			//_enemy->sp->setColor(_enemy->color);
			_enemy->sp->setScale(2*_enemy_r/_enemy->sp->getContentSize().width);
			_enemy->sp->setPosition(_enemy->pos);
			//
			_enemy->type = 0;
			//_enemy->rect = ;
		}
		
		this->addChild(_enemy->sp,ENEMY_ZORDER);
		_theGameView->addChild(this,ENEMY_ZORDER);


		bRet = true;

	} while (0);

	return bRet;
}

void Enemy::start()
{
    this->scheduleUpdate();
}
void Enemy::stop()
{
	this->unscheduleUpdate();
}
void Enemy::update(float dt)
{
	CCSize s = CCDirector::sharedDirector()->getVisibleSize();
	float _enemy_r = _enemy->sp->boundingBox().size.width/2;

	float Sx,Sy;

	//v0*t+1/2*at^2
	Sx = _enemy->vx*dt + .5*_enemy->ax*dt*dt;
	Sy = _enemy->vy*dt + .5*_enemy->ay*dt*dt;

	//
	_enemy->vx += _enemy->ax * dt;
	_enemy->vy += _enemy->ay * dt;
	//
	_enemy->pos.x += Sx;
	_enemy->pos.y -= Sy;
	
	_enemy->sp->setPosition(_enemy->pos);

	if(_enemy->pos.y < -_enemy_r - DEAD_POS
		||_enemy->pos.x <= -_enemy_r - DEAD_POS
		||_enemy->pos.x >s.width + _enemy_r + DEAD_POS)
	{
		//
		//remove and add other
		_enemy->sp->removeFromParentAndCleanup(true);
		this->removeFromParentAndCleanup(true);
		_theGameView->getTheEnemies()->removeObject(this);
	}
}
void Enemy::draw()
{
}
