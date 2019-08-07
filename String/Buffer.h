/*!
 * @file Buffer.h
 * @brief ���ݻ������ṹ���ļ�
 * @author FanKaiyu
 * @date 2019-03-20
 * @version 2.0
 */

#pragma once
#ifndef _BUFFER_H
#define _BUFFER_H

#include <iostream>

static const unsigned int MAX_BUFFER_SIZE = 1000 * 1024; /*!< 1000KB */

/*! @brief ���ݻ����� */
typedef struct TheBufferOfDataStruct
{
    unsigned char* m_pbyBuffer; /*!< ������ָ�� */
    unsigned int m_unSize;      /*!< ��������С */

    TheBufferOfDataStruct()
    {
        m_pbyBuffer = nullptr;
        m_unSize = 0;
    }
    
    TheBufferOfDataStruct(const TheBufferOfDataStruct& buffer)
    {
		m_pbyBuffer = nullptr;
		m_unSize = 0;

		Set(buffer.m_pbyBuffer, buffer.m_unSize);

		return;
    }

	TheBufferOfDataStruct(const unsigned char* pbyBuffer, const unsigned int unSize)
	{
		m_pbyBuffer = nullptr;
		m_unSize = 0;

		Set(pbyBuffer, unSize);

		return;
	}

	~TheBufferOfDataStruct()
	{
		Release();
	}

	void Release()
	{
		if (m_unSize == 0)
		{
			return;
		}
		
		// �ͷ�
		delete[] m_pbyBuffer;
		m_pbyBuffer = nullptr;
		m_unSize = 0;
	}

	void operator= (const TheBufferOfDataStruct& buffer)
	{
		Set(buffer.m_pbyBuffer, buffer.m_unSize);

		return;
	}

	void operator+= (const TheBufferOfDataStruct& buffer)
	{
		Add(buffer.m_pbyBuffer, buffer.m_unSize);

		return;
	}

	TheBufferOfDataStruct operator+ (const TheBufferOfDataStruct& buffer)
	{
		TheBufferOfDataStruct newBuffer(m_pbyBuffer, m_unSize);
		newBuffer += buffer;

		return newBuffer;
	}

	bool operator== (const TheBufferOfDataStruct& buffer)
	{
		return Compare(buffer.m_pbyBuffer, buffer.m_unSize);
	}

	/*!
	 * @brief ��ֵ
	 * @param const unsigned char* ���ݿ�ָ��
	 * @param const unsigned int ���ݿ��С
	*/
	void Set(const unsigned char* pbyBuffer, const unsigned int unSize)
	{
		if (Compare(pbyBuffer, unSize))
		{
			return;
		}

		Release();

		if (pbyBuffer == nullptr || unSize == 0)
		{
			return;
		}

		m_unSize = unSize;
		m_pbyBuffer = new unsigned char[m_unSize];
		// ��ʼ��
		memset(m_pbyBuffer, 0, m_unSize);
		// ����
		memcpy_s(m_pbyBuffer, m_unSize, pbyBuffer, m_unSize);

		return;
	}

	/*!
	 * @brief ��������
	 * @param const unsigned char* ���ݿ�ָ��
	 * @param const unsigned int ���ݿ��С
	*/
	void Add(const unsigned char* pbyBuffer, const unsigned int unSize)
	{
		if (pbyBuffer == nullptr || unSize == 0)
		{
			return;
		}

		unsigned int unNewBufferSize = m_unSize + unSize;		  /*!< �ϲ���Ļ�������С */

		int nDiff = unNewBufferSize - MAX_BUFFER_SIZE;			/*!< ������ֽ��� */

		// �����������
		if (nDiff > 0)
		{
			unNewBufferSize -= nDiff;
		}

		unsigned char* pbyNewBuffer = new unsigned char[unNewBufferSize]; /*!< �ϲ���Ļ����� */

		// ��ʼ��
		memset(pbyNewBuffer, 0, unNewBufferSize);

		if (m_unSize > 0)
		{
			// �����������
			if (nDiff > 0)
			{
				memcpy(pbyNewBuffer, m_pbyBuffer + nDiff, m_unSize - nDiff);
			}
			// �������������
			else
			{
				memcpy(pbyNewBuffer, m_pbyBuffer, m_unSize);
			}
		}

		// �����������
		if (nDiff > 0)
		{
			memcpy(pbyNewBuffer + m_unSize - nDiff, pbyBuffer, unSize);
		}
		// �������������
		else
		{
			memcpy(pbyNewBuffer + m_unSize, pbyBuffer, unSize);
		}

		Release();

		m_pbyBuffer = pbyNewBuffer;
		m_unSize = unNewBufferSize;

		return;
	}

	/*!
	 * @brief �Ƚ�
	 * @param const unsigned char* ���ݿ�ָ��
	 * @param const unsigned int ���ݿ��С
	 * @return bool ��ͬ����true,���򷵻�false
	*/
	bool Compare(const unsigned char* pbyBuffer, const unsigned int unSize)
	{
		if (m_unSize != unSize)
		{
			return false;
		}

		for (unsigned int i = 0; i < m_unSize; ++i)
		{
			if (m_pbyBuffer[i] != pbyBuffer[i])
			{
				return false;
			}
		}

		return true;
	}
}DataBuffer;

#endif //!_BUFFER_H