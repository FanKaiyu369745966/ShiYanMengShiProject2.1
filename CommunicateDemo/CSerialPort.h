/*!
 * @file CSerialPort.h
 * @brief RS-232����ͨ���ļ�
 *
 * �����봮�ڽ������ݽ�����ģ��
 * @author FanKaiyu
 * @date 2019-01-02
 * @version 2.0
*/

#pragma once
#ifndef _SERIALPORT_H
#define _SERIALPORT_H

#include <Windows.h>
#include <thread>
#include <mutex>

#ifndef TM_COMM_RXCHAR
#define TM_COMM_RXCHAR	WM_USER+1	/*!< ���ڽ������ݵ��߳���Ϣ */
#endif //!WM_COMM_RXCHAR

/*!
 * @brief RS-232����9���˵����DB9��ͷ��
 * 1	CD��Carier Detect��			�ز����
 * 2	RXD��Recevie Data��			��������
 * 3	TXD��Transmit Data��		��������
 * 4	DTR��Data Terminal Ready��	�����ն˾���
 * 5	GND��System Ground��		ϵͳ�ӵ�
 * 6	DSR��Data Set Readys��		�����豸����
 * 7	RTS��Request To Send��		������
 * 8	CTS��Clear To Send��		������
 * 9	RI��Ring Indicator��		����ָʾ��
*/

/*!
 * @brief �����ʳ�������
 * ֵ			winbase.h�еĶ���				˵��
 * CBR_110		#define CBR_110		110			������Ϊ110
 * CBR_300		#define CBR_300		300			������Ϊ300
 * CBR_600		#define CBR_600		600			������Ϊ600
 * CBR_1200		#define CBR_1200	1200		������Ϊ1200
 * CBR_2400		#define CBR_2400	2400		������Ϊ2400
 * CBR_4800		#define CBR_4800	4800		������Ϊ4800
 * CBR_9600		#define CBR_9600	9600		������Ϊ9600
 * CBR_14400	#define CBR_14400	14400		������Ϊ14400
 * CBR_19200	#define CBR_19200	19200		������Ϊ19200
 * CBR_38400	#define CBR_38400	38400		������Ϊ38400
 * CBR_56000	#define CBR_56000	56000		������Ϊ56000
 * CBR_57600	#define CBR_57600	57600		������Ϊ57600
 * CBR_115200	#define CBR_115200	115200		������Ϊ115200
 * CBR_128000	#define CBR_128000	128000		������Ϊ128000
 * CBR_256000	#define CBR_256000	256000		������Ϊ256000
*/

/*!
 * @brief ��żУ�鳣������
 * ֵ			winbase.h�еĶ���			˵��
 * NOPARITY		#define NOPARITY	0		������У�鷽ʽ
 * ODDPARITY	#define ODDPARITY	1		������У�鷽ʽ
 * EVENPARITY	#define EVENPARITY	2		����żУ�鷽ʽ
 * MARKPARITY	#define MARKPARITY	3		���ñ��У�鷽ʽ
*/

/*!
 * @brief ֹͣλ��������
 * ֵ				winbase,h�еĶ���				˵��
 * ONESTOPBIT		#define ONESTOPBIT		0		1λֹͣλ
 * ONE5STOPBITS		#define ONE5STOPBITS	1		1.5λֹͣλ
 * TWOSTOPBITS		#define TWOSTOPBITS		2		2λֹͣλ
*/

/*!
 * @brief ��ջ�������ֵ
 * ֵ				˵��
 * PURGE_RXABORT	�ж����ж��������������أ���ʹ��������û�����
 * PURGE_TXABORT	�ж�����д�������������أ���ʹд������û�����
 * PURGE_TXCLEAR	������������
 * PURGE_RXCLEAR	������뻺����
*/

/*!
 * @class CSerialPort
 * @brief RS-232����ͨ����
 *
 * �����봮�ڽ������ݽ�����ģ��
*/

#define NONE_PARITY 'N'
#define ODD_PARITY 'O'
#define EVEN_PARITY 'E'

class CSerialPort
{
public:
	CSerialPort();
	CSerialPort(unsigned short usPort, unsigned int unBaud, char chParity, unsigned char byDataBits, unsigned char byStopBits);
	~CSerialPort();
protected:
	/*!
	 * ���ڵĻ�������
	*/
	unsigned short m_usPort;		/*!< ���ں� */
	unsigned int m_unBaud;			/*!< ������ */
	char m_chParity;				/*!< ��żУ�� */
	unsigned char m_byDataBits;		/*!< ����λ */
	unsigned char m_byStopBits;		/*!< ֹͣλ */

protected:
	HANDLE m_hComm;					/*!< ���ڲ������ */
	std::thread* m_pThreadComm;		/*!< �߳�ָ�� */
	std::mutex m_mutexLock;			/*!< �̻߳����� */
	unsigned long m_ulThreadID;		/*!< �߳�ID */

	/*!
	 * �����¼�
	*/
	HANDLE m_hWriteEvent;				/*!< д�¼� */
	HANDLE m_hShutdownEvent;			/*!< �ر��¼� */
	HANDLE m_hEventArray[3];			/*!< �¼����� */
	DWORD m_dwCommEvents;				/*!< �����¼����� */
	OVERLAPPED m_ov;					/*!< �����첽I/0���� */
	COMMTIMEOUTS m_CommTimeouts;		/*!< ���ڳ�ʱ�ṹ�� */
	DCB m_dcb;							/*!< ����ͨ���е�DCB�ṹ */
	char* m_szWriteBuffer;				/*!< д������ */
	DWORD m_dwWriteBufferSize;			/*!< д��������С */

public:
	/*!
	 * @brief �򿪴���
	 * @param unsinged long �߳�ID
	 * @return bool �򿪳ɹ�����true,ʧ�ܷ���false
	*/
	bool OpenSerialPort(unsigned long ulThreadID = 0);

	/*!
	 * @brief ��ָ������
	 *
	 * ע:���ô˺������жϵ�ǰ�Ѵ򿪵Ĵ��ڣ������Ĵ�������
	 * @param unsigned short ���ں�
	 * @param unsigned int ������
	 * @param char ��żУ��λ
	 * @param unsigned char ����λ
	 * @param unsigned char ֹͣλ
	 * @param unsinged long �߳�ID
	 * @return bool �򿪳ɹ�����true,ʧ�ܷ���false
	*/
	bool OpenSerialPort(unsigned short usPort, unsigned int unBaud, char chParity, unsigned char byDataBits, unsigned char byStopBits, unsigned long ulThreadID = 0);

	/*!
	 * @brief �رմ���
	*/
	void ClosePort();

	/*!
	 * @brief ��ȡ�����¼�
	 * @return unsigned long �����¼�
	*/
	DWORD GetCommEvents();

	/*!
	 * @brief ��ȡ����������Ϣ
	 * @return DCB ����������Ϣ
	*/
	DCB GetDCB();

	/*!
	 * @brief �жϴ����Ƿ��
	 * @return bool �򿪷���true,δ�򿪷���false
	*/
	bool IsOpened();

	/*!
	 * @brief ��ȡ�����潫Ҫ���͵����ݣ������������¼�
	 * @param char* �ַ���
	 * @return bool ���ݱ���ɹ�����true,���ݱ���ʧ�ܷ���false
	*/
	void WriteToPort(char* string);

	/*!
	 * @brief ��ȡ�����潫Ҫ���͵����ݣ������������¼�
	 * @param char* �ַ���
	 * @param int �ַ�������
	 * @return bool ���ݱ���ɹ�����true,���ݱ���ʧ�ܷ���false
	*/
	void WriteToPort(char* string, int n);

protected:
	/*!
	 * @brief �����߳�
	*/
	void StartCommThread();

	/*!
	 * @brief �ر��߳�
	*/
	void EndCommThread();

	/*!
	 * @brief �����̴߳���
	*/
	void CommThread();

	/*!
	 * @brief ��ȡ��������
	 * @param COMSTAT ����״̬
	*/
	void ReceiveChar(COMSTAT comstat);

	/*!
	 * @brief �򴮿�д������
	*/
	void WriteChar();

	/*!
	 * @brief �Ӵ��ڶ�ȡ����
	 * @param char* ���ݿ�ָ��
	 * @param int ���ݳ���
	*/
	virtual void ReadChar(const char* data, int len);

	/*!
	 * @brief �����¼�����
	 * @param char* ������Ϣ
	*/
	void ProcessErrorMessage(const char* ErrorText);
};

#endif //!_SERIALPORT_H