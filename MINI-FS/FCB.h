#pragma once

#include <string>
using namespace std;

class FCB
{
public:
	int nodeId;//���  �ļ���ʶ������ϵͳ�����ļ���Ψһ��ʶ��
	int Ftype;//1--�ļ�  2--�ļ���
	FCB();
	~FCB();
	string getTime();//��ȡϵͳʱ��
};
