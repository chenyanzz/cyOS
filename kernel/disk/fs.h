#ifndef FS_H
#define FS_H
#include "types.h"
#include "basic_io.h"

#define ROOT_LBA (130)

#define name_size (31)
#define folder_name_size (name_size)
#define file_name_size (name_size)

#define file_data_size (470)

#define folder_child_size (57)

#define root_dir_name "cyOS"

struct fsAttr
{
	bool isNull : 1;
	bool isFolder : 1;
	bool isTail : 1;
	bool isHead : 1;
	bool isHidden : 1;
};

struct iFolder
{
	fsAttr attr;

	char name[name_size];

	lba48 parent;
	lba48 next;

	u64 usage;

	lba48 childs[folder_child_size]; //iFolder & iFile
};

struct iFile
{
	fsAttr attr;

	char name[name_size];

	lba48 next;

	word nData; //count

	byte data[file_data_size];
};

struct FOLDER
{
	lba48 lba;
	// iFolder* ifolder;
	// iFolder* parent;
	byte status;
	char name[31];
};

struct FILE
{
	lba48 lba;
	// iFile* ifile;
	// iFolder* ifolder;
	byte status;
	char name[31];
};

/**
 * 读取root扇区
 */
bool init_fs();

/**
 * 格式化磁盘
 */
void format_disk();

template<typename DISK_NODE> inline 
void disk_node_write(DISK_NODE &dn, lba48 lba);

template<typename DISK_NODE> inline 
void disk_node_read(DISK_NODE &dn, lba48 lba);

/**
 * 创建文件
 * @param  isHead: 是否是独立文件（因为有可能是新建一个.next）
 */
FILE create_file(char *name, void *data, u64 len, bool isHead = true);

/**
 * 同步内存中数据到硬盘中
 */
void sync();

#endif //FS_H