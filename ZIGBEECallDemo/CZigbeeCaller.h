/*!
 * @file CZigbeeCaller.h
 * @brief Zigbee���������������ļ�
 * @author FanKaiyu
 * @date 2019-04-19
 * @version 1.0
*/
#pragma once
#ifndef _ZGBCALLER_H
#define _ZGBCALLER_H

/*! @brief �������ź� */
enum CALLERSIGNAL
{
	SIGNAL_OFF,
	SIGNAL_ON,
};

/*! @brief ���������� */
enum SIGNAL_TYPE
{
	CALL_SIGNAL = 1,
	CANCELCALL_SIGNAL,
	PASS_SIGNAL,
	CANCELPASS_SIGNAL,
	NG_SIGNAL,
	CANCELNG_SIGNAL,
	SCREAM,
};

typedef class CZigbeeCaller
{
public:
	CZigbeeCaller(const unsigned char byNo, const unsigned char byStation, bool bEnable = true) { Init(byNo, byStation, bEnable); }
	~CZigbeeCaller() {}

protected:
	unsigned char m_byNo;			/*!< ��������� */
	unsigned char m_byStation;		/*!< �󶨹�λ��� */
	unsigned char m_byCall;			/*!< ���б�ʶ */
	unsigned char m_byCancelCall;	/*!< ȡ�����б�ʶ */
	unsigned char m_byPass;			/*!< ���б�ʶ */
	unsigned char m_byCancelPass;	/*!< ȡ�����б�ʶ */
	unsigned char m_byNG;			/*!< NG��ʶ */
	unsigned char m_byCancelNG;		/*!< ȡ��NG��ʶ */
	unsigned char m_byScream;		/*!< ��ͣ��ʶ */
	bool m_bPackOutBound;			/*!< ��վУ�� */
	bool m_bEnable;					/*!< ���ñ�ʶ */
	char m_achMAC[4];				/*!< ������MAC��ַ */

public:
	/*!
	 * @brief ��ʼ������
	 * @param unsigned char ���������
	*/
	void Init(const unsigned char byNo, const unsigned char byStation, bool Enable)
	{
		m_byNo = byNo;
		m_byStation = byStation;
		m_bEnable = Enable;

		m_byCall = SIGNAL_OFF;
		m_byCancelCall = SIGNAL_OFF;
		m_byPass = SIGNAL_OFF;
		m_byCancelPass = SIGNAL_OFF;
		m_byNG = SIGNAL_OFF;
		m_byCancelNG = SIGNAL_OFF;
		m_byScream = SIGNAL_OFF;
		m_bPackOutBound = false;

		memset(m_achMAC, 0, 4);
	}

	/*!
	 * @brief ��ȡ���
	 * @return unsigned char ���������
	*/
	const unsigned char GetNo() const { return m_byNo; }

	/*!
	 * @brief ��ȡ�󶨹�λ
	 * @return unsigned char �󶨹�λ���
	*/
	const unsigned char GetStation() const { return m_byStation; }

	/*!
	 * @brief �󶨹�λ
	 * @param unsigned char �¹�λ���
	 * @return bool �󶨳ɹ�����true,���򷵻�false
	*/
	const bool BindStation(const unsigned char byStation)
	{
		if (m_byStation == byStation)
		{
			return false;
		}

		m_byStation = byStation;

		return true;
	}

	/*!
	 * @brief ��ȡ���б�ʶ
	 * @reutrn unsigned char ���б�ʶ
	*/
	const unsigned char GetCallSignal() const
	{ 
		return m_byCall;
	}

	/*!
	 * @brief ���º��б�ʶ
	 * @param unsigned char ���б�ʶ
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdateCallSignal(const unsigned char byCall)
	{
		if (m_byCall == byCall)
		{
			return false;
		}

		m_byCall = byCall;

		return true;
	}

	/*!
	 * @brief ��ȡȡ�����б�ʶ
	 * @return unsigned char ȡ�����б�ʶ
	*/
	const unsigned char GetCancelCallSignal() const { return m_byCancelCall; }

	/*!
	 * @brief ����ȡ�����б�ʶ
	 * @param unsigned char ȡ�����б�ʶ
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdateCancelCallSignal(const unsigned char byCancelCall)
	{
		if (m_byCancelCall == byCancelCall)
		{
			return false;
		}

		m_byCancelCall = byCancelCall;

		return true;
	}

	/*!
	 * @brief ��ȡ���б�ʶ
	 * @return unsigned char ���б�ʶ
	*/
	const unsigned char GetPassSignal() const { return m_byPass; }

	/*!
	 * @brief ���·��б�ʶ
	 * @param unsigned char ���б�ʶ
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdatePassSignal(const unsigned char byPass)
	{
		if (m_byPass == byPass)
		{
			return false;
		}

		m_byPass = byPass;

		if (m_bPackOutBound && m_byPass == SIGNAL_OFF)
		{
			m_bPackOutBound = false;
		}

		return true;
	}

	/*!
	 * @brief ��ȡȡ�����б�ʶ
	 * @return unsigned char ȡ�����б�ʶ
	*/
	const unsigned char GetCancelPassSignal() const { return m_byCancelPass; }

	/*!
	 * @brief ����ȡ�����б�ʶ
	 * @param unsigned char ȡ�����б�ʶ
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdateCancelPassSignal(const unsigned char byCancelPass)
	{
		if (m_byCancelPass == byCancelPass)
		{
			return false;
		}

		m_byCancelPass = byCancelPass;

		return true;
	}

	/*!
	 * @brief �Ƿ�ͨ����վУ��
	 * @return bool ͨ������true,���򷵻�false
	*/
	const bool IsPackOutBounded() const { return m_bPackOutBound; }

	/*!
	 * @brief ���³�վУ��
	 * @param bool ��վУ��
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdatePackOutBound(bool bPackOutBound)
	{
		if (m_bPackOutBound == bPackOutBound)
		{
			return false;
		}

		m_bPackOutBound = bPackOutBound;

		return true;
	}

	/*!
	 * @brief ��ȡNG��ʶ
	 * @return unsigned char NG��ʶ
	*/
	const unsigned char GetNGSignal() const { return m_byNG; }

	/*!
	 * @brief ����NG��ʶ
	 * @param unsigned char NG��ʶ
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdateNGSignal(const unsigned char byNG)
	{
		if (m_byNG == byNG)
		{
			return false;
		}

		m_byNG = byNG;

		return true;
	}

	/*!
	 * @brief ��ȡȡ��NG��ʶ
	 * @return unsigned char ȡ��NG��ʶ
	*/
	const unsigned char GetCancelNGSignal() const { return m_byCancelNG; }

	/*!
	 * @brief ����ȡ��NG��ʶ
	 * @param unsigned char ȡ��NG��ʶ
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdateCancelNGSignal(const unsigned char byCancelNG)
	{
		if (m_byCancelNG == byCancelNG)
		{
			return false;
		}

		m_byCancelNG = byCancelNG;

		return true;
	}

	/*!
	 * @brief ��ȡ��ͣ��ʶ
	 * @return unsigned char ��ͣ��ʶ
	*/
	const unsigned char GetScreamSignal() const { return m_byScream; }

	/*!
	 * @brief ���¼�ͣ��ʶ
	 * @param unsigned char ��ͣ��ʶ
	 * @return bool ���·���true,���򷵻�false
	*/
	const bool UpdateScreamSignal(const unsigned char byScream)
	{
		if (m_byScream == byScream)
		{
			return false;
		}

		m_byScream = byScream;

		return true;
	}

	void SetEnableUse(const bool bEnable) { m_bEnable = bEnable; }
	const bool EnableUse() const { return m_bEnable; }

	void BindMACAddress(const char achMAC[4])
	{
		memcpy_s(m_achMAC, 4, achMAC, 4);

		return;
	}

	void GetMACAddress(char* achMAC)
	{
		memcpy_s(achMAC, 4, m_achMAC, 4);

		return;
	}

} ZGBCaller;

#endif //!_ZGBCALLER_H