#pragma once
#include "FCB.h"


class Block :public FCB//���ݿ�
{
	int nextblock;//ָ����һ�����ݿ�
	char data[4092];//��¼����

	Block();
	~Block();

	void clear();//�����������
	void write();//����????????���ݿ�д�����ݵ���Ҫ��ôд�룿
};