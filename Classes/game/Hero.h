#ifndef HERO_H
#define  HERO_H
#include "cocos2d.h"


using namespace cocos2d;

class GameView;

class HeroAttr{
public:
	ccColor3B color;
	CCSprite *sp;
	CCPoint pos;
	float aliveTime;
	bool isAlive;
};

class Hero : public CCNode
{
public:
	Hero(void);
	~Hero(void);
public:
	void update(float dt);
	void draw(void);
	void start();
	void stop();
	void heroDead();
	void gameOver();
public:
	void touchDown(CCPoint pos);
	void touchMove(CCPoint pos);
	void touchEnd(CCPoint pos);
//
public:
	static Hero* initHeroWithGameView(GameView* game);
	bool initHero(GameView* gameView);
	//getter//setter
	CC_SYNTHESIZE(GameView*, _theGameView, TheGameView);
	CC_SYNTHESIZE(HeroAttr*, _hero, Hero);
private:
	bool isAction;
	float _ori_r;
	CCMotionStreak *mHeroStreak;
	CCSprite *mWarningMinSize;
	void fixHeroPos();
	void calculateScore(float plus);
	bool checkIfCollide(CCPoint pos1,CCPoint pos2,float r1,float r2);
	void runScoreAction(float scale,float speed);
	void enableScale();
};

#endif //
