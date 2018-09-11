#define _CRT_SECURE_NO_WARNINGS
#include "DiskMannger.h"
#include<cstring>
#include<cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <string>
#include <iostream>
#include<queue>
#include<io.h>
#include <time.h>
#include"FAT.h"
#include"Folder.h"
#include"File.h"
//new

using namespace std;



const static int ALL_DATA_BLOCK_NUM = 262127;
FAT fat = FAT();
FILE * root_fp;
FCB * root_folder;//指向内存中根目录地址
FCB * now_pos;//指向当前内存的位置


DiskMannger::DiskMannger() {
	cout << "欢迎！！-----------您可输入help获得帮助------------" << endl;
	string cmd1;
	while (cin >> cmd1) {
		if (cmd1 == "help") {
			cout << "帮助文档" << endl;
			cout << "\n●fmt:\n" <<
				"●break:\n" <<
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
				"●? :\n" << endl;
		}
		else if (cmd1 == "create") {
			this->create();
		}
		else if (cmd1 == "mount") {
			bool flag = this->mount();
			if (!flag)
				continue;
			string cmd;
			while (cin >> cmd) {
				if (cmd == "help") {
					cout << "帮助文档" << endl;
					cout << "\n●fmt:\n" <<
						"●close:\n" <<
						"●make:\n" <<
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
						"●? :\n" << endl;
				}
				else if (cmd == "fmt") {
					this->fmt(root_fp);/////////////这里该传个啥？？启动块指针即root_fp呀？？
				}
				else if (cmd == "close") {
					this->close();
					exit(-1);
				}
				else if (cmd == "make") {
					this->make();
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
				else if (cmd == "dl") {
					this->dl();
				}
				else if (cmd == "cd") {
					this->cd();
				}
				else if (cmd == "att") {
					this->att();
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
				else if (cmd == "map") {
					this->map();
				}
				else if (cmd == "rm") {
					this->rm();
				}
				else {
					cout << "输入指令错误，请重新输入！！" << endl;
				}
			}
		}
		else if (cmd1 == "close") {
			exit(-1);
		}
		else {
			cout << "输入指令错误，请重新输入！！!" << endl;
		}
	}
}
DiskMannger::~DiskMannger() {}

void DiskMannger::create() {
	char SName[9];
	memset(SName, 0, sizeof(SName));
	cin >> SName;
	SName[8] = '\0';
	cout << "创建" << SName << "空间" << endl;
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
			return;
		}
	}
	FILE *fp = fopen(SName, "w+"); //读取这个文件系统
	fseek(fp, 0, SEEK_SET);//回到了开始的位置
	printf("初次建立文件系统，请稍等\n");

	int num = 8;
	int usenum = 262126;
	int binnum = 0;
	fseek(fp, 32768 * 32768 - 1, SEEK_SET); // 将文件的指针 移至 指定大小的位置 32768*32768
	fputc('a', fp); // 在要指定大小文件的末尾随便放一个数据
	fseek(fp, 0, SEEK_SET);//回到了开始的位置
	fwrite(magic, sizeof(char), 8, fp);
	fwrite(&BLOCK_SIZE, sizeof(int), 1, fp);
	fwrite(&num, sizeof(int), 1, fp);
	fwrite(&BLOCK_NUM, sizeof(int), 1, fp);
	fwrite(&usenum, sizeof(int), 1, fp);
	fwrite(&binnum, sizeof(int), 1, fp);
	num = 0;
	unsigned char t = 255;
	fseek(fp, 0, SEEK_SET);
	fseek(fp, 1024 * 4, SEEK_CUR);//跳过启动块
								  //freebitmap 置1 表示所有块都可用
	for (int i = 0; i < 32768; i++)
		fwrite(&t, sizeof(unsigned char), 1, fp);
	//recbitmap置0 表示没有块是可回收块
	t = 0;
	for (int i = 0; i < 32768; i++)
		fwrite(&t, sizeof(unsigned char), 1, fp);
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
	for (int i = 0; i < 512; i++)
		fwrite(&tmp, sizeof(int), 1, fp);

	//test begin
	fseek(fp, 0, SEEK_CUR);
	//fread(&tmp, sizeof(int), 1, fp);
	//cout << tmp;

	//test finish


	printf("ok\n");
}

void DiskMannger::close() {
	fseek(root_fp, 4096, SEEK_SET);
	unsigned char t = 0;
	for (int i = 0; i < 32768; i++)
		fwrite(&t, sizeof(unsigned char), 1, root_fp);
	int count = fat.freeblock.size();
	for (int i = 0; i < count; i++)
	{
		int x = fat.freeblock.top();
		fat.freeblock.pop();
		//byte做角标故-1
		int byte = (x - 1) / 8;
		int bit = (x - 1) % 8;
		fseek(root_fp, 4096 + byte, SEEK_SET);
		unsigned char read;
		fread(&read, sizeof(unsigned char), 1, root_fp);
		read |= 1 << (7 - bit);
		fseek(root_fp, 4096 + byte, SEEK_SET);
		fwrite(&read, sizeof(unsigned char), 1, root_fp);
	}
	fseek(root_fp, 0, SEEK_CUR);//保证数据写入
	//然后写入recbitmap
	fseek(root_fp, 4096 * 9, SEEK_SET);
	for (int i = 0; i < 32768; i++)
	{
		unsigned char t = fat.recmap[i];
		fwrite(&t, sizeof(unsigned char), 1, root_fp);
	}

	fseek(root_fp, 0, SEEK_CUR);//保证数据写入
	cout << "退出文件系统" << endl;
}

void DiskMannger::search(int cur_num, FCB * node_now, FCB * fa)
{
	cur_num = cur_num - 1;
	fseek(root_fp, 0, SEEK_SET);
	int tmpnode;
	fseek(root_fp, cur_num * 4 * 1024, SEEK_CUR);
	fread(&tmpnode, sizeof(int), 1, root_fp);
	int cur_type;
	fread(&cur_type, sizeof(int), 1, root_fp);
	if (cur_type == 1)//文件
	{
		File * file_now = (File*)node_now;//当前文件结点强行转化
		file_now->Ftype = 1;
		file_now->nodeId = tmpnode;
		fread(&file_now->size, sizeof(int), 1, root_fp);
		fread(&file_now->access, sizeof(int), 1, root_fp);
		fread(&file_now->father, sizeof(int), 1, root_fp);
		fread(&file_now->name, sizeof(char), 9, root_fp);
		fread(&file_now->create_time, sizeof(short), 6, root_fp);
		fread(&file_now->last_time, sizeof(short), 6, root_fp);
		fread(&file_now->type, sizeof(char), 4, root_fp);
		fread(&file_now->flag, sizeof(int), 1, root_fp);
		//???他的父亲r好目前不支持返回上一层。。。
		file_now->fa_node = fa;
	}
	else {
		//文件夹
		Folder * folder_now = (Folder*)node_now;//当前文件夹结点强行转化
		folder_now->Ftype = 2;
		folder_now->nodeId = tmpnode;
		fread(&folder_now->size, sizeof(int), 1, root_fp);
		fread(&folder_now->access, sizeof(int), 1, root_fp);
		fread(&folder_now->father, sizeof(int), 1, root_fp);
		fread(&folder_now->name, sizeof(char), 9, root_fp);
		fread(&folder_now->create_time, sizeof(short), 6, root_fp);
		fread(&folder_now->last_time, sizeof(short), 6, root_fp);
		fread(&folder_now->childsize, sizeof(int), 1, root_fp);
		for (int i = 1; i <= folder_now->childsize; i++)
		{
			int tmp;
			fread(&tmp, sizeof(int), 1, root_fp);
			folder_now->child.push_back(tmp);
			FCB fchild = FCB();
			folder_now->child_node.push_back(&fchild);
			this->search(tmp, &fchild, folder_now);//继续查找儿子的索引文件

		}
		folder_now->fa_node = fa;
	}
	return;
}



bool DiskMannger::mount()
{//挂载到已经有的文件系统
	char SName[9];
	memset(SName, 0, sizeof(SName));
	cin >> SName;
	SName[8] = '\0';
	char magic[8] = { 'm','i','n','i','f','s','s','s' };
	if (_access(SName, 0) == 0)//已经存在该系统
	{
		FILE *fp = fopen(SName, "r+");
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
			unsigned char now;
			fread(&now, sizeof(unsigned char), 1, fp);
			for (int i = 0; i < 6; i++)//特判19~24块
				if (((now >> i) & 1) == 1)
				{
					count_free++;
					fat.freeblock.push(24 - i);
				}
			for (int i = 3; i < 32768; i++)//从25判断
			{
				fread(&now, sizeof(unsigned char), 1, fp);
				for (int j = 0; j < 8; j++)
					if (((now >> j) & 1) == 1)
					{
						count_free++;
						fat.freeblock.push((i + 1) * 8 - j);
					}
			}
			fseek(fp, 2, SEEK_CUR);//可回收bitmap， 从第三字节计算 1~18块占用
			fread(&now, sizeof(unsigned char), 1, fp);
			fat.recmap[2] = now;
			for (int i = 0; i < 6; i++) //特判19~24块
				if (((now >> i) & 1) == 1)
					count_rec++;
			for (int i = 3; i < 32768; i++)//从25判断
			{
				fread(&now, sizeof(unsigned char), 1, fp);
				fat.recmap[i] = now;
				for (int j = 1; j <= 8; j++)
					if ((now >> (j - 1) & 1) == 1)
						count_rec++;
			}


			printf("当前文件系统中共有%d空块可用,共%lld字节\n", count_free, (long long)count_free * 4 * 1024);
			printf("当前文件系统中共有%d可回收块可用,共%lld字节\n", count_rec, (long long)count_rec * 4 * 1024);

			FCB * gen;
			gen = new FCB();
			root_folder = gen;
			DiskMannger::search(18, root_folder, root_folder);
			
			return true;
		}
		else
		{
			printf("该系统不存在且存在同名文件！\n");
			printf("请使用create命令创建其他名称的文件系统！\n");
			return false;
		}
	}
	else
	{
		printf("该系统不存在！\n");
		printf("请使用create命令先进行创建！\n");
		return false;
	}
}

void DiskMannger::fmt(FILE *fp) {
	string flag;
	cin >> flag;
	if (flag == "-1") {
		cout << "简单初始化，仅删除目录文件，磁盘数据不进行重写" << endl;
		char format = '\0';
		fseek(fp, 22, SEEK_SET);

		fwrite(&ALL_DATA_BLOCK_NUM, sizeof(int), 1, fp);	//将可用空间置为满状态：262127 
		for (int i = 0; i < 17 * 4 * 1024 - 26 + 16; i++) {//26为之前指针经过的字节数 ，并进行freebitmap，recbitmap的格式化  
			fwrite(&format, sizeof(char), 1, fp);
		}
		fseek(fp, 49, SEEK_CUR);	//根目录的文件夹名称，创建时间不做修改，最后修改时间暂不更改。
		fwrite(&format, sizeof(char), 2048, fp);//根目录的子目录、子文件索引的初始化
		cout << "简单格式化完成" << endl;
		return;
	}
	else if (flag == "-2") {
		cout << "普通初始化将清除所有数据文件，且耗时较长，是否继续？（yes / no）" << endl;
		while (1) {
			string input;
			cin >> input;
			if (input == "no") {
				cout << "格式化被取消" << endl;
				return;
			}
			else if (input == "yes") {
				cout << "正在格式化..." << endl;
				char format = 0;
				fseek(fp, 22, SEEK_SET);

				fwrite(&ALL_DATA_BLOCK_NUM, sizeof(int), 1, fp);	//将可用空间置为满状态：262127
				for (int i = 0; i < 17 * 4 * 1024 - 26; i++) {//26为之前指针经过的字节数 ，并进行freebitmap，recbitmap的格式化
					fwrite(&format, sizeof(char), 1, fp);
				}
				fwrite(&format, sizeof(char), 16, fp);	//根目录的部分格式化
				fseek(fp, 49, SEEK_CUR);	//根目录的文件夹名称，创建时间不做修改，最后修改时间暂不更改。
				fwrite(&format, sizeof(char), 2048, fp);//根目录的子目录、子文件索引的初始化

				for (int i = 0; i < 262126 * 4 * 1024 - 2113; i++) {	//内容区的初始化
					fwrite(&format, sizeof(char), 1, fp);
				}
				cout << "普通格式化完成" << endl;
				return;
			}
			else {
				cout << "输入错误" << endl;
				continue;
			}
		}
	}
	else {
		cout << "输入参数错误" << endl;
		return;
	}
}

void DiskMannger::map() {
	string FName;
	cin >> FName;
	string fileName, extName;
	fileName = FName.substr(0, FName.find_first_of('.'));
	extName = FName.substr(FName.find_first_of('.') + 1);
	int pos = now_pos->nodeId;	//当前目录(Folder)所在块的编号
	//fseek(root_fp, pos + 65, SEEK_SET);
	int num = 0;
	bool isFound = false;
	for (int i = 0; i < 512; i++) {	//寻找对应的FName所在的块的编号: num
		fseek(root_fp, (pos - 1) * BLOCK_SIZE + 65 + 4 * i, SEEK_SET);	//Folder类的一个block前65字节用于存储除child信息的其他信息
		fread(&num, sizeof(int), 1, root_fp);
		if (num != 0) {
			fseek(root_fp, (num - 1) * BLOCK_SIZE + 20, SEEK_SET);
			char fileName_t[9] = { 0 };
			char extName_t[4] = { 0 };
			fread(extName_t, sizeof(char), 4, root_fp);
			fread(fileName_t, sizeof(char), 9, root_fp);
			if (strcmp(fileName.c_str(), fileName_t) == 0 && strcmp(extName.c_str(), extName_t) == 0) {//若文件名和扩展名都匹配
				isFound = true;
				break;
			}
		}
	}
	if (!isFound) {
		cout << "该目录下文件不存在" << endl;
		return;
	}
	//找到文件，此时的num保存的是文件(File)的编号，并不是数据块(Block)的编号
	cout << "此文件数据块的编号: " << endl;
	while (1) {
		fseek(root_fp, (num - 1) * BLOCK_SIZE, SEEK_SET);
		fread(&num, sizeof(int), 1, root_fp);
		cout << num << ' ';
		if (num == 0) {
			break;
		}
	}
	cout << endl;
	return;
}

void DiskMannger::make() {
	int type;
	getchar();
	cin >> type;
	string FName;
	getchar();
	cin >> FName;
	bool find = false;     //查找是否存在同名文件夹
	char cur_name[9] = { 0 };
	strcpy(cur_name, FName.c_str());
	Folder *now = (Folder *)root_folder;    //需要更改为now_pos----------------------------------------------
	for (int j = 0; j < 0/*now->childsize*/; j++)
	{
		fseek(root_fp, (now->child[j] - 1) * BLOCK_SIZE + 4, SEEK_SET);   //找类型
		int tar_type;
		fread(&tar_type, sizeof(int), 1, root_fp);

		fseek(root_fp, (now->child[j] - 1) * BLOCK_SIZE + 8, SEEK_SET);   //找名字
		char tar_name[9];
		fread(tar_name, sizeof(char), 9, root_fp);
		if (strcmp(cur_name, tar_name) == 0 && type == tar_type)
		{
			find = true;
			break;
		}
	}
	if (find == false)   //不存在同名文件
	{
		int nodeId = fat.getBlock(); //获取空闲块Id
		if (nodeId == -1)
		{
			cout << "空间已满，无法进行创建" << endl;
		}
		if (type == 1)
		{
			//FILE *tar_pos = NULL;
			fseek(root_fp, (nodeId - 1)*BLOCK_SIZE, SEEK_SET);
			fwrite(&nodeId, sizeof(int), 1, root_fp);
			fwrite(&type, sizeof(int), 1, root_fp);
			time_t timep;
			struct tm *p;
			time(&timep);
			p = gmtime(&timep);
			string time;
			time = (1900 + p->tm_year) + (1 + p->tm_mon) + (p->tm_mday) + " " + (p->tm_hour) + ':' + (p->tm_min) + ':' + p->tm_sec;
			cout << time << endl;
			char t[20] = { '\0' };
			strcpy(t, time.c_str());
			fwrite(t, sizeof(char), sizeof(t), root_fp);
			fwrite(t, sizeof(char), sizeof(t), root_fp);
			char file_type[4] = { 't','r','z','\0' };
			fwrite(file_type, sizeof(char), sizeof(file_type), root_fp);
			int ini = 0;
			fwrite(&ini, sizeof(int), 1, root_fp);//文件大小
			fwrite(&ini, sizeof(int), 1, root_fp);//保护方式
			int fa_id = root_folder->nodeId/*now_pos->nodeId*/;    //差东西-----------------------------
			fwrite(&fa_id, sizeof(int), 1, root_fp);//父节点id
			fwrite(&ini, sizeof(int), 1, root_fp);//指向的第一个块
			cout << "创建" + FName + "文件成功" << endl;
		}
		else if (type == 2)
		{
			//FILE *tar_pos = NULL;
			fseek(root_fp, (nodeId - 1)*BLOCK_SIZE, SEEK_SET);
			fwrite(&nodeId, sizeof(int), 1, root_fp);  //nodeId
			fwrite(&type, sizeof(int), 1, root_fp);    //type
			time_t timep;
			struct tm *p;
			time(&timep);
			p = gmtime(&timep);
			string time;
			time = (1900 + p->tm_year) + (1 + p->tm_mon) + (p->tm_mday) + " " + (p->tm_hour) + ':' + (p->tm_min) + ':' + p->tm_sec;
			cout << time << endl;
			char t[20] = { '\0' };
			strcpy(t, time.c_str());
			fwrite(t, sizeof(char), sizeof(t), root_fp); //创建时间
			fwrite(t, sizeof(char), sizeof(t), root_fp); //修改时间
														 //char file_type[4] = { 't','r','z','\0' };
														 //fwrite(file_type, sizeof(char), sizeof(file_type), tar_pos);
			int ini = 0;
			fwrite(&ini, sizeof(int), 1, root_fp);//文件大小
			fwrite(&ini, sizeof(int), 1, root_fp);//保护方式
			int fa_id = root_folder->nodeId/*now_pos->nodeId*/;    //差东西------------------------------
			fwrite(&fa_id, sizeof(int), 1, root_fp);//父节点id
			fwrite(&ini, sizeof(int), 1, root_fp);//  childsize
			cout << "创建" + FName + "文件夹成功" << endl;
		}
		else
		{
			cout << "创建错误" << endl;
		}


	}
	cout << "创建" << FName << "文件" << endl;
}

bool DiskMannger::make(char *FName) {
	bool find = false;     //查找是否存在同名文件夹
	int type = 1;
	Folder *now = (Folder *)root_folder;   //更改为now_pos--------------------------
	for (int j = 0; j < 0/*now->childsize*/; j++)
	{
		FILE *now_name = NULL;
		FILE *now_type = NULL;
		fseek(now_type, now->child[j] * BLOCK_SIZE + 4, SEEK_SET);
		fseek(now_name, now->child[j] * BLOCK_SIZE + 20, SEEK_SET);
		char tar_name[9];
		int tar_type;
		fread(tar_name, 1, 9, (FILE *)now);
		fread(&tar_type, sizeof(int), 1, now_type);
		if (strcmp(FName, tar_name) == 0 && type == tar_type)
		{
			find = true;
			break;
		}
	}
	if (find == false)   //不存在同名文件
	{
		int nodeId = fat.getBlock(); //获取空闲块Id
		if (nodeId == -1)
		{
			cout << "空间已满，无法进行复制" << endl;
			return false;
		}
		//FILE *tar_pos = NULL;
		fseek(root_fp, (nodeId - 1)*BLOCK_SIZE, SEEK_SET);
		fwrite(&nodeId, sizeof(int), 1, root_fp);
		fwrite(&type, sizeof(int), 1, root_fp);
		time_t timep;
		struct tm *p;
		time(&timep);
		p = gmtime(&timep);
		string time;
		time = (1900 + p->tm_year) + (1 + p->tm_mon) + (p->tm_mday) + " " + (p->tm_hour) + ':' + (p->tm_min) + ':' + p->tm_sec;
		cout << time << endl;
		char t[20] = { '\0' };
		strcpy(t, time.c_str());
		fwrite(t, sizeof(char), sizeof(t), root_fp);
		fwrite(t, sizeof(char), sizeof(t), root_fp);
		char file_type[4] = { 't','r','z','\0' };
		fwrite(file_type, sizeof(char), sizeof(file_type), root_fp);
		int ini = 0;
		fwrite(&ini, sizeof(int), 1, root_fp);//文件大小
		fwrite(&ini, sizeof(int), 1, root_fp);//保护方式
		int fa_id = root_folder->nodeId/*now_pos->nodeId*/;    //差东西-----------------------------
		fwrite(&fa_id, sizeof(int), 1, root_fp);//父节点id
		fwrite(&ini, sizeof(int), 1, root_fp);//指向的第一个块
		return true;
	}
	return false;
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
	char sou[20], tar[20];
	strcpy(sou, source.c_str());
	strcpy(tar, target.c_str());
	FILE *fp_s = fopen(sou, "r+");
	int size = 1;
	make(tar);
	File *f = (File*)now_pos;
	f->data = fat.getBlock();
	int d = f->nodeId;
	fseek(root_fp, (d - 1)*BLOCK_SIZE + 12, 0);
	while (!feof(fp_s)) {
		if (size == 4085) {
			size = 1;
			int n = fat.getBlock();
			fseek(root_fp, (d - 1)*BLOCK_SIZE + 8, 0);
			fwrite(&n, sizeof(int), 1, root_fp);
			d = n;
			fseek(root_fp, (d - 1)*BLOCK_SIZE + 12, 0);
		}
		char temp;
		temp = fgetc(fp_s);
		if (temp == '\n')
			size++;
		fwrite(&temp, sizeof(char), 1, root_fp);
		size++;
	}
	fseek(root_fp, 0, 1);
}

void DiskMannger::rename() {
	string source, target;
	cin >> source >> target;
	cout << source << "->>" << target;
}


void dfs_file(int depth, FCB* now, int dfstype)//dfstype为1是列出普通目录，2是详细目录
{

	Folder *now_folder = (Folder*)now;
	if (now->Ftype == 1)//文件
	{
		File *now_file = (File *)now;
		if (dfstype == 2)
		{
			for (int i = 0; i < depth; i++)
				cout << "             ";
			cout << now_file->name << '.';
			cout << now_file->type << endl;
		}
		else
		{
			for (int i = 0; i < depth; i++)
				cout << "         ";
			cout << now_file->name << endl;
		}
	}
	else if (now->Ftype == 2)//文件夹
	{
		Folder *now_folder = (Folder*)now;
		if (dfstype == 1)
			for (int i = 0; i < depth; i++)
				cout << "         ";
		else
			for (int i = 0; i < depth; i++)
				cout << "             ";
		cout << now_folder->name << endl;
		for (int i = 0; i < now_folder->childsize; i++)
			dfs_file(depth + 1, now_folder, dfstype);
	}
}

void DiskMannger::dr() {
	char judge;
	judge = getchar();
	Folder *now_folder = (Folder*)root_folder;
	//cout << judge << endl;
	if (judge == '\n') {
		cout << "列出文件目录" << endl;
		dfs_file(0, root_folder, 1);

	}
	else if (judge == ' ') {
		string flag;
		cin >> flag;
		if (flag == "-l") {
			cout << "列出文件详细目录" << endl;
			dfs_file(0, root_folder, 2);
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
		fseek(root_fp, (nextNum - curBlockNum - 1)*BLOCK_SIZE + 8, SEEK_CUR);//定位到当前数据块，顺便取到下一个数据块的编号
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
	getchar();
	string address;
	cin >> address;
	Folder  *now = (Folder *)now_pos;  //当前目录
	bool success = true; //是否存在当前路径
	string cur_add;  //目标路径分割
	for (int i = 0; i < address.length(); i++)    //分割字符串并在各级目录中寻找是否存在目标目录
	{
		if (address[i] == '/' || i == address.length() - 1)
		{
			bool find = false;
			char cur_name[100];
			strcpy(cur_name, cur_add.c_str());
			for (int j = 0; j < now->child.size(); j++)
			{
				fseek(root_fp, (now->child[j] - 1) * BLOCK_SIZE + 8, SEEK_SET);
				char tar_name[9];
				fread(tar_name, sizeof(char), 9, root_fp);
				if (strcmp(cur_name, tar_name) == 0)
				{
					fseek(root_fp, (now->child[j] - 1) * BLOCK_NUM + 4, SEEK_SET);
					int type;
					fread(&type, sizeof(int), 1, root_fp);
					if (type == 2)
					{
						find == false;
						continue;
					}
					fseek(root_fp, (now->child[j] - 1) * BLOCK_NUM, SEEK_SET);

					find = true;
					now = (Folder *)root_fp;
					break;
				}
			}
			if (find == false)
			{
				cout << "不存在当前路径,请重新输入！！" << endl;
				success = false;
				break;
			}
		}
		else
		{
			cur_add = cur_add + address[i];  //分割字符串
		}
	}
	if (success == true)
	{
		now_pos = (FCB *)now;
		cout << "进入" << address << "的文件夹" << endl;
	}
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
	cout << "清空回收站" << endl;;
}

void DiskMannger::rm() {
	string FName;
	cin >> FName;
	cout << "彻底删除名为" << FName << "的文件或者文件夹" << endl;
}
