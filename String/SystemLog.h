/*!
 * @file SystemLog.h
 * @brief ϵͳ������־�ļ�
 * @author FanKaiYu
 * @date 2019-03-21
 * @version 2.0
*/

#pragma once
#ifndef _SYSTEMLOG_H
#define _SYSTEMLOG_H

#include <string>
#include <chrono>
#include <cinttypes>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdarg.h>
#include <io.h>

using namespace std;

namespace SystemLog
{
	static string g_strLogPath = "../SystemLog";    /*!< ���ϵͳ��־��Ŀ¼ */

	/*!
	 * @brief ����ļ��Ƿ����
	 * @param const char* �ļ�·��
	 * @return bool �ļ����ڷ���true,���򷵻�false
	*/
	static bool IsFileExisted(const char* strPath)
	{
		if (::_access(strPath, 0) == -1)
		{
			return false;
		}

		return true;
	}

	/*!
	 * @brief ����ļ��Ƿ����
	 * @param const wchar_t* �ļ�·��
	 * @return bool �ļ����ڷ���true,���򷵻�false
	*/
	static bool IsFileExisted(const wchar_t* wstrPath)
	{
		if (::_waccess(wstrPath, 0) == -1)
		{
			return false;
		}

		return true;
	}

	/*!
	 * @brief ����ļ��Ƿ����
	 * @param const string �ļ�·��
	 * @return bool �ļ����ڷ���true,���򷵻�false
	*/
	static bool IsFileExisted(const string strPath)
	{
		return IsFileExisted(strPath.c_str());
	}

	/*!
	 * @brief ����ļ��Ƿ����
	 * @param const wstring �ļ�·��
	 * @return bool �ļ����ڷ���true,���򷵻�false
	*/
	static bool IsFileExisted(const wstring wstrPath)
	{
		return IsFileExisted(wstrPath.c_str());
	}

	/*!
	 * @brief �����ļ�
	 * @param const char* �ļ�·��
	 * @return bool �����ɹ�true,���򷵻�false
	*/
	static bool CreateFileEx(const char* strPath)
	{
		char* _str = new char[strlen(strPath) + 1];
		memset(_str, 0, strlen(strPath) + 1);
		memcpy_s(_str, strlen(strPath), strPath, strlen(strPath));

		for (unsigned int i = 0; i < strlen(_str); ++i)
		{
			if (_str[i] == '/')
			{
				_str[i] = '\\';
			}
		}

		stringstream ss;

		char* pLast = nullptr;
		char* pNext = _str;

		pNext = strchr(pNext, '\\');

		// ����Ŀ¼
		if (pNext)
		{
			// �������һ��Ŀ¼
			while (true)
			{
				pNext = strchr(pNext, '\\');
				if (pNext)
				{
					pLast = pNext;
					pNext += 1;
				}
				else
				{
					break;
				}
			}

			if (pLast)
			{
				// �����ļ�
				if (strchr(pLast, '.'))
				{
					char* _path = new char[pLast - _str + 1];
					memset(_path, 0, pLast - _str + 1);
					memcpy_s(_path, pLast - _str, _str, pLast - _str);

					if (strcmp(_path, ".") != 0 && strcmp(_path,"..") != 0)
					{
						ss << "mkdir " << _path;

						delete[] _path;

						// ����Ŀ¼
						system(ss.str().c_str());
					}
				}
				// �������ļ�
				else
				{
					ss << "mkdir " << _str;

					// ����Ŀ¼
					system(ss.str().c_str());
					delete[] _str;

					return true;
				}		
			}
		}

		// �����ļ�
		FILE* pReadFile = nullptr;  /*!< ���ļ�*/

		fopen_s(&pReadFile, _str, "w+");

		if (pReadFile == nullptr)
		{
			delete[] _str;
			return false;
		}

		fclose(pReadFile);

		delete[] _str;
		return true;
	}

	/*!
	 * @brief �����ļ�
	 * @param const wchar* �ļ�·��
	 * @return bool �����ɹ�true,���򷵻�false
	*/
	static bool CreateFileEx(const wchar_t* wstrPath)
	{
		wchar_t* _wstr = new wchar_t[wcslen(wstrPath) + 1];
		wmemset(_wstr, L'\0', wcslen(wstrPath) + 1);
		wmemcpy_s(_wstr, wcslen(wstrPath), wstrPath, wcslen(wstrPath));

		for (unsigned int i = 0; i < wcslen(_wstr); ++i)
		{
			if (_wstr[i] == L'/')
			{
				_wstr[i] = L'\\';
			}
		}

		wstringstream wss;

		wchar_t* pLast = nullptr;
		wchar_t* pNext = _wstr;

		pNext = wcschr(pNext, L'\\');

		// ����Ŀ¼
		if (pNext)
		{
			// �������һ��Ŀ¼
			while (true)
			{
				pNext = wcschr(pNext, L'\\');
				if (pNext)
				{
					pLast = pNext;
					pNext += 1;
				}
				else
				{
					break;
				}
			}

			if (pLast)
			{
				// �����ļ�
				if (wcschr(pLast, L'.'))
				{
					wchar_t* _path = new wchar_t[pLast - _wstr + 1];
					wmemset(_path, 0, pLast - _wstr + 1);
					wmemcpy_s(_path, pLast - _wstr, _wstr, pLast - _wstr);

					if (wcscmp(_path, L".") != 0 && wcscmp(_path, L"..") != 0)
					{
						wss << L"mkdir " << _path;

						delete[] _path;

						_wsystem(wss.str().c_str());
					}
				}
				// �������ļ�
				else
				{
					wss << L"mkdir " << _wstr;

					// ����Ŀ¼
					_wsystem(wss.str().c_str());
					delete[] _wstr;

					return true;
				}
			}
		}

		// �����ļ�
		FILE* pReadFile = nullptr;  /*!< ���ļ�*/

		_wfopen_s(&pReadFile, _wstr, L"w+");

		if (pReadFile == nullptr)
		{
			delete[] _wstr;
			return false;
		}

		fclose(pReadFile);

		delete[] _wstr;
		return true;
	}

	/*!
	 * @brief �����ļ�
	 * @param const string �ļ�·��
	 * @return bool �����ɹ�true,���򷵻�false
	*/
	static bool CreateFileEx(const string strPath)
	{
		return CreateFileEx(strPath.c_str());
	}

	/*!
	 * @brief �����ļ�
	 * @param const wstring �ļ�·��
	 * @return bool �����ɹ�true,���򷵻�false
	*/
	static bool CreateFileEx(const wstring wstrPath)
	{
		return CreateFileEx(wstrPath.c_str());
	}

#ifdef _AFX
	/*!
	 * @brief �����ļ�
	 * @param const CString �ļ�·��
	 * @return bool �����ɹ�true,���򷵻�false
	*/
	static bool CreateFileEx(CString cstrPath)
	{
		return CreateFileEx(cstrPath.GetBuffer());
	}
#endif //_AFX

    /*!
     * @brief ��¼ϵͳ��־
     * @param const char* ��־�ı�
     * @retrun bool ��¼�ɹ�����true,���򷵻�false
    */
	static bool Record(const char* strText)
    {
        if(strlen(strText) == 0)
        {
            return false;
        }

		// ��ȡϵͳ��ǰʱ��
		std::stringstream ss;
		std::chrono::system_clock::time_point tpNow = std::chrono::system_clock::now();
		auto t = std::chrono::system_clock::to_time_t(tpNow);
		// ��ȡ����
		tm tmTime;
		localtime_s(&tmTime, &t);
		// �ϳ�·��
		ss << g_strLogPath << "/" << std::put_time(&tmTime, "%Y%m%d") << "_ϵͳ��־.log";

		if (IsFileExisted(ss.str().c_str()) == false)
		{
			if (CreateFileEx(ss.str().c_str()) == false)
			{
				return false;
			}
		}

		// ���ļ�
		FILE* pReadFile = nullptr;	/*!< �ļ�ָ�� */

		fopen_s(&pReadFile, ss.str().c_str(), "a+");

		if (pReadFile == nullptr)
		{
			return false;
		}

		ss.str("");

		// ��λ���ļ�β��
		fseek(pReadFile, 0, SEEK_END);

		ss << "[" << std::put_time(&tmTime, "%Y-%m-%d %X") << "]\n->>" << strText << "\n";

		fwrite(ss.str().c_str(), sizeof(char), ss.str().length(), pReadFile);

		fclose(pReadFile);

		return true;
    }

    /*!
     * @brief ��¼ϵͳ��־
     * @param const wchar_t* ��־�ı�
     * @retrun bool ��¼�ɹ�����true,���򷵻�false
    */
	static bool Record(const wchar_t* wstrText)
	{
		if (wcslen(wstrText) == 0)
		{
			return false;
		}

		string _str = "";	/*!< ACSII�ַ��� */
#ifdef _ATL
		_str = (char*)_bstr_t(wstrText);
#else
		//��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
		int len = WideCharToMultiByte(CP_ACP, 0, wstrText, (int)wcslen(wstrText), NULL, 0, NULL, NULL);
		char* buffer = new char[len + 1];
		//���ֽڱ���ת���ɶ��ֽڱ���  
		WideCharToMultiByte(CP_ACP, 0, wstrText, (int)wcslen(wstrText), buffer, len, NULL, NULL);
		buffer[len] = '\0';
		//ɾ��������������ֵ  
		_str.append(buffer);
		delete[] buffer;
#endif // !_ATL

		return Record(_str.c_str());
	}

    /*!
     * @brief ��¼ϵͳ��־
     * @param string ��־�ı�
     * @retrun bool ��¼�ɹ�����true,���򷵻�false
    */
	static bool Record(const string strText)
    {
        return Record(strText.c_str());
    }

    /*!
     * @brief ��¼ϵͳ��־
     * @param wstring ��־�ı�
     * @retrun bool ��¼�ɹ�����true,���򷵻�false
    */
	static bool Record(const wstring wstrText)
    {
        return Record(wstrText.c_str());
    }

    #ifdef _AFX
    /*!
     * @brief ��¼ϵͳ��־
     * @param CString ��־�ı�
     * @retrun bool ��¼�ɹ�����true,���򷵻�false
    */
	static bool Record(CString cstrText)
    {
        return Record(cstrText.GetBuffer());
    }
    #endif //_AFX

	/*!
 * @brief ��¼ϵͳ��־
 * @param const char*
 * @return bool ��¼�ɹ�����true,���򷵻�false
 */
	static bool Recordf(const char* _Format, ...)
	{
		std::string tmp;

		va_list marker = NULL;
		va_start(marker, _Format);

		size_t num_of_chars = _vscprintf(_Format, marker);

		if (num_of_chars > tmp.capacity())
		{
			tmp.resize(num_of_chars + 1);
		}

		vsprintf_s((char *)tmp.data(), tmp.capacity(), _Format, marker);

		va_end(marker);

		return Record(tmp.c_str());
	}

	/*!
	 * @brief ��¼ϵͳ��־
	 * @param const wchar_t*
	 * @return bool ��¼�ɹ�����true,���򷵻�false
	 */
	static bool Recordf(const wchar_t* _Format, ...)
	{
		std::wstring tmp;

		va_list marker = NULL;
		va_start(marker, _Format);

		size_t num_of_chars = _vscwprintf(_Format, marker);

		if (num_of_chars > tmp.capacity())
		{
			tmp.resize(num_of_chars + 1);
		}

		vswprintf_s((wchar_t *)tmp.data(), tmp.capacity(), _Format, marker);

		va_end(marker);

		return Record(tmp.c_str());
	}

    /*!
     * @brief �����ļ�·��
     * @param const char* �ļ�·��
     * @return bool ���óɹ�����true,���򷵻�false
    */
   static bool SetPath(const char* strPath)
   {
       if(strlen(strPath) == 0)
       {
           return false;
       }

	   g_strLogPath = strPath;

       return true;
   }

   /*!
	* @brief �����ļ�·��
	* @param const wchar_t* �ļ�·��
	* @return bool ���óɹ�����true,���򷵻�false
   */
   static bool SetPath(const wchar_t* wstrPath)
   {
	   if (wcslen(wstrPath) == 0)
	   {
		   return false;
	   }

#ifdef _ATL
	   g_strLogPath = (char*)_bstr_t(wstrPath);
#else
		//��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
	   int len = WideCharToMultiByte(CP_ACP, 0, wstrPath, (int)wcslen(wstrPath), NULL, 0, NULL, NULL);
	   char* buffer = new char[len + 1];
	   //���ֽڱ���ת���ɶ��ֽڱ���  
	   WideCharToMultiByte(CP_ACP, 0, wstrPath, (int)wcslen(wstrPath), buffer, len, NULL, NULL);
	   buffer[len] = '\0';
	   //ɾ��������������ֵ  
	   g_strLogPath.append(buffer);
	   delete[] buffer;
#endif // !_ATL

	   return true;
   }

   /*!
	* @brief �����ļ�·��
	* @param const string �ļ�·��
	* @return bool ���óɹ�����true,���򷵻�false
   */
   static bool SetPath(const string strPath)
   {
	   return SetPath(strPath.c_str());
   }

   /*!
	* @brief �����ļ�·��
	* @param const wstring �ļ�·��
	* @return bool ���óɹ�����true,���򷵻�false
   */
   static bool SetPath(const wstring wstrPath)
   {
	   return SetPath(wstrPath.c_str());
   }

#ifdef _AFX
   /*!
	* @brief �����ļ�·��
	* @param const CString �ļ�·��
	* @return bool ���óɹ�����true,���򷵻�false
   */
   static bool SetPath(CString cstrPath)
   {
	   return SetPath(cstrPath.GetBuffer());
   }
#endif //_AFX
}

#endif //!_SYSTEMLOG_H