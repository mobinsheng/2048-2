
#include "Util.h"
#include <time.h>
#include <limits>

int Util::GetRandomNum(int a,int b)// ��ȡ����[a,b)�ڵ������
{

	srand((unsigned)time(NULL)); 

	return rand()%(b-a)+a;
}

float Util::GetProbability()
{
	int n = GetRandomNum(1,101);

	return (n / 100.0);
}