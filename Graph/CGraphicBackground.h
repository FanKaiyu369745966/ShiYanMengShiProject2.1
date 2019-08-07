/*!
 * @file CGraphicBackground.h
 * @brief CGraphicBackground��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-10-15
 * @version 2.0
*/

#pragma once
#include "CGraphicWidget.h"
#include <mutex>

using namespace std;

#ifndef _CGRAPHICBACKGROUND_H
#define _CGRAPHICBACKGROUND_H

/*!
 * @class CGraph
 * @brief ����ͼ��
 *
 * �̳л���ͼ��,���Ʊ���ͼ���������Լ���ק�����ŵȲ���
*/
class CGraphicBackground :
	public CGraphicWidget
{
public:
	CGraphicBackground(Color colorLine = Color::Black,float fWidth = 2.0f);
	CGraphicBackground(const COLORREF colorLine, const float fWidth);
	~CGraphicBackground();

protected:
	Image *m_pImage;		/*!< ����ͼ */
	float m_fWidth;			/*!< �����߿�� */

protected:
	static bool g_bInstall;
	static mutex g_mutex;
	static Color g_gcrFont;
	static FontFamily* g_pFontFamily;	/*!< ���� */

public:
	static bool Install();

	static void SetFontType(const char* strFontType);

	static void SetFontType(const wchar_t* wstrFontType);

	static void Uninstall();
public:
	/*!
	 * @brief ���ø�����
	 * @param colorLine ��������ɫ
	 * @param fWidth �����߿��
	*/
	void SetLine(const Color colorLine, const float fWidth);
	void SetLine(const COLORREF colorLine, const float fWidth);

	/*!
	 * @brief ��ȡ������
	 * @param colorLine ���ظ�������ɫ
	 * @param fWidth ���ظ����߿��
	*/
	void GetLine(Color &colorLine, float &fWidth);
	void GetLine(COLORREF& colorLine, float& fWidth);

	/*!
	 * @brief ���ñ���ͼ
	 * @param strPath ����ͼ·��
	*/
	void SetImage(const char* strPath);
	void SetImage(const wchar_t* strPath);

	/*!
	 * @brief ����
	 *
	 * Ĭ�ϷŴ�
	 * @param bool ��С��ʶ
	*/
	void Scale(bool bShrink = false);

	/*!
	 * @brief ��λ
	*/
	void Reset();

	/*!
	 * @brief ��ק
	 * @param  ptWinpoint ��������
	 * @return HCURSOR ���ָ����״
	*/
	const HCURSOR Drag(const PointF ptWinPoint);
#ifdef _AFX
	const HCURSOR Drag(const CPoint ptWinPoint);
#endif //_AFX

public:
	void DrawImage(Graphics* graphic);

	/*!
	 * @brief ���Ʊ���ͼ
	 * @param pDC ��ͼ�����
	*/
	void DrawBackground(HDC hdc);
	void DrawBackground(Graphics* graphic);

	/*!
	 * @brief ���Ƹ�����
	 * @param pDC ��ͼ�����
	*/
	void DrawLine(HDC hdc);
	void DrawLine(Graphics* graphic);

	/*!
	 * @brief ��������
	 * @param pDC ��ͼ�����
	*/
	void DrawPoint(HDC hdc);
	void DrawPoint(Graphics* graphic);
};

#endif // !_CGRAPHICBACKGROUND_H