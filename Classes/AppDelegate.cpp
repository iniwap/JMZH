#include "AppDelegate.h"
#include "game/MainView.h"
#include "SimpleAudioEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "GKHWrapperCpp.h"
#endif

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

	//add search path may be removed
	std::vector<std::string> path;
    path.push_back("fonts");
	path.push_back(".");
	path.push_back("./Resources");
	CCFileUtils::sharedFileUtils()->setSearchPaths(path);


    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = MainView::scene();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//game center
    GKHWrapperCpp gkh;
    gkh.authenticateLocalPlayer();
    
#endif
    
    
    
    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
     SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
