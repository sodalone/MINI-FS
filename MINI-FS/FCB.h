#pragma once

#include <string>
using namespace std;

class FCB
{
public:
	int nodeId;//块号  文件标识：操作系统管理文件的唯一标识。
	int Ftype;//1--文件  2--文件夹
	FCB();
	~FCB();
	string getTime();//获取系统时间
};
