#include "FAT.h"

using namespace std;

void FAT::init()//简单初始化
{
	//删除文件目录索引
	priority_queue<int, vector<int>, greater<int> > empty;
	swap(empty, this->freeblock);
	for (int i = 19; i <= BLOCK_NUM; i++) {
		this->freeblock.push(i);
	}
	for (int i = 0; i < 262144; i++)
		recmap[i] = 0;
}
void FAT::inittot()//磁盘初始化
{
	//删除目录索引，同时删除磁盘数据内容。
	//从根目录进行dfs深度搜索整个目录，同时读取文件的数据，深度搜索到文件的最后一个数据块，递归将数据块置为‘/0’。
}

int FAT::getBlock()//获取空磁盘
{
	if (freeblock.size() > 0) {
		int blockId = this->freeblock.top();
		freeblock.pop();
		return blockId;
	}
	return -1;
}
void FAT::addBlock(int block)//回收磁盘块
{
	this->freeblock.push(block);
}

