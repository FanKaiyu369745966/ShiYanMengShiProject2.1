/*!
 * @file BaseFunction.h
 * @brief ʵ�ֵ���ϵͳ�������ܵ��ļ�
 * @author FanKaiyu
 * @date 2019-04-09
 * @version 2.2
*/
#pragma once
#ifndef _BASEFUNCTION_H
#define _BASEFUNCTION_H

#include "../AGVDemo/CAGV.h"
//#include "../AGVDemo/CAGV.cpp"
#include "CRFID.h"
#include "CTraffiic.h"
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

typedef bool(__stdcall* UpdateStatus)(Json::Value*, void*);

/*!
 * @class BeseFunction
 * @brief ʵ�ֵ���ϵͳ�������ܵ���
*/
class BaseFunction
{
public:
	BaseFunction();
	~BaseFunction();

protected:
	TCPServer m_tcpServer;								/*!< ����� */
	map<unsigned char, CAGV*> m_mapAGVs;				/*!< AGV���� */
	map<unsigned short, Traffic*> m_mapTraffics;		/*!< ��ͨ�������� */

	RFID m_aRFIDArr[USHORT_MAX];						/*!< RFID������ */
	unsigned short m_ausExhange[USHORT_MAX];			/*!< ת��RFID����ŵ����� */
	bool m_bUpdateAGV[UCHAR_MAX];						/*!< ����AGV���ݵ����� */

protected:
	string m_strAGVs;									/*!< ���Դ���AGV������Ϣ��JSON�ַ��� {"AGV":[{"No":1,"ServerIP":"","ServerPort":0,"ClientIP":"","ClientPort":-1,"CurRFID":0},{...}]} */
	string m_strServer;									/*!< ���Դ�������������Ϣ��JSON�ַ��� {"IP":"","Port":0} */
	string m_strTraffic;								/*!< ���Դ��潻ͨ����������Ϣ��JSON�ַ��� {"Traffic":[{"Control":1,"Stop":[2,3,4,5],"Compare":[6,7,8,9]},{...}]} */
	string m_strExchange;

protected:
	Json::CharReaderBuilder m_JsonBuilder;
	Json::CharReader* m_pJsonReader;
	JSONCPP_STRING m_JstrErrs;

	mutex m_mutexJson;
	HANDLE m_hShutdownEvent;							/*!< �ر��¼� */

	thread* m_pThreadMain;								/*!< ���߳� */

	UpdateStatus m_pFunc;
	void* m_pPtr;

protected:
	Json::Value*m_pStrUpdateServer;							/*!< ���Դ�ŷ���˽�����µ�JSON�ַ��� {"IP":"","Port":"","Listen":true}*/
	map<unsigned short, Json::Value*> m_mapStrUpdateAGVs;	/*!< ���Դ��AGV������µ�JSON�ַ��� {"No":1,"Connect":true,"RFID":{"Cur":0,"Next":0,"End":0},"Message":{"IP":"","Port":"","Status":{"Value":"","Color":""},"Speed":"","Power":{"Value":"","Color":""},"Cargo":{"Value":"","Color":""},"Lifter":"","Error":{"Value":"","Color":""},"Action":"",},"View":{"Obs":false,"Run":false,"Unvol":true,"Cargo":false,"Error":false,"Action":0}}*/

public:
	static mutex g_mutex;

public:
	/*!
	 * @brief ��ʼ������
	*/
	virtual bool Init(UpdateStatus func, void* ptr);

	/*!
	 * @brief ��������
	 * @param unsigned long ��Ϣ�߳�ID
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	virtual bool Open(UpdateStatus func, void* ptr);

	/*!
	 * @brief �رճ���
	*/
	virtual void Close();

	/*!
	 * @brief �ͷų����ڴ�
	*/
	virtual void Release();

	virtual void ReleaseString();

	/*!
	 * @brief ��JSON�ַ�������ȡ�����������Ϣ
	 * @param const char* JSON�ַ���
	*/
	void JsonStringToServer(const char* strJson);

	/*!
	 * @brief ��JSON�ַ�������ȡ�����������Ϣ
	 * @param const wchar_t* JSON�ַ���
	*/
	void JsonStringToServer(const wchar_t* wstrJson);

	/*!
	 * @brief ��JSON�ļ�����ȡ�����������Ϣ
	 * @param const char* JSON�ļ�·��
	*/
	void JsonFileToServer(const char* strFile);

	/*!
	 * @brief ��JSON�ļ�����ȡ�����������Ϣ
	 * @param const wchar_t* JSON�ļ�·��
	*/
	void JsonFileToServer(const wchar_t* wstrFile);

	/*!
	 * @brief ��JSON�ַ�������ȡAGV������Ϣ
	 * @param const char* JSON�ַ���
	*/
	void JsonStringToAGVs(const char* strJson);

	/*!
	 * @brief ��JSON�ַ�������ȡAGV������Ϣ
	 * @param const wchar_t* JSON�ַ���
	*/
	void JsonStringToAGVs(const wchar_t* wstrJson);

	/*!
	 * @brief ��JSON�ļ�����ȡAGV������Ϣ
	 * @param const char* JSON�ļ�·��
	*/
	void JsonFileToAGVs(const char* strFile);

	/*!
	 * @brief ��JSON�ļ�����ȡAGV������Ϣ
	 * @param const wchar_t* JSON�ļ�·��
	*/
	void JsonFileToAGVs(const wchar_t* wstrFile);

	/*!
	 * @brief ��JSON�ַ�������ȡ��ͨ���ƿ�����
	 * @param const char* JSON�ַ���
	*/
	void JsonStringToTraffics(const char* strJson);

	/*!
	 * @brief ��JSON�ַ�������ȡ��ͨ���ƿ�����
	 * @param const wchar_t* JSON�ַ���
	*/
	void JsonStringToTraffics(const wchar_t* wstrJson);

	/*!
	 * @brief ��JSON�ļ�����ȡ��ͨ���ƿ�����
	 * @param const char* JSON�ļ�·��
	*/
	void JsonFileToTraffics(const char* strFile);

	/*!
	 * @brief ��JSON�ļ�����ȡ��ͨ���ƿ�����
	 * @param const wchar_t* JSON�ļ�·��
	*/
	void JsonFileToTraffics(const wchar_t* wstrFile);

	/*!
	 * @brief ��JSON�ַ�������ȡRFIDת������
	 * @param const char* JSON�ַ���
	*/
	void JsonStringToRFIDExchanges(const char* strJson);

	/*!
	 * @brief ��JSON�ַ�������ȡRFIDת������
	 * @param const wchar_t* JSON�ַ���
	*/
	void JsonStringToRFIDExchanges(const wchar_t* wstrJson);

	/*!
	 * @brief ��JSON�ļ�����ȡRFIDת������
	 * @param const char* JSON�ļ�·��
	*/
	void JsonFileToRFIDExchanges(const char* strFile);

	/*!
	 * @brief ��JSON�ļ�����ȡRFIDת������
	 * @param const wchar_t* JSON�ļ�·��
	*/
	void JsonFileToRFIDExchanges(const wchar_t* wstrFile);

protected:
	/*!
	 * @brief �ͻ��˽���
	 * @param SOCKET �ͻ���SOCKETͨ��
	*/
	void Accept(SOCKET socket);

	/*!
	 * @brief ��ʼ�������
	*/
	void InitServer();

	/*!
	 * @brief ��ʼ��RFID
	*/
	void InitTraffic();

	/*!
	 * @brief ��ʼ��AGV
	*/
	bool InitAGVs();

	bool InitExchangeRFID();
	
	/*!
	 * @brief ����AGV���յı���
	*/
	void ProcessAGVPackage();

	/*!
	 * @brief ��ͨ���ƿ���
	*/
	void TrafficControl();

	/*!
	 * @brief ��ͨ���ƿ���
	 * @param void* ���ܿ��ƵĶ���ָ��
	*/
	void TrafficControl(void* pUser);

	/*!
	 * @brief ���Ͽ���
	 * @param void* ���ܿ��ƵĶ���ָ��
	*/
	void ObsControl(void* pUser);

	/*!
	 * @brief ����AGV��������
	 * @bool ���ӳɹ�����true,���򷵻�false
	*/
	bool AcceptAGV(SOCKET socket);
	void ConnectAGV();

	/*!
	 * @brief ���տͻ�����������
	 *
	 * ������
	 * @bool ���ӳɹ�����true,���򷵻�false
	*/
	virtual bool AcceptClient(SOCKET socket);

	/*!
	 * @brief AGV״̬����
	 * @param void* ���ܿ��ƵĶ���ָ��
	*/
	virtual void ProcessAGVStatus(void* pUser);

	const unsigned short ExchangeRFID(const unsigned short usRFID);

protected:
	/*!
	 * @brief ���߳�
	 *
	 * ����ʵ�ֳ�����
	*/
	void MainThread();

	/*!
	 * @brief �������߳�
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool StartMainThread();

	/*!
	 * @brief �ر����߳�
	*/
	void EndMainThread();

	/*!
	 * @brief ������
	 *
	 * ������
	*/
	virtual void Function();

protected:
	static void UpdateAGV(void* pAGV,void* ptr);
	static void UpdateServer(void* pServer, void* ptr,SOCKET socket);

	/*!
	 * @brief ����AGV״̬��Ϣ
	*/
	void UpdateAGV(CAGV* pAGV);

	/*!
	 * @brief ���·����״̬��Ϣ
	*/
	void UpdateServer(TCPServer& server);
};

#endif //!_BASEFUNCTION_H