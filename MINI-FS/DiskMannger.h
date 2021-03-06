#pragma once
#include <cstdio>
#include"FCB.h"
#include "File.h"
#include "Folder.h"

using namespace std;

class DiskMannger {
public:
	DiskMannger(); //构造函数
	~DiskMannger();//析构函数

	void create();//创建1G的Windows空间
	bool mount();//进入已创建空间
	void fmt();//初始化当前空间
	void close();

	void make(); //创建文件
	void mkdir(); //创建文件夹
	File* make(char *, Folder*); //创建文件
	void cp(); //文件或目录的复制
	void cp_in();
	void cp_out();

	void rename(); //文件重命名
	void dr(); //列出文件目录或者其详细信息
	void tree(); //显示空间树状结构
	void read(); //显示空间中的文本文件，只显示文件
	void map();	//显示文件的所有数据块的编号
	void dl(); //删除文件或者文件夹
	void cd(); //进入文件夹
	Folder* cd(string, Folder*);

	void att(); //显示文件属性
	void clear(); //清空回收站
	void rm(); //删除文件
	void search(int, FCB*,FCB *);

	//string split(string str, char * c);
};
