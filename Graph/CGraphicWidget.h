#pragma once
#include "CGraph.h"

#ifndef _CGRAPHICWIDGET_H
#define _CGRAPHICWIDGET_H

class CGraphicWidget :
	public CGraph
{
public:
	CGraphicWidget();
	~CGraphicWidget();
protected:
	Graphics* m_pGraphics;
	StringFormat m_gsFormat;
	SolidBrush* m_pBrush;
	Pen* m_pPen;
	Bitmap* m_pclBmp;
	
protected:
	PointF m_ptCenter;		/*!< �������� ʸ������ */
	PointF m_ptPrepoint;	/*!< ��ʼ���� */
	PointF m_ptLastpoint;	/*!< ��ֹ���� */
	PointF m_ptLocal;		/*!< ��¼���� */
	bool m_bMove;			/*!< �ƶ���ʶ */
	bool m_bSelect;			/*!< ѡ�б�ʶ */
	bool m_bNew;			/*!< ���ݸ��±�ʶ */
	bool m_bRefresh;
	
public:
	/*!
	 * @brief ��ȡѡ��״̬
	 * @return bool ѡ�з���true,δѡ�з���false
	*/
	virtual const bool IsSelected() const;
	virtual const bool IsSelected(PointF& pt) const;
#ifdef _AFX
	virtual const bool IsSelected(CPoint& pt) const;
#endif //_AFX

	virtual const bool IsNewed() const;
	virtual const bool IsMoveed() const;

	/*!
	 * @brief ѡ��
	 * @param ptWinpoint ��������
	*/
	virtual void Select(const PointF ptWinpoint);
#ifdef _AFX
	virtual void Select(const CPoint ptWinpoint);
#endif //_AFX

	/*!
	 * @brief ��ק
	 * @param  ptWinpoint ��������
	 * @return HCURSOR ���ָ����״
	*/
	virtual const HCURSOR Drag(const PointF ptWinPoint);
#ifdef _AFX
	virtual const HCURSOR Drag(const CPoint ptWinPoint);
#endif //_AFX

	/*!
	 * @brief ȷ��
	*/
	virtual void Confirm();

	/*!
	 * @brief ȡ��
	*/
	virtual void Cancel();

	/*!
	 * @brief ��ȡ��������
	 * @return CPoint ��������
	*/
	virtual const PointF GetCenterPoint() const;
#ifdef _AFX
	virtual const CPoint GetCenterPointC() const;
#endif //_AFX

	virtual const PointF GetLocalPoint() const;
#ifdef _AFX
	virtual const CPoint GetLocalPointC() const;
#endif //_AFX

	/*!
	 * @brief ������������
	 * @param ptCenter ��������
	*/
	virtual void SetCenterPoint(const PointF ptCenter);
#ifdef _AFX
	virtual void SetCenterPoint(const CPoint ptCenter);
#endif //_AFX

	void InitGraphic(HDC hdc);

	void Locate();

	void Draw(HDC hdc);
	void Draw(Graphics* graphic);

	virtual void DrawImage(Graphics* graphic) = 0;
};

#endif // !_CGRAPHICWIDGET_H