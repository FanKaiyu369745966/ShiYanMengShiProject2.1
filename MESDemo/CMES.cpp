#include "pch.h"
#include "CMES.h"

#ifdef _MES_H

#include "../String/SystemLog.h"

bool CMES::g_bInstall = false;

CMES::CMES(_bstr_t bstrProductLineNo)
{
	m_bstrProductLineNo = bstrProductLineNo;

	Install();

	HRESULT hs = m_client.CreateInstance(__uuidof(Kel_IFactory_Mqtt_Client::IFactoryMqttClient));
}


CMES::~CMES()
{
	if (m_client == NULL)
	{
		return;
	}

	m_client.Release();
}

void CMES::Install()
{
	if (g_bInstall == false)
	{
		CoFreeUnusedLibraries();
		HRESULT hs = CoInitialize(NULL);
		
		switch (hs)
		{
		case S_OK:
		{
			// ���سɹ�
			DebugPrint::Print("MES��̬dll���سɹ�.");
			break;
		}
		case S_FALSE:
		{
			// �Ѿ�����
			DebugPrint::Print("MES��̬dll�Ѽ���.");
			break;
		}
		}

		g_bInstall = true;
	}

	return;
}

void CMES::Uninstall()
{
	if (g_bInstall)
	{
		CoUninitialize();

		g_bInstall = false;
	}

	return;
}

bool CMES::IsOpen() const
{
	if (m_client == NULL)
	{
		return false;
	}

	VARIANT_BOOL vbWaiting = VARIANT_TRUE;
	m_client->get_IsOpen(&vbWaiting);

	if (vbWaiting == VARIANT_TRUE)
	{
		return true;
	}

	return false;
}

void CMES::OnReceiveControlCmd()
{
}

void CMES::OnReceiveResult()
{
}

void CMES::OnReceiveMsg()
{
}

void CMES::Close()
{
	if (m_client == NULL)
	{
		return;
	}

	HRESULT hs = m_client->Close();

	if (hs == SDK_ERROR_CODE_CLOSE)
	{
		return;
	}

	// ��¼����������Լ�������Ϣ
	_bstr_t bstrError = GetErrorMsg(hs, nullptr);

	DebugPrint::Printf("MES�ر�ʧ��.ԭ��:%s", (char*)bstrError);

	return;
}

bool CMES::Connect(_bstr_t bstrClientID)
{
	Kel_IFactory_Mqtt_Client::_BaseResult *tt = NULL;
	HRESULT hs = m_client->Connect(bstrClientID, &tt);

	if (hs == MES_ERROR_CODE_SUCCESS || SDK_ERROR_CODE_OPEN)
	{
		return true;
	}

	// ��¼����������Լ�������Ϣ
	_bstr_t bstrError = GetErrorMsg(hs, tt);

	DebugPrint::Printf("MES����ʧ��.ԭ��:%s", (char*)bstrError);

	tt->Release();

	return false;
}

bool CMES::RequestWithResult(_bstr_t bstrIri, _bstr_t bstrAction, _bstr_t bstrJsonData, bool bWaiting, _bstr_t & bstrResult)
{
	Kel_IFactory_Mqtt_Client::_RequestResultPair *result = NULL;
	Kel_IFactory_Mqtt_Client::_BaseResult *rstVal = NULL;	/*!< ���յķ��ؽ�� JSON���� */

	VARIANT_BOOL vbWaiting = VARIANT_TRUE;
	if (bWaiting == false)
	{
		vbWaiting = VARIANT_FALSE;
	}

	HRESULT hs = m_client->RequstWithResult(bstrIri, bstrAction, bstrJsonData, vbWaiting, &result, &rstVal);

	if (hs == MES_ERROR_CODE_SUCCESS)
	{
		return true;
	}

	_bstr_t bstrError = GetErrorMsg(hs, rstVal);

	DebugPrint::Printf("����MES�ӿڵ���ʧ��.ԭ��:%s", (char*)bstrError);
	SystemLog::Recordf("����MES�ӿڵ���ʧ��.ԭ��:%s", (char*)bstrError);
		
	rstVal->Release();
	result->Release();

	return false;
}

bool CMES::PackOutBound(_bstr_t bstrWPNo)
{
	_bstr_t bstrIri = "process.control";		/*!< �ӿ���Դ��ʶ(URI)*/
	_bstr_t bstrAction = "outbound";			/*!< ������� */
	_bstr_t bstrJsonData = "{\"WST_NO\":\"";	/*!< ������������ݶ��� */
	bstrJsonData += bstrWPNo;
	bstrJsonData += "\",\"ProductLineNO\":\"";
	// PACK-Line-1
	bstrJsonData += m_bstrProductLineNo;
	bstrJsonData += "\"}";
	_bstr_t bstrResult = "";					/*!< �ӿڷ��ؽ�� */

	if (RequestWithResult(bstrIri, bstrAction, bstrJsonData, true, bstrResult) == false)
	{
		DebugPrint::Printf("��վ:%sδͨ��MES��վУ��", (char*)bstrWPNo);
		return false;
	}

	return true;
}

_bstr_t CMES::GetErrorMsg(HRESULT erroCode, _BaseResult* pResult)
{
	_bstr_t bstrError = "";		/*!< ������Ϣ*/
	
	switch (erroCode)
	{
	case MES_ERROR_CODE_SUCCESS:
	{
		bstrError = "�����ɹ�";
		break;
	}
	case MES_ERROR_CODE_SERVER:
	{
		break;
	}
	case MES_ERROR_CODE_ACTION:
	{
		break;
	}
	case MES_ERROR_CODE_ACTPARAM:
	{
		break;
	}
	case MES_ERROR_CODE_LINKPARAM:
	{
		break;
	}
	case MES_ERROR_CODE_OVERTIME:
	{
		bstrError = "�ӿ�����ʱ";
		break;
	}
	case MES_ERROR_CODE_UNKNOWN:
	{
		break;
	}
	case MES_ERROR_CODE_CHECK:
	{
		break;
	}
	case MES_ERROR_CODE_UPDATEDATA:
	{
		break;
	}
	case MES_ERROR_CODE_INSERTDATA:
	{
		break;
	}
	case MES_ERROR_CODE_CTRLDATA:
	{
		break;
	}
	case MES_ERROR_CODE_CHECKPARAM:
	{
		break;
	}
	case MES_ERROR_CODE_PARAM:
	{
		break;
	}
	case MES_ERROR_CODE_SAVE:
	{
		bstrError = "����ʧ��";
		break;
	}
	case MES_ERROR_CODE_UPDATE:
	{
		bstrError = "����ʧ��";
		break;
	}
	case MES_ERROR_CODE_DELETE:
	{
		bstrError = "ɾ��ʧ��";
		break;
	}
	case MES_ERROR_CODE_INSERT:
	{
		bstrError = "����ʧ��";
		break;
	}
	case MES_ERROR_CODE_PATH:
	{
		break;
	}
	case MES_ERROR_CODE_LINK:
	{
		break;
	}
	case SDK_ERROR_CODE_SYSTEM:
	{
		bstrError = "ϵͳ����";
		break;
	}
	case SDK_ERROR_CODE_BUSY:
	{
		bstrError = "ϵͳ��æ";
		break;
	}
	case SDK_ERROR_CODE_OPEN:
	{
		bstrError = "�ͻ��������Ѵ�";
		break;
	}
	case SDK_ERROR_CODE_CLOSE:
	{
		bstrError = "�ͻ��������ѹر�";
		break;
	}
	case SDK_ERROR_CODE_SERVER:
	{
		break;
	}
	case PACK_ERROR_CODE_UNFINISH:
	{
		break;
	}
	case PACK_ERROR_CODE_CHECK:
	{
		break;
	}
	}

	return bstrError;
}

#endif //_MES_H