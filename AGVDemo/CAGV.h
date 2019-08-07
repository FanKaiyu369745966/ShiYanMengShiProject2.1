/*!
 * @file CAGV.h
 * @brief ����AGV���������빦�ܵ��ļ�
 * @author FanKaiyu
 * @date 2019-04-04
 * @version 2.2
*/
#pragma once
#ifndef _AGV_H
#define _AGV_H

#include "../json/json.h"
#include "../CommunicateDemo/CTCP.h"
#include "../String/Buffer.h"
#include <list>

using namespace std;

/*!< AGV����״̬ */
enum _AGVPOWER
{
	_AGVPOWER_EMPTY = 0,		/*!< δ��⵽��� */
	_AGVPOWER_LOW = 20,			/*!< �͵��� */
	_AGVPOWER_NORMAL = 50,		/*!< �������� */
	_AGVPOWER_HIGH = 90,		/*!< �ߵ��� */
	_AGVPOWER_FULL = 100,		/*!< ����*/
};

/*!< AGV����״̬ */
enum _AGVSTATUS
{
	_AGVSTA_WAIT,		/*!< ����״̬.\
						��λ�����յ���״̬,��ΪAGV�Ѿ�׼���ò����Խ�����λ���Ŀ��Ƶ���.\
						��ʱ��λ�����Խ��ղ�������λ�����͵��������͵ı���.\
						*/
	_AGVSTA_RUN,		/*!< ����״̬.\
						��λ�����յ���״̬,��ΪAGV���ƶ�������.��ʱ��λ�����˻���������״̬�ⲻ���κδ���.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.���ƶ�������������뽫AGV״̬�ָ�Ϊ����״̬.\
						*/
	_AGVSTA_STOP,		/*!< ֹͣ״̬.\
						��λ�����յ���״̬,��ΪAGV����Ϊֹͣ.�����ֶ�����;��ʱ��λ�����˻���������״̬�ⲻ���κδ���.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.�û������ֶ�����AGV,���û�������������뽫AGV״̬�ָ�Ϊ����״̬.\
						*/
	_AGVSTA_SCREAM,		/*!< ��ͣ״̬.\
						��λ�����յ���״̬,��ΪAGV������ֹͣ����,�����ֶ�����.��ʱ��λ�����ϱ��������û�,���ҳ�������״̬�ⲻ��������.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.���û�������������뽫AGV״̬�ָ�Ϊ����״̬.
						*/
	_AGVSTA_FIND,		/*!< Ѱ��/��λ״̬.\
						��λ�����յ���״̬,��ΪAGV������.��ʱ��λ�����˻���������״̬�ⲻ���κδ���.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.��λ��Ӧ����Ѱ��/��λ����,��һ��ʱ����Ѱ��/��λʧ�ܺ�,Ӧ��AGV״̬��Ϊֹͣ,�����쳣״̬��Ϊ�Ѵ�/�޷���λ.\
						*/
	_AGVSTA_OBSDOWN,	/*!< �ǽӴ�ʽ���ϼ���״̬.\
						��λ�����յ���״̬,��ΪAGV�����ǽӴ�ʽ���ϲ����ڼ�������״̬,��ʱ��λ�����˻���������״̬�ⲻ���κδ���.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.\
						*/
	_AGVSTA_OBSSTOP,	/*!< �ǽӴ�ʽ����ֹͣ״̬.\
						��λ�����յ���״̬,��ΪAGV�����ǽӴ�ʽ���ϲ����ڼ�������״̬,��ʱ��λ�����˻���������״̬�ⲻ���κδ���.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.\
						*/
	_AGVSTA_MOBSSTOP,	/*!< �Ӵ�ʽ����ֹͣ״̬.\
						��λ�����յ���״̬,��ΪAGV�����Ӵ�ʽ���ϲ�����ֹͣ״̬,��ʱ��λ���ϱ��쳣���û�,���ҳ��˻���������״̬�ⲻ���κδ���.\
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.\
						*/
	_AGVSTA_TRAFFICSTOP,/*!< ��ͨ����ֹͣ״̬.\
						��λ�����յ���״̬,��ΪAGV���ڽ�ͨ���Ƶ�ȴ���λ������,��ʱ��λ�����ݽ�ͨ���ƹ��������״̬��AGV�Ƿ�����ͨ��.\
						��ʱ��λ�����˴�����λ�����������������Լ���ͨ���ƿ��Ʊ���,�������ľ����Բ�����.\
						*/
	_AGVSTA_ARM,		/*!< ��е�ַ���.\
						��λ�����յ���״̬,�����е���Ѿ���AGV�������.
						*/
	_AGVSTA_ALLSCREAM, /*!< ȫ�߼�ͣ */
	_AGVSTA_TRAFFICPASS,/*!< ��ͨ���Ʒ���״̬ */
};

/*! @brief AGV������Ϣ�� */
enum _AGVACTION
{
	_AGVACT_NONE,		/*!< ��/ֹͣ����.
						δִ��״̬:AGV�յ��˶���,��AGV����ִ����������,��ֹͣ������ִ��.
						ִ��״̬:������.
						ִ�����״̬:������.
						*/
	_AGVACT_MOVE,		/*!< �ƶ�����
						δִ��״̬:AGV�յ��˶���,��AGV��ʼ�ƶ�����ֹ�ر꿨.��AGV��ǰ��������ֹ������ͬ,�����ƶ�.������״̬��Ϊִ�����.���򽫶���״̬��Ϊִ����.
						ִ��״̬:��AGV��ǰ��������ֹ������ͬ,�����ƶ�,������״̬��Ϊִ�����.
						ִ�����״̬:������.
						*/
	_AGVACT_REMOVE,		/*!< �����ƶ�����
						δִ��״̬:AGV�յ��˶���,��AGV�����ƶ�����ֹ����.��AGV��ǰ��������ֹ������ͬ�����ƶ�ʱ,��AGV�ɽ���ǰ�������ú������ƶ�.������״̬��Ϊִ����.
						ִ��״̬:��AGV��ǰ��������ֹ������ͬ,�����ƶ�.������״̬��Ϊִ�����.
						ִ�����״̬:������.
						*/
	_AGVACT_TRAFFIC,	/*!< ��ͨ���ƶ���
						δִ��״̬:AGV�յ��˶���,��AGV����ֹ�ر꿨�ȴ���ͨ����.
						ִ��״̬:������.
						ִ�����״̬:AGV�յ��˶���,��AGV�����뿪��ֹ�ر꿨.
						*/
	_AGVACT_LIFTUP,		/*!< ��������������.
						δִ��״̬:������ʽAGV�����ǹ�Ͳִ�����϶���;
								����Ǳ��ʽ��ǣ��ʽAGV������������ִ����������;
								�Բ泵ʽAGV�����ǻ���ִ��������;
								�Ի�е��AGV�����ǻ�е��ִ��ץȡ����,����е��AGV��ͬһ������Ҫ��ɶ����ͬ�Ķ���ʱ,ͨ�����Ӷ�����ִ����Ӧ�Ķ���;
								ִ�ж���ǰAGVӦ�ж��Ƿ��ڿ�ִ�д˶�����ָ��λ����.������,��Ӧ����ǰ�ر꿨��Ϊ0���ϱ��쳣.
								����ʼִ�ж���,�򽫶���״̬��Ϊִ����.
						ִ��״̬:������ɺ󽫶���״̬��Ϊִ�����.ӵ�л��ϼ�⹦�ܵ�AGVӦ��ȷ�ϻ����Ѵ����AGV�Ϻ��ٽ�����״̬��Ϊִ�����.
						ִ�����״̬:������.
						*/
	_AGVACT_LIFTDOWN,	/*!< �������½�
						δִ��״̬:������ʽAGV�����ǹ�Ͳִ�����϶���;
								����Ǳ��ʽ��ǣ��ʽAGV������������ִ���½�����;
								�Բ泵ʽAGV�����ǻ���ִ���½�����;
								�Ի�е��AGV�����ǻ�е��ִ���·Ŷ���,����е��AGV��ͬһ������Ҫ��ɶ����ͬ�Ķ���ʱ,ͨ�����Ӷ�����ִ����Ӧ�Ķ���;
								ִ�ж���ǰAGVӦ�ж��Ƿ��ڿ�ִ�д˶�����ָ��λ����.������,��Ӧ����ǰ�ر꿨��Ϊ0���ϱ��쳣.
								����ʼִ�ж���,�򽫶���״̬��Ϊִ����.
						ִ��״̬:������ɺ󽫶���״̬��Ϊִ�����.ӵ�л��ϼ�⹦�ܵ�AGVӦ��ȷ�ϻ������뿪AGV���ٽ�����״̬��Ϊִ�����.
						ִ�����״̬:������.
						*/
};

/*! @brief AGV����״̬��Ϣ�� */
enum _AGVACTIONSTATUS
{
	_AGVACTSTA_NONE,	/*!< ����δִ��״̬ */
	_AGVACTSTA_EXE,		/*!< ����ִ����״̬ */
	_AGVACTSTA_FINISH,	/*!< ����ִ�����״̬ */
	_AGVACTSTA_PAUSE,	/*!< ����ִ����ͣ״̬ */
};

/*! @brief AGV�쳣��Ϣ��*/
enum _AGVERROR
{
	_AGVERR_NONE,		/*!< ��/����
						˵��:AGV��������.
						��λ��:������.
						��λ��:������.
						*/
	_AGVERR_MISS,		/*!< �Ѵ�/�޷���λ
						˵��:�����û���AGV�ָ��������,��ֹ���Ѵ���ɵ��������̿���.
						��λ��:�ϱ��쳣���û�.
						��λ��:���״̬Ϊֹͣ.
						*/
	_AGVERR_PATH,		/*!< ·���쳣
						˵��:����λ�����͵��ƶ�ָ����󣨼�û�ж�Ӧ���ƶ�·���������.
						��λ��:�ϱ��쳣���û�.
						��λ��:���״̬Ϊֹͣ.
						*/
	_AGVERR_LOCATE,		/*!< �����쳣
						˵��:��AGVδ������λ��������ָ��λ�������.��λ��Ӧ���������ƶ����ƶ�����,ʹAGV���µ���ָ��λ��,��ִ֤�ж����İ�ȫ��.
						��λ��:�ϱ��쳣���û�.
						��λ��:����ǰ����������.
						*/
	_AGVERR_DIRECTION,	/*!< �����쳣
						˵��:��ȱ����Գ���������쳣��
						��λ��:�ϱ��쳣���û�.
						��λ��:���״̬Ϊֹͣ.
						*/
	_AGVERR_LIFTUP,		/*!< �����쳣 */
	_AGVERR_ACT = -2,	/*!< �����쳣
						˵��:��PC��λ��������ִ��ĳһ������ʱ��δ��������.
						��λ��:�ϱ��쳣���û�.
						��λ��:������.
						*/
	_AGVERR_COMM,		/*!< ͨ���쳣
						˵��:��PC��λ���������޷����ͱ��Ļ�ʱ��δ���ձ��������.
						��λ��:�ϱ��쳣���û�.
						��λ��:������.
						*/
};

/*! @brief ������״̬ */
enum _AGVLIFTER
{
	_AGVLIFTER_DOWN,	/*!< ���� */
	_AGVLIFTER_UP,		/*!< ���� */
};

/*! @brief �ػ�״̬ */
enum _AGVCARGO
{
	_AGVCARGO_EMPTY = 0,	/*!< ���� */
	_AGVCARGO_FULL = 255,	/*!< ���� */
};

/*!< @brief ���������� */
enum _CTRLERROR
{
	ERROR_NONE,			/*!< �����ɹ� */
	ERROR_END,			/*!< �յ���� */
	ERROR_CONNECT,		/*!< ���Ӵ��� */
	ERROR_BATTERY,		/*!< ��ش��� */
	ERROR_STATUS,		/*!< ״̬���� */
	ERROR_INTERNET,		/*!< ������� */
};

typedef void(__stdcall* UpdateAGV)(void*, void*);

/*!
 * @class CAGV
 * @brief ����SOCKETͨ��,����RFID����λ��AGV��
*/
class CAGV :
	public TCPClient
{
public:
	CAGV();
	CAGV(unsigned char byNo, UpdateAGV funcPrt = nullptr, void* ptr = nullptr);
	CAGV(unsigned char byNo, UpdateAGV funcPrt, void* ptr,const char* strSrvIP, const char* strLocalIP, int nSrvPort = -1, int nLocalPort = -1);
	CAGV(unsigned char byNo, UpdateAGV funcPrt, void* ptr, const wchar_t* wstrSrvIP, const wchar_t* wstrLocalIP, int nSrvPort = -1, int nLocalPort = -1);
	CAGV(unsigned char byNo, UpdateAGV funcPrt, void* ptr, const string strLocalIP, string strSrvIP = "", int nLocalPort = -1, int nSrvPort = -1);
	~CAGV();

protected:
	unsigned char m_byNo;					/*!< ��� */

protected:
	unsigned char m_byStatus;				/*!< ״̬ */
	char m_chSpeed;							/*!< �ٶ� 0 - 100ǰ���ٶ� -0 - -100�����ٶ� */
	unsigned char m_byPower;				/*!< ���� 0%-100% 0%û��,1%-20%��ѹ����,20%-90%������,90%-100%���� */
	unsigned short m_usCurRFID;				/*!< �н���ǰRFID */
	unsigned short m_usOldRFID;				/*!< �н���һRFID */
	unsigned short m_usNextRFID;			/*!< �н���һRFID*/
	unsigned short m_usEndRFID;				/*!< �ƶ��յ�RFID */
	unsigned char m_byCargo;				/*!< �ػ�״̬ 0-255 0���� 255���� */
	unsigned char m_byLifter;				/*!< ������״̬ 0���� 1���� */
	char m_chError;							/*!< �쳣��Ϣ */
	unsigned short m_usAction;				/*!< ִ�еĶ��� 0�޶��� 1�ƶ� 2�����ƶ� 3��ͨ���� 4���������� 5�������½� */
	unsigned char m_byActStatus;			/*!< ����״̬  0δִ�� 1ִ���� 2ִ����� 3��ִͣ��*/

protected:
	thread *m_pThreadSend;					/*!< ���ݷ����߳� */
	DataBuffer m_bufferReceive;				/*!< ������ ���Դ���δ��������� */
	HANDLE m_hShutdownEvent;				/*!< �ر��¼� */
	list<DataBuffer> m_listSingle;			/*!< ���η��͵������б� */
	list<DataBuffer> m_listWhile;			/*!< ѭ�����͵������б� */
	Json::CharReaderBuilder m_JsonBuilder;
	Json::CharReader* m_pJsonReader;
	JSONCPP_STRING m_JstrErrs;
	void* m_pFunc;
	UpdateAGV m_funcUpdateAGV;
	thread* m_pThreadConnect;				/*!< �����߳� */
	HANDLE m_hDestroyEvent;					/*!< �ر��¼� */
	std::mutex m_mutexLock;					/*!< ���ݷ��ͻ����� */

protected:
	static list<DataBuffer> m_listProcess;	/*!< ����������ݱ����б� */
	static mutex m_mutexProcess;			/*!< ���������ݻ����� */

public:
	/*!
	 * @brief �ƶ�
	 * 
	 * ����AGV�ƶ���ָ����RFID��
	 * AGV���յ�ָ������跴��������
	 * ����ͨ��AGV������״̬��ִ�еĶ������ƶ����յ�RFID���ж�ָ���Ƿ�AGVִ��
	 * @param unsigend short �ƶ��յ�RFID�����
	*/
	int MoveTo(const unsigned short usRFID);

	/*!
	 * @brief �����ƶ�
	 * 
	 * ����AGV�����ƶ����ѵ����RFID��
	 * AGV���յ�ָ������跴��������
	 * ����ͨ��AGV������״̬��ִ�еĶ������ƶ����յ�RFID���ж�ָ���Ƿ�AGVִ��
	*/
	int RemoveTo();

	/*!
	 * @brief ��ͨ����ͨ��
	 *
	 * ����AGV��ָ����RFID����ͨ��
	 * AGV���յ�ָ������跴��������
	 * ����ͨ��AGV������״̬��ִ�еĶ������ƶ����յ�RFID���ж�ָ���Ƿ�AGVִ��
	 * @param unsigned short ͨ��RFID�����
	*/
	int TrafficPass(const unsigned short usRFID);

	/*!
	 * @brief ��ͨ����ֹͣ
	 *
	 * ����AGV��ָ����RFID����ʱֹͣ
	 * AGV���յ�ָ����跴��������
	 * {"Message={},Result="Sucess/Failed"}
	 * @param unsigned short ֹͣRFID�����
	*/
	int TrafficStop(const unsigned short usRFID);

	/*!
	 * @brief �ػ�
	 *
	 * ����AGV�رյ�Դ
	 * AGV���յ�ָ������跴��������
	*/
	int Shutdown();

	/*!
	 * @brief ��ͣ
	 *
	 * ����AGV��ͣ
	 * AGV���յ�ָ������跴��������
	 * ����ͨ��AGV������״̬��ִ�еĶ������ƶ����յ�RFID���ж�ָ���Ƿ�AGVִ��
	*/
	int Scream();

	/*!
	 * @brief ��λ
	 *
	 * ����AGV��λ(�ָ���ͣ��״̬,ʹAGV���½��ճ������)
	 * AGV���յ�ָ������跴��������
	 * ����ͨ��AGV������״̬��ִ�еĶ������ƶ����յ�RFID���ж�ָ���Ƿ�AGVִ��
	*/
	int Reset();

	/*!
	 * @brief ����
	*/
	int Restart();

	/*!
	 * @brief ����������
	 *
	 * ����AGV����������
	 * AGV���յ�ָ������跴��������
	 * ����ͨ��AGV������״̬��ִ�еĶ������ƶ����յ�RFID���ж�ָ���Ƿ�AGVִ��
	 * @param unsigend short ִ�ж�����RFID�����
	*/
	int LiftUp(const unsigned short usRFID);

	/*!
	 * @brief �������½�
	 *
	 * ����AGV�������½�
	 * AGV���յ�ָ������跴��������
	 * ����ͨ��AGV������״̬��ִ�еĶ������ƶ����յ�RFID���ж�ָ���Ƿ�AGVִ��
	 * @param unsigend short ִ�ж�����RFID�����
	*/
	int LiftDown(const unsigned short usRFID);

	/*!
	 * @brief ֹͣ
	 *
	 * ����AGVֹͣ��ǰִ�еĶ���(�����ƶ��ȶ�������)
	 * AGV���յ�ָ������跴��������
	 * ����ͨ��AGV������״̬��ִ�еĶ������ƶ����յ�RFID���ж�ָ���Ƿ�AGVִ��
	*/
	int Stop();

	/*!
	 * @brief ��������
	 *
	 * ����AGVִ������δ�ڳ������趨�Ķ���
	 * AGV���յ�ָ������跴��������
	 * ����ͨ��AGV������״̬��ִ�еĶ������ƶ����յ�RFID���ж�ָ���Ƿ�AGVִ��
	 * @param unsigned short ִ�ж�������ʼRFID�����
	 * @param unsigend short ִ�ж����Ľ���RDIF�����
	 * @param unsigend short ִ�еĶ���
	*/
	int Action(const unsigned short usStartRFID,const unsigned short usEndRFID,const unsigned short usAction);

	/*!
	 * @brief ״̬����
	 *
	 * ����AGV�ĵ�ǰRFID����š�����״̬���ٶ�
	 * AGV���յ�ָ������跴��������
	 * ����ͨ��AGV������״̬��ִ�еĶ������ƶ����յ�RFID���ж�ָ���Ƿ�AGVִ��
	 * @param unsigned short ��ǰRFID�����
	 * @param unsigned char ��ǰ״̬
	 * @param char ��ǰ�ٶ�
	*/
	int StatusSet(const unsigned short usRFID, const unsigned char byStatus,const char chSpeed);

	/*!
	 * @brief ��ͣ
	 *
	 * ��ͣAGV��ǰ����ִ�еĶ���
	 * AGV���յ�ָ������跴��������
	 * ����ͨ��AGV������״̬��ִ�еĶ������ƶ����յ�RFID���ж�ָ���Ƿ�AGVִ��
	*/
	int Pause();

	/*!
	 * @brief ����
	 *
	 * ����AGV����ִ�еĶ���
	 * AGV���յ�ָ������跴��������
	 * ����ͨ��AGV������״̬��ִ�еĶ������ƶ����յ�RFID���ж�ָ���Ƿ�AGVִ��
	*/
	int Continue();

	/*!
	 * @brief �����ٶ�
	 *
	 * ����AGV��ǰ�ٶ�
	 * AGV���յ�ָ������跴��������
	 * ����ͨ��AGV������״̬��ִ�еĶ������ƶ����յ�RFID���ж�ָ���Ƿ�AGVִ��
	 * @param char ��ǰ����ٶ�%
	*/
	int SpeedControl(char chSpeed);

	/*!
	 * @brief ȫ�߼�ͣ
	*/
	int AllScream();

public:
	/*!
	 * @brief ��ȡ��ǰ״̬
	 * @return unsigned char ��ǰ״̬
	*/
	const unsigned char GetStatus() const;

	/*!
	 * @brief ��ȡ�н���ǰRFID�����
	 * @return unsigned short �н���ǰRFID�����
	*/
	const unsigned short GetCurRFID() const;

	/*!
	 * @brief ��ȡ�н���һRFID�����
	 * @return unsigned short �н���һRFID�����
	*/
	const unsigned short GetOldRFID() const;

	/*!
	 * @brief ��ȡ�н���һRFID�����
	 * @return unsigend short �н���һRFID�����
	*/
	const unsigned short GetNextRFID() const;

	/*!
	 * @brief ��ȡ�н��յ�RFID�����
	 * @return unsigned short �н��յ�RFID�����
	*/
	const unsigned short GetEndRFID() const;

	/*!
	 * @brief ��ȡ��ǰ�ٶ�
	 * @return char ��ǰ�ٶ�
	*/
	const char GetSpeed() const;

	/*!
	 * @brief ��ȡ��ǰ����
	 * @return unsigned char ��ǰ����
	*/
	const unsigned char GetPower() const;

	/*!
	 * @brief ��ȡ��ǰ�ػ�״̬
	 * @return unsigned char ��ǰ�ػ�״̬
	*/
	const unsigned char GetCargo() const;

	/*!
	 * @brief ��ȡ��ǰ������״̬
	 * @return unsigned char ��ǰ������״̬
	*/
	const unsigned char GetLifter() const;

	/*!
	 * @brief ��ȡ��ǰ�쳣
	 * @return char ��ǰ�쳣
	*/
	const char GetError() const;

	/*!
	 * @brief ��ȡ��ǰ����
	 * @return unsigned short ��ǰ����
	*/
	const unsigned short GetAction() const;

	/*!
	 * @breif ��ȡ��ǰ����״̬
	 * @return unsigned char ��ǰ����״̬
	*/
	const unsigned char GetActionStatus() const;

public:
	/*!
	 * @brief �����н���ǰRFID�����
	 * @param const unsigned short ��RFID�����
	 * @return const unsigned short ��RFID�����
	*/
	const unsigned short UpdateCurRFID(const unsigned short usRFID);

	/*!
	 * @brief �����н���һRFID�����
	 * @param const unsigned short ��RFID�����
	 * @return const unsigned short ��RFID�����
	*/
	const unsigned short UpdateNextRFID(const unsigned short usRFID);

	/*!
	 * @brief �����н��յ�RFID�����
	 * @param const unsigned short ��RFID�����
	 * @return const unsigned short ��RFID�����
	*/
	const unsigned short UpdateEndRFID(const unsigned short usRFID);

	/*!
	 * @brief ����״̬
	 * @param const unsigend char ��״̬
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdateStatus(const unsigned char byStatus);

	/*!
	 * @brief �����ٶ�
	 * @param const char ���ٶ�
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdateSpeed(const char chSpeed);

	/*!
	 * @brief ���µ���
	 * @param unsigned char �µ���
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdatePower(const unsigned char byPower);

	/*!
	 * @brief �����ػ�״̬
	 * @param unsigned char ���ػ�״̬
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdateCargo(const unsigned char byCargo);

	/*!
	 * @brief ����������״̬
	 * @param unsigned char ��������״̬
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdateLifter(const unsigned char byLifter); 

	/*!
	 * @brief �����쳣��Ϣ
	 * @param char ���쳣��Ϣ
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdateError(const char chError);

	/*!
	 * @brief ���¶���
	 * @param unsigned short �¶���
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdateAction(const unsigned short usAction);

	/*!
	 * @brief ���¶���״̬
	 * @param unsigned short �¶���״̬
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdateActStatus(const unsigned char byActStatus);

public:
	/*!
	 * @brief ���ӿͻ���
	 * @param const SOCKET �ɷ�����Accept���յĿͻ���SOCKETͨ��
	 * @return bool ���ӳɹ�����true,���򷵻�false
	*/
	bool Connect(const SOCKET socketClient);

	/*!
	 * @brief ���ӷ����
	 * @param const char* �����IP��ַ
	 * @param const unsigned short ����˶˿�
	 * @return bool ���ӳɹ�����true,���򷵻�false
	*/
	bool Connect(const char* strSrvIP, const unsigned short usSrvPort);

	/*!
	 * @brief ���ӷ����
	 * @param const wchar_t* �����IP��ַ
	 * @param const unsigned short ����˶˿�
	 * @return bool ���ӳɹ�����true,���򷵻�false
	*/
	bool Connect(const wchar_t* wstrSrvIP, const unsigned short usSrvPort);

	/*!
	 * @brief �ж�����
	*/
	void Break();

	void Release();

	/*!
	 * @brief �Ƿ�����
	 * @return bool �����ӷ���true,���򷵻�false
	*/
	bool IsConnected();

	/*!
	 * @brief ��ȡ���������ݰ�
	 * @return DataBuffer ��ȡ���������ݰ�
	*/
	static DataBuffer GetProcessPackage();

	/*!
	 * @brief ��ȡAGV���
	 * @return unsigned char AGV���
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief ��ʼ������״̬
	*/
	int InitAttribute();

	bool StartConnectThread();

protected:
	/*! 
	 * @brief ����
	 *
	 * ��ȡAGV��ǰʵʱ״̬��Ϣ
	 * AGV���յ�ָ����跴��������
	 * {"Message"={},Result={"Status"="","Speed"="","Power"="","CurRFID"="","EndRFID"="","Cargo"="","Lifter"="","Error"="","Action"="","ActStatus"=""}}
	*/
	void Heartbeat();

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
	 * @brief ���ݴ���
	 * @param unsigned char ���յ�����ָ��
	 * @param unsigned int ���յ����ݳ���
	*/
	void ProcessData(const unsigned char * pData, const unsigned int unSize);

	/*!
	 * @brief �������ݰ�
	 * @param const unsigned char* ���ݰ�ָ��
	 * @param const int ���ݰ�����
	*/
	void ProcessPackage(const unsigned char* pPack, const int nPackLen);

	/*!
	 * @brief �ϳɱ���
	 * @param char* Json�ַ���
	 * @return DataBuffer �ϳɺ������
	*/
	DataBuffer CreatePackage(const char* lpszStr);

	/*!
	 * @brief �ϳɱ���
	 * @param char* ���ݿ�ָ��
	 * @param int ���ݿ鳤��
	 * @return DataBuffer �ϳɺ������
	*/
	DataBuffer CreatePackage(const char* lpszStr,const int nLen);

	/*!
	 * @brief ���ͱ���
	 * @param char* Json�ַ���
	*/
	void SendPackage(const char* lpszStr);
	
	/*!
	 * @brief ���ͱ���
	 * @param char* ���ݿ�ָ��
	 * @param int ���ݿ鳤��
	*/
	void SendPackage(const char* pData,const int nLen);

	/*!
	 * @brief ת��
	 * @param const char* ��ת�������
	 * @param const int ��ת������ݳ���
	 * @param char* ת��������
	 * @param int& ת�������ݳ���
	*/
	void Translation(const char* pSourceData, int nSourceLen, char* pTransData, int& pTransLen);

	/*!
	 * @brief ��ת��
	 * @param const char* �跴ת�������
	 * @param const int �跴ת������ݳ���
	 * @param char*Դ����
	 * @param int& Դ���ݳ���
	*/
	void Retranslation(const char* pTransData, int pTransLen, char* pSourceData, int& nSourceLen);

	/*!
	 * @brief ��������ջظ��������б�
	 * @param const char* ������ָ��
	 * @param const int �����峤��
	*/
	void InsertIntoWhileList(const char* pData, const int nLen);
	void InsertIntoWhileList(const char* string);

	/*!
	 * @brief �������ݷ����б�
	 * @param const char* ������ָ��
	 * @param const int �����峤��
	*/
	void InsertIntoSingleList(const char* pData, const int nLen);

	/*!
	 * @brief �Ӵ����ջظ��������б��Ƴ�
	 * @param const char* ������ָ��
	 * @param const int �����峤��
	*/
	void RemoveFromWhileList(const char* pData, const int nLen);

	/*!
	 * @brief ������������ݰ�
	 * @param const char* ������ָ��
	 * @param const int �����峤��
	*/
	static void InsertIntoProcessList(const char* pData, const int nLen);

	void ConnectThread();

	void EndConnectThread();
};

#endif //!_AGV_H
