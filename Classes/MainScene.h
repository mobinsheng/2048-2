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

// ��ʾ��Ϸ�е�һ����
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

	int								m_nX;				// �����꣨�����ڵ��У�������������Եõ���ʵ����Ļ����
	int								m_nY;				// �����꣨�����ڵ��У�������������Եõ���ʵ����Ļ����
	int								m_nValue;			// ֵ
	bool								m_bVisibale;		// �Ƿ���ã����ӣ�ֵΪ0ʱ�����ã�
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

	void											registerEvent();													// ע���¼�

#ifdef _USE_PC_ // ʹ�õ��Ե�ʱ������ü��̽��п���
	void											onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void											onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void											onMouseUp(EventMouse* e);
#else
	bool virtual								touchBegan(Touch*touch,Event*event);			// ��������
	void virtual								touchMoved(Touch *touch,Event *event);			// �����ƶ�����
	void virtual								touchEnded(Touch *touch,Event *event);			// ������������
	void virtual								touchCancelled(Touch*touch,Event *event);
#endif // _USE_PC_

private:

	void											GameProcess();													// ��Ҫ�����߼�

	BLOCK*									GenerateBlock();												// �ڿ��е�λ���������һ���飨2����4��

	void											LoadBackGround();											// ���ر���

	Direction									GetMoveDirection();											// ��ȡ�ƶ��ķ���

	BLOCK*									GetUnUsedBlock();											// ��ȡһ��δʹ�õĿ飨ͨ����GenerateBlockһ��ʹ�ã�

	void											ShowBlock(BLOCK* pBlock);								// ��ʾһ����

	void											ShowScore();													// ��ʾ����

	Point										CalculateBlockPosition(BLOCK* pBlock);			// ����һ�����λ��

	void											MoveUp();														// �����ƶ�
		
	void											MoveDown();													// �����ƶ�

	void											MoveLeft();														// �����ƶ�

	void											MoveRight();														// �����ƶ�

	bool											CheckGameCanContinue();								// �ж���Ϸ�Ƿ񻹿��Լ���������ȥ

	void											GameOver();														// ��Ϸ����

	string										GetPicName(int num);										// ���ݿ��ֵ��ȡ��Ӧ��ͼƬ

	bool											CanMove();														// �ж���ĳһ���������Ƿ�����ƶ�


	bool											IsBlockOfRowContinuity(int y,int direction);		// �ƶ���ʱ������������ƶ�����ô���ұ߿�ʼ��
																															// �ж�һ��ĳһ�е����п��Ƿ����ڣ����ҿ����ұ߽磻����ͬ��

	bool											IsBlockOfColContinuity(int x,int direction);		// �ƶ���ʱ������������ƶ�����ô���±߿�ʼ��
																															// �ж�һ��ĳһ�е����п��Ƿ����ڣ����ҿ����±߽磻����ͬ��

	bool											IsRowCanMerge(int y);										// �ж�ĳһ���Ƿ��п���Ժϲ�

	bool											IsColCanMerge(int x);										// �ж�ĳһ���Ƿ��п���Ժϲ�

private:

	BLOCK										m_BlockMap[TOTAL_BLOCK_NUM][TOTAL_BLOCK_NUM];	// ��ͼ����¼���еĿ�

	vector<BLOCK*>						m_UnUsedBlockList;																// δʹ�õĿ���б�

	Size											m_WinVisibleSize;																	// ���ڴ�С

	Point										m_PrePoint;																			// ��¼ǰһ���������λ�ã������жϻ����ķ���

	Point										m_CurrentPoint;																	// ��¼��ǰ�������λ�ã������жϻ����ķ���

	Direction									m_nDirection;																		// �������ƶ��ķ���

	unsigned long long					m_nScore;																			// �ܵ÷�

	cocos2d::extension::ControlButton*		m_pScoreNode ;														// �����ľ���

	bool											m_bGameOver;																		// ��Ϸ�Ƿ�����ı�־
};

#endif // 
