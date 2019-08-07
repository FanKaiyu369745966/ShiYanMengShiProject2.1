/*!
 * @file CTraffic.h
 * @brief ������ͨ��������Ļ��������Լ�����
 * @author FanKaiyu
 * @date 2019-04-10
 * @version 2.0
*/
#pragma once
#ifndef _TRAFFIC_H
#define _TRAFFIC_H

#include <set>

using namespace std;

/*!
 * @class CTrafficArea
 * @brief ������ͨ��������Ļ��������Լ�����
*/
typedef class CTrafficArea
{
public:
	CTrafficArea(unsigned short usRFID) :m_usCtrlRFID(usRFID) {}
	~CTrafficArea() {}
protected:
	unsigned short m_usCtrlRFID;		/*!< ����RFID��:AGV�ڴ�RFID��ֹͣ�����Ϊ��ͨ����״̬,�ȴ�������� */
	set<unsigned short> m_setStop;		/*!< ����RFID����:������RFID����AGV���н�ʱ,�����˹�����������������˹���������صĹ���AGV�������� */
	set<unsigned short> m_setCompare;	/*!< ���Ʊȶ���:������RFID���ϴ����н�AGVʱ,������������������Ա�,�ȵ����� */

public:
	/*!
	 * @brief ��ӽ���RFID��
	 * @param unsigned short ����RFID�����
	 * @return bool ��ӳɹ�����true,ʧ�ܷ���false
	*/
	bool AddStopRFID(const unsigned short usRFID)
	{
		if (m_usCtrlRFID == usRFID)
		{
			return false;
		}

		if (m_setStop.find(usRFID) != m_setStop.end())
		{
			return false;
		}

		m_setStop.insert(usRFID);

		return true;
	}

	/*!
	 * @brief ��ӱȶԹ�������
	 * @param unsigned short ����������
	 * @return bool ��ӳɹ�����true,ʧ�ܷ���false
	*/
	bool AddCompareArea(const unsigned short usArea)
	{
		if (m_usCtrlRFID == usArea)
		{
			return false;
		}

		if (m_setCompare.find(usArea) != m_setCompare.end())
		{
			return false;
		}

		m_setCompare.insert(usArea);

		return true;
	}

	/*!
	 * @brief ��ȡ����RFID��
	 * @return unsigned short ����RFID��
	*/
	const unsigned short GetControlRFID() const { return m_usCtrlRFID; }

	/*!
	 * @brief ��ȡ����RFID����
	 * @return set<unsigned short>& ����RFID����
	*/
	const set<unsigned short>& GetStopArray() const { return m_setStop; }

	/*!
	 * @brief ��ȡ�ȶԹ�����
	 * @return set<unsigned short>& �ȶԹ�����
	*/
	const set<unsigned short>& GetCompareArray() const { return m_setCompare; }

}Traffic;

#endif //!_TRAFFIC_H