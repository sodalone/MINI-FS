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
	cout << "��ӭ����-----------��������help��ð���------------" << endl;
	string cmd;
	while (cin >> cmd) {
		if (cmd == "help") {
			cout << "�����ĵ�" << endl;
			cout << "\n��fmt:\n" <<
				"��close:\n" <<
				"��make:\n" <<
				"��open : \n" <<
				"��exit : \n" <<
				"��write : \n" <<
				"��read : \n" <<
				"��cp : \n" <<
				"��dr : \n" <<
				"��dl : \n" <<
				"��tp : \n" <<
				"��more : \n" <<
				"��att : \n" <<
				"��back :\n" <<
				"��rename:\n" <<
				"��recover:\n" <<
				"��clear:\n" <<
				"��rm:\n" <<
				"��* :\n" <<
				"��? :\n"<< endl;
		}
		else if (cmd == "create") {
			this->create();
		}
		else if (cmd == "mount") {
			this->mount();
		}
		else if (cmd == "fmt") {
			this->fmt(root_fp);/////////////����ô���ɶ����������ָ�뼴root_fpѽ����
		}
		else if (cmd == "close") {
			cout << "�˳���ǰ�ռ�" << endl;
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
			cout << "����ָ��������������룡��" << endl;
		}
	}
}
DiskMannger::~DiskMannger(){}

void DiskMannger::create() {
	char SName[9];
	memset(SName, 0, sizeof(SName));
	cin >> SName;
	SName[8] = '\0';
	cout << "����"<< SName <<"�ռ�" << endl; 
	char magic[8] = { 'm','i','n','i','f','s','s','s' };
	if (_access(SName, 0) == 0)
	{
		//�Ѿ���������ļ�ϵͳ
		FILE* fp = fopen(SName, "r"); //��ȡ����ļ�ϵͳ
		fseek(fp, 0, SEEK_SET);//�ص��˿�ʼ��λ��
		char buf[8];
		fread(buf, 1, 8, fp);//��ȡmagic numberƥ��ϵͳ
		bool flag = true;
		for (int i = 0; i < 8 && flag; i++)
			if (magic[i] != buf[i]) flag = false;
		if (1)//���ƥ��
		{
			printf("���ļ�ϵͳ�Ѿ������������������롭��\n");
			return ;
		}
	}
		FILE *fp = fopen(SName, "w+"); //��ȡ����ļ�ϵͳ
		fseek(fp, 0, SEEK_SET);//�ص��˿�ʼ��λ��
		printf("���ν����ļ�ϵͳ�����Ե�\n");
		
		int num = 8;
		int usenum = 262126;
		int binnum = 0;
		fseek(fp, 32768*32768 - 1, SEEK_SET); // ���ļ���ָ�� ���� ָ����С��λ�� 32768*32768
		fputc('a', fp); // ��Ҫָ����С�ļ���ĩβ����һ������
		fseek(fp, 0, SEEK_SET);//�ص��˿�ʼ��λ��
		fwrite(magic,sizeof(char), 8, fp);
		fwrite(&BLOCK_SIZE, sizeof(int), 1, fp);
		fwrite(&num, sizeof(int), 1, fp);
		fwrite(&BLOCK_NUM, sizeof(int), 1, fp);
		fwrite(&usenum, sizeof(int), 1, fp);
		fwrite(&binnum, sizeof(int), 1, fp);
		num = 0;
		unsigned char t=255;
		fseek(fp, 0, SEEK_SET);
		fseek(fp, 1024 * 4, SEEK_CUR);//����������
		//freebitmap ��1 ��ʾ���п鶼����
		for(int i = 0; i < 32768; i ++)
			fwrite(&t, sizeof(char) , 1, fp);
		//recbitmap��0 ��ʾû�п��ǿɻ��տ�
		t = 0;
		for (int i = 0; i < 32768; i++)
			fwrite(&t, sizeof(char), 1, fp);
		//����һ����Ŀ¼ ��18���¼��Ŀ¼��Ϣ
		fseek(fp, 0, SEEK_SET);
		fseek(fp, 1024 * 4 * 17, SEEK_CUR);

		int nodeId = 18;
		int Ftype = 2;
		int size = 0;
		int access = 3;
		int father = 0;
		//��ȡʱ��
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
		int tmp = 0;//ռλ
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
{//���ص��Ѿ��е��ļ�ϵͳ
	char SName[9];
	memset(SName, 0, sizeof(SName));
	cin >> SName;
	SName[8] = '\0';
	char magic[8] = { 'm','i','n','i','f','s','s','s' };
	if (_access(SName, 0) == 0)//�Ѿ����ڸ�ϵͳ
	{
		FILE *fp = fopen(SName, "r");
		root_fp = fp;
		fseek(fp, 0, SEEK_SET);//�ص��˿�ʼ��λ��
		char buf[8];
		fread(buf, 1, 8, fp);//��ȡmagic numberƥ��ϵͳ
		bool flag = true;
		for (int i = 0; i < 8 && flag; i++)
			if (magic[i] != buf[i]) flag = false;
		if (flag)//���ƥ�� ����֮ǰ��¼�Ŀ��ÿ顢�ɻ��տ�Ŀ����Ϣ
		{
			int count_free, count_rec;//�Կ��ÿ顢�ɻ��տ����
			count_free = count_rec = 0;
			fseek(fp, 0, SEEK_SET);
			fseek(fp, 4096 + 2, SEEK_CUR);//ָ���һ��bitmap���ӵ����ֽڿ�ʼ����
										//1~18���ѱ�ռ��
			char now;
			fread(&now, sizeof(char), 1, fp);
			for(int i = 0; i < 6; i ++)//����19~24��
				if (((now >> i ) & 1)== 1)
				{
					count_free++;
					fat.freeblock.push(24 - i);
				}
			for (int i = 3; i < 32768; i++)//��25�ж�
			{
				fread(&now, sizeof(char), 1, fp);
				for (int j = 0; j < 8; j++)
					if (((now >> j ) & 1)== 1)
					{
						count_free++;
						fat.freeblock.push((i + 1) * 8 - j);
					}
			}
			fseek(fp, 2, SEEK_CUR);//�ɻ���bitmap���ӵ����ֽڼ��� 1~18��ռ��
			fread(&now, sizeof(char), 1, fp);
			fat.recmap[2] = now;
			for (int i = 0; i < 6; i++) //����19~24��
				if (((now >> i) & 1) == 1)
					count_rec++;
			for (int i = 3; i < 32768; i++)//��25�ж�
			{
				fread(&now, sizeof(char), 1, fp);
				fat.recmap[i] = now;
				for (int j = 1; j <= 8; j++)
					if ((now >> (j - 1) & 1) == 1)
						count_rec++;
			}


			printf("��ǰ�ļ�ϵͳ�й���%d�տ����,��%lld�ֽ�\n", count_free,(long long) count_free * 4 * 1024);
			printf("��ǰ�ļ�ϵͳ�й���%d�ɻ��տ����,��%lld�ֽ�\n", count_rec, (long long)count_rec * 4 * 1024 );


			root_fp = fopen(SName, "r+");

			fseek(root_fp, 8 + 3 * sizeof(int), SEEK_SET);
			fwrite(&count_free, sizeof(int), 1, root_fp);
			fwrite(&count_rec, sizeof(int), 1, root_fp);
			return;
		}
		else
		{
			printf("��ϵͳ�������Ҵ���ͬ���ļ���\n");
			printf("��ʹ��create������������Ƶ��ļ�ϵͳ��\n");
			return ;
		}
	}
	else
	{
		printf("��ϵͳ�����ڣ�\n");
		printf("��ʹ��create�����Ƚ��д�����\n");
		return;
	}
}

void DiskMannger::fmt(FILE *fp) {////////////////fpָ��ָ��ʲô��
	string flag;
	cin >> flag;
	if (flag == "-1") {
		cout << "�򵥳�ʼ������ɾ��Ŀ¼�ļ����������ݲ�������д" << endl;
		char format = '\0';
		fseek(fp, 22, SEEK_SET);///////λ�ò���ˣ�����recmap������freebitmapλ�ò���????

		fwrite(&ALL_DATA_BLOCK_NUM, sizeof(int), 1, fp);	//�����ÿռ���Ϊ��״̬��262127 
		fwrite(&format, sizeof(char), 17 * 4 * 1024 - 26, fp);//26Ϊ֮ǰָ�뾭�����ֽ��� ��������freebitmap��recbitmapp�ĸ�ʽ��  

		fwrite(&format, sizeof(char), 16, fp);	//��Ŀ¼�Ĳ��ָ�ʽ�� 

		fseek(fp, 49, SEEK_CUR);	//��Ŀ¼���ļ������ƣ�����ʱ�䲻���޸ģ�����޸�ʱ���ݲ����ġ� 

		fwrite(&format, sizeof(char), 2048, fp);//��Ŀ¼����Ŀ¼�����ļ������ĳ�ʼ�� 
	}
	else if (flag == "-2") {
		cout << "��ͨ��ʼ����ɾ��Ŀ¼�ļ���ͬʱ������еĴ�������" << endl;

		char format = '\0';
		fseek(fp, 22, SEEK_SET);

		fwrite(&ALL_DATA_BLOCK_NUM, sizeof(int), 1, fp);	//�����ÿռ���Ϊ��״̬��262127 
		fwrite(&format, sizeof(char), 17 * 4 * 1024 - 26, fp);//26Ϊ֮ǰָ�뾭�����ֽ��� ��������freebitmap��recbitmapp�ĸ�ʽ��  

		fwrite(&format, sizeof(char), 16, fp);	//��Ŀ¼�Ĳ��ָ�ʽ�� 

		fseek(fp, 49, SEEK_CUR);	//��Ŀ¼���ļ������ƣ�����ʱ�䲻���޸ģ�����޸�ʱ���ݲ����ġ� 

		fwrite(&format, sizeof(char), 2048, fp);//��Ŀ¼����Ŀ¼�����ļ������ĳ�ʼ�� 

		fwrite(&format, sizeof(char), 262126 * 4 * 1024 - 2113, fp);//�������ĳ�ʼ�� 
	}
	else {
		cout << "�����������" << endl;
	}
}

void DiskMannger::make() {
	string FName;
	cin >> FName;
	cout << "����" << FName << "�ļ�" <<endl;
}

void DiskMannger::open() {
	string FName;
	cin >> FName;
	cout << "��" << FName << "�ļ�" << endl;
}

void DiskMannger::exit() {//��������bitmap ��Ӧ���ÿ顢�ɻ��տ�����Ȼ���˳�ϵͳ
	//ָ��ڶ����飬Ȼ��д��freebitmap
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
	//Ȼ��д��recbitmap
	fseek(root_fp, 4096 * 9, SEEK_SET);
	for (int i = 0; i < 32768; i++)
	{
		char t = fat.recmap[i];
		fwrite(&t, sizeof(char), 1, root_fp);
	}
	cout << "�˳��ļ�ϵͳ" << endl;
}

void DiskMannger::write() {
	string text;
	cin >> text;
	cout << "д��\n" << text << endl;
}



void DiskMannger::read() {
	cout << "�����ļ�" << endl;
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
		cout << "�г��ļ�Ŀ¼" << endl;
	}
	else if (judge == ' ') {
		string flag;
		cin >> flag;
		if (flag == "-l") {
			cout << "�г��ļ���ϸĿ¼" << endl;
		}
		else {
			cout << "����ָ��������������룡��" << endl;
		}
	}
}

void DiskMannger::tp(int curBlockNum) {
	cout << "��ʾ�ռ��е��ı��ļ���ֻ��ʾ�ļ�" << endl;

	fseek(root_fp, curBlockNum * BLOCK_SIZE, SEEK_SET);	//��λ�����File���͵Ŀ�
	fseek(root_fp, 8, SEEK_CUR);
	int nextNum;
	fread(&nextNum, sizeof(int), 1, root_fp);			//ȡ��File���ڵ�һ�����ݿ�ı��
	while (nextNum != 0) {
		fseek(root_fp, nextNum - curBlockNum + 8, SEEK_CUR);//��λ����ǰ���ݿ飬˳��ȡ����һ�����ݿ�ı��
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
	cout << "ɾ����Ϊ" << FName << "���ļ������ļ���" << endl;
}

void DiskMannger::cd() {
	string address;
	cin >> address;
	cout << "����" << address << "���ļ���" << endl;
}

void DiskMannger::att() {
	string FName;
	cin >> FName;
	cout << "��ʾ��Ϊ" << FName << "���ļ������ļ���" << endl;
}
void DiskMannger::more() {
	string FName;
	cin >> FName;
	cout << "��ҳ��ʾ��Ϊ" << FName << "���ļ������ļ���" << endl;
}

void DiskMannger::back() {
	cout << "�ص��ϼ�Ŀ¼��" << endl;
}

void DiskMannger::recover() {
	string FName;
	cin >> FName;
	cout << "�ָ���Ϊ" << FName << "���ļ������ļ���" << endl;
}

void DiskMannger::clear() {
	cout << "��ջ���վ" << endl;
}

void DiskMannger::rm() {
	string FName;
	cin >> FName;
	cout << "����ɾ����Ϊ" << FName << "���ļ������ļ���" << endl;	
}