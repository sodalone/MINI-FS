#include "File.h"
#include"FAT.h"
#include "DiskMannger.h"

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
			fseek(fp, sizeof(int), 1);///////////////中间参数是unsigned型不能是负数！
			fwrite(&id[i - 1], sizeof(int), 1, fp);
		}
		if (i != sum - 1) {
			fwrite(p, sizeof(char), 4084, fp);
			p += 4084;
		}
		else
			fwrite(p, sizeof(char), (lenth % 4084), fp);
	}
	return true;/////////////////返回值？？？？？？？
}

void File::delete_file(File *now, FILE *root, FAT * fat)//now_pos  root_fp
{
	now->flag = 1;
	int nowId = now->data;
	int byte, bit;
	//回收文件索引块
	byte = now->nodeId / 8;
	bit = now->nodeId % 8;
	fat->recmap[byte] |= 1 << (7 - bit);
	if (now->data == 0) return;
	while (nowId != -1)//回收文件数据块
	{
		//更新recbitmap
		byte = (nowId - 1) / 8;
		bit = (nowId - 1) % 8;
		fat->recmap[byte] |= 1 << (7 - bit);

		//移动到下一个数据块blockId前
		fseek(root, 4096 * (nowId - 1) + 2 * sizeof(int), SEEK_SET);
		fread(&nowId, sizeof(int), 1, root);
	}
}

