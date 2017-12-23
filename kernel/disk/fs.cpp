#include "fs.h"
#include "stdlib.h"
#include "stdio.h"
#include "asm.h"

iFolder iROOT;
FOLDER ROOTDIR;
FOLDER currdir;

#define nsu (4)
byte SectUsage[nsu * 512]; //前128个扇区，每一位代表一个扇区是否正在使用（是为1）

static lba48 nextEmptySect;

lba48 getNextSect()
{
	int i, j;
	for (i = 0; i < nsu * 512; i++)
	{
		if (SectUsage[i] != 255)
			break;
	}
	byte b = SectUsage[i];
	for (j = 0; j < 8; j++)
	{
		if (bit(b, j) == 0)
		{
			setb(SectUsage[i], j);
			break;
		}
	}
	return (lba48)i * 8 + j;
}

void format_disk()
{
	printf("formatting...");

	//清零
	memset(SectUsage, 0, sizeof(SectUsage));

	int LastUsingSect = 130; //已使用的最后一个扇区

	int i;
	for (i = 0; i <= (LastUsingSect / 8); i++)
	{
		SectUsage[i] = 255;
	}
	SectUsage[i] = ~((255 >> LastUsingSect / 8) << LastUsingSect / 8);
	disk_write(SectUsage, nsu, 1);

	//初始化根目录
	fsAttr attr;
	attr.isHead = true;
	attr.isFolder = true;
	attr.isHidden = false;
	attr.isTail = true;
	attr.isNull = false;
	iROOT.attr = attr;
	char *name = root_dir_name;
	memcpy(iROOT.name, name, sizeof(root_dir_name));
	nextEmptySect = ROOT_LBA + 1;

	iROOT.usage = 0;

	disk_write(&iROOT, 1, ROOT_LBA);

	printf("done.\n");
}

bool init_fs()
{
	disk_read(SectUsage, nsu, 1);
	disk_node_read(iROOT, ROOT_LBA);

	ROOTDIR.lba = ROOT_LBA;
	memcpy(ROOTDIR.name, iROOT.name, folder_name_size);

	currdir = ROOTDIR;
	// printf("root folder's name:%s\n",root.name);
	return true;
}

void sync()
{
	disk_write(SectUsage, nsu, 1);
}

template <typename DISK_NODE>
void disk_node_write(DISK_NODE &dn, lba48 lba)
{
	disk_write(&dn, 1, lba);
}

template <typename DISK_NODE>
void disk_node_read(DISK_NODE &dn, lba48 lba)
{
	disk_read(&dn, 1, lba);
}

struct FindResult
{
	iFolder ifolder;
	lba48 lba_file, lba_folder;
	u8 pos;
	bool isSucceed;
};

template <typename DISK_NODE>
FindResult find_folder_child(FOLDER &fd, DISK_NODE &dn)
{
}

FindResult find_folder_empty_child(FOLDER &fd)
{
	// disk_node_read(ifolder,fd.lba);
	lba48 lba = fd.lba;
	lba48 lastlba;
	FindResult fr;
	//当目录节点孩子满时，切换到next节点
	iFolder ifolder;
	do
	{
		disk_node_read(ifolder, lba);
		lastlba = lba;
		lba = ifolder.next;
		if (lba == 0)
		{
			fr.isSucceed = false;
			return fr;
		}
	} while (ifolder.usage == ((1 << (folder_child_size + 1)) - 1)); //while(usage.last $size bits are all 1)

	fr.ifolder = ifolder;
	fr.isSucceed = true;
	//fr.pos = ifolder.nChilds;
	int usage = ifolder.usage;
	int pos = 0;
	for (pos = 0; (usage & 1) == 1; pos++, usage >> 1)
	{
	}
	fr.pos = pos;
	fr.lba_folder = lastlba;
	return fr;
}

void folder_add(FOLDER &fd, lba48 childlba)
{
	FindResult fr = find_folder_empty_child(fd);
	iFolder ifd;
	lba48 lba;
	if (fr.isSucceed)
	{
		//在子节点中找到空项，加进去
		ifd = fr.ifolder;
		u8 fpos = fr.pos;
		ifd.childs[fpos] = childlba;
		setb(ifd.usage, fpos);
		lba = fr.lba_folder;
	}
	else
	{
		//没找到则创建新目录节点

		lba = getNextSect();
		fsAttr attr;
		attr.isFolder = true;
		attr.isHead = false;
		attr.isHidden = false;
		attr.isNull = false;
		attr.isTail = true;
		ifd.attr = attr;

		lba48 lastlba = fr.lba_folder;
		iFolder lastfolder;
		disk_node_read(lastfolder, lastlba);
		lastfolder.attr.isTail = false;
		lastfolder.next = lba;
		disk_node_write(lastfolder, lastlba);
	}
	disk_node_write(ifd, lba);
}

template <typename DISK_NODE>
inline void operator+=(FOLDER &fd, DISK_NODE &dn)
{
	folder_add(fd, dn);
}

FILE create_file(char *name, void *pdata, u64 len, bool isHead)
{
	byte *data = (byte *)pdata;
	iFile ifile;
	memcpy(ifile.name, name, 31);
	memset(ifile.data, 0, file_data_size);

	//设置属性
	fsAttr attr;
	attr.isHead = isHead;
	attr.isFolder = false;
	attr.isHidden = false;
	attr.isNull = false;

	if (len < file_data_size)
	{
		//如果数据小于单节点大小，存进去
		attr.isTail = true;
		ifile.nData = len;
		memcpy(ifile.data, data, len);
	}
	else
	{
		//如果大于，存储前面的，把后面的递归交给新文件节点
		attr.isTail = len > file_data_size;
		ifile.nData = file_data_size;
		memcpy(ifile.data, data, file_data_size);
		FILE next = create_file(name, data + file_data_size, len - file_data_size);
		ifile.next = next.lba;
	}
	ifile.attr = attr;

	lba48 lba = getNextSect();
	disk_node_write(ifile, lba);

	FILE f;
	f.lba = lba;
	memcpy(f.name, name, 31);

	folder_add(ROOTDIR, lba);

	return f;
}