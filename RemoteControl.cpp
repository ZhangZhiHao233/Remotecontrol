#include "RemoteControl.h"


CRemoteControl::CRemoteControl(void)
{

}


CRemoteControl::~CRemoteControl(void)
{
}

int CRemoteControl::Startup(char *chMac)
{
	//判断Mac地址格式
	for (char *a=chMac; *a; a++)
	{
		if (*a != '-' && !isxdigit(*a))
		{
			OutputDebugStringA("Mac address must be like this: 00-d0-4c-bf-52-ba");
			return -1;
		}
	}

	//字符串转数组
	int dstaddr[6];
	int i = sscanf(chMac, "%2x-%2x-%2x-%2x-%2x-%2x", &dstaddr[0], &dstaddr[1], &dstaddr[2], &dstaddr[3]
	, &dstaddr[4], &dstaddr[5]);


	if (i != 6)
	{
		OutputDebugStringA("Invalid mac address!");
		return -1;
	}

	//构造Magic Packet (包格式: "FFFFFFFFFFFF" + 重复16编mac地址)
	unsigned char ether_addr[6];
	for (i=0; i<6; i++)
	{
		ether_addr[i] = dstaddr[i];
	}

	u_char magicpacket[200];
	memset(magicpacket, 0xff, 6);
	int packetsize = 6;
	for (i=0; i<16; i++)
	{
		memcpy(magicpacket+packetsize, ether_addr, 6);
		packetsize += 6;
	}

	//创建广播套接字
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
	{
		OutputDebugStringA("WSAStartup failed");
		return -1;
	}

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		OutputDebugStringA("Socket Create error");
		return -1;
	}

	BOOL bOptval = TRUE;
	int iOptLen = sizeof(BOOL);
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bOptval, iOptLen) == SOCKET_ERROR)
	{
		OutputDebugStringA("setsocketopt error!");
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	sockaddr_in to;
	to.sin_family = AF_INET;
	to.sin_port = htonl(0);
	to.sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST);

	//发送魔法包
	if (sendto(sock, (const char*)magicpacket, packetsize, 0, (const struct sockaddr*)&to, sizeof(to)) == SOCKET_ERROR)
	{
		OutputDebugStringA("Send error!");
	}
	else
	{
		OutputDebugStringA("Send success!");
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}

int CRemoteControl::Shutdown(char *chIP, int iTimeOut, bool bForceAppsClosed, bool bRebootAfterShutDown)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	BOOL fResult;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);  
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;  
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);  
	if (GetLastError() != ERROR_SUCCESS)  
		return false;

	WCHAR wszIP[20];
	memset(wszIP, 0, sizeof(wszIP));
	MultiByteToWideChar(CP_ACP, 0, chIP, strlen(chIP)+1, wszIP, sizeof(wszIP)/sizeof(wszIP[0]));

	//发送指令，iTimeOut如果小于60，则对方机器关机前会显示“一分钟内关机”
	fResult = ::InitiateSystemShutdown(wszIP, NULL, iTimeOut, bForceAppsClosed, bRebootAfterShutDown);
	if (!fResult)
		return false;

	tkp.Privileges[0].Attributes = 0;
	AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if(GetLastError() != ERROR_SUCCESS)
		return false;

	return true;
}
