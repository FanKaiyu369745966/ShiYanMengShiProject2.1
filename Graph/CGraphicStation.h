/*!
 * @file CGraphicStation.h
 * @brief CGraphicStation��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-10-22
 * @version 2.0
*/

#pragma once
#include "CGraphicWidget.h"
#include <mutex>

using namespace std;

#ifndef _CGRAPHICSTATION_H
#define _CGRAPHICSTATION_H

/*!
 * @class CGraphicStation
 * @brief ��λͼ��
 *
 * �̳л���ͼ��,���ƹ�ͼ�Լ���ק�Ȳ���
*/
class CGraphicStation :
	public CGraphicWidget
{
public:
	CGraphicStation(const unsigned char byNo, unsigned short usLocation,const char* strName,const char* strParam);
	CGraphicStation(const unsigned char byNo, unsigned short usLocation, const wchar_t* wstrName,const wchar_t* wstrParam);
	CGraphicStation(const unsigned char byNo, unsigned short usLocation, string strName = "",string strParam = "");
	CGraphicStation(const unsigned char byNo, unsigned short usLocation, const wstring wstrName,const wstring wstrParam = L"");
	~CGraphicStation();

protected:
	unsigned char m_byNo;			/*!< ��� */
	string m_strName;				/*!< ���� */
	unsigned short m_usLocation;	/*!< RFID�� */
	string m_strParam;				/*!< ���Դ���������ò�����JSON�ַ��� */

protected:
	static bool g_bInstall;
	static mutex g_mutex;
	static unsigned int g_unWidth;	/*!< �� */
	static unsigned int g_unHeight;	/*!< �� */
	static FontFamily* g_pFontFamily;	/*!< ���� */
	static Color g_colorFont;
	static Image* g_pImageBK;

public:
	static bool Install();

	static void SetWidth(const unsigned int unWidth);

	static void SetHeight(const unsigned int unHeight);

	static void SetFontType(const char* strFontType);

	static void SetFontType(const wchar_t* wstrFontType);

	static void SetFontColor(const Color color);

	static void SetFontColr(const COLORREF color);

	static void Uninstall();

	static const unsigned int GetWidth();

	static const unsigned int GetHeight();

	static const float GetReallyWidth();

	static const float GetReallyHeight();

	/*!
	 * @brief ���ñ�����ͼ
	 * @param const char* ������ͼ·��
	*/
	static void SetBKImage(const char* strBK);

	static void SetBKImage(const wchar_t* wstrBK);

public:
	/*!
	 * @brief ��ȡ���
	 * @return unsigned char ���
	*/
	const unsigned char GetNo() const;

	/*!
	 * @brief ��������
	 * @param CString ����
	*/
	void SetName(const char* strName);
	void SetName(const wchar_t* strName);

	/*!
	 * @brief ��ȡ����
	 * @return CString ����
	*/
	const string GetName() const;
	const wstring GetNameW() const;

	/*!
	 * @brief ���õر꿨
	 * @param unsigned shor �ر꿨
	*/
	void SetLocation(const unsigned short usLocation);

	/*!
	 * @brief ��ȡ�ر꿨
	 * @return unsigned short �ر꿨
	*/
	const unsigned short GetLocation() const;

	/*!
	 * @brief ������������
	 * @param CString ����
	*/
	void SetParam(const char* strParam);
	void SetParam(const wchar_t* strParam);

	/*!
	 * @brief ��ȡ��������
	 * @return CString ����
	*/
	const string GetParam() const;
	const wstring GetParamW() const;

	/*!
	 * @brief ���������Ƿ���������
	 * @param ptWinpoint ��������
	 * @return �������ڷ���true,���ڷ���false
	*/
	const bool IsInside(const PointF ptWinpoint);
#ifdef _AFX
	const bool IsInside(const CPoint ptWinpoint);
#endif //_AFX

	/*!
	 * @brief ����
	 * @param pDC ���ھ��
	*/
	void DrawImage(Graphics* graphic);
};

#endif //!_CGRAPHICSTATION_H