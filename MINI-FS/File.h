#pragma once
#include "FCB.h"
#include"FAT.h"

class File :public FCB //�ļ� 
{
public:
	int size;//�ļ���С�����ֽڻ��Ϊ��λ���ļ����ȡ�
	int access;//�ļ��ı�����ʽ:1ֻ�� 2��д 3��ִ��
	int father;//���ļ��ڵ�
	int data;//ָ���һ����
	char name[9];//�ļ�����    �ļ���
	short create_time[6];//����ʱ��
	short last_time[6];//����޸�ʱ��
	char type[4];//�ļ����ͣ�����չ��������
	int flag;//����ļ����Ƿ��Ѿ���ɾ��
	File();
	File(string _name, int file_access);
	~File();

	bool write(const char * content, int data, FAT fat_temp, FILE *fp);//д�ļ� 
	void delete_file();//�ͷŴ��̿�
	FCB * fa_node;
};
