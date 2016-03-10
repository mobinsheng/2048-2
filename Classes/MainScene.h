#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <cocos2d.h>
#include <cocos-ext.h>
#include "ui\CocosGUI.h"

#include "define.h"
USING_NS_CC; 

// 表示游戏中的一个块
struct BLOCK
{
	BLOCK()
	{
		m_pNode = NULL;
		m_nX = -1;
		m_nY = -1;
		m_bVisibale = false;
		m_nValue = 0;
	}

	cocos2d::extension::ControlButton * m_pNode;

	int								m_nX;				// 列坐标（即所在的列），经过计算可以得到真实的屏幕坐标
	int								m_nY;				// 行坐标（即所在的行），经过计算可以得到真实的屏幕坐标
	int								m_nValue;			// 值
	bool								m_bVisibale;		// 是否可用（可视，值为0时不可用）
};

typedef BLOCK* PBLOCK;

typedef vector<BLOCK*>	 VecBlockIter;

class MainScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MainScene);

public:

	void											registerEvent();													// 注册事件

#ifdef _USE_PC_ // 使用电脑的时候可以用键盘进行控制
	void											onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void											onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void											onMouseUp(EventMouse* e);
#else
	bool virtual								touchBegan(Touch*touch,Event*event);			// 触摸函数
	void virtual								touchMoved(Touch *touch,Event *event);			// 触摸移动函数
	void virtual								touchEnded(Touch *touch,Event *event);			// 触摸结束函数
	void virtual								touchCancelled(Touch*touch,Event *event);
#endif // _USE_PC_

private:

	void											GameProcess();													// 主要处理逻辑

	BLOCK*									GenerateBlock();												// 在空闲的位置随机产生一个块（2或者4）

	void											LoadBackGround();											// 加载背景

	Direction									GetMoveDirection();											// 获取移动的方向

	BLOCK*									GetUnUsedBlock();											// 获取一个未使用的块（通常和GenerateBlock一起使用）

	void											ShowBlock(BLOCK* pBlock);								// 显示一个块

	void											ShowScore();													// 显示分数

	Point										CalculateBlockPosition(BLOCK* pBlock);			// 计算一个块的位置

	void											MoveUp();														// 向上移动
		
	void											MoveDown();													// 向下移动

	void											MoveLeft();														// 向左移动

	void											MoveRight();														// 向右移动

	bool											CheckGameCanContinue();								// 判断游戏是否还可以继续进行下去

	void											GameOver();														// 游戏结束

	string										GetPicName(int num);										// 根据块的值获取对应的图片

	bool											CanMove();														// 判断在某一个方向上是否可以移动


	bool											IsBlockOfRowContinuity(int y,int direction);		// 移动的时候，如果是向右移动，那么从右边开始，
																															// 判断一个某一行的所有块是否都相邻，并且靠近右边界；向左同理

	bool											IsBlockOfColContinuity(int x,int direction);		// 移动的时候，如果是向下移动，那么从下边开始，
																															// 判断一个某一列的所有块是否都相邻，并且靠近下边界；向上同理

	bool											IsRowCanMerge(int y);										// 判断某一行是否有块可以合并

	bool											IsColCanMerge(int x);										// 判断某一列是否有块可以合并

private:

	BLOCK										m_BlockMap[TOTAL_BLOCK_NUM][TOTAL_BLOCK_NUM];	// 地图，记录所有的块

	vector<BLOCK*>						m_UnUsedBlockList;																// 未使用的块的列表

	Size											m_WinVisibleSize;																	// 窗口大小

	Point										m_PrePoint;																			// 记录前一个触摸点的位置（用于判断滑动的方向）

	Point										m_CurrentPoint;																	// 记录当前触摸点的位置（用于判断滑动的方向）

	Direction									m_nDirection;																		// 滑动（移动的方向）

	unsigned long long					m_nScore;																			// 总得分

	cocos2d::extension::ControlButton*		m_pScoreNode ;														// 分数的精灵

	bool											m_bGameOver;																		// 游戏是否结束的标志
};

#endif // 
