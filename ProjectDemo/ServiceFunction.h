/*!
 * @file ServiceFunction.h
 * @brief ʵ�ֵ���ϵͳҵ���ܵ��ļ�
 * @author FanKaiyu
 * @date 2019-04-15
 * @version 1.0
 *
 * update 2019-05-12 FanKaiyu ȡ����2��е������2�λ���
*/
#pragma once

#ifndef _SERVICEFUNCTION_H
#define _SERVICEFUNCTION_H

#include "BaseFunction.h"
#include "../MESDemo/CMES.h"
#include "../ZIGBEECallDemo/CZigbeeControler.h"
#include "../ZIGBEECallDemo/CZigbeeCaller.h"
#include "../ADO/ADOConn.h"
#include "CStation.h"
#include <map>
#include <list>

using namespace std;

/*!< @brief �ӹ����� */
enum _MES_PROCESS
{
	_MES_NG,								/*!< NG��λ */
	_MES_BoxStorageArea = 1,				/*!< �������� */
	_MES_BoxAccessorierInstallBuffer,		/*!< ���帨����װ���� */
	_MES_BoxAccessoriesInstall,				/*!< ���帨����װ:WP-001#1*/
	_MES_WaterPipesInstall,					/*!< ˮ�ܰ�װ:WP-002#1 */
	_MES_LowerModuleLifting,				/*!< �²�ģ���װ,��1��е��:WP-003#1 */
	_MES_LowerModuleInstall,				/*!< �²�ģ�鰲װ:WP-004#1 */
	_MES_UpperModuleLifting,				/*!< �ϲ�ģ���װ,��2��е��:WP-005#1 */
	_MES_UpperModuleInstall,				/*!< �ϲ�ģ�鰲װ,����2��:WP-006#1 */
	_MES_OfflineBDUAssembly,				/*!< ����BDU��װ:WP-007#1 */
	_MES_BoxBDUAssembly,					/*!< ����BDU��װ:WP-008#1 */
	_MES_TheAuxiliaryControlPanelSwipes,	/*!< Ԥ����վ1(�ӿذ�ˢд) */
	_MES_Unknown2,							/*!< Ԥ����վ2(δ֪) */
	_MES_EOLTest,							/*!< EOL����:WP-009#1 */
	_MES_BatteryBoxTest1,					/*!< ��ذ�����1:WP-010#1 */
	_MES_BoxCoverInstallBuffer,				/*!< �����ϸǰ�װ���� */
	_MES_BatteryBoxTest2,					/*!< ��ذ�����2:WP-010#1 */
	_MES_BoxCoverInstall,					/*!< �����ϸǰ�װ:WP-011#1 */
	_MES_BoxAirTightTest,					/*!< �������ܲ���:WP-012#1 */
	_MES_BoxUnloadBuffer,					/*!< ��Ʒ�����ϻ��� */
	_MES_BoxUnload,							/*!< ��Ʒ������ */
};

#define _TASK_STATUS_NONE		"δִ��"
#define _TASK_STATUS_PRE		"׼���׶�"
#define _TASK_STATUS_EXE		"����ִ��"
#define _TASK_STATUS_FINISH		"�����"
#define _TASK_STATUS_CANCEL		"��ȡ��"

#define _TASK_STATUS_NONE_W		L"δִ��"
#define _TASK_STATUS_PRE_W		L"׼���׶�"
#define _TASK_STATUS_EXE_W		L"����ִ��"
#define _TASK_STATUS_FINISH_W	L"�����"
#define _TASK_STATUS_CANCEL_W	L"��ȡ��"

#define _TASK_TYPE_CALL			"����"
#define _TASK_TYPE_PASS			"����"
#define _TASK_TYPE_NG			"NG"
#define _TASK_TYPE_RETURN		"����"
#define _TASK_TYPE_HANDFULL		"�ֶ������ƶ�"
#define _TASK_TYPE_HANDEMPTY	"�ֶ������ƶ�"

#ifndef TM_UPDATE_KEY
#define TM_UPDATE_KEY WM_USER+7		/*!< ����Ȩ����Ϣ���߳���Ϣ */
#endif //!TM_UPDATE_KEY

/*!
 * @class ServiceFunction
 * @brief ʵ�ֵ���ϵͳҵ���ܵ���
*/
class ServiceFunction :
	public BaseFunction
{
public:
	ServiceFunction();
	~ServiceFunction();

protected:
	CMES* m_pMes;									/*!< MESָ�� */
	ADOConn m_ado;									/*!< ���ݿ�ADO����*/
	ZGBCtrler* m_pZGBCtrler;						/*!< Zigbee������ָ�� */
	map<unsigned char, CStation*> m_mapStations;	/*!< ��λ�� */
	map<unsigned char, ZGBCaller*> m_mapCallers;	/*!< Zigbee�������� */
	bool m_bScream;									/*!< ȫ�߼�ͣ��ʶ */
	bool m_bKey;									/*!< Ȩ�ޱ�ʶ */
	
protected:
	string m_strMES;		/*!< ���Դ��MES������Ϣ��JSON�ַ���:{"ProductLineNO":"PACK-Line-1","WorkPositionNo":{0:"",1:"WP-001#1",2:"WP-002#1",3:"WP-003#1",4:"WP-004#1",5:"WP-005#1",6:"WP-006#1",7:"WP-007#1",8:"WP-008#1",9:"WP-009#1",10:"WP-010#1",11:"WP-011#1",12:"WP-012#1",13:""}} */
	string m_strADO;		/*!< ���Դ���ADO������Ϣ��JSON�ַ���:{"Connect":"","User":"HarbinBrain","Password":"0451HarbinBrain58973970"} */
	string m_strZigbee;		/*!< ���Դ���ZIGBEE������Ϣ��JSON�ַ���:{"Control":{"No":1,"Port":0,"Baud":115200,"Parity":"N","DataBits":8,"StopBits":1},"Caller":[{"No":1,"Station":0,"PackOutBound":true},...]} */
	string m_strStation;	/*!< ���Դ��湤λ������Ϣ��JSON�ַ���:{"Station":[{"No":1,"Name":"","Location":0,"Process":0},{},...]} */

protected:
	map<unsigned char, Json::Value*> m_mapStrUpdateCallers;	/*!< ���Դ�ź�����������µ�JSON�ַ��� {"No":1,"PackOutBound":true,"Call":0,"CancelCall":0,"Pass":0,"CancelPass":0,"NG":0,"CancelNG":0,"Scream":0} */
	map<unsigned char, Json::Value*> m_mapStrUpdateTasks;	/*!< ���Դ�����������µ�JSON�ַ��� */
	bool m_bUpdateCaller[UCHAR_MAX];

protected:
	bool AcceptClient(SOCKET socket);

	void Function();

	void ProcessAGVStatus(void* pUser);

	/*!
	 * @brief ��ʼ��MES
	*/
	bool InitMES();

	/*!
	 * @brief ��ʼ��ADO
	*/
	void InitADO();

	/*!
	 * @brief ��ʼ����λ
	*/
	bool InitStations();

	/*!
	 * @brief ��ʼ��ZIGBEE
	*/
	bool InitZigbee();
public:
	/*!
	 * @brief ��ʼ������
	*/
	bool Init(UpdateStatus func, void* ptr);

	/*!
	 * @brief ��JSON�ַ�������ȡMES������Ϣ
	 * @param const char* JSON�ַ���
	*/
	void JsonStringToMES(const char* strJson);
	void JsonStringToMES(const wchar_t* wstrJson);

	/*!
	 * @brief ��JSON�ļ�����ȡMES������Ϣ
	 * @param const char* JSON�ļ�·��
	*/
	void JsonFileToMES(const char* strFile);
	void JsonFileToMES(const wchar_t* wstrFile);

	/*!
	 * @brief ��JSON�ַ�������ȡADO������Ϣ
	 * @param const char* JSON�ַ���
	*/
	void JsonStringToADO(const char* strJson);
	void JsonStringToADO(const wchar_t* wstrJson);

	/*!
	 * @brief ��JSON�ļ�����ȡADO������Ϣ
	 * @param const char* JSON�ļ�·��
	*/
	void JsonFileToADO(const char* strFile);
	void JsonFileToADO(const wchar_t* wstrFile);

	/*!
	 * @brief ��JSON�ַ�������ȡ��λ������Ϣ
	 * @param const char* JSON �ַ���
	*/
	void JsonStringToStations(const char* strJson);
	void JsonStringToStations(const wchar_t* wstrJson);

	/*!
	 * @brief ��JSON�ļ�����ȡ��λ��Ϣ
	 * @param const char* JSON�ļ�·��
	*/
	void JsonFileToStations(const char* strFile);
	void JsonFileToStations(const wchar_t* wstrFile);

	/*!
	 * @brief ��JSON�ַ�������ȡZigbee������Ϣ
	 * @param const char* JSON�ַ���
	*/
	void JsonStringToZigbee(const char* strJson);
	void JsonStringToZigbee(const wchar_t* wstrJson);

	/*!
	 * @brief ��JSON�ļ�����ȡZigbee������Ϣ
	 * @param const char* JSON�ļ�·��
	*/
	void JsonFileToZigbee(const char* strFile);
	void JsonFileToZigbee(const wchar_t* wstrFile);

	/*!
	 * @brief ��������
	 * @param unsigned long ��Ϣ�߳�ID
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool Open(UpdateStatus func, void* ptr);

	/*!
	 * @brief �رչ���
	*/
	void Close();

	/*!
	 * @brief �ͷ��ڴ�
	*/
	void Release();

	void ReleaseString();

	/*!
	 * @brief Ȩ������
	 * @param bool Ȩ��
	 * @return bool ���óɹ�����true,���򷵻�false
	*/
	bool SetKey(bool bKey);

	/*!
	 * @brief �������
	 * @param int �����
	 * @param const char* ����ע
	 * @return bool ɾ���ɹ�����true,���򷵻�false
	*/
	bool FinishTask(const int nNo, const char* _Format, ...);
	bool FinishTask(const int nNo, const wchar_t* _Format, ...);
	bool CancelTask(const int nNo, const char* _Format, ...);
	bool CancelTask(const int nNo, const wchar_t* _Format, ...);
	bool CancelTask(const unsigned char byAGVNo, const char* strType, const char* _Format, ...);
	bool CancelTask(const unsigned char byAGVNo, const wchar_t* strType, const wchar_t* _Format, ...);

	/*!
	 * @brief ��������
	 * @param unsigned char ������ʼ��վ���
	 * @param unsigend char ������ֹ��վ���
	 * @param unsigned char ִ������AGV���
	 * @return bool �����ɹ�����true
	*/
	bool CreateTask(const unsigned char byStartWP, const unsigned char byEndWP, unsigned char byExeAGV, const char* strType);

	/*!
	 * @brief �û�����AGV
	 * @param const char* JSON�ַ���
	 * @return int ���ز����������ֵ
	*/
	int ControlAGV(Json::Value& JsonValue);

protected:
	/*!
	 * @brief PACK����У��
	 * @param const char* ��վ���
	 * @return bool У��ɹ�����true,���򷵻�false
	*/
	bool PackOutBound(const char* strWorkPosition);

	/*!
	 * @brief ����
	 * @param unsigned char ������
	 * @param unsigned short ����
	 * @param const char* ��վ���
	*/
	void Pass(unsigned char byProcess, unsigned short usRFID);

	/*!
	 * @brief ����
	 * @param unsigned char ������
	 * @param unsigned short ����
	*/
	void Call(unsigned char byProcess, unsigned short usRFID);

	/*! 
	 * @brief NG
	 * @param unsigned char ������
	 * @param unsigned short ����
	*/
	void NG(unsigned char byProcess, unsigned short usRFID);

	/*!
	 * @brief ȫվ��ͣ
	*/
	void Scream();

	/*!
	 * @brief ȡ������
	 * @param unsigned char ������
	 * @param unsigned short ����
	*/
	void CancelPass(unsigned char byProcess, unsigned short usRFID);

	/*!
	 * @brief ȡ������
	 * @param unsigned char ������
	 * @param unsigned short ����
	*/
	void CancelCall(unsigned char byProcess, unsigned short usRFID);

	/*!
	 * @brief ȡ��NG
	 * @param unsigned char ������
	 * @param unsigned short ����
	*/
	void CancelNG(unsigned char byProcess, unsigned short usRFID);

	/*!
	 * @brief ȡ����ͣ
	*/
	void CancelScream();

	/*!
	 * @brief ��ȡ����AGV
	 * @param unsigned char ������
	 * @param unsigned char ��λ
	*/
	CAGV* GetFreeAGV(CStation* pStation);

	/*!
	 * @brief ɾ������
	 * @param int �����
	 * @param const char* ����ע
	 * @return bool ɾ���ɹ�����true,���򷵻�false
	*/
	bool DeleteTask(const int nNo);

	/*!
	 * @brief ��������ע
	*/
	bool UpdateTaskText(const int nNo, const char* _Format, ...);
	bool UpdateTaskText(const int nNo, const wchar_t* _Format, ...);

	/*!
	 * @brief ִ������
	*/
	bool ExecuteTask(const int nTaskNo, const unsigned char byAGVNo,const char* strStatus);

	/*!
	 * @brief ��ȡ������
	 * @param unsigned char ����
	 * @return list<ZGBCaller*> ��˹���󶨵ĺ������б�
	*/
	list<ZGBCaller*> GetCaller(const unsigned char byProcess);

	/*!
	 * @brief ��ȡ������
	 * @param unsigned short RFID��
	 * @return ZGBCaller* ���RFID���󶨵ĺ�����
	*/		
	ZGBCaller* GetCaller(const unsigned short usRFID);

	/*!
	 * @brief ��ȡ������
	 * @param unsigned char ��λ���
	 * @return ZGBCaller* ���RFID���󶨵ĺ�����
	*/
	ZGBCaller* GetCallerS(const unsigned char byStation);

	/*!
	 * @brief ��ȡ��վ
	 * @param unsigned char ����
	 * @return list<CStation*> ��˹���󶨵Ĺ�վ�б�
	*/
	list<CStation*> GetStation(const unsigned char byProcess);

	/*!
	 * @brief ��ȡ��λ
	 * @param unsigned short RFID��
	 * @return CStation* ���RFID���󶨵Ĺ�վ
	*/
	CStation* GetStation(const unsigned short usRFID);

	/*!
	 * @brief AGV������
	*/
	void ProcessAGVTask();

	/*!
	 * @brief �����������
	 * @param int ������
	 * @param unsigned char ������ʼ��λ
	 * @param unsigned char ������ֹ��λ
	 * @param unsigned char ִ������AGV���
	 * @param unsigned char ����״̬
	*/
	void ProcessAGVCallTask(const int nTaskNo,const unsigned char byStart,const unsigned char byEnd,const unsigned char byAGVNo,const char* strStatus);

	/*!
	 * @brief ����NG����
	 * @param int ������
	 * @param unsigned char ������ʼ��λ
	 * @param unsigned char ������ֹ��λ
	 * @param unsigned char ִ������AGV���
	 * @param unsigned char ����״̬
	*/
	void ProcessAGVNGTask(const int nTaskNo, const unsigned char byStart, const unsigned char byEnd, const unsigned char byAGVNo, const char* strStatus);

	/*!
	 * @brief �����������
	 * @param int ������
	 * @param unsigned char ������ʼ��λ
	 * @param unsigned char ������ֹ��λ
	 * @param unsigned char ִ������AGV���
	 * @param unsigned char ����״̬
	*/
	void ProcessAGVPassTask(const int nTaskNo, const unsigned char byStart, const unsigned char byEnd, const unsigned char byAGVNo, const char* strStatus);

	/*!
	 * @brief ����������
	 * @param int ������
	 * @param unsigned char ������ʼ��λ
	 * @param unsigned char ������ֹ��λ
	 * @param unsigned char ִ������AGV���
	 * @param unsigned char ����״̬
	*/
	void ProcessAGVReturnTask(const int nTaskNo, const unsigned char byStart, const unsigned char byEnd, const unsigned char byAGVNo, const char* strStatus);

	/*!
	 * @brief �����ֶ���������
	 * @param int ������
	 * @param unsigned char ������ʼ��λ
	 * @param unsigned char ������ֹ��λ
	 * @param unsigned char ִ������AGV���
	 * @param unsigned char ����״̬
	*/
	void ProcessAGVHandFullTask(const int nTaskNo, const unsigned char byStart, const unsigned char byEnd, const unsigned char byAGVNo, const char* strStatus);

	/*!
	 * @brief �����ֶ�����
	 * @param int ������
	 * @param unsigned char ������ʼ��λ
	 * @param unsigned char ������ֹ��λ
	 * @param unsigned char ִ������AGV���
	 * @param unsigned char ����״̬
	*/
	void ProcessAGVHandEmptyTask(const int nTaskNo, const unsigned char byStart, const unsigned char byEnd, const unsigned char byAGVNo, const char* strStatus);

	/*!
	 * @brief �������źŴ���
	*/
	void ProcessCallSignal();

	/*!
	 * @brief ����Zigbee��ȡ�ı���
	*/
	void ProcessZigbeePackage();

	CAGV* ApplyForBatteryBoxTest1(CStation* pStation);
	CAGV* ApplyForBatteryBoxTest2(CStation* pStation);
	CAGV* ApplyForBoxCoverInstallBuffer(CStation* pStation);
	CAGV* ApplyForEolTest(CStation* pStation);
	CAGV* ApplyForNG(CStation* pStation);
	CAGV* ApplyForOther(CStation* pStation);

	/*!
	 * @brief �ӹ����ϻ�ȡ����AGV
	 * @param CStation* ����AGV�Ĺ�λָ��
	 * @param unsigned char �����Ĺ���
	 * @param bool& ��û�п���AGVʱ���˱�־λ��ֵ�����Ƿ���AGV�������﹤��
	 * @return CAGV* ���ؿ��õ�AGV
	*/
	CAGV* CatchAGVFromProcess(CStation* pStation,const unsigned char byProcess,bool& bContinue);
	CAGV* CatchAGVFromNG(CStation* pStation);
	CAGV* CatchAGVFromOther(CStation* pStation);

	int DoesAGVHaveATask(unsigned char byNo);
	CAGV* IsAGVComing(CStation *pStation);
	CAGV* IsAGVRunning(CStation* pStation);

protected:
	/*!
	 * @brief ���º�����״̬��Ϣ
	*/
	void UpdateZGBCaller(ZGBCaller* pCaller);

	void RecordAGVStatus();
	void RecordCallerStatus();
};

#endif //!_SERVICEFUNCTION_H