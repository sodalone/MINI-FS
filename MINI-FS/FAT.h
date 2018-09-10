#pragma once
#include<cstring>
#include<stack>
#include<queue>
#include<algorithm>
const static int TOT_SIZE = 32768 * 32768;  //总空间1G
const static int BLOCK_SIZE = 1024 * 4;    //磁盘块大小4k
const static int BLOCK_NUM = 262144;     //磁盘块的数目

using namespace std;

class FAT
{
public:
	priority_queue <int>freeblock;//改为使用队列维护，用以恢复删除文件
	char recmap[32769];//用来存储垃圾回收位图
	void init();
	void inittot();
	int getBlock();
	void addBlock(int block);
};

