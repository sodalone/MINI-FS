#define _CRT_SECURE_NO_WARNINGS
#include "DiskMannger.h"
#include<cstring>
#include<cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include<queue>
#include<io.h>
#include <time.h>
#include"FAT.h"
#include"Folder.h"
#include"File.h"
#include"windows.h"
#include<algorithm>

using namespace std;


FAT fat = FAT();
const static int ALL_DATA_BLOCK_NUM = 262127;
FILE * root_fp;
Folder * root_folder;//指向内存中根目录地址
FCB * now_pos;//指向当前内存的位置
string path;

int findBlockNum(char *name, char *type) {	//name[9], type[4]
	Folder* now = (Folder*)now_pos;
	int i = 0;
	bool isFound = false;
	int child_node_size = now->child_node.size();
	for (i = 0; i < child_node_size; i++) {
		if (now->child_node[i]->Ftype == 1) {	//如果是文件
			File* foundFile = (File*)now->child_node[i];
			if (strcmp(name, foundFile->name) == 0 && strcmp(type, foundFile->type) == 0) {
				return i;
			}
		}
	}
	if (i == now->child_node.size()) {
		return -1;
	}
}

void printFun(int n) {
	for (int i = 0; i < n; i++) {
		printf(".");
	}
	return;
}

int int_len(int n) {
	int ret = 0;
	do {
		n /= 10;
		ret++;
	} while (n != 0);
	return ret;
}

DiskMannger::DiskMannger() {

	cout << "                           _ooOoo_				" << endl;
	cout << "                          o8888888o					" << endl;
	cout << "                          88' . '88					" << endl;
	cout << "                          (| -_- |)					" << endl;
	cout << "                           O\\ = /O					" << endl;
	cout << "                       ____/`---'\\____				" << endl;
	cout << "                     .   ' \\\\| |// `.				" << endl;
	cout << "                      / \\\\||| : |||// \\				" << endl;
	cout << "                    / _||||| -:- |||||- \\			" << endl;
	cout << "                      | | \\\\\\ - /// | |				" << endl;
	cout << "                    | \_| ''\\---/'' | |				" << endl;
	cout << "                     \\ .-\\__ `-` ___/-. /			" << endl;
	cout << "                  ___`. .' /--.--\\ `. . __			" << endl;
	cout << "               ."" '< `.___\\_<|>_/___.' >'"".		" << endl;
	cout << "              | | : `- \\`.;`\\ _ /`;.`/ - ` : | |	" << endl;
	cout << "                \\ \\ `-. \\_ __\\ /__ _/ .-` / /		" << endl;
	cout << "        ======`-.____`-.___\\_____/___.-`____.-'=====" << endl;
	cout << "                           `=---='					" << endl;
	cout << "													" << endl;
	cout << "        ............................................" << endl;
	cout << "                 佛祖镇楼                  BUG辟易  " << endl;
	cout << "         佛曰:									  " << endl;
	cout << "                 写字楼里写字间，写字间里程序员；	" << endl;
	cout << "                 程序人员写程序，又拿程序换酒钱。	" << endl;
	cout << "                 酒醒只在网上坐，酒醉还来网下眠；	" << endl;
	cout << "                 酒醉酒醒日复日，网上网下年复年。	" << endl;
	cout << "                 但愿老死电脑间，不愿鞠躬老板前；	" << endl;
	cout << "                 奔驰宝马贵者趣，公交自行程序员。	" << endl;
	cout << "                 别人笑我忒疯癫，我笑自己命太贱；	" << endl;
	cout << "                 不见满街漂亮妹，哪个归得程序员？	" << endl;
	cout << "													" << endl;
	cout << "													" << endl;
	cout << "欢迎！！-----------您可输入help获得帮助------------" << endl;
	string cmd1;

	path.append("MINI - FS");
	cout << path << '>';

	while (cin >> cmd1) {//////////////
		if (cmd1 == "help") {
			cout << "帮助文档" << endl;
			cout << "\n" <<
				"●create Name.....................新建一个名为Name空间 \n" <<
				"●mount Name:.....................安装一个名为Name空间\n" <<
				"●fmt -1:.........................格式化当前空间(不清除磁盘数据)\n" <<
				"●fmt -2:.........................格式化当前空间(清除磁盘数据)\n" <<
				"●make Name:......................新建一个名为Name的文件\n" <<
				"●mkdir Name:.....................新建一个名为Name文件夹\n" <<
				"●read path: .....................读取指定名为Name的文件内容\n" <<
				"●cp source target: ..............拷贝指定source路径下的名为xx的文件到指定target路径(本空间内的文件)\n" <<
				"●cp_in source target: ...........拷贝指定source路径下的名为xx的文件到指定target路径(windows到本空间)\n" <<
				"●cp_out source target: ..........拷贝指定source路径下的名为xx的文件到指定target路径(本空间到windows)\n" <<
				"●dr : ...........................展示当前目录的结构\n" <<
				"●dr -l: .........................展示当前目录的结构(后加后缀)\n" <<
				"●dl path: .......................删除指定名为Name文件\n" <<
				"●att path: ......................显示名为Name的文件属性\n" <<
				"●rename source target:...........重命名指定source文件为target文件\n" <<
				"●cd path:........................转到目标路径（paht=..返回上级目录，path=~返回根目录）\n" <<
				"●close:..........................退出当前空间\n" <<
				"●exit:...........................退出本文件系统\n" <<
				"●map path:.......................显示指定文件在空间所占用的全部块的块号。\n" <<
				"●tree:...........................显示当前空间树形结构(自带排序功能)\n" <<
				"●tree -l:........................详细显示当前空间树形结构(自带排序功能)\n" <<
				"●cl:.............................清屏\n" <<
				"●* :.............................匹配0-n个字符\n" <<
				"●? :.............................匹配0-1个字符\n" << endl;

		}
		else if (cmd1 == "create") {
			this->create();
		}
		else if (cmd1 == "cl")
		{
			this->clear();
		}
		else if (cmd1 == "mount") {
			bool flag = this->mount();
			cout << path << '>';//打印当前路径
			if (!flag)
				continue;
			string cmd;
			while (cin >> cmd) {

				if (cmd == "help") {
					cout << "帮助文档" << endl;
					cout << "\n" <<
						"●create Name.....................新建一个名为Name空间 \n" <<
						"●mount Name:.....................安装一个名为Name空间\n" <<
						"●fmt -1:.........................格式化当前空间(不清除磁盘数据)\n" <<
						"●fmt -2:.........................格式化当前空间(清除磁盘数据)\n" <<
						"●make Name:......................新建一个名为Name的文件\n" <<
						"●mkdir Name:.....................新建一个名为Name文件夹\n" <<
						"●read path: .....................读取指定名为Name的文件内容\n" <<
						"●cp source target: ..............拷贝指定source路径下的名为xx的文件到指定target路径(本空间内的文件)\n" <<
						"●cp_in source target: ...........拷贝指定source路径下的名为xx的文件到指定target路径(windows到本空间)\n" <<
						"●cp_out source target: ..........拷贝指定source路径下的名为xx的文件到指定target路径(本空间到windows)\n" <<
						"●dr : ...........................展示当前目录的结构\n" <<
						"●dr -l: .........................展示当前目录的结构(后加后缀)\n" <<
						"●dl path: .......................删除指定名为Name文件\n" <<
						"●att path: ......................显示名为Name的文件属性\n" <<
						"●rename source target:...........重命名指定source文件为target文件\n" <<
						"●cd path:........................转到目标路径（paht=..返回上级目录，path=~返回根目录）\n" <<
						"●close:..........................退出当前空间\n" <<
						"●exit:...........................退出本文件系统\n" <<
						"●map name:.......................显示指定文件在空间所占用的全部块的块号。\n" <<
						"●tree:...........................显示当前空间树形结构(自带排序功能)\n" <<
						"●tree -l:........................详细显示当前空间树形结构(自带排序功能)\n" <<
						"●* :.............................匹配0-n个字符\n" <<
						"●? :.............................匹配0-1个字符\n" << endl;

				}
				else if (cmd == "cl")
				{
					this->clear();
				}
				else if (cmd == "fmt") {
					this->fmt();
				}
				else if (cmd == "close") {
					this->close();
					break;
				}
				else if (cmd == "make") {
					this->make();
				}
				else if (cmd == "mkdir") {
					this->mkdir();
				}
				else if (cmd == "cpin") {
					this->cp_in();
				}
				else if (cmd == "cpout") {
					this->cp_out();
				}
				else if (cmd == "cp") {
					this->cp();
				}
				else if (cmd == "read") {
					this->read();
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
				else if (cmd == "clear") {
					this->clear();
				}
				else if (cmd == "map") {
					this->map();
				}
				else if (cmd == "rm") {
					this->rm();
				}
				else if (cmd == "tree") {
					this->tree();
				}
				else {
					cout << "输入指令错误，请重新输入！！" << endl;
				}
				cout << path << '>';//打印当前路径
			}
		}
		else if (cmd1 == "exit") {
			exit(0);
		}
		else {
			cout << "输入指令错误，请重新输入！！!" << endl;
		}
		cout << path << '>';//打印当前路径
	}
}

DiskMannger::~DiskMannger() {}

void DiskMannger::clear() {
	system("cls");
}

void DiskMannger::create() {
	string tmname;
	cin >> tmname;
	if (tmname.size() > 8)
	{
		cout << "名称必须小于八个字符！" << endl;
		return;
	}
	char SName[9] = { 0 };
	strcpy(SName, tmname.c_str());
	cout << "创建" << SName << "空间" << endl;
	char magic[8] = { 'm','i','n','i','f','s','s','s' };
	char all[13] = { 0 };
	strcpy(all, SName);
	//strcat(all, ".fs");
	if (_access(all, 0) == 0)
	{
		//已经存在这个文件系统
		FILE* fp = fopen(all, "rb"); //读取这个文件系统
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
	FILE *fp = fopen(all, "wb+"); //读取这个文件系统
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

	fseek(fp, 0, SEEK_CUR);
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

	fseek(root_fp, 17 * 4096, SEEK_SET);
	int size = root_folder->size;

	short last_time[6];
	last_time[0] = root_folder->last_time[0];
	last_time[1] = root_folder->last_time[1];
	last_time[2] = root_folder->last_time[2];
	last_time[3] = root_folder->last_time[3];
	last_time[4] = root_folder->last_time[4];
	last_time[5] = root_folder->last_time[5];

	int childsize = root_folder->childsize;
	int tmp = 0;//占位
	fseek(root_fp, sizeof(int) * 2, SEEK_CUR);
	fwrite(&size, sizeof(int), 1, root_fp);
	fseek(root_fp, sizeof(int) * 2 + 9 + sizeof(short) * 6, SEEK_CUR);
	fwrite(last_time, sizeof(short), 6, root_fp);
	fwrite(&childsize, sizeof(int), 1, root_fp);
	for (int i = 0; i < childsize; i++)
		fwrite(&root_folder->child_node[i]->nodeId, sizeof(int), 1, root_fp);


	fseek(root_fp, 0, SEEK_CUR);//保证数据写入

	path.clear();
	path.append("MINI - FS\\..");

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
		fread(&file_now->data, sizeof(int), 1, root_fp);
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
		for (int i = 1; i <= folder_now->childsize; i++)//childsize是崩的！！！QAQ
		{
			int tmp;
			fread(&tmp, sizeof(int), 1, root_fp);
			folder_now->child.push_back(tmp);
		}
		for (int i = 0; i < folder_now->childsize; i++)
		{
			fseek(root_fp, (folder_now->child[i] - 1) * 4096 + sizeof(int), SEEK_SET);
			int ft;
			fread(&ft, sizeof(int), 1, root_fp);
			if (ft == 1)
			{
				File *fchild = new File();
				folder_now->child_node.push_back(fchild);
				this->search(folder_now->child[i], fchild, folder_now);//继续查找儿子的索引文件
			}
			else {
				Folder *fchild = new Folder();
				folder_now->child_node.push_back(fchild);
				this->search(folder_now->child[i], fchild, folder_now);//继续查找儿子的索引文件
			}
		}
		folder_now->fa_node = fa;
	}
	return;
}



bool DiskMannger::mount()
{//挂载到已经有的文件系统
	string tmname;
	cin >> tmname;
	if (tmname.size() > 8)
	{
		cout << "请输入正确名称" << endl;
		return false;
	}
	char SName[9] = { 0 };
	strcpy(SName, tmname.c_str());
	SName[8] = '\0';
	char all[13] = { 0 };
	strcpy(all, SName);
	//strcat(all, ".fs");
	char magic[8] = { 'm','i','n','i','f','s','s','s' };
	if (_access(all, 0) == 0)//已经存在该系统
	{
		FILE *fp = fopen(all, "rb+");
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


			root_folder = new Folder();
			DiskMannger::search(18, root_folder, root_folder);
			now_pos = root_folder;
			path.append("\\");
			path.append(SName);
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

void DiskMannger::fmt() {
	string flag;
	cin >> flag;
	if (flag == "-1") {
		cout << "简单初始化，仅删除目录文件，磁盘数据不进行重写" << endl;
		/*在磁盘中进行格式化*/
		char format = '\0';
		unsigned char avaliable = 0xFF;

		fseek(root_fp, 22, SEEK_SET);
		fwrite(&ALL_DATA_BLOCK_NUM, sizeof(int), 1, root_fp);	//将可用空间置为满状态：262127
		fwrite(&format, sizeof(char), 4, root_fp);				//垃圾箱使用块数置0
		fseek(root_fp, BLOCK_SIZE - 30, SEEK_CUR);
		for (int i = 0; i < 8 * BLOCK_SIZE; i++) {			//freebitmap置为1
			fwrite(&avaliable, sizeof(char), 1, root_fp);
		}
		for (int i = 0; i < 8 * BLOCK_SIZE; i++) {			//recbitmap置为0
			fwrite(&format, sizeof(char), 1, root_fp);
		}
		fseek(root_fp, 53, SEEK_CUR);	//根目录的文件夹名称，创建时间不做修改，最后修改时间暂不更改
		fwrite(&format, sizeof(char), 2048 + 4, root_fp);//根目录的子目录、子文件索引的初始化以及将子文件个数置为零
		fseek(root_fp, 0, SEEK_CUR);
		/*在内存中进行相应的更改*/
		fat.init();
		root_folder->childsize = 0;
		root_folder->child.clear();
		root_folder->size = 0;
		root_folder->child_node.clear();

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
				/*在磁盘中进行格式化*/
				cout << "正在格式化..." << endl;
				char format = '\0';
				unsigned char avaliable = 0xff;

				fseek(root_fp, 22, SEEK_SET);
				fwrite(&ALL_DATA_BLOCK_NUM, sizeof(int), 1, root_fp);	//将可用空间置为满状态：262127
				fwrite(&format, sizeof(char), 4, root_fp);				//垃圾箱使用块数置0
				fseek(root_fp, BLOCK_SIZE - 30, SEEK_CUR);
				for (int i = 0; i < 8 * BLOCK_SIZE; i++) {			//freebitmap置为1
					fwrite(&avaliable, sizeof(char), 1, root_fp);
				}
				for (int i = 0; i < 8 * BLOCK_SIZE; i++) {			//recbitmap置为0
					fwrite(&format, sizeof(char), 1, root_fp);
				}
				fseek(root_fp, 53, SEEK_CUR);	//根目录的文件夹名称，创建时间不做修改，最后修改时间暂不更改。


				fwrite(&format, sizeof(char), 2048 + 4, root_fp);//根目录的子目录、子文件索引的初始化以及将子文件个数置为零
				for (int i = 0; i <1321111; i++) {	//内容区的初始化
					fwrite(&format,1, 81, root_fp);
				}

				/*在内存中进行相应的更改*/
				fat.init();
				root_folder->childsize = 0;
				root_folder->child.empty();
				root_folder->size = 0;
				root_folder->child_node.empty();

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
	Folder * cp_now = (Folder*)now_pos;
	string FName;
	cin >> FName;
	while (1) {
		if (FName.find('/') == string::npos)
			break;
		else {
			int place = FName.find('/');
			string temp;
			temp = FName.substr(0, place);
			cp_now = DiskMannger::cd(temp, cp_now);
			if (cp_now == NULL) {
				cout << "目录不存在!" << endl;
				return;
			}
			FName = FName.substr(place + 1);
		}
	}
	if (FName.length() > 12) {
		cout << "文件名太长!" << endl;
		return;
	}
	char all[20] = { 0 }, name[9] = { 0 }, type[4] = { 0 };
	strcpy(all, FName.c_str());
	char *point, *p;
	point = strrchr(all, '.');
	if (point == NULL) {
		if (strlen(all) > 8) {
			cout << "文件名超过8位!" << endl;
			return;
		}
		strcpy(name, all);
	}
	else {
		if (strlen(point + 1) > 3) {
			cout << "文件后缀名超过3位!" << endl;
			return;
		}
		if (FName.length() - 1 - strlen(point + 1) > 8) {
			cout << "文件名超过8位!" << endl;
			return;
		}
		int j = 0;
		for (p = all, j = 0; p < point; p++) {
			name[j++] = *p;
		}
		for (p = point + 1, j = 0; *p != '\0'; p++) {
			type[j++] = *p;
		}
	}
	Folder *now = cp_now;
	int j;
	bool isFound = false;
	for (j = 0; j < now->child_node.size(); j++)
	{
		if (now->child_node[j]->Ftype == 1)
		{
			File *now_file = (File *)(now->child_node[j]);
			if (strcmp(name, now_file->name) == 0 && strcmp(type, now_file->type) == 0)
			{
				isFound = true;
				break;
			}
		}
	}
	if (!isFound) {
		cout << "该目录下文件不存在" << endl;
		return;
	}
	int child_id = now->child_node[j]->nodeId;
	fseek(root_fp, (child_id - 1)*BLOCK_SIZE + 20, 0);
	int block_id;
	fread(&block_id, sizeof(int), 1, root_fp);
	cout << "文件索引块的编号: " << child_id << endl;
	if (block_id == 0) {
		cout << "该文件为空！" << endl;
		return;
	}
	else {
		int count = 1;
		while (block_id != -1) {
			cout << "文件数据第" << count << "块：" << block_id << endl;
			fseek(root_fp, (block_id - 1)*BLOCK_SIZE + 8, 0);
			fread(&block_id, sizeof(int), 1, root_fp);
			count++;
		}
	}
	return;
}


void DiskMannger::make() {
	int type = 1;
	string FName;
	getchar();
	cin >> FName;
	if (FName.length() > 12) {
		cout << "文件名称太长!" << endl;
		return;
	}
	if (FName.find('/') != string::npos) {
		cout << "文件名称含非法字符’/'!" << endl;
		return;
	}
	bool find = false;     //查找是否存在同名文件夹
	char all[15] = { 0 }, cur_name[15] = { 0 }, file_type[4] = { 0 };
	strcpy(all, FName.c_str());
	char *point, *p;
	int i = 0;
	point = strrchr(all, '.');
	if (point == NULL) {
		if (strlen(all) > 8) {
			cout << "文件名超过8位!" << endl;
			return;
		}
		strcpy(cur_name, all);
	}
	else {
		if (strlen(point + 1) > 3) {
			cout << "文件后缀名超过3位!" << endl;
			return;
		}
		if (FName.length() - 1 - strlen(point + 1) > 8) {
			cout << "文件名超过8位!" << endl;
			return;
		}
		for (p = all, i = 0; p < point; p++) {
			cur_name[i++] = *p;
		}
		for (p = point + 1, i = 0; *p != '\0'; p++) {
			file_type[i++] = *p;
		}
	}

	Folder *now = (Folder *)now_pos;
	for (int j = 0; j < now->child_node.size(); j++)
	{
		if (now->child_node[j]->Ftype == 1)
		{
			File *now_file = (File *)(now->child_node[j]);
			if (strcmp(cur_name, now_file->name) == 0 && type == now_file->Ftype
				&& strcmp(file_type, now_file->type) == 0)
			{
				find = true;
				break;
			}
		}
	}
	if (find == false)   //不存在同名文件
	{
		int nodeId = fat.getBlock(); //获取空闲块Id
		if (nodeId == -1)
		{
			cout << "空间已满，无法进行创建" << endl;
		}
		fseek(root_fp, (nodeId - 1)*BLOCK_SIZE, SEEK_SET);
		fwrite(&nodeId, sizeof(int), 1, root_fp);  //nodeid
		fwrite(&type, sizeof(int), 1, root_fp);    //type
		int ini = 0;
		fwrite(&ini, sizeof(int), 1, root_fp);//文件大小 size
		fwrite(&ini, sizeof(int), 1, root_fp);//保护方式 access
		int fa_id = now_pos->nodeId;
		fwrite(&fa_id, sizeof(int), 1, root_fp);//父节点id
		fwrite(&ini, sizeof(int), 1, root_fp);//指向的第一个块
		fwrite(cur_name, sizeof(char), 9, root_fp);  //name
		time_t timep;
		struct tm *p;
		time(&timep);
		p = gmtime(&timep);
		short t[6];
		t[0] = (short)(p->tm_year + 1900);
		t[1] = (short)(p->tm_mon + 1);
		t[2] = (short)p->tm_mday;
		t[3] = (short)(p->tm_hour + 8);
		t[4] = (short)p->tm_min;
		t[5] = (short)p->tm_sec;
		fwrite(&t[0], sizeof(short), 1, root_fp);//create time
		fwrite(&t[1], sizeof(short), 1, root_fp);
		fwrite(&t[2], sizeof(short), 1, root_fp);
		fwrite(&t[3], sizeof(short), 1, root_fp);
		fwrite(&t[4], sizeof(short), 1, root_fp);
		fwrite(&t[5], sizeof(short), 1, root_fp);
		fwrite(&t[0], sizeof(short), 1, root_fp);  //last time
		fwrite(&t[1], sizeof(short), 1, root_fp);
		fwrite(&t[2], sizeof(short), 1, root_fp);
		fwrite(&t[3], sizeof(short), 1, root_fp);
		fwrite(&t[4], sizeof(short), 1, root_fp);
		fwrite(&t[5], sizeof(short), 1, root_fp);
		fwrite(file_type, sizeof(char), 4, root_fp);
		fwrite(&ini, sizeof(int), 1, root_fp);
		fseek(root_fp, 0, 1);

		//维护父节点的child
		fseek(root_fp, (now->nodeId - 1)*BLOCK_SIZE + 53, SEEK_SET);
		int size = now->childsize + 1;
		fwrite(&size, sizeof(int), 1, root_fp);
		fseek(root_fp, (now->nodeId - 1)*BLOCK_SIZE + 57 + now->childsize * 4, SEEK_SET);
		fwrite(&nodeId, sizeof(int), 1, root_fp);
		fseek(root_fp, 0, 1);

		//写入内存
		File *new_file = new File();
		new_file->nodeId = nodeId;
		new_file->Ftype = type;
		strcpy(new_file->name, cur_name);
		//create_time
		//last_time
		new_file->size = 0;//size
		new_file->access = 3;//access
		new_file->father = now->nodeId;//father
		new_file->flag = 0;//flag
						   //vector child
		new_file->fa_node = now;
		strcpy(new_file->type, file_type);
		//childnode
		now->child_node.push_back((FCB *)new_file);
		now->child.push_back(new_file->nodeId);
		now->childsize++;
		cout << "创建" + FName + "文件成功" << endl;
	}
	else
	{
		cout << "存在同名文件，请重新选择文件名" << endl;
		return;
	}
	return;
}
void DiskMannger::mkdir() {
	int type = 2;
	string FName;
	getchar();
	cin >> FName;
	if (FName.length() > 8) {
		cout << "文件名称太长!" << endl;
		return;
	}
	if (FName.find('/') != string::npos) {
		cout << "文件名称含非法字符’/'!" << endl;
		return;
	}
	bool find = false;     //查找是否存在同名文件夹
	char  cur_name[9] = { 0 };
	strcpy(cur_name, FName.c_str());
	Folder *now = (Folder *)now_pos;
	for (int j = 0; j < now->child_node.size(); j++)
	{
		if (now->child_node[j]->Ftype == 2)
		{
			Folder *now_folder = (Folder *)(now->child_node[j]);
			if (strcmp(cur_name, now_folder->name) == 0 && type == now_folder->Ftype)
			{
				find = true;
				break;
			}
		}
	}
	if (find == false)   //不存在同名文件
	{
		int nodeId = fat.getBlock(); //获取空闲块Id
		if (nodeId == -1)
		{
			cout << "空间已满，无法进行创建" << endl;
		}
		fseek(root_fp, (nodeId - 1)*BLOCK_SIZE, SEEK_SET);
		fwrite(&nodeId, sizeof(int), 1, root_fp);  //nodeId
		fwrite(&type, sizeof(int), 1, root_fp);    //type
		int ini = 0;
		fwrite(&ini, sizeof(int), 1, root_fp);//文件大小
		fwrite(&ini, sizeof(int), 1, root_fp);//保护方式
		int fa_id = now_pos->nodeId;//fa-id
		fwrite(&fa_id, sizeof(int), 1, root_fp);//父节点id
		for (int i = 0; i < 9; i++)
			fwrite(&cur_name[i], sizeof(char), 1, root_fp); //name
		time_t timep;
		struct tm *p;
		time(&timep);
		p = gmtime(&timep);
		short t[6];
		t[0] = (short)(p->tm_year + 1900);
		t[1] = (short)(p->tm_mon + 1);
		t[2] = (short)p->tm_mday;
		t[3] = (short)(p->tm_hour + 8);
		t[4] = (short)p->tm_min;
		t[5] = (short)p->tm_sec;
		fwrite(&t[0], sizeof(short), 1, root_fp);//create time
		fwrite(&t[1], sizeof(short), 1, root_fp);
		fwrite(&t[2], sizeof(short), 1, root_fp);
		fwrite(&t[3], sizeof(short), 1, root_fp);
		fwrite(&t[4], sizeof(short), 1, root_fp);
		fwrite(&t[5], sizeof(short), 1, root_fp);
		fwrite(&t[0], sizeof(short), 1, root_fp);  //last time
		fwrite(&t[1], sizeof(short), 1, root_fp);
		fwrite(&t[2], sizeof(short), 1, root_fp);
		fwrite(&t[3], sizeof(short), 1, root_fp);
		fwrite(&t[4], sizeof(short), 1, root_fp);
		fwrite(&t[5], sizeof(short), 1, root_fp);
		fwrite(&ini, sizeof(int), 1, root_fp);//  childsize
		fseek(root_fp, 0, 1);

		//维护父节点的child
		fseek(root_fp, (now->nodeId - 1)*BLOCK_SIZE + 53, SEEK_SET);
		int size = now->childsize + 1;
		fwrite(&size, sizeof(int), 1, root_fp);
		fseek(root_fp, (now->nodeId - 1)*BLOCK_SIZE + 57 + now->childsize * 4, SEEK_SET);
		fwrite(&nodeId, sizeof(int), 1, root_fp);
		fseek(root_fp, 0, 1);

		//写入内存
		Folder *new_folder = new Folder();
		new_folder->nodeId = nodeId;
		new_folder->Ftype = type;
		strcpy(new_folder->name, cur_name);
		//create_time
		//last_time
		new_folder->size = 0;//size
		new_folder->access = 3;//access
		new_folder->father = now->nodeId;//father
		new_folder->childsize = 0;//childsize
								  //vector child
		new_folder->fa_node = now;
		//childnode
		now->child_node.push_back((FCB *)new_folder);
		now->childsize++;
		now->child.push_back(new_folder->nodeId);
		cout << "创建" + FName + "文件夹成功" << endl;

	}
	else
	{
		cout << "存在同名文件夹，请重新选择文件夹名" << endl;
		return;
	}
	return;
}

File * DiskMannger::make(char *FName, Folder* cp_now) {
	if (strlen(FName) > 12) {
		cout << "文件名称太长!" << endl;
		return NULL;
	}
	if (strrchr(FName, '/') != NULL) {
		cout << "文件名称含非法字符’/'!" << endl;
		return NULL;
	}
	bool find = false;     //查找是否存在同名文件
	char all[15] = { 0 }, cur_name[15] = { 0 }, file_type[4] = { 0 };
	strcpy(all, FName);
	char *point, *p;
	int i = 0;
	point = strrchr(all, '.');
	if (point == NULL) {
		if (strlen(all) > 8) {
			cout << "文件名超过8位!" << endl;
			return NULL;
		}
		strcpy(cur_name, all);
	}
	else {
		if (strlen(point + 1) > 3) {
			cout << "文件后缀名超过3位!" << endl;
			return NULL;
		}
		if (strlen(all) - 1 - strlen(point + 1) > 8) {
			cout << "文件名超过8位!" << endl;
			return NULL;
		}
		for (p = all, i = 0; p < point; p++) {
			cur_name[i++] = *p;
		}
		for (p = point + 1, i = 0; *p != '\0'; p++) {
			file_type[i++] = *p;
		}
	}
	Folder *now = (Folder *)cp_now;
	for (int j = 0; j < now->child_node.size(); j++)
	{
		if (now->child_node[j]->Ftype == 1)
		{
			File *now_file = (File *)(now->child_node[j]);
			if (strcmp(cur_name, now_file->name) == 0 && strcmp(file_type, now_file->type) == 0)
			{
				find = true;
				break;
			}
		}

	}
	if (find == false)   //不存在同名文件
	{
		int nodeId = fat.getBlock(); //获取空闲块Id
		if (nodeId == -1)
		{
			cout << "空间已满，无法进行创建" << endl;
			return NULL;
		}
		int type = 1;
		fseek(root_fp, (nodeId - 1)*BLOCK_SIZE, SEEK_SET);
		fwrite(&nodeId, sizeof(int), 1, root_fp);  //nodeid
		fwrite(&type, sizeof(int), 1, root_fp);    //type
		int ini = 0;
		fwrite(&ini, sizeof(int), 1, root_fp);//文件大小 size
		fwrite(&ini, sizeof(int), 1, root_fp);//保护方式 access
		int fa_id = now_pos->nodeId;
		fwrite(&fa_id, sizeof(int), 1, root_fp);//父节点id
		fwrite(&ini, sizeof(int), 1, root_fp);//指向的第一个块
		fwrite(cur_name, sizeof(char), 9, root_fp);  //name
		time_t timep;
		struct tm *p;
		time(&timep);
		p = gmtime(&timep);
		short t[6];
		t[0] = (short)(p->tm_year + 1900);
		t[1] = (short)(p->tm_mon + 1);
		t[2] = (short)p->tm_mday;
		t[3] = (short)(p->tm_hour + 8);
		t[4] = (short)p->tm_min;
		t[5] = (short)p->tm_sec;
		fwrite(&t[0], sizeof(short), 1, root_fp);//create time
		fwrite(&t[1], sizeof(short), 1, root_fp);
		fwrite(&t[2], sizeof(short), 1, root_fp);
		fwrite(&t[3], sizeof(short), 1, root_fp);
		fwrite(&t[4], sizeof(short), 1, root_fp);
		fwrite(&t[5], sizeof(short), 1, root_fp);
		fwrite(&t[0], sizeof(short), 1, root_fp);  //last time
		fwrite(&t[1], sizeof(short), 1, root_fp);
		fwrite(&t[2], sizeof(short), 1, root_fp);
		fwrite(&t[3], sizeof(short), 1, root_fp);
		fwrite(&t[4], sizeof(short), 1, root_fp);
		fwrite(&t[5], sizeof(short), 1, root_fp);
		fwrite(file_type, sizeof(char), 4, root_fp);
		fseek(root_fp, 0, 1);

		//维护父节点的child
		fseek(root_fp, (now->nodeId - 1)*BLOCK_SIZE + 53, SEEK_SET);
		int size = now->childsize + 1;
		fwrite(&size, sizeof(int), 1, root_fp);
		fseek(root_fp, (now->nodeId - 1)*BLOCK_SIZE + 57 + now->childsize * 4, SEEK_SET);
		fwrite(&nodeId, sizeof(int), 1, root_fp);
		fseek(root_fp, 0, 1);

		//写入内存
		File *new_file = new File();
		new_file->nodeId = nodeId;
		new_file->Ftype = type;
		strcpy(new_file->name, cur_name);
		//create_time
		//last_time
		new_file->size = 0;//size
		new_file->access = 3;//access
		new_file->father = now->nodeId;//father
		new_file->flag = 0;//flag
							 //vector child
		new_file->fa_node = now;
		strcpy(new_file->type, file_type);
		//childnode
		now->child_node.push_back((FCB *)new_file);
		now->childsize++;
		now->child.push_back(new_file->nodeId);
		return new_file;
	}
	else {
		cout << "存在同名目标文件!" << endl;
		return NULL;
	}
}


void DiskMannger::cp() {
	string source, target;
	cin >> source >> target;
	Folder *sou_now = (Folder*)now_pos;
	while (1) {
		if (source.find('/') == string::npos)
			break;
		else {
			int place = source.find('/');
			string temp;
			temp = source.substr(0, place);
			sou_now = DiskMannger::cd(temp, sou_now);
			if (sou_now == NULL) {
				cout << "不存在源文件" << endl;
				return;
			}
			source = source.substr(place + 1);
		}
	}
	if (source.length() > 12) {
		cout << "源文件名称太长!" << endl;
		return;
	}
	bool find = false;     //查找是否存在同名文件夹
	char all_s[15] = { 0 }, name_s[9] = { 0 }, type_s[4] = { 0 };
	char all_t[15] = { 0 }, name_t[9] = { 0 }, type_t[4] = { 0 };
	strcpy(all_s, source.c_str());
	char *point, *p;
	int i = 0;
	point = strrchr(all_s, '.');
	if (point == NULL) {
		if (strlen(all_s) > 8) {
			cout << "文件名超过8位!" << endl;
			return;
		}
		strcpy(name_s, all_s);
	}
	else {
		if (strlen(point + 1) > 3) {
			cout << "源文件后缀名超过3位!" << endl;
			return;
		}
		if (source.length() - 1 - strlen(point + 1) > 8) {
			cout << "源文件名超过8位!" << endl;
			return;
		}
		for (p = all_s, i = 0; p < point; p++) {
			name_s[i++] = *p;
		}
		for (p = point + 1, i = 0; *p != '\0'; p++) {
			type_s[i++] = *p;
		}
	}
	int j;
	for (j = 0; j < sou_now->child_node.size(); j++)
	{
		if (sou_now->child_node[j]->Ftype == 1)
		{
			File *now_file = (File *)(sou_now->child_node[j]);
			if (strcmp(name_s, now_file->name) == 0 && strcmp(type_s, now_file->type) == 0)
			{
				break;
			}
		}
	}
	if (j == sou_now->child_node.size()) {
		cout << "源文件不存在！" << endl;
		return;
	}

	int id_s = sou_now->child_node[j]->nodeId;
	int data_s;
	fseek(root_fp, (id_s - 1)*BLOCK_SIZE + 20, 0);
	fread(&data_s, sizeof(int), 1, root_fp);
	if (data_s == 0) {
		cout << "源文件为空！" << endl;
		return;
	}

	Folder *tar_now = (Folder*)now_pos;
	while (1) {
		if (target.find('/') == string::npos)
			break;
		else {
			int place = target.find('/');
			string temp;
			temp = target.substr(0, place);
			tar_now = DiskMannger::cd(temp, tar_now);
			if (tar_now == NULL) {
				cout << "不存在源文件" << endl;
				return;
			}
			target = target.substr(place + 1);
		}
	}
	if (target.length() > 12) {
		cout << "目标文件名称太长!" << endl;
		return;
	}
	strcpy(all_t, target.c_str());
	point = strrchr(all_t, '.');
	if (point == NULL) {
		if (strlen(all_t) > 8) {
			cout << "文件名超过8位!" << endl;
			return;
		}
		strcpy(name_t, all_t);
	}
	else {
		if (strlen(point + 1) > 3) {
			cout << "源文件后缀名超过3位!" << endl;
			return;
		}
		if (target.length() - 1 - strlen(point + 1) > 8) {
			cout << "源文件名超过8位!" << endl;
			return;
		}
		for (p = all_t, i = 0; p < point; p++) {
			name_t[i++] = *p;
		}
		for (p = point + 1, i = 0; *p != '\0'; p++) {
			type_t[i++] = *p;
		}
	}

	File *now = make(all_t, tar_now);
	if (now == NULL) {
		return;
	}
	int id_t = now->nodeId;
	int data_t, flag = 0;

	fseek(root_fp, (data_s - 1)*BLOCK_SIZE + 8, 0);
	fread(&data_s, sizeof(int), 1, root_fp);
	char text[4100] = { 0 };
	int size = 0;
	while (true) {
		if (data_s == -1) {
			int i = 0;
			char temp;
			while (true) {
				fread(&temp, sizeof(char), 1, root_fp);
				text[i++] = temp;
				if (temp == '\0')
					break;
			}
			int n = fat.getBlock();
			if (n == -1) {
				cout << "文件复制失败,当前空间不足" << endl;
				return;
			}
			if (!flag) {
				now->data = n;
				flag = 1;
				fseek(root_fp, (id_t - 1)*BLOCK_SIZE + 20, 0);
				fwrite(&n, sizeof(int), 1, root_fp);
				fseek(root_fp, 0, 1);
				fseek(root_fp, (n - 1)*BLOCK_SIZE, 0);
				fwrite(&n, sizeof(int), 1, root_fp);
			}
			else {
				fseek(root_fp, (n - 1)*BLOCK_SIZE, 0);
				fwrite(&n, sizeof(int), 1, root_fp);
				fseek(root_fp, (data_t - 1)*BLOCK_SIZE + 8, 0);
				fwrite(&n, sizeof(int), 1, root_fp);
			}
			data_t = n;
			fseek(root_fp, (data_t - 1) * BLOCK_SIZE + 12, 0);
			fwrite(text, sizeof(char), i, root_fp);
			fseek(root_fp, 0, 1);
			int end = -1;
			fseek(root_fp, (data_t - 1)*BLOCK_SIZE + 8, 0);
			fwrite(&end, sizeof(int), 1, root_fp);

			fseek(root_fp, (id_t - 1)*BLOCK_SIZE + 8, 0);
			size += strlen(text);
			fwrite(&size, sizeof(int), 1, root_fp);
			now->size = size;
			fseek(root_fp, 0, 1);

			cout << "文件复制成功!" << endl;
			break;
		}
		else {
			fread(&text, sizeof(char), 4084, root_fp);
			int n = fat.getBlock();
			if (n == -1) {
				cout << "文件复制失败,当前空间不足" << endl;
				return;
			}
			if (!flag) {
				now->data = n;
				flag = 1;

				fseek(root_fp, (id_t - 1)*BLOCK_SIZE + 20, 0);
				fwrite(&n, sizeof(int), 1, root_fp);

				fseek(root_fp, 0, 1);
				fseek(root_fp, (n - 1)*BLOCK_SIZE, 0);
				fwrite(&n, sizeof(int), 1, root_fp);
			}
			else {
				fseek(root_fp, (n - 1)*BLOCK_SIZE, 0);
				fwrite(&n, sizeof(int), 1, root_fp);

				fseek(root_fp, (data_t - 1)*BLOCK_SIZE + 8, 0);
				fwrite(&n, sizeof(int), 1, root_fp);
			}
			data_t = n;
			fseek(root_fp, (data_t - 1)* BLOCK_SIZE + 12, 0);
			fwrite(text, sizeof(char), 4084, root_fp);
			size += 4084;
			fseek(root_fp, (data_s - 1)*BLOCK_SIZE + 8, 0);
			fread(&data_s, sizeof(int), 1, root_fp);
		}
	}
}

void DiskMannger::cp_out() {
	string source, target;
	cin >> source >> target;
	char tar[1000] = { 0 };
	Folder *cp_now = (Folder*)now_pos;
	while (1) {
		if (source.find('/') == string::npos)
			break;
		else {
			int place = source.find('/');
			string temp;
			temp = source.substr(0, place);
			cp_now = DiskMannger::cd(temp, cp_now);
			if (cp_now == NULL) {
				cout << "不存在源文件" << endl;
				return;
			}
			source = source.substr(place + 1);
		}
	}
	if (source.length() > 12) {
		cout << "不存在源文件" << endl;
		return;
	}
	char all[20] = { 0 }, name[9] = { 0 }, type[4] = { 0 };
	strcpy(all, source.c_str());
	char *point, *p;
	point = strrchr(all, '.');
	if (point == NULL) {
		if (strlen(all) > 8) {
			cout << "不存在源文件" << endl;
			return;
		}
		strcpy(name, all);
	}
	else {
		if (strlen(point + 1) > 3) {
			cout << "不存在源文件" << endl;
			return;
		}
		if (source.length() - 1 - strlen(point + 1) > 8) {
			cout << "不存在源文件" << endl;
			return;
		}
		int j = 0;
		for (p = all, j = 0; p < point; p++) {
			name[j++] = *p;
		}
		for (p = point + 1, j = 0; *p != '\0'; p++) {
			type[j++] = *p;
		}
	}
	int j;
	for (j = 0; j < cp_now->child_node.size(); j++)
	{
		if (cp_now->child_node[j]->Ftype == 1)
		{
			File *now_file = (File *)(cp_now->child_node[j]);
			if (strcmp(name, now_file->name) == 0 && strcmp(type, now_file->type) == 0)
				break;
		}
	}
	if (j == cp_now->child_node.size()) {
		cout << "不存在源文件" << endl;
		return;
	}
	strcpy(tar, target.c_str());
	FILE *fp_t = fopen(tar, "wb+");
	if (fp_t == NULL) {
		cout << "目标文件打不开！" << endl;
		return;
	}
	int id = cp_now->child_node[j]->nodeId;
	int block_id;
	fseek(root_fp, (id - 1)*BLOCK_SIZE + 20, 0);
	fread(&block_id, sizeof(int), 1, root_fp);
	if (block_id == 0) {
		cout << "源文件为空！" << endl;
		return;
	}
	fseek(root_fp, (block_id - 1)*BLOCK_SIZE + 8, 0);
	fread(&block_id, sizeof(int), 1, root_fp);
	while (true) {
		char temp;
		if (block_id == -1) {
			while (true) {
				fread(&temp, sizeof(char), 1, root_fp);
				if (temp == '\0')
					break;
				fwrite(&temp, sizeof(char), 1, fp_t);
				fseek(fp_t, 0, 1);
			}
			break;
		}
		else {
			for (int k = 0; k < 4084; k++) {
				fread(&temp, sizeof(char), 1, root_fp);
				fwrite(&temp, sizeof(char), 1, fp_t);
				fseek(fp_t, 0, 1);
			}
			fseek(root_fp, (block_id - 1)*BLOCK_SIZE + 8, 0);
			fread(&block_id, sizeof(int), 1, root_fp);
		}
	}
	cout << "文件复制成功!" << endl;
	fclose(fp_t);
}

void DiskMannger::cp_in() {
	string source, target;
	cin >> source >> target;
	Folder *cp_now = (Folder*)now_pos;
	while (1) {
		if (target.find('/') == string::npos)
			break;
		else {
			int place = target.find('/');
			string temp;
			temp = target.substr(0, place);
			cp_now = DiskMannger::cd(temp, cp_now);
			if (cp_now == NULL) {
				cout << "目录不存在!" << endl;
				return;
			}
			target = target.substr(place + 1);
		}
	}
	char sou[1000] = { 0 }, tar[1000] = { 0 };
	strcpy(sou, source.c_str());
	strcpy(tar, target.c_str());
	struct stat buf;
	if (stat(sou, &buf) < 0) {
		cout << "源文件不存在！" << endl;
		return;
	}
	if (buf.st_size > fat.freeblock.size() * 4084) {
		cout << "源文件太大！" << endl;
		return;
	}
	FILE *fp_s = fopen(sou, "rb+");
	if (fp_s == NULL) {
		cout << "源文件不存在！" << endl;
		return;
	}
	int size = 1, size_all = 1;
	File *now = make(tar, cp_now);
	if (now == NULL) {
		cout << "文件复制失败" << endl;
		return;
	}
	int block_id = now->nodeId; //文件
	now->data = fat.getBlock();
	int d = now->data;
	fseek(root_fp, (d - 1)*BLOCK_SIZE, 0);
	fwrite(&d, sizeof(int), 1, root_fp);
	fseek(root_fp, (block_id - 1)*BLOCK_SIZE + 20, 0);
	fwrite(&d, sizeof(int), 1, root_fp);
	char temp;
	fseek(root_fp, (d - 1)*BLOCK_SIZE + 12, 0);
	while (!feof(fp_s)) {
		size_all++;
		if (size == 4085) {
			size = 1;
			int n = fat.getBlock();
			if (n == -1) {
				cout << "文件复制失败,当前空间不足" << endl;
				return;
			}
			fseek(root_fp, (n - 1)*BLOCK_SIZE, 0);
			fwrite(&n, sizeof(int), 1, root_fp);
			fseek(root_fp, (d - 1)*BLOCK_SIZE + 8, 0);
			fwrite(&n, sizeof(int), 1, root_fp);
			d = n;
			fseek(root_fp, (d - 1)*BLOCK_SIZE + 12, 0);
		}
		temp = fgetc(fp_s);
		fwrite(&temp, sizeof(char), 1, root_fp);
		size++;
	}
	temp = '\0';
	fwrite(&temp, sizeof(char), 1, root_fp);
	fseek(root_fp, 0, 1);
	int end = -1;
	fseek(root_fp, (d - 1)*BLOCK_SIZE + 8, 0);
	fwrite(&end, sizeof(int), 1, root_fp);
	fseek(root_fp, (block_id - 1)*BLOCK_SIZE + 8, 0);
	fwrite(&size_all, sizeof(int), 1, root_fp);
	now->size = size_all;
	fseek(root_fp, 0, 1);
	fclose(fp_s);
	cout << "文件复制成功!" << endl;
}


void DiskMannger::rename() {//规则：要改名的文件必须输入全名，即filename.typename的形式，本身没有拓展名的除外（只能对文件改名）
	string source, target;
	cin >> source >> target;
	if (source.length() > 12) {
		cout << "源文件名称太长!" << endl;
		return;
	}
	if (target.length() > 12) {
		cout << "目标文件名称太长!" << endl;
		return;
	}
	bool find = false;     //查找是否存在同名文件夹
	char all_s[15] = { 0 }, name_s[9] = { 0 }, type_s[4] = { 0 };
	char all_t[15] = { 0 }, name_t[9] = { 0 }, type_t[4] = { 0 };
	strcpy(all_s, source.c_str());
	char *point, *p;
	int i = 0;
	point = strrchr(all_s, '.');
	if (point == NULL) {
		if (strlen(all_s) > 8) {
			cout << "文件名超过8位!" << endl;
			return;
		}
		strcpy(name_s, all_s);
	}
	else {
		if (strlen(point + 1) > 3) {
			cout << "源文件后缀名超过3位!" << endl;
			return;
		}
		if (source.length() - 1 - strlen(point + 1) > 8) {
			cout << "源文件名超过8位!" << endl;
			return;
		}
		for (p = all_s, i = 0; p < point; p++) {
			name_s[i++] = *p;
		}
		for (p = point + 1, i = 0; *p != '\0'; p++) {
			type_s[i++] = *p;
		}
	}
	int blockNum = findBlockNum(name_s, type_s);
	if (blockNum == -1) {
		cout << "该文件不存在！" << endl;
		return;
	}
	strcpy(all_t, target.c_str());
	point = strrchr(all_t, '.');
	if (point == NULL) {
		if (strlen(all_t) > 8) {
			cout << "文件名超过8位!" << endl;
			return;
		}
		strcpy(name_t, all_t);
	}
	else {
		if (strlen(point + 1) > 3) {
			cout << "源文件后缀名超过3位!" << endl;
			return;
		}
		if (target.length() - 1 - strlen(point + 1) > 8) {
			cout << "源文件名超过8位!" << endl;
			return;
		}
		for (p = all_t, i = 0; p < point; p++) {
			name_t[i++] = *p;
		}
		for (p = point + 1, i = 0; *p != '\0'; p++) {
			type_t[i++] = *p;
		}
	}


	int  findfile = findBlockNum(name_t, type_t);
	if (findfile != -1)
	{
		cout << "目标路径存在同名文件夹，请重新命名！" << endl;
		return;
	}



	Folder* now_folder = (Folder*)now_pos;
	int blockNum_inDisk = now_folder->child_node[blockNum]->nodeId;
	/*内存中更改*/
	strcpy(((File*)now_folder->child_node[blockNum])->name, name_t);
	strcpy(((File*)now_folder->child_node[blockNum])->type, type_t);

	/*磁盘中更改*/
	fseek(root_fp, (blockNum_inDisk - 1) * BLOCK_SIZE + 24, SEEK_SET);
	fwrite(name_t, sizeof(char), 9, root_fp);
	fseek(root_fp, 24, SEEK_CUR);
	fwrite(type_t, sizeof(char), 4, root_fp);
	fseek(root_fp, 0, SEEK_CUR);
	/*统一更新最后修改时间*/
	time_t timep;
	struct tm *pt;
	time(&timep);
	pt = gmtime(&timep);
	fseek(root_fp, (blockNum_inDisk - 1) * BLOCK_SIZE + 45, SEEK_SET);
	short t[6] = { 0 };
	t[0] = (short)(pt->tm_year + 1900);
	t[1] = (short)(pt->tm_mon + 1);
	t[2] = (short)pt->tm_mday;
	t[3] = (short)(pt->tm_hour + 8);
	t[4] = (short)pt->tm_min;
	t[5] = (short)pt->tm_sec;
	/*内存中*/
	for (int i = 0; i < 6; i++) {
		((File*)now_folder->child_node[blockNum])->last_time[i] = t[i];
	}
	/*磁盘中*/
	fwrite(&t[0], sizeof(short), 1, root_fp);
	fwrite(&t[1], sizeof(short), 1, root_fp);
	fwrite(&t[2], sizeof(short), 1, root_fp);
	fwrite(&t[3], sizeof(short), 1, root_fp);
	fwrite(&t[4], sizeof(short), 1, root_fp);
	fwrite(&t[5], sizeof(short), 1, root_fp);
	fseek(root_fp, 0, SEEK_CUR);
	cout << "操作成功" << endl;
	return;
}


void sortname(Folder * folder)//按名称顺序排列子文件、文件夹
{
	for (int i = 0; i < folder->childsize; i++)
	{
		for (int j = i; j < folder->childsize; j++)
		{
			if (folder->child_node[i]->Ftype == 1)
			{
				if (folder->child_node[j]->Ftype == 1)//两个文件
				{
					File *f1 = (File*)folder->child_node[i];
					File *f2 = (File*)folder->child_node[j];
					if (strcmp(f1->name, f2->name) > 0)
					{
						swap(folder->child[i], folder->child[j]);
						swap(folder->child_node[i], folder->child_node[j]);
					}
					else if (strcmp(f1->name, f2->name) == 0)//文件名相同
					{
						if (strcmp(f1->type, f2->type) > 0)//比较后缀
						{
							swap(folder->child[i], folder->child[j]);
							swap(folder->child_node[i], folder->child_node[j]);
						}
					}
				}
				else//i是文件j是文件夹
				{
					File *f1 = (File*)folder->child_node[i];
					Folder *f2 = (Folder*)folder->child_node[j];
					if (strcmp(f1->name, f2->name) > 0)
					{
						swap(folder->child[i], folder->child[j]);
						swap(folder->child_node[i], folder->child_node[j]);
					}
					//如果名称相同则维持文件在前文件夹在后
				}
			}
			else if (folder->child_node[i]->Ftype == 2)
			{
				if (folder->child_node[j]->Ftype == 2)//两个文件夹
				{
					Folder *f1 = (Folder*)folder->child_node[i];
					Folder *f2 = (Folder*)folder->child_node[j];
					if (strcmp(f1->name, f2->name) > 0)
					{
						swap(folder->child[i], folder->child[j]);
						swap(folder->child_node[i], folder->child_node[j]);
					}
				}
				else//j是文件i是文件夹
				{
					Folder *f1 = (Folder*)folder->child_node[i];
					File *f2 = (File*)folder->child_node[j];
					if (strcmp(f1->name, f2->name) > 0)
					{
						swap(folder->child[i], folder->child[j]);
						swap(folder->child_node[i], folder->child_node[j]);
					}
					//如果名称相同则维持文件在前文件夹在后
					/*else if (strcmp(f1->name, f2->name) == 0)
					{
						swap(folder->child[i], folder->child[j]);
						swap(folder->child_node[i], folder->child_node[j]);
					}*/
				}
			}
		}
	}
}



void dfs_file(int depth, FCB* now, int dfstype, int drortree)//dfstype为1是列出普通目录，2是详细目录
{
	if (drortree == 0) return;
	if (now->Ftype == 1)//文件
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_GREEN);//绿色
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

		if (drortree < 0)
		{
			sortname((Folder*)now);
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);//黄色
		Folder *now_folder = (Folder*)now;
		if (dfstype == 1)
			for (int i = 0; i < depth; i++)
				cout << "         ";
		else
			for (int i = 0; i < depth; i++)
				cout << "             ";
		cout << now_folder->name << endl;
		for (int i = 0; i < now_folder->childsize; i++)
			dfs_file(depth + 1, now_folder->child_node[i], dfstype, drortree - 1);
	}
}


void DiskMannger::dr() {
	char judge;
	judge = getchar();
	if (now_pos->Ftype == 2)
	{
		Folder * now_folder = (Folder *)now_pos;
		sortname(now_folder);
		if (judge == '\n') {
			cout << "列出文件目录" << endl;
			dfs_file(0, now_folder, 1, 2);

		}
		else if (judge == ' ') {
			string flag;
			cin >> flag;
			if (flag == "-l") {
				cout << "列出文件详细目录" << endl;
				dfs_file(0, now_folder, 2, 2);
			}
			else {
				cout << "输入指令错误，请重新输入！！" << endl;
			}
		}
	}
	else
	{
		cout << "请先返回上层文件夹" << endl;
		return;
	}//白色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void DiskMannger::tree() {
	char judge;
	judge = getchar();
	Folder *now_folder = (Folder*)root_folder;
	sortname(root_folder);
	if (judge == '\n') {
		cout << "列出文件目录" << endl;
		dfs_file(0, root_folder, 1, -1);

	}
	else if (judge == ' ') {
		string flag;
		cin >> flag;
		if (flag == "-l") {
			cout << "列出文件详细目录" << endl;
			dfs_file(0, root_folder, 2, -1);
		}
		else {
			cout << "输入指令错误，请重新输入！！" << endl;
		}
	}
	//白色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void DiskMannger::read() {
	Folder *cp_now = (Folder *)now_pos;
	string FName;
	cin >> FName;
	while (1) {
		if (FName.find('/') == string::npos)
			break;
		else {
			int place = FName.find('/');
			string temp;
			temp = FName.substr(0, place);
			cp_now = DiskMannger::cd(temp, cp_now);
			if (cp_now == NULL) {
				cout << "目录不存在!" << endl;
				return;
			}
			FName = FName.substr(place + 1);
		}
	}
	if (FName.length() > 12) {
		cout << "文件名太长!" << endl;
		return;
	}
	char all[20] = { 0 }, name[9] = { 0 }, type[4] = { 0 };
	strcpy(all, FName.c_str());
	char *point, *p;
	point = strrchr(all, '.');
	if (point == NULL) {
		if (strlen(all) > 8) {
			cout << "文件名超过8位!" << endl;
			return;
		}
		strcpy(name, all);
	}
	else {
		if (strlen(point + 1) > 3) {
			cout << "文件后缀名超过3位!" << endl;
			return;
		}
		if (FName.length() - 1 - strlen(point + 1) > 8) {
			cout << "文件名超过8位!" << endl;
			return;
		}
		int j = 0;
		for (p = all, j = 0; p < point; p++) {
			name[j++] = *p;
		}
		for (p = point + 1, j = 0; *p != '\0'; p++) {
			type[j++] = *p;
		}
	}
	Folder *now = (Folder *)cp_now;
	int i = 0;
	for (i = 0; i < now->child_node.size(); i++)
	{
		if (now->child_node[i]->Ftype == 1)
		{
			File *now_file = (File *)(now->child_node[i]);
			if (strcmp(name, now_file->name) == 0 && strcmp(type, now_file->type) == 0)
				break;
		}
	}
	if (i == now->child_node.size()) {
		cout << "该文件不存在!" << endl;
		return;
	}
	else {
		int id = now->child_node[i]->nodeId;
		int block_id;
		fseek(root_fp, (id - 1)*BLOCK_SIZE + 20, 0);
		fread(&block_id, sizeof(int), 1, root_fp);
		if (block_id == 0) {
			cout << "文件为空！" << endl;
			return;
		}
		fseek(root_fp, (block_id - 1)*BLOCK_SIZE + 8, 0);
		fread(&block_id, sizeof(int), 1, root_fp);
		while (true) {
			char temp;
			if (block_id == -1) {
				while (true) {
					fread(&temp, sizeof(char), 1, root_fp);
					if (temp == '\0')
						break;
					putchar(temp);
				}
				putchar('\n');
				break;
			}
			else {
				for (int k = 0; k < 4084; k++) {
					fread(&temp, sizeof(char), 1, root_fp);
					putchar(temp);
				}
				fseek(root_fp, (block_id - 1)*BLOCK_SIZE + 8, 0);
				fread(&block_id, sizeof(int), 1, root_fp);
			}
		}
	}
}

int match(char *s, char *a) {//s是正则表达式，a是被匹配的字符串 
	if (*s == '\0' && *a == '\0') {
		return 1;
	}
	if (*s == '\0' && *a != '\0') {
		return 0;
	}
	if (*s != '\0' && *a == '\0') {
		if (*s != '*') {
			return 0;
		}
		else {
			return 1;
		}
	}

	if (*s != '*' && *s != '?') {
		if (*s != *a) {
			return 0;
		}
		else {
			return match(s + 1, a + 1);
		}
	}
	if (*s == '?') {
		return match(s + 1, a + 1);
	}
	if (*s == '*') {
		int len = strlen(a);
		for (int i = 0; i <= len; i++) {
			if (match(s + 1, a + i)) {
				return 1;
			}
		}
		return 0;
	}
}


void DiskMannger::dl() {
	string FName;
	cin >> FName;
	Folder *cp_now = (Folder*)now_pos;
	while (1) {
		if (FName.find('/') == string::npos)
			break;
		else {
			int place = FName.find('/');
			string temp;
			temp = FName.substr(0, place);
			cp_now = DiskMannger::cd(temp, cp_now);
			if (cp_now == NULL) {
				cout << "目录不存在!" << endl;
				return;
			}
			FName = FName.substr(place + 1);
		}
	}
	if (FName.size() > 12)
	{
		cout << "请输入合法文件名" << endl;
		return;
	}
	char all[15] = { 0 }, cur_name[15] = { 0 }, cur_type[4] = { 0 };
	strcpy(all, FName.c_str());
	char *point, *p;
	int i = 0;
	point = strrchr(all, '.');
	if (point == NULL)
		strcpy(cur_name, all);
	else
	{
		for (p = all, i = 0; p < point; p++)
			cur_name[i++] = *p;
		for (p = point + 1, i = 0; *p != '\0'; p++)
			cur_type[i++] = *p;
	}
	if (strlen(cur_name) > 8)
	{
		cout << "请输入合法文件名" << endl;
		return;
	}
	if (strlen(cur_type) > 3)
	{
		cout << "请输入合法文件后缀名" << endl;
		return;
	}
	if (cp_now->Ftype == 1)
	{
		cout << "若删除此文件请先使用cd ..命令返回上层文件夹" << endl;
		return;
	}
	Folder *now_folder = cp_now;
	bool flag = false;
	for (int j = 0; j < now_folder->childsize; j++)
		if (now_folder->child_node[j]->Ftype == 1)
		{
			File * now_file = (File *)(now_folder->child_node[j]);
			//char all[13] = { 0 };
			//strcpy(all, now_file->name);
			//strcat(all, ".");
			//strcat(all, now_file->type);
			if (match(cur_name, now_file->name) && match(cur_type, now_file->type))
			{
				flag = true;
				now_file->delete_file(now_file, root_fp, &fat);
				//now_pos = now_file->fa_node;
				//删除父文件夹中对应的FCB*child_node及int child
				//维护fcb* vector
				int fileid = now_folder->child_node[j]->nodeId;
				now_folder->child_node.erase(now_folder->child_node.begin() + j);
				//维护int vector
				int i;
				for (i = 0; i < now_folder->childsize; i++)
					if (fileid == now_folder->child[i]) break;
				now_folder->child.erase(now_folder->child.begin() + i);
				//同时保证向磁盘中写回维护vector int child
				now_folder->childsize--;
				if (now_folder->childsize > 0)
				{
					int last_Id = now_folder->child[now_folder->childsize - 1];
					int folderId = now_folder->nodeId;
					fseek(root_fp, 4096 * (folderId - 1) + 53, SEEK_SET);
					fwrite(&now_folder->childsize, sizeof(int), 1, root_fp);
					fseek(root_fp, sizeof(int) * (i - 1), SEEK_CUR);
					fwrite(&last_Id, sizeof(int), 1, root_fp);
					//保证写回
					fseek(root_fp, 0, SEEK_CUR);
				}
				else
				{
					int folderId = now_folder->nodeId;
					fseek(root_fp, 4096 * (folderId - 1) + 53, SEEK_SET);
					fwrite(&now_folder->childsize, sizeof(int), 1, root_fp);
				}
				//我知道了我写了一晚上bug啊啊啊啊啊啊！！！！QAQ
				j--;
				cout << "删除名为" << now_file->name << "."
					<< now_file->type << "的文件" << endl;
			}
		}
		else continue;
	if (!flag)
	{
		cout << "当前目录下不存在该文件！" << endl;
		return;
	}
}

Folder * DiskMannger::cd(string p, Folder * cp_now) {
	string address = p;
	Folder  *now = cp_now;  //当前目录
	Folder *tar_now = NULL;
	if (address == "..")
	{
		if (now->nodeId == 18)   //现在在根目录
		{
			return now;
		}
		else
		{
			tar_now = (Folder *)now->fa_node;
			return tar_now;
		}
	}
	if (address == "~")
	{
		tar_now = root_folder;
		return tar_now;
	}
	if (address == ".")
	{
		return now;
	}
	bool success = true; //是否存在当前路径
	string cur_add;  //目标路径分割
	for (int i = 0; i < address.length(); i++)    //分割字符串并在各级目录中寻找是否存在目标目录
	{
		if (address[i] == '\\' || i == address.length() - 1)
		{
			bool find = false;
			if (address[i] != '\\')
			{
				cur_add = cur_add + address[i];
			}
			char cur_name[100];
			memset(cur_name, 0, sizeof(cur_name));
			strcpy(cur_name, cur_add.c_str());
			for (int j = 0; j < now->child_node.size(); j++)
			{
				if (now->child_node[j]->Ftype == 1)
				{
					find = false;
					continue;
				}
				else
				{
					Folder *now_folder = (Folder *)(now->child_node[j]);
					if (strcmp(cur_name, now_folder->name) == 0)
					{
						find = true;
						now = now_folder;
						cur_add.clear();

						break;
					}
				}
			}
			if (find == false)
			{
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
		tar_now = now;
		return tar_now;
	}
	else
	{
		return NULL;
	}
}


void DiskMannger::cd() {
	getchar();
	string address;
	cin >> address;
	Folder  *now = (Folder *)now_pos;  //当前目录
	if (address == "..")
	{
		if (now->nodeId == 18)   //现在在根目录
		{
			cout << "已到达根目录不可返回" << endl;
			return;
		}
		else
		{
			int now_length = path.length();
			int name_length = strlen(now->name);
			path.erase(now_length - name_length - 1);
			now_pos = now->fa_node;
			cout << "已返回上级目录" << endl;
			return;
		}
	}
	if (address == "~")
	{
		path.erase(0);
		path.append("MINI - FS\\");
		path.append(root_folder->name);
		now_pos = root_folder;
		cout << "已返回根目录" << endl;
		return;
	}
	bool success = true; //是否存在当前路径
	string cur_add;  //目标路径分割
	for (int i = 0; i < address.length(); i++)    //分割字符串并在各级目录中寻找是否存在目标目录
	{
		if (address[i] == '\\' || i == address.length() - 1)
		{
			bool find = false;
			if (address[i] != '\\')
			{
				cur_add = cur_add + address[i];
			}
			char cur_name[100];
			memset(cur_name, 0, sizeof(cur_name));
			strcpy(cur_name, cur_add.c_str());
			for (int j = 0; j < now->child_node.size(); j++)
			{
				//fseek(root_fp, (now->child[j] - 1) * BLOCK_SIZE + 8, SEEK_SET);
				//擦汗ar tar_name[9];
				//fread(tar_name, sizeof(char), 9, root_fp);
				if (now->child_node[j]->Ftype == 1)
				{
					find = false;
					continue;
				}
				else
				{
					Folder *now_folder = (Folder *)(now->child_node[j]);
					if (strcmp(cur_name, now_folder->name) == 0)
					{
						find = true;
						now = now_folder;
						cur_add.clear();

						break;
					}
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
		path.append("\\" + address);
		cout << "进入" << address << "的文件夹" << endl;
	}
}



void DiskMannger::att() {
	string FName;
	cin >> FName;
	Folder *cp_now = (Folder*)now_pos;
	while (1) {
		if (FName.find('/') == string::npos)
			break;
		else {
			int place = FName.find('/');
			string temp;
			temp = FName.substr(0, place);
			cp_now = DiskMannger::cd(temp, cp_now);
			if (cp_now == NULL) {
				cout << "目录不存在!" << endl;
				return;
			}
			FName = FName.substr(place + 1);
		}
	}
	if (FName.length() > 12) {
		cout << "文件名太长!" << endl;
		return;
	}
	char all[20] = { 0 }, name[9] = { 0 }, type[4] = { 0 };
	strcpy(all, FName.c_str());
	char *point, *p;
	point = strrchr(all, '.');
	if (point == NULL) {
		if (strlen(all) > 8) {
			cout << "文件名超过8位!" << endl;
			return;
		}
		strcpy(name, all);
	}
	else {
		if (strlen(point + 1) > 3) {
			cout << "文件后缀名超过3位!" << endl;
			return;
		}
		if (FName.length() - 1 - strlen(point + 1) > 8) {
			cout << "文件名超过8位!" << endl;
			return;
		}
		int j = 0;
		for (p = all, j = 0; p < point; p++) {
			name[j++] = *p;
		}
		for (p = point + 1, j = 0; *p != '\0'; p++) {
			type[j++] = *p;
		}
	}
	Folder *now = cp_now;
	int j;
	bool isFound = false;
	for (j = 0; j < now->child_node.size(); j++)
	{
		if (now->child_node[j]->Ftype == 1)
		{
			File *now_file = (File *)(now->child_node[j]);
			if (strcmp(name, now_file->name) == 0 && strcmp(type, now_file->type) == 0)
			{
				isFound = true;
				break;
			}
		}
	}
	if (!isFound) {
		cout << "该目录下文件不存在" << endl;
		return;
	}
	int id = now->child_node[j]->nodeId;
	int this_id, size, access, father, data;	//长度分别为(Byte)：4, 4, 4，4，4
	char this_name[9] = { 0 };			//9 Byte
	char this_type[4] = { 0 };			//4 Byte
	short create_time[6], last_time[6];//长度分别为(Byte)：12，12

	fseek(root_fp, (id - 1)*BLOCK_SIZE, 0);
	fread(&this_id, sizeof(int), 1, root_fp);
	fseek(root_fp, sizeof(int), 1);
	fread(&size, sizeof(int), 1, root_fp);
	fread(&access, sizeof(int), 1, root_fp);
	fread(&father, sizeof(int), 1, root_fp);
	fread(&data, sizeof(int), 1, root_fp);

	fread(this_name, sizeof(char), 9, root_fp);
	fread(create_time, sizeof(short), 6, root_fp);
	fread(last_time, sizeof(short), 6, root_fp);
	fread(this_type, sizeof(char), 4, root_fp);

	//width = 50
	printf("文件名");
	printFun(44 - strlen(this_name));
	printf("%s\n", this_name);

	printf("文件类型");
	printFun(42 - strlen(this_type));
	printf("%s\n", this_type);

	printf("文件大小");
	printFun(42 - int_len(size));
	printf("%d\n", size);

	printf("文件保护类型");
	printFun(38 - int_len(access));
	printf("%d\n", access);

	printf("文件索引块编号");
	printFun(36 - int_len(this_id));
	printf("%d\n", this_id);

	printf("父文件索引块编号");
	printFun(34 - int_len(father));
	printf("%d\n", father);

	printf("文件首个数据块编号");
	printFun(32 - int_len(data));
	printf("%d\n", data);

	printf("文件创建时间");
	printFun(20);
	printf("%d/%d/%d %02d:%02d:%02d\n", create_time[0], create_time[1],
		create_time[2], create_time[3], create_time[4], create_time[5]);

	printf("文件最后修改时间");
	printFun(16);
	printf("%d/%d/%d %02d:%02d:%02d\n", last_time[0], last_time[1],
		last_time[2], last_time[3], last_time[4], last_time[5]);
	return;

}





void DiskMannger::rm() {
	string FName;
	cin >> FName;
	cout << "彻底删除名为" << FName << "的文件或者文件夹" << endl;
}
