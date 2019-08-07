/*!
 * @file CStation.h
 * @brief ��λ�����Լ������ļ�
 * @author FanKaiyu
 * @date 2019-04-19
 * @version 2.1
*/

#pragma once
#ifndef _STATION_H
#define _STATION_H

#include <string>
#include <Windows.h>
#include "../String/StringEx.h"

using namespace std;

class CStation
{
public:
	CStation(unsigned char byNo, unsigned short usLocation, unsigned char byProcess, const char* strName,const char* strMESWP,bool bMES = true)
		:m_byNo(byNo), m_usLocation(usLocation), m_byProcess(byProcess) 
	{ 
		m_strName = strName;
		m_strMESWP = strMESWP;
		m_bMES = bMES;

		if (m_strMESWP == "")
		{
			m_bMES = false;
		}
	}

	CStation(unsigned char byNo, unsigned short usLocation, unsigned char byProcess, const wchar_t* wstrName, const wchar_t* wstrMESWP, bool bMES = true)
		:m_byNo(byNo), m_usLocation(usLocation), m_byProcess(byProcess)
	{
		m_strName = StringEx::wstring_to_string(wstrName).c_str();
		m_strMESWP = StringEx::wstring_to_string(wstrMESWP).c_str();
		m_bMES = bMES;

		if (m_strMESWP == "")
		{
			m_bMES = false;
		}
	}

	CStation(unsigned char byNo, unsigned short usLocation, unsigned char byProcess, string strName = "", string strMESWP = "", bool bMES = false)
		:m_byNo(byNo), m_usLocation(usLocation), m_byProcess(byProcess)
	{ 
		m_strName = strName;
		m_strMESWP = strMESWP;
		m_bMES = bMES;

		if (m_strMESWP == "")
		{
			m_bMES = false;
		}
	}

	CStation(unsigned char byNo, unsigned short usLocation, unsigned char byProcess, wstring wstrName, wstring wstrMESWP, bool bMES = true)
		:m_byNo(byNo), m_usLocation(usLocation), m_byProcess(byProcess)
	{
		m_strName = StringEx::wstring_to_string(wstrName).c_str();
		m_strMESWP = StringEx::wstring_to_string(wstrMESWP).c_str();
		m_bMES = bMES;

		if (m_strMESWP == "")
		{
			m_bMES = false;
		}
	}

	~CStation(){}

protected:
	unsigned char m_byNo;			/*!< ��λ��� */
	unsigned short m_usLocation;	/*!< ��λ���� */
	string m_strName;				/*!< ��λ���� */
	unsigned char m_byProcess;		/*!< ������ */
	string m_strMESWP;				/*!< MES��վ��� */
	bool m_bMES;					/*!< MES���ñ�ʶ */

public:
	/*!
	 * @brief ��ȡ���
	 * @return unsigned char ��λ���
	*/
	const unsigned char GetNo() const { return m_byNo; }

	/*!
	 * @brief ��ȡ����
	 * @return unsigned short ��λ����
	*/
	const unsigned short GetLocation() const { return m_usLocation; }

	/*!
	 * @brief ������
	 * @param unsigned short ������
	 * @return bool �󶨳ɹ�����true,���򷵻�false
	*/
	bool BindLocaion(const unsigned short usLocation)
	{
		if (m_usLocation == usLocation)
		{
			return false;
		}

		m_usLocation = usLocation;

		return true;
	}

	/*!
	 * @brief ��ȡ����
	 * @return unsigned char ����
	*/
	const unsigned char GetProcess() const { return m_byProcess; }

	/*!
	 * @brief �󶨹���
	 * @param unsigned char �¹���
	 * @return bool �󶨳ɹ�����true,���򷵻�false
	*/
	bool BindProcess(const unsigned char byProcess)
	{
		if (m_byProcess == byProcess)
		{
			return false;
		}

		m_byProcess = byProcess;

		return true;
	}

	/*!
	 * @brief ��ȡ����
	 * @return string ��λ����
	*/
	string GetName() const { return m_strName; }
	
	/*!
	 * @brief ��ȡ����
	 * @return wstring ��λ����
	*/
	wstring GetNameW()
	{
		return StringEx::string_to_wstring(m_strName);
	}

	/*!
	 * @brief ��������
	 * @param const char* ��λ����
	 * @return bool ���óɹ�����true,���򷵻�false
	*/
	bool SetName(const char* strName)
	{
		if (strcmp(m_strName.c_str(), strName) == 0)
		{
			return false;
		}

		m_strName = strName;

		return true;
	}

	bool SetName(const wchar_t* wstrName)
	{
		return SetName(StringEx::wstring_to_string(wstrName).c_str());
	}
	bool SetName(string strName){ return SetName(strName.c_str()); }
	bool SetName(wstring wstrName) { return SetName(wstrName.c_str()); }

	void SetEnableMES(const bool bMES) { m_bMES = bMES; }
	const bool EnbaleMES() const { return m_bMES; }

	bool SetMESWP(const char* str)
	{
		if (strcmp(m_strName.c_str(), str) == 0)
		{
			return false;
		}

		m_strMESWP = str;

		return true;
	}

	bool SetMESWP(const wchar_t* wstr)
	{
		return SetName(StringEx::wstring_to_string(wstr).c_str());
	}
	bool SetMESWP(string str) { return SetName(str.c_str()); }
	bool SetMESWP(wstring wstr) { return SetName(wstr.c_str()); }

	string GetMESWP() const { return m_strMESWP; }

	wstring GetMESWPW()
	{
		return StringEx::string_to_wstring(m_strMESWP);
	}
};

#endif //_STATION_H