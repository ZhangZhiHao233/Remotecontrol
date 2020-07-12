/*****************************************************
 Author: ��־��
 Mail: 791745123@qq.com
 Time: 2019-1-20
 Function:
	Զ�̿��ػ�����,��Ҫ��Ŀ����������������

 Զ�̿�����
	1��BIOS����
	��DELL����
		system setup->BIOS����->Power Management->����Deep Sleep ControlΪDisable Wake on LanΪLAN only
	��ASUS����
		Advance->APM->������Power On By PCIE����PCIE: PCI-Ethernet��
	2��ϵͳ����
		step one���豸������->����������->ѡ���Ӧ�����豸, ����->�߼�->������Wake on Magic Packet��->������Wake			on Pattern match��->�������ػ� ���绽�ѡ�->�رա��������ܡ�

		step two���豸������->����������->ѡ���Ӧ�����豸, ����->��Դ����->�رա����������رմ��豸�Խ�Լ��Դ��

 Զ�̹ػ���
	1������Guest�˻�
		����->���������->�����û�����->�û�->ѡ��Guest��������->�رա��˻���ͣ�á�

	2������Guest�û���������ʴ˼����
		���ذ�ȫ����->�û�Ȩ�޷���->�ӡ��ܾ������������̨���������ɾ��Guest

	3������Guest�û���Զ��ϵͳǿ�ƹػ�
		���ذ�ȫ����->�û�Ȩ�޷���->��Guest���뵽����Զ��ϵͳǿ�ƹػ���

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

