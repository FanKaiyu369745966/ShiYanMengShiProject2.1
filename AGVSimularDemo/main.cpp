#pragma once
#include "AGVSimular.h"
#include "AGVSimular.c"
#include <iostream>
#include <string>
#include <thread>
#include "../CommunicateDemo/CTCP.h"
#include "../CommunicateDemo/CTCP.cpp"
#include "../Thread/ThreadEx.h"
#include "../String/DebugPrint.h"
#include "../String/StringEx.h"
#include "../json/json.h"
#include "../json/json_value.cpp"
#include "../json/json_reader.cpp"
#include "../json/json_writer.cpp"

using namespace std;

HANDLE m_hShutdownEvent = NULL;	/*!< �ر��¼� */
thread *m_pThreadMsg = NULL;
thread *m_pThreadReceive = NULL;
SOCKET m_socket = INVALID_SOCKET;
int m_nReceive = 0;				/*!< ��ǰ���յ��ֽ��� */
HANDLE m_hProcess = NULL;
mutex m_lock;

void ProcessData(const unsigned char * pData, const unsigned int unSize)
{
	/*!
	 * �����յ������ݴ���
	 * ���������3��
	 * 1��ͨ����̳�����ProcessData�������ں�����ʵ�ֶ����ݵĴ���
	 * 2��ͨ���ص�����
	 * 3��ͨ���߳���Ϣ����ȫ���߳���Ϣ�ǵ��ֽڴ��ݣ����Ҫһ���Դ��ݴ�����������2�ַ���
	 * 3.1�������ڴ�ռ䣬������ݣ���������ָ�룬�˷������״����ڴ�й©
	 * 3.2��ʹ��SendThreadMessage�����������ݣ�������ݴ����ٶȹ�����Ӱ�����ݵĽ���
	*/

	if (ThreadEx::GetThreadID(m_pThreadMsg) == 0)
	{
		return;
	}

	for (unsigned int unI = 0; unI < unSize; ++unI)
	{
		PostThreadMessage(ThreadEx::GetThreadID(m_pThreadMsg), TM_TCPCLIENT_RECEIVE, *(pData + unI), 1);
	}

	return;
}

void MessageThread()
{
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
			{
				return;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == TM_TCPSERVER_ACCEPT)
			{
				m_socket = msg.wParam;
			}
			else if(msg.message == TM_TCPCLIENT_RECEIVE)
			{
				g_achReceive[m_nReceive] = msg.wParam;
				++m_nReceive;

				int nResult = ProcessData((unsigned char*)g_achReceive, m_nReceive, (unsigned char*)g_achPackage, MAX_PACKAGE_LEN);

				if (nResult > 0)
				{
					memset(g_achReceive, 0, MAX_RECEIVE_LEN);
					m_nReceive = 0;

					SetEvent(m_hProcess);
				}
				else if (nResult == -1)
				{
					memset(g_achReceive, 0, MAX_RECEIVE_LEN);
					m_nReceive = 0;
				}
			}
		}

		if (WaitForSingleObject(m_hShutdownEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
	}

	return;
}

void ReceiveThread()
{
	char achDataBuffer[1500];		/*!< ���ݽ�����*/
	int nBufferLen = 0;				/*!< ���������ݳ���*/

	fd_set fds;

	while (m_socket != INVALID_SOCKET)
	{
		nBufferLen = 0;

		// ��ʼ��������
		memset(achDataBuffer, 0, 1500);

		FD_ZERO(&fds);
		FD_SET(m_socket, &fds);

		switch (select((int)m_socket + 1, &fds, NULL, NULL, NULL))
		{
		case  SOCKET_ERROR:
		{
			if (errno == WSAEINTR)
			{
				continue;
			}

			shutdown(m_socket, SD_BOTH);
			m_socket = INVALID_SOCKET;

			break;
		}
		case 0:
		{
			continue;
		}
		default:
		{
			if (FD_ISSET(m_socket, &fds))
			{
				// ��������
				nBufferLen = recv(m_socket, achDataBuffer, 1500, 0);

				if (SOCKET_ERROR == nBufferLen)
				{
					shutdown(m_socket, SD_BOTH);
					m_socket = INVALID_SOCKET;

					return;
				} // if (SOCKET_ERROR == nBufferLen)

				if (nBufferLen > 0)
				{
					// ��¼���յ�����
					stringstream ssHex; /*!< 16�������� */
					for (int nI = 0; nI < nBufferLen; ++nI)
					{
						ssHex << StringEx::string_format("%02X ", (*(achDataBuffer + nI) & 0xFF));
					}
					char* lpszStr = new char[nBufferLen + 1]; /*!< ASCII�ַ����� */
					memset(lpszStr, 0, nBufferLen + 1);
					memcpy_s(lpszStr, nBufferLen, achDataBuffer, nBufferLen);

					DebugPrint::Printf("�ͻ��˽������ݳɹ�!\n16����:%s", ssHex.str().c_str());
					DebugPrint::Printf("ASCII�ַ���:%s", lpszStr);

					delete[] lpszStr;

					ProcessData((unsigned char*)achDataBuffer, nBufferLen);
				} // if (nBufferLen > 0)
				else
				{
					shutdown(m_socket, SD_BOTH);
					m_socket = INVALID_SOCKET;

					return;
				}
			} // if (FD_ISSET(m_socket, &fds))
			break;
		}
		}

		Sleep(1);
	}


	return;
}

int main()
{
	InitAGV(1);

	printf("������˿�:");

	char achPort[6];	/*!< �û�����˿� */

	// ��ʼ�� 
	memset(achPort, 0, 6);

	cin.get(achPort, 6);
	
	int nPort = stoi(achPort);

	TCPServer server(nPort);	/*!< �����*/

	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(nullptr, true, false, nullptr);
	}

	printf("������Ϣ�߳�\n");
	m_pThreadMsg = new thread(&MessageThread);

	printf("��������˼���\n");
	server.StartListen(ThreadEx::GetThreadID(m_pThreadMsg));

	printf("�ȴ��ͻ�������......\n");

	while (m_socket == INVALID_SOCKET);

	printf("�ͻ������ӳɹ�\n");

	if (m_hProcess)
	{
		ResetEvent(m_hProcess);
	}
	else
	{
		m_hProcess = CreateEvent(nullptr, true, false, nullptr);
	}

	printf("�������ݽ����߳�\n");
	m_pThreadReceive = new thread(&ReceiveThread);

	while (true)
	{
		if (WaitForSingleObject(m_hProcess, INFINITE) == WAIT_OBJECT_0)
		{
			ResetEvent(m_hProcess);

			Json::CharReaderBuilder builder;
			Json::CharReader* JsonReader(builder.newCharReader());
			Json::Value JsonRoot;

			JSONCPP_STRING errs;
			const char* pstr = (char*)g_achPackage;

			if (!JsonReader->parse(pstr, pstr + strlen(pstr), &JsonRoot, &errs))
			{
				break;
			}

			string strAction = "";
			strAction = Json_ReadString(JsonRoot["Cmd"]);

			if (strcmp(strAction.c_str(), "Heartbeat") == 0)
			{
				printf("������������\n");
				int nSendLen = FeedbackHeartbeat(g_achPackage, strlen(g_achPackage), g_achSend, MAX_SEND_LEN);
				send(m_socket, g_achSend, nSendLen,0);

				// ��¼���͵�����
				stringstream ssHex; /*!< 16�������� */
				for (int nI = 0; nI < nSendLen; ++nI)
				{
					ssHex << StringEx::string_format("%02X ", (*(g_achSend + nI) & 0xFF));
				}
				char* lpszStr = new char[nSendLen + 1]; /*!< ASCII�ַ����� */
				memset(lpszStr, 0, nSendLen + 1);
				memcpy_s(lpszStr, nSendLen, g_achSend, nSendLen);

				DebugPrint::Printf("�ͻ��˷������ݳɹ�!\n16����:%s", ssHex.str().c_str());
				DebugPrint::Printf("ASCII�ַ���:%s", lpszStr);

				delete[] lpszStr;
			}
			else
			{
				printf("���ձ��Ķ���%s\n", strAction.c_str());
				int nSendLen = FeedbackOther(g_achPackage, strlen(g_achPackage), g_achSend, MAX_SEND_LEN);
				send(m_socket, g_achSend, nSendLen, 0);

				// ��¼���͵�����
				stringstream ssHex; /*!< 16�������� */
				for (int nI = 0; nI < nSendLen; ++nI)
				{
					ssHex << StringEx::string_format("%02X ", (*(g_achSend + nI) & 0xFF));
				}
				char* lpszStr = new char[nSendLen + 1]; /*!< ASCII�ַ����� */
				memset(lpszStr, 0, nSendLen + 1);
				memcpy_s(lpszStr, nSendLen, g_achSend, nSendLen);

				DebugPrint::Printf("�ͻ��˷������ݳɹ�!\n16����:%s", ssHex.str().c_str());
				DebugPrint::Printf("ASCII�ַ���:%s", lpszStr);

				delete[] lpszStr;
			}
		}
	}

	// �ر����ݽ����߳�
	printf("�ر����ݽ����߳�\n");
	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;

	m_pThreadReceive->join();

	delete m_pThreadReceive;
	m_pThreadReceive = NULL;

	CloseHandle(m_hProcess);
	m_hProcess = NULL;

	// �ر���Ϣ�߳�	
	printf("�ر���Ϣ�߳�\n");
	SetEvent(m_hShutdownEvent);

	server.CloseListen();

	m_pThreadMsg->join();

	CloseHandle(m_hShutdownEvent);
	m_hShutdownEvent = NULL;

	delete m_pThreadMsg;
	m_pThreadMsg = NULL;

	printf("�����˳�\n");

	return 0;
}