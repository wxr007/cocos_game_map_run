#include "GameScene.h"

USING_NS_CC;

#define TILEMAP_ZORDER 0
#define TILEMAP_TAG 1

// OpenGL坐标：原点为屏幕左下角（单位：像素）  
// tile坐标：原点为瓦片地图的左上角（单位：瓦片）  

// OpenGL坐标 转成 格子坐标  
Vec2 tileCoordForPosition(const TMXTiledMap* tiledMap,const Vec2& position) {
	Size mapSize = tiledMap->getMapSize();
	Size tileSize = tiledMap->getTileSize();
	int x = position.x / tileSize.width;
	int y = (mapSize.height * tileSize.height - position.y) / tileSize.height;
	return Vec2(x, y);
}

// tile坐标 转成 瓦片格子中心的OpenGL坐标  
Vec2 positionForTileCoord(const TMXTiledMap* tiledMap,const Vec2& tileCoord) {
	Size mapSize = tiledMap->getMapSize();
	Size tileSize = tiledMap->getTileSize();
	int x = tileCoord.x * tileSize.width + tileSize.width / 2;
	int y = (mapSize.height - tileCoord.y) * tileSize.height - tileSize.height / 2;
	return Vec2(x, y);
}

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

	//load map
	map = TMXTiledMap::create("Maps/test.tmx");
	this->addChild(map, TILEMAP_ZORDER, TILEMAP_TAG);

	TMXLayer* treelayer = map->getLayer("tree");

	treelayer->setTileGID(2, Vec2(5, 6));//第二个是小人
	my_sprite = treelayer->getTileAt(Vec2(5, 6));

	my_sprite->setLocalZOrder(10);

	//my_sprite = Sprite::createWithTexture(treelayer->getTexture(), Rect(2+101+2, 0, 101, 171));

	//my_sprite = Sprite::create("Maps/player.png");

	//treelayer->addChild(my_sprite);
	my_sprite->retain();
	//my_sprite->setAnchorPoint(Vec2(0.5f, 0));
	//my_sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

// 	my_drawNode = DrawNode::create();
// 	this->addChild(my_drawNode);
// 
// 	my_drawNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
// 	my_drawNode->drawDot(Vec2::ZERO, 16, Color4F::RED);

	auto listenerkeyPad = EventListenerKeyboard::create();//键盘监听
	listenerkeyPad->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	auto listenermouse = EventListenerMouse::create(); //鼠标监听
	listenermouse->onMouseDown = CC_CALLBACK_1(GameScene::onMouseDown, this);
	listenermouse->onMouseUp = CC_CALLBACK_1(GameScene::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenermouse, this);

	//auto listenertouch = EventListenerTouchOneByOne::create();//触摸监听

	scheduleUpdate();

	initKeyCodeMap();
	return true;
}

void GameScene::initKeyCodeMap()
{
	//w -> ↑
	keycode_mapping.insert(std::make_pair(EventKeyboard::KeyCode::KEY_W, EventKeyboard::KeyCode::KEY_UP_ARROW));
	//s -> ↓
	keycode_mapping.insert(std::make_pair(EventKeyboard::KeyCode::KEY_S, EventKeyboard::KeyCode::KEY_DOWN_ARROW));
	//a -> ←
	keycode_mapping.insert(std::make_pair(EventKeyboard::KeyCode::KEY_A, EventKeyboard::KeyCode::KEY_LEFT_ARROW));
	//d -> →
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
	}else{
		pressed_keys.erase(keycode);
	}
	//CCLOG("keyboard released <- code = 0x%04x", keycode);
}

void GameScene::onMouseDown(EventMouse* event)
{
	mouse_down_pos = event->getLocationInView();
	CCLOG("onMouseDown [%f,%f]", mouse_down_pos.x, mouse_down_pos.y);
//	my_sprite->setPosition(mouse_down_pos);

	auto title_pos =  tileCoordForPosition(map,mouse_down_pos);
	CCLOG("title_pos [%f,%f]", title_pos.x, title_pos.y);

	TMXLayer* earthlayer = map->getLayer("earth");
	int32_t gid = earthlayer->getTileGIDAt(title_pos);
	CCLOG("point gid = %d", gid);

	TMXLayer* treelayer = map->getLayer("tree");
	auto title_sprite = treelayer->getTileAt(title_pos);
	if (title_sprite){
		int Z = title_sprite->getLocalZOrder();
		CCLOG("zorder = %d", Z);
	}
}

void GameScene::onMouseUp(EventMouse* event)
{
	//Vec2 mouse_pos = event->getLocationInView();
	//CCLOG("onMouseUp [%f,%f]", mouse_pos.x, mouse_pos.y);
}

void GameScene::update(float delta)
{
	float steps = delta * 60 *2;
	Vec2 movevec = Vec2::ZERO;//移动方向
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
	if (Vec2::ZERO != movevec){//是否移动
		my_sprite->setPosition(my_sprite->getPosition() + movevec*steps);
	}
}