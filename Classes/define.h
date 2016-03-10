#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <vector>
#include <string>
using namespace std;

// ����ͼƬ
#define BACK_IMG ("image/back.png")

// ǰ��ͼƬ
#define FRONT_IMG ("image/front.png")

// ��Ĵ�С
#define BLOCK_SIZE (50)

// ������������ʵ�Ŀ������
#define REAL_BLOCK_NUM (4)

// ��ͼ�Ͽ������
#define TOTAL_BLOCK_NUM (6)

// ����ֵΪ2�ĸ���
#define PROBABILITY_OF_2 (0.6)

// ʹ��PC�ĺ�
#define _USE_PC_

// ����
enum Direction
{
	static_direction,		// �޷��򣨻�ֹ��
	left_direction,			// ��
	right_direction,			// ��
	up_direction,				// ��
	down_direction			// ��
};

#endif // !_DEFINE_H_
