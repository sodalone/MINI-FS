#pragma once
#include "FCB.h"
#include<vector>


class Folder :public FCB //文件夹
{
public:
	char name[9];//文件名：    文件名
	char create_time[20];//创建时间
	char last_time[20];//最后修改时间
	int size;//文件大小：以字节或块为单位的文件长度。
	int access;//文件的保护方式:1只读 2可写 3可执行
	int father;//指向父文件索引块
	int childsize;
	vector <int >child;//子文件索引表?????????多少个

	Folder(string _name, int  file_access);
	~Folder();

	void addChild(FCB* file);
	bool count(FCB *file);//判断目标文件是否存在
	int find(FCB *file);//找到目标文件并返回文件索引块的块号
	bool delete_folder(FCB* file);//删除文件夹 
};
