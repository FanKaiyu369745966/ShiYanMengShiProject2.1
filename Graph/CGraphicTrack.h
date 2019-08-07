#pragma once
#include "CGraphicWidget.h"

#ifndef _CGRAPHICTRACK_H
#define _CGRAPHICTRACK_H

#include <math.h>

class CGraphicTrack :
	public CGraphicWidget
{
public:
	CGraphicTrack(const bool bArc, PointF ptStart = PointF(0, 0), PointF ptEnd = PointF(0, 0));
#ifdef _AFX
	CGraphicTrack(const bool bArc, CPoint ptStart, CPoint ptEnd);
#endif //_AFX
	~CGraphicTrack();

protected:
	bool m_bArc;			/*!< ���߱�ʶ */
	PointF m_ptDistance;	/*!< �յ�������ƫ����*/
	bool m_bShow;			/*!< ��ʾ��ʶ */

protected:
	static float g_fWidth;	/*!< �� */

public:
	/*!
	 * @brief �����������
	 * @param ptStart �������(ʸ������)
	*/
	void SetCenterPoint(const PointF ptStart);
#ifdef _AFX
	void SetCenterPoint(const CPoint ptCenter);
#endif //_AFX

	/*!
	 * @brief �����յ�����
	 * @param ptEnd �յ�����(ʸ������)
	*/
	void SetEndPoint(const PointF ptEnd);
#ifdef _AFX
	void SetEndPoint(const CPoint ptCenter);
#endif //_AFX

	/*!
	 * @brief ���ù������
	 * @param bArc ����Ϊtrue,ֱ��Ϊfalse
	*/
	void SetType(const bool bArc);

	/*!
	 * @brief ��ȡ�յ�����(ʸ������)
	 * @return CPoint �յ�����
	*/
	const PointF GetEndPoint() const;
	const PointF GetLocalEndPoint() const;
	const PointF GetDistancePoint() const;

	/*!
	 * @brief ��ȡ�������
	 * @return bool trueΪ����,falseΪֱ��
	*/
	const bool GetType() const;

	/*!
	 * @brief ���������Ƿ���������
	 * @param ptWinpoint ��������
	 * @return �������ڷ���true,���ڷ���false
	*/
	const bool IsInside(const PointF ptWinpoint);

	/*!
	 * @brief ��ק
	 * @param  ptWinpoint ��������
	 * @return HCURSOR ���ָ����״
	*/
	const HCURSOR Drag(const PointF ptWinPoint);
#ifdef _AFX
	const HCURSOR Drag(const CPoint ptWinPoint);
#endif //_AFX

	/*!
	 * @brief ����
	 * @param pDC ���ھ��
	*/
	void DrawImage(Graphics* graphic);

	/*!
	 * @brief ���ù�����
	 * @param fWidth ���
	*/
	static void SetWidth(const float fWidth);

	/*!
	 * @brief ��ȡ������
	 * @return float ���
	*/
	static const float GetWidth();

	/*!
	 * @brief ����ת�Ƕ�
	 * @param dbRadian ����ֵ
	 * @return double �Ƕ�ֵ
	*/
	static const double RadianToAngle(const double dbRadian);

	/*!
	 * @brief �Ƕ�ת����
	 * @param dbAngle �Ƕ�ֵ
	 * @return double ����ֵ
	*/
	static const double AngleToRadian(const double dbAngle);

	/*!
	 * @brief ����ת�Ƕ�
	 * @param dbRadian ����ֵ
	 * @return double �Ƕ�ֵ
	*/
	static const float RadianToAngleF(const float dbRadian);

	/*!
	 * @brief �Ƕ�ת����
	 * @param dbAngle �Ƕ�ֵ
	 * @return double ����ֵ
	*/
	static const float AngleToRadianF(const float dbAngle);
};

#endif //!_CGRAPHICTRACK_H