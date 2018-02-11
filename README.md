# cyOS


## 项目简介

这是一个岩岩小天使正在做的操作系统【有参考linux】  
目前正在超级超级  **初级**  开发阶段.....

### done-list

- 进入main函数之前的引导部分
- xmake自动化编译
- 屏幕输出相关stdio函数
- 用户态双线程切换
- 基于vscode调试代码
- 基于lba的扇区读写
- timer (irq0)
- 获取键盘输入 (irq1)
- 简易内存页管理（getFreePage）
- 基于timer的多线程切换
- 使用clion+qemu调试代码
- 3种timer（APIC PIT RTC）
- 获取系统时间
- 实现grub引导

### todo-list

近期
- 读取grub数据
- scanf函数

长期
- 完善文件系统
- 交互界面
- malloc

## 依赖软件

- xmake 用做构建系统
- mingw,nasm 用来编译生成
- qemu + clion 调试真是绝配
- 引导：一个带有grub引导的U盘+ubuntu虚拟机 暂时没找到更好的方案
- 一款好用的开发环境，我用的是vscode+clion


## 关于作者

1. 	陈岩cy  
	QQ/微信：740310894  
	是一个正在广中念初三的小天使  
2. 	王骁扬wxy  
	${这个人很懒，没有写简介}


欢迎感兴趣人士一同研究，一同开发  
@xy[Smirk]  
正准备搞个工作室，一起写点超级小项目（web app，游戏之类的）