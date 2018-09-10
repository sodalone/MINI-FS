#pragma once
#include "FCB.h"
class Block :public FCB//数据块
{
	int nextblock;//指向下一个数据块
	char data[4092];//记录数据

	Block();
	~Block();

	void clear();//清除块内数据
	void write();//疑问????????数据块写入数据到底要怎么写入？
};