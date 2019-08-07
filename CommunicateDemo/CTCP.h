/*!
 * @file TCP.h
 * @brief 基于SOCKET通信的TCP/IP通信
 * @author FanKaiyu
 * @date 2018-03-24
 * @version 2.2
 * 
 * @update FanKaiyu 2019-06-06
*/

#pragma once
#ifndef _TCP_H
#define _TCP_H

#ifdef _WIN32
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <WinDNS.h>

#pragma comment(lib, "WS2_32")
#else
#include <netdb.h>
#endif
#include <sstream>
#include <string>
#include <thread>
#include <mutex>
#include <set>

#ifdef _WIN64
#ifndef ULL
#define ULL unsigned long long
#endif //!ULL
#ifndef GetTickCountEX
#define GetTickCountEX() GetTickCount64()
#endif //!GetTickCountEX()
#else
#ifndef ULL
#define ULL unsigned long
#endif //!ULL
#ifndef GetTickCountEX
#define GetTickCountEX() GetTickCount()
#endif//!GetTickCountEX()
#endif//_WIN64

namespace TCP
{
	/*!
	 * @brief 获取远程IP地址和端口号
	 * @param const SOCKET socket通道描述符
	 * @param char* IP地址
	 * @param unsigned short& 端口
	 * @return bool 获取成功返回true,否则返回false
	 */
	static bool GetPeerIPandPort(const SOCKET socket, char* lpszIP, unsigned short& usPort);

	/*!
	 * @brief 获取远程IP地址和端口号
	 * @param const SOCKET socket通道描述符
	 * @param string& IP地址
	 * @param unsigned short& 端口
	 * @return bool 获取成功返回true,否则返回false
	 */
	static bool GetPeerIPandPort(const SOCKET socket, std::string& lpszIP, unsigned short& usPort);

	/*!
	 * @brief 获取本地IP地址和端口号
	 * @param const SOCKET socket通道描述符
	 * @param char* IP地址
	 * @param unsigned short& 端口
	 * @return bool 获取成功返回true,否则返回false
	*/
	static bool GetSockIPandPort(const SOCKET socket, char* lpszIP, unsigned short& usPort);

	/*!
	 * @brief 获取本地IP地址和端口号
	 * @param const SOCKET socket通道描述符
	 * @param string& IP地址
	 * @param unsigned short& 端口
	 * @return bool 获取成功返回true,否则返回false
	*/
	static bool GetSockIPandPort(const SOCKET socket, std::string& lpszIP, unsigned short& usPort);

	/*!
	 * @brief 获取计算机本地IP
	 * @param char* 本地IP地址字符串
	 * @param int 制定端口号 值为-1则不指定
	 * @return bool 成功返回true,失败返回false
	*/
	static bool GetLocalIP(char* lpszIP, int nPort = -1);

	/*!
	 * @brief 将DWORD的值转换为IP地址字符串
	 * @param DWORD IP地址
	 * @return string IP地址字符串
	*/
	static const std::string DWORDToIPAddress(const DWORD dwIP);

	/*!
	 * @breif 将IP地址字符串转换为DWORD值
	 * @param const char* IP地址字符串
	 * @return DWORD IP地址
	*/
	static const DWORD IPAddressToDWORD(const char* strIP);
};

/*!
 * @brief 数据处理回调函数
 * @param const char* 接收的数据数组首地址
 * @param unsigned short 实际接收到的数据长度
 * @param void* 数据处理对象指针
*/
typedef void(__stdcall* ProcessData)(const char*, unsigned short, void*);

/*!
 * @brief 接收客户端请求回调函数
 * @param SOCKET 客户端请求SOCKET通道
 * @param void* 接收请求的对象指针
*/
typedef void(__stdcall* AcceptClient)(SOCKET, void*);

/*!
 * @class SocketClient
 * @brief 基于SOCKET通信的TCP客户端对象
*/
typedef class SocketClient
{
public:
	SocketClient();
	~SocketClient();

protected:
	std::string m_strPeerIP;			/*!< 远程IP */
	int m_nPeerPort;					/*!< 远程端口 */
	unsigned short m_usPeerPort;		/*!< 连接成功后实际的远程端口 */

	ULL m_ullReceiveTime;				/*!< 数据接收的时间（系统运行时间） */
	ULL m_ullSendTime;					/*!< 数据发送的时间（系统运行时间） */

private:
	std::thread* m_pReceiveThread;		/*!< 数据接收线程指针 */
	ProcessData m_pFuncProcessData;		/*!< 数据处理回调函数指针 */
	void* m_pProcesser;					/*!< 数据处理对象指针 */
	std::thread* m_pConnectThread;		/*!< 连接服务端线程 */
	HANDLE m_hShutdownEvent;			/*!< 线程关闭事件 */

	mutex m_mutex;
	SOCKET m_socket;					/*!< 客户端绑定SOCKET通道 */

public:
	/*!
	 * @brief 初始化
	 * @param const char* IP地址
	 * @param int 端口
	 * @param ProcessData 数据处理回调函数指针
	 * @param void* 数据处理对象指针
	*/
	void Init(const char* ip,int port,ProcessData pFunc,void* pProcesser);

	/*!
	 * @brief 连接服务端
	 * @param ProcessData 数据处理回调函数指针
	 * @param void* 数据处理对象指针
	 * @return bool 连接成功返回true,否则返回false
	*/
	bool Connect(ProcessData pFunc,void* pProcesser);

	/*!
	 * @brief 接收客户端请求
	 * @param SOCKET  客户端SOCKET通道
	 * @param ProcessData 数据处理回调函数指针
	 * @param void* 数据处理对象指针
	 * @return bool 成功返回true,否则返回false
	*/
	bool Accept(SOCKET socket, ProcessData pFunc, void* pProcesser);

	/*!
	 * @brief 是否已连接客户端
	 * @return bool 已连接返回true,否则返回false
	*/
	bool IsConnected();

	/*!
	 * @brief 发送数据
	 * @param char* 数据数组首地址指针
	 * @param unsigned int 数据长度
	 * @return bool 发送成功返回true,否则返回false
	*/
	bool Send(const char* data,unsigned int len);

	/*!
	 * @brief 发送数据
	 * @param const char* 字符串首地址
	 * @return bool 发送成功返回true,否则返回false
	*/
	bool Send(const char* str);

	/*!
	 * @brief 设置IP和端口
	 * @param const char* IP地址
	 * @param int 端口
	*/
	void SetIPandPort(const char* ip,int port);

	/*!
	 * @brief 获取IP地址
	 * @param const char* IP地址
	 * @param int& 端口
	*/
	void GetIPandPort(char* ip,int& port);
	
	/*!
	 * @brief 获取IP地址
	 * @param string& IP地址
	 * @param int& 端口
	*/
	void GetIPandPort(std::string& ip, int& port);

	/*!
	 * @brief 启动连接服务端线程
	 * @return bool 启动成功返回true,否则返回false
	*/
	bool StartConnectThread();

	/*!
	 * @brief 关闭连接服务端线程
	*/
	void CloseConnectThread();

private:
	/*!
	 * @brief 数据接收线程启动
	 * @return bool 启动成功返回true,否则返回false
	*/
	bool StartReceiveThread();

	/*!
	 * @brief 数据接收线程终止
	*/
	void EndReceiveThread();

	/*!
	 * @brief 数据接收线程
	*/
	void ReceiveThread();

	/*!
	 * @brief 服务端连接线程
	*/
	void ConnectThread();
} TCPClient;

/*!
 * @class SockerServer
 * @brief 基于SOCKET通信的TCP服务端对象
*/
typedef class SocketServer
{
public:
	SocketServer();
	SocketServer(const unsigned short usPort, const char* strIP, UpdateServer funcPrt = nullptr, void* ptr = nullptr);
	SocketServer(const unsigned short usPort, const wchar_t* wstrIP, UpdateServer funcPrt = nullptr, void* ptr = nullptr);
	SocketServer(const unsigned short usPort, std::string strIP = "", UpdateServer funcPrt = nullptr, void* ptr = nullptr);
	SocketServer(const unsigned short usPort, std::wstring wstrIP, UpdateServer funcPrt = nullptr, void* ptr = nullptr);
	~SocketServer();

protected:
	SOCKET m_socket;					/*!< 服务端SOCKET通道 */
	std::string m_strIP;				/*!< 服务端绑定IP地址 */
	unsigned short m_usPort;			/*!< 服务端绑定端口 */
	std::thread *m_pAcceptThread;		/*!< 数据接收线程指针 */
	unsigned long m_ulThreadID;			/*!< 线程ID */
	UpdateServer m_funcPrt;
	void* m_funcParamPrt;

public:
	/*!
	 * @brief 启动服务端监听
	 * @param unsigned long 线程ID
	 * @return bool 启动成功返回true,否则返回false
	*/
	bool StartListen(UpdateServer funcPrt = nullptr, void* ptr = nullptr,const unsigned long ulThreadID = 0);

	/*!
	 * @brief 启动服务端监听
	 * @param const unsigned short 服务端端口
	 * @param const char* 服务端绑定IP
	 * @param unsigned long 线程ID
	 * @return bool 启动成功返回true,否则返回false
	*/
	bool StartListen(const unsigned short usPort, const char* strIP, UpdateServer funcPrt = nullptr, void* ptr = nullptr, const unsigned long ulThreadID = 0);

	/*!
	 * @brief 启动服务端监听
	 * @param const unsigned short 服务端端口
	 * @param const wchar_t* 服务端绑定IP
	 * @param unsigned long 线程ID
	 * @return bool 启动成功返回true,否则返回false
	*/
	bool StartListen(const unsigned short usPort, const wchar_t* wstrIP, UpdateServer funcPrt = nullptr, void* ptr = nullptr, const unsigned long ulThreadID = 0);

	/*!
	 * @brief 启动服务端监听
	 * @param const unsigned short 服务端端口
	 * @param string 服务端绑定IP
	 * @param unsigned long 线程ID
	 * @return bool 启动成功返回true,否则返回false
	*/
	bool StartListen(const unsigned short usPort, std::string strIP = "", UpdateServer funcPrt = nullptr, void* ptr = nullptr, const unsigned long ulThreadID = 0);

	/*!
	 * @brief 启动服务端监听
	 * @param const unsigned short 服务端端口
	 * @param wstring 服务端绑定IP
	 * @param unsigned long 线程ID
	 * @return bool 启动成功返回true,否则返回false
	*/
	bool StartListen(const unsigned short usPort, std::wstring wstrIP = L"", UpdateServer funcPrt = nullptr, void* ptr = nullptr, const unsigned long ulThreadID = 0);

	/*!
	 * @brief 关闭服务端监听
	*/
	void CloseListen();

	/*!
	 * @brief 是否已监听
	 * @return bool 已监听返回true,否则返回false
	*/
	bool IsListened();

	/*!
	 * @brief 绑定IP和端口
	 * @param unsigned short 端口
	 * @param const char* IP地址
	*/
	void Bind(unsigned short usPort,const char* strIP);

	/*!
	 * @brief 绑定IP和端口
	 * @param unsigned short 端口
	 * @param const wchar_t* IP地址
	*/
	void Bind(unsigned short usPort, const wchar_t* wstrIP);

	/*!
	 * @brief 绑定IP和端口
	 * @param unsigned short 端口
	 * @param string IP地址
	*/
	void Bind(unsigned short usPort, std::string strIP = "");

	/*!
	 * @brief 绑定IP和端口
	 * @param unsigned short 端口
	 * @param wstring IP地址
	*/
	void Bind(unsigned short usPort, std::wstring wstrIP);

	/*!
	 * @brief 获取IP地址和端口
	 * @param char* 服务端IP地址
	 * @param unsigned short& 服务端端口
	*/
	void GetIPandPort(char* strIP,unsigned short& usPort) const;

	/*!
	 * @brief 获取IP地址和端口
	 * @param wchar_t* 服务端IP地址
	 * @param unsigned short& 服务端端口
	*/
	void GetIPandPort(wchar_t* wstrIP, unsigned short& usPort) const;

	/*!
	 * @brief 获取IP地址和端口
	 * @param string& 服务端IP地址
	 * @param unsigned short& 服务端端口
	*/
	void GetIPandPort(std::string& strIP, unsigned short& usPort) const;

	/*!
	 * @brief 获取IP地址和端口
	 * @param wstring& 服务端IP地址
	 * @param unsigned short& 服务端端口
	*/
	void GetIPandPort(std::wstring& wstrIP, unsigned short& usPort) const;

protected:
	/*!
	 * @brief 启动客户端接收线程
	 * @return bool 启动成功返回true,否则返回false
	*/
	bool StartAcceptThread();

	/*!
	 * @brief 关闭客户端接收线程
	*/
	void EndAcceptThread();

	/*!
	 * @brief 客户端接收线程
	*/
	void AcceptThread();

	/*!
	 * @brief 客户端接收
	 * @param const SOCKET 客户端SOCKET通道
	*/
	void AcceptClient(const SOCKET socket);

} TCPServer;

bool TCP::GetPeerIPandPort(const SOCKET socket, char* lpszIP, unsigned short& usPort)
{
	if (socket == INVALID_SOCKET)
	{
		return false;
	}

	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);

	//根据套接字获取地址信息
	if (::getpeername(socket, (SOCKADDR*)& addrClient, &addrClientlen) != 0)
	{
		return false;
	}

	if (inet_ntop(AF_INET, (void*)& addrClient.sin_addr, lpszIP, IP4_ADDRESS_STRING_BUFFER_LENGTH) == nullptr)
	{
		perror("fail to convert");
		return false;
	}

	usPort = htons(addrClient.sin_port);

	return true;
}

bool TCP::GetPeerIPandPort(const SOCKET socket, std::string& lpszIP, unsigned short& usPort)
{
	char _ip[IP4_ADDRESS_STRING_BUFFER_LENGTH];

	if (GetPeerIPandPort(socket, _ip, usPort) == false)
	{
		return false;
	}

	lpszIP = _ip;

	return true;
}

bool TCP::GetSockIPandPort(const SOCKET socket, char* lpszIP, unsigned short& usPort)
{
	if (socket == INVALID_SOCKET)
	{
		return false;
	}

	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);

	//根据套接字获取地址信息
	if (::getsockname(socket, (SOCKADDR*)& addrClient, &addrClientlen) != 0)
	{
		return false;
	}

	if (inet_ntop(AF_INET, (void*)& addrClient.sin_addr, lpszIP, IP4_ADDRESS_STRING_BUFFER_LENGTH) == nullptr)
	{
		perror("fail to convert");
		return false;
	}

	usPort = htons(addrClient.sin_port);

	return true;
}

bool TCP::GetSockIPandPort(const SOCKET socket, std::string& lpszIP, unsigned short& usPort)
{
	char _ip[IP4_ADDRESS_STRING_BUFFER_LENGTH];

	if (GetSockIPandPort(socket, _ip, usPort) == false)
	{
		return false;
	}

	lpszIP = _ip;

	return true;
}

bool TCP::GetLocalIP(char* lpszIP, int nPort = -1)
{
	//1.初始化wsa  
	WSADATA wsaData;
	int ret = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		return false;
	}

	//2.获取主机名  
	char hostname[256];
	ret = ::gethostname(hostname, sizeof(hostname));
	if (ret == SOCKET_ERROR)
	{
		return false;
	}

	//3.获取主机ip  
	struct addrinfo hints;
	struct addrinfo* res, * cur;
	struct sockaddr_in* addr;
	// char lpszIP[IP4_ADDRESS_STRING_BUFFER_LENGTH];

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;		/* Allow IPv4 */
	hints.ai_flags = AI_PASSIVE;	/* For wildcard IP address */
	hints.ai_protocol = 0;			/* Any protocol */
	hints.ai_socktype = SOCK_STREAM;

	char lpszPort[6];
	if (nPort > -1)
	{
		sprintf_s(lpszPort, 6, "%d", nPort);
		getaddrinfo(hostname, lpszPort, &hints, &res);
	}
	else
	{
		getaddrinfo(hostname, NULL, &hints, &res);
	}

	if (ret == -1)
	{
		perror("getaddrinfo");
		return false;
	}

	for (cur = res; cur != NULL; cur = cur->ai_next)
	{
		addr = (struct sockaddr_in*)cur->ai_addr;

		sprintf_s(lpszIP, IP4_ADDRESS_STRING_BUFFER_LENGTH, "%d.%d.%d.%d",
			(*addr).sin_addr.S_un.S_un_b.s_b1,
			(*addr).sin_addr.S_un.S_un_b.s_b2,
			(*addr).sin_addr.S_un.S_un_b.s_b3,
			(*addr).sin_addr.S_un.S_un_b.s_b4);
	}

	freeaddrinfo(res);

	WSACleanup();

	return true;
}

const std::string TCP::DWORDToIPAddress(const DWORD dwIP)
{
	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = htonl(dwIP);

	char lpszIP[IP4_ADDRESS_STRING_BUFFER_LENGTH];
	if (inet_ntop(AF_INET, (void*)& addr.sin_addr, lpszIP, IP4_ADDRESS_STRING_BUFFER_LENGTH) == nullptr)
	{
		perror("fail to convert");
		return "";
	}

	return lpszIP;
}

const DWORD TCP::IPAddressToDWORD(const char* strIP)
{
	SOCKADDR_IN addr;
	if (strcmp(strIP, "") == 0)
	{
		inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr.s_addr);
	}
	else
	{
		inet_pton(AF_INET, strIP, &addr.sin_addr.s_addr);
	}

	return htonl(addr.sin_addr.s_addr);
}

#endif //!_TCP_H