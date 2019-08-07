/*!
 * @file CRFID.h
 * @breif ����RFID���Ļ���������Ϣ�Լ������ļ�
 * @author FanKaiyu
 * @date 2019-04-10
 * @version 2.1
*/
#pragma once
#ifndef _RFID_H
#define _RFID_H

#include <chrono>

using namespace std;
using namespace chrono;

/*!
 * @class RFIDMark
 * @brief RFID�ر꿨��
 *
 * ����RFID���Ļ���������Ϣ�Լ�����
*/
typedef class RFIDMark
{
public:
	RFIDMark() :m_usNo(0), m_pLocker(nullptr), m_pPreLocker(nullptr), m_tpLockTime(steady_clock::now()) {};
	RFIDMark(unsigned short usNo) :m_usNo(usNo), m_pLocker(nullptr), m_pPreLocker(nullptr), m_tpLockTime(steady_clock::now()){};
	~RFIDMark() {};

protected:
	unsigned short m_usNo;								/*!< ��� */
	void* m_pLocker;									/*!< ������RFID����ָ�� */
	steady_clock::time_point m_tpLockTime;				/*!< ������RFID����ʱ�� */
	void* m_pPreLocker;									/*!< Ԥ��������RFID����ָ�� */
public:
	/*!
	 * @brief ��ȡRFID�����
	 * @return unsigned short ���
	*/
	unsigned short GetNo() const { return m_usNo; }

	/*!
	 * @brief ����RFID�����
	 * @param unsigned short ��RFID�����
	*/
	bool SetNo(const unsigned short usRFID) 
	{
		if (m_usNo == usRFID)
		{
			return false;
		}

		m_usNo = usRFID;

		return true;
	}

	/*!
	 * @brief ����
	 * @param void* ������RFID����ָ��
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool Lock(void* pLocker)
	{
		if (pLocker == nullptr)
		{
			return false;
		}

		if (m_pLocker == nullptr)
		{
			m_pLocker = pLocker;
			m_tpLockTime = steady_clock::now();
		}

		if (m_pLocker != pLocker)
		{
			return false;
		}


		if (m_pPreLocker == m_pLocker)
		{
			m_pPreLocker = nullptr;
		}	

		return true;
	}

	/*!
	 * @brief �������
	 * @param void* ������RFID����ָ��
	*/
	void Unlock(void* pLocker)
	{
		if (m_pLocker == pLocker)
		{
			m_pLocker = nullptr;

			return;
		}

		return;
	}

	/*!
	 * @brief Ԥ������
	 * @param void* ������RFID����ָ��
	 * @return bool �����ɹ�����true,���򷵻�false
	*/
	bool PreformLock(void* pLocker)
	{
		if (pLocker == nullptr)
		{
			return false;
		}

		if (m_pPreLocker == nullptr)
		{
			m_pPreLocker = pLocker;
		}

		if (m_pPreLocker != pLocker)
		{
			return false;
		}

		return true;
	}

	/*!
	 * @brief ���Ԥ������
	 * @param void* ������RFID����ָ��
	*/
	void PreformUnlock(void* pLocker)
	{
		if (m_pPreLocker == pLocker)
		{
			m_pPreLocker = nullptr;

			return;
		}

		return;
	}

	/*!
	 * @brief �ͷ�
	 *
	 * ���RFID����������
	*/
	void Release()
	{
		m_pLocker = nullptr;
		m_pPreLocker = nullptr;
	}

	/*!
	 * @brief ��ȡ������RFID����ָ��
	 * @return void* ������RFID����ָ��
	*/
	void* GetLocker() const { return m_pLocker; }

	/*!
	 * @brief ��ȡ������RFID����ʱ��
	 * @param chrono::system_clock::time_point& ������RFID����ʱ��
	 * @return void* ������RFID����ָ��
	*/
	void* GetLockTime(steady_clock::time_point& tpLockTime) const
	{
		tpLockTime = m_tpLockTime;

		return m_pLocker;
	}

	/*!
	 * @brief �Ա�������RFID����ʱ��
	 * @param const chrono::system_clock::time_point& �Աȵ�ʱ��
	 * @return long long ʱ����ڶԱ�ʱ�䷵������,���ڷ���0,С�ڷ��ظ���
	*/
	long long Compare(const steady_clock::time_point& tpCompareTime)
	{
		return duration_cast<milliseconds>(m_tpLockTime - tpCompareTime).count();
	}

	/*!
	 * @brief �Ա�������RFID��
	 * @param const RFIDMark& �Աȵ�RFID��
	 * @return long long ʱ����ڶԱ�ʱ�䷵������,���ڷ���0,С�ڷ��ظ���
	*/
	long long Compare(const RFIDMark& rfid)
	{
		steady_clock::time_point tpCompareTime;	/*!< �Ա�ʱ�� */
		if (rfid.GetLockTime(tpCompareTime) == nullptr)
		{
			return -1;
		}

		return Compare(tpCompareTime);
	}

	/*!
	 * @brief ��ȡԤ��������RFID����ָ��
	 * @return void* Ԥ��������RFID����ָ��
	*/
	void* GetPreformLocker() const
	{
		return m_pPreLocker;
	}
} RFID;

#endif //!_RFID_H