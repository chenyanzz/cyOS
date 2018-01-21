#pragma once


#include "types.h"
#include "basic_io.h"

#define ROOT_LBA (130)

#define name_size (31)
#define folder_name_size (name_size)
#define file_name_size (name_size)

#define file_data_size (470)

#define folder_child_size (57)

#define root_dir_name "cyOS"

///文件头部的属性
struct fsAttr
{
	bool isNull : 1;///<此节点是否存在
	bool isFolder : 1;///<是否是目录节点
	bool isTail : 1;///<是否是链表尾
	bool isHead : 1;///<是否是头结点
	bool isHidden : 1;///<是否是隐藏属性
};

///目录节点
struct iFolder
{
	fsAttr attr;///<属性

	char name[name_size];///<名称

	lba48 parent;///<父目录
	lba48 next;///<链表的下一节点

	u64 usage;///<使用情况 @todo还没弄好

	lba48 childs[folder_child_size]; //iFolder & iFile 目录下面的孩子 @todo 删除怎么办？！
};

///文件节点
struct iFile
{
	fsAttr attr;///<文件属性

	char name[name_size];///<名称

	lba48 next;///<链表的下一个节点

	word nData; ///<count 使用了的字节数

	byte data[file_data_size];///<数据
};

/*@todo 下面这两个的功能还没实现*/

///目录描述表
struct FOLDER
{
	lba48 lba;
	// iFolder* ifolder;
	// iFolder* parent;
	byte status;
	char name[31];
};

///文件描述表
struct FILE
{
	lba48 lba;
	// iFile* ifile;
	// iFolder* ifolder;
	byte status;
	char name[31];
};

/**
 * 初始化文件系统
 * 读取root扇区
 */
bool init_fs();

/**
 * 格式化磁盘
 */
void format_disk();

/**
 * 写节点
 * @tparam DISK_NODE iFolder or iFile
 * @param dn  那个节点
 * @param lba 扇区号
 */
template<typename DISK_NODE> inline 
void disk_node_write(DISK_NODE &dn, lba48 lba);

/**
 * 读节点
 * @tparam DISK_NODE iFolder or iFile
 * @param dn  那个节点
 * @param lba 扇区号
 */
template<typename DISK_NODE> inline 
void disk_node_read(DISK_NODE &dn, lba48 lba);

/**
 * 创建文件
 * @param  isHead: 是否是独立文件（因为有可能是新建一个.next）
 * @todo 还没能把文件加入到目录里
 */
FILE create_file(char *name, void *data, u64 len, bool isHead = true);

/**
 * 同步内存中数据到硬盘中
 */
void sync();

