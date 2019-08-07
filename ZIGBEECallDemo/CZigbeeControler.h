/*!
 * @file CZigbeeControler.h
 * @brief ʵ����Zigbeeͨ�ŵ���ع���
 * @author FanKaiyu
 * @date 2019-04-02
 * @version 1.0
*/

#pragma once
#ifndef _ZGBCTRLER_H
#define _ZGBCTRLER_H

#include "../CommunicateDemo/CSerialPort.h"
#include "../String/Buffer.h"
#include "../json/json.h"
#include "CZigbeeCaller.h"
#include <thread>
#include <map>
#include <list>

using namespace std;

/*! @brief ������ */
enum FUNC_TYPE
{
	FUNC_HEARTBEAT = 0x1F,
	FUNC_RESET = 0x2F,
	FUNC_SETSIGNAL = 0x3F,
};

/*!
 * @class CZigbeeControler
 * @brief Zigbeeͨ�ſ�������
*/
typedef class CZigbeeControler
	:protected CSerialPort
{
public:
	CZigbeeControler(unsigned char byNo);
	CZigbeeControler(unsigned char byNo, unsigned short usPort, unsigned int unBaud, char chParity, unsigned char byDataBits, unsigned char byStopBits);
	~CZigbeeControler();
protected:
	unsigned char m_byNo;						/*!< ��������� */

protected:
	thread* m_pThreadSend;						/*!< ���ݷ����߳� */
	DataBuffer m_bufferRead;					/*!< ������ ���Դ���δ��������� */
	HANDLE m_hShutdownEvent;					/*!< �ر��¼� */
	list<DataBuffer> m_listSingle;				/*!< ���η��͵������б� */
	list<DataBuffer> m_listWhile;				/*!< ѭ�����͵������б� */
	mutex m_mutexSend;							/*!< �������ݻ����� */
	Json::CharReaderBuilder m_JsonBuilder;
	Json::CharReader* m_pJsonReader;
	JSONCPP_STRING m_JstrErrs;

protected:
	static list<DataBuffer> m_listProcess;		/*!< ����������ݱ����б� */
	static mutex m_mutexProcess;				/*!< ���������ݻ����� */

public:
	/*!
	 * @brief ����
	 *
	 * ��ȡ��������ǰʵʱ״̬��Ϣ
	 * ���������յ�ָ����跴��������
	 * {"Message"={},Result={"Call":0,"CancelCall":0,"Pass":0,"CancelPass":0,"NG":0,"CancelNG":0,"Scream"��0}}
	 * @param unsigned char ���������
	*/
	void Heartbeat(ZGBCaller* pCaller);
	void Heartbeat(const unsigned char byNo, char achMAC[4]);

	/*!
	 * @brief ����
	 *
	 * ���ú������ź���
	 * ����������ظ�
	 * @param unsigned char ���������
	*/
	void Reset(ZGBCaller* pCaller);
	void Reset(const unsigned char byNo, char achMAC[4]);

	/*!
	 * @brief �����ź�
	 * @param unsigned char ���������
	 * @param const char* �ź�����
	 * @param unsigned char �źſ���
	*/
	void SetSignal(ZGBCaller* pCaller, const unsigned char bySignal, const unsigned char bySwitch);
	void SetSignal(const unsigned char byNo, char achMAC[4], const unsigned char bySignal, const unsigned char bySwitch);
	//void SetSignal(const unsigned char byNo,const char* strSignal,const unsigned char bySwitch);
	//void SetSignal(const unsigned char byNo, const wchar_t* strSignal, const unsigned char bySwitch);

public:
	/*!
	 * @brief ����������
	 * @param unsigned short ���ں�
	 * @param unsigned int ������
	 * @param char ��żУ��λ
	 * @param unsigned char ����λ
	 * @param unsigned char ֹͣλ
	 * @param unsinged long �߳�ID
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool Open();
	bool Open(unsigned short usPort, unsigned int unBaud = CBR_115200, char chParity = NONE_PARITY, unsigned char byDataBits = 8, unsigned char byStopBits = 1);

	/*!
	 * @brief �رտ�����
	*/
	void Close();

	/*!
	 * @brief �������Ƿ��ѿ���
	 * @return bool ��������true,���򷵻�false
	*/
	bool IsOpened();

	/*!
	 * @brief ��ȡ���������ݰ�
	 * @return DataBuffer ��ȡ���������ݰ�
	*/
	static DataBuffer GetProcessPackage();
protected:
	/*!
	 * @brief �Ӵ��ڶ�ȡ����
	 * @param char* ���ݿ�ָ��
	 * @param int ���ݳ���
	*/
	void ReadChar(const char* data, int len);

	/*!
	 * @brief �������ݰ�
	 * @param const unsigned char* ���ݰ�ָ��
	 * @param const int ���ݰ�����
	*/
	void ProcessPackage(const unsigned char* pPack,const int nPackLen);

	/*!
	 * @brief ���ͱ���
	 * @param char* Json�ַ���
	*/
	void SendPackage( const char* lpszStr, char achMAC[4]);

	/*!
	 * @brief ���ͱ���
	 * @param char* ���ݿ�ָ��
	 * @param int ���ݿ鳤��
	*/
	void SendPackage( const char* pData, const int nLen, char achMAC[4]);

	/*!
	 * @brief ת��
	 * @param const char* ��ת�������
	 * @param const int ��ת������ݳ���
	 * @param char* ת��������
	 * @param int& ת�������ݳ���
	*/
	void Translation(const char* pSourceData,int nSourceLen,char* pTransData,int& pTransLen);

	/*!
	 * @brief ��ת��
	 * @param const char* �跴ת�������
	 * @param const int �跴ת������ݳ���
	 * @param char*Դ����
	 * @param int& Դ���ݳ���
	*/
	void Retranslation(const char* pTransData, int pTransLen, char* pSourceData, int& nSourceLen);

	/*!
	 * @brief �������ݷ����߳�
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool StartSendThread();

	/*!
	 * @brief �ر����ݷ����߳�
	*/
	void EndSendThread();

	/*!
	 * @brief ���ݷ����߳�
	*/
	void SendThread();

	/*!
	 * @brief �ϳɱ���
	 * @param const char* ������
	 * @param const int �����峤��
	 * @return DataBuffer �ϳɺ����������
	*/
	DataBuffer CreatePackage(const char* pData,const int nLen, char achMAC[4]);

	/*!
	 * @brief ��������ջظ��������б�
	 * @param const char* ������ָ��
	 * @param const int �����峤��
	*/
	void InsertIntoWhileList(const char* pData, const int nLen, char achMAC[4]);

	/*!
	 * @brief �������ݷ����б�
	 * @param const char* ������ָ��
	 * @param const int �����峤��
	*/
	void InsertIntoSingleList(const char* pData, const int nLen, char achMAC[4]);

	/*!
	 * @brief �Ӵ����ջظ��������б��Ƴ�
	 * @param const char* ������ָ��
	 * @param const int �����峤��
	*/
	void RemoveFromWhileList(const char* pData, const int nLen, char achMAC[4]);

	/*!
	 * @brief ������������ݰ�
	 * @param const char* ������ָ��
	 * @param const int �����峤��
	*/
	static void InsertIntoProcessList(const char* pData, const int nLen);
} ZGBCtrler;

#endif //!_ZGBCTRLER_H