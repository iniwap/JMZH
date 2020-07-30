#include "GlobalSingleton.h"
using namespace std;

DECLARE_SINGLETONTEMPLATE_MEMBER(GlobalSingleton);


GlobalSingleton::GlobalSingleton(void)
{
	GAME_MODEL = GAME_EASY;
    language = cocos2d::kLanguageChinese;
    best_score = 0;
    current_score = 0;
	scale = 1;
}
GlobalSingleton::~GlobalSingleton(void)
{
}
