/*****************************************************
 Author: 张志浩
 Mail: 791745123@qq.com
 Time: 2019-1-20
 Function:
	远程开关机工具,需要对目标主机作如下设置

 远程开机：
	1、BIOS设置
	（DELL）：
		system setup->BIOS设置->Power Management->设置Deep Sleep Control为Disable Wake on Lan为LAN only
	（ASUS）：
		Advance->APM->开启【Power On By PCIE】（PCIE: PCI-Ethernet）
	2、系统设置
		step one：设备管理器->网络适配器->选择对应网卡设备, 属性->高级->开启【Wake on Magic Packet】->开启【Wake			on Pattern match】->开启【关机 网络唤醒】->关闭【环保节能】

		step two：设备管理器->网络适配器->选择对应网卡设备, 属性->电源管理->关闭【允许计算机关闭此设备以节约电源】

 远程关机：
	1、启动Guest账户
		管理->计算机管理->本地用户和组->用户->选择Guest进入属性->关闭【账户已停用】

	2、允许Guest用户从网络访问此计算机
		本地安全属性->用户权限分配->从【拒绝从网络访问这台计算机】中删除Guest

	3、允许Guest用户从远端系统强制关机
		本地安全属性->用户权限分配->将Guest加入到【从远端系统强制关机】

 Version: v 1.0
*****************************************************/
#pragma once

#include <Windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "advapi32.lib")

class CRemoteControl
{
public:
	CRemoteControl(void);
	~CRemoteControl(void);


public:
	static int Startup(char *chMac);
	static int Shutdown(char *chIP, int iTimeOut, bool bForceAppsClosed, bool bRebootAfterShutDown);
};

