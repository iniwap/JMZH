#include "PlatformInterface.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	#include "PlatformInterfaceIOS.h"
#endif
 
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#include "PlatformInterfaceAndroid.h"
#endif


void PlatformInterface::showAdView(int type)
{
//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlatformInterfaceIOS::showAdView(type);
#endif
//
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    PlatformInterfaceAndroid::showAdView(type);
#endif
   
}
void PlatformInterface::hideADView(int type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlatformInterfaceIOS::hideADView(type);
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    PlatformInterfaceAndroid::hideADView(type);
#endif
   
}
void PlatformInterface::share(int sns,int level,int score)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PlatformInterfaceIOS::share(sns,level,score);
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    PlatformInterfaceAndroid::share(sns,level,score);
#endif
}
void PlatformInterface::showToast(std::string content,int time)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    PlatformInterfaceAndroid::showToast(content,time);
#endif
}
