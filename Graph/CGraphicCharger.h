/*!
 * @file CGraphicCharger.h
 * @brief CGraphicCharger��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-10-22
 * @version 2.0
*/

#pragma once
#include "CGraphicWidget.h"
#include <mutex>

using namespace std;

#ifndef _CGRAPHICCHARGER_H
#define _CGRAPHICCHARGER_H

/*!
 * @class CGraphicCharger
 * @brief �����ͼ��
 *
 * �̳л���ͼ��,���Ƴ����ͼ���Լ���ק�Ȳ���
*/
class CGraphicCharger :
	public CGraphicWidget
{
public:
	CGraphicCharger(const unsigned char byNo, const unsigned short usLocation, const unsigned char byCtrler,const char* strName,const char* strParam);
	CGraphicCharger(const unsigned char byNo, const unsigned short usLocation, const unsigned char byCtrler, const wchar_t* wstrName,const wchar_t* wstrParam);
	CGraphicCharger(const unsigned char byNo, const unsigned short usLocation, const unsigned char byCtrler, string strName = "",string strParam = "");
	CGraphicCharger(const unsigned char byNo, const unsigned short usLocation, const unsigned char byCtrler, const wstring wstrName,const wstring wstrParam = L"");
	~CGraphicCharger();

protected:
	unsigned char m_byNo;			/*!< ��� */
	unsigned short m_usLocation;	/*!< ���� */
	unsigned char m_byCtrler;		/*!< ������ */
	string m_strName;				/*!< ���� */
	string m_strParam;				/*!< ���Դ���������ò�����JSON�ַ��� */

protected:
	bool m_bPower;					/*!< ͨ��״̬ */
	ULL m_ullChargeTime;			/*!< ��翪ʼʱ�� */

protected:
	static bool g_bInstall;
	static mutex g_mutex;
	static unsigned int g_unWidth;	/*!< �� */
	static unsigned int g_unHeight;	/*!< �� */
	static FontFamily* g_pFontFamily;	/*!< ���� */
	static Color g_colorFont;
	static Image* g_pImageBK;
	static Image* g_pImagePower;

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

	/*!
	 * @brief ���ó����ͼ
	 * @param const char* �����ͼ·��
	*/
	static void SetPowerImage(const char* strPower);

	static void SetPowerImage(const wchar_t* wstrPower);
public:
	/*!
	 * @brief ��ȡ���
	 * @return unsigned char ���
	*/
	const unsigned char GetNo() const;

	/*!
	 * @brief ��������
	 * @param const char* ����
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
	 * @brief ���ó��״̬
	 * @param bPower trueΪ���,falseΪ�ϵ�
	*/
	void SetPower(const bool bPower, const ULL ullChargeTime);

	/*!
	 * @brief ���ÿ�����
	 * @param unsigned char ���������
	*/
	void SetCtrler(const unsigned char byCtrler);

	/*!
	 * @brief ��ȡ������
	 * @return unsigned char ���������
	*/
	const unsigned char GetCtrler() const;

	/*!
	 * @brief ���õر꿨
	 * @param unsigned short �ر꿨
	*/
	void SetLocation(const unsigned short usLocation);

	/*!
	 * @brief ��ȡ�ر꿨
	 * @return unsigned short �ر꿨
	*/
	const unsigned short GetLocation() const;

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

#endif //!_CGRAPHICCHARGER_H