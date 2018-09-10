#pragma once
#include <cstdio>
#include"FCB.h"
using namespace std;
class DiskMannger {
public:
	DiskMannger(); //���캯��
	~DiskMannger();//��������

	void create();//����1G��Windows�ռ�
	void mount();//�����Ѵ����ռ�
	void fmt(FILE *fp);//��ʼ����ǰ�ռ�

	void make(); //�����ļ�
	void open(); //���ļ�
	void exit(); //�˳���ǰ���ļ�
	void write(); //�ļ���д�룻
	void read(); //�ļ��Ķ��룻
	void cp(); //�ļ���Ŀ¼�ĸ���
	void rename(); //�ļ�������
	void dr(); //�г��ļ�Ŀ¼��������ϸ��Ϣ
	void tp(int curBlockNum); //��ʾ�ռ��е��ı��ļ���ֻ��ʾ�ļ�
	void dl(); //ɾ���ļ������ļ���
	void cd(); //�����ļ���
	void att(); //��ʾ�ļ�����
	void more(); //��ҳ��ʾ
	void back(); //�����ϼ�Ŀ¼
	void recover(); //�ָ�һ��ɾ���ļ�
	void clear(); //��ջ���վ
	void rm(); //ɾ���ļ�
	void search(int, FCB*);
};