/*!
 * @file CGraph.h
 * @brief CGraph��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-10-16
 * @version 2.0
*/
#pragma once
//#include <afx.h>
#include <Windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

#ifndef _CGRAPH_H
#define _CGRAPH_H

#ifdef _WIN64
#define ULL unsigned long long
#define GetTickCountEX() GetTickCount64()
#else
#define ULL unsigned long
#define GetTickCountEX() GetTickCount()
#endif

#define MAX_SCALE 10.0f
#define MIN_SCALE 0.4f
#define DEFALUT_SCALE 10.0f

static const PointF PointToPointF(const Point pt)
{
	return PointF((REAL)pt.X, (REAL)pt.Y);
}

static const Point PointFToPoint(const PointF pt)
{
	return Point((INT)pt.X, (INT)pt.Y);
}

static const RectF RectToRectF(const Rect rc)
{
	return RectF((REAL)rc.X, (REAL)rc.Y, (REAL)rc.Width, (REAL)rc.Height);
}

static const Rect RectFToRect(const RectF rc)
{
	return Rect((INT)rc.X, (INT)rc.Y, (INT)rc.Width, (INT)rc.Height);
}

#ifdef _AFX
static const Point CPointToPoint(const CPoint pt)
{
	return Point(pt.x, pt.y);
}

static const PointF CPointToPointF(const CPoint pt)
{
	return PointToPointF(CPointToPoint(pt));;
}

static const CPoint PointToCPoint(const Point pt)
{
	return CPoint(pt.X, pt.Y);
}

static const CPoint PointToCPoint(const PointF pt)
{
	return PointToCPoint(PointFToPoint(pt));;
}

static const Rect CRectToRect(const CRect rc)
{
	return Rect(rc.left, rc.top,rc.Width(), rc.Height());
}

static const RectF CRectToRectF(const CRect rc)
{
	return RectToRectF(CRectToRect(rc));
}

static const CRect RectToCRect(const Rect rc)
{
	return CRect(rc.X, rc.Y, rc.GetRight(), rc.GetBottom());
}

static const CRect RectToCRect(const RectF rc)
{
	return RectToCRect(RectFToRect(rc));;
}
#endif //_AFX

/*!
 * @class CGraph
 * @brief ����ͼ��
 *
 * ��ͼ�Ļ�������,����������ͼ����̳�
 * 10�������10cm
 * ��ͼ���ߴ�Ϊ500*500m
 * ��50000 * 50000
*/
class CGraph
{
public:
	CGraph();
	~CGraph();

protected:
	GdiplusStartupInput m_gdiplusInput;		/*!< GDI+��ʼ�������� */
	GdiplusStartupOutput m_gdiplusOutput;	/*!< GDI+��ʼ�������� */
	ULONG_PTR m_ulGdiplusToken;

protected:
	static PointF g_ptOffset;			/*!< ƫ���� */
	static float g_fScale;				/*!< ���ű��� */
	static unsigned int g_unDistance;	/*!< ������ */
	static Size g_sizeVector;			/*!< ʸ��ͼ�ߴ� */
	static RectF g_rcClient;			/*!< �ͻ����ߴ� */
	static bool g_bFlash;				/*!< ��˸��ʶ */
	static ULL g_ullFlashTime;			/*!< ��˸��ʼʱ�� */
	static bool g_bRefresh;
	static bool g_bEdit;
	
public:
	/*!
	 * @brief ���ÿͻ����ߴ�
	 * @param rcClient �ͻ����ߴ�
	*/
	static void SetClientRect(const RectF rcClient);
#ifdef _AFX
	static void SetClientRect(const CRect rcClient);
#endif //_AFX

	/*!
	 * @brief ��ȡ�ͻ����ߴ�
	*/
	static const RectF GetClientRect();
#ifdef _AFX
	static const CRect GetClientRectC();
#endif
	

	/*!
	 * @brief ��ȡƫ����
	 * @return CPoint ƫ����
	*/
	static const PointF GetOffset();

	/*!
	 * @brief ����ƫ����
	 * @param ptOffset ƫ����
	*/
	static void SetOffset(const Point ptOffset);
	static void SetOffset(const PointF ptOffset);
#ifdef _AFX
	static void SetOffset(const CPoint ptOffset);
#endif //_AFX

	/*!
	 * @brief ��ȡ���ű���
	 * @return float fScale
	*/
	static const float GetScale();

	/*!
	 * @brief �������ű���
	 * @param fScale ���ű���
	*/
	static void SetScale(const float fScale);

	/*!
	 * @brief ��ʸ������ת��Ϊ��������
	 * @param ptVector ʸ������
	 * @return CPoint ��������
	*/
	static const Point VecToWin(const Point ptVector);
	static const PointF VecToWin(const PointF ptVector);
#ifdef _AFX
	static const CPoint VecToWin(const CPoint ptVector);
#endif //_AFX
	static const RectF VecToWin(const RectF rcVector);
	static const Rect VecToWin(const Rect rcVector);

	/*!
	 * @brief ����������ת��δʸ������
	 * @param ptWindow ��������
	 * @return CPoint ʸ������
	*/
	static const PointF WinToVec(const PointF ptWindow);
	static const Point WinToVec(const Point ptWindow);
#ifdef _AFX
	static const CPoint WinToVec(const CPoint ptWindow);
#endif //_AFX
	static const RectF WinToVec(const RectF rcWindow);
	static const Rect WinToVec(const Rect rcWindow);

	/*!
	 * @brief ����ʸ������
	 * @param ptVector ʸ������
	 * @return CPoint �������ʸ������
	*/
	static const Point Correct(const Point ptVector);
	static const PointF Correct(const PointF ptVector);
#ifdef _AFX
	static const CPoint Correct(const CPoint ptVector);
#endif //_AFX

	/*!
	 * @brief ��λ
	 *
	 * ����ͼ�����ƶ���ָ���������
	 * @param CPoint ��λ����
	*/
	static void Locate(const Point ptLocation);
	static void Locate(const PointF ptLocation);
#ifdef _AFX
	static void Locate(const CPoint ptLocation);
#endif //_AFX

	/*!
	 * @brief ˢ��
	 * @param bool Ϊtrueˢ��,����ˢ��
	*/
	static void Refresh(bool bFresh = true);

	static void SetEnableEdit(bool bEdit);

	static const bool EnableEdit();
};

#endif // !_CGRAPH_H