#include "GameScene.h"

USING_NS_CC;

cocos2d::Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	my_drawNode = DrawNode::create();
	this->addChild(my_drawNode);

	my_drawNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	my_drawNode->drawDot(Vec2::ZERO, 16, Color4F::RED);

	auto listenerkeyPad = EventListenerKeyboard::create();//¼üÅÌ¼àÌý
	listenerkeyPad->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	auto listenermouse = EventListenerMouse::create(); //Êó±ê¼àÌý
	listenermouse->onMouseDown = CC_CALLBACK_1(GameScene::onMouseDown, this);
	listenermouse->onMouseUp = CC_CALLBACK_1(GameScene::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenermouse, this);

	//auto listenertouch = EventListenerTouchOneByOne::create();//´¥Ãþ¼àÌý

	scheduleUpdate();

	initKeyCodeMap();
	return true;
}

void GameScene::initKeyCodeMap()
{
	//w -> ¡ü
	keycode_mapping.insert(std::make_pair(EventKeyboard::KeyCode::KEY_W, EventKeyboard::KeyCode::KEY_UP_ARROW));
	//s -> ¡ý
	keycode_mapping.insert(std::make_pair(EventKeyboard::KeyCode::KEY_S, EventKeyboard::KeyCode::KEY_DOWN_ARROW));
	//a -> ¡û
	keycode_mapping.insert(std::make_pair(EventKeyboard::KeyCode::KEY_A, EventKeyboard::KeyCode::KEY_LEFT_ARROW));
	//d -> ¡ú
	keycode_mapping.insert(std::make_pair(EventKeyboard::KeyCode::KEY_D, EventKeyboard::KeyCode::KEY_RIGHT_ARROW));
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	//CCLOG("keyboard pressed -> code = 0x%04x", keycode);
	KeyCodeMap::iterator it = keycode_mapping.find(keycode);
	if (it != keycode_mapping.end()){
		pressed_keys.insert(it->second);
	}else{
		pressed_keys.insert(keycode);
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	KeyCodeMap::iterator it = keycode_mapping.find(keycode);
	if (it != keycode_mapping.end()){
		pressed_keys.erase(it->second);
	}
	else{
		pressed_keys.erase(keycode);
	}
	//CCLOG("keyboard released <- code = 0x%04x", keycode);
}

void GameScene::onMouseDown(EventMouse* event)
{
	mouse_down_pos = event->getLocationInView();
	//CCLOG("onMouseDown [%f,%f]", mouse_pos.x, mouse_pos.y);
	my_drawNode->setPosition(mouse_down_pos);
}

void GameScene::onMouseUp(EventMouse* event)
{
	//Vec2 mouse_pos = event->getLocationInView();
	//CCLOG("onMouseUp [%f,%f]", mouse_pos.x, mouse_pos.y);
}

void GameScene::update(float delta)
{
	float steps = delta * 60 *2;
	Vec2 movevec = Vec2::ZERO;//ÒÆ¶¯·½Ïò
	for (KeyCodeSet::iterator it = pressed_keys.begin(); it != pressed_keys.end(); it++){
		switch (*it){
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			movevec += Vec2::UNIT_Y;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			movevec -= Vec2::UNIT_Y;
			break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			movevec -= Vec2::UNIT_X;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			movevec += Vec2::UNIT_X;
			break;
		default:
			break;
		}
	}
	if (Vec2::ZERO != movevec){//ÊÇ·ñÒÆ¶¯
		my_drawNode->setPosition(my_drawNode->getPosition() + movevec*steps);
	}
}