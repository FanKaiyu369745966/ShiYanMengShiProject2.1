#pragma once
#include "../Graph/CGraphicWidget.h"

#ifndef _CGRAPHICCALLER_H
#define _CGRAPHICCALLER_H
#include <string>
#include <mutex>

using namespace std;

class CGraphicCaller :
	public CGraphicWidget
{
public:
	CGraphicCaller(const unsigned char byNo, const char* strParam,const unsigned char byStation = 0);
	CGraphicCaller(const unsigned char byNo, const wchar_t* wstrParam, const unsigned char byStation = 0);
	CGraphicCaller(const unsigned char byNo, const unsigned char byStation = 0,string strParam="");
	CGraphicCaller(const unsigned char byNo, const wstring wstrParam, const unsigned char byStation = 0);
	~CGraphicCaller();

protected:
	unsigned char m_byNo;					/*!< ��� */
	unsigned char m_byStation;				/*!< �󶨹�λ��� */
	unsigned char m_byCallSignal;			/*!< �����ź� */
	unsigned char m_byCancelCallSignal;		/*!< ȡ�������ź� */
	unsigned char m_byPassSignal;			/*!< �����ź� */
	unsigned char m_byCancelPassSignal;		/*!< ȡ�������ź� */
	unsigned char m_byNGSignal;				/*!< NG�ź� */
	unsigned char m_byCancelNGSignal;		/*!< ȡ��NG�ź� */
	unsigned char m_byScreamSignal;			/*!< ��ͣ�ź� */
	bool m_bPackOutbound;					/*!< ��վУ�� */
	string m_strParam;						/*!< ���Դ���������ò�����JSON�ַ��� */

protected:
	static bool g_bInstall;
	static mutex g_mutex;
	static unsigned int g_unWidth;		/*!< �� */
	static unsigned int g_unHeight;		/*!< �� */
	static FontFamily* g_pFontFamily;	/*!< ���� */
	static Color g_colorFont;
	static Image* g_pImageCaller;
	static Image* g_pImageCallSignal;
	static Image* g_pImageCancelCallSignal;
	static Image* g_pImagePassSignal;
	static Image* g_pImageCancelPassSignal;
	static Image* g_pImageNGSignal;
	static Image* g_pImageCancelNGSignal;
	static Image* g_pImageScreamSignal;
	static Image* g_pImagePackOutbound;

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
	 * @brief ���ú�������ͼ
	 * @param const char* ��������ͼ·��
	*/
	static void SetCallerImage(const char* strImagePath);

	static void SetCallerImage(const wchar_t* wstrImagePath);

	/*!
	 * @brief ���ú����ź���ͼ
	 * @param const char* �����ź���ͼ·��
	*/
	static void SetCallSignalImage(const char* strImagePath);

	static void SetCallSignalImage(const wchar_t* wstrImagePath);

	/*!
	 * @brief ����ȡ�������ź���ͼ
	 * @param const char* ȡ�������ź���ͼ·��
	*/
	static void SetCancelCallImage(const char* strImagePath);

	static void SetCancelCallImage(const wchar_t* wstrImagePath);

	/*!
	 * @brief ���÷����ź���ͼ
	 * @param const char* �����ź���ͼ·��
	*/
	static void SetPassSignalImage(const char* strImagePath);

	static void SetPassSignalImage(const wchar_t* wstrImagePath);

	/*!
	 * @brief ����ȡ�������ź���ͼ
	 * @param const char* ȡ�������ź���ͼ·��
	*/
	static void SetCancelPassSignalImage(const char* strImagePath);

	static void SetCancelPassSignalImage(const wchar_t* wstrImagePath);

	/*!
	 * @brief ����NG�ź���ͼ
	 * @param const char* NG�ź���ͼ·��
	*/
	static void SetNGSignalImage(const char* strImagePath);

	static void SetNGSignalImage(const wchar_t* wstrImagePath);

	/*!
	 * @brief ����ȡ��NG�ź���ͼ
	 * @param const char* ȡ��NG�ź���ͼ·��
	*/
	static void SetCancelNGSignalImage(const char* strImagePath);

	static void SetCancelNGSignalImage(const wchar_t* wstrImagePath);

	/*!
	 * @brief ���ü�ͣ�ź���ͼ
	 * @param const char* ��ͣ�ź���ͼ·��
	*/
	static void SetScreamSignalImage(const char* strImagePath);

	static void SetScreamSignalImage(const wchar_t* wstrImagePath);

	/*!
	 * @brief ���ó�վУ����ͼ
	 * @param const char* ��ͣ�ź���ͼ·��
	*/
	static void SetPackOutboundImage(const char* strImagePath);

	static void SetPackOutboundImage(const wchar_t* wstrImagePath);

public:
	/*!
	 * @brief ��ȡ���
	 * @return unsigned char ���
	*/
	const unsigned char GetNo() const;

	/*!
	 * @brief ��ȡ��λ���
	 * @return unsigned char ��λ���
	*/
	const unsigned char GetStation() const;

	/*!
	 * @brief �󶨹�λ
	 * @param unsigned char ��λ���
	*/
	void BindStation(const unsigned char byStation);

	/*!
	 * @brief ���ú����ź�
	 * @param unsigned char �źſ���
	*/
	void SetCallSignal(const unsigned char bySignal);

	/*!
	 * @brief ����ȡ�������ź�
	 * @param unsigned char �źſ���
	*/
	void SetCancelCallSignal(const unsigned char bySignal);

	/*!
	 * @brief ���÷����ź�
	 * @param unsigned char �źſ���
	*/
	void SetPassSignal(const unsigned char bySignal);

	/*!
	 * @brief ����ȡ�������ź�
	 * @param unsigned char �źſ���
	*/
	void SetCancelPassSignal(const unsigned char bySignal);

	/*!
	 * @brief ����NG�ź�
	 * @param unsigned char �źſ���
	*/
	void SetNGSignal(const unsigned char bySignal);

	/*!
	 * @brief ����ȡ��NG�ź�
	 * @param unsigned char �źſ���
	*/
	void SetCancelNGSignal(const unsigned char bySignal);

	/*!
	 * @brief ���ü�ͣ�ź�
	 * @param unsigned char �źſ���
	*/
	void SetScreamSignal(const unsigned char bySignal);

	/*!
	 * @brief ���ó�վУ��
	 * @param bool ��վ����
	*/
	void SetPackOutboundSignal(const bool bOutbound);

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
	 * @brief ��ͼ
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

#endif //!_CGRAPHICCALLER_H