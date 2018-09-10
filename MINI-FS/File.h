#pragma once
#include "FCB.h"
#include"FAT.h"

class File :public FCB //�ļ� 
{
public:
	char name[9];//�ļ�����    �ļ���
	char create_time[20];//����ʱ��
	char last_time[20];//����޸�ʱ��
	char type[4];//�ļ����ͣ�����չ��������
	int size;//�ļ���С�����ֽڻ��Ϊ��λ���ļ����ȡ�
	int access;//�ļ��ı�����ʽ:1ֻ�� 2��д 3��ִ��
	int father;//���ļ��ڵ�
	int data;//ָ���һ����
	File();
	File(string _name, int file_access);
	~File();

	bool write(const char * content, int data, FAT fat_temp, FILE *fp);//д�ļ� 
	void delete_file();//�ͷŴ��̿�
};
