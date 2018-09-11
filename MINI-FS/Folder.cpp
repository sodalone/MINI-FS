#include "Folder.h"

Folder::Folder()
{
	this->nodeId = 0;
	this->Ftype = 1;
	this->size = 0;
	this->access = 3;
	this->father = 0;
	this->childsize = 0;
	this->flag = 0;
	this->fa_node = NULL;
	this->child = vector<int>(0);
	this->child_node = vector<FCB *>(0);
}

Folder::~Folder()
{
}

void Folder::addChild(FCB * file)
{
}

bool Folder::count(FCB * file)
{
	return false;
}

int Folder::find(FCB * file)
{
	return 0;
}

bool Folder::delete_folder(FCB * file)
{
	return false;
}
