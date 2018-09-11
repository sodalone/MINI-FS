#include "FAT.h"

using namespace std;

void FAT::init()//�򵥳�ʼ��
{
	//ɾ���ļ�Ŀ¼����
	priority_queue<int, vector<int>, greater<int> > empty;
	swap(empty, this->freeblock);
	for (int i = 19; i <= BLOCK_NUM; i++) {
		this->freeblock.push(i);
	}
	for (int i = 0; i < 262144; i++)
		recmap[i] = 0;
}
void FAT::inittot()//���̳�ʼ��
{
	//ɾ��Ŀ¼������ͬʱɾ�������������ݡ�
	//�Ӹ�Ŀ¼����dfs�����������Ŀ¼��ͬʱ��ȡ�ļ������ݣ�����������ļ������һ�����ݿ飬�ݹ齫���ݿ���Ϊ��/0����
}

int FAT::getBlock()//��ȡ�մ���
{
	if (freeblock.size() > 0) {
		int blockId = this->freeblock.top();
		freeblock.pop();
		return blockId;
	}
	return -1;
}
void FAT::addBlock(int block)//���մ��̿�
{
	this->freeblock.push(block);
}

