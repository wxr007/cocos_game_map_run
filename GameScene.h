#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include <set>
#include <map>

USING_NS_CC;

typedef std::set<EventKeyboard::KeyCode> KeyCodeSet;
typedef std::map<EventKeyboard::KeyCode, EventKeyboard::KeyCode> KeyCodeMap;

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	//init the keycode map
	//初始化按键映射
	void initKeyCodeMap();

	void onKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *event);//keyboard pressed
	void onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event);//keyboard Released

	void onMouseDown(EventMouse* event);
	void onMouseUp(EventMouse* event);

	void update(float delta);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
private:
	cocos2d::DrawNode* my_drawNode;
	//record which keys is pressed
	//记录哪些键被按下
	KeyCodeSet pressed_keys;
	//record where mouse down
	//记录鼠标被按下的位置
	Vec2 mouse_down_pos;
	//mapping the keycode
	//按键映射
	KeyCodeMap keycode_mapping;

	cocos2d::Sprite* my_sprite;

	cocos2d::TMXTiledMap* map;
};

#endif // __GAME_SCENE_H__