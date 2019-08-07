/*!
 * @file TCP.h
 * @brief ����SOCKETͨ�ŵ�TCP/IPͨ��
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
	 * @brief ��ȡԶ��IP��ַ�Ͷ˿ں�
	 * @param const SOCKET socketͨ��������
	 * @param char* IP��ַ
	 * @param unsigned short& �˿�
	 * @return bool ��ȡ�ɹ�����true,���򷵻�false
	 */
	static bool GetPeerIPandPort(const SOCKET socket, char* lpszIP, unsigned short& usPort);

	/*!
	 * @brief ��ȡԶ��IP��ַ�Ͷ˿ں�
	 * @param const SOCKET socketͨ��������
	 * @param string& IP��ַ
	 * @param unsigned short& �˿�
	 * @return bool ��ȡ�ɹ�����true,���򷵻�false
	 */
	static bool GetPeerIPandPort(const SOCKET socket, std::string& lpszIP, unsigned short& usPort);

	/*!
	 * @brief ��ȡ����IP��ַ�Ͷ˿ں�
	 * @param const SOCKET socketͨ��������
	 * @param char* IP��ַ
	 * @param unsigned short& �˿�
	 * @return bool ��ȡ�ɹ�����true,���򷵻�false
	*/
	static bool GetSockIPandPort(const SOCKET socket, char* lpszIP, unsigned short& usPort);

	/*!
	 * @brief ��ȡ����IP��ַ�Ͷ˿ں�
	 * @param const SOCKET socketͨ��������
	 * @param string& IP��ַ
	 * @param unsigned short& �˿�
	 * @return bool ��ȡ�ɹ�����true,���򷵻�false
	*/
	static bool GetSockIPandPort(const SOCKET socket, std::string& lpszIP, unsigned short& usPort);

	/*!
	 * @brief ��ȡ���������IP
	 * @param char* ����IP��ַ�ַ���
	 * @param int �ƶ��˿ں� ֵΪ-1��ָ��
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	static bool GetLocalIP(char* lpszIP, int nPort = -1);

	/*!
	 * @brief ��DWORD��ֵת��ΪIP��ַ�ַ���
	 * @param DWORD IP��ַ
	 * @return string IP��ַ�ַ���
	*/
	static const std::string DWORDToIPAddress(const DWORD dwIP);

	/*!
	 * @breif ��IP��ַ�ַ���ת��ΪDWORDֵ
	 * @param const char* IP��ַ�ַ���
	 * @return DWORD IP��ַ
	*/
	static const DWORD IPAddressToDWORD(const char* strIP);
};

/*!
 * @brief ���ݴ���ص�����
 * @param const char* ���յ����������׵�ַ
 * @param unsigned short ʵ�ʽ��յ������ݳ���
 * @param void* ���ݴ������ָ��
*/
typedef void(__stdcall* ProcessData)(const char*, unsigned short, void*);

/*!
 * @brief ���տͻ�������ص�����
 * @param SOCKET �ͻ�������SOCKETͨ��
 * @param void* ��������Ķ���ָ��
*/
typedef void(__stdcall* AcceptClient)(SOCKET, void*);

/*!
 * @class SocketClient
 * @brief ����SOCKETͨ�ŵ�TCP�ͻ��˶���
*/
typedef class SocketClient
{
public:
	SocketClient();
	~SocketClient();

protected:
	std::string m_strPeerIP;			/*!< Զ��IP */
	int m_nPeerPort;					/*!< Զ�̶˿� */
	unsigned short m_usPeerPort;		/*!< ���ӳɹ���ʵ�ʵ�Զ�̶˿� */

	ULL m_ullReceiveTime;				/*!< ���ݽ��յ�ʱ�䣨ϵͳ����ʱ�䣩 */
	ULL m_ullSendTime;					/*!< ���ݷ��͵�ʱ�䣨ϵͳ����ʱ�䣩 */

private:
	std::thread* m_pReceiveThread;		/*!< ���ݽ����߳�ָ�� */
	ProcessData m_pFuncProcessData;		/*!< ���ݴ���ص�����ָ�� */
	void* m_pProcesser;					/*!< ���ݴ������ָ�� */
	std::thread* m_pConnectThread;		/*!< ���ӷ�����߳� */
	HANDLE m_hShutdownEvent;			/*!< �̹߳ر��¼� */

	mutex m_mutex;
	SOCKET m_socket;					/*!< �ͻ��˰�SOCKETͨ�� */

public:
	/*!
	 * @brief ��ʼ��
	 * @param const char* IP��ַ
	 * @param int �˿�
	 * @param ProcessData ���ݴ���ص�����ָ��
	 * @param void* ���ݴ������ָ��
	*/
	void Init(const char* ip,int port,ProcessData pFunc,void* pProcesser);

	/*!
	 * @brief ���ӷ����
	 * @param ProcessData ���ݴ���ص�����ָ��
	 * @param void* ���ݴ������ָ��
	 * @return bool ���ӳɹ�����true,���򷵻�false
	*/
	bool Connect(ProcessData pFunc,void* pProcesser);

	/*!
	 * @brief ���տͻ�������
	 * @param SOCKET  �ͻ���SOCKETͨ��
	 * @param ProcessData ���ݴ���ص�����ָ��
	 * @param void* ���ݴ������ָ��
	 * @return bool �ɹ�����true,���򷵻�false
	*/
	bool Accept(SOCKET socket, ProcessData pFunc, void* pProcesser);

	/*!
	 * @brief �Ƿ������ӿͻ���
	 * @return bool �����ӷ���true,���򷵻�false
	*/
	bool IsConnected();

	/*!
	 * @brief ��������
	 * @param char* ���������׵�ַָ��
	 * @param unsigned int ���ݳ���
	 * @return bool ���ͳɹ�����true,���򷵻�false
	*/
	bool Send(const char* data,unsigned int len);

	/*!
	 * @brief ��������
	 * @param const char* �ַ����׵�ַ
	 * @return bool ���ͳɹ�����true,���򷵻�false
	*/
	bool Send(const char* str);

	/*!
	 * @brief ����IP�Ͷ˿�
	 * @param const char* IP��ַ
	 * @param int �˿�
	*/
	void SetIPandPort(const char* ip,int port);

	/*!
	 * @brief ��ȡIP��ַ
	 * @param const char* IP��ַ
	 * @param int& �˿�
	*/
	void GetIPandPort(char* ip,int& port);
	
	/*!
	 * @brief ��ȡIP��ַ
	 * @param string& IP��ַ
	 * @param int& �˿�
	*/
	void GetIPandPort(std::string& ip, int& port);

	/*!
	 * @brief �������ӷ�����߳�
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool StartConnectThread();

	/*!
	 * @brief �ر����ӷ�����߳�
	*/
	void CloseConnectThread();

private:
	/*!
	 * @brief ���ݽ����߳�����
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool StartReceiveThread();

	/*!
	 * @brief ���ݽ����߳���ֹ
	*/
	void EndReceiveThread();

	/*!
	 * @brief ���ݽ����߳�
	*/
	void ReceiveThread();

	/*!
	 * @brief ����������߳�
	*/
	void ConnectThread();
} TCPClient;

/*!
 * @class SockerServer
 * @brief ����SOCKETͨ�ŵ�TCP����˶���
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
	SOCKET m_socket;					/*!< �����SOCKETͨ�� */
	std::string m_strIP;				/*!< ����˰�IP��ַ */
	unsigned short m_usPort;			/*!< ����˰󶨶˿� */
	std::thread *m_pAcceptThread;		/*!< ���ݽ����߳�ָ�� */
	unsigned long m_ulThreadID;			/*!< �߳�ID */
	UpdateServer m_funcPrt;
	void* m_funcParamPrt;

public:
	/*!
	 * @brief ��������˼���
	 * @param unsigned long �߳�ID
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool StartListen(UpdateServer funcPrt = nullptr, void* ptr = nullptr,const unsigned long ulThreadID = 0);

	/*!
	 * @brief ��������˼���
	 * @param const unsigned short ����˶˿�
	 * @param const char* ����˰�IP
	 * @param unsigned long �߳�ID
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool StartListen(const unsigned short usPort, const char* strIP, UpdateServer funcPrt = nullptr, void* ptr = nullptr, const unsigned long ulThreadID = 0);

	/*!
	 * @brief ��������˼���
	 * @param const unsigned short ����˶˿�
	 * @param const wchar_t* ����˰�IP
	 * @param unsigned long �߳�ID
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool StartListen(const unsigned short usPort, const wchar_t* wstrIP, UpdateServer funcPrt = nullptr, void* ptr = nullptr, const unsigned long ulThreadID = 0);

	/*!
	 * @brief ��������˼���
	 * @param const unsigned short ����˶˿�
	 * @param string ����˰�IP
	 * @param unsigned long �߳�ID
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool StartListen(const unsigned short usPort, std::string strIP = "", UpdateServer funcPrt = nullptr, void* ptr = nullptr, const unsigned long ulThreadID = 0);

	/*!
	 * @brief ��������˼���
	 * @param const unsigned short ����˶˿�
	 * @param wstring ����˰�IP
	 * @param unsigned long �߳�ID
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool StartListen(const unsigned short usPort, std::wstring wstrIP = L"", UpdateServer funcPrt = nullptr, void* ptr = nullptr, const unsigned long ulThreadID = 0);

	/*!
	 * @brief �رշ���˼���
	*/
	void CloseListen();

	/*!
	 * @brief �Ƿ��Ѽ���
	 * @return bool �Ѽ�������true,���򷵻�false
	*/
	bool IsListened();

	/*!
	 * @brief ��IP�Ͷ˿�
	 * @param unsigned short �˿�
	 * @param const char* IP��ַ
	*/
	void Bind(unsigned short usPort,const char* strIP);

	/*!
	 * @brief ��IP�Ͷ˿�
	 * @param unsigned short �˿�
	 * @param const wchar_t* IP��ַ
	*/
	void Bind(unsigned short usPort, const wchar_t* wstrIP);

	/*!
	 * @brief ��IP�Ͷ˿�
	 * @param unsigned short �˿�
	 * @param string IP��ַ
	*/
	void Bind(unsigned short usPort, std::string strIP = "");

	/*!
	 * @brief ��IP�Ͷ˿�
	 * @param unsigned short �˿�
	 * @param wstring IP��ַ
	*/
	void Bind(unsigned short usPort, std::wstring wstrIP);

	/*!
	 * @brief ��ȡIP��ַ�Ͷ˿�
	 * @param char* �����IP��ַ
	 * @param unsigned short& ����˶˿�
	*/
	void GetIPandPort(char* strIP,unsigned short& usPort) const;

	/*!
	 * @brief ��ȡIP��ַ�Ͷ˿�
	 * @param wchar_t* �����IP��ַ
	 * @param unsigned short& ����˶˿�
	*/
	void GetIPandPort(wchar_t* wstrIP, unsigned short& usPort) const;

	/*!
	 * @brief ��ȡIP��ַ�Ͷ˿�
	 * @param string& �����IP��ַ
	 * @param unsigned short& ����˶˿�
	*/
	void GetIPandPort(std::string& strIP, unsigned short& usPort) const;

	/*!
	 * @brief ��ȡIP��ַ�Ͷ˿�
	 * @param wstring& �����IP��ַ
	 * @param unsigned short& ����˶˿�
	*/
	void GetIPandPort(std::wstring& wstrIP, unsigned short& usPort) const;

protected:
	/*!
	 * @brief �����ͻ��˽����߳�
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool StartAcceptThread();

	/*!
	 * @brief �رտͻ��˽����߳�
	*/
	void EndAcceptThread();

	/*!
	 * @brief �ͻ��˽����߳�
	*/
	void AcceptThread();

	/*!
	 * @brief �ͻ��˽���
	 * @param const SOCKET �ͻ���SOCKETͨ��
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

	//�����׽��ֻ�ȡ��ַ��Ϣ
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

	//�����׽��ֻ�ȡ��ַ��Ϣ
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
	//1.��ʼ��wsa  
	WSADATA wsaData;
	int ret = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		return false;
	}

	//2.��ȡ������  
	char hostname[256];
	ret = ::gethostname(hostname, sizeof(hostname));
	if (ret == SOCKET_ERROR)
	{
		return false;
	}

	//3.��ȡ����ip  
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