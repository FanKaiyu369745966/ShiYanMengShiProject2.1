#pragma once
#ifndef _MAP_H
#define _MAP_H

#include "../Graph/CGraphicAGV.h"
#include "../Graph/CGraphicBackground.h"
#include "../Graph/CGraphicCharger.h"
#include "../Graph/CGraphicRest.h"
#include "../Graph/CGraphicRFID.h"
#include "../Graph/CGraphicStation.h"
#include "../Graph/CGraphicTrack.h"
#include "CGraphicCaller.h"
#include "../json/json.h"
#include "../ADO/ADOConn.h"
#include <map>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

typedef void(__stdcall* UpdateWidget)(Json::Value&,void*);

class CGraphicMap
{
public:
	CGraphicMap(void *pMainFram = nullptr, UpdateWidget func = nullptr);
	~CGraphicMap();

protected:
	static bool g_bInstall;	/*!< ��ʼ����ʶ */

protected:
	mutex m_mutex;

protected:
	CGraphicBackground* m_pBK;							/*!< ����ͼָ�� */
	map<unsigned char, CGraphicAGV*> m_mapAGVs;			/*!< AGVͼ��ָ�� */
	map<unsigned char, CGraphicCharger*> m_mapChargers;	/*!< �����ͼ��ָ�� */
	map<unsigned char, CGraphicRest*> m_mapRests;		/*!< ����λͼ��ָ�� */
	map<unsigned short, CGraphicRFID*> m_mapRFIDs;		/*!< RFID��ͼ��ָ�� */
	map<unsigned char, CGraphicStation*> m_mapStations;	/*!< ��λͼ��ָ�� */
	map<unsigned char, CGraphicCaller*> m_mapCallers;	/*!< ������ͼ��ָ�� */
	list<CGraphicTrack*> m_listTracks;					/*!< �Ź�ͼ��ָ�� */

protected:
	CGraphicCharger* m_pTmpCharger;		/*!< ��ʱ�����ָ�� */
	CGraphicRest* m_pTmpRest;			/*!< ��ʱ����λָ�� */
	CGraphicRFID* m_pTmpRFID;			/*!< ��ʱRFID��ָ�� */
	CGraphicStation* m_pTmpStation;		/*!< ��ʱ��λָ�� */
	CGraphicTrack* m_pTmpTrack;			/*!< ��ʱ�Ź�ָ�� */
	CGraphicCaller* m_pTmpCaller;		/*!< ��ʱ������ָ�� */

protected:
	bool m_bCreateNew;					/*!< �½�ͼ���ʶ */
	bool m_bRefresh;					/*!< ˢ�±�ʶ */
	//bool m_bEdit;						/*!< �༭��ʶ */
	bool m_bMulti;						/*!< ��ѡ��ʶ */
	Json::CharReaderBuilder m_JsonBuilder;
	Json::CharReader* m_pJsonReader;
	JSONCPP_STRING m_JstrErrs;			
	ADOConn m_ado;						/*!< ADO���ݿ����Ӵ����� */
	//bool m_bSaveByADO;				/*!< ͨ��ADO���ݿⴢ������ı�ʶ */
	string m_strADO;					/*!< ���Դ���ADO������Ϣ��JSON�ַ���:{"Connect":"","User":"HarbinBrain","Password":"0451HarbinBrain58973970"} */
	void* m_pMainFram;					/*!< �����ָ�� */
	UpdateWidget m_funcUpdateWidget;	/*!< ֪ͨ����ͼ��������µĻص�����ָ�� */
	bool m_bInstall;					/*!< ��ʼ����ʶ */
	unsigned int m_unNewed;				/*!< �ռ�������ʶ */
	Bitmap *m_pclBmp;

public:
	/*!
	 * @brief ������ͼ��
	 * @param const char* ��¼ͼ�������JSON�ַ���{"Type":"","Param":{"No":0,"Param":{}}}
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	const bool CreateNewWidget(Json::Value& JsonValue);

	/*!
	 * @brief ����ͼ��
	 * @param const char* ��¼ͼ�������JSON�ַ���{"Type":"","Param":{}}
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	const bool FindWidget(Json::Value& JsonValue);

	/*!
	 * @brief �༭ͼ��
	 * @param const char* ��¼ͼ�������JSON�ַ���{"Type":"","Param":{}}
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	const bool EditWidget(Json::Value& JsonValue);

	/*!
	 * @breif ��ȡͼ����ϸ����
	 * @param const char* ��¼ͼ�������JSON�ַ���{"Type":"","Param":{}}
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	Json::Value GetWidget(Json::Value& JsonValue);
	Json::Value GetWidget(PointF ptWinpoint);
#ifdef _AFX
	Json::Value GetWidget(CPoint ptWinpoint);
#endif //_AFX

	/*!
	 * @brief ɾ��ͼ��
	 * @param const char* ��¼ͼ�������JSON�ַ���
	*/
	const bool DeleteWidget(Json::Value& JsonValue);

	/*!
	 * @brief ��קͼ��
	*/
	const HCURSOR Drag(const PointF ptWinPoint);
#ifdef _AFX
	const HCURSOR Drag(const CPoint ptWinPoint);
#endif //_AFX

	/*!
	 * @brief ����
	 * @param pDC ���ھ��
	*/
	void Draw(HDC hdc);

	/*!
	 * @brief �����༭ģʽ
	*/
	void OpenEditMode();

	/*!
	 * @brief �رձ༭ģʽ
	*/
	void CloseEditMode();

	/*!
	 * @brief ��С
	*/
	void ZoomedIn();

	/*!
	 * @brief �Ŵ�
	*/
	void ZoomedOut();

	/*!
	 * @brief ��λ
	*/
	void Reset();

	/*!
	 * @brief �ߴ���
	*/
	void OnSize(RectF rcClient);
#ifdef _AFX
	void OnSize(CRect rcClient);
#endif //_AFX

	/*!
	 * @brief ȡ��
	*/
	void Cancel();

	/*!
	 * @brief ��ʼ��
	 * @return bool δ��ʼ������true,���򷵻�false
	*/
	bool Init(void* pMainFram = nullptr, UpdateWidget func = nullptr);

	/*!
	 * @brief ѡ��ͼ��
	*/
	void Select(const PointF ptWinpoint);
#ifdef _AFX
	void Select(const CPoint ptWinpoint);
#endif //_AFX

	/*!
	 * @brief ѡ�񱳾�
	*/
	void SelectBackground(const PointF ptWinpoint);
#ifdef _AFX
	void SelectBackground(const CPoint ptWinpoint);
#endif //_AFX

	/*!
	 * @brief ȷ��
	*/
	void Confirm(const PointF ptWinpoint);
#ifdef _AFX
	void Confirm(const CPoint ptWinpoint);
#endif //_AFX

	void ConfirmBackground();

	/*!
	 * @brief ɾ��ͼ��
	*/
	void DeleteWidget();

	/*!
	 * @brief ˢ��
	*/
	void Refresh();

	/*!
	 * @brief ��ѡ
	*/
	void MultiSelect();

	/*!
	 * @brief ��ѡ
	*/
	void SignalSelect();

	/*!
	 * @brief ѡ��ȫ��ͼ��
	*/
	void SelectAll();

	/*!
	 * @brief ���³����״̬
	 * @param const char* ��¼�����״̬��Ϣ��JSON�ַ���
	*/
	void UpdateChargerStatus(Json::Value& JsonValue);

	/*!
	 * @brief ����AGV״̬��Ϣ
	 * @param const char* ��¼AGV״̬��Ϣ��JSON�ַ���
	*/
	void UpdateAGVStatus(Json::Value& JsonValue);

	/*!
	 * @brief ����Caller״̬��Ϣ
	 * @param const char* ��¼AGV״̬��Ϣ��JSON�ַ���
	*/
	void UpdateCallerStatus(Json::Value& JsonValue);

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

	void SaveMap();
	void LoadMap();

protected:
	/*!
	 * @brief �ͷ���ʱͼ��
	*/
	void ReleaseTmpWidget();

	/*!
	 * @brief �ͷ�ͼ��
	*/
	void ReleaseWidget();

	/*!
	 * @brief ��ק��ʱͼ��
	*/
	const HCURSOR DragTmpWidget(const PointF ptWinPoint);

	/*!
	 * @brief ��קͼ��
	*/
	const HCURSOR DragWidget(const PointF ptWinPoint);

	/*!
	 * @brief ȷ����ʱͼ��
	*/
	void ConfirmTmpWidget();

	/*!
	 * @brief ȷ��ͼ��
	*/
	void ConfirmWidget(const PointF ptWinpoint);

	/*!
	 * @brief ѡ����ʱͼ��
	*/
	void SelectTmpWidget(const PointF ptWinpoint);

	/*!
	 * @brief ѡ��ͼ��
	*/
	void SelectWidget(const PointF ptWinpoint);

	/*!
	 * @brief ��RFID��
	*/
	void BindRFID(CGraphicAGV* pAGV, const unsigned short usRFID);

	/*!
	 * @brief �ͷ�RFID
	*/
	void ReleaseRFID(CGraphicAGV* pAGV, const unsigned short usRFID);

	void DrawWidget(Graphics* grPtr);

	void DrawTmpWidget(Graphics* grPtr);

	/*!
	 * @brief ��ʼ��ADO
	*/
	void InitADO();

	/*!
	 * @brief ͨ�����ݿ��ȡ����
	*/
	void LoadBackgroundByADO();

	/*!
	 * @brief ͨ�����ݿⴢ�汳��
	*/
	void SaveBackgroundByADO();

	void JsonStringToAGV(const char* strJson);
	void JsonStringToRFID(const char* strJson);
	void JsonStringToStation(const char* strJson);
	void JsonStringToTrack(const char* strJson);
	void JsonStringToCaller(const char* strJson);
	void JsonStringToCharger(const char* strJson);
	void JsonStringToRest(const char* strJson);

	void SaveAGVToJsonString(Json::Value& json);
	void SaveRFIDToJsonString(Json::Value& json);
	void SaveStationToJsonString(Json::Value& json);
	void SaveTrackToJsonString(Json::Value& json);
	void SaveCallerToJsonString(Json::Value& json);
	void SaveChargerToJsonString(Json::Value& json);
	void SaveRestToJsonString(Json::Value& json);
};

#endif //!_MAP_H
