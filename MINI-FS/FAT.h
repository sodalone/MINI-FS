#pragma once
#include<cstring>
#include<stack>
#include<queue>
#include<algorithm>
const static int TOT_SIZE = 32768 * 32768;  //�ܿռ�1G
const static int BLOCK_SIZE = 1024 * 4;    //���̿��С4k
const static int BLOCK_NUM = 262144;     //���̿����Ŀ

using namespace std;

class FAT
{
public:
	priority_queue <int>freeblock;//��Ϊʹ�ö���ά�������Իָ�ɾ���ļ�
	char recmap[32769];//�����洢��������λͼ
	void init();
	void inittot();
	int getBlock();
	void addBlock(int block);
};

