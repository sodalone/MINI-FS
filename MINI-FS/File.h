#pragma once
#include "FCB.h"
#include"FAT.h"

class File :public FCB //文件 
{
public:
	int size;//文件大小：以字节或块为单位的文件长度。
	int access;//文件的保护方式:1只读 2可写 3可执行
	int father;//父文件节点
	int data;//指向第一个数
	char name[9];//文件名：    文件名
	short create_time[6];//创建时间
	short last_time[6];//最后修改时间
	char type[4];//文件类型：由扩展名给出。
	int flag;//标记文件夹是否已经被删除
	File();
	File(string _name, int file_access);
	~File();

	bool write(const char * content, int data, FAT fat_temp, FILE *fp);//写文件 
	void delete_file(File*, FILE*, FAT*);//释放磁盘块
	FCB * fa_node;
};
