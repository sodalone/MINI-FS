#pragma once
#include <cstdio>
#include"FCB.h"
using namespace std;
class DiskMannger {
public:
	DiskMannger(); //构造函数
	~DiskMannger();//析构函数

	void create();//创建1G的Windows空间
	void mount();//进入已创建空间
	void fmt(FILE *fp);//初始化当前空间

	void make(); //创建文件
	void open(); //打开文件
	void exit(); //退出当前打开文件
	void write(); //文件的写入；
	void read(); //文件的读入；
	void cp(); //文件或目录的复制
	void rename(); //文件重命名
	void dr(); //列出文件目录或者其详细信息
	void tp(int curBlockNum); //显示空间中的文本文件，只显示文件
	void dl(); //删除文件或者文件夹
	void cd(); //进入文件夹
	void att(); //显示文件属性
	void more(); //分页显示
	void back(); //进入上级目录
	void recover(); //恢复一个删除文件
	void clear(); //清空回收站
	void rm(); //删除文件
	void search (int, FCB*,FCB *);//建立一个文件索引树
};
