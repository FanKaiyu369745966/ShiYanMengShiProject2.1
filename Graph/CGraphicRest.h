/*!
 * @file CGraphicRest.h
 * @brief CGraphicRest��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-10-22
 * @version 2.0
*/

#pragma once
#include "CGraphicWidget.h"

#ifndef _CGRAPHICREST_H
#define _CGRAPHICREST_H

#include <string>
#include <mutex>

using namespace std;

class CGraphicRest :
	public CGraphicWidget
{
public:
	CGraphicRest(const unsigned char byNo, const unsigned short usLocation,const char* strName,const char* strParam);
	CGraphicRest(const unsigned char byNo, const unsigned short usLocation, const wchar_t* wstrName,const wchar_t* wstrParam);
	CGraphicRest(const unsigned char byNo, const unsigned short usLocation, string strName = "", string strParam = "");
	CGraphicRest(const unsigned char byNo, const unsigned short usLocation, const wstring wstrName,const wstring wstrParam = L"");
	~CGraphicRest();

protected:
	unsigned char m_byNo;			/*!< ��� */
	unsigned short m_usLocation;	/*!< ���� */
	string m_strName;				/*!< ���� */ 
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
	 * @brief ���õر꿨
	 * @param usMark �ر꿨
	*/
	void SetLocation(const unsigned short usLocation);

	/*!
	 * @brief ��ȡ�ر꿨
	 * @return unsigned short �ر꿨
	*/
	const unsigned short GetLocation() const;

	/*!
	 * @brief ��������
	 * @param CString ����
	*/
	void SetName(const char* strName);
	void SetName(const wchar_t* wstrName);

	/*!
	 * @brief ��ȡ����
	 * @return CString ����
	*/
	const string GetName() const;
	const wstring GetNameW() const;

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
};

#endif // !_CGRAPHICREST_H