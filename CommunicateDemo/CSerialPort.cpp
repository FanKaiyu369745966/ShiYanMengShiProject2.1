#include "pch.h"
#include "CSerialPort.h"
#include "../String/DebugPrint.h"
#include "../String/StringEx.h"

#ifdef _SERIALPORT_H

CSerialPort::CSerialPort()
{
	m_usPort = 0;
	m_unBaud = 0;
	m_chParity = 0;
	m_byDataBits = 0;
	m_byStopBits = 0;

	m_hComm = nullptr;
	m_pThreadComm = nullptr;

	// ��ʼ���¼�
	m_hWriteEvent = nullptr;
	m_hShutdownEvent = nullptr;
	// ��ʼ���첽������Ա����
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;
	// �����첽�������¼���Ա����
	m_ov.hEvent = nullptr;

	m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;

	//��ʼ�����ͱ���
	m_szWriteBuffer = nullptr;
	m_dwWriteBufferSize = 0;
}

CSerialPort::CSerialPort(unsigned short usPort, unsigned int unBaud, char chParity, unsigned char byDataBits, unsigned char byStopBits)
{
	m_usPort = usPort;
	m_unBaud = unBaud;
	m_chParity = chParity;
	m_byDataBits = byDataBits;
	m_byStopBits = byStopBits;

	m_hComm = nullptr;
	m_pThreadComm = nullptr;

	// ��ʼ���¼�
	m_hWriteEvent = nullptr;
	m_hShutdownEvent = nullptr;
	// ��ʼ���첽������Ա����
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;
	// �����첽�������¼���Ա����
	m_ov.hEvent = nullptr;

	m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;

	//��ʼ�����ͱ���
	m_szWriteBuffer = nullptr;
	m_dwWriteBufferSize = 0;
}

CSerialPort::~CSerialPort()
{
	ClosePort();

	if (m_hShutdownEvent)
	{
		CloseHandle(m_hShutdownEvent);
		m_hShutdownEvent = nullptr;
	}

	if (m_ov.hEvent)
	{
		CloseHandle(m_ov.hEvent);
		m_ov.hEvent = nullptr;
	}

	if (m_hWriteEvent)
	{
		CloseHandle(m_hWriteEvent);
		m_hWriteEvent = nullptr;
	}

	if (m_szWriteBuffer)
	{
		delete[] m_szWriteBuffer;
		m_dwWriteBufferSize = 0;
	}
}

bool CSerialPort::OpenSerialPort(unsigned long ulThreadID)
{
	if (m_usPort == 0 || m_unBaud == 0)
	{
		DebugPrint::Print("�򿪴���ʧ�ܣ���Ч�Ĵ��ڻ�����");
		return false;
	}

	ClosePort();

	m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;

	m_ulThreadID = ulThreadID;

	// ���������첽ͨ���¼�
	if (m_ov.hEvent)
	{
		ResetEvent(m_ov.hEvent);
	}
	else
	{
		m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	// ���������¼�
	if (m_hWriteEvent)
	{
		ResetEvent(m_hWriteEvent);
	}
	else
	{
		m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	// �����ر��¼�
	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	m_hEventArray[0] = m_hShutdownEvent; // ������ߵ����ȼ�
	m_hEventArray[1] = m_hWriteEvent;
	m_hEventArray[2] = m_ov.hEvent;

	char *szPort = new char[50];
	char *szBaud = new char[50];

	std::lock_guard<std::mutex> lock(m_mutexLock);

	if (m_hComm)
	{
		CloseHandle(m_hComm);
		m_hComm = nullptr;
	}

	sprintf_s(szPort, 50, "COM%d", m_usPort);
	sprintf_s(szBaud, 50, "baud=%d parity=%c data=%d stop=%d", m_unBaud, m_chParity, m_byDataBits, m_byStopBits);

	// �򿪴���
	m_hComm = CreateFileA(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		delete[] szPort;
		delete[] szBaud;

		return false;
	}

	// ��ʱ����
	m_CommTimeouts.ReadIntervalTimeout = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;

	// ���ô��ڲ���
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{
		// ��ʱ����
		if (SetCommMask(m_hComm, m_dwCommEvents))
		{
			// �¼�����
			if (GetCommState(m_hComm, &m_dcb))
			{
				// ��������
				m_dcb.EvtChar = 'q';
				m_dcb.fRtsControl = RTS_CONTROL_ENABLE;

				if (BuildCommDCBA(szBaud, &m_dcb))
				{
					if (!SetCommState(m_hComm, &m_dcb))
					{
						ProcessErrorMessage("SetCommState()");
					}
				}
				else
				{
					// ���ڲ�������ʧ��
					ProcessErrorMessage("BuildCommDCBA()");
				}
			}
			else
			{
				// ���ڲ�����ȡʧ��
				ProcessErrorMessage("GetCommState()");
			}
		}
		else
		{
			// �����¼�����ʧ��
			ProcessErrorMessage("SetCommMask()");
		}
	}
	else
	{
		// ���ڳ�ʱ����ʧ��
		ProcessErrorMessage("SetCommTimeouts()");
	}

	delete[] szPort;
	delete[] szBaud;

	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	StartCommThread();

	return true;
}

bool CSerialPort::OpenSerialPort(unsigned short usPort, unsigned int unBaud, char chParity, unsigned char byDataBits, unsigned char byStopBits, unsigned long ulThreadID)
{
	m_usPort = usPort;
	m_unBaud = unBaud;
	m_chParity = chParity;
	m_byDataBits = byDataBits;
	m_byStopBits = byStopBits;

	return OpenSerialPort(ulThreadID);
}

void CSerialPort::ClosePort()
{
	EndCommThread();

	if (m_hComm)
	{
		CloseHandle(m_hComm);
		m_hComm = nullptr;
	}

	return;
}

DWORD CSerialPort::GetCommEvents()
{
	return m_dwCommEvents;
}

DCB CSerialPort::GetDCB()
{
	return m_dcb;
}

bool CSerialPort::IsOpened()
{
	if (m_hComm == nullptr)
	{
		return false;
	}

	if (m_pThreadComm == nullptr)
	{
		CloseHandle(m_hComm);
		m_hComm = nullptr;
		return false;
	}

	if (m_pThreadComm->joinable())
	{
		return true;
	}

	CloseHandle(m_hComm);
	m_hComm = nullptr;

	return false;
}

void CSerialPort::WriteToPort(char * string)
{
	WriteToPort(string, strlen(string));
}

void CSerialPort::WriteToPort(char * string, int n)
{
	if (m_hComm == nullptr)
	{
		DebugPrint::Print("�򴮿�д������ʧ�ܣ�����δ����");
		return;
	}

	m_mutexLock.lock();

	if (m_szWriteBuffer)
	{
		delete[] m_szWriteBuffer;
		m_dwWriteBufferSize = 0;
	}

	m_szWriteBuffer = new char[n];
	memset(m_szWriteBuffer, 0, n);
	memcpy_s(m_szWriteBuffer, n, string, n);
	m_dwWriteBufferSize = n;

	m_mutexLock.unlock();

	// �÷����¼�Ϊ�е�״̬
	SetEvent(m_hWriteEvent);
	//WriteChar();

	return;
}

void CSerialPort::StartCommThread()
{
	EndCommThread();

	m_pThreadComm = new std::thread(&CSerialPort::CommThread, this);

	return;
}

void CSerialPort::EndCommThread()
{
	if (m_pThreadComm)
	{
		if (m_pThreadComm->joinable())
		{
			SetEvent(m_hShutdownEvent);

			m_pThreadComm->join();
		}

		delete m_pThreadComm;
		m_pThreadComm = nullptr;
	}

	return;
}

void CSerialPort::CommThread()
{
	// ������Ϣ����
	DWORD BytesTransfered = 0;
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	static COMSTAT comstat;
	BOOL bResult = TRUE;

	//memset(&comstat, 0, sizeof(COMSTAT));

	// ǿ�ƹرմ���
	if (m_hComm)
	{
		// ��մ��ڻ���
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	}

	// �߳�ѭ��
	while (true)
	{
		bResult = WaitCommEvent(m_hComm, &Event, &m_ov);

		// ��ȡʧ�ܴ���
		if (!bResult)
		{
			switch (dwError = GetLastError())
			{
			case ERROR_IO_PENDING:
				// ��ȡ����Ϊ��
				break;
			case 87:
				break;
			default:
				ProcessErrorMessage("WaitCommEvent()");
				break;
			}
		}
		else
		{
			// ��մ��ڴ���
			bResult = ClearCommError(m_hComm, &dwError, &comstat);
			if (comstat.cbInQue == 0)
			{
				continue;
			}
		}

		// ��ȡ�����¼���������Ӧ����
		Event = WaitForMultipleObjects(3, m_hEventArray, FALSE, INFINITE);

		switch (Event)
		{
		case 0:
			// �رմ���
			CloseHandle(m_hComm);
			m_hComm = nullptr;
			return;
		case 1:
			// д����
			WriteChar();
			break;
		case 2:
			// ������
			GetCommMask(m_hComm, &CommEvent);
			if (CommEvent & EV_RXCHAR)
			{
				ReceiveChar(comstat);
			}
			break;
		}
	}

	return;
}

void CSerialPort::ReceiveChar(COMSTAT comstat)
{
	BOOL bRead = TRUE;
	BOOL bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	unsigned char RXBuff;

	// ��ȡ��������
	while (true)
	{
		std::lock_guard<std::mutex> lock(m_mutexLock);

		bResult = ClearCommError(m_hComm, &dwError, &comstat);
		if (comstat.cbInQue == 0)
		{
			// �����ݿɶ�
			break;
		}

		if (bRead)
		{
			RXBuff = 0;
			bResult = ReadFile(m_hComm, &RXBuff, 1, &BytesRead, &m_ov);

			if (!bResult)
			{
				// ��ȡʧ��
				switch (dwError = GetLastError())
				{
					// �������
				case ERROR_IO_PENDING:
					// ������δ����
					bRead = FALSE;
					break;
				default:
					// һ���������
					ProcessErrorMessage("ReadFile()");
					break;
				}
			}
			else
			{
				// ��ȡ�ɹ�
				bRead = TRUE;
			}
		}

		if (!bRead)
		{
			// ��ȡʧ�ܴ���
			bRead = TRUE;
			bResult = GetOverlappedResult(m_hComm, &m_ov, &BytesRead, TRUE);

			if (!bResult)
			{
				ProcessErrorMessage("GetOverlappedResult() in ReadFile()");
			}
		}

		ReadChar((char*)& RXBuff, BytesRead);
	}

	return;
}

void CSerialPort::WriteChar()
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;
	DWORD BytesSent = 0;
	ResetEvent(m_hWriteEvent);

	std::lock_guard<std::mutex> lock(m_mutexLock);

	if (bWrite)
	{
		m_ov.Offset = 0;
		m_ov.OffsetHigh = 0;

		// ��մ��ڻ���
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

		bResult = WriteFile(m_hComm, m_szWriteBuffer, m_dwWriteBufferSize, &BytesSent, &m_ov);

		if (!bResult)
		{
			// д����ʧ��
			DWORD dwError = GetLastError();

			switch (dwError)
			{
			case ERROR_IO_PENDING:
				// ��δд������
				BytesSent = 0;
				bWrite = FALSE;
				break;
			default:
				ProcessErrorMessage("WriteFile()");
				break;
			}
		}
	}

	if (!bWrite)
	{
		bWrite = TRUE;

		// �ȴ�д�������
		bResult = GetOverlappedResult(m_hComm, &m_ov, &BytesSent, TRUE);

		if (!bResult)
		{
			ProcessErrorMessage("GetOverlappedResult() in WriteFile()");
		}
	}

	// ����ʧ��
	if (BytesSent != m_dwWriteBufferSize)
	{
		DebugPrint::Printf("WARNING��WriteFile() error.. Bytes Sent��%d��Message Length��%d", BytesSent, m_dwWriteBufferSize);

		//ClosePort();
	}
	
	/*
	// ���ͳɹ�
	else
	{
		// ��¼���͵�����
		stringstream ssHex; // 16��������
		for (unsigned int unI = 0; unI < m_dwWriteBufferSize; ++unI)
		{
			ssHex << StringEx::string_format("%02X ", (*(m_szWriteBuffer + unI)) & 0xFF);
		}
		char* lpszStr = new char[m_dwWriteBufferSize + 1]; // ASCII�ַ�����
		memset(lpszStr, 0, m_dwWriteBufferSize + 1);
		memcpy_s(lpszStr, m_dwWriteBufferSize, m_szWriteBuffer, m_dwWriteBufferSize);

		DebugPrint::Printf("�򴮿�%d�������ݳɹ�!\n16����:%s", m_usPort, ssHex.str().c_str());
		DebugPrint::Printf("ASCII�ַ���:%s", lpszStr);

		delete[] lpszStr;
	}
	*/
	
	return;
}

void CSerialPort::ReadChar(const char * data, int len)
{
	if (m_ulThreadID == 0)
	{
		return;
	}

	for (int nI = 0; nI < len; ++nI)
	{
		PostThreadMessage(m_ulThreadID, TM_COMM_RXCHAR, *(data + nI), 1);
	}

	return;
}

void CSerialPort::ProcessErrorMessage(const char * ErrorText)
{
	char *Temp = new char[200];
	LPVOID lpMsgBuf; /*!< ���������Ϣ��ָ�� */

	// ��GetLastError()�õ��Ĵ�����Ϣת�����ַ�����Ϣ
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, NULL);

	// ��ʾ������Ϣ
	DebugPrint::Printf("WARNING��%s Failed with the following error��\n%s\nPort��%d", ErrorText, (LPSTR)lpMsgBuf, m_usPort);

	// �ͷ���Դ
	LocalFree(lpMsgBuf);
	delete[] Temp;

	return;
}

#endif //_SERIALPORT_H
