#ifndef __MainView_H__
#define __MainView_H__

#include "cocos2d.h"

class MainView : public cocos2d::CCLayerColor
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    virtual void keyBackClicked(void);
    virtual void keyMenuClicked(void);
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    bool m_bHasClicked;
    void resetBackClick(float dt);
    // a selector callback
    void menuClickCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(MainView);

//////////////////////////////////////////////////////////
//Score,Menu
//////////////////////////////////////////////////////////
};

#endif // __MainView_SCENE_H__
