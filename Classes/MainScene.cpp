#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <cocos2d.h>
#include <cocos-ext.h>
#include "ui\CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

#include "MainScene.h"
#include "define.h"
#include "Util.h"

Scene* MainScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	// 分数精灵
	m_pScoreNode = 0;

	// 当前得分
	m_nScore = 0;

	// 滑动方向
	m_nDirection = static_direction;

	// 游戏结束标志
	m_bGameOver = false;

	// 获取窗口大小
	m_WinVisibleSize = Director::getInstance()->getVisibleSize(); 

	// 注册事件
	registerEvent();

	// 初始化块地图
	for (int i = 0; i < TOTAL_BLOCK_NUM; ++i)
	{
		for (int j = 0; j < TOTAL_BLOCK_NUM; ++j)
		{
			m_BlockMap[i][j].m_nX = i;
			m_BlockMap[i][j].m_nY = j;

			if (i >= 1 && i < TOTAL_BLOCK_NUM - 1 && j >=1 && j < TOTAL_BLOCK_NUM - 1)
			{
				m_UnUsedBlockList.push_back(&m_BlockMap[i][j]);
			}
		}
	}

	// 显示分数
	ShowScore();

	// 加载背景
	LoadBackGround();

	// 随机产生一个块
	GenerateBlock();

	return true;
}

/************************************************************************
*  游戏的主逻辑
************************************************************************/
void MainScene::GameProcess()
{
	// 判断游戏是否还可以继续玩下去
	if(!CheckGameCanContinue())
	{
		m_bGameOver = true;
		GameOver();
		return;
	}

	// 根据滑动的方向判断屏幕上的块是否可以移动
	if (!CanMove())
	{
		return;
	}

	// 向上滑动
	if (m_nDirection == up_direction)
	{
		MoveUp();
	}

	// 向右滑动
	if (m_nDirection == right_direction)
	{
		MoveRight();
	}

	// 向下滑动
	if (m_nDirection == down_direction)
	{
		m_nDirection = down_direction;
		MoveDown();
	}

	// 向左滑动
	if (m_nDirection == left_direction)
	{
		MoveLeft();
	}

	// 显示分数
	ShowScore();

	// 产生一个随机块
	GenerateBlock();
}

/************************************************************************
*  注册事件
************************************************************************/
void MainScene::registerEvent()
{

#ifdef _USE_PC_ // 使用电脑的时候
	auto listenerKeyboard = EventListenerKeyboard::create();
	listenerKeyboard->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
	listenerKeyboard->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeyboard, this);

	auto _mouseListener = EventListenerMouse::create();
	// 鼠标右键消息
	_mouseListener->onMouseUp = [=](Event *event)
	{
		EventMouse* e = (EventMouse*)event;
		onMouseUp(e);
	};
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener,this);//addEventListenerWithSceneGraphPriority(_mouseListener,this);
#else
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(false);

	listener->onTouchBegan = CC_CALLBACK_2(MainScene::touchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(MainScene::touchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(MainScene::touchEnded,this);
	listener->onTouchCancelled = CC_CALLBACK_2(MainScene::touchCancelled,this);
	_eventDispatcher->addEventListenerWithFixedPriority(listener,-255);

#endif
}

/************************************************************************
*  加载背景
************************************************************************/
void MainScene::LoadBackGround()
{
}

/************************************************************************
*  产生一个随机的块
************************************************************************/
BLOCK* MainScene::GenerateBlock()
{
	// 获取一个未使用的块
	BLOCK* pBlock = GetUnUsedBlock();

	if (pBlock == 0)
	{
		return 0;
	}

	// 设置这个块对应的属性
	pBlock->m_bVisibale = true;

	pBlock->m_nValue = 2;

	// 根据概率设置这个块的值
	double probability =  Util::GetProbability();

	if (probability > PROBABILITY_OF_2)
	{
		pBlock->m_nValue = 4;
	}

	// 显示这个块
	ShowBlock(pBlock);

	return pBlock;
}

/************************************************************************
*  显示一个块
************************************************************************/
void MainScene::ShowBlock(BLOCK* pBlock)
{

	string strNum = Util::ConvertToStr(pBlock->m_nValue);

	// 根据块的值获取对应的图片的路径
	string strPicName = GetPicName(pBlock->m_nValue);

	cocos2d::ui::Scale9Sprite *	pBlockSprite = cocos2d::ui::Scale9Sprite::create(strPicName);

	LabelTTF * pLabel= LabelTTF::create(strNum, "Arial", 25);

	pBlock->m_pNode = cocos2d::extension::ControlButton::create(pLabel , pBlockSprite);

	pBlock->m_pNode->setPreferredSize(Size(BLOCK_SIZE,BLOCK_SIZE));

	pBlock->m_pNode->setPosition(Vec2((BLOCK_SIZE / 2) + pBlock->m_nX* BLOCK_SIZE,(BLOCK_SIZE / 2)+ pBlock->m_nY * BLOCK_SIZE));

	pBlock->m_pNode->setTag(0);

	this->addChild(pBlock->m_pNode);
}

/************************************************************************
*  显示分数
************************************************************************/
void MainScene::ShowScore()
{
	this->removeChild(m_pScoreNode);

	string strNum = Util::ConvertToStr((long long)m_nScore);

	cocos2d::ui::Scale9Sprite *	pBlockSprite = cocos2d::ui::Scale9Sprite::create("");

	LabelTTF * pLabel= LabelTTF::create(strNum, "Arial", 40);

	m_pScoreNode = cocos2d::extension::ControlButton::create(pLabel , pBlockSprite);

	m_pScoreNode->setPreferredSize(Size(BLOCK_SIZE,BLOCK_SIZE));

	m_pScoreNode->setPosition(m_WinVisibleSize.width/2,m_WinVisibleSize.height - 30);

	m_pScoreNode->setTag(256);

	this->addChild(m_pScoreNode , 256);
}

/************************************************************************
*  从未使用的块列表中随机获取一个块
************************************************************************/
BLOCK* MainScene::GetUnUsedBlock()
{
	// 首先先清空这个列表，因为移动之后，每个块的位置都会变得不一样
	m_UnUsedBlockList.clear();

	// 初始化这个列表
	for (int i = 1; i < TOTAL_BLOCK_NUM - 1; ++i)
	{
		for (int j = 1; j < TOTAL_BLOCK_NUM - 1;++j)
		{
			if (m_BlockMap[i][j].m_bVisibale == false)
			{
				m_BlockMap[i][j].m_nX = i;
				m_BlockMap[i][j].m_nY = j;
				m_BlockMap[i][j].m_nValue = 0;
				m_UnUsedBlockList.push_back(&m_BlockMap[i][j]);
			}
		}
	}

	if (m_UnUsedBlockList.size() == 0)
	{
		return 0;
	}

	// 产生一个随机数
	int index = Util::GetRandomNum(0,m_UnUsedBlockList.size());

	vector<BLOCK*>::iterator it = m_UnUsedBlockList.begin() + index;

	BLOCK* pBlock = (*it);

	m_UnUsedBlockList.erase(it);

	return pBlock;
}

/************************************************************************
*  根据一个块计算它在屏幕上的位置
************************************************************************/
Point MainScene::CalculateBlockPosition(BLOCK* pBlock)
{
	Point point;
	point.x = pBlock->m_nX * BLOCK_SIZE + (BLOCK_SIZE / 2);
	point.y = pBlock->m_nY * BLOCK_SIZE + (BLOCK_SIZE / 2);
	return point;
}


/************************************************************************
*  获取滑动的方向
************************************************************************/
Direction MainScene::GetMoveDirection()
{
	int nXDistance = 0;
	int nYDistance = 0;

	if (m_CurrentPoint.x > m_PrePoint.x)
	{
		nXDistance = m_CurrentPoint.x - m_PrePoint.x;

		m_nDirection = right_direction;
	}
	else
	{
		nXDistance = m_PrePoint.x - m_CurrentPoint.x;

		m_nDirection = left_direction;
	}


	Direction tempDirection = static_direction;

	if (m_CurrentPoint.y > m_PrePoint.y)
	{
		nYDistance = m_CurrentPoint.y - m_PrePoint.y;

		tempDirection = up_direction;
	}
	else
	{
		nYDistance = m_PrePoint.y - m_CurrentPoint.y;

		tempDirection = down_direction;
	}

	if (nYDistance > nXDistance)
	{
		m_nDirection = tempDirection;
	}

	return m_nDirection;
}


void MainScene::MoveUp()
{
	for (int x = 1; x < TOTAL_BLOCK_NUM - 1; ++x)
	{
		int nValueArray[TOTAL_BLOCK_NUM] = {0};
		int nValueArrayCount = 0;

		// 将原来的块从屏幕中移除
		// 并记录对应块的值
		for (int y = TOTAL_BLOCK_NUM - 2; y >=  1; --y)
		{
			if(m_BlockMap[x][y].m_bVisibale)
			{
				nValueArray[nValueArrayCount++] = m_BlockMap[x][y].m_nValue;
				m_BlockMap[x][y].m_bVisibale = false;
				removeChild(m_BlockMap[x][y].m_pNode);
				m_BlockMap[x][y].m_nValue = 0;
			}
		}

		if (nValueArrayCount == 0)
		{
			continue;
		}

		if (nValueArrayCount > 1)
		{
			// 如果相邻块的值相同，那么合并
			for (int i = 0; i < nValueArrayCount - 1; ++i)
			{
				if (nValueArray[i] == 0)
				{
					continue;
				}

				if ( nValueArray[i] == nValueArray[i + 1])
				{
					nValueArray[i] *= 2;
					nValueArray[i + 1] = 0;
					m_nScore += nValueArray[i];
				}
			}
		}

		// 存放计算之后的值
		int nRealArray[TOTAL_BLOCK_NUM] = {0};
		int nRealArrayCount = 0;

		for (int i = 0; i < nValueArrayCount; ++i)
		{
			if (nValueArray[i] == 0)
			{
				continue;
			}
			nRealArray[nRealArrayCount++] = nValueArray[i];
		}

		// 重新在屏幕中显示块
		int index = 0;
		int realY = TOTAL_BLOCK_NUM - 2;
		while (index < nRealArrayCount)
		{
			m_BlockMap[x][realY].m_nValue = nRealArray[index];
			m_BlockMap[x][realY].m_bVisibale = true;
			m_BlockMap[x][realY].m_nX = x;
			m_BlockMap[x][realY].m_nY = realY;

			ShowBlock(&m_BlockMap[x][realY]);
			++index;
			--realY;
		}
	}
}

void MainScene::MoveDown()
{
	for (int x = 1; x < TOTAL_BLOCK_NUM - 1; ++x)
	{
		int nValueArray[TOTAL_BLOCK_NUM] = {0};
		int nValueArrayCount = 0;

		// 将原来的块从屏幕中移除
		// 并记录对应块的值
		for (int y = 1; y < TOTAL_BLOCK_NUM - 1; ++y)
		{
			if(m_BlockMap[x][y].m_bVisibale)
			{
				nValueArray[nValueArrayCount++] = m_BlockMap[x][y].m_nValue;
				m_BlockMap[x][y].m_bVisibale = false;
				removeChild(m_BlockMap[x][y].m_pNode);
				m_BlockMap[x][y].m_nValue = 0;
			}
		}

		if (nValueArrayCount == 0)
		{
			continue;
		}

		if (nValueArrayCount > 1)
		{
			// 如果相邻块的值相同，那么合并
			for (int i = 0; i < nValueArrayCount - 1; ++i)
			{
				if (nValueArray[i] == 0)
				{
					continue;
				}

				if ( nValueArray[i] == nValueArray[i + 1])
				{
					nValueArray[i] *= 2;
					nValueArray[i + 1] = 0;
					m_nScore += nValueArray[i];
				}

			}
		}

		// 存放计算之后的值
		int nRealArray[TOTAL_BLOCK_NUM] = {0};
		int nRealArrayCount = 0;

		for (int i = 0; i < nValueArrayCount; ++i)
		{
			if (nValueArray[i] == 0)
			{
				continue;
			}
			nRealArray[nRealArrayCount++] = nValueArray[i];
		}

		// 重新在屏幕中显示块
		int index = 0;
		int realY = 1;
		while (index < nRealArrayCount)
		{
			m_BlockMap[x][realY].m_nValue = nRealArray[index];
			m_BlockMap[x][realY].m_bVisibale = true;
			m_BlockMap[x][realY].m_nX = x;
			m_BlockMap[x][realY].m_nY = realY;

			ShowBlock(&m_BlockMap[x][realY]);

			++index;
			++realY;
		}
	}
}

void MainScene::MoveLeft()
{
	for (int y = 1; y < TOTAL_BLOCK_NUM - 1; ++y)
	{
		int nValueArray[TOTAL_BLOCK_NUM] = {0};
		int nValueArrayCount = 0;

		// 将原来的块从屏幕中移除
		// 并记录对应块的值
		for (int x = 1; x < TOTAL_BLOCK_NUM - 1; ++x)
		{
			if(m_BlockMap[x][y].m_bVisibale)
			{
				nValueArray[nValueArrayCount++] = m_BlockMap[x][y].m_nValue;
				m_BlockMap[x][y].m_bVisibale = false;
				removeChild(m_BlockMap[x][y].m_pNode);
				m_BlockMap[x][y].m_nValue = 0;
			}
		}

		if (nValueArrayCount == 0)
		{
			continue;
		}

		if (nValueArrayCount > 1)
		{
			// 如果相邻块的值相同，那么合并
			for (int i = 0; i < nValueArrayCount - 1; ++i)
			{
				if (nValueArray[i] == 0)
				{
					continue;
				}

				if ( nValueArray[i] == nValueArray[i + 1])
				{
					nValueArray[i] *= 2;
					nValueArray[i + 1] = 0;
					m_nScore += nValueArray[i];
				}
			}
		}

		// 存放计算之后的值
		int nRealArray[TOTAL_BLOCK_NUM] = {0};
		int nRealArrayCount = 0;

		for (int i = 0; i < nValueArrayCount; ++i)
		{
			if (nValueArray[i] == 0)
			{
				continue;
			}
			nRealArray[nRealArrayCount++] = nValueArray[i];
		}

		// 重新在屏幕中显示块
		int index = 0;
		int realX = 1;
		while (index < nRealArrayCount)
		{
			m_BlockMap[realX][y].m_nValue = nRealArray[index];
			m_BlockMap[realX][y].m_bVisibale = true;
			m_BlockMap[realX][y].m_nX = realX;
			m_BlockMap[realX][y].m_nY = y;

			ShowBlock(&m_BlockMap[realX][y]);

			++index;
			++realX;
		}
	}
}

void MainScene::MoveRight()
{
	for (int y = 1; y < TOTAL_BLOCK_NUM - 1; ++y)
	{
		int nValueArray[TOTAL_BLOCK_NUM] = {0};
		int nValueArrayCount = 0;

		// 将原来的块从屏幕中移除
		// 并记录对应块的值
		for (int x = TOTAL_BLOCK_NUM- 2 ; x  >= 1; --x)
		{
			if(m_BlockMap[x][y].m_bVisibale)
			{
				nValueArray[nValueArrayCount++] = m_BlockMap[x][y].m_nValue;
				m_BlockMap[x][y].m_bVisibale = false;
				removeChild(m_BlockMap[x][y].m_pNode);
				m_BlockMap[x][y].m_nValue = 0;
			}
		}

		if (nValueArrayCount == 0)
		{
			continue;
		}

		if (nValueArrayCount > 1)
		{
			// 如果相邻块的值相同，那么合并
			for (int i = 0; i < nValueArrayCount - 1; ++i)
			{
				if (nValueArray[i] == 0)
				{
					continue;
				}

				if ( nValueArray[i] == nValueArray[i + 1])
				{
					nValueArray[i] *= 2;
					nValueArray[i + 1] = 0;
					m_nScore += nValueArray[i];
				}
			}
		}

		// 存放计算之后的值
		int nRealArray[TOTAL_BLOCK_NUM] = {0};
		int nRealArrayCount = 0;

		for (int i = 0; i < nValueArrayCount; ++i)
		{
			if (nValueArray[i] == 0)
			{
				continue;
			}
			nRealArray[nRealArrayCount++] = nValueArray[i];
		}

		// 重新在屏幕中显示块
		int index = 0;
		int realX = TOTAL_BLOCK_NUM - 2;
		while (index < nRealArrayCount)
		{
			m_BlockMap[realX][y].m_nValue = nRealArray[index];
			m_BlockMap[realX][y].m_bVisibale = true;
			m_BlockMap[realX][y].m_nX = realX;
			m_BlockMap[realX][y].m_nY = y;
			ShowBlock(&m_BlockMap[realX][y]);
			++index;
			--realX;
		}
	}
}




#ifdef _USE_PC_

void MainScene::onMouseUp(EventMouse* event)
{
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (m_bGameOver)
	{
		return;
	}

	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		m_nDirection = up_direction;
		GameProcess();
		break;
	case EventKeyboard::KeyCode::KEY_D:
		m_nDirection = right_direction;
		GameProcess();
		break;
	case EventKeyboard::KeyCode::KEY_S:
		m_nDirection = down_direction;
		GameProcess();
		break;
	case EventKeyboard::KeyCode::KEY_A:
		m_nDirection = left_direction;
		GameProcess();
		break;
	default:
		break;
	}
}
#else
bool MainScene::touchBegan(Touch*touch,Event*event)
{
	m_PrePoint = touch->getLocation();
	return true;
}

void MainScene::touchMoved(Touch *touch,Event *event)
{
}

void MainScene::touchEnded(Touch *touch,Event *event)
{
	if (m_bGameOver)
	{
		return;
	}

	m_CurrentPoint = touch->getLocation();

	m_nDirection = GetMoveDirection();

	switch (m_nDirection)
	{
	case static_direction:
		log("static_direction");
		break;
	case left_direction:
		MoveLeft();
		ShowScore();
		GenerateBlock();
		if(!CheckGameCanContinue())
		{
			m_bGameOver = true;
			GameOver();
		}
		break;
	case right_direction:
		MoveRight();
		ShowScore();
		GenerateBlock();
		if(!CheckGameCanContinue())
		{
			m_bGameOver = true;
			GameOver();
		}
		break;
	case up_direction:
		MoveUp();
		ShowScore();
		GenerateBlock();
		if(!CheckGameCanContinue())
		{
			m_bGameOver = true;
			GameOver();
		}
		break;
	case down_direction:
		MoveDown();
		ShowScore();
		GenerateBlock();
		if(!CheckGameCanContinue())
		{
			m_bGameOver = true;
			GameOver();
		}
		break;
	default:
		break;
	}
}

void MainScene::touchCancelled(Touch*touch,Event*event)
{
}
#endif

void MainScene::GameOver()
{
	cocos2d::ui::Scale9Sprite *	pBlockSprite = cocos2d::ui::Scale9Sprite::create("");

	LabelTTF * pLabel= LabelTTF::create("Game Over!", "Arial", 50);

	auto pNode = cocos2d::extension::ControlButton::create(pLabel , pBlockSprite);

	pNode->setPreferredSize(Size(BLOCK_SIZE,BLOCK_SIZE));

	pNode->setPosition(Vec2(m_WinVisibleSize.width/2,m_WinVisibleSize.height / 2));

	pNode->setTag(0);

	this->addChild(pNode);
}

bool MainScene::CheckGameCanContinue()
{
	for (int x = 1; x < TOTAL_BLOCK_NUM - 1; ++x)
	{
		for (int y = 1; y < TOTAL_BLOCK_NUM - 1; ++y)
		{
			if (m_BlockMap[x][y].m_nValue == 0)
			{
				return true;
			}
			

			if (x == 1)		// 左边界
			{
				if (y == 1) // 左下角
				{
					if (m_BlockMap[x][y].m_nValue == m_BlockMap[x + 1][y].m_nValue || 
						m_BlockMap[x][y].m_nValue == m_BlockMap[x][y +1].m_nValue)
					{
						return true;
					}
				}
				else if (y == TOTAL_BLOCK_NUM - 2) // 左上角
				{
					if (m_BlockMap[x][y].m_nValue == m_BlockMap[x][y - 1].m_nValue || 
						m_BlockMap[x][y].m_nValue == m_BlockMap[x + 1][y].m_nValue
						)
					{
						return true;
					}
				}
				else // 普通左边界
				{
					if (m_BlockMap[x][y].m_nValue == m_BlockMap[x+1][y].m_nValue || 
						m_BlockMap[x][y].m_nValue == m_BlockMap[x][y+1].m_nValue ||
						m_BlockMap[x][y].m_nValue == m_BlockMap[x][y-1].m_nValue
						)
					{
						return true;
					}
				}				
			}

			if (x == TOTAL_BLOCK_NUM - 2) // 右边界
			{
				if (y == 1) // 右下角
				{
					if (m_BlockMap[x][y].m_nValue == m_BlockMap[x - 1][y].m_nValue ||
						m_BlockMap[x][y].m_nValue == m_BlockMap[x][y + 1].m_nValue
						)
					{
						return true;
					}

				}
				else if (y == TOTAL_BLOCK_NUM - 2) // 右上角
				{
					if (m_BlockMap[x][y].m_nValue == m_BlockMap[x - 1][y].m_nValue || 
						m_BlockMap[x][y].m_nValue == m_BlockMap[x][y - 1].m_nValue
						)
					{
						return true;
					}

				}
				else // 普通右边界
				{
					if (m_BlockMap[x][y].m_nValue == m_BlockMap[x - 1][y].m_nValue || 
						m_BlockMap[x][y].m_nValue == m_BlockMap[x][y + 1].m_nValue ||
						m_BlockMap[x][y].m_nValue == m_BlockMap[x][y - 1].m_nValue)
					{
						return true;
					}
				}
			}
		
			//////

			if (y == 1)		// 下边界
			{
				if (x == 1) // 左下角
				{
					if (m_BlockMap[x][y].m_nValue == m_BlockMap[x + 1][y].m_nValue || 
						m_BlockMap[x][y].m_nValue == m_BlockMap[x][y +1].m_nValue)
					{
						return true;
					}
				}
				else if (x == TOTAL_BLOCK_NUM - 2) // 左上角
				{
					if (m_BlockMap[x][y].m_nValue == m_BlockMap[x][y - 1].m_nValue || 
						m_BlockMap[x][y].m_nValue == m_BlockMap[x + 1][y].m_nValue
						)
					{
						return true;
					}
				}
				else // 普通下边界
				{
					if (m_BlockMap[x][y].m_nValue == m_BlockMap[x+1][y].m_nValue || 
						m_BlockMap[x][y].m_nValue == m_BlockMap[x - 1][y].m_nValue ||
						m_BlockMap[x][y].m_nValue == m_BlockMap[x][y +1].m_nValue
						)
					{
						return true;
					}
				}				
			}

			if (y == TOTAL_BLOCK_NUM - 2) // 上边界
			{
				if (x == 1) // 右下角
				{
					if (m_BlockMap[x][y].m_nValue == m_BlockMap[x - 1][y].m_nValue ||
						m_BlockMap[x][y].m_nValue == m_BlockMap[x][y + 1].m_nValue
						)
					{
						return true;
					}

				}
				else if (x == TOTAL_BLOCK_NUM - 2) // 右上角
				{
					if (m_BlockMap[x][y].m_nValue == m_BlockMap[x - 1][y].m_nValue || 
						m_BlockMap[x][y].m_nValue == m_BlockMap[x][y - 1].m_nValue
						)
					{
						return true;
					}

				}
				else // 普通上边界
				{
					if (m_BlockMap[x][y].m_nValue == m_BlockMap[x - 1][y].m_nValue || 
						m_BlockMap[x][y].m_nValue == m_BlockMap[x + 1][y ].m_nValue ||
						m_BlockMap[x][y].m_nValue == m_BlockMap[x][y - 1].m_nValue)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

string MainScene::GetPicName(int num)
{
	
	if (num == 2)
	{
		return "image/2.png";
	}

	if (num == 4)
	{
		return "image/4.png";
	}

	if (num == 8)
	{
		return "image/8.png";
	}

	if (num == 16)
	{
		return "image/16.png";
	}

	if (num == 32)
	{
		return "image/32.png";
	}

	if (num == 64)
	{
		return "image/64.png";
	}

	if (num == 128)
	{
		return "image/128.png";
	}

	if (num == 256)
	{
		return "image/256.png";
	}

	if (num == 512)
	{
		return "image/512.png";
	}

	if (num == 1024)
	{
		return "image/1024.png";
	}

	if (num == 2048)
	{
		return "image/2048.png";
	}
}

bool MainScene::CanMove()
{
	if (m_nDirection == up_direction)
	{
		for (int x = 1 ; x < TOTAL_BLOCK_NUM - 1; ++x)
		{
			if (!IsBlockOfColContinuity(x,up_direction))
			{
				return true;
			}

			if (IsColCanMerge(x))
			{
				return true;
			}
		}
	}

	if (m_nDirection == down_direction)
	{
		for (int x = 1 ; x < TOTAL_BLOCK_NUM - 1; ++x)
		{
			if (!IsBlockOfColContinuity(x,down_direction))
			{
				return true;
			}

			if (IsColCanMerge(x))
			{
				return true;
			}
		}
	}

	if (m_nDirection == left_direction)
	{
		for (int  y= 1; y < TOTAL_BLOCK_NUM - 1; ++y)
		{
			if (!IsBlockOfRowContinuity(y,left_direction))
			{
				return true;
			}

			if (IsRowCanMerge(y))
			{
				return true;
			}
		}
	}

	if (m_nDirection == right_direction)
	{
		for (int  y= 1; y < TOTAL_BLOCK_NUM - 1; ++y)
		{
			if (!IsBlockOfRowContinuity(y,right_direction))
			{
				return true;
			}

			if (IsRowCanMerge(y))
			{
				return true;
			}
		}
	}
	return false;
}

// 判断一列中的块是否连续
bool MainScene::IsBlockOfColContinuity(int col,int direction)
{
	bool flag = true;

	if (direction == up_direction)
	{
		int y =  TOTAL_BLOCK_NUM - 1;

		for (y =  TOTAL_BLOCK_NUM - 2; y >= 1; --y)
		{
			if (m_BlockMap[col][y].m_nValue == 0)
			{
				break;
			}
		}

		if (y == 1)
		{
			return true;
		}

		--y;

		for (; y >= 1 ; --y)
		{
			if (m_BlockMap[col][y].m_nValue != 0)
			{
				flag = false;
				break;
			}
		}
	}

	if (direction == down_direction)
	{
		int y =  0;

		for (y =  1; y < TOTAL_BLOCK_NUM - 1; ++y)
		{
			if (m_BlockMap[col][y].m_nValue == 0)
			{
				break;
			}
		}

		if (y == TOTAL_BLOCK_NUM - 2)
		{
			return true;
		}

		++y;

		for (; y < TOTAL_BLOCK_NUM - 1 ; ++y)
		{
			if (m_BlockMap[col][y].m_nValue != 0)
			{
				flag = false;
				break;
			}
		}
	}

	return flag;
}

bool MainScene::IsBlockOfRowContinuity(int row,int direction)
{
	bool flag = true;

	if (direction == right_direction)
	{
		int x = TOTAL_BLOCK_NUM - 1;

		for (x = TOTAL_BLOCK_NUM - 2; x >= 1; --x)
		{
			if (m_BlockMap[x][row].m_nValue == 0)
			{
				break;
			}
		}

		if (x == 1)
		{
			return true;
		}

		--x;

		for (; x >=1 ; --x)
		{
			if (m_BlockMap[x][row].m_nValue != 0)
			{
				flag = false;
				break;
			}
		}
	}

	if (direction == left_direction)
	{
		int x = 0;

		for (x = 1; x < TOTAL_BLOCK_NUM - 1; ++x)
		{
			if (m_BlockMap[x][row].m_nValue == 0)
			{
				break;
			}
		}

		if (x == TOTAL_BLOCK_NUM - 2)
		{
			return true;
		}

		++x;

		for (; x < TOTAL_BLOCK_NUM -1; ++x)
		{
			if (m_BlockMap[x][row].m_nValue != 0)
			{
				flag = false;
				break;
			}
		}
	}

	return flag;
}

bool MainScene::IsRowCanMerge(int y)
{
	int firstValue = 0;
	int secondValue = 0;

	for (int x = 1; x < TOTAL_BLOCK_NUM - 2; ++x)
	{
		if (m_BlockMap[x][y].m_nValue == 0)
		{
			continue;
		}

		if (firstValue == 0)
		{
			firstValue = m_BlockMap[x][y].m_nValue;
		}

		if (secondValue == 0)
		{
			secondValue = m_BlockMap[x+1][y].m_nValue;
		}

		if (firstValue !=0 && secondValue != 0)
		{
			if (firstValue == secondValue)
			{
				return true;
			}

			firstValue = 0;
			secondValue = 0;
		}
	}
	return false;
}

bool MainScene::IsColCanMerge(int x)
{
	int firstValue = 0;
	int secondValue = 0;

	for (int y = 1; y < TOTAL_BLOCK_NUM - 1; ++y)
	{
		if (m_BlockMap[x][y].m_nValue == 0)
		{
			continue;
		}

		if (firstValue == 0)
		{
			firstValue = m_BlockMap[x][y].m_nValue;
		}

		if (secondValue == 0)
		{
			secondValue = m_BlockMap[x][y+1].m_nValue;
		}

		if (firstValue !=0 && secondValue != 0)
		{
			if (firstValue == secondValue)
			{
				return true;
			}

			firstValue = 0;
			secondValue = 0;
		}
	}
	return false;
}