#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "cocos2d.h"


using namespace cocos2d;

class EnemyAttr{
public:
	ccColor3B color;
	bool isWhite;
	CCRect rect;
	//float r;
	int type;
	//
	float vx;
	float vy;
	float ax;
	float ay;
	CCPoint pos;
	CCSprite *sp;
};

class GameView;

class Enemy : public CCNode
{
public:
	Enemy(void);
	virtual ~Enemy(void);

public:
	static Enemy* initEnemyWithGameView(GameView* gameView);
	bool initEnemy(GameView* gameView);
public:
	void update(float dt);
	void draw(void);
    void start();
	void stop();
public:
	CC_SYNTHESIZE(GameView*, _theGameView, TheGameView);

	CC_SYNTHESIZE(EnemyAttr*,_enemy,Enemy);

	CC_SYNTHESIZE(bool,_balloonHasCollide,BalloonHasCollide);
};
#endif

