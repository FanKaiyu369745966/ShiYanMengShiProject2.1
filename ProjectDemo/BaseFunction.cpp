#include "../ProjectApp2.0/pch.h"
#include "BaseFunction.h"

#ifdef _BASEFUNCTION_H
//#include "../AGVDemo/CAGV.cpp"
#include "../String/DebugPrint.h"
#include "../String/SystemLog.h"
#include "../Thread/ThreadEx.h"
#include "../String/Buffer.h"
#include "../String/StringEx.h"

#include "../json/json.h"
//#include "../json/json_value.cpp"
//#include "../json/json_reader.cpp"
//#include "../json/json_writer.cpp"


mutex BaseFunction::g_mutex;

BaseFunction::BaseFunction()
{
	m_strAGVs = "";
	m_strServer = "";
	m_strTraffic = "";

	m_pThreadMain = nullptr;
	m_hShutdownEvent = nullptr;

	m_pFunc = nullptr;
	m_pPtr = nullptr;

	m_pStrUpdateServer = nullptr;

	m_pJsonReader = m_JsonBuilder.newCharReader();

	
	// ��ʼ��
	for (unsigned short i = 0; i < USHORT_MAX; ++i)
	{
		m_aRFIDArr[i].SetNo(i);
	}

	for (unsigned short i = 0; i < USHORT_MAX; ++i)
	{
		m_ausExhange[i] = i;
	}

	for (unsigned char i = 0; i < UCHAR_MAX; ++i)
	{
		m_bUpdateAGV[i] = false;
	}
}

BaseFunction::~BaseFunction()
{
	m_pFunc = nullptr;

	Close();
	Release();

	delete m_pJsonReader;

	ReleaseString();
}

bool BaseFunction::Init(UpdateStatus func, void* ptr)
{
	m_pFunc = func;
	m_pPtr = ptr;

	InitServer();
	InitTraffic();
	InitExchangeRFID();
	if (InitAGVs() == false)
	{
		return false;
	}

	DebugPrint::Print("�������ܳ�ʼ��");
	SystemLog::Record("�������ܳ�ʼ��");

	return true;
}

bool BaseFunction::Open(UpdateStatus func, void* ptr)
{
	m_pFunc = func;
	m_pPtr = ptr;

	if (StartMainThread() == false)
	{
		Close();

		return false;
	}

	if (m_tcpServer.StartListen(UpdateServer,this) == false)
	{
		Close();

		return false;
	}

	UpdateServer(m_tcpServer);

	DebugPrint::Print("������������");
	SystemLog::Record("������������");

	return true;
}

void BaseFunction::Close()
{
	EndMainThread();

	if (m_tcpServer.IsListened())
	{
		m_tcpServer.CloseListen();

		UpdateServer(m_tcpServer);
	}

	for (map<unsigned char, CAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		if (it->second->IsConnected())
		{
			it->second->Release();

			DebugPrint::Printf("�ж�AGV%d����", it->second->GetNo());
			SystemLog::Recordf("�ж�AGV%d����", it->second->GetNo());

			UpdateAGV(it->second);
		}
	}

	for (unsigned short i = 0; i < USHORT_MAX; ++i)
	{
		m_aRFIDArr[i].Release();
	}

	DebugPrint::Print("�������ܹر�");
	SystemLog::Record("�������ܹر�");

	return;
}

void BaseFunction::Release()
{
	m_pFunc = nullptr;
	m_pPtr = nullptr;

	Close();

	for (map<unsigned char, CAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); it = m_mapAGVs.erase(it))
	{
		delete it->second;
	}

	for (map<unsigned short, Traffic*>::iterator it = m_mapTraffics.begin(); it != m_mapTraffics.end(); it = m_mapTraffics.erase(it))
	{
		delete it->second;
	}

	for (map<unsigned short, Json::Value*>::iterator it = m_mapStrUpdateAGVs.begin(); it != m_mapStrUpdateAGVs.end(); ++it)
	{
		it->second->clear();
	}

	if (m_pStrUpdateServer)
	{
		m_pStrUpdateServer->clear();
	}

	for (unsigned short i = 0; i < USHORT_MAX; ++i)
	{
		m_ausExhange[i] = i;
	}

	CloseHandle(m_hShutdownEvent);
	m_hShutdownEvent = nullptr;

	m_strAGVs = "";
	m_strServer = "";
	m_strTraffic = "";

	DebugPrint::Print("����������Դ�ͷ�");
	SystemLog::Record("����������Դ�ͷ�");

	return;
}

void BaseFunction::ReleaseString()
{
	for (map<unsigned short, Json::Value*>::iterator it = m_mapStrUpdateAGVs.begin(); it != m_mapStrUpdateAGVs.end(); it = m_mapStrUpdateAGVs.erase(it))
	{
		delete it->second;
	}

	if (m_pStrUpdateServer)
	{
		delete m_pStrUpdateServer;
		m_pStrUpdateServer = nullptr;
	}
}

void BaseFunction::JsonStringToServer(const char * strJson)
{
	m_strServer = strJson;

	return;
}

void BaseFunction::JsonStringToServer(const wchar_t * wstrJson)
{
	m_strServer = StringEx::wstring_to_string(wstrJson);

	return;
}

void BaseFunction::JsonFileToServer(const char * strFile)
{
	ifstream fin;
	fin.open(strFile);
	if (!fin)
	{
		//TCHAR("error");
		return;
	}
	ostringstream ostring;
	ostring << fin.rdbuf();
	fin.close();

	m_strServer = ostring.str();

	return;
}

void BaseFunction::JsonFileToServer(const wchar_t * wstrFile)
{
	ifstream fin;
	fin.open(StringEx::wstring_to_string(wstrFile).c_str());
	if (!fin)
	{
		//TCHAR("error");
		return;
	}
	ostringstream ostring;
	ostring << fin.rdbuf();
	fin.close();

	m_strServer = ostring.str();

	return;
}

void BaseFunction::JsonStringToAGVs(const char * strJson)
{
	m_strAGVs = strJson;

	return;
}

void BaseFunction::JsonStringToAGVs(const wchar_t * wstrJson)
{
	m_strAGVs = StringEx::wstring_to_string(wstrJson);

	return;
}

void BaseFunction::JsonFileToAGVs(const char * strFile)
{
	ifstream fin;
	fin.open(strFile);
	if (!fin)
	{
		//TCHAR("error");
		return;
	}
	ostringstream ostring;
	ostring << fin.rdbuf();
	fin.close();

	m_strAGVs = ostring.str();

	return;
}

void BaseFunction::JsonFileToAGVs(const wchar_t * wstrFile)
{
	ifstream fin;
	fin.open(StringEx::wstring_to_string(wstrFile).c_str());
	if (!fin)
	{
		//TCHAR("error");
		return;
	}
	ostringstream ostring;
	ostring << fin.rdbuf();
	fin.close();

	m_strAGVs = ostring.str();

	return;
}

void BaseFunction::JsonStringToTraffics(const char * strJson)
{
	m_strTraffic = strJson;

	return;
}

void BaseFunction::JsonStringToTraffics(const wchar_t * wstrJson)
{
	m_strTraffic = StringEx::wstring_to_string(wstrJson);

	return;
}

void BaseFunction::JsonFileToTraffics(const char * strFile)
{
	ifstream fin;
	fin.open(strFile);
	if (!fin)
	{
		//TCHAR("error");
		return;
	}
	ostringstream ostring;
	ostring << fin.rdbuf();
	fin.close();

	m_strTraffic = ostring.str();

	return;
}

void BaseFunction::JsonFileToTraffics(const wchar_t * wstrFile)
{
	ifstream fin;
	fin.open(StringEx::wstring_to_string(wstrFile).c_str());
	if (!fin)
	{
		//TCHAR("error");
		return;
	}
	ostringstream ostring;
	ostring << fin.rdbuf();
	fin.close();

	m_strTraffic = ostring.str();

	return;
}

void BaseFunction::JsonStringToRFIDExchanges(const char* strJson)
{
	m_strExchange = strJson;

	return;
}

void BaseFunction::JsonStringToRFIDExchanges(const wchar_t* wstrJson)
{
	m_strExchange = StringEx::wstring_to_string(wstrJson);

	return;
}

void BaseFunction::JsonFileToRFIDExchanges(const char* strFile)
{
	ifstream fin;
	fin.open(strFile);
	if (!fin)
	{
		//TCHAR("error");
		return;
	}
	ostringstream ostring;
	ostring << fin.rdbuf();
	fin.close();

	m_strExchange = ostring.str();

	return;
}

void BaseFunction::JsonFileToRFIDExchanges(const wchar_t* wstrFile)
{
	ifstream fin;
	fin.open(StringEx::wstring_to_string(wstrFile).c_str());
	if (!fin)
	{
		//TCHAR("error");
		return;
	}
	ostringstream ostring;
	ostring << fin.rdbuf();
	fin.close();

	m_strExchange = ostring.str();

	return;
}

void BaseFunction::Accept(SOCKET socket)
{
	if (AcceptClient(socket))
	{
		return;
	}

	shutdown(socket, SD_BOTH);
	closesocket(socket);

	return;
}

void BaseFunction::InitServer()
{
	Json::Value JsonRoot;

	const char* pstr = (char*)m_strServer.c_str();

	m_mutexJson.lock();
	if (!m_pJsonReader->parse(pstr, pstr + strlen(pstr), &JsonRoot, &m_JstrErrs))
	{
		DebugPrint::Print(m_JstrErrs.c_str());
		DebugPrint::Print("��ʼ�������ʧ��:\n��JSON�����ַ���");

		m_JstrErrs.clear();
		m_mutexJson.unlock();
		return;
	}
	m_mutexJson.unlock();

	string strIP = Json_ReadString(JsonRoot["IP"]);
	int nPort = Json_ReadInt(JsonRoot["Port"]);

	m_tcpServer.Bind(nPort, strIP);

	strIP.clear();
	JsonRoot.clear();

	return;
}

void BaseFunction::InitTraffic()
{
	Json::Value JsonRoot;

	const char* pstr = m_strTraffic.c_str();

	m_mutexJson.lock();
	if (!m_pJsonReader->parse(pstr, pstr + strlen(pstr), &JsonRoot, &m_JstrErrs))
	{
		DebugPrint::Print(m_JstrErrs.c_str());
		DebugPrint::Print("��ʼ����ͨ����ʧ��:\n��JSON�����ַ���");

		m_JstrErrs.clear();
		m_mutexJson.unlock();
		return;
	}
	m_mutexJson.unlock();

	Json::Value ObjectArray;
	ObjectArray = JsonRoot["Traffic"];

	for (size_t k = 0; k < ObjectArray.size(); ++k)
	{
		int nCtrl = Json_ReadInt(ObjectArray[(int)k]["Control"]);

		if (m_mapTraffics.find(nCtrl) == m_mapTraffics.end())
		{
			m_mapTraffics[nCtrl] = new Traffic(nCtrl);
		}

		Json::Value ArrInt;
		ArrInt = ObjectArray[(int)k]["Stop"];

		for (size_t i = 0; i < ArrInt.size(); ++i)
		{
			int nRFID = Json_ReadInt(ArrInt[(int)i]);

			m_mapTraffics[nCtrl]->AddStopRFID(nRFID);
		}

		ArrInt.clear();

		ArrInt = ObjectArray[(int)k]["Compare"];

		for (size_t i = 0; i < ArrInt.size(); ++i)
		{
			int nArea = Json_ReadInt(ArrInt[(int)i]);

			m_mapTraffics[nCtrl]->AddCompareArea(nArea);
		}

		ArrInt.clear();
	}

	ObjectArray.clear();
	JsonRoot.clear();

	return;
}

bool BaseFunction::InitAGVs()
{
	Json::Value JsonRoot;

	const char* pstr = (char*)m_strAGVs.c_str();

	m_mutexJson.lock();
	if (!m_pJsonReader->parse(pstr, pstr + strlen(pstr), &JsonRoot, &m_JstrErrs))
	{
		DebugPrint::Print(m_JstrErrs.c_str());
		DebugPrint::Print("��ʼ��AGVʧ��:\n��JSON�����ַ���");

		m_JstrErrs.clear();
		m_mutexJson.unlock();
		return false;
	}
	m_mutexJson.unlock();
	
	Json::Value ObjectArray, Param;
	ObjectArray = JsonRoot["AGV"];

	if (ObjectArray.size() == 0)
	{
		return false;
	}

	for (size_t k = 0; k < ObjectArray.size(); ++k, Param.clear())
	{
		int nNo = Json_ReadInt(ObjectArray[(int)k]["No"]);
		int nCurRFID = Json_ReadInt(ObjectArray[(int)k]["RFID"]["Cur"]);
		int nEndRFID = Json_ReadInt(ObjectArray[(int)k]["RFID"]["End"]);
		int nStatus = Json_ReadInt(ObjectArray[(int)k]["Status"]);
		Param = ObjectArray[(int)k]["Param"];
		string strSrvIP = Json_ReadString(Param["ServerIP"]);		/*!< �����IP:������Ϊ�����,AGVΪ�ͻ���ʱֵΪ��;��AGVΪ�����,����Ϊ�ͻ���ʱֵΪAGV��IP��ַ */
		int nSrvPort = Json_ReadInt(Param["ServerPort"]);			/*!< ����˶˿�:������Ϊ�����,AGVΪ�ͻ���ʱֵΪ-1;��AGVΪ�����,����Ϊ�ͻ���ʱֵΪAGV�Ķ˿� */
		string strClientIP = Json_ReadString(Param["ClientIP"]);	/*!< �ͻ���IP:������Ϊ�����,AGVΪ�ͻ���ʱֵΪAGV��IP��ַ;��AGVΪ�����,����Ϊ�ͻ���ʱֵΪ�� */
		int nClientPort = Json_ReadInt(Param["ClientPort"]);		/*!< �ͻ��˶˿�:������Ϊ�����,AGVΪ�ͻ���ʱֵΪAGV�Ķ˿�,ֵΪ-1ʱ��Ϊ����˿�;��AGVΪ�����,����Ϊ�ͻ���ʱֵΪ����ָ���˿�,ֵΪ-1ʱ��Ϊ����˿� */

		if (m_mapAGVs.find(nNo) == m_mapAGVs.end())
		{
			m_mapAGVs[nNo] = new CAGV(nNo, UpdateAGV,this, strClientIP,strSrvIP, nClientPort,nSrvPort);
		}

		unsigned short usOldRFID = m_mapAGVs[nNo]->UpdateCurRFID(nCurRFID);
		m_mapAGVs[nNo]->UpdateEndRFID(nEndRFID);
		m_mapAGVs[nNo]->UpdateStatus(nStatus);

		// ������RFID
		m_aRFIDArr[usOldRFID].Unlock(m_mapAGVs[nNo]);

		// ������RFID
		m_aRFIDArr[nCurRFID].Lock(m_mapAGVs[nNo]);

		UpdateAGV(m_mapAGVs[nNo]);

		strSrvIP.clear();
		strClientIP.clear();
	}

	ObjectArray.clear();
	JsonRoot.clear();

	return true;
}

bool BaseFunction::InitExchangeRFID()
{
	Json::Value JsonRoot;

	const char* pstr = (char*)m_strExchange.c_str();

	m_mutexJson.lock();
	if (!m_pJsonReader->parse(pstr, pstr + strlen(pstr), &JsonRoot, &m_JstrErrs))
	{
		DebugPrint::Print(m_JstrErrs.c_str());
		DebugPrint::Print("��ʼ��RFIDת����ʧ��:\n��JSON�����ַ���");

		m_JstrErrs.clear();
		m_mutexJson.unlock();
		return false;
	}
	m_mutexJson.unlock();

	Json::Value ObjectArray, Source;
	ObjectArray = JsonRoot["Exchange"];

	if (ObjectArray.size() == 0)
	{
		return false;
	}

	for (size_t k = 0; k < ObjectArray.size(); ++k, Source.clear())
	{
		Source = ObjectArray[(int)k]["Source"];
		int nExchange = Json_ReadInt(ObjectArray[(int)k]["Exchange"]);
		int nSource = 0;

		for (size_t s = 0; s < Source.size(); ++s, nSource = 0)
		{
			nSource = Json_ReadInt(Source[(int)s]);
			m_ausExhange[nSource] = nExchange;
		}
	}

	return false;
}

void BaseFunction::ProcessAGVPackage()
{
	DataBuffer package = CAGV::GetProcessPackage();

	if (package.m_unSize == 0)
	{
		return;
	}

	Json::Value JsonRoot;

	const char* pstr = (char*)package.m_pbyBuffer;

	m_mutexJson.lock();
	if (!m_pJsonReader->parse(pstr, pstr + strlen(pstr), &JsonRoot, &m_JstrErrs))
	{
		DebugPrint::Print(m_JstrErrs.c_str());
		DebugPrint::Print("����AGV����ʧ��:\n��JSON�����ַ���");

		m_JstrErrs.clear();
		m_mutexJson.unlock();
		return;
	}
	m_mutexJson.unlock();

	Json::Value JsonMsg = JsonRoot["Message"];

	int nNo = Json_ReadInt(JsonMsg["No"]);				/*!< AGV��� */
	string strCmd = Json_ReadString(JsonMsg["Cmd"]);	/*!< AGVָ�� */
	string strType = Json_ReadString(JsonMsg["Type"]);	/*!< AGV���� */

	if (strcmp(strType.c_str(), "AGV") != 0)
	{
		strType.clear();
		strType.clear();
		JsonMsg.clear();

		return;
	}

	if (m_mapAGVs.find(nNo) == m_mapAGVs.end())
	{
		strType.clear();
		strType.clear();
		JsonMsg.clear();

		return;
	}

	// ��������
	if (strcmp(strCmd.c_str(), "Heartbeat") == 0)
	{
		Json::Value JsonResult = JsonRoot["Result"];

		Json::Value JsonRFID = JsonResult["RFID"];

		unsigned short usCurRFID = Json_ReadInt(JsonRFID["Cur"]);					/*!< ��ǰRFID�� */
		usCurRFID = ExchangeRFID(usCurRFID);

		if (usCurRFID != 0)
		{
			unsigned short usOldRFID = m_mapAGVs[nNo]->UpdateCurRFID(usCurRFID);	/*!< ��һRFID��*/

			m_aRFIDArr[usOldRFID].Unlock(m_mapAGVs[nNo]);

			m_aRFIDArr[usCurRFID].Lock(m_mapAGVs[nNo]);
		}

		unsigned short usNextRFID = Json_ReadInt(JsonRFID["Next"]);					/*!< ��һRFID�� */
		unsigned short usOldNextRFID = m_mapAGVs[nNo]->UpdateNextRFID(usNextRFID);	/*!< ����һRFID�� */

		m_aRFIDArr[usOldNextRFID].PreformUnlock(m_mapAGVs[nNo]);

		m_aRFIDArr[usNextRFID].PreformLock(m_mapAGVs[nNo]);

		unsigned short usEndRFID = Json_ReadInt(JsonRFID["End"]);				/*!< �н��յ�RFID�� */
		unsigned short usOldEndRFID = m_mapAGVs[nNo]->UpdateEndRFID(usEndRFID);	/*!< ���н��յ�RFID�� */

		m_aRFIDArr[usOldEndRFID].PreformUnlock(m_mapAGVs[nNo]);

		m_aRFIDArr[usEndRFID].PreformLock(m_mapAGVs[nNo]);

		m_mapAGVs[nNo]->UpdateStatus(Json_ReadInt(JsonResult["Status"]));
		m_mapAGVs[nNo]->UpdateSpeed(Json_ReadInt(JsonResult["Speed"]));
		m_mapAGVs[nNo]->UpdatePower(Json_ReadInt(JsonResult["Power"]));
		m_mapAGVs[nNo]->UpdateCargo(Json_ReadInt(JsonResult["Cargo"]));
		m_mapAGVs[nNo]->UpdateLifter(Json_ReadInt(JsonResult["Lifter"]));
		m_mapAGVs[nNo]->UpdateAction(Json_ReadInt(JsonResult["Action"]));
		m_mapAGVs[nNo]->UpdateActStatus(Json_ReadInt(JsonResult["ActStatus"]));
		m_mapAGVs[nNo]->UpdateError(Json_ReadInt(JsonResult["Error"]));

		UpdateAGV(m_mapAGVs[nNo]);

		ProcessAGVStatus(m_mapAGVs[nNo]);

		JsonRFID.clear();
		JsonResult.clear();
	}
	// ��������
	else
	{

		strType.clear();
		strType.clear();
		JsonMsg.clear();

		return;
	}

	strType.clear();
	strType.clear();
	JsonMsg.clear();

	return;
}

void BaseFunction::TrafficControl()
{
	for (map<unsigned short, Traffic*>::iterator it = m_mapTraffics.begin(); it != m_mapTraffics.end(); ++it)
	{
		unsigned short usCtrlRFID = it->second->GetControlRFID();	/*!< ����RFID�� */

		TrafficControl(m_aRFIDArr[usCtrlRFID].GetLocker());
	}

	return;
}

void BaseFunction::TrafficControl(void * pUser)
{
	// ָ����Ч
	if (pUser == nullptr)
	{
		return;
	}

	CAGV* pAGV = (CAGV*)pUser;	/*!< AGVָ�� */

	// AGV�����ڽ�ͨ����״̬
	if (pAGV->GetStatus() != _AGVSTA_TRAFFICSTOP)
	{
		return;
	}

	unsigned short usCurRFID = pAGV->GetCurRFID();	/*!< AGV��ǰRFID�� */

	if (m_mapTraffics.find(usCurRFID) == m_mapTraffics.end())
	{
		DebugPrint::Printf("AGV%d��ͨ���ƿ���ʧ��:\n��������Ч�Ľ�ͨ��������", pAGV->GetNo());
		return;
	}

	/*
	 * ����������
	*/
	set<unsigned short> setStop = m_mapTraffics[usCurRFID]->GetStopArray();	/*!< ����RFID�� */

	for (set<unsigned short>::iterator it = setStop.begin(); it != setStop.end(); ++it)
	{
		unsigned short usStopRFID = *it;	/*!< ����RFID�� */

		CAGV* pStopAGV = (CAGV*)m_aRFIDArr[usStopRFID].GetLocker();	/*!< ����RFID���ϵ��н�AGVָ�� */

		// ����RFID���ϴ���AGV
		if (pStopAGV != nullptr && pStopAGV != pAGV)
		{
			DebugPrint::Printf("AGV%d��ֹ����:\n������������н�AGV%d", pAGV->GetNo(), pStopAGV->GetNo());
			return;
		}
	}

	setStop.clear();

	/*
	 * �ȶ�������
	*/
	set<unsigned short> setCompare = m_mapTraffics[usCurRFID]->GetCompareArray();

	for (set<unsigned short>::iterator it = setCompare.begin(); it != setCompare.end(); ++it)
	{
		unsigned short usCompareArea = *it;

		if (m_mapTraffics.find(usCompareArea) == m_mapTraffics.end())
		{
			continue;
		}

		/*
		 * ����������
		*/
		setStop = m_mapTraffics[usCompareArea]->GetStopArray();	/*!< ����RFID�� */

		for (set<unsigned short>::iterator itStop = setStop.begin(); itStop != setStop.end(); ++itStop)
		{
			unsigned short usStopRFID = *itStop;	/*!< ����RFID�� */

			CAGV* pStopAGV = (CAGV*)m_aRFIDArr[usStopRFID].GetLocker();	/*!< ����RFID���ϵ��н�AGVָ�� */

			// ����RFID���ϴ���AGV
			if (pStopAGV != nullptr && pStopAGV != pAGV)
			{
				DebugPrint::Printf("AGV%d��ֹ����:\n������������н�AGV%d", pAGV->GetNo(), pStopAGV->GetNo());
				return;
			}
		}

		CAGV* pAGVCmp = (CAGV*)m_aRFIDArr[usCompareArea].GetLocker();

		if (pAGVCmp == nullptr)
		{
			continue;
		}

		unsigned char byStatus = pAGVCmp->GetStatus();

		if(byStatus != _AGVSTA_TRAFFICSTOP && byStatus != _AGVSTA_TRAFFICPASS && byStatus != _AGVSTA_RUN)
		{
			continue;
		}

		long long nResul = m_aRFIDArr[usCurRFID].Compare(m_aRFIDArr[usCompareArea]);	/*!< �ȶ�ʱ���ֵ */

		if (nResul > 0)
		{
			DebugPrint::Printf("AGV%d��ֹ����:\n�����ȵ���������AGV", pAGV->GetNo());
			return;
		}
	}

	pAGV->TrafficPass(usCurRFID);

	return;
}

void BaseFunction::ObsControl(void * pUser)
{
	// ָ����Ч
	if (pUser == nullptr)
	{
		return;
	}

	CAGV* pAGV = (CAGV*)pUser;								/*!< AGVָ�� */

	unsigned short usCurRFID = pAGV->GetCurRFID();			/*!< ��ǰ�н�RFID�� */
	unsigned short usNextRFID = pAGV->GetNextRFID();		/*!< �н���һRFID�� */
	char chSpeed = pAGV->GetSpeed();						/*!< ��ǰ�ٶ� */
	unsigned short usAction = pAGV->GetAction();			/*!< ������Ϣ */
	unsigned char byActStatus = pAGV->GetActionStatus();	/*!< ����״̬ */
	unsigned char byStatus = pAGV->GetStatus();				/*!< ״̬��Ϣ */

	// AGV�������л������״̬
	if (byStatus != _AGVSTA_RUN && byStatus != _AGVSTA_WAIT)
	{
		return;
	}

	if (usCurRFID != 0)
	{
		CAGV* pLockerAGV = (CAGV*)m_aRFIDArr[usCurRFID].GetLocker();

		if (pLockerAGV != nullptr && pLockerAGV != pAGV)
		{
			char chLockerSpeed = pLockerAGV->GetSpeed();			/*!< ǰ��AGV���ٶ� */
			unsigned char byLockerStatus = pLockerAGV->GetStatus();	/*!< ǰ��AGV��״̬*/

			if (byLockerStatus == _AGVSTA_RUN || byLockerStatus == _AGVSTA_OBSDOWN)
			{
				// �ٶȹ��������
				if (chSpeed > chLockerSpeed)
				{
					pAGV->SpeedControl(chLockerSpeed);
				}
				else
				{
					// �ƶ���ͣ�����
					if ((usAction == _AGVACT_MOVE || usAction == _AGVACT_REMOVE) && byActStatus == _AGVACTSTA_PAUSE)
					{
						pAGV->Continue();
					}
				}
			}
			else
			{
				if (chSpeed > 0 && (usAction == _AGVACT_MOVE || usAction == _AGVACT_REMOVE))
				{
					pAGV->Pause();
				}
			}
		}
	}
	
	if (usNextRFID != 0)
	{
		CAGV* pLockerAGV = (CAGV*)m_aRFIDArr[usNextRFID].GetLocker();

		if (pLockerAGV != nullptr && pLockerAGV != pAGV)
		{
			char chLockerSpeed = pLockerAGV->GetSpeed();
			unsigned char byLockerStatus = pLockerAGV->GetStatus();

			if (byLockerStatus == _AGVSTA_RUN || byLockerStatus == _AGVSTA_OBSDOWN)
			{
				if ((usAction == _AGVACT_MOVE || usAction == _AGVACT_REMOVE) && byActStatus == _AGVACTSTA_PAUSE)
				{
					pAGV->Continue();
				}
			}
			else
			{
				if (chSpeed > 0 && (usAction == _AGVACT_MOVE || usAction == _AGVACT_REMOVE))
				{
					pAGV->Pause();
				}
			}
		}
	}
	
	return;
}

bool BaseFunction::AcceptAGV(SOCKET socket)
{
	// ����AGV
	for (map<unsigned char, CAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		if (it->second->Connect(socket))
		{
			DebugPrint::Printf("������AGV%d", it->second->GetNo());
			SystemLog::Recordf("������AGV%d", it->second->GetNo());

			it->second->UpdateError(_AGVERR_NONE);

			UpdateAGV(it->second);

			return true;
		}
	}

	return false;
}

void BaseFunction::ConnectAGV()
{
	for (map<unsigned char, CAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		it->second->StartConnectThread();
	}

	return;
}

bool BaseFunction::AcceptClient(SOCKET socket)
{
	if (AcceptAGV(socket))
	{
		return true;
	}

	return false;
}

void BaseFunction::ProcessAGVStatus(void* pUser)
{
	if (pUser == nullptr)
	{
		return;
	}

	CAGV* pAGV = (CAGV*)pUser;

	if (pAGV->GetStatus() == _AGVSTA_TRAFFICSTOP)
	{
		TrafficControl(pAGV);
	}
	else
	{
		ObsControl(pAGV);
	}
}

const unsigned short BaseFunction::ExchangeRFID(const unsigned short usRFID)
{
	return m_ausExhange[usRFID];
}

void BaseFunction::MainThread()
{
	ULL ullTime = 0;
	while (true)
	{
		Function();
		
		if (WaitForSingleObject(m_hShutdownEvent, 100) == WAIT_OBJECT_0)
		{
			break;
		}
	}

	return;
}

bool BaseFunction::StartMainThread()
{
	EndMainThread();

	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(nullptr, true, false, nullptr);
	}

	m_pThreadMain = new std::thread(&BaseFunction::MainThread, this);

	if (m_pThreadMain)
	{
		return true;
	}

	return false;
}

void BaseFunction::EndMainThread()
{
	if (m_pThreadMain == nullptr)
	{
		return;
	}

	if (m_pThreadMain->joinable())
	{
		SetEvent(m_hShutdownEvent);

		m_pThreadMain->join();
	}

	delete m_pThreadMain;
	m_pThreadMain = nullptr;

	return;
}

void BaseFunction::Function()
{
	ConnectAGV();

	ProcessAGVPackage();

	//TrafficControl();
}

void BaseFunction::UpdateAGV(void* pAGV, void* ptr)
{
	return ((BaseFunction*)ptr)->UpdateAGV((CAGV*)pAGV);
}

void BaseFunction::UpdateServer(void* pServer, void* ptr, SOCKET socket)
{
	if (socket != INVALID_SOCKET)
	{
		((BaseFunction*)ptr)->AcceptClient(socket);

		return;
	}

	return ((BaseFunction*)ptr)->UpdateServer(((BaseFunction*)ptr)->m_tcpServer);
}

void BaseFunction::UpdateAGV(CAGV * pAGV)
{
	if (m_pFunc == nullptr)
	{
		return;
	}

	if (pAGV == nullptr)
	{
		return;
	}
	/*
	{
		"No":1,
		"Connect":true,
		"Status":0,
		"RFID":
		{
			"Old":0,
			"Cur":0,
			"Next":0,
			"End":0
		},
		"Message":
		{
			"IP":"",
			"Port":"",
			"Status":
			{
				"Value":"",
				"Color":""
			},
			"Speed":"",
			"Power":
			{
				"Value":"",
				"Color":""
			},
			"Cargo":
			{
				"Value":"",
				"Color":""
			},
			"Lifter":"",
			"Error":
			{
				"Value":"",
				"Color":""
			},
			"Action":"",
		},
		"View":
		{
			"Obs":false,
			"Run":false,
			"Unvol":true,
			"Cargo":false,
			"Error":false,
			"Action":0
		}
	}
	*/
	string strMsg = "";							/*!< ��Ϣ */
	string strColor = "";						/*!< ��ɫ */

	string strIP = "";	/*!< IP��ַ */
	int nPort = -1;		/*!< �˿�*/
	pAGV->GetIPandPort(strIP, nPort);
	unsigned char byNo = pAGV->GetNo();						/*!< ��� */
	unsigned char byStatus = pAGV->GetStatus();				/*!< ����״̬ */
	unsigned char byPower = pAGV->GetPower();				/*!< ���� */
	unsigned char byCargo = pAGV->GetCargo();				/*!< �ػ�״̬ */
	unsigned char byLifter = pAGV->GetLifter();				/*!< ������״̬ */
	char chError = pAGV->GetError();						/*!< �쳣��Ϣ */
	unsigned short usAction = pAGV->GetAction();			/*!< ������Ϣ */
	unsigned char byActStatus = pAGV->GetActionStatus();	/*!< ����״̬ */
	bool bObs = false;										/*!< ���ϱ�ʶ */
	bool bStop = true;										/*!< ֹͣ��ʶ */
	bool Unvol = false;										/*!< �͵�����ʶ */
	bool bCargo = true;										/*!< �ػ���ʶ */
	bool bNone = false;										/*!< ���쳣��ʶ */
	char chLoad = 0;										/*!< ��/����״̬ 0�޶��� 1������ 2������ */
		
	Json::Value JsonRoot, JsonRFID,JsonMsg, JsonView, JsonStatus, JsonPower, JsonCargo, JsonError;

	JsonRoot["Type"] = Json::Value("AGV");
	JsonRoot["No"] = Json::Value(byNo);
	JsonRoot["Connect"] = Json::Value(pAGV->IsConnected());
	JsonRoot["Status"] = Json::Value(pAGV->GetStatus());

	JsonRFID["Old"] = Json::Value(pAGV->GetOldRFID());
	JsonRFID["Cur"] = Json::Value(pAGV->GetCurRFID());
	JsonRFID["Next"] = Json::Value(pAGV->GetNextRFID());
	JsonRFID["End"] = Json::Value(pAGV->GetEndRFID());

	JsonMsg["IP"] = Json::Value(strIP.c_str());
	JsonMsg["Port"] = Json::Value(StringEx::string_format("%d",nPort).c_str());

	strMsg = "";
	strColor = "";
	switch (byStatus)
	{
	case _AGVSTA_WAIT:
	{
		strMsg = "����";
		strColor = "White";
		break;
	}
	case _AGVSTA_RUN:
	{
		strMsg = "����";
		strColor = "Green";
		bStop = false;
		break;
	}
	case _AGVSTA_STOP:
	{
		strMsg = "ֹͣ";
		strColor = "Yellow";
		break;
	}
	case _AGVSTA_SCREAM:
	{
		strMsg = "��ͣ";
		strColor = "Red";
		break;
	}
	case _AGVSTA_FIND:
	{
		strMsg = "Ѱ��";
		strColor = "Yellow";
		break;
	}
	case _AGVSTA_OBSDOWN:
	{
		strMsg = "���ϼ���";
		strColor = "Green";
		bStop = false;
		bObs = true;
		break;
	}
	case _AGVSTA_OBSSTOP:
	{
		strMsg = "����ֹͣ";
		strColor = "Yellow";
		bObs = true;
		break;
	}
	case _AGVSTA_MOBSSTOP:
	{
		strMsg = "��ײֹͣ";
		strColor = "Red";
		bObs = true;
		break;
	}
	case _AGVSTA_TRAFFICSTOP:
	{
		strMsg = "��ͨ����ֹͣ";
		strColor = "Yellow";
		break;
	}
	case _AGVSTA_ARM:
	{
		strMsg = "��е�۷���";
		strColor = "Green";
		break;
	}
	case _AGVSTA_ALLSCREAM:
	{
		strMsg = "ȫ�߼�ͣ";
		strColor = "Red";
		break;
	}
	case _AGVSTA_TRAFFICPASS:
	{
		strMsg = "��ͨ���Ʒ���";
		strColor = "Green";
		break;
	}
	}
	
	JsonStatus["Value"] = Json::Value(StringEx::AcsiiToUtf8(strMsg.c_str()));
	JsonStatus["Color"] = Json::Value(strColor.c_str());

	JsonMsg["Speed"] = Json::Value(StringEx::string_format("%d%%",pAGV->GetSpeed()).c_str());

	strColor = "";
	if (byPower == _AGVPOWER_EMPTY)
	{
		strColor = "Red";
		Unvol = true;
	}
	else if (byPower <= _AGVPOWER_LOW)
	{
		strColor = "Red";
		Unvol = true;
	}
	else if (byPower <= _AGVPOWER_NORMAL)
	{
		strColor = "White";
	}
	else if (byPower <= _AGVPOWER_HIGH)
	{
		strColor = "Green";
	}
	else if (byPower <= _AGVPOWER_FULL)
	{
		strColor = "Green";
	}
	JsonPower["Value"] = Json::Value(StringEx::string_format("%d%%",byPower).c_str());
	JsonPower["Color"] = Json::Value(strColor.c_str());

	strMsg = "";
	strColor = "";
	switch (byCargo)
	{
	case _AGVCARGO_EMPTY:
	{
		strMsg = "����";
		strColor = "Green";
		bCargo = false;
		break;
	}
	case _AGVCARGO_FULL:
	{
		strMsg = "����";
		strColor = "Red";
		break;
	}
	default:
	{
		strMsg = StringEx::string_format("%d��", byCargo);
		break;
	}
	}
	JsonCargo["Value"] = Json::Value(StringEx::AcsiiToUtf8(strMsg.c_str()));
	JsonCargo["Color"] = Json::Value(strColor.c_str());

	strMsg = "";	
	switch (byLifter)
	{
	case _AGVLIFTER_UP:
	{
		strMsg = "����";
		break;
	}
	case _AGVLIFTER_DOWN:
	{
		strMsg = "����";
		break;
	}
	}
	JsonMsg["Lifter"] = Json::Value(StringEx::AcsiiToUtf8(strMsg.c_str()));

	strMsg = "";
	strColor = "Red";
	switch (chError)
	{
	case _AGVERR_NONE:
	{
		strMsg = "����";
		strColor = "Green";
		bNone = true;
		break;
	}
	case _AGVERR_MISS:
	{
		strMsg = "�Ѵ�";
		break;
	}
	case _AGVERR_PATH:
	{
		strMsg = "·���쳣";
		break;
	}
	case _AGVERR_LOCATE:
	{
		strMsg = "�����쳣";
		break;
	}
	case _AGVERR_DIRECTION:
	{
		strMsg = "�����쳣";
		break;
	}
	case _AGVERR_ACT:
	{
		strMsg = "�����쳣";
		break;
	}
	case _AGVERR_COMM:
	{
		strMsg = "ͨ���쳣";
		break;
	}
	case _AGVERR_LIFTUP:
		strMsg = "�����쳣";
		break;
	}
	JsonError["Value"] = Json::Value(StringEx::AcsiiToUtf8(strMsg.c_str()));
	JsonError["Color"] = Json::Value(strColor.c_str());

	strMsg = "";
	switch (usAction)
	{
	case _AGVACT_NONE:
	{
		strMsg = "�޶���";

		break;
	}
	case _AGVACT_MOVE:
	{
		switch (byActStatus)
		{
		case _AGVACTSTA_NONE:
		{
			strMsg = "�ƶ�δִ��";
			break;
		}
		case _AGVACTSTA_EXE:
		{
			strMsg = "�ƶ���";
			break;
		}
		case _AGVACTSTA_FINISH:
		{
			strMsg = "�ƶ����";
			break;
		}
		case _AGVACTSTA_PAUSE:
		{
			strMsg = "�ƶ���ͣ";
			break;
		}
		}

		break;
	}
	case _AGVACT_REMOVE:
	{
		switch (byActStatus)
		{
		case _AGVACTSTA_NONE:
		{
			strMsg = "�����ƶ�δִ��";
			break;
		}
		case _AGVACTSTA_EXE:
		{
			strMsg = "�����ƶ���";
			break;
		}
		case _AGVACTSTA_FINISH:
		{
			strMsg = "�����ƶ����";
			break;
		}
		case _AGVACTSTA_PAUSE:
		{
			strMsg = "�����ƶ���ͣ";
			break;
		}
		}

		break;
	}
	case _AGVACT_LIFTUP:
	{
		switch (byActStatus)
		{
		case _AGVACTSTA_NONE:
		{
			strMsg = "����������δִ��";
			chLoad = 1;
			break;
		}
		case _AGVACTSTA_EXE:
		{
			strMsg = "������������";
			chLoad = 1;
			break;
		}
		case _AGVACTSTA_FINISH:
		{
			strMsg = "������������";
			break;
		}
		case _AGVACTSTA_PAUSE:
		{
			strMsg = "������������ͣ";
			chLoad = 1;
			break;
		}
		}

		break;
	}
	case _AGVACT_LIFTDOWN:
	{
		switch (byActStatus)
		{
		case _AGVACTSTA_NONE:
		{
			strMsg = "����������δִ��";
			chLoad = 2;
			break;
		}
		case _AGVACTSTA_EXE:
		{
			strMsg = "�������½���";
			chLoad = 2;
			break;
		}
		case _AGVACTSTA_FINISH:
		{
			strMsg = "������������";
			break;
		}
		case _AGVACTSTA_PAUSE:
		{
			strMsg = "�������½���ͣ";
			chLoad = 2;
			break;
		}
		}

		break;
	}
	}
	JsonMsg["Action"] = Json::Value(StringEx::AcsiiToUtf8(strMsg.c_str()));

	JsonView["Obs"] = Json::Value(bObs);
	JsonView["Run"] = Json::Value(!bStop);
	JsonView["Unvol"] = Json::Value(Unvol);
	JsonView["Cargo"] = Json::Value(bCargo);
	JsonView["Error"] = Json::Value(!bNone);
	JsonView["Action"] = Json::Value(chLoad);

	JsonMsg["Status"] = JsonStatus;
	JsonMsg["Power"] = JsonPower;
	JsonMsg["Cargo"] = JsonCargo;
	JsonMsg["Error"] = JsonError;

	JsonRoot["RFID"] = JsonRFID;
	JsonRoot["Message"] = JsonMsg;
	JsonRoot["View"] = JsonView;

	bool bUpdate = false;
	if (m_mapStrUpdateAGVs.find(byNo) == m_mapStrUpdateAGVs.end())
	{
		g_mutex.lock();
		m_mapStrUpdateAGVs[byNo] = new Json::Value(JsonRoot);
		g_mutex.unlock();

		bUpdate = true;
	}
	else
	{
		if (JsonRoot != *m_mapStrUpdateAGVs[byNo])
		{
			if (JsonRoot["RFID"] != (*m_mapStrUpdateAGVs[byNo])["RFID"]
				|| JsonRoot["Status"] != (*m_mapStrUpdateAGVs[byNo])["Status"])
			{
				m_bUpdateAGV[byNo - 1] = true;
			}

			g_mutex.lock();
			m_mapStrUpdateAGVs[byNo]->clear();
			*m_mapStrUpdateAGVs[byNo] = JsonRoot;
			g_mutex.unlock();

			bUpdate = true;
		}
	}
	
	if (bUpdate == false)
	{
		JsonRoot.clear();
		JsonRFID.clear();
		JsonMsg.clear();
		JsonView.clear();
		JsonStatus.clear();
		JsonPower.clear();
		JsonCargo.clear();
		JsonError.clear();
		strMsg.clear();
		strColor.clear();
		strIP.clear();

		return;
	}

	if (m_pFunc && m_pFunc(m_mapStrUpdateAGVs[byNo], m_pPtr) == false)
	{
		g_mutex.lock();
		m_mapStrUpdateAGVs[byNo]->clear();
		g_mutex.unlock();
	}

	JsonRoot.clear();
	JsonRFID.clear();
	JsonMsg.clear();
	JsonView.clear();
	JsonStatus.clear();
	JsonPower.clear();
	JsonCargo.clear();
	JsonError.clear();
	strMsg.clear();
	strColor.clear();
	strIP.clear();

	return;
}

void BaseFunction::UpdateServer(TCPServer & server)
{
	if (m_pFunc == nullptr)
	{
		return;
	}

	string strIP = "";			/*!< IP��ַ */
	unsigned short usPort = 0;	/*!< �˿� */
	server.GetIPandPort(strIP, usPort);

	Json::Value JsonRoot;
	JsonRoot["Type"] = Json::Value("Server");
	JsonRoot["IP"] = Json::Value(strIP.c_str());
	JsonRoot["Port"] = Json::Value(StringEx::string_format("%d", usPort).c_str());
	JsonRoot["Listen"] = Json::Value(server.IsListened());

	bool bUpdate = false;
	if (m_pStrUpdateServer == nullptr)
	{
		g_mutex.lock();
		m_pStrUpdateServer = new Json::Value(JsonRoot);
		g_mutex.unlock();

		bUpdate = true;
	}
	else
	{
		if (JsonRoot != *m_pStrUpdateServer)
		{
			g_mutex.lock();
			m_pStrUpdateServer->clear();
			*m_pStrUpdateServer = JsonRoot;
			g_mutex.unlock();

			bUpdate = true;
		}
	}

	if (bUpdate == false)
	{
		strIP.clear();
		JsonRoot.clear();

		return;
	}

	if (m_pFunc && m_pFunc(m_pStrUpdateServer, m_pPtr) == false)
	{
		g_mutex.lock();
		m_pStrUpdateServer->clear();
		g_mutex.unlock();
	}

	strIP.clear();
	JsonRoot.clear();

	return;
}

#endif //_BASEFUNCTION_H