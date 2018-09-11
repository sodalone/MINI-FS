#include "Folder.h"

Folder::Folder()
{
	vector<FCB*>child_node(0);
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
