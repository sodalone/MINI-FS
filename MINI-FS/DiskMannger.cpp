#define _CRT_SECURE_NO_WARNINGS
#include "DiskMannger.h"
#include<cstring>
#include<cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include<queue>
#include"FAT.h"
#include<io.h>
#include <time.h>
using namespace std;



 const static int ALL_DATA_BLOCK_NUM = 262127;
 FAT fat = FAT();
 FILE * root_fp;


DiskMannger::DiskMannger() {
	cout << "欢迎！！-----------您可输入help获得帮助------------" << endl;
	string cmd;
	while (cin >> cmd) {
		if (cmd == "help") {
			cout << "帮助文档" << endl;
			cout << "\n●fmt:\n" <<
				"●close:\n" <<
				"●make:\n" <<
				"●open : \n" <<
				"●exit : \n" <<
				"●write : \n" <<
				"●read : \n" <<
				"●cp : \n" <<
				"●dr : \n" <<
				"●dl : \n" <<
				"●tp : \n" <<
				"●more : \n" <<
				"●att : \n" <<
				"●back :\n" <<
				"●rename:\n" <<
				"●recover:\n" <<
				"●clear:\n" <<
				"●rm:\n" <<
				"●* :\n" <<
				"●? :\n"<< endl;
		}
		else if (cmd == "create") {
			this->create();
		}
		else if (cmd == "mount") {
			this->mount();
		}
		else if (cmd == "fmt") {
			this->fmt(root_fp);/////////////这里该传个啥？？启动块指针即root_fp呀？？
		}
		else if (cmd == "close") {
			cout << "退出当前空间" << endl;
			break;
		}
		else if (cmd == "make") {
			this->make();
		}
		else if (cmd == "open") {
			this->open();
		}
		else if (cmd == "exit") {
			this->exit();
		}
		else if (cmd == "write") {
			this->write();
		}
		else if (cmd == "read") {
			this->read();
		}
		else if (cmd == "exit") {
			this->exit();
			return;
		}
		else if (cmd == "cp") {
			this->cp();
		}
		else if (cmd == "rename") {
			this->rename();
		}
		else if (cmd == "dr") {
			this->dr();
		}
		else if (cmd == "tp") {
			this->tp(5);
		}
		else if (cmd == "dl") {
			this->dl();
		}
		else if (cmd == "cd") {
			this->cd();
		}
		else if (cmd == "att") {
			this->att();
		}
		else if (cmd == "more") {
			this->more();
		}
		else if (cmd == "back") {
			this->back();
		}
		else if (cmd == "recover") {
			this->recover();
		}
		else if (cmd == "clear") {
			this->clear();
		}
		else if (cmd == "rm") {
			this->rm();
		}
		else {
			cout << "输入指令错误，请重新输入！！" << endl;
		}
	}
}
DiskMannger::~DiskMannger(){}

void DiskMannger::create() {
	char SName[9];
	memset(SName, 0, sizeof(SName));
	cin >> SName;
	SName[8] = '\0';
	cout << "创建"<< SName <<"空间" << endl; 
	char magic[8] = { 'm','i','n','i','f','s','s','s' };
	if (_access(SName, 0) == 0)
	{
		//已经存在这个文件系统
		FILE* fp = fopen(SName, "r"); //读取这个文件系统
		fseek(fp, 0, SEEK_SET);//回到了开始的位置
		char buf[8];
		fread(buf, 1, 8, fp);//读取magic number匹配系统
		bool flag = true;
		for (int i = 0; i < 8 && flag; i++)
			if (magic[i] != buf[i]) flag = false;
		if (1)//如果匹配
		{
			printf("此文件系统已经被建立！请重新输入……\n");
			return ;
		}
	}
		FILE *fp = fopen(SName, "w+"); //读取这个文件系统
		fseek(fp, 0, SEEK_SET);//回到了开始的位置
		printf("初次建立文件系统，请稍等\n");
		
		int num = 8;
		int usenum = 262126;
		int binnum = 0;
		fseek(fp, 32768*32768 - 1, SEEK_SET); // 将文件的指针 移至 指定大小的位置 32768*32768
		fputc('a', fp); // 在要指定大小文件的末尾随便放一个数据
		fseek(fp, 0, SEEK_SET);//回到了开始的位置
		fwrite(magic,sizeof(char), 8, fp);
		fwrite(&BLOCK_SIZE, sizeof(int), 1, fp);
		fwrite(&num, sizeof(int), 1, fp);
		fwrite(&BLOCK_NUM, sizeof(int), 1, fp);
		fwrite(&usenum, sizeof(int), 1, fp);
		fwrite(&binnum, sizeof(int), 1, fp);
		num = 0;
		unsigned char t=255;
		fseek(fp, 0, SEEK_SET);
		fseek(fp, 1024 * 4, SEEK_CUR);//跳过启动块
		//freebitmap 置1 表示所有块都可用
		for(int i = 0; i < 32768; i ++)
			fwrite(&t, sizeof(char) , 1, fp);
		//recbitmap置0 表示没有块是可回收块
		t = 0;
		for (int i = 0; i < 32768; i++)
			fwrite(&t, sizeof(char), 1, fp);
		//创建一个根目录 在18块记录根目录信息
		fseek(fp, 0, SEEK_SET);
		fseek(fp, 1024 * 4 * 17, SEEK_CUR);

		int nodeId = 18;
		int Ftype = 2;
		int size = 0;
		int access = 3;
		int father = 0;
		//获取时间
		time_t timep;
		struct tm *p;
		time(&timep);
		p = gmtime(&timep);
		short create_time[6];
		short last_time[6];
		last_time[0] = create_time[0] = 1900 + p->tm_year;
		last_time[1] = create_time[1] = 1 + p->tm_mon;
		last_time[2] = create_time[2] = p->tm_mday;
		last_time[3] = create_time[3] = 8 + p->tm_hour;
		last_time[4] = create_time[4] = p->tm_min;
		last_time[5] = create_time[5] = p->tm_sec;

		int childsize = 0;
		int tmp = 0;//占位
		fwrite(&nodeId, sizeof(int), 1, fp);
		fwrite(&Ftype, sizeof(int), 1, fp);
		fwrite(&size, sizeof(int), 1, fp);
		fwrite(&access, sizeof(int), 1, fp);
		fwrite(&father, sizeof(int), 1, fp);
		fwrite(SName, sizeof(char), 9, fp);
		fwrite(create_time, sizeof(short), 6, fp);
		fwrite(last_time, sizeof(short), 6, fp);
		fwrite(&childsize, sizeof(int), 1, fp);
		for(int i = 0; i < 512; i ++)
		fwrite(&tmp, sizeof(int), 1 ,fp);

		printf("ok\n");
		fseek(fp, 0, SEEK_SET);
		root_fp = fopen(SName, "r+");
}





void DiskMannger::mount() 
{//挂载到已经有的文件系统
	char SName[9];
	memset(SName, 0, sizeof(SName));
	cin >> SName;
	SName[8] = '\0';
	char magic[8] = { 'm','i','n','i','f','s','s','s' };
	if (_access(SName, 0) == 0)//已经存在该系统
	{
		FILE *fp = fopen(SName, "r");
		root_fp = fp;
		fseek(fp, 0, SEEK_SET);//回到了开始的位置
		char buf[8];
		fread(buf, 1, 8, fp);//读取magic number匹配系统
		bool flag = true;
		for (int i = 0; i < 8 && flag; i++)
			if (magic[i] != buf[i]) flag = false;
		if (flag)//如果匹配 载入之前记录的可用块、可回收块的块号信息
		{
			int count_free, count_rec;//对可用块、可回收块计数
			count_free = count_rec = 0;
			fseek(fp, 0, SEEK_SET);
			fseek(fp, 4096 + 2, SEEK_CUR);//指向第一个bitmap并从第三字节开始计算
										//1~18块已被占用
			char now;
			fread(&now, sizeof(char), 1, fp);
			for(int i = 0; i < 6; i ++)//特判19~24块
				if (((now >> i ) & 1)== 1)
				{
					count_free++;
					fat.freeblock.push(24 - i);
				}
			for (int i = 3; i < 32768; i++)//从25判断
			{
				fread(&now, sizeof(char), 1, fp);
				for (int j = 0; j < 8; j++)
					if (((now >> j ) & 1)== 1)
					{
						count_free++;
						fat.freeblock.push((i + 1) * 8 - j);
					}
			}
			fseek(fp, 2, SEEK_CUR);//可回收bitmap，从第三字节计算 1~18块占用
			fread(&now, sizeof(char), 1, fp);
			fat.recmap[2] = now;
			for (int i = 0; i < 6; i++) //特判19~24块
				if (((now >> i) & 1) == 1)
					count_rec++;
			for (int i = 3; i < 32768; i++)//从25判断
			{
				fread(&now, sizeof(char), 1, fp);
				fat.recmap[i] = now;
				for (int j = 1; j <= 8; j++)
					if ((now >> (j - 1) & 1) == 1)
						count_rec++;
			}


			printf("当前文件系统中共有%d空块可用,共%lld字节\n", count_free,(long long) count_free * 4 * 1024);
			printf("当前文件系统中共有%d可回收块可用,共%lld字节\n", count_rec, (long long)count_rec * 4 * 1024 );


			root_fp = fopen(SName, "r+");

			fseek(root_fp, 8 + 3 * sizeof(int), SEEK_SET);
			fwrite(&count_free, sizeof(int), 1, root_fp);
			fwrite(&count_rec, sizeof(int), 1, root_fp);
			return;
		}
		else
		{
			printf("该系统不存在且存在同名文件！\n");
			printf("请使用create命令创建其他名称的文件系统！\n");
			return ;
		}
	}
	else
	{
		printf("该系统不存在！\n");
		printf("请使用create命令先进行创建！\n");
		return;
	}
}

void DiskMannger::fmt(FILE *fp) {////////////////fp指针指向什么？
	string flag;
	cin >> flag;
	if (flag == "-1") {
		cout << "简单初始化，仅删除目录文件，磁盘数据不进行重写" << endl;
		char format = '\0';
		fseek(fp, 22, SEEK_SET);///////位置查错了？？？recmap？？？freebitmap位置不对????

		fwrite(&ALL_DATA_BLOCK_NUM, sizeof(int), 1, fp);	//将可用空间置为满状态：262127 
		fwrite(&format, sizeof(char), 17 * 4 * 1024 - 26, fp);//26为之前指针经过的字节数 ，并进行freebitmap，recbitmapp的格式化  

		fwrite(&format, sizeof(char), 16, fp);	//根目录的部分格式化 

		fseek(fp, 49, SEEK_CUR);	//根目录的文件夹名称，创建时间不做修改，最后修改时间暂不更改。 

		fwrite(&format, sizeof(char), 2048, fp);//根目录的子目录、子文件索引的初始化 
	}
	else if (flag == "-2") {
		cout << "普通初始化，删除目录文件，同时清空所有的磁盘数据" << endl;

		char format = '\0';
		fseek(fp, 22, SEEK_SET);

		fwrite(&ALL_DATA_BLOCK_NUM, sizeof(int), 1, fp);	//将可用空间置为满状态：262127 
		fwrite(&format, sizeof(char), 17 * 4 * 1024 - 26, fp);//26为之前指针经过的字节数 ，并进行freebitmap，recbitmapp的格式化  

		fwrite(&format, sizeof(char), 16, fp);	//根目录的部分格式化 

		fseek(fp, 49, SEEK_CUR);	//根目录的文件夹名称，创建时间不做修改，最后修改时间暂不更改。 

		fwrite(&format, sizeof(char), 2048, fp);//根目录的子目录、子文件索引的初始化 

		fwrite(&format, sizeof(char), 262126 * 4 * 1024 - 2113, fp);//内容区的初始化 
	}
	else {
		cout << "输入参数错误" << endl;
	}
}

void DiskMannger::make() {
	string FName;
	cin >> FName;
	cout << "创建" << FName << "文件" <<endl;
}

void DiskMannger::open() {
	string FName;
	cin >> FName;
	cout << "打开" << FName << "文件" << endl;
}

void DiskMannger::exit() {//更新两个bitmap 对应可用块、可回收块数，然后退出系统
	//指向第二个块，然后写入freebitmap
	fseek(root_fp, 4096, SEEK_SET);
	char t = 0;
	for (int i = 0; i < 32768; i++)
		fwrite(&t, sizeof(char), 1, root_fp);
	int count = fat.freeblock.size();
	for (int i = 0; i < count; i++)
	{
		int x = fat.freeblock.top();
		fat.freeblock.pop();
		int byte = (x - 1) / 8;
		int bit = (x - 1) % 8;
		fseek(root_fp, 4096 + byte, SEEK_SET);
		char read;
		fread(&read, sizeof(char), 1, root_fp);
		read |= 1 << (7 - bit);
		fseek(root_fp, 4096 + byte, SEEK_SET);
		fwrite(&read, sizeof(char), 1, root_fp);
	}
	//然后写入recbitmap
	fseek(root_fp, 4096 * 9, SEEK_SET);
	for (int i = 0; i < 32768; i++)
	{
		char t = fat.recmap[i];
		fwrite(&t, sizeof(char), 1, root_fp);
	}
	cout << "退出文件系统" << endl;
}

void DiskMannger::write() {
	string text;
	cin >> text;
	cout << "写入\n" << text << endl;
}



void DiskMannger::read() {
	cout << "读出文件" << endl;
}

void DiskMannger::cp() {
	string source, target;
	cin >> source >> target;
	cout << source << "->>" << target;
}

void DiskMannger::rename() {
	string source, target;
	cin >> source >> target;
	cout << source << "->>" << target;
}

void DiskMannger::dr() {
	char judge;
	judge = getchar();
	//cout << judge << endl;
	if (judge == '\n') {
		cout << "列出文件目录" << endl;
	}
	else if (judge == ' ') {
		string flag;
		cin >> flag;
		if (flag == "-l") {
			cout << "列出文件详细目录" << endl;
		}
		else {
			cout << "输入指令错误，请重新输入！！" << endl;
		}
	}
}

void DiskMannger::tp(int curBlockNum) {
	cout << "显示空间中的文本文件，只显示文件" << endl;

	fseek(root_fp, curBlockNum * BLOCK_SIZE, SEEK_SET);	//定位到这个File类型的块
	fseek(root_fp, 8, SEEK_CUR);
	int nextNum;
	fread(&nextNum, sizeof(int), 1, root_fp);			//取到File块内第一个数据块的编号
	while (nextNum != 0) {
		fseek(root_fp, nextNum - curBlockNum + 8, SEEK_CUR);//定位到当前数据块，顺便取到下一个数据块的编号
		curBlockNum = nextNum;
		fread(&nextNum, sizeof(int), 1, root_fp);
		char outStr;
		for (int i = 0; i < 4084; i++) {
			fread(&outStr, sizeof(char), 1, root_fp);
			if (outStr == '\0') {
				break;
			}
			putchar(outStr);
		}
	}
}


void DiskMannger::dl() {
	string FName;
	cin >> FName;
	cout << "删除名为" << FName << "的文件或者文件夹" << endl;
}

void DiskMannger::cd() {
	string address;
	cin >> address;
	cout << "进入" << address << "的文件夹" << endl;
}

void DiskMannger::att() {
	string FName;
	cin >> FName;
	cout << "显示名为" << FName << "的文件或者文件夹" << endl;
}
void DiskMannger::more() {
	string FName;
	cin >> FName;
	cout << "分页显示名为" << FName << "的文件或者文件夹" << endl;
}

void DiskMannger::back() {
	cout << "回到上级目录下" << endl;
}

void DiskMannger::recover() {
	string FName;
	cin >> FName;
	cout << "恢复名为" << FName << "的文件或者文件夹" << endl;
}

void DiskMannger::clear() {
	cout << "清空回收站" << endl;
}

void DiskMannger::rm() {
	string FName;
	cin >> FName;
	cout << "彻底删除名为" << FName << "的文件或者文件夹" << endl;	
}