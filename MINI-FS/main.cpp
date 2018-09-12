#include "DiskMannger.h"
#include"windows.h"


using namespace std;
const static int BLOCKSIZE = 4 * 1024;
const static int BLOCKNUM = 212644;
int main() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
	DiskMannger();
	return 0;
}