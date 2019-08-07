/*!
* @file ADOConn.h
* @brief interface for the ADOConn class.
* @author 
* @date 
* @version 2.0
*/

#if !defined(AFX_ADOCONN_H__5FB9A9B2_8D94_44F7_A2DA_1F37A4F33D10__INCLUDED_)
#define AFX_ADOCONN_H__5FB9A9B2_8D94_44F7_A2DA_1F37A4F33D10__INCLUDED_

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace\
	rename("EOF", "adoEOF")rename("BOF", "adoBOF")

#if _MSC_VER > 1000

#pragma once
#include <Windows.h>
#include <string>
#include <mutex>

using namespace std;
#endif // _MSC_VER > 1000

/*!
* @class ADOConn  
* @brief ���ݿ�ADO����
*
* ���ݿ�ADO������,������ѯ���޸����ݿ�Ȳ���
*/
class ADOConn  
{
public:
	/*!
	* @brief �޲ι���
	*/
	ADOConn();

	ADOConn(_bstr_t bstrConnect, _bstr_t bstrUser, _bstr_t bstrPassword);

	/*!
	* @brief ��������
	* @param cAdo ������ADO����
	*/
	ADOConn(const ADOConn& cAdo);

	/*!
	* @brief ��������
	*/
	~ADOConn();

protected:
	_ConnectionPtr m_pConnection; /*!< ���Ӷ���ָ��*/
	_RecordsetPtr m_pRecordset; /*!< �����ָ��*/

protected:
	static bool m_bInstall;

public:
	_bstr_t m_bstrConnect; /*!< ���Ӵ� */
	_bstr_t m_bstrUser;		/*!< �û��� */
	_bstr_t m_bstrPassword; /*!< �û����� */

public:
	/*!
	* @brief ��ʼ��ADO����
	* @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool OnInitADOConn();

	bool OnInitADOConn(_bstr_t bstrConnect, _bstr_t bstrUser, _bstr_t bstrPassword);

	/*!
	* @brief ��ѯ���ݿ�
	* @param _bstr_t SQL���
	* @param _RecordsetPtr ���ز�ѯ�����
	* @return bool �ɹ�����true,���򷵻�false
	*/
	bool GetRecordSet(_bstr_t bstrSQL, _RecordsetPtr& result);

	/*!
	* @brief ִ��SQL���
	* @param _bstr_t SQL���
	* @return bool �ɹ�����true,ʧ�ܷ���false
	*/
    BOOL ExecuteSQL(_bstr_t bstrSQL , VARIANT& rs);

	/*!
	* @brief �ر�ADO����
	*/
	void ExitConnect();

public:
	/*!
	* @brief ����=����
	* @param cAdo ������ADO����
	*/
	void operator=(const ADOConn& cAdo);
};

#endif // !defined(AFX_ADOCONN_H__5FB9A9B2_8D94_44F7_A2DA_1F37A4F33D10__INCLUDED_)
