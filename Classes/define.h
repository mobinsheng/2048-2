#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <vector>
#include <string>
using namespace std;

// 背景图片
#define BACK_IMG ("image/back.png")

// 前景图片
#define FRONT_IMG ("image/front.png")

// 块的大小
#define BLOCK_SIZE (50)

// 横向（纵向）上真实的块的数量
#define REAL_BLOCK_NUM (4)

// 地图上块的数量
#define TOTAL_BLOCK_NUM (6)

// 产生值为2的概率
#define PROBABILITY_OF_2 (0.6)

// 使用PC的宏
#define _USE_PC_

// 方向
enum Direction
{
	static_direction,		// 无方向（或静止）
	left_direction,			// 左
	right_direction,			// 右
	up_direction,				// 上
	down_direction			// 下
};

#endif // !_DEFINE_H_
