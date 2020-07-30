#ifndef __PLATFORM_INTERFACE_H__
#define __PLATFORM_INTERFACE_H__

#include "cocos2d.h"

namespace PlatformInterface {
    void showAdView(int type);
    void hideADView(int type);
    void share(int sns,int level,int score);
    void showToast(std::string content,int time);
};


#endif  // __PLATFORM_INTERFACE_H__
