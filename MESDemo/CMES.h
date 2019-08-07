/*!
 * @file CMES.h
 * @brief ����MES�ӿڻ��������Լ����ܵ��ļ�
 * @author FanKaiyu
 * @date 2019-03-18
 * @version 1.0
*/

#pragma once
#ifndef _MES_H
#define _MES_H

#include <Windows.h>
#include <string>
#include "../String/DebugPrint.h"

using namespace std;

#import "../MES_SDK/Kel.IFactory.Mqtt.Client.tlb" named_guids raw_interfaces_only
using namespace Kel_IFactory_Mqtt_Client;

/*!< @brief MES����������� */
enum MES_ERROR_CODE
{
	MES_ERROR_CODE_SUCCESS = 0,			/*!< �����ɹ� */
	MES_ERROR_CODE_SERVER = 100,		/*!< �������쳣:{�쳣��Ϣ} */
	MES_ERROR_CODE_ACTION,				/*!< δ�ҵ�ƥ��Ľӿ�:{routingKey}#{action} */
	MES_ERROR_CODE_ACTPARAM,			/*!< δ�ҵ��ӿ�{routingKey}#{action}�Ĳ���������Ϣ */
	MES_ERROR_CODE_LINKPARAM = 105,		/*!< ����Ŀ¼���Ӳ�������ȷ:{requestJsonData} */
	MES_ERROR_CODE_OVERTIME = 110,		/*!< �ӿ�����ʱ */
	MES_ERROR_CODE_UNKNOWN = 1000,		/*!< ����δ֪�쳣:{�쳣��Ϣ} */
	MES_ERROR_CODE_CHECK = 1005,		/*!< ��������У��ʧ��:{У����Ϣ};У�����:craft_rule */
	MES_ERROR_CODE_UPDATEDATA,			/*!< ��������ʧ��:δ�ҵ���Ʒ����{ProductBarCode}��{���ձ��}���յ��������� */
	MES_ERROR_CODE_INSERTDATA,			/*!< ��������ʧ��:{������Ϣ} */
	MES_ERROR_CODE_CTRLDATA,			/*!< ���ݲ����쳣:{�쳣��Ϣ} */
	MES_ERROR_CODE_CHECKPARAM,			/*!< ��������У��ʧ��:����{0}����Ϊ���ҳ��Ȳ�����{1}���ַ�;У�����:craft_metadata */
	MES_ERROR_CODE_PARAM = 1002,		/*!< �����������:����{0}����Ϊ�������ݳ��Ȳ��ܳ���{1}���ַ�;������:mq_interface_metadata */
	MES_ERROR_CODE_SAVE = 1010,			/*!< ����ʧ�� */
	MES_ERROR_CODE_UPDATE,				/*!< ����ʧ�� */
	MES_ERROR_CODE_DELETE,				/*!< ɾ��ʧ�� */
	MES_ERROR_CODE_INSERT,				/*!< ����ʧ�� */
	MES_ERROR_CODE_PATH = 1102,			/*!< ·������:������Ϣ */
	MES_ERROR_CODE_LINK,				/*!< ���ӹ���Ŀ¼����:������Ϣ */
};

/*!< SDK�ͻ������Ӵ������ */
enum SDK_ERROR_CODE
{
	SDK_ERROR_CODE_SYSTEM = -2,			/*!< ϵͳ���� */
	SDK_ERROR_CODE_BUSY,				/*!< ϵͳ��æ */
	SDK_ERROR_CODE_OPEN = 111,			/*!< �ͻ��������Ѵ� */
	SDK_ERROR_CODE_CLOSE,				/*!< �ͻ��������ѹر� */
	SDK_ERROR_CODE_SERVER,				/*!< ����MQ������ʧ��:{������Ϣ} */
	SDK_ERROR_CODE_UNKNOWN = 1000,		/*!< δ֪����:{�쳣��Ϣ} */
};

/*!< PACK��վУ�������� */
enum PACK_ERROR_CODE
{
	PACK_ERROR_CODE_UNFINISH = 2201,	/*!< ����װ��δ��ɣ�����{0}Ҫ����������{1},��װ������{2} */
	PACK_ERROR_CODE_CHECK,				/*!< ���չ���У��ʧ��:{ResultMsg} */
};

/*!< @biref ��վ��Ϣ */
typedef struct MESWorkPositionStruct
{
	unsigned char m_byNo;	/*!< ��λ��� */
	_bstr_t m_bstrNo;		/*!< ��վ��� */
	_bstr_t m_bstrName;		/*!< ��λ���� */
	_bstr_t m_bstrLevel;	/*!< ���� */
	_bstr_t m_bstrAction;	/*!< ���� */
}MESWP;

/*!
 * @class CMES
 * @brief ʵ����MESϵͳ�ĶԽ�
*/
class CMES
{
public:
	CMES(_bstr_t bstrProductLineNo);
	~CMES();
protected:
	static bool g_bInstall;				/*!< dll���ر�ʶ */
	_bstr_t m_bstrProductLineNo;		/*!< ���߱�� */
	IIFactoryMqttClientPtr m_client;	/*!< MES�ӿڶ���ʵ�� */
public:
	/*!
	 * @brief ����
	*/
	static void Install();

	/*!
	 * @brief ж��
	*/
	static void Uninstall();
public:
	/*!
	 * @brief �Ƿ�������
	*/
	bool IsOpen() const;

	/*!
	 * @brief ���յ��пؿ�������
	 *
	 * �ͻ������Ӻ��ͨ�����¼������пط��͵��豸����ָ�
	 * �硰һ�������������豸ͣ������ָ�
	*/
	void OnReceiveControlCmd();

	/*!
	 * @brief ���յ����񷵻ؽ��
	 *
	 * ���ͻ��˽��յ�����˷��صĴ�����ʱ�������¼���
	 * ��ʹ���첽��ʽ����MES����ʱ������ͨ�����¼����շ��ؽ����
	*/
	void OnReceiveResult();

	/*!
	 * @brief ���յ��п���Ϣ
	 *
	 * �ͻ��˽��յ��пط��͵���Ҫ��Ϣʱ���ͣ�
	 * �����ڽ����пط�����֪ͨ��Ϣ��������Ϣ�����ݡ�
	*/
	void OnReceiveMsg();

	/*!
	 * @brief �رտͻ�������
	 *
	 * ��λ���˳�ʱ�ر�����
	*/
	void Close();

	/*!
	 * @brief ���ӵ�MES������
	 *
	 * ��λ������ʱͨ��������Ӧ�÷��������ɹ�����ܽ��в���MES�ӿڵĵ��á�
	 * ����ʱ���������λ����Ψһ��ʶ���Թ�λ�����Ϊ��ʶ��
	 * @param _bstr_t ��λ���
	 * @return bool ���ӳɹ�����true,���򷵻�false
	*/
	bool Connect(_bstr_t bstrClientID);

	/*!
	 * @brief ����MES�ӿڵ���
	 * @param _bstr_t �ӿ���Դ��ʶ(URI)�����������ܿط���ӿڵ�Ψһ��ʶ��
	 * @param _bstr_t ����������硰add/edit/query/validate/get���ȡ�
	 * @param _bstr_t ������������ݶ�����JSON��ʽ��װ
	 * @param bool �ȴ���ͬ����ʽ��Ϊtrue�������첽��ʽ��Ϊfalse����ʱ��ͨ���¼�OnReceiveResult���շ��ؽ��
	 * @param _bstr_t& �ӿڷ��ؽ����waitingΪtrueʱ���ؽӿڽ����falseʱ��resutl��msg�з����������š�
	 * @return bool ����ɹ�����true,���򷵻�false
	*/
	bool RequestWithResult(_bstr_t bstrIri, _bstr_t bstrAction, _bstr_t bstrJsonData, bool bWaiting, _bstr_t& bstrResult);

	/*!
	 * @brief Pack��վУ��
	 * @param _bstr_t ��վ���
	 * @return bool �ɹ�����true,���򷵻�false
	*/
	bool PackOutBound(_bstr_t bstrWPNo);

protected:
	/*!
	 * @brief ��ȡ�쳣��Ϣ
	 * @param HRESULT ������
	 * @param _BaseResult* ������Ϣָ��
	 * @return _bstr_t �쳣��Ϣ
	*/
	_bstr_t GetErrorMsg(HRESULT erroCode, _BaseResult* pResult);
};

#endif //!_MES_H