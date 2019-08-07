/*!
 * @file CGraphicMark.h
 * @brief CGraphicMark��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-10-22
 * @version 2.0
*/

#pragma once
#include "CGraphicWidget.h"

#ifndef _CGRAPHICRFID_H
#define _CGRAPHICRFID_H

#include <mutex>

using namespace std;

/*!
 * @class CGraphicMark
 * @brief �ر꿨��
 *
 * �̳л���ͼ��,���Ƶر꿨ͼ���Լ���ק�Ȳ���
*/
class CGraphicRFID :
	public CGraphicWidget
{
public:
	CGraphicRFID(unsigned short usNo,PointF ptCenter = PointF(0,0));
	~CGraphicRFID();

protected:
	unsigned short m_usNo;	/*!< ��� */

protected:
	static bool g_bInstall;
	static mutex g_mutex;
	static unsigned int g_unWidth;	/*!< �� */
	static unsigned int g_unHeight;	/*!< �� */
	static FontFamily* g_pFontFamily;	/*!< ���� */
	static Color g_colorFont;

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

public:
	/*!
	 * @brief ��ȡ���
	 * @return unsigned short ���
	*/
	const unsigned short GetNo() const;

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

#endif //!_CGRAPHICRFID_H