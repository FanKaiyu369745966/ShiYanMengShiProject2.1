//#include "pch.h"
#include "CAGV.h"

#ifdef _AGV_H
#include "../json/json.h"
//#include "../json/json_value.cpp"
//#include "../json/json_reader.cpp"
//#include "../json/json_writer.cpp"

#include "../CommunicateDemo/CTCP.cpp"
#include "../String/DebugPrint.h"
#include "../String/SystemLog.h"
#include "../Data/CRC.h"
#include "../Thread/ThreadEx.h"

list<DataBuffer> CAGV::m_listProcess;
mutex CAGV::m_mutexProcess;

CAGV::CAGV()
	:m_byStatus(_AGVSTA_WAIT)
	, m_chSpeed(0)
	, m_byPower(_AGVPOWER_EMPTY)
	, m_usCurRFID(0)
	, m_usOldRFID(0)
	, m_usNextRFID(0)
	, m_usEndRFID(0)
	, m_byCargo(_AGVCARGO_EMPTY)
	, m_byLifter(_AGVLIFTER_DOWN)
	, m_chError(_AGVERR_NONE)
	, m_usAction(_AGVACT_NONE)
	, m_byActStatus(_AGVACTSTA_NONE)
	, m_pThreadSend(nullptr)
	, m_hShutdownEvent(nullptr)
	, m_pThreadConnect(nullptr)
	, m_hDestroyEvent(nullptr)
	, m_byNo(0)
	, m_pFunc(nullptr)
	, m_funcUpdateAGV(nullptr)
{
	m_pJsonReader = m_JsonBuilder.newCharReader();
}

CAGV::CAGV(unsigned char byNo, UpdateAGV funcPrt, void* ptr)
	:m_byStatus(_AGVSTA_WAIT)
	, m_chSpeed(0)
	, m_byPower(_AGVPOWER_EMPTY)
	, m_usCurRFID(0)
	, m_usOldRFID(0)
	, m_usNextRFID(0)
	, m_usEndRFID(0)
	, m_byCargo(_AGVCARGO_EMPTY)
	, m_byLifter(_AGVLIFTER_DOWN)
	, m_chError(_AGVERR_NONE)
	, m_usAction(_AGVACT_NONE)
	, m_byActStatus(_AGVACTSTA_NONE)
	, m_pThreadSend(nullptr)
	, m_hShutdownEvent(nullptr)
	, m_pThreadConnect(nullptr)
	, m_hDestroyEvent(nullptr)
	, m_byNo(byNo)
	, m_pFunc(ptr)
	, m_funcUpdateAGV(funcPrt)
{
	m_pJsonReader = m_JsonBuilder.newCharReader();
}

CAGV::CAGV(unsigned char byNo, UpdateAGV funcPrt, void* ptr, const char* strSrvIP, const char* strLocalIP, int nSrvPort, int nLocalPort)
	: TCPClient(strSrvIP, strLocalIP, nSrvPort, nLocalPort)
	, m_byStatus(_AGVSTA_WAIT)
	, m_chSpeed(0)
	, m_byPower(_AGVPOWER_EMPTY)
	, m_usCurRFID(0)
	, m_usOldRFID(0)
	, m_usNextRFID(0)
	, m_usEndRFID(0)
	, m_byCargo(_AGVCARGO_EMPTY)
	, m_byLifter(_AGVLIFTER_DOWN)
	, m_chError(_AGVERR_NONE)
	, m_usAction(_AGVACT_NONE)
	, m_byActStatus(_AGVACTSTA_NONE)
	, m_pThreadSend(nullptr)
	, m_hShutdownEvent(nullptr)
	, m_pThreadConnect(nullptr)
	, m_hDestroyEvent(nullptr)
	, m_byNo(byNo)
	, m_pFunc(ptr)
	, m_funcUpdateAGV(funcPrt)
{
	m_pJsonReader = m_JsonBuilder.newCharReader();
}

CAGV::CAGV(unsigned char byNo, UpdateAGV funcPrt, void* ptr, const wchar_t* wstrSrvIP, const wchar_t* wstrLocalIP, int nSrvPort, int nLocalPort)
	: TCPClient(wstrSrvIP, wstrLocalIP, nSrvPort, nLocalPort)
	, m_byStatus(_AGVSTA_WAIT)
	, m_chSpeed(0)
	, m_byPower(_AGVPOWER_EMPTY)
	, m_usCurRFID(0)
	, m_usOldRFID(0)
	, m_usNextRFID(0)
	, m_usEndRFID(0)
	, m_byCargo(_AGVCARGO_EMPTY)
	, m_byLifter(_AGVLIFTER_DOWN)
	, m_chError(_AGVERR_NONE)
	, m_usAction(_AGVACT_NONE)
	, m_byActStatus(_AGVACTSTA_NONE)
	, m_pThreadSend(nullptr)
	, m_hShutdownEvent(nullptr)
	, m_pThreadConnect(nullptr)
	, m_hDestroyEvent(nullptr)
	, m_byNo(byNo)
	, m_pFunc(ptr)
	, m_funcUpdateAGV(funcPrt)
{
	m_pJsonReader = m_JsonBuilder.newCharReader();
}

CAGV::CAGV(unsigned char byNo, UpdateAGV funcPrt, void* ptr, const string strLocalIP, string strSrvIP, int nLocalPort, int nSrvPort)
	: TCPClient(strSrvIP.c_str(), strLocalIP.c_str(), nSrvPort, nLocalPort)
	, m_byStatus(_AGVSTA_WAIT)
	, m_chSpeed(0)
	, m_byPower(_AGVPOWER_EMPTY)
	, m_usCurRFID(0)
	, m_usOldRFID(0)
	, m_usNextRFID(0)
	, m_usEndRFID(0)
	, m_byCargo(_AGVCARGO_EMPTY)
	, m_byLifter(_AGVLIFTER_DOWN)
	, m_chError(_AGVERR_NONE)
	, m_usAction(_AGVACT_NONE)
	, m_byActStatus(_AGVACTSTA_NONE)
	, m_pThreadSend(nullptr)
	, m_hShutdownEvent(nullptr)
	, m_pThreadConnect(nullptr)
	, m_hDestroyEvent(nullptr)
	, m_byNo(byNo)
	, m_pFunc(ptr)
	, m_funcUpdateAGV(funcPrt)
{
	m_pJsonReader = m_JsonBuilder.newCharReader();
}

CAGV::~CAGV()
{
	//EndSendThread();

	Release();

	delete m_pJsonReader;
}

int CAGV::MoveTo(const unsigned short usRFID)
{
	// ��Ч���ƶ��յ�RFID�����
	if (usRFID == 0)
	{
		DebugPrint::Printf("����AGV:%d�ƶ�ʧ��:\n��Ч���ƶ��յ�RFID�����!", m_byNo);
		return ERROR_END;
	}

	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d�ƶ�ʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// AGV�޵�,�޷��ƶ�
	if (m_byPower == _AGVPOWER_EMPTY)
	{
		DebugPrint::Printf("����AGV:%d�ƶ�ʧ��:\nδ��⵽AGV���״̬!", m_byNo);
		return ERROR_BATTERY;
	}

	// AGV�����ڴ���״̬
	if (m_byStatus != _AGVSTA_WAIT)
	{
		DebugPrint::Printf("����AGV:%d�ƶ�ʧ��:\nAGV��ǰ״̬�޷����ճ������.\n״̬��:%02X", m_byNo, m_byStatus);
		return ERROR_STATUS;
	}

	// �Ѿ������ƶ��յ�
	if (m_usCurRFID == usRFID)
	{
		return ERROR_NONE;
	}

	if (m_usAction == _AGVACT_MOVE && m_byActStatus != _AGVACTSTA_NONE)
	{
		return ERROR_NONE;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("Move");
	JsonRoot["Start"] = Json::Value(m_usCurRFID);
	JsonRoot["End"] = Json::Value(usRFID);

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d�ƶ��ɹ�.\n��%d�ƶ���%d", m_byNo, m_usCurRFID, usRFID);
	SystemLog::Recordf("����AGV:%d�ƶ��ɹ�.\n��%d�ƶ���%d", m_byNo, m_usCurRFID, usRFID);

	JsonRoot.clear();
	
	return ERROR_NONE;
}

int CAGV::RemoveTo()
{
	// ��Ч���ƶ��յ�RFID�����
	if (m_usEndRFID == 0)
	{
		DebugPrint::Printf("����AGV:%d�����ƶ�ʧ��:\n��Ч���ƶ��յ�RFID�����!", m_byNo);

		return ERROR_END;
	}

	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d�����ƶ�ʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// AGV�޵�,�޷��ƶ�
	if (m_byPower == _AGVPOWER_EMPTY)
	{
		DebugPrint::Printf("����AGV:%d�����ƶ�ʧ��:\nδ��⵽AGV���״̬!", m_byNo);
		return ERROR_BATTERY;
	}

	// AGV�����ڴ���״̬
	if (m_byStatus != _AGVSTA_WAIT)
	{
		DebugPrint::Printf("����AGV:%d�����ƶ�ʧ��:\nAGV��ǰ״̬�޷����ճ������.\n״̬��:%02X", m_byNo, m_byStatus);
		return ERROR_STATUS;
	}

	if (m_usAction == _AGVACT_REMOVE && m_byActStatus != _AGVACTSTA_NONE)
	{
		return ERROR_NONE;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("Remove");

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d�����ƶ��ɹ�.\n�����ƶ���%d", m_byNo, m_usEndRFID);
	SystemLog::Recordf("����AGV:%d�����ƶ��ɹ�.\n�����ƶ���%d", m_byNo, m_usEndRFID);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::TrafficPass(const unsigned short usRFID)
{
	// ��Ч��ͨ��RRFID�����
	if (usRFID == 0)
	{
		DebugPrint::Printf("����AGV:%d��ͨ����ͨ��ʧ��:\n��Ч��ͨ��RFID�����!", m_byNo);
		return ERROR_END;
	}

	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d��ͨ����ͨ��ʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// AGV�޵�,�޷��ƶ�
	if (m_byPower == _AGVPOWER_EMPTY)
	{
		DebugPrint::Printf("����AGV:%d��ͨ����ͨ��ʧ��:\nδ��⵽AGV���״̬!", m_byNo);
		return ERROR_BATTERY;
	}

	// AGV�����ڽ�ͨ����״̬
	if (m_byStatus != _AGVSTA_TRAFFICSTOP)
	{
		DebugPrint::Printf("����AGV:%d��ͨ����ͨ��ʧ��:\nAGV��ǰ״̬�޷����ճ������.\n״̬��:%02X", m_byNo, m_byStatus);
		return ERROR_STATUS;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("TrafficPass");
	JsonRoot["Start"] = Json::Value(m_usCurRFID);
	JsonRoot["End"] = Json::Value(usRFID);

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d��ͨ����ͨ�гɹ�.\n��%dͨ��", m_byNo, usRFID);
	SystemLog::Recordf("����AGV:%d��ͨ����ͨ�гɹ�.\n��%dͨ��", m_byNo, usRFID);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::TrafficStop(const unsigned short usRFID)
{
	// ��Ч��ֹͣ��RRFID�����
	if (usRFID == 0)
	{
		DebugPrint::Printf("����AGV:%d��ͨ����ͨ��ֹͣʧ��:\n��Ч��ֹͣRFID�����!", m_byNo);
		return ERROR_END;
	}

	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d��ͨ����ͨ��ֹͣʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// AGV�޵�,�޷��ƶ�
	if (m_byPower == _AGVPOWER_EMPTY)
	{
		DebugPrint::Printf("����AGV:%d��ͨ����ͨ��ֹͣʧ��:\nδ��⵽AGV���״̬!", m_byNo);
		return ERROR_BATTERY;
	}

	// AGV��ͣ����е���ϡ���ͨ�����޷��ƶ�
	if (m_byStatus == _AGVSTA_SCREAM || m_byStatus == _AGVSTA_MOBSSTOP)
	{
		DebugPrint::Printf("����AGV:%d��ͨ����ͨ��ֹͣʧ��:\nAGV��ǰ״̬�޷����ճ������.\n״̬��:%02X", m_byNo, m_byStatus);
		return ERROR_STATUS;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("TrafficStop");
	JsonRoot["Start"] = Json::Value(m_usCurRFID);
	JsonRoot["End"] = Json::Value(usRFID);

	// ���ͱ���
	InsertIntoWhileList(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d��ͨ����ͨ��ֹͣ�ɹ�.\n��%dֹͣ", m_byNo, usRFID);
	SystemLog::Recordf("����AGV:%d��ͨ����ͨ��ֹͣ�ɹ�.\n��%dֹͣ", m_byNo, usRFID);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::Shutdown()
{
	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d�ػ�ʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("Shutdown");

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d�ػ��ɹ�.", m_byNo);
	SystemLog::Recordf("����AGV:%d�ػ��ɹ�.", m_byNo);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::Scream()
{
	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d��ͣʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// �Ѽ�ͣ
	if (m_byStatus == _AGVSTA_SCREAM)
	{
		return ERROR_NONE;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("Scream");

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d��ͣ�ɹ�.", m_byNo);
	SystemLog::Recordf("����AGV:%d��ͣ�ɹ�.", m_byNo);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::Reset()
{
	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d��λʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// �Ѹ�λ
	if (m_byStatus == _AGVSTA_WAIT)
	{
		return ERROR_NONE;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("Reset");

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d��λ�ɹ�.", m_byNo);
	SystemLog::Recordf("����AGV:%d��λ�ɹ�.", m_byNo);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::Restart()
{
	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d����ʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// ������
	if (m_byStatus == _AGVSTA_WAIT && m_usCurRFID == 0)
	{
		return ERROR_NONE;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("Restart");

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d���óɹ�.", m_byNo);
	SystemLog::Recordf("����AGV:%d���óɹ�.", m_byNo);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::LiftUp(const unsigned short usRFID)
{
	// ��Ч�Ķ���ִ��RFID�����
	if (usRFID == 0)
	{
		DebugPrint::Printf("����AGV:%d����������:\n��Ч�Ķ���ִ��RFID�����!", m_byNo);
		return ERROR_END;
	}

	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d����������ʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// AGV�޵�,�޷��ƶ�
	if (m_byPower == _AGVPOWER_EMPTY)
	{
		DebugPrint::Printf("����AGV:%d����������ʧ��:\nδ��⵽AGV���״̬!", m_byNo);
		return ERROR_BATTERY;
	}

	// AGV�����ڴ���״̬
	if (m_byStatus != _AGVSTA_WAIT)
	{
		DebugPrint::Printf("����AGV:%d����������ʧ��:\nAGV��ǰ״̬�޷����ճ������.\n״̬��:%02X", m_byNo, m_byStatus);
		return ERROR_STATUS;
	}

	// ������������ �� ��������������
	if (m_byLifter == _AGVLIFTER_UP && m_usAction == _AGVACT_LIFTUP && m_byActStatus != _AGVACTSTA_NONE)
	{
		return ERROR_NONE;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("LiftUp");
	JsonRoot["Start"] = Json::Value(m_usCurRFID);
	JsonRoot["End"] = Json::Value(usRFID);

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d�����������ɹ�.", m_byNo);
	SystemLog::Recordf("����AGV:%d�����������ɹ�.", m_byNo);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::LiftDown(const unsigned short usRFID)
{
	// ��Ч�Ķ���ִ��RFID�����
	if (usRFID == 0)
	{
		DebugPrint::Printf("����AGV:%d�������½�ʧ��:\n��Ч�Ķ���ִ��RFID�����!", m_byNo);
		return ERROR_END;
	}

	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d�������½�ʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// AGV�޵�,�޷��ƶ�
	if (m_byPower == _AGVPOWER_EMPTY)
	{
		DebugPrint::Printf("����AGV:%d�������½�ʧ��:\nδ��⵽AGV���״̬!", m_byNo);
		return ERROR_BATTERY;
	}

	// AGV�����ڴ���״̬
	if (m_byStatus != _AGVSTA_WAIT)
	{
		DebugPrint::Printf("����AGV:%d�������½�ʧ��:\nAGV��ǰ״̬�޷����ճ������.\n״̬��:%02X", m_byNo, m_byStatus);
		return ERROR_STATUS;
	}

	// ������������ �� �����������½�
	if (m_byLifter == _AGVLIFTER_DOWN && m_usAction == _AGVACT_LIFTDOWN && m_byActStatus != _AGVACTSTA_NONE)
	{
		return ERROR_NONE;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("LiftDown");
	JsonRoot["Start"] = Json::Value(m_usCurRFID);
	JsonRoot["End"] = Json::Value(usRFID);

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d�������½��ɹ�.", m_byNo);
	SystemLog::Recordf("����AGV:%d�������½��ɹ�.", m_byNo);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::Stop()
{
	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d����ֹ����ʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// �޶���ִ�� �� ��ͨ���ƶ���
	if (m_usAction == _AGVACT_NONE && m_byActStatus == _AGVACTSTA_NONE)
	{
		return ERROR_STATUS;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("Stop");

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d��ֹ�����ɹ�.", m_byNo);
	SystemLog::Recordf("����AGV:%d��ֹ�����ɹ�.", m_byNo);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::Action(const unsigned short usStartRFID, const unsigned short usEndRFID, const unsigned short usAction)
{
	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%dִ�ж���ʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// AGV�޵�,�޷��ƶ�
	if (m_byPower == _AGVPOWER_EMPTY)
	{
		DebugPrint::Printf("����AGV:%dִ�ж���ʧ��:\nδ��⵽AGV���״̬!", m_byNo);
		return ERROR_BATTERY;
	}

	if (m_byStatus != _AGVSTA_WAIT)
	{
		DebugPrint::Printf("����AGV:%dִ�ж���ʧ��:\nAGV��ǰ״̬�޷����ճ������.\n״̬��:%02X", m_byNo, m_byStatus);

		return ERROR_STATUS;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("Other");
	JsonRoot["Action"] = Json::Value(usAction);
	JsonRoot["Start"] = Json::Value(usStartRFID);
	JsonRoot["End"] = Json::Value(usEndRFID);

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%dִ�ж����ɹ�.\n��%d��%d\n������:%02X", m_byNo, usStartRFID, usEndRFID, usAction);
	SystemLog::Recordf("����AGV:%dִ�ж����ɹ�.\n��%d��%d\n������:%02X", m_byNo, usStartRFID, usEndRFID, usAction);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::StatusSet(const unsigned short usRFID, const unsigned char byStatus, const char chSpeed)
{
	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d����״̬ʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("Set");
	JsonRoot["Status"] = Json::Value(byStatus);
	JsonRoot["RFID"] = Json::Value(usRFID);
	JsonRoot["Speed"] = Json::Value(chSpeed);

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d����״̬�ɹ�.\n״̬��:%02X\n��ǰRFID:%d", m_byNo, byStatus, usRFID);
	SystemLog::Recordf("����AGV:%d����״̬�ɹ�.\n״̬��:%02X\n��ǰRFID:%d", m_byNo, byStatus, usRFID);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::Pause()
{
	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d��ͣ����ʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// AGV�޵�,�޷��ƶ�
	if (m_byPower == _AGVPOWER_EMPTY)
	{
		DebugPrint::Printf("����AGV:%d��ͣ����ʧ��:\nδ��⵽AGV���״̬!", m_byNo);
		return ERROR_BATTERY;
	}

	// �޶���ִ�� �� ������ֹͣ
	if (m_usAction == _AGVACT_NONE || m_usAction == _AGVACT_TRAFFIC)
	{
		return ERROR_STATUS;
	}

	// ���������
	if (m_byActStatus == _AGVACTSTA_FINISH)
	{
		return ERROR_NONE;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("Pause");

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d��ͣ�����ɹ�.", m_byNo);
	SystemLog::Recordf("����AGV:%d��ͣ�����ɹ�.", m_byNo);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::Continue()
{
	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d��������ʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// AGV�޵�,�޷��ƶ�
	if (m_byPower == _AGVPOWER_EMPTY)
	{
		DebugPrint::Printf("����AGV:%d��������ʧ��:\nδ��⵽AGV���״̬!", m_byNo);
		return ERROR_BATTERY;
	}

	// �޶���ִ�� �� ��ͨ����
	if (m_usAction == _AGVACT_NONE || m_usAction == _AGVACT_TRAFFIC)
	{
		return ERROR_STATUS;
	}

	// ��������� �� ����δ��ͣ
	if (m_byActStatus == _AGVACTSTA_FINISH || m_byActStatus != _AGVACTSTA_PAUSE)
	{
		return ERROR_NONE;
	}

	if (m_byStatus != _AGVSTA_WAIT)
	{
		DebugPrint::Printf("����AGV:%d��������ʧ��:\nAGV��ǰ״̬�޷����ճ������.\n״̬��:%02X", m_byNo, m_byStatus);
		return ERROR_STATUS;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("Continue");

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d���������ɹ�.", m_byNo);
	SystemLog::Recordf("����AGV:%d���������ɹ�.", m_byNo);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::SpeedControl(char chSpeed)
{
	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%d�����ٶ�ʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// AGV�޵�,�޷��ƶ�
	if (m_byPower == _AGVPOWER_EMPTY)
	{
		DebugPrint::Printf("����AGV:%d�����ٶ�ʧ��:\nδ��⵽AGV���״̬!", m_byNo);
		return ERROR_BATTERY;
	}

	if (m_chSpeed == 0 || m_chSpeed <= chSpeed)
	{
		return ERROR_NONE;
	}

	if (m_byStatus != _AGVSTA_RUN && m_byStatus != _AGVSTA_OBSDOWN)
	{
		return ERROR_NONE;
	}

	if (m_usAction != _AGVACT_MOVE && m_usAction == _AGVACT_REMOVE)
	{
		return ERROR_NONE;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("SetSpeed");
	JsonRoot["Speed"] = Json::Value(chSpeed);

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%d�����ٶȳɹ�.\n��ǰ����ٶȵ���Ϊ:%d%%\n", m_byNo, chSpeed);
	SystemLog::Recordf("����AGV:%d�����ٶȳɹ�.\n��ǰ����ٶȵ���Ϊ:%d%%\n", m_byNo, chSpeed);

	JsonRoot.clear();

	return ERROR_NONE;
}

int CAGV::AllScream()
{
	// δ����
	if (IsConnected() == false)
	{
		DebugPrint::Printf("����AGV:%dȫ�߼�ͣʧ��:\nAGVδ����!", m_byNo);
		return ERROR_CONNECT;
	}

	// �Ѽ�ͣ
	if (m_byStatus == _AGVSTA_ALLSCREAM)
	{
		return ERROR_NONE;
	}

	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("AllScream");

	// ���ͱ���
	SendPackage(JsonRoot.toStyledString().c_str());

	DebugPrint::Printf("����AGV:%dȫ�߼�ͣ�ɹ�.", m_byNo);
	SystemLog::Recordf("����AGV:%dȫ�߼�ͣ�ɹ�.", m_byNo);

	JsonRoot.clear();

	return ERROR_NONE;
}

const unsigned char CAGV::GetStatus() const
{
	return m_byStatus;
}

const unsigned short CAGV::GetCurRFID() const
{
	return m_usCurRFID;
}

const unsigned short CAGV::GetOldRFID() const
{
	return m_usOldRFID;
}

const unsigned short CAGV::GetNextRFID() const
{
	return m_usNextRFID;
}

const unsigned short CAGV::GetEndRFID() const
{
	return m_usEndRFID;
}

const char CAGV::GetSpeed() const
{
	return m_chSpeed;
}

const unsigned char CAGV::GetPower() const
{
	return m_byPower;
}

const unsigned char CAGV::GetCargo() const
{
	return m_byCargo;
}

const unsigned char CAGV::GetLifter() const
{
	return m_byLifter;
}

const char CAGV::GetError() const
{
	return m_chError;
}

const unsigned short CAGV::GetAction() const
{
	return m_usAction;
}

const unsigned char CAGV::GetActionStatus() const
{
	return m_byActStatus;
}

const unsigned short CAGV::UpdateCurRFID(const unsigned short usRFID)
{
	if (m_usCurRFID != usRFID)
	{
		m_usOldRFID = m_usCurRFID;
		m_usCurRFID = usRFID;
	}

	return m_usOldRFID;
}
const unsigned short CAGV::UpdateNextRFID(const unsigned short usRFID)
{
	if (m_usNextRFID == usRFID)
	{
		return 0;
	}

	const unsigned short usOldNextRFID = m_usNextRFID;
	m_usNextRFID = usRFID;

	return usOldNextRFID;
}
const unsigned short CAGV::UpdateEndRFID(const unsigned short usRFID)
{
	if (m_usEndRFID == usRFID)
	{
		return 0;
	}

	const unsigned short usOldEndRFID = m_usEndRFID;
	m_usEndRFID = usRFID;

	return usOldEndRFID;
}

const bool CAGV::UpdateStatus(const unsigned char byStatus)
{
	if (m_byStatus == byStatus)
	{
		return false;
	}

	m_byStatus = byStatus;

	return true;
}

const bool CAGV::UpdateSpeed(const char chSpeed)
{
	if (m_chSpeed == chSpeed)
	{
		return false;
	}

	m_chSpeed = chSpeed;

	return true;
}

const bool CAGV::UpdatePower(const unsigned char byPower)
{
	if (m_byPower == byPower)
	{
		return false;
	}

	m_byPower = byPower;

	return true;
}

const bool CAGV::UpdateCargo(const unsigned char byCargo)
{
	if (m_byCargo == byCargo)
	{
		return false;
	}

	m_byCargo = byCargo;

	return true;
}

const bool CAGV::UpdateLifter(const unsigned char byLifter)
{
	if (m_byLifter == byLifter)
	{
		return false;
	}

	m_byLifter = byLifter;

	return true;
}

const bool CAGV::UpdateError(const char chError)
{
	if (m_chError == chError)
	{
		return false;
	}

	m_chError = chError;

	return true;
}

const bool CAGV::UpdateAction(const unsigned short usAction)
{
	if (m_usAction == usAction)
	{
		return false;
	}

	m_usAction = usAction;

	return true;
}

const bool CAGV::UpdateActStatus(const unsigned char byActStatus)
{
	if (m_byActStatus == byActStatus)
	{
		return false;
	}

	m_byActStatus = byActStatus;

	return true;
}

bool CAGV::Connect(const SOCKET socketClient)
{
	EndSendThread();
	
	if (TCPClient::ConnectPeer(socketClient) == false)
	{
		return false;
	}

	Heartbeat();

	if (StartSendThread() == false)
	{
		Break();
		return false;
	}

	m_chError = _AGVERR_NONE;

	return true;
}

bool CAGV::Connect(const char * strSrvIP, const unsigned short usSrvPort)
{
	EndSendThread();

	if (TCPClient::Connect(strSrvIP, usSrvPort) == false)
	{
		return false;
	}

	Heartbeat();

	if (StartSendThread() == false)
	{
		Break();
		return false;
	}

	m_chError = _AGVERR_NONE;

	return true;
}

bool CAGV::Connect(const wchar_t * wstrSrvIP, const unsigned short usSrvPort)
{
	EndSendThread();

	if (TCPClient::Connect(wstrSrvIP, usSrvPort) == false)
	{
		return false;
	}

	StartSendThread();

	Heartbeat();

	m_chError = _AGVERR_NONE;

	return true;
}

void CAGV::Break()
{
	EndSendThread();

	TCPClient::Break();

	m_byPower = _AGVPOWER_EMPTY;

	m_bufferReceive.Release();

	m_listSingle.clear();
	m_listWhile.clear();
}

void CAGV::Release()
{
	m_pFunc = nullptr;
	m_funcUpdateAGV = nullptr;

	EndConnectThread();

	Break();
}

bool CAGV::IsConnected()
{
	if (TCPClient::IsConnected() == false)
	{
		// EndSendThread();

		return false;
	}

	/*
	if (m_pThreadSend == nullptr)
	{
		StartSendThread();
	}
	*/

	return true;
}

DataBuffer CAGV::GetProcessPackage()
{
	if (m_listProcess.size() == 0)
	{
		return DataBuffer();
	}

	lock_guard<mutex> lock(m_mutexProcess);

	DataBuffer buffer = m_listProcess.front();
	m_listProcess.pop_front();

	return buffer;
}

unsigned char CAGV::GetNo() const
{
	return m_byNo;
}

int CAGV::InitAttribute()
{
	m_byStatus = _AGVSTA_WAIT;
	m_chSpeed = 0;
	m_byPower = _AGVPOWER_EMPTY;
	m_usCurRFID = 0;
	m_usOldRFID = 0;
	m_usNextRFID = 0;
	m_usEndRFID = 0;
	m_byCargo = _AGVCARGO_EMPTY;
	m_byLifter = _AGVLIFTER_DOWN;
	m_chError = _AGVERR_NONE;
	m_usAction = _AGVACT_NONE;
	m_byActStatus = _AGVACTSTA_NONE;

	return ERROR_NONE;
}

void CAGV::Heartbeat()
{
	// ����Json�����������Ϊ��
	Json::Value JsonRoot;
	JsonRoot["No"] = Json::Value(m_byNo);
	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["Cmd"] = Json::Value("Heartbeat");

	// ���ͱ���
	InsertIntoWhileList(JsonRoot.toStyledString().c_str());

	JsonRoot.clear();

	return;
}

bool CAGV::StartSendThread()
{
	EndSendThread();

	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(nullptr, true, false, nullptr);
	}

	m_pThreadSend = new std::thread(&CAGV::SendThread, this);

	if (m_pThreadSend)
	{
		return true;
	}

	return false;
}

void CAGV::EndSendThread()
{
	if (m_pThreadSend)
	{
		if (m_pThreadSend->joinable())
		{
			SetEvent(m_hShutdownEvent);

			m_pThreadSend->join();
		}

		delete m_pThreadSend;
		m_pThreadSend = nullptr;
	}

	return;
}

void CAGV::SendThread()
{
	list<DataBuffer> listSend;

	while (true)
	{
		if (WaitForSingleObject(m_hShutdownEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}

		// ����30sδ�յ��ظ���Ϣ
		if (m_ullReceiveTime > 0 && GetTickCountEX() - m_ullReceiveTime > 30 * 1000)
		{
			SetEvent(m_hShutdownEvent);

			m_chError = _AGVERR_COMM;
			m_byPower = _AGVPOWER_EMPTY;

			TCPClient::Break();

			m_bufferReceive.Release();

			if (m_funcUpdateAGV)
			{
				m_funcUpdateAGV(this, m_pFunc);
			}

			break;
		}

		// TCPͨ���쳣�ж�
		if (TCPClient::IsConnected() == false)
		{
			SetEvent(m_hShutdownEvent);

			m_chError = _AGVERR_COMM;
			m_byPower = _AGVPOWER_EMPTY;;

			m_bufferReceive.Release();

			if (m_funcUpdateAGV)
			{
				m_funcUpdateAGV(this, m_pFunc);
			}

			break;
		}

		if (m_mutexLock.try_lock() == false)
		{
			if (WaitForSingleObject(m_hShutdownEvent, 1) == WAIT_OBJECT_0)
			{
				break;
			}

			continue;
		}

		listSend = m_listWhile;
		if (m_listSingle.size() > 0)
		{
			listSend.splice(listSend.begin(), m_listSingle);
			m_listSingle.size();
		}

		m_mutexLock.unlock();

		for (list<DataBuffer>::iterator it = listSend.begin(); it != listSend.end(); it = listSend.erase(it))
		{
			if (Send((*it).m_pbyBuffer, (*it).m_unSize) == false)
			{
				SetEvent(m_hShutdownEvent);

				m_chError = _AGVERR_COMM;
				m_byPower = _AGVPOWER_EMPTY;

				TCPClient::Break();

				m_bufferReceive.Release();

				if (m_funcUpdateAGV)
				{
					m_funcUpdateAGV(this, m_pFunc);
				}

				break;
			}

			if (WaitForSingleObject(m_hShutdownEvent, 100) == WAIT_OBJECT_0)
			{
				break;
			}
		}
	}

	listSend.clear();

	DebugPrint::Printf("AGV:%d���ݷ����߳̽���", m_byNo);

	return;
}

void CAGV::ProcessData(const unsigned char * pData, const unsigned int unSize)
{
	m_bufferReceive.Add((unsigned char*)pData, unSize);
	DataBuffer& bufferProcess = m_bufferReceive;

	char* pLeftData = (char*)bufferProcess.m_pbyBuffer;	/*!< ʣ������ */
	int nLeftDataLen = bufferProcess.m_unSize;			/*!< ʣ�����ݳ��� */

	char* pHead = nullptr;			/*!< ����ͷ��ַ */
	char* pTail = nullptr;			/*!< ����β��ַ */
	int nBody = 0;					/*!< �����峤�� */
	char* pSourceData = nullptr;	/*!< Դ���� */
	int nSourceLen = 0;				/*!< Դ���ݳ��� */
	unsigned short usPackLen = 0;	/*!< ���ĳ��� */
	unsigned short usRealLen = 0;	/*!< ʵ�ʳ��� */
	unsigned short usPackCheck = 0;	/*!< У��ֵ */
	unsigned short usRealCheck = 0;	/*!< ʵ��У��ֵ */

	for (;; pHead = nullptr, pTail = nullptr, nBody = 0, pSourceData = nullptr, nSourceLen = 0, usPackLen = 0, usRealLen = 0, usPackCheck = 0, usRealCheck = 0)
	{
		pHead = (char*)memchr(pLeftData, 0xBA, nLeftDataLen);

		// ʣ���������ޱ���ͷ
		if (pHead == nullptr)
		{
			bufferProcess.Release();
			break;
		}

		pTail = (char*)memchr(pHead + 1, 0xBE, bufferProcess.m_unSize - (pHead - (char*)bufferProcess.m_pbyBuffer) - 1);

		// ʣ���������ޱ���β
		if (pTail == nullptr)
		{
			// ��������ͷ�������
			bufferProcess = DataBuffer((unsigned char*)pHead, bufferProcess.m_unSize - (pHead - (char*)bufferProcess.m_pbyBuffer));

			break;
		}

		nBody = pTail - pHead - 1;
		pSourceData = new char[nBody];
		nSourceLen = 0;

		if (pSourceData == nullptr)
		{
			continue;
		}

		if (nBody < 2)
		{
			delete[] pSourceData;
			continue;
		}

		// ��ʼ��
		memset(pSourceData, 0, nBody);
		// ��ת��
		Retranslation(pHead + 1, nBody, pSourceData, nSourceLen);

		usPackLen = ((pSourceData[0] & 0xFF) << 8)| (pSourceData[1] & 0xFF);

		usRealLen = nSourceLen - 4;	/*!< ʵ�ʳ��� */

		// ���ݳ��Ȳ���ͬ
		if (usPackLen != usRealLen)
		{
			DebugPrint::Printf("AGV%d���ݴ������:\n���ĳ���:%d;\nʵ�ʽ��ճ���:%d;", m_byNo, usPackLen, usRealLen);

			// �ͷ�Դ����
			delete[] pSourceData;

			pLeftData = pTail + 1;
			nLeftDataLen = bufferProcess.m_unSize - (pTail + 1 - (char*)bufferProcess.m_pbyBuffer);

			// ��ʣ������
			if (nLeftDataLen <= 0)
			{
				bufferProcess.Release();
				break;
			}

			continue;
		}

		usPackCheck = (*(pSourceData + 2 + usPackLen) & 0xFF) << 8 |(*(pSourceData + 3 + usPackLen) & 0xFF);

		usRealCheck = CRC::CRC16((unsigned char*)pSourceData, usRealLen + 2);	/*!< ʵ��У��ֵ */

		if (usPackCheck != usRealCheck)
		{
			DebugPrint::Printf("AGV%d���ݴ������:\n����У��ֵ:%04X;\nʵ��У��ֵ:%04X;", m_byNo, usPackCheck, usRealCheck);

			// �ͷ�Դ����
			delete[] pSourceData;

			pLeftData = pTail + 1;
			nLeftDataLen = bufferProcess.m_unSize - (pTail + 1 - (char*)bufferProcess.m_pbyBuffer);

			// ��ʣ������
			if (nLeftDataLen <= 0)
			{
				bufferProcess.Release();
				break;
			}

			continue;
		}
		/*
		// ��¼���յı���
		stringstream ssHex; // 16��������
		for (unsigned int unI = 0; unI < usPackLen; ++unI)
		{
			ssHex << StringEx::string_format("%02X ", (*(pSourceData + 2 + unI) & 0xFF));
		}
		char* lpszStr = new char[usPackLen + 1];	// ASCII�ַ�����
		// ��ʼ��
		memset(lpszStr, 0, usPackLen + 1);
		memcpy_s(lpszStr, usPackLen, pSourceData + 2, usPackLen);

		DebugPrint::Printf("AGV%d���ձ��ĳɹ�!\n16����:%s", m_byNo, ssHex.str().c_str());
		DebugPrint::Printf("ASCII�ַ���:%s", lpszStr);
		
		// �ͷ��ַ��� 
		delete[] lpszStr;
		*/

		ProcessPackage((unsigned char*)(pSourceData + 2), usPackLen);

		// �ͷ�Դ����
		delete[] pSourceData;

		pLeftData = pTail + 1;
		nLeftDataLen = bufferProcess.m_unSize - (pTail+1 - (char*)bufferProcess.m_pbyBuffer);

		// ��ʣ������
		if (nLeftDataLen <= 0)
		{
			bufferProcess.Release();
			break;
		}
	}

	return;
}

void CAGV::ProcessPackage(const unsigned char * pPack, const int nPackLen)
{
	Json::Value JsonRoot;

	char* _package = new char[nPackLen + 1];
	memset(_package, 0, nPackLen + 1);

	memcpy_s(_package, nPackLen, pPack, nPackLen);

	const char* pstr = _package;

	if (!m_pJsonReader->parse(pstr, pstr + strlen(pstr), &JsonRoot, &m_JstrErrs))
	{
		DebugPrint::Printf(m_JstrErrs.c_str());
		DebugPrint::Printf("AGV%d���ݰ��������:\n��JSON��ʽ������",m_byNo);

		m_JstrErrs.clear();

		delete[] _package;

		return;
	}

	delete[] _package;

	Json::Value JsonObj = JsonRoot["Message"];
	string strKey = Json_ReadString(JsonObj["Cmd"]);

	if (strcmp(strKey.c_str(), "Heartbeat") == 0)
	{
		m_mutexLock.lock();

		InsertIntoProcessList((char*)pPack, nPackLen);

		m_mutexLock.unlock();

		strKey.clear();
	}
	else
	{
		string strResult = Json_ReadString(JsonRoot["Result"]);

		if (strcmp(strResult.c_str(), "Success") == 0)
		{
			RemoveFromWhileList(JsonObj.toStyledString().c_str(), (int)JsonObj.toStyledString().length());
		}

		strResult.clear();
	}

	JsonObj.clear();
	JsonRoot.clear();

	return;
}

DataBuffer CAGV::CreatePackage(const char * lpszStr)
{
	return CreatePackage(lpszStr, (int)strlen(lpszStr));
}

DataBuffer CAGV::CreatePackage(const char * lpszStr, const int nLen)
{
	int nSourceLen = nLen + 4;	/*!< Դ���ݳ��� ��������λ */
	int nTransLen = nSourceLen * 2 + 2;	/*!< ת����������ݳ��� ������λ */

	char *lpszSource = (char*)malloc(nSourceLen);	/*!< ������Ҫת���Դ���� */

	if (lpszSource == nullptr)
	{
		return DataBuffer();
	}

	if (nSourceLen < 2)
	{
		free(lpszSource);	/*!< �ͷ�Դ�����ڴ� */
		return DataBuffer();
	}

	// ��ʼ��Դ����
	memset(lpszSource, 0, nSourceLen);
	lpszSource[0] = (nLen >> 8) & 0xFF;		/*!< �������ݳ��ȣ���λ�� */
	lpszSource[1] = nLen & 0xFF;			/*!< �������ݳ��ȣ���λ��*/
	memcpy_s(&lpszSource[2], nLen, lpszStr, nLen);

	unsigned short usCheck = CRC::CRC16((unsigned char*)lpszSource, nLen + 2);	/*! У��ֵ */
	lpszSource[nSourceLen - 2] = (usCheck >> 8) & 0xFF;	/*!< ����У��ֵ����λ��*/
	lpszSource[nSourceLen - 1] = usCheck & 0xFF;	/*!< ����У��ֵ(��λ) */

	char* pTrans = (char*)malloc(nTransLen);	/*!< ����ת�������� */
	int nTrans = 0;	/*!< ת����ʵ�����ݳ��� */

	if (pTrans == nullptr)
	{
		free(lpszSource);
		return DataBuffer();
	}

	if (nTransLen < 2)
	{
		free(lpszSource);
		free(pTrans);
		return DataBuffer();
	}

	memset(pTrans, 0, nTransLen);
	Translation(lpszSource, nSourceLen, &pTrans[1], nTrans);
	free(lpszSource);	/*!< �ͷ�Դ�����ڴ� */

	pTrans[0] = (char)0xBA;
	pTrans[nTrans + 1] = (char)0xBE;

	DataBuffer package((unsigned char*)pTrans, nTrans + 2);

	free(pTrans);	/*!< �ͷ�ת�������ڴ� */

	return package;
}

void CAGV::SendPackage(const char * lpszStr)
{
	return SendPackage(lpszStr, (int)strlen(lpszStr));
}

void CAGV::SendPackage(const char * pData, const int nLen)
{
	InsertIntoSingleList(pData, nLen);

	return;
}

void CAGV::Translation(const char * pSourceData, int nSourceLen, char * pTransData, int & pTransLen)
{
	/*!
	 * �����������е���0xBA������ת��Ϊ0xB0+0x01
	 * �����������е���0xBE������ת��Ϊ0xB0+0x02
	 * �����������е���0xB0������ת��Ϊ0xB0+0x03
	*/
	size_t max_tmp = (size_t)nSourceLen * 2;
	char* lpszTmp = new char[max_tmp];

	if (lpszTmp == nullptr)
	{
		return;
	}

	if (max_tmp < 4)
	{
		delete[] lpszTmp;
		return;
	}

	memset(lpszTmp, 0, max_tmp);

	int nT = 0;
	for (int nS = 0; nS < nSourceLen; ++nS, ++nT)
	{
		switch (*(pSourceData + nS) & 0xFF)
		{
		case 0xBA:
		{
			lpszTmp[nT] = (char)0xB0;
			lpszTmp[++nT] = (char)0x01;
			break;
		}
		case 0xBE:
		{
			lpszTmp[nT] = (char)0xB0;
			lpszTmp[++nT] = (char)0x02;
			break;
		}
		case 0xB0:
		{
			lpszTmp[nT] = (char)0xB0;
			lpszTmp[++nT] = (char)0x03;
			break;
		}
		default:
		{
			lpszTmp[nT] = *(pSourceData + nS) & 0xFF;
			break;
		}
		}
	}

	pTransLen = nT;
	memcpy_s(pTransData, nT, lpszTmp, nT);

	delete[] lpszTmp;

	return;
}

void CAGV::Retranslation(const char * pTransData, int pTransLen, char * pSourceData, int & nSourceLen)
{
	/*!
	 * ��������0xB0+0x01ת��Ϊ0xBA
	 * ��������0xB0+0x02ת��Ϊ0xBE
	 * ��������0xB0+0x03ת��Ϊ0xB0
	*/
	char* lpszTmp = new char[pTransLen];

	if (lpszTmp == nullptr)
	{
		return;
	}

	memset(lpszTmp, 0, pTransLen);

	int nS = 0;
	for (int nT = 0; nT < pTransLen; ++nT, ++nS)
	{
		if ((*(pTransData + nT) & 0xFF) == 0xB0)
		{
			++nT;
			switch (*(pTransData + nT))
			{
			case 0x01:
			{
				lpszTmp[nS] = (char)0xBA;
				break;
			}
			case 0x02:
			{
				lpszTmp[nS] = (char)0xBE;
				break;
			}
			case 0x03:
			{
				lpszTmp[nS] = (char)0xB0;
				break;
			}
			}
		}
		else
		{
			lpszTmp[nS] = *(pTransData + nT) & 0xFF;
		}
	}

	nSourceLen = nS;
	memcpy_s(pSourceData, nS, lpszTmp, nS);

	delete[] lpszTmp;

	return;
}

void CAGV::InsertIntoWhileList(const char * pData, const int nLen)
{
	DataBuffer pack = CreatePackage(pData, nLen);

	lock_guard<mutex> lock(m_mutexLock);

	for (list<DataBuffer>::iterator it = m_listWhile.begin(); it != m_listWhile.end(); ++it)
	{
		if (*it == pack)
		{
			pack.Release();
			return;
		}
	}

	if (m_listWhile.size() == m_listWhile.max_size())
	{
		m_listWhile.pop_front();
	}

	m_listWhile.push_back(pack);

	return;
}

void CAGV::InsertIntoWhileList(const char* string)
{
	return InsertIntoWhileList(string,(int)strlen(string));
}

void CAGV::InsertIntoSingleList(const char * pData, const int nLen)
{
	DataBuffer pack = CreatePackage(pData, nLen);

	lock_guard<mutex> lock(m_mutexLock);

	for (list<DataBuffer>::iterator it = m_listSingle.begin(); it != m_listSingle.end(); ++it)
	{
		if (*it == pack)
		{
			pack.Release();
			return;
		}
	}

	if (m_listSingle.size() == m_listSingle.max_size())
	{
		m_listSingle.pop_front();
	}

	m_listSingle.push_back(pack);

	return;
}

void CAGV::RemoveFromWhileList(const char * pData, const int nLen)
{
	if (m_listWhile.size() == 0)
	{
		return;
	}

	DataBuffer pack = CreatePackage(pData, nLen);

	lock_guard<mutex> lock(m_mutexLock);

	for (list<DataBuffer>::iterator it = m_listWhile.begin(); it != m_listWhile.end();)
	{
		if (*it == pack)
		{
			it = m_listWhile.erase(it);

			pack.Release();
			break;
		}
		else
		{
			++it;
		}
	}

	return;
}

void CAGV::InsertIntoProcessList(const char * pData, const int nLen)
{
	DataBuffer pack((unsigned char*)pData, nLen);

	lock_guard<mutex> lock(m_mutexProcess);

	for (list<DataBuffer>::iterator it = m_listProcess.begin(); it != m_listProcess.end();)
	{
		if (*it == pack)
		{
			pack.Release();
			return;
		}
		else
		{
			++it;
		}
	}

	if (m_listProcess.size() == m_listProcess.max_size())
	{
		m_listProcess.pop_front();
	}

	m_listProcess.push_back(pack);

	return;
}

bool CAGV::StartConnectThread()
{
	if (m_pThreadConnect)
	{
		return true;
	}

	if (m_hDestroyEvent)
	{
		ResetEvent(m_hDestroyEvent);
	}
	else
	{
		m_hDestroyEvent = CreateEvent(nullptr, true, false, nullptr);
	}

	m_pThreadConnect = new std::thread(&CAGV::ConnectThread, this);

	if (m_pThreadConnect == nullptr)
	{
		return false;
	}

	return true;
}

void CAGV::ConnectThread()
{	
	while (true)
	{
		if (IsConnected() == true)
		{
			if (WaitForSingleObject(m_hDestroyEvent, 1) == WAIT_OBJECT_0)
			{
				break;
			}

			continue;
		}

		if (WaitForSingleObject(m_hDestroyEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}

		if (m_strSrvIP == "")
		{
			if (WaitForSingleObject(m_hDestroyEvent, 1) == WAIT_OBJECT_0)
			{
				break;
			}

			continue;
		}

		if (WaitForSingleObject(m_hDestroyEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
	
		if (TCPClient::Connect() == false)
		{
			if (WaitForSingleObject(m_hDestroyEvent, 1) == WAIT_OBJECT_0)
			{
				break;
			}

			continue;
		}

		Heartbeat();

		if (StartSendThread() == false)
		{
			Break();
		}

		m_chError = _AGVERR_NONE;

		if (m_funcUpdateAGV)
		{
			m_funcUpdateAGV(this, m_pFunc);
		}
		
		if (WaitForSingleObject(m_hDestroyEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
	}

	return;
}

void CAGV::EndConnectThread()
{
	if (m_pThreadConnect == nullptr)
	{
		return;
	}

	if (m_pThreadConnect->joinable())
	{
		SetEvent(m_hDestroyEvent);

		if (m_socketTmp != INVALID_SOCKET)
		{
			shutdown(m_socketTmp, SD_BOTH);
			closesocket(m_socketTmp);
			m_socketTmp = INVALID_SOCKET;
		}

		m_pThreadConnect->join();
	}

	delete m_pThreadConnect;
	m_pThreadConnect = nullptr;

	return;
}

#endif //_AGV_H