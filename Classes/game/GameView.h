#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "Hero.h"

#define MAX_GENERATE 9
typedef enum
{
	BG_ZORDER = 0,
	ENEMY_ZORDER = 101,
	HERO_ZORDER = 201,
}LayerZoder;

class Hero;


class GameView : public cocos2d::CCLayerColor
{
public:
	GameView();
	~GameView();
public:
	virtual bool init();  
	static cocos2d::CCScene* scene();
	void menuClickCallback(CCObject* pSender);
	CREATE_FUNC(GameView);

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	//
private:

	void backToMainView(float dt);
	void initScore();
private:
	float _time;

	int _gameScore;
	int _forAnimateScore;
	CCLabelBMFont* _gameScoreLabel;

    CC_SYNTHESIZE(bool, _isGameStart, IsGameStart);
public:
	CC_SYNTHESIZE(int, _time_count,TimeCT);
	CC_SYNTHESIZE(Hero*, _theHero,TheHero);
	CC_SYNTHESIZE(CCArray *, _enemies,TheEnemies);

	void setGameScore(int score);
	int getGameScore();
public:
	void goToGameOver();
	void gameOver();
	void update(float dt);
    void start();

	void stopGame();
	void resumeGame();
	void enemyGenerate(float dt);
	CCSprite *createNumLabel(int num,int size);
	void updateScore(CCNode *pTarget,void *data);
};

#endif // __GAME_VIEW_H__
