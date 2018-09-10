#pragma once
#include "FCB.h"
#include<vector>


class Folder :public FCB //�ļ���
{
public:
	char name[9];//�ļ�����    �ļ���
	char create_time[20];//����ʱ��
	char last_time[20];//����޸�ʱ��
	int size;//�ļ���С�����ֽڻ��Ϊ��λ���ļ����ȡ�
	int access;//�ļ��ı�����ʽ:1ֻ�� 2��д 3��ִ��
	int father;//ָ���ļ�������
	int childsize;
	vector <int >child;//���ļ�������?????????���ٸ�

	Folder();
	~Folder();

	void addChild(FCB* file);
	bool count(FCB *file);//�ж�Ŀ���ļ��Ƿ����
	int find(FCB *file);//�ҵ�Ŀ���ļ��������ļ�������Ŀ��
	bool delete_folder(FCB* file);//ɾ���ļ��� 
};
