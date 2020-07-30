#ifndef __GameOverView_H__
#define __GameOverView_H__

#include "cocos2d.h"

class CCNumber : public cocos2d::CCNode
{
public:
	int num;
	CREATE_FUNC(CCNumber);
};

class GameOverView : public cocos2d::CCLayerColor
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuClickCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(GameOverView);

public:
    int m_title;
    void switchTitle(float dt);
	 static cocos2d::CCSprite *createNumLabel(int num);
	 static float getScaledValue(float value);
	 static cocos2d::CCSprite *getScaledSprite(const char* filename);
};

#endif // __GameOverView_SCENE_H__
