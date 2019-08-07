/*!
 * @file CGraphicAGV.h
 * @brief CGraphicAGV��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-10-15
 * @version 2.0
*/

#pragma once
#include "CGraphicWidget.h"

#ifndef _CGRAPHICAGV_H
#define _CGRAPHICAGV_H

#include <string>
#include <mutex>

using namespace std;

enum _GraphicAGVActionStatus
{
	_GRAPHICAGVACTSTA_NONE,
	_GRAPHICAGVACTSTA_LOAD,
	_GRAPHICAGVACTSTA_UNLOAD,
};

/*!
 * @class CGraphicAGV
 * @brief AGVͼ��
 *
 * �̳л���ͼ��,����AGVͼ���Լ�����AGV״̬����AGVͼ�εȲ���
*/
class CGraphicAGV :
	public CGraphicWidget
{
public:
	CGraphicAGV(const unsigned char byNo,const char* strParam);
	CGraphicAGV(const unsigned char byNo, const wchar_t* wstrParam);
	CGraphicAGV(const unsigned char byNo, string strParam = "");
	CGraphicAGV(const unsigned char byNo, const wstring wstrParam);
	~CGraphicAGV();

protected:
	unsigned char m_byNo;				/*!< ��� */
	bool m_bRun;						/*!< �ƶ���ʶ */
	bool m_bUnVol;						/*!< ���������ʶ */
	bool m_bError;						/*!< �쳣��ʶ */
	bool m_bObs;						/*!< ���ϱ�ʶ */
	bool m_bCargo;						/*!< �ػ�״̬ */
	unsigned char m_byAction;			/*!< ����״̬ 0-�޶���/��� 1-������ 2-������ */
	string m_strParam;					/*!< ���Դ���������ò�����JSON�ַ��� */

protected:
	bool m_bShow;						/*!< ��ʾ��ʶ�� */

protected:
	static bool g_bInstall;
	static mutex g_mutex;
	static unsigned int g_unWidth;		/*!< �� */
	static unsigned int g_unHeight;		/*!< �� */
	static FontFamily *g_pFontFamily;	/*!< ���� */
	static Color g_colorFont;
	static Image* g_pImageAGV;
	static Image* g_pImageRun;
	static Image* g_pImageUnvol;
	static Image* g_pImageObs;
	static Image* g_pImageError;
	static Image* g_pImageCargo;
	static Image* g_pImageLoad;
	static Image* g_pImageUnload;

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
	 * @brief ����AGV��ͼ
	 * @param const char* AGV��ͼ·��
	*/
	static void SetAGVImage(const char* strAGV);

	static void SetAGVImage(const wchar_t* wstrAGV);

	/*!
	 * @brief �����ƶ���ͼ
	 * @param const char* �ƶ���ͼ·��
	*/
	static void SetRunImage(const char* strRun);

	static void SetRunImage(const wchar_t* wstrRun);

	/*!
	 * @brief ���õ���������ͼ
	 * @param const char* ����������ͼ·��
	*/
	static void SetUnVolImage(const char* strUnVol);

	static void SetUnVolImage(const wchar_t* wstrUnVol);

	/*!
	 * @brief �����쳣��ͼ
	 * @param const char* �쳣��ͼ·��
	*/
	static void SetErrorImage(const char* strError);

	static void SetErrorImage(const wchar_t* wstrError);

	/*!
	 * @brief ���ñ�����ͼ
	 * @param const char* ������ͼ·��
	*/
	static void SetObsImage(const char* strObs);

	static void SetObsImage(const wchar_t* wstrObs);

	/*!
	 * @brief ����������ͼ
	 * @param const char* ������ͼ·��
	*/
	static void SetLoadImage(const char* strLoad);

	static void SetLoadImage(const wchar_t* wstrLoad);

	/*!
	 * @brief ����������ͼ
	 * @param const char* ������ͼ·��
	*/
	static void SetUnloadImage(const char* strUnload);

	static void SetUnloadImage(const wchar_t* wstrUnload);

	/*!
	 * @brief �����ػ���ͼ
	 * @param const char* �ػ���ͼ·��
	*/
	static void SetCargoImage(const char* strCargo);

	static void SetCargoImage(const wchar_t* wstrCargo);
public:
	/*!
	 * @brief ��ȡ���
	 * @return unsigned int ���
	*/
	const unsigned char GetNo() const;

	/*!
	 * @brief �����ƶ�״̬
	 * @param bool �ƶ���ʶ 
	*/
	void SetRunStatus(const bool bRun);

	/*!
	 * @brief ���õ�������״̬
	 * @param bool ���������ʶ
	*/
	void SetUnVolStatus(const bool bUnVol);

	/*!
	 * @brief �����쳣״̬
	 * @param bool �쳣��ʶ
	*/
	void SetErrorStatus(const bool bError);

	/*!
	 * @brief ���ñ���״̬
	 * @param bool ���ϱ�ʶ
	*/
	void SetObsStatus(const bool bObs);

	/*!
	 * @brief �����ػ�״̬
	 * @param bool �ػ���ʶ
	*/
	void SetCargoStatus(const bool bCargo);

	/*!
	 * @brief ���ö���״̬
	 * @param unsigned char ����״̬
	*/
	void SetActionStatus(const unsigned char byAction);

	/*!
	 * @brief �Ƿ���ʾ
	 * @param bShow trueΪ��ʾ falseΪ����
	*/
	void IsShow(const bool bShow);

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

#endif //!_CGRAPHICAGV_H