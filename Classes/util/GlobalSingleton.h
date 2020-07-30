/**
*
*ºÃ≥–◊‘µ•¿˝¿‡£¨”√”⁄±£¥Ê»´æ÷∂‘œÛµƒ∑√Œ 
*
*/

#ifndef __GLOBAL_SINGLETON_H__
#define __GLOBAL_SINGLETON_H__

#include "SingletonTemplate.h"

#define FONT_NAME "fonts/Marker Felt.ttf"//"A Damn Mess"

#include "cocos2d.h"

typedef enum
{
	GAME_EASY = 1,
	GAME_HARD,
	GAME_TERRIBLE,
    GAME_ET,
}GameModel;
class GlobalSingleton : public SingletonTemplate<GlobalSingleton>
{
public:
	GlobalSingleton(void);
	~GlobalSingleton(void);
public:
	GameModel GAME_MODEL;
    cocos2d::LanguageType language;
    int best_score;
    int current_score;
	float scale;
};

#define s_GlobalSingleton GlobalSingleton::instance()

#endif  // __GLOBAL_SINGLETON_H__