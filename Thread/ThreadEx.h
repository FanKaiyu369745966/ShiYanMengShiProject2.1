/*!
 * @file ThreadEx.h
 * @brief �߳���չ�ļ�
 * @author FanKaiyu
 * @date 2019-04-03
 * @version 1.0
 */

#pragma once
#ifndef _THREADEX_H
#define _THREADEX_H

#include <thread>
#include <sstream>

using namespace std;

namespace ThreadEx
{
    /*!
	 * @brief ��ȡ�߳�ID
	 * @param thread* �߳�ָ��
	 * @return unsigned long �߳�ID
	*/
	static unsigned long GetThreadID(thread* pThread)
	{
		if (pThread == nullptr)
		{
			return 0;
		}

		thread::id thID = pThread->get_id();
		stringstream ss;
		ss << thID;

		return std::stoul(ss.str());
	}
};

#endif //!_THREADEX_H