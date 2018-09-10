#include "File.h"
#include"FAT.h"
File::File()
{
}

File::File(string _name, int file_access)
{
}

File::~File()
{
}

bool File::write(const char * content, int data, FAT fat_temp, FILE *fp)
{
	int lenth = strlen(content), sum;
	if (lenth % 4084 == 0)
		sum = lenth / 4084;
	else
		sum = lenth / 4084 + 1;
	int *id;
	id = (int *)malloc(sum * sizeof(int));
	id[0] = data;
	for (int i = 1; i < sum; i++)
		id[i] = fat_temp.getBlock();
	const char *p = content;
	for (int i = 0; i < sum; i++) {
		fseek(fp, 0, 0);
		fseek(fp, (id[i - 1] - 1)*BLOCK_SIZE + sizeof(int) * 3, 0);
		if (i != 0) {
			fseek(fp, sizeof(int), 1);///////////////�м������unsigned�Ͳ����Ǹ�����
			fwrite(&id[i - 1], sizeof(int), 1, fp);
		}
		if (i != sum - 1) {
			fwrite(p, sizeof(char), 4084, fp);
			p += 4084;
		}
		else
			fwrite(p, sizeof(char), (lenth % 4084), fp);
	}
	return true;/////////////////����ֵ��������������
}

void File::delete_file()
{
}
