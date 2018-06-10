#pragma once

#include "TxStdlib.hpp"

#include <stdlib.h>
#include <assert.h>
#include <set>
#include <algorithm>
#include <list>
#include <cstdint>
#include <functional>
#include <ctime>
#include <string>
#include <atlstr.h>
#include <comdef.h>
#include <Winsock.h>
#include <mswsock.h>
#include <process.h>
#include <Windows.h>
#include <winnt.h>
#include <intrin.h>
#include <process.h>
#include <shellapi.h>
#include <tchar.h>
#include <atlstr.h>
#include <Dbghelp.h>
#include <Tlhelp32.h>
#include <Objbase.h>
#include <psapi.h>
#include <Shlobj.h>
#include <MMSystem.h>
#include <Endpointvolume.h>
#include <Mmdeviceapi.h>
#include <Iphlpapi.h>
#include <winioctl.h>
#include <NtDDNdis.h>
#include <intrin.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"Dbghelp.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Iphlpapi.lib")

#ifndef NDEBUG
#define _Def_tx_sysMessageBoxDebugInfo___(_wtext) \
	do{ ::MessageBoxW(NULL, (_TxPrivateNs_::CSyslibBase::sysLastErrorInfo() + TxBlobWString(L"\r\n") \
	+ TxBlobWString(_wtext)).c_str(),TxBlobString(typeid(this).name()).utf8Codecvt().c_str(), MB_OK); }while (false)
#else
#define _Def_tx_sysMessageBoxDebugInfo___(_wtext) 
#endif


//--------------------------------------TxSysMutex--------------------------------------//
class TxSysMutex : private TxForbidDirectCopyObject
{
private:
	CRITICAL_SECTION m_cs;
public:
	TxSysMutex()
	{
		::memset(&this->m_cs, 0, sizeof(this->m_cs));
		::InitializeCriticalSection(&this->m_cs);
	}
	~TxSysMutex()
	{
		::DeleteCriticalSection(&this->m_cs);
	}
	void lock()
	{
		::EnterCriticalSection(&this->m_cs);
	}
	bool try_lock()
	{
		return !!::TryEnterCriticalSection(&this->m_cs);
	}
	void unlock()
	{
		::LeaveCriticalSection(&this->m_cs);
	}
};


//--------------------------------------TxSysMemoryPool--------------------------------------//
class TxSysMemoryPool
{
private:
	static TxMemoryPool<TxSysMutex>* _memPool_()
	{
		static TxMemoryPool<TxSysMutex> memPool;
		return &memPool;
	}
public:
	static void initEnv() { _memPool_(); }
	static void* malloc(size_t _size, size_t *_maxSize = NULL)
	{
		return _memPool_()->malloc(_size, _maxSize);
	}
	static void free(void *_ptr)
	{
		return _memPool_()->free(_ptr);
	}
	static void clearDirty()
	{
		return _memPool_()->clearDirty();
	}
};


namespace _TxPrivateNs_ {
#define _Def_sysCallLogFunction___(_err_cb_,_msg_text) ((_err_cb_)!=NULL?(_err_cb_(_msg_text,__FUNCTIONW__),true):(true))

	class CSyslibBase
	{
	public:
		typedef void(*TypeErrCb)(const wchar_t *_text, const wchar_t *_func);
	public:
		static TxBlobWString sysLastErrorInfo()
		{
			TxBlobWString ret;
			LPVOID lpMsgBuf = NULL;
			DWORD lc_errCode = ::GetLastError();
			::FormatMessageW(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, //dwFlags 
				NULL, //lpSource 
				lc_errCode, //dwMessageId 
				0, //GetUserDefaultLangID(),// MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),//dwLanguageId 
				(LPWSTR)&lpMsgBuf, //lpBuffer 
				0, //nSize 
				NULL //Arguments 
			);
			if (lpMsgBuf)
			{
				ret.compose(L" GetLastError({0})->[{1}]. ").args(lc_errCode, TxBlobWString((const wchar_t*)lpMsgBuf).trimLR(L"\r\n\t "));
				::LocalFree((HLOCAL)lpMsgBuf);
				lpMsgBuf = NULL;
			}
			return ret;
		}
		static wchar_t* getWorkingDirectory(const wchar_t **_procName, TypeErrCb _err_cb_)
		{
			struct T_R
			{
				enum { e_arry_size = (FILENAME_MAX | MAX_PATH) + sizeof(long long), };
				wchar_t sWorkDir[e_arry_size];
				wchar_t sProcName[e_arry_size];
				T_R()
				{
					::memset(this->sWorkDir, 0, sizeof(this->sWorkDir));
					::memset(this->sProcName, 0, sizeof(this->sProcName));
					wchar_t szLcPathTem_[e_arry_size] = { 0 };
					if (::GetModuleFileNameW(NULL, szLcPathTem_, e_arry_size - 1) <= 0)
						assert(false);
					wchar_t szLcPath[e_arry_size] = { 0 };
					if (::GetFullPathNameW(szLcPathTem_, e_arry_size - 1, szLcPath, NULL) <= 0)
						assert(false);
					for (int i = 0; szLcPath[i] != 0; i++)
					{
						if (szLcPath[i] == L'/')
							szLcPath[i] = L'\\';
					}
					int pos = -1;
					for (int i = 0; szLcPath[i] != 0; i++)
					{
						if (szLcPath[i] == L'\\')
							pos = i;
					}
					::wcscpy(this->sProcName, &szLcPath[pos + 1]);
					if (::wcslen(this->sProcName) <= 0)
						assert(false);
					if (pos >= 1 && szLcPath[pos] == L'\\')
						szLcPath[pos + 1] = 0;
					else
						::wcscpy(szLcPath, L".\\");
					::wcscpy(this->sWorkDir, szLcPath);
				}
			};
			static T_R _tem__;
			if (_tem__.sProcName[0] == 0 || _tem__.sWorkDir[0] == 0)
				_Def_sysCallLogFunction___(_err_cb_, L"if (_tem__.sProcName[0] == 0 || _tem__.sWorkDir[0] == 0)执行错误");
			if (_procName != NULL)
				*_procName = _tem__.sProcName;
			return _tem__.sWorkDir;
		}
		static long long getHighPrecisionMicrosecond(TypeErrCb _err_cb_)
		{
			static LONGLONG ll_param[2] = { 0 };

			LARGE_INTEGER lcPerformanceCounter = { 0 };
			if (!::QueryPerformanceCounter(&lcPerformanceCounter))
			{
				_Def_sysCallLogFunction___(_err_cb_, L"QueryPerformanceCounter()执行失败。");
				return 0;
			}

			if (ll_param[0] == 0)
			{
				LARGE_INTEGER lcPerformanceFrequency = { 0 };
				if (!::QueryPerformanceFrequency(&lcPerformanceFrequency))
				{
					_Def_sysCallLogFunction___(_err_cb_, L"QueryPerformanceFrequency()函数调用失败。");
					return 0;
				}
				ll_param[0] = lcPerformanceFrequency.QuadPart;
				if (ll_param[0] == 0)
				{
					_Def_sysCallLogFunction___(_err_cb_, L"lcPerformanceFrequency.QuadPart==0");
					return 0;
				}
				ll_param[1] = lcPerformanceCounter.QuadPart - lcPerformanceFrequency.QuadPart;
			}

			return (long long)(lcPerformanceCounter.QuadPart - ll_param[1]) * (long long)1000000 / (long long)ll_param[0];
		}
		static bool getSystemMemoryStatus(long long *_totalMemSize, long long *_usedMemSize, TypeErrCb _err_cb_)
		{
			MEMORYSTATUSEX lc_statex = { 0 };
			lc_statex.dwLength = sizeof(lc_statex);
			if (!::GlobalMemoryStatusEx(&lc_statex))
			{
				_Def_sysCallLogFunction___(_err_cb_, L"GlobalMemoryStatusEx()函数调用失败。");
				return false;
			}
			if (_totalMemSize != NULL)
				*_totalMemSize = (long long)lc_statex.ullTotalPhys;
			if (_usedMemSize != NULL)
			{
				*_usedMemSize = TxStdlib::adjustRange(
					(long long)(lc_statex.ullTotalPhys - lc_statex.ullAvailPhys),
					(long long)0, (long long)lc_statex.ullTotalPhys);
			}
			return true;
		}
	};
#undef _Def_sysCallLogFunction___
}


//--------------------------------------TxSysProcessMutex--------------------------------------//
template<bool B_THREAD_SAFE>
class TxSysProcessMutex : private TxForbidDirectCopyObject
{
private:
	HANDLE m_hMutex;
	unsigned char memMutexBuffer[B_THREAD_SAFE ? sizeof(TxSysMutex) : sizeof(long long)];
private:
	bool _lock_(DWORD _dw_msec)
	{
		if (this->m_hMutex == NULL)
			return false;
		if (B_THREAD_SAFE)
			((TxSysMutex*)this->memMutexBuffer)->lock();
#ifndef NDEBUG
		DWORD tem = ::WaitForSingleObject(this->m_hMutex, _dw_msec);
		bool ret = false;
		switch (tem)
		{
		case WAIT_OBJECT_0:
			ret = true;
			tem = 0;
			break;
		case WAIT_ABANDONED:
			tem = 0;
			break;
		case WAIT_TIMEOUT:
			tem = 0;
			break;
		default:
			tem = 0;
			break;
		}
		return ret;
#else
		return WAIT_OBJECT_0 == ::WaitForSingleObject(this->m_hMutex, _dw_msec);
#endif
	}
public:
	TxSysProcessMutex(const TxBlobString &_key)
	{
		if (B_THREAD_SAFE)
			new ((TxSysMutex*)this->memMutexBuffer) TxSysMutex();
		this->m_hMutex = ::OpenMutexA(MUTEX_ALL_ACCESS, FALSE, _key.c_str());
		if (this->m_hMutex == NULL)
			this->m_hMutex = ::CreateMutexA(NULL, FALSE, _key.c_str());
		if (this->m_hMutex == NULL)
		{
			::Sleep(200);
			this->m_hMutex = ::OpenMutexA(MUTEX_ALL_ACCESS, FALSE, _key.c_str());
		}
		if (this->m_hMutex == NULL)
		{
			_Def_tx_sysMessageBoxDebugInfo___(L"OpenMutexA()CreateMutexA()执行失败");
		}
	}
	~TxSysProcessMutex()
	{
		if (this->m_hMutex != NULL)
		{
			if (!::CloseHandle(this->m_hMutex))
			{
				_Def_tx_sysMessageBoxDebugInfo___(L"CloseHandle()执行失败");
			}
			this->m_hMutex = NULL;
		}
		if (B_THREAD_SAFE)
			((TxSysMutex*)this->memMutexBuffer)->~TxSysMutex();
	}
	bool try_lock()
	{
		return this->_lock_((DWORD)0);
	}
	bool lock(unsigned int _msec = 0)
	{
		return this->_lock_(_msec <= 0 ? (DWORD)(INFINITE) : (DWORD)_msec);
	}
	void unlock()
	{
		if (this->m_hMutex != NULL)
		{
			::ReleaseMutex(this->m_hMutex);
			if (B_THREAD_SAFE)
				((TxSysMutex*)this->memMutexBuffer)->unlock();
		}
	}
};


//--------------------------------------TxSysDebugObjectMemory & TxSysMemoryLeakObject--------------------------------------//
class TxSysDebugObjectMemory
{
public:
	class CSysMemoryLeakObject_
	{
#ifndef NDEBUG
	public:
		CSysMemoryLeakObject_()
		{
			TxSysDebugObjectMemory::malloc(this);
		}
		CSysMemoryLeakObject_(const CSysMemoryLeakObject_&)
		{
			TxSysDebugObjectMemory::malloc(this);
		}
		virtual ~CSysMemoryLeakObject_()
		{
			TxSysDebugObjectMemory::free(this);
		}
#endif
	};

#ifndef NDEBUG
	class _TagUnitInfo_
	{
	private:
		std::string name;
		CSysMemoryLeakObject_ *ptr;
	public:
		_TagUnitInfo_(CSysMemoryLeakObject_ *_ptr, const std::string &_name)
		{
			this->name = _name;
			this->ptr = _ptr;
		}
		bool operator<(const _TagUnitInfo_ &_tem) const
		{
			if (this->name.length() != _tem.name.length())
				return this->name.length() < _tem.name.length();
			int val = this->name.compare(_tem.name);
			if (val != 0)
				return val < 0;
			return this->ptr < _tem.ptr;
		}
	};
	std::set<_TagUnitInfo_> mObjectSet;
	TxSysMutex mMutex;
private:
	TxSysDebugObjectMemory()
	{
	}
	~TxSysDebugObjectMemory()
	{
		this->checkPerfect();
	}
	void _lock___()
	{
		this->mMutex.lock();
	}
	void _unlock___()
	{
		this->mMutex.unlock();
	}
	static TxSysDebugObjectMemory* _env_()
	{
		static TxSysDebugObjectMemory ret;
		return &ret;
	}
	void _my_checkPerfect()
	{
		this->_lock___();
		if (this->mObjectSet.size() != 0)
			_Def_tx_sysMessageBoxDebugInfo___(L"if(this->mObjectSet.size() != 0)");
		this->_unlock___();
	}
	template<class TYPE>
	void _my_malloc(TYPE *_ptr)
	{
		_TagUnitInfo_ lc_elem((CSysMemoryLeakObject_*)_ptr, typeid(TYPE).name());
		this->_lock___();
		if (this->mObjectSet.find(lc_elem) != this->mObjectSet.end())
			_Def_tx_sysMessageBoxDebugInfo___(L"if (this->mObjectSet.find(lc_elem) != this->mObjectSet.end())");
		this->mObjectSet.insert(lc_elem);
		this->_unlock___();
	}
	template<class TYPE>
	void _my_free(TYPE *_ptr)
	{
		_TagUnitInfo_ lc_elem((CSysMemoryLeakObject_*)_ptr, typeid(TYPE).name());
		this->_lock___();
		std::set<_TagUnitInfo_>::iterator iter = this->mObjectSet.find(lc_elem);
		if (iter == this->mObjectSet.end())
			_Def_tx_sysMessageBoxDebugInfo___(L"if (iter == this->mObjectSet.end())");
		else
			this->mObjectSet.erase(iter);
		this->_unlock___();
	}
public:
	template<class TYPE> static inline void malloc(TYPE *_ptr) { _env_()->_my_malloc(_ptr); }
	template<class TYPE> static inline void free(TYPE *_ptr) { _env_()->_my_free(_ptr); }
	static void checkPerfect() { _env_()->_my_checkPerfect(); }
#else
	static inline void malloc(void*) { }
	static inline void free(void*) { }
	static void checkPerfect() { }
#endif
};
typedef TxSysDebugObjectMemory::CSysMemoryLeakObject_ TxSysMemoryLeakObject;


//--------------------------------------TxSysProcessMemory--------------------------------------//
class TxSysProcessMemory
{
private:
	HANDLE hMemHandle;
	void *pMemoryPtr;
private:
	bool _initialize_(bool *_bCreate, const char *_key, int _size, bool _bOnlyOpen = false)
	{
		this->uninitialize();
		bool lc_bCreate_temp = false;
		if (_bCreate == NULL)
			_bCreate = &lc_bCreate_temp;
		*_bCreate = false;
		if (_size < 0) _size = 0;
		_size = ((_size + 63) / 64) * 64;
		this->hMemHandle = ::OpenFileMappingA(FILE_MAP_ALL_ACCESS, 0, _key);
		if (_bOnlyOpen&&this->hMemHandle == NULL)
			return false;
		if (this->hMemHandle == NULL)
		{
			this->hMemHandle = ::CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, _size, _key);
			if (this->hMemHandle != NULL&&::GetLastError() != ERROR_ALREADY_EXISTS)
				*_bCreate = true;
		}
		if (this->hMemHandle == NULL)
		{
			::Sleep(200);
			this->hMemHandle = ::OpenFileMappingA(FILE_MAP_ALL_ACCESS, 0, _key);
		}
		if (this->hMemHandle == NULL)
		{
			_Def_tx_sysMessageBoxDebugInfo___(L"OpenFileMappingA()CreateFileMappingA()执行失败");
		}
		if (this->hMemHandle != NULL)
			this->pMemoryPtr = ::MapViewOfFile(this->hMemHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (this->pMemoryPtr == NULL)
			this->uninitialize();
		if (this->pMemoryPtr == NULL)
		{
			_Def_tx_sysMessageBoxDebugInfo___(L"MapViewOfFile()执行失败");
		}
		return this->pMemoryPtr != NULL;
	}
public:
	TxSysProcessMemory()
	{
		this->hMemHandle = NULL;
		this->pMemoryPtr = NULL;
	}
	~TxSysProcessMemory()
	{
		this->uninitialize();
	}
	bool initializeOnlyOpen(const char *_key)
	{
		return this->_initialize_(NULL, _key, 0, true);;
	}
	bool initialize(bool *_bCreate, const char *_key, int _size)
	{
		return this->_initialize_(_bCreate, _key, _size);
	}
	void uninitialize()
	{
		if (this->pMemoryPtr != NULL)
		{
			if (!::UnmapViewOfFile(this->pMemoryPtr))
			{
				_Def_tx_sysMessageBoxDebugInfo___(L"if (!::UnmapViewOfFile(this->pMemoryPtr))执行失败！");
			}
			this->pMemoryPtr = NULL;
		}
		if (this->hMemHandle != NULL)
		{
			if (!::CloseHandle(this->hMemHandle))
			{
				_Def_tx_sysMessageBoxDebugInfo___(L"if (!::CloseHandle(this->hMemHandle))执行失败！");
			}
			this->hMemHandle = NULL;
		}
	}
	unsigned char* getMemoryPtr()
	{
		return this->hMemHandle != NULL ? (unsigned char*)this->pMemoryPtr : NULL;
	}
};


//--------------------------------------TxSysLog--------------------------------------//
namespace _TxPrivateNs_ {
	class _MyLogHandleBasic
	{
	public:
		enum EtLogLevel {
			ELL_info = 1,
			ELL_warn = 2,
			ELL_error = 3,
		};
	public:
		virtual ~_MyLogHandleBasic() { }
		virtual void vfWrite(const TxBlobWString &_text, _MyLogHandleBasic::EtLogLevel _logLevel) = 0;
	};
	class _MyLogDebugOutputWrap : public _MyLogHandleBasic
	{
	public:
		virtual void vfWrite(const TxBlobWString &_text, _MyLogHandleBasic::EtLogLevel _logLevel)
		{
			(void)_logLevel;
			::OutputDebugStringW(_text.c_str());
		}
	};
	class _MyWndLogWrap_ : public _MyLogHandleBasic
	{
	private:
		HANDLE hEventSource;
		TxSysMutex mMutex;
	public:
		_MyWndLogWrap_()
		{
			const wchar_t *lc_proc_name = NULL;
			_TxPrivateNs_::CSyslibBase::getWorkingDirectory(&lc_proc_name, NULL);
			if (lc_proc_name == NULL || lc_proc_name[0] == 0)
				lc_proc_name = L"unkown";
			this->hEventSource = ::RegisterEventSourceW(NULL, lc_proc_name);
			if (this->hEventSource == NULL)
			{
				_Def_tx_sysMessageBoxDebugInfo___(L"this->hEventSource = ::RegisterEventSourceW(NULL, lc_proc_name);调用失败。");
			}
		}
		~_MyWndLogWrap_()
		{
			if (this->hEventSource != NULL)
			{
				if (!::DeregisterEventSource(this->hEventSource))
				{
					_Def_tx_sysMessageBoxDebugInfo___(L"if (!::DeregisterEventSource(this->hEventSource))调用失败。");
				}
				this->hEventSource = NULL;
			}
		}
		virtual void vfWrite(const TxBlobWString &_text, _MyLogHandleBasic::EtLogLevel _logLevel)
		{
			WORD lc_log_level = EVENTLOG_SUCCESS;
			switch (_logLevel)
			{
			case _TxPrivateNs_::_MyLogHandleBasic::ELL_info:
				lc_log_level = EVENTLOG_INFORMATION_TYPE;
				break;
			case _TxPrivateNs_::_MyLogHandleBasic::ELL_warn:
				lc_log_level = EVENTLOG_WARNING_TYPE;
				break;
			case _TxPrivateNs_::_MyLogHandleBasic::ELL_error:
				lc_log_level = EVENTLOG_ERROR_TYPE;
				break;
			default:
				assert(false);
				lc_log_level = EVENTLOG_SUCCESS;
				break;
			}

			std::list<TxBlobWString> lcListLog;
			const wchar_t *lc_trim_chs = L"\r\n\t ";
			const wchar_t *lc_data_ptr = _text.data();
			while (lc_data_ptr != NULL)
			{
				const wchar_t *lc_find_pos = std::wcschr(lc_data_ptr, L'\n');
				if (lc_find_pos == NULL)
				{
					lcListLog.push_back(TxBlobWString(lc_data_ptr).trimLR(lc_trim_chs));
					lc_data_ptr = NULL;
				}
				else
				{
					lcListLog.push_back(TxBlobWString(lc_data_ptr, (int)(lc_find_pos - lc_data_ptr)).trimLR(lc_trim_chs));
					lc_data_ptr = lc_find_pos + 1;
				}
				if (lcListLog.size() > 0 && lcListLog.back().length() <= 0)
					lcListLog.pop_back();
			}
			std::vector<const wchar_t*> lc_vct_log_texts_ptr;
			for (auto iter = lcListLog.begin(); iter != lcListLog.end() && lc_vct_log_texts_ptr.size() < 8; ++iter)
				lc_vct_log_texts_ptr.push_back(iter->c_str());

			this->mMutex.lock();
			if (this->hEventSource != NULL)
			{
				if (!::ReportEventW(this->hEventSource, lc_log_level, 0, 26889, NULL,
					(WORD)lc_vct_log_texts_ptr.size(), 0, lc_vct_log_texts_ptr.data(), NULL))
				{
					_Def_tx_sysMessageBoxDebugInfo___(L"ReportEventW调用失败。");
				}
			}
			this->mMutex.unlock();
		}
	};
#pragma pack(push)
#pragma pack(1)
	struct tagMyFileInfoMemInfo_ {
		FILE *logFp;//日志文件(self)
		std::uint32_t refCount;
		std::uint32_t fileSize;
		unsigned char _reserved[64];
	};
	struct tagMyLogContentMemInfo_ {
		enum { e_file_max_size = 2 * 1024 * 1024 };
		std::uint32_t contentLen;
		unsigned char logContent[2 * e_file_max_size];
	};
#pragma pack(pop)

	//未测试
	class MyLogHandleFile_ : public _MyLogHandleBasic
	{
	private:
		TxSysProcessMemory mFileInfoMemObj;
		TxSysProcessMemory mLogContentMemObj;
		tagMyFileInfoMemInfo_ *pFileInfoMemInfo;
		tagMyLogContentMemInfo_ *pLogContentMemInfo;
		TxSysProcessMutex<false> mEnterLocker;
		TxSysProcessMutex<true> mGlobalLocker;
	public:
		MyLogHandleFile_() :mEnterLocker(TxStdlib::calcMd5(_log_filepath_().utf8Codecvt() + "log_handle_file_enter_locker")),
			mGlobalLocker(TxStdlib::calcMd5(_log_filepath_().utf8Codecvt() + "log_handle_file_global_locker"))
		{
			this->mGlobalLocker.lock();
			this->pFileInfoMemInfo = NULL;
			this->pLogContentMemInfo = NULL;
			bool lc_bCreateSign = false;
			this->mLogContentMemObj.initialize(&lc_bCreateSign,
				TxStdlib::calcMd5(_log_filepath_().utf8Codecvt() + "log_handle_file_log_info_mem").c_str(), sizeof(*this->pLogContentMemInfo));
			this->pLogContentMemInfo = (tagMyLogContentMemInfo_*)this->mLogContentMemObj.getMemoryPtr();
			if (lc_bCreateSign)
			{
				::memset(this->pLogContentMemInfo, 0, sizeof(*this->pLogContentMemInfo));
				this->pLogContentMemInfo->contentLen = 0;
			}
			this->mGlobalLocker.unlock();
		}
		~MyLogHandleFile_()
		{
			this->mGlobalLocker.lock();
			if (this->pFileInfoMemInfo != NULL)
			{
				--this->pFileInfoMemInfo->refCount;
				assert(this->pFileInfoMemInfo->refCount >= 0);
				if (this->pFileInfoMemInfo->refCount == 0 && this->pFileInfoMemInfo->logFp != NULL)
				{
					::fclose(this->pFileInfoMemInfo->logFp);
					this->pFileInfoMemInfo->logFp = NULL;
				}
				this->mFileInfoMemObj.uninitialize();
				this->mEnterLocker.unlock();
			}
			this->mGlobalLocker.unlock();
		}
		static TxBlobWString _logTextPrefix_(EtLogLevel _logLevel)
		{
			TxBlobWString ret;
			//SYSTEMTIME lcCurTime = { 0 };
			//::GetLocalTime(&lcCurTime);
			//ret.appendFormat(L"[%04d-%02d-%02d %02d:%02d:%02d] ",
			//	(int)lcCurTime.wYear,
			//	(int)lcCurTime.wMonth,
			//	(int)lcCurTime.wDay,
			//	(int)lcCurTime.wHour,
			//	(int)lcCurTime.wMinute,
			//	(int)lcCurTime.wSecond);
			ret += L"{";
			if (_logLevel == ELL_info) ret += TxBlobWString(L"info ");
			else if (_logLevel == ELL_warn) ret += TxBlobWString(L"warn ");
			else if (_logLevel == ELL_error) ret += TxBlobWString(L"error");
			else assert(false);
			ret += L"} ";
			return ret;
		}
		void writeLogFile_unsafe_(FILE *&_fp, const unsigned char *_log_text, int _log_len)
		{
			if (_fp == NULL)
			{
				_fp = ::_wfopen(this->_log_filepath_().c_str(), L"ab+");
				this->pFileInfoMemInfo->fileSize = 0;
				if (_fp == NULL)
				{
					_Def_tx_sysMessageBoxDebugInfo___(L"_wfopen调用失败");
				}
				else
				{
					::_fseeki64(_fp, 0, SEEK_END);
					this->pFileInfoMemInfo->fileSize
						= (std::uint32_t)::_ftelli64(this->pFileInfoMemInfo->logFp);
				}
			}
			if (_fp == NULL)
			{
				this->writeLogMemory_unsafe_(_log_text, _log_len);
			}
			else
			{
				if (this->pLogContentMemInfo->contentLen > 0)
				{
					this->writeLogFile_unsafe_(_fp, this->pLogContentMemInfo->logContent, this->pLogContentMemInfo->contentLen);
					this->pLogContentMemInfo->contentLen = 0;
				}
				this->pFileInfoMemInfo->fileSize += (int)_log_len;
				::fwrite(_log_text, 1, _log_len, _fp);
				::fflush(_fp);
				if (this->pFileInfoMemInfo->fileSize > tagMyLogContentMemInfo_::e_file_max_size * 3 / 2 || this->pFileInfoMemInfo->fileSize < 0)
				{
					::_fseeki64(_fp, -(int)tagMyLogContentMemInfo_::e_file_max_size, SEEK_END);
					std::vector<unsigned char> lcVctBuffer;
					lcVctBuffer.resize(tagMyLogContentMemInfo_::e_file_max_size);
					int iLcSize = (int)::fread(lcVctBuffer.data(), 1, lcVctBuffer.size(), _fp);
					if ((int)lcVctBuffer.size() > iLcSize&&iLcSize >= 0)
					{
						assert(false);
						lcVctBuffer.resize(iLcSize, true);
					}
					::fclose(_fp);
					_fp = NULL;
					this->pFileInfoMemInfo->fileSize = 0;
					if (!::DeleteFileW(this->_log_filepath_().c_str()))
						assert(false);
					this->writeLogFile_unsafe_(_fp, lcVctBuffer.data(), (int)lcVctBuffer.size());
				}
			}
		}
		void writeLogMemory_unsafe_(const unsigned char *_log_text, int _log_len)
		{
			if ((int)(this->pLogContentMemInfo->contentLen) + _log_len <= TxStdlib::arrayLength(this->pLogContentMemInfo->logContent))
			{
				memcpy(&this->pLogContentMemInfo->logContent[this->pLogContentMemInfo->contentLen], _log_text, _log_len);
				this->pLogContentMemInfo->contentLen += _log_len;
			}
		}
		static TxBlobWString _log_filepath_()
		{
			const wchar_t *lc_procname = NULL;
			TxBlobWString ret(_TxPrivateNs_::CSyslibBase::getWorkingDirectory(&lc_procname, NULL));
			return ret + lc_procname + L".log";
		}
		virtual void vfWrite(const TxBlobWString &_text, _MyLogHandleBasic::EtLogLevel _logLevel)
		{
			TxBlobWString lc_text(_text);
			lc_text.replaceSelf(L'。', L'\r');
			lc_text.replaceSelf(L'。', L'\n');
			TxBlobString lc_utf8_text = (this->_logTextPrefix_(_logLevel) + lc_text + L"\r\n").utf8Codecvt();
			this->mGlobalLocker.lock();
			if (this->pFileInfoMemInfo == NULL)
			{
				if (this->mEnterLocker.try_lock())
				{
					bool lc_bCreateSign = false;
					this->mFileInfoMemObj.initialize(&lc_bCreateSign,
						TxStdlib::calcMd5(_log_filepath_().utf8Codecvt() + "_file_info_mem").c_str(), sizeof(*this->pFileInfoMemInfo));
					this->pFileInfoMemInfo = (tagMyFileInfoMemInfo_*)this->mFileInfoMemObj.getMemoryPtr();
					if (lc_bCreateSign)
					{
						::memset(this->pFileInfoMemInfo, 0, sizeof(*this->pFileInfoMemInfo));
						this->pFileInfoMemInfo->logFp = NULL;
						this->pFileInfoMemInfo->fileSize = 0;
						this->pFileInfoMemInfo->refCount = 0;
					}
					++this->pFileInfoMemInfo->refCount;
				}
			}
			if (this->pFileInfoMemInfo != NULL)
				this->writeLogFile_unsafe_(this->pFileInfoMemInfo->logFp, (const unsigned char*)lc_utf8_text.data(), (int)lc_utf8_text.size());
			else this->writeLogMemory_unsafe_((const unsigned char*)lc_utf8_text.data(), (int)lc_utf8_text.size());
			this->mGlobalLocker.unlock();
		}
	};
}
class TxSysLog :public TxSingletonTmpl<TxSysLog>
{
public:
	typedef _TxPrivateNs_::_MyLogHandleBasic ILogOutput;
private:
	std::vector<std::shared_ptr<ILogOutput>> vctLog;
	TxBlobWString wsCurrentProcessId;
	long long llInitTime;
	long long llInitCount;
private:
	void _constructor_fn_()
	{
		this->llInitCount = 0;
		_TxPrivateNs_::CSyslibBase::getWorkingDirectory(NULL, NULL);
		this->llInitTime = _TxPrivateNs_::CSyslibBase::getHighPrecisionMicrosecond(NULL);
		this->wsCurrentProcessId.format(L"%5d", (int)::GetCurrentProcessId());
	}
	void _destructor_fn_()
	{
		this->vctLog.clear();
	}
	inline static void _assert_messagebox_(const wchar_t * _lpszText = NULL)
	{
		TX_UNUSED(_lpszText);
#ifndef NDEBUG
		static BOOL bbbbb = TRUE;
		if (bbbbb)
		{
			bbbbb = FALSE;
			TxBlobWString lc_wtext;
			lc_wtext.compose(L"[{0}:{1},{2}]{3}\r\nerror={4}\r\n").args(__FILEW__, __LINE__, __FUNCTIONW__,
				(_lpszText == NULL ? L"" : _lpszText), _TxPrivateNs_::CSyslibBase::sysLastErrorInfo());
			::MessageBoxW(NULL, lc_wtext.c_str(), L"error", MB_OK);
			::OutputDebugStringW(lc_wtext.c_str());
		}
#endif
	}
	void write_log_(const TxBlobWString &_log, ILogOutput::EtLogLevel _logLevel)
	{
		if (_logLevel >= ILogOutput::ELL_error)//assert断言
			this->_assert_messagebox_(_log.c_str());
		for (int i = (int)this->vctLog.size(); (--i) >= 0;)
			this->vctLog[i]->vfWrite(_log, _logLevel);
	}
	static TxBlobWString _getCurrentTimeString_()
	{
		SYSTEMTIME lcTime = { 0 };
		::GetLocalTime(&lcTime);
		char lc_ret[128] = { 0 };
		::sprintf_s(lc_ret, "%04d-%02d-%02d %02d:%02d:%02d.%03d", (int)lcTime.wYear, (int)lcTime.wMonth, (int)lcTime.wDay,
			(int)lcTime.wHour, (int)lcTime.wMinute, (int)lcTime.wSecond, (int)lcTime.wMilliseconds);
		return TxBlobString(lc_ret).utf8Codecvt();
	}
protected:
	TxSysLog(const TxSysLog&)
	{
		this->_constructor_fn_();
	}
	TxSysLog()
	{
		this->_constructor_fn_();
	}
	~TxSysLog()
	{
		this->_destructor_fn_();
	}
public:
	static void initialize(bool _enableDebugOutput = true, bool _enableFileLog = true, bool _enableSysLog = true)
	{
		__super::initialize();
		TxSysLog *pThis = object();
		if ((++pThis->llInitCount) == 1)
		{
			if (_enableDebugOutput)
				pThis->vctLog.push_back(std::make_shared<_TxPrivateNs_::_MyLogDebugOutputWrap>());
			if (_enableFileLog)
				pThis->vctLog.push_back(std::make_shared<_TxPrivateNs_::MyLogHandleFile_>());
			if (_enableSysLog)
				pThis->vctLog.push_back(std::make_shared<_TxPrivateNs_::_MyWndLogWrap_>());
		}
	}
	static TxBlobWString::TxComposeParameter syslog_input(const TxBlobWString &_fmt,
		const wchar_t *_func, ILogOutput::EtLogLevel _logLevel)
	{
		TxBlobWString lc_log_text;
		TxSysLog *pThis = object();
		if (pThis == NULL)
		{
			_assert_messagebox_();
			return TxBlobWString().compose(L"");
		}
		lc_log_text.compose(L"[ProcessID={0}, TIME={1}, FUNCTION={2}]\n").args(
			pThis->wsCurrentProcessId, _getCurrentTimeString_(), _func);

		if ((unsigned int)_logLevel >= (unsigned int)ILogOutput::ELL_warn)
			lc_log_text.appendCompose(L"[GetLastError()=[{0}], errno={1}]\n").args(::GetLastError(), errno);

		return TxBlobWString::compose(_fmt, [lc_log_text, _logLevel](const TxBlobWString &_log_content) {
			TxSysLog *pThis = object();
			if (pThis == NULL)
				_assert_messagebox_();
			else pThis->write_log_(lc_log_text + _log_content, _logLevel);
		});
	}
	static void syslog_applicationInformations(const wchar_t *_func)
	{
		TxSysLog *pThis = object();
		if (pThis == NULL)
		{
			_assert_messagebox_();
			return;
		}

		TxBlobWString lc_log_text;

		//代码信息
		lc_log_text.appendCompose(L"[ProcessID={0}, TIME={1}, FUNCTION={2}]\n").args(
			pThis->wsCurrentProcessId, _getCurrentTimeString_(), _func);

		//运行时间以及进程ID
		lc_log_text.appendCompose(L"程序运行时间：{0}秒\n").args(
			(long long)((_TxPrivateNs_::CSyslibBase::getHighPrecisionMicrosecond(NULL) - pThis->llInitTime) / (1e6)));

		//LocalTime时间信息
		SYSTEMTIME lcCurLocalTime = { 0 };
		::GetLocalTime(&lcCurLocalTime);
		lc_log_text.appendCompose(L"Local Time: [{0}/{1}/{2} {3}:{4}:{5}.{6}]\n").args(
			lcCurLocalTime.wYear, lcCurLocalTime.wMonth, lcCurLocalTime.wDay,
			lcCurLocalTime.wHour, lcCurLocalTime.wMinute, lcCurLocalTime.wSecond, lcCurLocalTime.wMilliseconds);

		//SystemTime时间信息
		SYSTEMTIME lcCurSystemTime = { 0 };
		::GetSystemTime(&lcCurSystemTime);
		lc_log_text.appendCompose(L"System Time: [{0}/{1}/{2} {3}:{4}:{5}.{6}]\n").args(
			lcCurSystemTime.wYear, lcCurSystemTime.wMonth, lcCurSystemTime.wDay,
			lcCurSystemTime.wHour, lcCurSystemTime.wMinute, lcCurSystemTime.wSecond, lcCurSystemTime.wMilliseconds);

		//程序内存
		PROCESS_MEMORY_COUNTERS lc_pmc = { 0 };
		if (!::GetProcessMemoryInfo(::GetCurrentProcess(), &lc_pmc, sizeof(lc_pmc)))
			_assert_messagebox_();
		else
		{
			lc_log_text.appendCompose(L"程序内存：当前内存[{0}]，内存峰值[{1}]. \n").args(
				lc_pmc.WorkingSetSize, lc_pmc.PeakWorkingSetSize);
		}

		//系统内存
		long long lc_totalMemSize = 0, lc_usedMemSize = 0;
		if (!_TxPrivateNs_::CSyslibBase::getSystemMemoryStatus(&lc_totalMemSize, &lc_usedMemSize, NULL))
			_assert_messagebox_();
		else
		{
			lc_log_text.appendCompose(L"系统内存：总内存[{0}]，剩余内存[{1}]\n").args(lc_totalMemSize, lc_usedMemSize);
		}

		//磁盘空间大小
		lc_log_text.appendCompose(L"磁盘空间信息：");
		for (wchar_t disk_ch = L'C'; disk_ch <= L'Z'; ++disk_ch)
		{
			const wchar_t lc_dir[4] = { disk_ch, L':', 0, 0 };
			ULARGE_INTEGER lcFreeBytesAvailableToCaller = { 0 }, lcTotalNumberOfBytes = { 0 }, lcTotalNumberOfFreeBytes = { 0 };
			if (::GetDiskFreeSpaceExW(lc_dir,
				&lcFreeBytesAvailableToCaller, &lcTotalNumberOfBytes, &lcTotalNumberOfFreeBytes))
			{
				if (lcTotalNumberOfFreeBytes.QuadPart < 0 || lcFreeBytesAvailableToCaller.QuadPart != lcTotalNumberOfFreeBytes.QuadPart)
					_assert_messagebox_();
				else
				{
					lc_log_text.appendCompose(L"[{0}]用户可用的磁盘空间：{1}，磁盘总共的空间：{2}，磁盘空闲的空间：{3}. ").args(lc_dir,
						lcFreeBytesAvailableToCaller.QuadPart, lcTotalNumberOfBytes.QuadPart, lcTotalNumberOfFreeBytes.QuadPart);
				}
			}
		}
		lc_log_text.appendCompose(L"\n");

		//写日志
		pThis->write_log_(lc_log_text, ILogOutput::ELL_info);
	}
};

#define defTxSysLog____(_logLevel,_fmt) TxSysLog::syslog_input((_fmt),__FUNCTIONW__,_logLevel)
#define sysLogInfoW(_fmt) defTxSysLog____(TxSysLog::ILogOutput::ELL_info,(_fmt))
#define sysLogWarnW(_fmt) defTxSysLog____(TxSysLog::ILogOutput::ELL_warn,(_fmt))
#define sysLogErrorW(_fmt) defTxSysLog____(TxSysLog::ILogOutput::ELL_error,(_fmt))
#define sysLogInfo(_fmt) sysLogInfoW(L##_fmt)
#define sysLogWarn(_fmt) sysLogWarnW(L##_fmt)
#define sysLogError(_fmt) sysLogErrorW(L##_fmt)
#define sysLogAppHealth() TxSysLog::syslog_applicationInformations(__FUNCTIONW__)


//--------------------------------------CSyslib--------------------------------------//
namespace _TxPrivateNs_ {
	class CSyslib : public CSyslibBase
	{
	public:
		static void __syslog_err_func_(const wchar_t *_text, const wchar_t *_func)
		{
			TxSysLog::syslog_input(_text, _func, TxSysLog::ILogOutput::ELL_error);
		}
	public:
		static const wchar_t* getWorkingDirectory(const wchar_t **_procName = NULL)
		{
			return __super::getWorkingDirectory(_procName, &__syslog_err_func_);
		}
		static long long getHighPrecisionMicrosecond()
		{
			return __super::getHighPrecisionMicrosecond(&__syslog_err_func_);
		}
		static bool getSystemMemoryStatus(long long *_totalMemSize, long long *_usedMemSize)
		{
			return __super::getSystemMemoryStatus(_totalMemSize, _usedMemSize, &__syslog_err_func_);
		}
		static bool createDirectories(const TxBlobWString &_wdir)
		{
			DWORD lc_fileAttr = ::GetFileAttributesW(_wdir.c_str());
			if (lc_fileAttr != INVALID_FILE_ATTRIBUTES)
			{
				if (lc_fileAttr&FILE_ATTRIBUTE_DIRECTORY)//文件夹
					return true;
				else//文件
					::DeleteFileW(_wdir.c_str());
			}
			int lc_err = ::SHCreateDirectoryExW(NULL, _wdir.c_str(), NULL);
			return lc_err == ERROR_SUCCESS;
		}
		template<class TFUNC>
		static void ergodicProcessSnapshot(const TFUNC &_func)
		{
			HANDLE lc_hProcess = { 0 };
			PROCESSENTRY32W lc_pe = { 0 };
			lc_pe.dwSize = sizeof(PROCESSENTRY32);
			HANDLE lc_hShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			DWORD dwCurPgsId = ::GetCurrentProcessId();
			if (::Process32FirstW(lc_hShot, &lc_pe))
			{
				do {
					_func(lc_pe);
				} while (::Process32NextW(lc_hShot, &lc_pe));
			}
			::CloseHandle(lc_hShot);
		}
		static int killBrotherProcess()
		{
			int ret = 0;
			const wchar_t *lc_exe_name_ptr = NULL;
			CSyslib::getWorkingDirectory(&lc_exe_name_ptr);
			TxBlobWString lc_exe_name(lc_exe_name_ptr);
			DWORD lc_dwCurPgsId = ::GetCurrentProcessId();
			ergodicProcessSnapshot([&lc_exe_name, lc_dwCurPgsId, &ret](const PROCESSENTRY32W &_pe) {
				if (lc_exe_name.equalNoCase(_pe.szExeFile) && _pe.th32ProcessID != lc_dwCurPgsId)
				{
					HANDLE lc_hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, _pe.th32ProcessID);
					if (!::TerminateProcess(lc_hProcess, 0))
						sysLogError("if (!::TerminateProcess(lc_hProcess, 0))调用失败。");
					::WaitForSingleObject(lc_hProcess, INFINITE);
					++ret;
				}
			});
			return ret;
		}
		static int waitBrotherProcessFinish(int _waitMsec = -1)
		{
			int ret = 0;
			const wchar_t *lc_exe_name_ptr = NULL;
			CSyslib::getWorkingDirectory(&lc_exe_name_ptr);
			TxBlobWString lc_exe_name(lc_exe_name_ptr);
			DWORD lc_dwCurPgsId = ::GetCurrentProcessId();
			ergodicProcessSnapshot([&lc_exe_name, lc_dwCurPgsId, &ret, _waitMsec](const PROCESSENTRY32W &_pe) {
				if (lc_exe_name.equalNoCase(_pe.szExeFile) && _pe.th32ProcessID != lc_dwCurPgsId)
				{
					HANDLE lc_hProcess = ::OpenProcess(PROCESS_VM_OPERATION | SYNCHRONIZE, FALSE, _pe.th32ProcessID);
					::WaitForSingleObject(lc_hProcess, (_waitMsec <= 0 ? (DWORD)(INFINITE) : (DWORD)(_waitMsec)));
					::CloseHandle(lc_hProcess);
					++ret;
				}
			});
			return ret;
		}
		static int killProcessByExecName(const wchar_t *_wsExecName)
		{
			int ret = 0;
			TxBlobWString lc_exe_name(_wsExecName);
			ergodicProcessSnapshot([&lc_exe_name, &ret, _wsExecName](const PROCESSENTRY32W &_pe) {
				if (lc_exe_name.equalNoCase(_pe.szExeFile))
				{
					sysLogInfo("killProcessByExecName(),file=[{0}]").args(_wsExecName);
					HANDLE lc_hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, _pe.th32ProcessID);
					if (!::TerminateProcess(lc_hProcess, 0))
						sysLogError("if (!::TerminateProcess(lc_hProcess, 0))调用失败。");
					::WaitForSingleObject(lc_hProcess, INFINITE);
					++ret;
				}
			});
			return ret;
		}
		static bool killProcessByHwnd(HWND _hwnd)
		{
			bool ret = false;
			if (_hwnd == NULL)
				return ret;
			DWORD lc_dwProcessId = (DWORD)-1;
			::GetWindowThreadProcessId(_hwnd, &lc_dwProcessId);
			if (lc_dwProcessId == (DWORD)-1)
				return ret;

			ergodicProcessSnapshot([lc_dwProcessId, &ret, _hwnd](const PROCESSENTRY32W &_pe) {
				if (_pe.th32ProcessID == lc_dwProcessId)
				{
					::SendMessageW(_hwnd, WM_QUIT, 0, 0);
					HANDLE lc_hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, _pe.th32ProcessID);
					if (WAIT_OBJECT_0 != ::WaitForSingleObject(lc_hProcess, 10 * 1000))
					{
						if (!::TerminateProcess(lc_hProcess, 0))
							sysLogError("if (!::TerminateProcess(lc_hProcess, 0))调用失败。");
						::WaitForSingleObject(lc_hProcess, INFINITE);
					}
					ret = true;
				}
			});

			return ret;
		}
		static bool createProcess(const wchar_t *_file, unsigned int _SW_FLAG = SW_SHOW, const wchar_t *_argstr = NULL,
			bool _bWaitFinish = false, unsigned long _dwCreationFlags = 0,
			const std::function<void(const PROCESS_INFORMATION&)> &_fn = std::function<void(const PROCESS_INFORMATION&)>())
		{
			std::wstring lc_ws_file(_file);
			std::wstring lc_ws_cur_dir;
			std::wstring lc_ws_args(_argstr == NULL || _argstr[0] == 0 ? std::wstring() : (std::wstring(L" ") + _argstr));
			for (int i = 0; i < (int)lc_ws_file.size(); ++i)
			{
				if (lc_ws_file[i] == L'/')
					lc_ws_file[i] = L'\\';
			}
			wchar_t lc_name_buffer[FILENAME_MAX * 2 + 8] = { 0 };
			wchar_t *lc_file_name = NULL;
			::GetFullPathNameW(_file, sizeof(lc_name_buffer) / sizeof(lc_name_buffer[0]) - 1, lc_name_buffer, &lc_file_name);
			lc_ws_file.assign(lc_name_buffer);
			if (lc_file_name != NULL)
				*lc_file_name = 0;
			lc_ws_cur_dir.assign(lc_name_buffer);
			if (lc_ws_cur_dir.length() == 0)
				lc_ws_cur_dir = L".";

			STARTUPINFOW lc_si = { 0 };
			lc_si.cb = sizeof(lc_si);
			lc_si.dwFlags = STARTF_USESHOWWINDOW; //enact wShowWindow
			lc_si.wShowWindow = (WORD)_SW_FLAG; //SW_SHOW or SW_HIDE or SW_SHOWNORMAL etc
			PROCESS_INFORMATION lc_pi = { 0 };
			if (::CreateProcessW(lc_ws_file.c_str(), const_cast<wchar_t*>((TxBlobWString(L"\"") + lc_ws_file + L"\" " + lc_ws_args).c_str()),
				NULL, NULL, FALSE, (DWORD)_dwCreationFlags, NULL, lc_ws_cur_dir.c_str(), &lc_si, &lc_pi))
			{
				if (_fn)
					_fn(lc_pi);
				if (_bWaitFinish)
					::WaitForSingleObject(lc_pi.hThread, INFINITE);
				::CloseHandle(lc_pi.hThread);
				::CloseHandle(lc_pi.hProcess);
				return true;
			}
			return false;
		}
		static void sleep_msec(int _msec)
		{
			::Sleep((DWORD)_msec);
		}
		static TxBlobString generateGuid()
		{
			GUID lc_guid = { 0 };
			struct HEX_
			{
				unsigned char str[(2 * sizeof(lc_guid)) + 32];
				unsigned char *pos;
				inline void push_hex(unsigned long _data) {
					*(this->pos++) = (_data >= 10 ? (char)(_data + ('A' - 10)) : (char)(_data + '0'));
				}
				inline void push_char(char _ch) {
					*(this->pos++) = _ch;
				}
			} m_ret = { 0 };
			m_ret.pos = m_ret.str;
			if (::CoCreateGuid(&lc_guid) != S_OK)
				sysLogError("CoCreateGuid()调用失败");
			else
			{
				for (int i = 32; (i -= 4) >= 0;)
					m_ret.push_hex((lc_guid.Data1 >> i) & 0xf);
				m_ret.push_char('_');
				for (int i = 16; (i -= 4) >= 0;)
					m_ret.push_hex((lc_guid.Data2 >> i) & 0xf);
				m_ret.push_char('_');
				for (int i = 16; (i -= 4) >= 0;)
					m_ret.push_hex((lc_guid.Data3 >> i) & 0xf);
				m_ret.push_char('_');
				for (int i = 0; i < sizeof(lc_guid.Data4) / sizeof(lc_guid.Data4[0]); ++i)
				{
					if (i == 2)
						m_ret.push_char('_');
					m_ret.push_hex((lc_guid.Data4[i] >> 4) & 0xf);
					m_ret.push_hex((lc_guid.Data4[i] >> 0) & 0xf);
				}
				m_ret.push_char('\0');
				if ((size_t)(m_ret.pos - &m_ret.str[0]) >= sizeof(m_ret.str) / sizeof(m_ret.str[0]))
					sysLogError("(size_t)(m_ret.pos - &m_ret.str[0]) >= sizeof(m_ret.str) / sizeof(m_ret.str[0])");
			}
			return TxBlobString((char*)m_ret.str);
		}
		static std::list<TxBlobWString> killProgramByDirectory(const TxBlobWString &_directory,
			const std::set<DWORD> &_dwEliminateProcessId = std::set<DWORD>())
		{
			enum { e_file_name_size = ((MAX_PATH) | (FILENAME_MAX)) + 64, };
			DWORD lc_aPid[1024] = { 0 };
			DWORD lc_cbNeeded = 0;
			std::list<TxBlobWString> ret;
			if (!::EnumProcesses(lc_aPid, sizeof(lc_aPid), &lc_cbNeeded))
			{
				sysLogError("EnumProcesses()调用失败。");
				return ret;
			}
			sysLogInfo("killProgramByDirectory(),_directory=[{0}]").args(_directory);
			std::vector<wchar_t> lc_vct_full_path;
			lc_vct_full_path.resize(e_file_name_size);
			auto fnGetAbsPath = [&lc_vct_full_path](const wchar_t *_path)->TxBlobWString {
				std::memset(lc_vct_full_path.data(), 0, lc_vct_full_path.size() * sizeof(lc_vct_full_path.data()[0]));
				::_wfullpath(lc_vct_full_path.data(), _path, lc_vct_full_path.size() - 8);
				TxBlobWString ret = TxBlobWString(lc_vct_full_path.data()).trimLR(L"\r\n\t \\/");
				ret.lowerCaseSelf();
				ret.replaceSelf(L'\\', L'/');
				return ret;
			};
			TxBlobWString lc_match_basic_directory = fnGetAbsPath(_directory.c_str());
			if (lc_match_basic_directory.backChar() != L'\\')
				lc_match_basic_directory.append(L"\\");
			for (int i = 0; i < (int)(lc_cbNeeded / sizeof(DWORD)); i++)
			{
				if (_dwEliminateProcessId.find(lc_aPid[i]) != _dwEliminateProcessId.end())
					continue;
				HANDLE lc_hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, lc_aPid[i]);
				if (lc_hProcess != NULL && lc_hProcess != INVALID_HANDLE_VALUE)
				{
					wchar_t lc_wsModName[e_file_name_size] = { 0 };
					if (::GetModuleFileNameExW(lc_hProcess, NULL, lc_wsModName, e_file_name_size - 8) > 0)
					{
						TxBlobWString lc_good_file_path = fnGetAbsPath(lc_wsModName);
						if (std::wcsstr(lc_good_file_path.c_str(), lc_match_basic_directory.c_str()) != NULL)
						{
							ret.push_back(lc_good_file_path);
							if (!::TerminateProcess(lc_hProcess, 0))
								sysLogError("if (!::TerminateProcess(lc_hProcess, 0))调用失败。");
							::WaitForSingleObject(lc_hProcess, INFINITE);
						}
					}
					if (!::CloseHandle(lc_hProcess))
						sysLogError("if (!::CloseHandle(lc_hProcess))调用失败。");
				}
			}
			return ret;
		}
		static int detectProgram(const std::list<TxBlobWString> &_wsProgramPathList, bool _bKill = false)
		{
			enum { e_file_name_size = ((MAX_PATH) | (FILENAME_MAX)) + 64, };
			DWORD lc_aPid[1024] = { 0 };
			DWORD lc_cbNeeded = 0;
			int ret = 0;
			if (!::EnumProcesses(lc_aPid, sizeof(lc_aPid), &lc_cbNeeded))
			{
				sysLogError("if (!::EnumProcesses(lc_aPid, sizeof(lc_aPid), &lc_cbNeeded))调用失败。");
				return ret;
			}
			std::vector<wchar_t> lc_vct_full_path;
			lc_vct_full_path.resize(e_file_name_size);
			auto fnGetAbsPath = [&lc_vct_full_path](const wchar_t *_path)->TxBlobWString {
				std::memset(lc_vct_full_path.data(), 0, lc_vct_full_path.size() * sizeof(lc_vct_full_path.data()[0]));
				::_wfullpath(lc_vct_full_path.data(), _path, lc_vct_full_path.size() - 8);
				TxBlobWString ret = TxBlobWString(lc_vct_full_path.data()).trimLR(L"\r\n\t \\/");
				ret.lowerCaseSelf();
				ret.replaceSelf(L'\\', L'/');
				return ret;
			};
			std::set<TxBlobWString> lc_wsProgramPathSet;
			for (auto iter = _wsProgramPathList.cbegin(); iter != _wsProgramPathList.cend(); ++iter)
				lc_wsProgramPathSet.insert(fnGetAbsPath(iter->c_str()));
			for (int i = 0; i < (int)(lc_cbNeeded / sizeof(DWORD)); i++)
			{
				HANDLE lc_hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, lc_aPid[i]);
				if (lc_hProcess != NULL && lc_hProcess != INVALID_HANDLE_VALUE)
				{
					wchar_t lc_wsModName[e_file_name_size] = { 0 };
					if (::GetModuleFileNameExW(lc_hProcess, NULL, lc_wsModName, e_file_name_size - 8) > 0)
					{
						if (lc_wsProgramPathSet.find(fnGetAbsPath(lc_wsModName)) != lc_wsProgramPathSet.end())
						{
							if (_bKill)
							{
								sysLogInfo("TerminateProcess(),file=[{0}]").args(lc_wsModName);
								if (!::TerminateProcess(lc_hProcess, 0))
									sysLogError("if (!::TerminateProcess(lc_hProcess, 0))调用失败。");
								::WaitForSingleObject(lc_hProcess, INFINITE);
							}
							++ret;
						}
					}
					if (!::CloseHandle(lc_hProcess))
						sysLogError("if (!::CloseHandle(lc_hProcess))调用失败。");
				}
			}
			return ret;
		}
		static bool _switchPcPower__(DWORD _dwReason, int _delayMsec)
		{
			if (_delayMsec > 0)
			{//延后关机
				char lc_cmd[128] = { 0 };
				int lc_delaySecond = (int)TxStdlib::maximum<int>(1, (int)(_delayMsec / 1000.0 + 0.5));
				switch (_dwReason)
				{
				case EWX_REBOOT:
					sprintf(lc_cmd, "shutdown -r -t %d", (int)lc_delaySecond);
					break;
				case EWX_SHUTDOWN:
					sprintf(lc_cmd, "shutdown -s -t %d", (int)lc_delaySecond);
					break;
				case EWX_LOGOFF:
					sprintf(lc_cmd, "shutdown -l -t %d", (int)lc_delaySecond);
					break;
				default:
					sysLogError("无法识别参数：_dwReason={0}").args(_dwReason);
					return false;
					break;
				}
				if (lc_cmd[0] != 0)
					system(lc_cmd);
			}
			else
			{//立刻关机
				HANDLE hToken = NULL;
				TOKEN_PRIVILEGES lc_tkp = { 0 };
				if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
				{
					sysLogError("if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))调用失败。");
					return false;
				}
				if (!::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &lc_tkp.Privileges[0].Luid))
				{
					sysLogError("if (!::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &lc_tkp.Privileges[0].Luid))调用失败。");
					return false;
				}
				lc_tkp.PrivilegeCount = 1;   //  one  privilege  to  set 
				lc_tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
				if (!::AdjustTokenPrivileges(hToken, FALSE, &lc_tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
				{
					sysLogError("if (!::AdjustTokenPrivileges(hToken, FALSE, &lc_tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))调用失败。");
					return false;
				}
				if (!::ExitWindowsEx(_dwReason | EWX_FORCEIFHUNG, 0))
				{
					sysLogError("if (!::ExitWindowsEx(_dwReason | EWX_FORCEIFHUNG, 0))调用失败。_dwReason=[{0}]").args(_dwReason);
					return false;
				}
			}
			return true;
		}
		static bool rebootWindows(int _delayMsec) { return _switchPcPower__(EWX_REBOOT, _delayMsec); };
		static bool shutdownWindows(int _delayMsec) { return _switchPcPower__(EWX_SHUTDOWN, _delayMsec); };
		static bool logoffWindows(int _delayMsec) { return _switchPcPower__(EWX_LOGOFF, _delayMsec); };
		static std::vector<TxBlobWString> commandLineArgs()
		{
			std::vector<TxBlobWString> ret;
			wchar_t* lc_pszCmdLineW = ::GetCommandLineW();
			if (lc_pszCmdLineW == NULL)
			{
				sysLogError("lc_pszCmdLineW = ::GetCommandLineW()调用失败。lc_pszCmdLineW==NULL");
				lc_pszCmdLineW = L"";
			}
			int lc_argc = 0;
			wchar_t* *lc_argv = ::CommandLineToArgvW(lc_pszCmdLineW, &lc_argc);
			if (lc_argv == NULL || lc_argc <= 0)
			{
				sysLogError("wchar_t* *lc_argv = ::CommandLineToArgvW(lc_pszCmdLineW, &lc_argc)调用失败。lc_argv=[{0}],lc_argc=[{1}]").args(
					(size_t)lc_argv, lc_argc);
			}
			else
			{
				ret.resize(lc_argc);
				for (int i = 0; i < lc_argc; ++i)
					ret[i].assign(lc_argv[i]);
				if (NULL != ::LocalFree(lc_argv))
					sysLogError("if (NULL != ::LocalFree(lc_argv))调用失败。");
			}
			return ret;
		}
		static long long getDiskFreeSpace(const wchar_t _disk_ch)
		{
			long long ret = 0;
			const wchar_t lc_dir[] = { _disk_ch, L':', 0, 0 };
			ULARGE_INTEGER lcFreeBytesAvailableToCaller = { 0 };
			ULARGE_INTEGER lcTotalNumberOfBytes = { 0 };
			ULARGE_INTEGER lcTotalNumberOfFreeBytes = { 0 };
			if (!::GetDiskFreeSpaceExW(lc_dir, &lcFreeBytesAvailableToCaller, &lcTotalNumberOfBytes, &lcTotalNumberOfFreeBytes))
			{
				sysLogError("if (!::GetDiskFreeSpaceExW(lc_dir, &lcFreeBytesAvailableToCaller, &lcTotalNumberOfBytes, &lcTotalNumberOfFreeBytes))调用失败。");
			}
			else
			{
				if (lcFreeBytesAvailableToCaller.QuadPart != lcTotalNumberOfFreeBytes.QuadPart)
					sysLogError("if (lcFreeBytesAvailableToCaller.QuadPart != lcTotalNumberOfFreeBytes.QuadPart)");
				ret = (long long)lcFreeBytesAvailableToCaller.QuadPart;
				if (ret > (long long)lcTotalNumberOfFreeBytes.QuadPart)
					ret = (long long)lcTotalNumberOfFreeBytes.QuadPart;
				if (ret <= 0)
					sysLogError("if (ret <= 0)");
			}
			return ret;
		}
		static float calcSystemCpuOccupyRate()
		{
			class MyTimeInfoExt
			{
				struct tagTimeInfo {
					FILETIME idleTime;
					FILETIME kernelTime;
					FILETIME userTime;
					bool reloadTime() { return !!::GetSystemTimes(&this->idleTime, &this->kernelTime, &this->userTime); }
				};
			private:
				struct tagTimeInfo mRecordTimeInfo;
				TxSysMutex mMutex;
				std::time_t mBgnTime;
				float arryVal[8];
				std::size_t bInitSign;
			private:
				inline static long long gCompareFileTime(FILETIME time1, FILETIME time2)
				{
					long long val1 = ((long long)time1.dwHighDateTime << 32) | ((long long)time1.dwLowDateTime);
					long long val2 = ((long long)time2.dwHighDateTime << 32) | ((long long)time2.dwLowDateTime);
					return val1 - val2;
				}
			public:
				MyTimeInfoExt()
				{
					for (int i = 0; i < TxStdlib::arrayLength(this->arryVal); ++i)
						this->arryVal[i] = 0;
					this->bInitSign = !!this->mRecordTimeInfo.reloadTime();
					this->mBgnTime = ::time(NULL);
				}
				float calcCpuOccupyRate()
				{
					std::time_t lc_curTime_t = std::time(NULL);
					this->mMutex.lock();
					float ret = this->arryVal[4];
					if ((double)std::difftime(lc_curTime_t, this->mBgnTime) >= (double)0.99)
					{
						struct tagTimeInfo lc_curTime;
						if (lc_curTime.reloadTime())
						{
							if (!this->bInitSign)
								this->bInitSign = !!this->mRecordTimeInfo.reloadTime();
							else
							{
								long long lc_idle = gCompareFileTime(lc_curTime.idleTime, this->mRecordTimeInfo.idleTime);
								long long lc_kernel = gCompareFileTime(lc_curTime.kernelTime, this->mRecordTimeInfo.kernelTime);
								long long lc_user = gCompareFileTime(lc_curTime.userTime, this->mRecordTimeInfo.userTime);
								long long lc_kernel_user = lc_kernel + lc_user;
								if (lc_kernel_user > 0 && 0 <= lc_idle&&lc_idle <= lc_kernel_user)
								{
									ret = this->arryVal[4] = (lc_kernel_user - lc_idle) / (float)lc_kernel_user;
									this->mRecordTimeInfo = lc_curTime;
								}
							}
							this->mBgnTime = lc_curTime_t;
						}
					}
					this->mMutex.unlock();
					return TxStdlib::adjustRange(ret, 0.0f, 0.999999f);
				}
			};
			static MyTimeInfoExt lc_timeInfo;
			return lc_timeInfo.calcCpuOccupyRate();
		}
		static std::vector<RECT> allDisplayMonitorGeometry()
		{
			struct TT {
				std::vector<RECT> vctRect;
				static BOOL CALLBACK gMonitorEnumProc(HMONITOR _hMonitor, HDC _hdcMonitor, LPRECT _lprcMonitor, LPARAM _dwData)
				{
					TX_UNUSED(_hMonitor);
					TX_UNUSED(_hdcMonitor);
					TX_UNUSED(_lprcMonitor);
					TX_UNUSED(_dwData);
					if (_lprcMonitor != NULL)
					{
						if (_lprcMonitor->left >= 0 && _lprcMonitor->top >= 0
							&& _lprcMonitor->right > _lprcMonitor->left
							&&_lprcMonitor->bottom > _lprcMonitor->top)
						{
							((struct TT*)_dwData)->vctRect.push_back(*_lprcMonitor);
						}
					}
					return TRUE;
				}
			} mm;
			if (!::EnumDisplayMonitors(NULL, NULL, &TT::gMonitorEnumProc, (LPARAM)&mm))
			{
				sysLogError("遍历显示屏失败。125656812");
			}
			if (mm.vctRect.size() <= 0)
			{
				RECT lc_rect = { 0 };
				lc_rect.right = ::GetSystemMetrics(SM_CXSCREEN);
				lc_rect.bottom = ::GetSystemMetrics(SM_CYSCREEN);
				if (lc_rect.right > lc_rect.left&&lc_rect.bottom > lc_rect.top)
					mm.vctRect.push_back(lc_rect);
			}
			return mm.vctRect;
		}
		static bool isForegroundFullWindow(HWND *_pHwnd = NULL, RECT *_rect = NULL)
		{
			if (_pHwnd != NULL)
				*_pHwnd = NULL;
			std::vector<RECT> lc_vctDisplay = allDisplayMonitorGeometry();
			for (HWND lc_wnd = ::GetForegroundWindow(); lc_wnd != NULL; lc_wnd = ::GetParent(lc_wnd))
			{
				RECT lc_wnd_rect = { 0 };
				if (::GetWindowRect(lc_wnd, &lc_wnd_rect))
				{
					for (auto iter = lc_vctDisplay.begin(); iter != lc_vctDisplay.end(); ++iter)
					{
						if (TxStdlib::absolute(lc_wnd_rect.left - iter->left) <= 1
							&& TxStdlib::absolute(lc_wnd_rect.right - iter->right) <= 1
							&& TxStdlib::absolute(lc_wnd_rect.top - iter->top) <= 1
							&& TxStdlib::absolute(lc_wnd_rect.bottom - iter->bottom) <= 1)
						{
							if (iter->right > iter->left&&iter->bottom > iter->top)
							{
								if (_pHwnd != NULL)
									*_pHwnd = lc_wnd;
								if (_rect != NULL)
									*_rect = *iter;
								return true;
							}
						}
					}
				}
				else
				{
					sysLogError("获取窗口位置失败。112233889888");
				}
			}
			return false;
		}
		//_csidl=CSIDL_DESKTOPDIRECTORY
		static TxBlobWString getSystemSpecialDirectory(int _csidl)
		{
			LPITEMIDLIST lc_ppidl = NULL;
			wchar_t lpsbuf[((FILENAME_MAX) | (FILENAME_MAX)) + 64] = { 0 };
			if (S_OK != ::SHGetSpecialFolderLocation(NULL, _csidl, &lc_ppidl))
				sysLogError("if (S_OK != ::SHGetSpecialFolderLocation(NULL, _csidl, &lc_ppidl))调用失败。");
			if (!::SHGetPathFromIDListW(lc_ppidl, lpsbuf))
				sysLogError("if (!::SHGetPathFromIDListW(lc_ppidl, lpsbuf))调用失败。");
			TxBlobWString ret(lpsbuf);
			ret.replaceSelf(L'\\', L'/');
			if (ret.length() > 0 && ret.backChar() != L'\\')
				ret.append(L"\\");
			return ret;
		}
		static bool writeFile(const wchar_t _filename[], long long _llpos, const void *_buff, int _buffsize)
		{
			bool ret = false;
			HANDLE pfile = INVALID_HANDLE_VALUE;
			for (BOOL bfstiii = TRUE;; bfstiii = FALSE)
			{
				pfile = ::CreateFileW(
					_filename,
					GENERIC_WRITE,
					FILE_SHARE_WRITE,
					NULL,
					OPEN_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL
				);
				if (pfile != INVALID_HANDLE_VALUE || !bfstiii)
					break;
				TxBlobWString lc_wstrfile = _filename;
				lc_wstrfile.replaceSelf(L'\\', L'/');
				while (lc_wstrfile.length() > 0 && lc_wstrfile.backChar() != L'\\')
					lc_wstrfile.pop_back(1);
				if (!::PathFileExistsW(lc_wstrfile.c_str()))
					::SHCreateDirectory(NULL, lc_wstrfile.c_str());
			}
			if (pfile == INVALID_HANDLE_VALUE || pfile == NULL)
				sysLogError("CreateFileW()调用失败。_filename=[{0}]。").args(_filename);
			else
			{
				if (_buffsize > 0)
				{
					DWORD dwCount = 0;
					OVERLAPPED lv_ovlpd = { 0 };
					lv_ovlpd.Offset = (DWORD)_llpos;
					lv_ovlpd.OffsetHigh = (DWORD)(((unsigned long long)_llpos) >> (8 * sizeof(DWORD)));
					if (::WriteFile(pfile, _buff, (DWORD)_buffsize, &dwCount, &lv_ovlpd) && dwCount >= 0 && dwCount == (DWORD)_buffsize)
					{
						ret = true;
					}
					else
					{
						sysLogError("if (::WriteFile()调用失败。_filename=[{0}]").args(_filename);
					}
				}
				if (!::CloseHandle(pfile))
					sysLogError("if (!::CloseHandle(pfile))调用失败。_filename=[{0}]").args(_filename);
			}
			return ret;
		}

		//static FILE* wFileOpen(const TxBlobWString &_file, bool _bWrite)
		//{
		//	FILE *ret = ::_wfopen(_file.c_str(), _bWrite ? L"wb+" : L"rb");
		//	if (ret == NULL)
		//		sysLogError("_wfopen()调用失败。_file=[{0}],_bWrite=[{1}]").args(_file, _bWrite);
		//	return ret;
		//}
		static FILE* wFileOpen(const TxBlobWString &_file, const char *_mode, bool _bAssertLog = true)
		{
			wchar_t lc_mode[64] = { 0 };
			for (int i = 0; i < (sizeof(_mode) / (sizeof(_mode[0]))) - 1 && _mode[i] != 0; ++i)
				lc_mode[i] = _mode[i];
			FILE *ret = ::_wfopen(_file.c_str(), lc_mode);
			if (ret == NULL&&_bAssertLog)
				sysLogError("_wfopen()调用失败。_file=[{0}],_bWrite=[{1}]").args(_file, _mode);
			return ret;
		}

		static bool readFile(void *_buff, int *_readsize, long long *_llTotalSize, const wchar_t *_filename, long long _llpos, int _buffsize)
		{
			if (_filename == NULL || _llpos < 0 || _buffsize < 0)
			{
				sysLogError("参数错误。_filename=[{0}],_llpos=[{1}], _buffsize=[{2}]").args(
					(_filename == NULL ? L"" : _filename), _llpos, _buffsize);
				return false;
			}
			long long _lc_ll_tem = 0;
			if (_llTotalSize == NULL)
				_llTotalSize = &_lc_ll_tem;
			HANDLE pfile = ::CreateFileW(
				_filename,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);
			bool ret = false;
			if (pfile == INVALID_HANDLE_VALUE || pfile == NULL)
				sysLogError("CreateFileW()调用失败。_filename=[{0}]").args(_filename);
			else
			{
				LARGE_INTEGER lcFileSize = { 0 };
				if (!::GetFileSizeEx(pfile, &lcFileSize))
					sysLogError("if (!::GetFileSizeEx(pfile, &lcFileSize))调用失败。");
				else
				{
					*_llTotalSize = lcFileSize.QuadPart;
					DWORD dwCount = 0;
					OVERLAPPED lv_ovlpd = { 0 };
					lv_ovlpd.Offset = (DWORD)_llpos;
					lv_ovlpd.OffsetHigh = (DWORD)(((unsigned long long)_llpos) >> (8 * sizeof(DWORD)));
					BOOL lc_res = ::ReadFile(pfile, _buff, (DWORD)_buffsize, &dwCount, &lv_ovlpd);
					if (!lc_res || dwCount <= 0)
					{
						sysLogError("lc_res = ::ReadFile(pfile, _buff, (DWORD)_buffsize, &dwCount, &lv_ovlpd);调用失败。lc_res=[{0]},dwCount=[{1}]").args(
							lc_res, dwCount);
					}
					else
					{
						*_readsize = (int)dwCount;
						ret = true;
					}
				}
				if (!::CloseHandle(pfile))
					sysLogError("if (!::CloseHandle(pfile))调用失败。");
			}
			return ret;
		}
		static TxBlobString sysCodecvt(const TxBlobWString &_unicode)
		{
			TxBlobString lc_ret;
			if (_unicode.length() > 0)
			{
				int lc_ret_len = (int)::WideCharToMultiByte(CP_ACP, 0,
					_unicode.c_str(), _unicode.size(), NULL, 0, NULL, NULL);
				if (lc_ret_len > 0)
				{
					lc_ret.resize(lc_ret_len, false);
					::WideCharToMultiByte(CP_ACP, 0,
						_unicode.c_str(), _unicode.size(), lc_ret.data(), lc_ret.length(), NULL, NULL);
				}
			}
			return lc_ret;
		}
		static TxBlobWString sysCodecvt(const TxBlobString &_ascii)
		{
			TxBlobWString lc_ret;
			if (_ascii.length() > 0)
			{
				int lc_ret_len = (int)::MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS,
					_ascii.c_str(), _ascii.length(), NULL, 0);
				if (lc_ret_len > 0)
				{
					lc_ret.resize(lc_ret_len, false);
					::MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS,
						_ascii.c_str(), _ascii.length(), lc_ret.data(), lc_ret.length());
				}
			}
			return lc_ret;
		}
		//_fuzzy_path=L"D:\\master\\tool\\*"
		static bool fuzzyFindPath(const TxBlobWString &_fuzzy_path, const std::function<bool(const WIN32_FIND_DATAW&)> &_loopFn)
		{
			WIN32_FIND_DATAW lc_ffd = { 0 };
			HANDLE lc_hFind = ::FindFirstFileW(_fuzzy_path.c_str(), &lc_ffd);
			if (lc_hFind == INVALID_HANDLE_VALUE)
			{
				sysLogError("if (lc_hFind == INVALID_HANDLE_VALUE)。::FindFirstFileW(_fuzzy_path.c_str(), &lc_ffd)返回错误。");
				return false;
			}
			do {
				if (!_loopFn)
					sysLogError("if (!_loopFn)参数错误。");
				else
				{
					if (!(lc_ffd.cFileName[0] == 0
						|| (lc_ffd.cFileName[0] == L'.'&& lc_ffd.cFileName[1] == 0)
						|| (lc_ffd.cFileName[0] == L'.'&& lc_ffd.cFileName[1] == L'.'&& lc_ffd.cFileName[2] == 0)))
					{
						if (!_loopFn(lc_ffd))
							break;
					}
				}
			} while (::FindNextFileW(lc_hFind, &lc_ffd));
			if (!::FindClose(lc_hFind))
				sysLogError("if (!::FindClose(lc_hFind))返回错误。");
			return true;
		}
		static void openExplorerSelectFile(const wchar_t _strFilePath[])
		{
			TxBlobWString lc_wsLcDir(_strFilePath);
			for (int nn = (int)lc_wsLcDir.length(); nn--;)
			{
				if (lc_wsLcDir.at(nn) == L'/')
					lc_wsLcDir.data()[nn] = L'\\';
			}
			if (lc_wsLcDir.length() <= 0 || lc_wsLcDir.backChar() != L'\\')
				lc_wsLcDir.append(L"\\");
			for (int yy = (int)lc_wsLcDir.length(); yy--;)
			{
				if (lc_wsLcDir.at(yy) == L'\\')
				{
					TxBlobWString sss = lc_wsLcDir.left(yy);
					DWORD lc_dw_ret = ::GetFileAttributesW(sss.c_str());
					if (lc_dw_ret == INVALID_FILE_ATTRIBUTES)
					{
						continue;
					}
					else if (lc_dw_ret&FILE_ATTRIBUTE_DIRECTORY)
					{
						::ShellExecuteW(NULL, L"open", sss.c_str(), NULL, NULL, SW_SHOW);
						break;
					}
					else
					{
						TxBlobWString lc_wsParam(L"/e,/select, ");
						lc_wsParam.append(sss);
						::ShellExecuteW(NULL, L"open", L"explorer", lc_wsParam.c_str(), NULL, SW_SHOW);
						break;
					}
				}
			}
		}
		static void openDirectoryOrFile(const wchar_t _strPath[])
		{
			TxBlobWString lc_wsLcPath(_strPath);
			for (int i = (int)lc_wsLcPath.length(); i--;)
			{
				if (lc_wsLcPath.at(i) == L'/')
					lc_wsLcPath.data()[i] = L'\\';
			}
			if (lc_wsLcPath.length() <= 0 || lc_wsLcPath.backChar() != L'\\')
				lc_wsLcPath.append(L"\\");
			for (int yy = (int)lc_wsLcPath.length(); yy--;)
			{
				if (lc_wsLcPath.at(yy) == L'\\')
				{
					TxBlobWString sss = lc_wsLcPath.left(yy);
					DWORD lc_dw_ret = ::GetFileAttributesW(sss.c_str());
					if (lc_dw_ret == INVALID_FILE_ATTRIBUTES)
					{
						continue;
					}
					else
					{
						::ShellExecuteW(NULL, L"open", sss.c_str(), NULL, NULL, SW_SHOW);
						break;
					}
				}
			}
		}
		static bool isWow64(bool *_isWow64, const TxBlobWString &_exeFile)
		{
			bool ret = false;
			DWORD lcReadSize = 0;
			HANDLE lc_pfile = ::CreateFileW(_exeFile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL);
			if (lc_pfile != INVALID_HANDLE_VALUE)
			{
				IMAGE_DOS_HEADER lc_idh = { 0 };
				if (::ReadFile(lc_pfile, &lc_idh, sizeof(lc_idh), &lcReadSize, NULL)
					&& lcReadSize == sizeof(lc_idh))
				{
					IMAGE_FILE_HEADER ifh;
					OVERLAPPED lc_OVERLAPPED = { 0 };
					lc_OVERLAPPED.Offset = (DWORD)(lc_idh.e_lfanew + 4);
					lcReadSize = 0;
					if (::ReadFile(lc_pfile, &ifh, sizeof(ifh), &lcReadSize, &lc_OVERLAPPED)
						&& lcReadSize == sizeof(ifh))
					{
						//0x014C: x86, 0x0200: ia64, 0x8664: x64
						*_isWow64 = (ifh.Machine != IMAGE_FILE_MACHINE_I386);
					}
				}
				ret = true;
				::CloseHandle(lc_pfile);
			}
			return ret;
		}
		static bool setSystemVolume(float _fVolume)
		{
			bool ret = false;
			IMMDeviceEnumerator *lc_enumerator = NULL;
			IMMDevice *lc_endpoint = NULL;
			IAudioEndpointVolume *lc_volume = NULL;
			do {
				HRESULT lc_hr = S_OK;
				lc_hr = ::CoCreateInstance(
					__uuidof(MMDeviceEnumerator), NULL,
					CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
					(void**)&lc_enumerator);
				if (lc_hr != S_OK)
					break;
				lc_hr = lc_enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &lc_endpoint);
				if (lc_hr != S_OK)
					break;
				lc_hr = lc_endpoint->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void **)(&lc_volume));
				if (lc_hr != S_OK)
					break;
				lc_hr = lc_volume->SetMasterVolumeLevelScalar((float)_fVolume, NULL);
				if (lc_hr != S_OK)
					break;
				ret = true;
			} while (false);
			if (lc_volume != NULL)
				lc_volume->Release();
			if (lc_endpoint)
				lc_endpoint->Release();
			if (lc_enumerator)
				lc_enumerator->Release();
			return ret;
		}
		static void parseDnsToIp(TxBlobString _dnsName,
			std::vector<TxBlobString> *_ip_v4 = NULL, std::vector<TxBlobString> *_ip_v6 = NULL)
		{
			if (_ip_v4 != NULL) _ip_v4->clear();
			if (_ip_v6 != NULL) _ip_v6->clear();
			_dnsName = _dnsName.trimLR("\r\n\t \\/");
			if ((_ip_v4 == NULL&&_ip_v6 == NULL) || _dnsName.length() <= 0)
				return;
			struct addrinfo *lc_result = NULL;
			getaddrinfo(_dnsName.c_str(), NULL, NULL, &lc_result);
			std::set<TxBlobString> lc_ipv4_set, lc_ipv6_set;
			for (struct addrinfo *ppp = lc_result; ppp != NULL; ppp = ppp->ai_next)
			{
				if (ppp->ai_family == AF_INET)
				{
					char lc_ip_v4_str[48] = { 0 };
					::inet_ntop(ppp->ai_family, &((struct sockaddr_in*)ppp->ai_addr)->sin_addr,
						lc_ip_v4_str, TxStdlib::arrayLength(lc_ip_v4_str) - sizeof(size_t));
					TxBlobString lc_ip(TxBlobString(lc_ip_v4_str).trimLR("\r\n\t "));
					if (lc_ip.length() > 0)
						lc_ipv4_set.insert(lc_ip);
				}
				else if (ppp->ai_family == AF_INET6)
				{
					char lc_ip_v6_str[128 + sizeof(size_t)] = { 0 };
					::inet_ntop(ppp->ai_family, &((struct sockaddr_in6*)ppp->ai_addr)->sin6_addr,
						lc_ip_v6_str, TxStdlib::arrayLength(lc_ip_v6_str) - sizeof(size_t));
					TxBlobString lc_ip(TxBlobString(lc_ip_v6_str).trimLR("\r\n\t "));
					if (lc_ip.length() > 0)
						lc_ipv6_set.insert(lc_ip);
				}
			}
			if (lc_result != NULL)
				::freeaddrinfo(lc_result);
			if (lc_ipv4_set.size() > 0 && _ip_v4 != NULL)
			{
				_ip_v4->reserve(lc_ipv4_set.size());
				_ip_v4->assign(lc_ipv4_set.begin(), lc_ipv4_set.end());
			}
			if (lc_ipv6_set.size() > 0 && _ip_v4 != NULL)
			{
				_ip_v6->reserve(lc_ipv6_set.size());
				_ip_v6->assign(lc_ipv6_set.begin(), lc_ipv6_set.end());
			}
		}
	};

}


//--------------------------------------TxSysAtomicLong--------------------------------------//
class TxSysAtomicLong
{
private:
	TXSTDALIGNAS_16 long data_value;
	long long _buffer_tem_;
public:
	TxSysAtomicLong(long _data = 0)
	{
		this->data_value = _data;
		this->_buffer_tem_ = 0;
	}
	TxSysAtomicLong(const TxSysAtomicLong &_obj)
	{
		this->setValue(_obj.getValue());
	}
	TxSysAtomicLong& operator=(TxSysAtomicLong& _tem)
	{
		this->setValue(_tem.getValue());
		return *this;
	}
	long exchange(long _data)
	{
		return ::InterlockedExchange(&this->data_value, _data);
	}
	long increment()
	{
		return ::InterlockedIncrement(&this->data_value);
	}
	long decrement()
	{
		return ::InterlockedDecrement(&this->data_value);
	}
	void setValue(long _data)
	{
		this->exchange(_data);
	}
	long getValue() const
	{
		return ::InterlockedExchangeAdd(&const_cast<TxSysAtomicLong*>(this)->data_value, 0);
	}
};


//--------------------------------------TxSysEvent--------------------------------------//
class TxSysEvent : private TxForbidDirectCopyObject
{
private:
	HANDLE hEvent;
public:
	TxSysEvent()
	{
		this->hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if (this->hEvent == NULL)
			sysLogError("CreateEvent()执行失败");
	}
	~TxSysEvent()
	{
		if (!::CloseHandle(this->hEvent))
			sysLogError("CloseHandle()执行失败");
	}
	void resetEvent()
	{
		if (!::ResetEvent(this->hEvent))
			sysLogError("ResetEvent()执行失败").args();
	}
	void setEvent()
	{
		if (!::SetEvent(this->hEvent))
			sysLogError("SetEvent()执行失败");
	}
	bool waitEvent(int _msec = -1)
	{
#ifndef NDEBUG
		DWORD tem = ::WaitForSingleObject(this->hEvent, (_msec < 0 ? (DWORD)(INFINITE) : (DWORD)_msec));
		bool ret = false;
		switch (tem)
		{
		case WAIT_OBJECT_0:
			ret = true;
			tem = 0;
			break;
		case WAIT_ABANDONED:
			tem = 0;
			break;
		case WAIT_TIMEOUT:
			tem = 0;
			break;
		default:
			tem = 0;
			break;
		}
		return ret;
#else
		return WAIT_OBJECT_0 == ::WaitForSingleObject(this->hEvent, (_msec < 0 ? (DWORD)(INFINITE) : (DWORD)_msec));
#endif
	}
};


//--------------------------------------TxSysPreciseDelay--------------------------------------//
namespace _TxPrivateNs_ {
	class _MySysTimerPreciseDelaySetInstance__ : public TxSysMemoryLeakObject, private TxForbidDirectCopyObject
	{
	private:
		MMRESULT m_timerId;
		std::vector<std::weak_ptr<TxSysEvent>> vctEvent;
		struct {
			std::list<std::weak_ptr<TxSysEvent>> listNewEvent;
			TxSysMutex mMutex;
		}mAddCs;
	private:
		static void _instance_(
			std::shared_ptr<_MySysTimerPreciseDelaySetInstance__> *_get_M,
			const std::shared_ptr<_MySysTimerPreciseDelaySetInstance__> *_set_M)
		{
			static std::pair<TxSysMutex, std::weak_ptr<_MySysTimerPreciseDelaySetInstance__>> lc_info;
			lc_info.first.lock();
			if (_set_M != NULL)
				lc_info.second = *_set_M;
			if (_get_M != NULL)
				*_get_M = lc_info.second.lock();
			lc_info.first.unlock();
		}
		static void CALLBACK _fnTimeEvent(UINT, UINT, DWORD_PTR _userdata, DWORD_PTR, DWORD_PTR)
		{
			std::shared_ptr<_MySysTimerPreciseDelaySetInstance__> lc_spObj;
			_instance_(&lc_spObj, NULL);
			_MySysTimerPreciseDelaySetInstance__ *pThis = lc_spObj.get();
			if (pThis != NULL)
			{
				if (pThis->mAddCs.mMutex.try_lock())
				{
					pThis->vctEvent.insert(pThis->vctEvent.end(),
						pThis->mAddCs.listNewEvent.begin(), pThis->mAddCs.listNewEvent.end());
					pThis->mAddCs.listNewEvent.clear();
					pThis->mAddCs.mMutex.unlock();
				}
				int lc_realCount = (int)pThis->vctEvent.size();
				for (int i = lc_realCount; i--;)
				{
					std::shared_ptr<TxSysEvent> lc_spEvent = pThis->vctEvent[i].lock();
					if (lc_spEvent) lc_spEvent->setEvent();
					else pThis->vctEvent[i] = pThis->vctEvent[--lc_realCount];
				}
				if (lc_realCount != (int)pThis->vctEvent.size())
					pThis->vctEvent.erase(pThis->vctEvent.begin() + lc_realCount, pThis->vctEvent.end());
			}
		}
	public:
		_MySysTimerPreciseDelaySetInstance__()
		{
			this->m_timerId = ::timeSetEvent(1, 0, _fnTimeEvent, (DWORD_PTR)this, TIME_PERIODIC);
			if (this->m_timerId == NULL)
			{
				sysLogErrorW(L"初始化系统定时器失败。"
					L"if (this->m_timerId == NULL)，this->m_timerId=[{0}]").args(this->m_timerId);
			}
		}
		~_MySysTimerPreciseDelaySetInstance__()
		{
			if (this->m_timerId != NULL)
			{
				auto lc_errCode = ::timeKillEvent(this->m_timerId);
				if (lc_errCode != TIMERR_NOERROR)
				{
					sysLogErrorW(L"杀死系统定时器失败。"
						L"if (lc_errCode != TIMERR_NOERROR)，this->m_timerId=[{0}]"
						L"lc_errCode=[{1}]").args(this->m_timerId, lc_errCode);
				}
			}
		}
		static void initEnv()
		{
			_instance_(NULL, NULL);
			newDelayEvent(NULL, NULL);
		}
		static void newDelayEvent(
			std::shared_ptr<_MySysTimerPreciseDelaySetInstance__> *_spDelaySetInstance,
			std::shared_ptr<TxSysEvent> *_spDelayEvent)
		{
			struct TTT {
				TxSysMutex mDelaySetInstanceMutex, execTimerMutex;
				std::weak_ptr<_MySysTimerPreciseDelaySetInstance__> wkDelaySetInstance;
			};
			static TTT lc_static_obj;//全局变量
			if (_spDelaySetInstance != NULL&&_spDelayEvent != NULL)
			{
				//初始化_MySysTimerPreciseDelaySetInstance__
				lc_static_obj.mDelaySetInstanceMutex.lock();
				*_spDelaySetInstance = lc_static_obj.wkDelaySetInstance.lock();
				if (!(*_spDelaySetInstance))
				{
					lc_static_obj.wkDelaySetInstance
						= *_spDelaySetInstance = std::make_shared<_MySysTimerPreciseDelaySetInstance__>();
					_instance_(NULL, _spDelaySetInstance);
				}
				lc_static_obj.mDelaySetInstanceMutex.unlock();
				//初始化TxSysEvent
				*_spDelayEvent = std::make_shared<TxSysEvent>();
				//添加TxSysEvent
				(*_spDelaySetInstance)->mAddCs.mMutex.lock();
				(*_spDelaySetInstance)->mAddCs.listNewEvent.push_back(*_spDelayEvent);
				(*_spDelaySetInstance)->mAddCs.mMutex.unlock();
			}
		}
		static void deleteDelayEvent(std::shared_ptr<_MySysTimerPreciseDelaySetInstance__> *_spDelaySetInstance,
			std::shared_ptr<TxSysEvent> *_spDelayEvent)
		{
			_spDelaySetInstance->reset();
			_spDelayEvent->reset();
		}
	};
}
class TxSysPreciseDelay :private TxForbidDirectCopyObject
{
private:
	std::shared_ptr<_TxPrivateNs_::_MySysTimerPreciseDelaySetInstance__> spDelaySetInstance;
	std::shared_ptr<TxSysEvent> spDelayEvent;
public:
	TxSysPreciseDelay()
	{
		_TxPrivateNs_::_MySysTimerPreciseDelaySetInstance__::newDelayEvent(
			&this->spDelaySetInstance, &this->spDelayEvent);
	}
	~TxSysPreciseDelay()
	{
		this->release();
	}
	void release()
	{
		_TxPrivateNs_::_MySysTimerPreciseDelaySetInstance__::deleteDelayEvent(
			&this->spDelaySetInstance, &this->spDelayEvent);
	}
	void delay()
	{
		this->spDelayEvent->waitEvent(1);
	}
};


//--------------------------------------TxSysFileSlice--------------------------------------//
class TxSysFileSlice : public std::enable_shared_from_this<TxSysFileSlice>
{
private:
	class MyFileSliceSingleton : protected TxSliceFileEntirety
	{
	private:
		friend class TxSysFileSlice;
		std::wstring wsFileName;
		HANDLE pfile;
		std::uint64_t llRecordFileMaxSize;
	public:
		MyFileSliceSingleton(const std::wstring &_filename)
		{
			this->llRecordFileMaxSize = 0;
			this->wsFileName = _filename;
			if (this->wsFileName.length() <= 0)
			{
				this->vfError(L"MyFileSliceSingleton(): if(this->wsFileName.length()<=0)");
				this->pfile = INVALID_HANDLE_VALUE;
			}
			else
			{
				this->wsFileName = _filename;
				for (int ii = 0;; ++ii)
				{
					this->pfile = ::CreateFileW(
						this->wsFileName.c_str(),
						GENERIC_WRITE | GENERIC_READ,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL
					);
					if (this->pfile != INVALID_HANDLE_VALUE)
						break;
					if (ii == 1)
						this->wsFileName += _TxPrivateNs_::CSyslib::generateGuid().utf8Codecvt().toStdString();
					::DeleteFileW(this->wsFileName.c_str());
					if (ii >= 2)
						break;
				}
				if (this->pfile == INVALID_HANDLE_VALUE)
					this->vfError(L"MyFileSliceSingleton(): if(pThis->pfile==INVALID_HANDLE_VALUE)");
			}
		}
		~MyFileSliceSingleton()
		{
			if (this->pfile != INVALID_HANDLE_VALUE)
			{
				::CloseHandle(this->pfile);
				this->pfile = INVALID_HANDLE_VALUE;
			}
			::DeleteFileW(this->wsFileName.c_str());
		}
		virtual int vfFileRead(std::uint64_t _pos, void *_buffer, int _size)
		{
			if (this->pfile == INVALID_HANDLE_VALUE)
			{
				this->vfError(L"vfFileRead: if(this->pfile==INVALID_HANDLE_VALUE)");
				return 0;
			}
			DWORD lc_dwCount = 0;
			OVERLAPPED lv_ovlpd = { 0 };
			lv_ovlpd.Offset = (DWORD)_pos;
			lv_ovlpd.OffsetHigh = (DWORD)(((unsigned long long)_pos) >> (8 * sizeof(DWORD)));
			BOOL lc_res = ::ReadFile(this->pfile, _buffer, (DWORD)_size, &lc_dwCount, &lv_ovlpd);
			if (!lc_res || lc_dwCount < 0)
			{
				this->vfError(L"vfFileRead: if (!lc_res || dwCount <= 0)");
				lc_dwCount = 0;
			}
			return (int)lc_dwCount;
		}
		virtual bool vfFileWrite(std::uint64_t _pos, const void *_buffer, int _size)
		{
			if (this->pfile == INVALID_HANDLE_VALUE)
			{
				this->vfError(L"vfFileWrite: if(this->pfile==INVALID_HANDLE_VALUE)");
				return false;
			}
			bool ret = false;
			DWORD lc_dwCount = 0;
			OVERLAPPED lv_ovlpd = { 0 };
			lv_ovlpd.Offset = (DWORD)_pos;
			lv_ovlpd.OffsetHigh = (DWORD)(((unsigned long long)_pos) >> (8 * sizeof(DWORD)));
			if (::WriteFile(this->pfile, _buffer, (DWORD)_size, &lc_dwCount, &lv_ovlpd))
			{
				if (lc_dwCount >= 0 && lc_dwCount == (DWORD)_size)
					ret = true;
				else
					this->vfError(L"WriteFile");
			}
			else
			{
				this->vfError(L"WriteFile return false");
			}
			return ret;
		}
		virtual void vfFileResize(std::uint64_t _fileSize)
		{
			BOOL lc_doSign = FALSE;
			if (this->pfile == INVALID_HANDLE_VALUE)
			{
				this->vfError(L"vfFileResize: if(this->pfile==INVALID_HANDLE_VALUE)");
				lc_doSign = FALSE;
			}
			else if ((std::int64_t)_fileSize < 0)
			{
				this->vfError(L"vfFileResize: if((std::int64_t)_fileSize<=0)");
				lc_doSign = FALSE;
			}
			else if (this->llRecordFileMaxSize < _fileSize)
			{
				lc_doSign = TRUE;
			}
			else if ((std::uint64_t)this->llRecordFileMaxSize >= (std::uint64_t)(~(std::uint32_t)0) / 2 - 1
				&& _fileSize < (std::uint64_t)(~(std::uint32_t)0) / 4 - 1)
			{
				lc_doSign = TRUE;
			}
			if (lc_doSign)
			{
				LARGE_INTEGER lc_file_pos = { 0 };
				lc_file_pos.QuadPart = (LONGLONG)_fileSize;
				if (!::SetFilePointerEx(this->pfile, lc_file_pos, NULL, FILE_BEGIN))
				{
					this->vfError(L"vfFileResize : if(!::SetFilePointerEx(this->pfile,lc_file_pos, NULL, FILE_BEGIN))");
				}
				else
				{
					if (!::SetEndOfFile(this->pfile))
					{
						this->vfError(L"vfFileResize : if(!::SetEndOfFile(this->pfile))");
					}
					else
					{
						this->llRecordFileMaxSize = _fileSize;
					}
				}
			}
		}
		virtual void vfError(const std::wstring &_err)
		{
			sysLogError("class MyFileSliceSingleton -> [{0}]").args(_err);
		}
	};
private:
	TxSliceFileEntirety::TFileIdent mFileIdent;
	std::shared_ptr<MyFileSliceSingleton> spSysFileSliceSingleton;
	friend class std::shared_ptr<TxSysFileSlice>;
private:
	struct _tagSliceSingletonInfo_ {
		TxSysMutex mMutex;
		std::wstring ws_filename;
		std::weak_ptr<MyFileSliceSingleton> wpSysFileSliceSingleton;
	};
	static struct _tagSliceSingletonInfo_* _getSysFileSliceSingleton()
	{
		static struct _tagSliceSingletonInfo_ lc_data;
		return &lc_data;
	}
public:
	TxSysFileSlice(long long _fileSize)
	{
		struct _tagSliceSingletonInfo_ *lcGblObj = _getSysFileSliceSingleton();
		lcGblObj->mMutex.lock();
		if (_fileSize <= 0)
			this->mFileIdent.reset();
		else
		{
			this->spSysFileSliceSingleton = lcGblObj->wpSysFileSliceSingleton.lock();
			if (!this->spSysFileSliceSingleton && lcGblObj->ws_filename.length() > 0)
			{
				this->spSysFileSliceSingleton = std::make_shared<MyFileSliceSingleton>(lcGblObj->ws_filename);
				lcGblObj->wpSysFileSliceSingleton = this->spSysFileSliceSingleton;
			}
			this->mFileIdent = this->spSysFileSliceSingleton->do_create(_fileSize);
		}
		lcGblObj->mMutex.unlock();
	}
	~TxSysFileSlice()
	{
		this->release();
	}
	static std::shared_ptr<TxSysFileSlice> create(long long _fileSize)
	{
		return std::make_shared<TxSysFileSlice>(_fileSize);
	}
	static std::shared_ptr<TxSysFileSlice> create(const TxBlobString &_data)
	{
		std::shared_ptr<TxSysFileSlice> ret = create((long long)_data.size());
		ret->write(_data.data(), _data.size(), 0);
		return ret;
	}
	static void initEnv()
	{
		_getSysFileSliceSingleton();
	}
	static void initFileName(const std::wstring &_filename)
	{
		struct _tagSliceSingletonInfo_ *lcGblObj = _getSysFileSliceSingleton();
		lcGblObj->mMutex.lock();
		if (_filename.length() > 0)
			lcGblObj->ws_filename = _filename;
		lcGblObj->mMutex.unlock();
	}
	bool isEmpty()
	{
		struct _tagSliceSingletonInfo_ *lcGblObj = _getSysFileSliceSingleton();
		lcGblObj->mMutex.lock();
		bool ret = this->mFileIdent.isEmpty();
		lcGblObj->mMutex.unlock();
		return ret;
	}
	void release()
	{
		struct _tagSliceSingletonInfo_ *lcGblObj = _getSysFileSliceSingleton();
		lcGblObj->mMutex.lock();
		if (!this->mFileIdent.isEmpty())
		{
			this->spSysFileSliceSingleton->do_release(this->mFileIdent);
			this->spSysFileSliceSingleton.reset();
			this->mFileIdent.reset();
		}
		lcGblObj->mMutex.unlock();
	}
	bool seek(long long _offset, TxSliceFileEntirety::EORGSK _origin = TxSliceFileEntirety::ORIGIN_SEEK_SET)
	{
		struct _tagSliceSingletonInfo_ *lcGblObj = _getSysFileSliceSingleton();
		bool ret = false;
		lcGblObj->mMutex.lock();
		if (this->spSysFileSliceSingleton)
			ret = this->spSysFileSliceSingleton->do_seek(this->mFileIdent, _offset, _origin);
		lcGblObj->mMutex.unlock();
		return ret;
	}
	int read(void *_buffer, int _size, long long _pos = -1)
	{
		struct _tagSliceSingletonInfo_ *lcGblObj = _getSysFileSliceSingleton();
		int ret = false;
		lcGblObj->mMutex.lock();
		if (this->spSysFileSliceSingleton)
			ret = this->spSysFileSliceSingleton->do_read(this->mFileIdent, _buffer, _size, _pos);
		lcGblObj->mMutex.unlock();
		return ret;
	}
	TxBlobString readAll()
	{
		struct _tagSliceSingletonInfo_ *lcGblObj = _getSysFileSliceSingleton();
		TxBlobString ret;
		lcGblObj->mMutex.lock();
		if (this->spSysFileSliceSingleton)
		{
			if (!this->spSysFileSliceSingleton->do_read_all(&ret, this->mFileIdent))
				ret.clear();
		}
		lcGblObj->mMutex.unlock();
		return ret;
	}
	bool write(const void *_buffer, int _size, long long _pos = -1)
	{
		struct _tagSliceSingletonInfo_ *lcGblObj = _getSysFileSliceSingleton();
		bool ret = false;
		lcGblObj->mMutex.lock();
		if (this->spSysFileSliceSingleton)
			ret = this->spSysFileSliceSingleton->do_write(this->mFileIdent, _buffer, _size, _pos);
		lcGblObj->mMutex.unlock();
		return ret;
	}
};


//--------------------------------------TxSysHighPrecisionTimeLine--------------------------------------//
class TxSysHighPrecisionTimeLine
{
private:
	TxSysPreciseDelay mSysPreciseDelay;
	TxSysEvent mWaitEvent;
	TxThreadSafeBoolean bVaild;
	long long lstTime;
private:
	void _sleep_msec_(long long _msec)
	{
		if (_msec > 10) this->mWaitEvent.waitEvent((DWORD)_msec);
		else this->mSysPreciseDelay.delay();
	}
	static long long _get_tick_count_()
	{
		return _TxPrivateNs_::CSyslib::getHighPrecisionMicrosecond() / 1000;
	}
public:
	TxSysHighPrecisionTimeLine()
	{
		this->bVaild = false;
		this->lstTime = 0;
		this->reset();
	}
	~TxSysHighPrecisionTimeLine()
	{
		this->destory();
	}
	void reset()
	{
		this->bVaild = true;
		this->lstTime = this->_get_tick_count_();
	}
	void destory()
	{
		this->bVaild = false;
		this->mWaitEvent.setEvent();
	}
	void wait(int _msec)
	{
		if (_msec <= 0)
		{
			sysLogError("if (_msec <= 0)参数错误。_msec=[{0}]").args(_msec);
			return;
		}
		while (this->bVaild)
		{
			long long lcSurpTime = this->lstTime + _msec - this->_get_tick_count_();
			if (lcSurpTime <= 0)
			{
				this->lstTime = this->lstTime + _msec;
				break;
			}
			else if (lcSurpTime >= 100)
				this->_sleep_msec_(TxStdlib::maximum(lcSurpTime * 2 / 3, lcSurpTime - 100));
			else
				this->_sleep_msec_(lcSurpTime / 2);
		}
	}
};


//--------------------------------------TxSysThread--------------------------------------//
class TxSysThread : private TxForbidDirectCopyObject
{
private:
	class T_FUNC_BASIC_
	{
	private:
		TxObjectLifePeriod::TxObjectEntity mThreadObjectEntity;
	public:
		virtual ~T_FUNC_BASIC_() {}
		virtual void vfExec() = 0;
		TxObjectLifePeriod getObjectLifePeriod() { return TxObjectLifePeriod(&this->mThreadObjectEntity); }
	};
private:
	HANDLE hThread;//线程句柄
	unsigned threadID;
	std::shared_ptr<T_FUNC_BASIC_> sp_func;
private:
	static unsigned __stdcall _static_start_address(void* pArguments)
	{
		if (((TxSysThread*)pArguments)->sp_func)
		{
			((TxSysThread*)pArguments)->sp_func->vfExec();
			((TxSysThread*)pArguments)->sp_func.reset();
		}
		::_endthreadex(0);
		return 0;
	}
	template<class TFUNC_OBJ>
	TxObjectLifePeriod _my_create__(const TFUNC_OBJ &_mThreadFunc, BOOL _bPriority)
	{
		this->release();

		class T_FUNC_IMP : public T_FUNC_BASIC_
		{
		private:
			TFUNC_OBJ obj_fn;
		public:
			T_FUNC_IMP(const TFUNC_OBJ *_obj_fn) :obj_fn(*_obj_fn) { }
			virtual ~T_FUNC_IMP() {}
			virtual void vfExec() { this->obj_fn(); }
		};
		this->sp_func = std::make_shared<T_FUNC_IMP>(&_mThreadFunc);

		this->hThread = (HANDLE)::_beginthreadex(NULL, 0, &_static_start_address, this, 0, &this->threadID);
		if (this->hThread == NULL || this->hThread == INVALID_HANDLE_VALUE)
			sysLogError("_beginthreadex()执行失败");
		else if (_bPriority)
		{
			int lcArryPriority[] = {
				THREAD_PRIORITY_IDLE,
				THREAD_PRIORITY_LOWEST,
				THREAD_PRIORITY_BELOW_NORMAL,
				THREAD_PRIORITY_NORMAL,
				THREAD_PRIORITY_ABOVE_NORMAL,
				THREAD_PRIORITY_HIGHEST,
				THREAD_PRIORITY_TIME_CRITICAL,
			};
			for (int i = TxStdlib::arrayLength(lcArryPriority); (--i) >= 0;)
			{
				if (lcArryPriority[i] == THREAD_PRIORITY_NORMAL)
					break;
				if (!::SetThreadPriority(this->hThread, lcArryPriority[i]))
					sysLogError("SetThreadPriority()执行失败");
				else break;
			}
		}
		return this->sp_func->getObjectLifePeriod();
	}
public:
	TxSysThread() :hThread(NULL), threadID(0)
	{
		TxSysDebugObjectMemory::malloc(this);
	}
	virtual ~TxSysThread()
	{
		TxSysDebugObjectMemory::free(this);
		this->release();
	}
	inline DWORD getThreadId() const { return (DWORD)this->threadID; }
	template<class TFUNC>
	TxObjectLifePeriod create(const TFUNC &_mThreadFunc)
	{
		return this->_my_create__(_mThreadFunc, FALSE);
	}
	template<class TFUNC>
	TxObjectLifePeriod createWithPriority(const TFUNC &_mThreadFunc)
	{
		return this->_my_create__(_mThreadFunc, TRUE);
	}
	void release()
	{
		if (this->hThread != NULL)
		{
			if (this->hThread == ::GetCurrentThread())
				sysLogError("程序异常。11205，线程内部不能析构其对象。if (this->hThread == ::GetCurrentThread())");
			else this->join();
			if (!::CloseHandle(this->hThread))
				sysLogError("if (!::CloseHandle(this->hThread))调用失败");
			this->hThread = NULL;
		}
	}
	void join(int _msec = 0)
	{
		if (this->hThread != NULL)
			::WaitForSingleObject(this->hThread, _msec <= 0 ? INFINITE : (DWORD)_msec);
	}
};


//--------------------------------------TxSysAtomicObjectTmpl--------------------------------------//
template<class TYPE, class T_DATA>
class TxSysAtomicObjectTmpl : private TxForbidDirectCopyObject
{
private:
	struct tagPublicData
	{
		friend TYPE;
		TYPE *obj;
		tagPublicData(TYPE *_obj) :obj(_obj)
		{
			this->obj->superLock();
			if (this->obj->uiRefCount > 0)
			{
				sysLogError("if (this->obj->uiRefCount > 0)");
				::MessageBoxW(NULL, L"Illegal use multi-times class TxSysAtomicObjectTmpl[n]", L"error", MB_OK);
			}
			++this->obj->uiRefCount;
		}
		tagPublicData(const tagPublicData &_tem)
		{
			this->obj = _tem.obj;
			++this->obj->uiRefCount;
		}
		~tagPublicData()
		{
			if ((--this->obj->uiRefCount) == 0)
				this->obj->superUnlock();
		}
		T_DATA* operator->()
		{
			return this->obj->superData();
		}
	};
private:
	unsigned int uiRefCount;
	friend TYPE;
private:
	static TYPE* _init_env_(int _sign)
	{
		static TYPE *ret = NULL;
		if (_sign > 0)
		{
			assert_error_(ret == NULL, L"Illegal Use class TxSysAtomicObjectTmpl[x]");
			if (ret == NULL)
				ret = new TYPE();
		}
		else if (_sign < 0)
		{
			assert_error_(ret != NULL, L"Illegal Use class TxSysAtomicObjectTmpl[y]");
			if (ret != NULL)
			{
				delete ret;
				ret = NULL;
			}
		}
		return ret;
	}
	static void assert_error_(bool _b_err, const wchar_t *_text)
	{
		static bool lc_b_msgbox_sign = true;
		if (!_b_err&&lc_b_msgbox_sign)
		{
			lc_b_msgbox_sign = false;
			sysLogError("{0}").args(_text);
			::MessageBoxW(NULL, _text, L"error", MB_OK);
			lc_b_msgbox_sign = true;
		}
	}
	TxSysAtomicObjectTmpl(const TxSysAtomicObjectTmpl&)
	{
		TxSysDebugObjectMemory::malloc(this);
		assert_error_(false, L"TxSysAtomicObjectTmpl(const TxSysAtomicObjectTmpl&): copy a object,but it is forbided copyable![a]");
	}
	TxSysAtomicObjectTmpl& operator=(const TxSysAtomicObjectTmpl&)
	{
		assert_error_(false, L"operator=(const TxSysAtomicObjectTmpl&): copy a object,but it is forbided copyable![b]");
	}
protected:
	TxSysAtomicObjectTmpl()
	{
		TxSysDebugObjectMemory::malloc(this);
		this->assert_error_(true, L"Illegal Use class TxSysAtomicObjectTmpl[m]");
		this->uiRefCount = 0;
	}
	virtual ~TxSysAtomicObjectTmpl()
	{
		TxSysDebugObjectMemory::free(this);
		assert_error_(this->uiRefCount == 0, L"Illegal Use class TxSysAtomicObjectTmpl[z]");
	}
public:
	static tagPublicData object()
	{
		return tagPublicData(_init_env_(0));
	}
	static void initialize()
	{
		_init_env_(1);
	}
	static void uninitialize()
	{
		_init_env_(-1);
	}
	template<class TFUNC>
	static void rawData(const TFUNC &_mfunc)
	{
		TYPE *pThis = _init_env_(0);
		pThis->superLock();
		_mfunc(pThis->superData());
		pThis->superUnlock();
	}
};


//--------------------------------------TxSysAutoLocker--------------------------------------//
class TxSysAutoLocker : private TxForbidDirectCopyObject
{
private:
	struct {
		TxSysMutex *pMutex_0;
		TxSysProcessMutex<false> *pMutex_1;
		TxSysProcessMutex<true> *pMutex_2;
	} mtxs;
private:
	TxSysAutoLocker& operator=(const TxSysAutoLocker&) const
	{
		return *(TxSysAutoLocker*)NULL;
	}
	inline void _init_zero_()
	{
		this->mtxs.pMutex_0 = NULL;
		this->mtxs.pMutex_1 = NULL;
		this->mtxs.pMutex_2 = NULL;
	}
	void _lock_()
	{
		if (this->mtxs.pMutex_0 != NULL)
			this->mtxs.pMutex_0->lock();
		if (this->mtxs.pMutex_1 != NULL)
			this->mtxs.pMutex_1->lock();
		if (this->mtxs.pMutex_2 != NULL)
			this->mtxs.pMutex_2->lock();
	}
	void _unlock_()
	{
		if (this->mtxs.pMutex_0 != NULL)
		{
			this->mtxs.pMutex_0->unlock();
			this->mtxs.pMutex_0 = NULL;
		}
		if (this->mtxs.pMutex_1 != NULL)
		{
			this->mtxs.pMutex_1->unlock();
			this->mtxs.pMutex_1 = NULL;
		}
		if (this->mtxs.pMutex_2 != NULL)
		{
			this->mtxs.pMutex_2->unlock();
			this->mtxs.pMutex_2 = NULL;
		}
	}
public:
	TxSysAutoLocker(TxSysAutoLocker &_tem)
	{
		this->_init_zero_();
		this->mtxs = _tem.mtxs;
		this->_lock_();
	}
	TxSysAutoLocker(TxSysMutex *_pMutex)
	{
		this->_init_zero_();
		this->mtxs.pMutex_0 = _pMutex;
		this->_lock_();
	}
	TxSysAutoLocker(TxSysMutex &_mMutex)
	{
		this->_init_zero_();
		this->mtxs.pMutex_0 = &_mMutex;
		this->_lock_();
	}
	TxSysAutoLocker(TxSysProcessMutex<false> *_pMutex)
	{
		this->_init_zero_();
		this->mtxs.pMutex_1 = _pMutex;
		this->_lock_();
	}
	TxSysAutoLocker(TxSysProcessMutex<false> &_mMutex)
	{
		this->_init_zero_();
		this->mtxs.pMutex_1 = &_mMutex;
		this->_lock_();
	}
	TxSysAutoLocker(TxSysProcessMutex<true> *_pMutex)
	{
		this->_init_zero_();
		this->mtxs.pMutex_2 = _pMutex;
		this->_lock_();
	}
	TxSysAutoLocker(TxSysProcessMutex<true> &_mMutex)
	{
		this->_init_zero_();
		this->mtxs.pMutex_2 = &_mMutex;
		this->_lock_();
	}
	~TxSysAutoLocker()
	{
		this->_unlock_();
	}
	void unlock()
	{
		this->_unlock_();
	}
};


//--------------------------------------TxSysTimeSpan--------------------------------------//
template<bool B_ACTIVE_MUTEX, bool B_ACCURATE_TIME = false>
class TxSysTimeSpan :private TxForbidDirectCopyObject
{
	friend class TxSysTimeSpan<!B_ACTIVE_MUTEX>;
private:
	long long curTime;
	long long lstTime;
	enum tag_e_ull_time_ratio_base: std::uint64_t { e_ull_time_ratio_base = 0x40000000 };
	std::uint64_t ullTimeRatio;
	char buff_mutex__tem__[B_ACTIVE_MUTEX ? sizeof(TxSysMutex) : sizeof(long long)];
private:
	inline void _constructor_()
	{
		this->ullTimeRatio = 0;
		if (B_ACTIVE_MUTEX)
			new ((TxSysMutex*)&this->buff_mutex__tem__[0]) TxSysMutex();
	}
	inline void _destructor_()
	{
		if (B_ACTIVE_MUTEX)
			((TxSysMutex*)&this->buff_mutex__tem__[0])->~TxSysMutex();
	}
	inline void _lock_obj()
	{
		if (B_ACTIVE_MUTEX)
			((TxSysMutex*)&this->buff_mutex__tem__[0])->lock();
	}
	inline void _unlock_obj()
	{
		if (B_ACTIVE_MUTEX)
			((TxSysMutex*)&this->buff_mutex__tem__[0])->unlock();
	}
	DWORD _get_time_()
	{
		DWORD ret = 0;
		if (B_ACCURATE_TIME)//高精度
			ret = (DWORD)(_TxPrivateNs_::CSyslibBase::getHighPrecisionMicrosecond(NULL) / 1000);
		else ret = ::GetTickCount();
		if (this->ullTimeRatio <= 0 || this->ullTimeRatio == e_ull_time_ratio_base)
			return ret;
		return (DWORD)((ret*this->ullTimeRatio) / e_ull_time_ratio_base);
	}
public:
	TxSysTimeSpan(long long _init_time = 0)
	{
		this->_constructor_();
		this->reset(_init_time);
	}
	TxSysTimeSpan(const TxSysTimeSpan<B_ACTIVE_MUTEX>& _tem)
	{
		this->_constructor_();
		this->reset(const_cast<TxSysTimeSpan<B_ACTIVE_MUTEX>&>(_tem).getCurrentMillisecond());
	}
	TxSysTimeSpan(const TxSysTimeSpan<!B_ACTIVE_MUTEX>& _tem)
	{
		this->_constructor_();
		this->reset(const_cast<TxSysTimeSpan<!B_ACTIVE_MUTEX>&>(_tem).getCurrentMillisecond());
	}
	~TxSysTimeSpan()
	{
		this->_destructor_();
	}
	TxSysTimeSpan& operator=(const TxSysTimeSpan<B_ACTIVE_MUTEX> &_tem)
	{
		this->reset(const_cast<TxSysTimeSpan<B_ACTIVE_MUTEX>&>(_tem).getCurrentMillisecond());
		return *this;
	}
	TxSysTimeSpan& operator=(const TxSysTimeSpan<!B_ACTIVE_MUTEX> &_tem)
	{
		this->reset(const_cast<TxSysTimeSpan<!B_ACTIVE_MUTEX>&>(_tem).getCurrentMillisecond());
		return *this;
	}
	void setTimeMultiple(double _dTimeMultiple)
	{
		this->_lock_obj();
		DWORD ttt = this->_get_time_();
		this->ullTimeRatio = (std::uint64_t)(TxStdlib::absolute(_dTimeMultiple) * e_ull_time_ratio_base + 0.5);
		this->lstTime += this->_get_time_() - ttt;
		this->_unlock_obj();
	}
	void reset(long long _curTime = 0)
	{
		this->_lock_obj();
		DWORD ttt = this->_get_time_();
		this->lstTime = ttt;
		this->curTime = (long long)(_curTime);
		this->_unlock_obj();
	}
	void resetNegativeInfinity()
	{
		this->reset(-9999999999999999i64);
	}
	void resetPositiveInfinity()
	{
		this->reset(9999999999999999i64);
	}
	inline long long getCurrentMillisecond(bool _bResetZeroTime = false)
	{
		this->_lock_obj();
		long long tc = this->_get_time_();
		this->curTime += (tc > lstTime) ? (tc - lstTime) : 0;
		this->lstTime = tc;
		long long ret = this->curTime;
		if (_bResetZeroTime)
			this->curTime = 0;
		this->_unlock_obj();
		return ret;
	}
	bool dtctCurTimeResetSpanMsec(long long _outTime, bool _bClearSurplusTime = false)
	{
		bool ret = false;
		this->_lock_obj();
		long long tc = this->_get_time_();
		this->curTime += (tc > lstTime) ? (tc - lstTime) : 0;
		this->lstTime = tc;
		if (this->curTime >= (long long)(_outTime))
		{
			ret = true;
			if (_bClearSurplusTime) this->curTime = 0;
			else this->curTime -= (long long)(_outTime);
		}
		this->_unlock_obj();
		return ret;
	}
	inline void plusMillisecond(long long _deltaMsec)
	{
		this->_lock_obj();
		this->curTime += _deltaMsec;
		this->_unlock_obj();
	}
};


//--------------------------------------TxSysAtomicObjectTmpl--------------------------------------//
class TxSysStillThread
{
public:
	class TProcess
	{
	private:
		friend TxSysStillThread;
		TxSysStillThread *pThis;
	private:
		TProcess(TxSysStillThread *_pThis) :pThis(_pThis) { }
	public:
		void sleep(int _msec)
		{
			TxSysTimeSpan<false> lc_timeSpan((int)(_msec*0.01));
			int lc_sleepCount = 1;
			int lc_unit_msec = _msec;
			if (this->pThis->iProcessMsec < _msec&&this->pThis->iProcessMsec > 0)
			{
				lc_unit_msec = this->pThis->iProcessMsec;
				lc_sleepCount = (_msec - 1) / this->pThis->iProcessMsec + 1;
			}
			while (lc_sleepCount--)
			{
				this->process();
				if (lc_timeSpan.getCurrentMillisecond() >= _msec)
					this->pThis->mDelayEvent.waitEvent(lc_unit_msec);
			}
			this->process();
		}
		void process()
		{
			std::list<std::shared_ptr<std::function<void()>>> lc_listTask;
			this->pThis->mInnerMutex.lock();
			lc_listTask.swap(this->pThis->m_listTask);
			this->pThis->m_listTask.clear();
			this->pThis->mInnerMutex.unlock();
			for (auto iter = lc_listTask.begin(); iter != lc_listTask.end(); ++iter)
				(**iter)();
			if (this->pThis->fnProcess&&this->pThis->iProcessMsec > 0
				&& this->pThis->mProcessTimeSpan.dtctCurTimeResetSpanMsec(
				(int)(this->pThis->iProcessMsec*0.9 + 0.5), true))
			{
				this->pThis->fnProcess();
			}
		}
	};
private:
	friend TProcess;
	std::shared_ptr<TxSysThread> spSysThread;
	std::function<void()> fnProcess;
	std::list<std::shared_ptr<std::function<void()>>> m_listTask;
	TxSysTimeSpan<false> mProcessTimeSpan;
	std::weak_ptr<std::pair<std::function<void(TxSysStillThread::TProcess*)>,
		TxThreadSafeBoolean>> wpThreadFunc;
	TxThreadSafeBoolean bObjectVaild;
	TxSysEvent mDelayEvent;
	TxSysMutex mInnerMutex;//供线程内部使用
	TxSysMutex mGlbMutex;//线程内部不能使用改mutex
	TxSysEvent mGlbEvent;
	int iProcessMsec;
private:
	void _constructor_(int _processMsec, const std::function<void()> &_fnProcess)
	{
		this->_destructor_();
		this->iProcessMsec = _processMsec;
		this->fnProcess = _fnProcess;
		this->spSysThread = std::make_shared<TxSysThread>();
		this->bObjectVaild = true;
		this->spSysThread->create([this]() {
			TxSysStillThread::TProcess lc_process(this);
			while (this->bObjectVaild)
			{
				this->mInnerMutex.lock();
				auto lc_fnThread = this->wpThreadFunc.lock();
				if (lc_fnThread)
					lc_fnThread->second = true;
				this->mInnerMutex.unlock();
				if (lc_fnThread)
				{
					this->mGlbEvent.setEvent();
					if (lc_fnThread->first)
						lc_fnThread->first(&lc_process);
					lc_fnThread.reset();
					this->mGlbEvent.setEvent();
				}
				if (!this->bObjectVaild)
					break;
				this->mDelayEvent.waitEvent(TxStdlib::maximum(1, this->iProcessMsec));
			}
		});
	}
	void _destructor_()
	{
		if (this->spSysThread)
		{
			this->bObjectVaild = false;
			this->mDelayEvent.setEvent();
			this->mGlbEvent.setEvent();
			this->spSysThread->join();
			this->spSysThread.reset();
		}
	}
public:
	TxSysStillThread()
	{
		this->iProcessMsec = -1;
	}
	~TxSysStillThread()
	{
		if (this->spSysThread)
			sysLogError("if (this->spSysThread)未释放线程对象。123123028965");
		this->destructor();
	}
	void constructor(int _processMsec, const std::function<void()> &_fnProcess)
	{
		this->mGlbMutex.lock();
		this->_constructor_(_processMsec, _fnProcess);
		this->mGlbMutex.unlock();
	}
	void destructor()
	{
		this->mGlbMutex.lock();
		this->_destructor_();
		this->mGlbMutex.unlock();
	}
	void postTask(const std::function<void()> &_func)
	{
		assert(!!this->spSysThread);
		if (_func)
		{
			auto lc_fn = std::make_shared<std::function<void()>>();
			*lc_fn = _func;
			this->mInnerMutex.lock();
			this->m_listTask.push_back(lc_fn);
			this->mInnerMutex.unlock();
			this->mDelayEvent.setEvent();
		}
	}
	void enter(const std::function<void(TxSysStillThread::TProcess*)> &_thread_fn)
	{
		assert(!!this->spSysThread);
		this->leave();
		this->mInnerMutex.lock();
		auto lc_fn = std::make_shared<std::pair<
			std::function<void(TxSysStillThread::TProcess*)>, TxThreadSafeBoolean>>();
		lc_fn->first = _thread_fn;
		lc_fn->second = false;
		this->wpThreadFunc = lc_fn;
		this->mInnerMutex.unlock();
		while (!lc_fn->second)
			this->mGlbEvent.waitEvent();
	}
	void leave()
	{
		assert(!!this->spSysThread);
		this->mInnerMutex.lock();
		auto lc_wpThreadFunc = this->wpThreadFunc;
		this->mInnerMutex.unlock();
		while (!lc_wpThreadFunc.expired())
			this->mGlbEvent.waitEvent();
	}
};


//--------------------------------------TxSysRandomReadFile--------------------------------------//
template<bool B_MUTEX>
class TxSysRandomReadFile
{
	typedef TxSysMutex T_MUTEX_TYPE;
private:
	HANDLE pfile;
	unsigned char buff_mutex__tem__[B_MUTEX ? sizeof(T_MUTEX_TYPE) : sizeof(long long)];
private:
	inline void _constructor_()
	{
		TxSysDebugObjectMemory::malloc(this);
		if (B_MUTEX)
			new ((T_MUTEX_TYPE*)&this->buff_mutex__tem__[0]) T_MUTEX_TYPE();
	}
	inline void _destructor_()
	{
		TxSysDebugObjectMemory::free(this);
		if (B_MUTEX)
			((T_MUTEX_TYPE*)&this->buff_mutex__tem__[0])->~T_MUTEX_TYPE();
	}
	inline void _lock_obj() const
	{
		if (B_MUTEX)
			const_cast<T_MUTEX_TYPE*>((const T_MUTEX_TYPE*)&this->buff_mutex__tem__[0])->lock();
	}
	inline void _unlock_obj() const
	{
		if (B_MUTEX)
			const_cast<T_MUTEX_TYPE*>((const T_MUTEX_TYPE*)&this->buff_mutex__tem__[0])->unlock();
	}
	bool _open_nomutex_(const TxBlobWString &_filepath)
	{
		this->_close_nomutex_();
		this->pfile = ::CreateFileW(
			_filepath.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		if (this->pfile == NULL)
			sysLogError("CreateFileW()执行失败, _filepath={0}").args(_filepath);
		return this->_is_file_open_nomutex_();
	}
	void _close_nomutex_()
	{
		if (this->pfile != INVALID_HANDLE_VALUE)
		{
			if (!::CloseHandle(this->pfile))
				sysLogError("CloseHandle()执行失败");
			this->pfile = INVALID_HANDLE_VALUE;
		}
	}
	long long _get_filesize_nomutex_()
	{
		long long ret = 0;
		if (this->_is_file_open_nomutex_())
		{
			LARGE_INTEGER lcFileSize = { 0 };
			if (!::GetFileSizeEx(this->pfile, &lcFileSize))
				sysLogError("GetFileSizeEx()执行失败");
			else ret = (long long)lcFileSize.QuadPart;
		}
		if (ret < 0)
			ret = 0;
		return ret;
	}
	bool _is_file_open_nomutex_()
	{
		return this->pfile != INVALID_HANDLE_VALUE&&this->pfile != NULL;
	}
	int _readFile_nomutex_(void *_buffer, long long _llpos, int _buffsize)
	{
		if (_buffer == NULL || _llpos < 0 || _buffsize <= 0)
		{
			sysLogError("if (_buffer == NULL || _llpos < 0 || _buffsize <= 0)参数错误。_buffer=[{0}],_llpos=[{1}],_buffsize=[{2}]").args(
				(size_t)_buffer, _llpos, _buffsize);
			return 0;
		}
		int ret = 0;
		if (this->pfile != INVALID_HANDLE_VALUE)
		{
			DWORD lc_dwCount = 0;
			OVERLAPPED lc_lv_ovlpd = { 0 };
			lc_lv_ovlpd.Offset = (DWORD)_llpos;
			lc_lv_ovlpd.OffsetHigh = (DWORD)(((unsigned long long)_llpos) >> (8 * sizeof(DWORD)));
			if (!::ReadFile(this->pfile, _buffer, (DWORD)_buffsize, &lc_dwCount, &lc_lv_ovlpd) || lc_dwCount < 0)
			{
				sysLogError("ReadFile()执行失败");
			}
			else
			{
				ret = (int)lc_dwCount;
			}
		}
		if (ret < 0)
			ret = 0;
		return ret;
	}
public:
	TxSysRandomReadFile()
	{
		this->_constructor_();
		this->pfile = INVALID_HANDLE_VALUE;
	}
	~TxSysRandomReadFile()
	{
		this->close();
		this->_destructor_();
	}
	bool open(const TxBlobWString &_filepath)
	{
		this->_lock_obj();
		this->_close_nomutex_();
		bool ret = this->_open_nomutex_(_filepath);
		this->_unlock_obj();
		return ret;
	}
	void close()
	{
		this->_lock_obj();
		this->_close_nomutex_();
		this->_unlock_obj();
	}
	bool isOpen()
	{
		this->_lock_obj();
		bool ret = this->_is_file_open_nomutex_();
		this->_unlock_obj();
		return ret;
	}
	long long getFileSize()
	{
		this->_lock_obj();
		long long ret = this->_get_filesize_nomutex_();
		this->_unlock_obj();
		return ret;
	}
	int readFile(void *_buffer, long long _llpos, int _buffsize, long long *_llTotalFileSize = NULL)
	{
		this->_lock_obj();
		int ret = this->_readFile_nomutex_(_buffer, _llpos, _buffsize);
		if (_llTotalFileSize != NULL)
			*_llTotalFileSize = this->_get_filesize_nomutex_();
		this->_unlock_obj();
		return ret;
	}
};


//--------------------------------------TxSysUniqueSharedLock--------------------------------------//
class TxSysUniqueSharedLock
{
private:
	struct tagInterLockedVar
	{
		volatile LONG lReadCount;
		volatile LONG _tem__1;
		volatile LONG lWriteCount;
		volatile LONG _tem__2;
	} *pAtmcVar;
	TxSysEvent mEvent;
	TxSysMutex mMutex;
private:
	struct T_FUNC {
		void(*func)(void*); void *arg;
		T_FUNC(void(*_func)(void*), void *_arg) :func(_func), arg(_arg) { }
		inline void operator()()const { (this->func)(this->arg); }
	};
	template<class TFUNC>
	bool __uniqueLock_(int _msec, const TFUNC& _mFunc)
	{
		if (_msec >= 0)
		{
			bool lc_sign = false;
			enum { e_time_tt = 10, };
			int lc_surp_msec = _msec;
			if (lc_surp_msec < e_time_tt * 5)
				lc_surp_msec = e_time_tt * 5;
			while (!this->mMutex.try_lock())
			{
				if (lc_surp_msec <= e_time_tt)
					return false;
				lc_surp_msec -= e_time_tt;
				::Sleep(e_time_tt);
			}
			for (;;)
			{
				if (::InterlockedExchangeAdd(&this->pAtmcVar->lReadCount, 0) == 0 && ::InterlockedExchangeAdd(&this->pAtmcVar->lWriteCount, 0) == 0)
				{
					lc_sign = true;
					break;
				}
				if (!this->mEvent.waitEvent(lc_surp_msec))
				{//timeout
					break;
				}
			}
			if (lc_sign)
				::InterlockedIncrement(&this->pAtmcVar->lWriteCount);
			this->mMutex.unlock();
			if (!lc_sign)
				return false;
		}
		else
		{
			this->mMutex.lock();
			while (::InterlockedExchangeAdd(&this->pAtmcVar->lReadCount, 0) != 0
				|| ::InterlockedExchangeAdd(&this->pAtmcVar->lWriteCount, 0) != 0)
			{
				this->mEvent.waitEvent();
			}
			::InterlockedIncrement(&this->pAtmcVar->lWriteCount);
			this->mMutex.unlock();
		}

		const_cast<TFUNC&>(_mFunc)();

		::InterlockedDecrement(&this->pAtmcVar->lWriteCount);
		this->mEvent.setEvent();
		return true;
	}
	template<class TFUNC>
	bool __sharedLock_(int _msec, const TFUNC& _mFunc)
	{
		if (_msec >= 0)
		{
			bool lc_sign = false;
			enum { e_time_tt = 10, };
			int lc_surp_msec = _msec;
			if (lc_surp_msec < e_time_tt * 5)
				lc_surp_msec = e_time_tt * 5;
			while (!this->mMutex.try_lock())
			{
				if (lc_surp_msec <= e_time_tt)
					return false;
				lc_surp_msec -= e_time_tt;
				::Sleep(e_time_tt);
			}
			for (;;)
			{
				if (::InterlockedExchangeAdd(&this->pAtmcVar->lWriteCount, 0) == 0)
				{
					lc_sign = true;
					break;
				}
				if (!this->mEvent.waitEvent(lc_surp_msec))
				{//timeout
					break;
				}
			}
			if (lc_sign)
				::InterlockedIncrement(&this->pAtmcVar->lReadCount);
			this->mMutex.unlock();
			if (!lc_sign)
				return false;
		}
		else
		{
			this->mMutex.lock();
			while (::InterlockedExchangeAdd(&this->pAtmcVar->lWriteCount, 0) != 0)
				this->mEvent.waitEvent();
			::InterlockedIncrement(&this->pAtmcVar->lReadCount);
			this->mMutex.unlock();
		}

		const_cast<TFUNC&>(_mFunc)();

		if (::InterlockedDecrement(&this->pAtmcVar->lReadCount) == 0)
			this->mEvent.setEvent();
		return true;
	}
public:
	TxSysUniqueSharedLock()
	{
		TxSysDebugObjectMemory::malloc(this);
		this->pAtmcVar = (tagInterLockedVar*)::_aligned_malloc(sizeof(tagInterLockedVar), 64);
		::memset(this->pAtmcVar, 0, sizeof(*this->pAtmcVar));
	}
	~TxSysUniqueSharedLock()
	{
		TxSysDebugObjectMemory::free(this);
		::_aligned_free(this->pAtmcVar);
	}
	template<class TFUNC>
	bool tryUniqueLock(int _msec, const TFUNC& _mFunc)
	{
		if (_msec < 0)
			_msec = 0;
		return this->__uniqueLock_(_msec, T_FUNC(_func, _arg));
	}
	template<class TFUNC>
	bool trySharedLock(int _msec, const TFUNC& _mFunc)
	{
		if (_msec < 0)
			_msec = 0;
		return this->__sharedLock_(_msec, T_FUNC(_func, _arg));
	}
	template<class TFUNC> void uniqueLock(const TFUNC& _mFunc) { this->__uniqueLock_(-1, _mFunc); }
	template<class TFUNC> void sharedLock(const TFUNC& _mFunc) { this->__sharedLock_(-1, _mFunc); }
	void uniqueLock(void(*_func)(void*), void *_arg) { this->__uniqueLock_(-1, T_FUNC(_func, _arg)); }
	void sharedLock(void(*_func)(void*), void *_arg) { this->__sharedLock_(-1, T_FUNC(_func, _arg)); }
};


//--------------------------------------TxSysFile--------------------------------------//
class TxSysFile : private TxForbidDirectCopyObject
{
private:
	FILE *pfile;
	std::wstring filepath;
public:
	TxSysFile()
	{
		TxSysDebugObjectMemory::malloc(this);
		this->pfile = NULL;
	}
	~TxSysFile()
	{
		TxSysDebugObjectMemory::free(this);
		this->close();
	}
	inline bool is_open() const
	{
		return this->pfile != NULL;
	}
	bool open(const std::wstring &_file, const char *_rwMode)
	{
		this->close();
		this->filepath = _file;
		this->pfile = ::_wfopen(_file.c_str(), TxBlobString(_rwMode).utf8Codecvt().c_str());
		if (!this->is_open())
		{
			sysLogWarn("_wfopen()执行失败，_file=[{0}],_bWite=[{1}]").args(
				this->filepath, TxBlobString(_rwMode).utf8Codecvt());
			this->filepath.clear();
			return false;
		}
		return true;
	}
	void close()
	{
		if (this->pfile != NULL)
		{
			int res = ::fclose(this->pfile);
			if (res != 0)
			{
				sysLogWarn("fclose()执行失败，this->filepath=[{0}],res=[{1}]").args(this->filepath, res);
			}
			this->pfile = NULL;
			this->filepath.clear();
		}
	}
	long long tell()
	{
		long long res = 0;
		if (this->pfile != NULL)
		{
			res = ::_ftelli64(this->pfile);
			if (res < 0)
			{
				sysLogError("_ftelli64()执行失败，this->filepath=[{0}]],res=[{1}]").args(this->filepath, res);
				res = 0;
			}
		}
		return res;
	}
	bool seekToEnd()
	{
		return this->seek(0, SEEK_END);
	}
	bool seekToBegin()
	{
		return this->seek(0, SEEK_SET);
	}
	bool seek(long long _offset, int _origin)
	{
		if (this->pfile != NULL)
		{
			int res = ::_fseeki64(this->pfile, (__int64)_offset, _origin);
			if (res != 0)
			{
				sysLogError("_fseeki64()执行失败，_file=[{0}],_offset=[{1]},_origin=[{2}],res=[{3}]").args(
					this->filepath, _offset, _origin, res);
			}
		}
		return true;
	}
	int read(void *_buffer, int _size)
	{
		int res = 0;
		if (this->pfile == NULL || _buffer == NULL || _size <= 0)
		{
			sysLogError("参数错误，this->filepath=[{0}],this->pfile=[{1}],_buffer=[{2]},_size=[{3}]").args(
				this->filepath, (size_t)this->pfile, _size, res);
		}
		else
		{
			res = (int)::fread(_buffer, 1, _size, this->pfile);
			if (res < 0 || res > _size)
			{
				sysLogError("fread()执行失败，this->filepath=[{0}],_size=[{1]},res=[{2}]").args(this->filepath, _size, res);
				res = 0;
			}
		}
		return res;
	}
	bool write(const void *_buffer, int _size)
	{
		if (_size == 0)
			return this->pfile != NULL;
		if (this->pfile == NULL || _buffer == NULL || _size < 0)
		{
			sysLogError("参数错误，this->filepath=[{0}],this->pfile=[{1}],_buffer=[{2}],_size=[{3}]").args(
				this->filepath, (size_t)this->pfile, (size_t)_buffer, _size);
			return false;
		}
		int res = (int)::fwrite(_buffer, 1, _size, this->pfile);
		if (res < 0 || res != _size)
		{
			sysLogError("fwrite()执行失败，this->filepath=[{0}],_size=[{1}],res=[{2}]").args(this->filepath, _size, res);
			return false;
		}
		return true;
	}
};


//--------------------------------------TxSysSerialPort--------------------------------------//
class TxSysSerialPort
{
public:
	enum { BUFFER_SIZE = 8 * 1024, };
private:
	HANDLE hComHandle;
private:
	static std::wstring gSettleComName_(const wchar_t *_com)
	{
		std::wstring ret;
		while (_com != NULL && ((*_com) == L' ' || (*_com) == L'\t' || (*_com) == L'\r' || (*_com) == L'\n'))
			_com++;
		if (_com == NULL) _com = L"";
		if (_com[0] == L'c' || _com[0] == L'C')
			ret.append(L"\\\\.\\");
		ret.append(_com);
		return ret;
	}
public:
	static std::vector<std::string> availablePorts()
	{
		std::list<std::string> ret;
		HKEY lc_hKey = NULL;
		if (::RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm",
			NULL, KEY_READ, &lc_hKey) == ERROR_SUCCESS)  //Open the serial port registry
		{
			for (int it = 0;; ++it)
			{
				enum { e_str_size = 128, };
				char lc_portName[e_str_size] = { 0 }, lc_commName[e_str_size] = { 0 };
				DWORD lc_dwSize = e_str_size, lc_dwLong = e_str_size;
				if (::RegEnumValueA(lc_hKey, it, lc_portName, &lc_dwLong,
					NULL, NULL, (PUCHAR)lc_commName, &lc_dwSize) == ERROR_NO_MORE_ITEMS)
				{
					break;
				}
				ret.push_back(std::string(lc_commName));
			}
			::RegCloseKey(lc_hKey);
		}
		ret.sort([](const std::string &_str1, const std::string &_str2) {
			return (_str1.length() != _str2.length()) ? (_str1.length() < _str2.length()) : (_str1 < _str2);
		});
		return std::vector<std::string>(ret.begin(), ret.end());
	}
	static std::vector<int> availableBauds()
	{
		std::vector<int> ret;
		int baud[] = { CBR_110, CBR_300, CBR_600, CBR_1200, CBR_2400, CBR_4800, CBR_9600,
			CBR_14400, CBR_19200, CBR_38400, CBR_56000, CBR_57600, CBR_115200, CBR_128000, CBR_256000 };
		for (int i = 0; i < sizeof(baud) / sizeof(baud[0]); i++)
			ret.push_back(baud[i]);
		return ret;
	}
public:
	TxSysSerialPort()
	{
		TxSysDebugObjectMemory::malloc(this);
		this->hComHandle = INVALID_HANDLE_VALUE;
	}
	~TxSysSerialPort()
	{
		TxSysDebugObjectMemory::free(this);
		this->close();
	}
	bool open(const char *_com, int _baud)
	{
		wchar_t lc_unicode[32] = { 0 };
		if (_com != NULL)
		{
			for (int i = 0; _com[i] != 0 && i < sizeof(lc_unicode) / sizeof(lc_unicode[0]) - 1; ++i)
				lc_unicode[i] = _com[i];
		}
		lc_unicode[sizeof(lc_unicode) / sizeof(lc_unicode[0]) - 1] = 0;
		return this->open(lc_unicode, _baud);
	}
	bool open(const wchar_t *_com, int _baud)
	{
		do {
			this->close();
			if (true)
			{//test parameters
				if (_com == NULL || _com[0] == 0 || _baud <= 0)
				{
					sysLogError("参数错误。_com=[{0}]，_baud=[{1}]。").args((size_t)_com, _baud);
					break;
				}
			}
			if (true)
			{//Open Uart
				std::wstring lc_com = gSettleComName_(_com);
				if (lc_com.length() > 0)
				{
					this->hComHandle = ::CreateFileW(lc_com.c_str(),
						GENERIC_READ | GENERIC_WRITE, //Allow to read and write
						0, //exclusive mode
						NULL,
						OPEN_EXISTING, //Open And No Create
						0,
						NULL);
				}
				if (this->hComHandle == INVALID_HANDLE_VALUE)
					break;
			}
			if (true)
			{//Set baud rate, and other properties
				DCB lc_dcb = { 0 };
				lc_dcb.DCBlength = sizeof(lc_dcb);
				if (!::GetCommState(this->hComHandle, &lc_dcb))
				{
					sysLogError("GetCommState()执行失败，com=[{0}]，baud=[{1}]").args(_com, _baud);
					break;
				}
				lc_dcb.DCBlength = sizeof(lc_dcb);
				lc_dcb.BaudRate = (DWORD)_baud;//baud
				lc_dcb.fBinary = TRUE;//Whether to allow transfer the binary
				lc_dcb.fParity = FALSE;//odd-even check
				lc_dcb.ByteSize = 8;//data bits
				lc_dcb.Parity = NOPARITY;//odd-even check mode
				lc_dcb.StopBits = ONESTOPBIT;//A stop bit
				if (!::SetCommState(this->hComHandle, &lc_dcb))
				{
					sysLogError("SetCommState()执行失败，com=[{0}]，baud=[{1}]").args(_com, _baud);
					break;
				}
			}
			if (true)
			{//reading and writing timeouts
				COMMTIMEOUTS lcTimeOuts = { 0 };
				if (!::GetCommTimeouts(this->hComHandle, &lcTimeOuts))
				{
					sysLogError("GetCommTimeouts()执行失败，com=[{0}]，baud=[{1}]").args(_com, _baud);
					break;
				}
				lcTimeOuts.ReadIntervalTimeout = 10;
				lcTimeOuts.ReadTotalTimeoutMultiplier = 0;
				lcTimeOuts.ReadTotalTimeoutConstant = 10;
				if (!::SetCommTimeouts(this->hComHandle, &lcTimeOuts))
				{
					sysLogError("SetCommTimeouts()执行失败，com=[{0}]，baud=[{1}]").args(_com, _baud);
					break;
				}
			}
			if (true)
			{//Set the buffer size
				if (!::SetupComm(this->hComHandle, BUFFER_SIZE, BUFFER_SIZE))
				{
					sysLogError("SetupComm()执行失败，com=[{0}]，baud=[{1}]").args(_com, _baud);
					break;
				}
			}
			if (true)
			{//Clear
				COMSTAT lc_ComStat = { 0 };
				DWORD lc_dwErrorFlags = 0;
				if (!::ClearCommError(this->hComHandle, &lc_dwErrorFlags, &lc_ComStat))
				{
					sysLogError("ClearCommError()执行失败，com=[{0}]，baud=[{1}]").args(_com, _baud);
					break;
				}
				if (!::PurgeComm(this->hComHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR))
				{
					sysLogError("PurgeComm()执行失败，com=[{0}]，baud=[{1}]").args(_com, _baud);
					break;
				}
			}
			return true;
		} while (false);
		this->close();
		return false;
	}
	void clearBuffer()
	{
		if (this->hComHandle != INVALID_HANDLE_VALUE)
		{
			COMSTAT lc_ComStat = { 0 };
			DWORD lc_dwErrorFlags = 0;
			::ClearCommError(this->hComHandle, &lc_dwErrorFlags, &lc_ComStat);
			::PurgeComm(this->hComHandle, PURGE_RXCLEAR | PURGE_RXABORT);
			::ClearCommError(this->hComHandle, &lc_dwErrorFlags, &lc_ComStat);
		}
	}
	void close()
	{
		if (this->hComHandle != INVALID_HANDLE_VALUE)
		{
			COMSTAT lc_ComStat = { 0 };
			DWORD lc_dwErrorFlags = 0;
			::ClearCommError(this->hComHandle, &lc_dwErrorFlags, &lc_ComStat);
			::PurgeComm(this->hComHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
			::ClearCommError(this->hComHandle, &lc_dwErrorFlags, &lc_ComStat);
			::CloseHandle(this->hComHandle);
			this->hComHandle = INVALID_HANDLE_VALUE;
			::Sleep((DWORD)(0.2 * 1000));
		}
	}
	inline bool is_open() const
	{
		return this->hComHandle != INVALID_HANDLE_VALUE;
	}
	int read(void *_buffer, int _size)
	{
		COMSTAT lc_ComStat = { 0 };
		DWORD lc_dwErrorFlags = 0;
		DWORD dwLcReadLen = 0;
		if (this->hComHandle != INVALID_HANDLE_VALUE
			&&::ClearCommError(this->hComHandle, &lc_dwErrorFlags, &lc_ComStat)
			&& ::ReadFile(this->hComHandle, _buffer, _size, &dwLcReadLen, NULL))
		{
			return (int)dwLcReadLen;
		}
		this->close();
		return -1;
	}
	bool write(const void *_buffer, int _size)
	{
		COMSTAT lc_ComStat = { 0 };
		DWORD lc_dwErrorFlags = 0;
		DWORD lcWriteCount = 0;
		bool ret = (this->hComHandle != INVALID_HANDLE_VALUE
			&& ::ClearCommError(this->hComHandle, &lc_dwErrorFlags, &lc_ComStat)
			&& ::WriteFile(this->hComHandle, _buffer, _size, &lcWriteCount, NULL)
			&& ::ClearCommError(this->hComHandle, &lc_dwErrorFlags, &lc_ComStat)
			&& ::PurgeComm(this->hComHandle, PURGE_TXCLEAR)
			&& (int)_size == (int)lcWriteCount);
		if (!ret)
			this->close();
		return ret;
	}
};


//--------------------------------------TxSysProcessPeriod--------------------------------------//
class TxSysProcessPeriod : private TxForbidDirectCopyObject
{
private:
	struct tagSharedMemorData {
		DWORD dwProcessId;
		std::time_t launch_time;
		unsigned char reserve__[64];
	};
	struct {
		TxSysProcessMemory* pMemory;
		TxSysProcessMutex<false>* pMutex;
		DWORD self_dwProcessId;
		std::time_t self_launch_time;
		tagSharedMemorData *pSharedMemorData;
	} mMsgInfo;
	TxSysProcessMutex<false>* pProcessMutex;
	DWORD dwMainThreadId;//Only used to judge the main thread
	TxSysMutex mutex;
private:
	TxSysProcessPeriod()
	{
		this->dwMainThreadId = ::GetCurrentThreadId();
		this->mMsgInfo.self_dwProcessId = ::GetCurrentProcessId();
		std::time(&this->mMsgInfo.self_launch_time);
		this->pProcessMutex = NULL;
		this->mMsgInfo.pMutex = new TxSysProcessMutex<false>(this->get_proc_key_("mtx").c_str());
		this->mMsgInfo.pMemory = new TxSysProcessMemory();
		this->mMsgInfo.pMemory->initialize(NULL, this->get_proc_key_("mem").c_str(), sizeof(*this->mMsgInfo.pSharedMemorData));
		this->mMsgInfo.pSharedMemorData = (tagSharedMemorData*)this->mMsgInfo.pMemory->getMemoryPtr();
	}
	~TxSysProcessPeriod()
	{
		this->_leave_();
	}
	bool _enter__()
	{
		bool ret_true = true;
		this->mutex.lock();
		if (this->mMsgInfo.pSharedMemorData == NULL || this->mMsgInfo.pMutex == NULL || this->mMsgInfo.pMemory == NULL)
		{
			sysLogError("if (this->mMsgInfo.pSharedMemorData == NULL || this->mMsgInfo.pMutex == NULL || this->mMsgInfo.pMemory == NULL)错误，this->mMsgInfo.pSharedMemorData=[{0}],this->mMsgInfo.pMutex=[{1}],this->mMsgInfo.pMemory={[2]}").args(
				(size_t)this->mMsgInfo.pSharedMemorData, (size_t)this->mMsgInfo.pMutex, (size_t)this->mMsgInfo.pMemory);
		}
		else
		{
			this->mMsgInfo.pMutex->lock();
			this->mMsgInfo.pSharedMemorData->dwProcessId = this->mMsgInfo.self_dwProcessId;
			this->mMsgInfo.pSharedMemorData->launch_time = this->mMsgInfo.self_launch_time;
			this->mMsgInfo.pMutex->unlock();
		}

		if (this->pProcessMutex == NULL)
		{
			this->pProcessMutex = new TxSysProcessMutex<false>(get_proc_key_("prg_mtx").c_str());
			if (!this->pProcessMutex->lock(500) && !this->pProcessMutex->lock(500))
			{
				delete this->pProcessMutex;
				this->pProcessMutex = NULL;
				ret_true = false;
			}
		}
		this->mutex.unlock();
		return ret_true;
	}
	bool _is_triggered_()
	{
		bool ret = false;
		this->mutex.lock();
		if (this->mMsgInfo.pMutex != NULL || this->mMsgInfo.pSharedMemorData != NULL)
		{
			this->mMsgInfo.pMutex->lock();
			if (this->mMsgInfo.pSharedMemorData->dwProcessId != this->mMsgInfo.self_dwProcessId)
			{
				if (std::difftime(this->mMsgInfo.pSharedMemorData->launch_time, this->mMsgInfo.self_launch_time) > 6)
					ret = true;
				this->mMsgInfo.pSharedMemorData->dwProcessId = this->mMsgInfo.self_dwProcessId;
				this->mMsgInfo.pSharedMemorData->launch_time = this->mMsgInfo.self_launch_time;
			}
			this->mMsgInfo.pMutex->unlock();
		}
		this->mutex.unlock();
		return ret;
	}
	void _leave_()
	{
		this->mutex.lock();
		if (this->mMsgInfo.pMemory != NULL)
		{
			delete this->mMsgInfo.pMemory;
			this->mMsgInfo.pMemory = NULL;
		}
		if (this->mMsgInfo.pMutex != NULL)
		{
			delete this->mMsgInfo.pMutex;
			this->mMsgInfo.pMutex = NULL;
		}
		this->mMsgInfo.pSharedMemorData = NULL;

		if (this->pProcessMutex != NULL)
		{
			this->pProcessMutex->unlock();
			delete this->pProcessMutex;
			this->pProcessMutex = NULL;
		}
		this->mutex.unlock();
	}
	bool _is_alive_() const
	{
		bool ret = false;
		if (this->pProcessMutex != NULL)
			ret = true;
		return ret;
	}
	TxBlobString get_proc_key_(const TxBlobString &_pre)
	{
		const wchar_t *lc_proc_name = NULL;
		TxBlobWString lc_path(_TxPrivateNs_::CSyslib::getWorkingDirectory(&lc_proc_name));
		lc_path.append(lc_proc_name);
		lc_path.replaceSelf(L'\\', L'/');
		lc_path.lowerCaseSelf();
		lc_path.append(".TxSysProcessPeriod.instance.enqueue.process.locker");
		return _pre + TxStdlib::calcMd5(lc_path.c_str(), lc_path.length() * sizeof(lc_path.data()[0]));
	}
	static TxSysProcessPeriod* _init_env_()
	{
		static TxSysProcessPeriod ret;
		return &ret;
	}
public:
	static bool enter() { return _init_env_()->_enter__(); }
	static void leave() { return _init_env_()->_leave_(); }
	static bool is_triggered() { return _init_env_()->_is_triggered_(); }
	static bool is_alive() { return _init_env_()->_is_alive_(); }
};


//--------------------------------------TxSysRegisterEncryption--------------------------------------//
namespace _TxPrivateNs_ {
	class CDiskInfo_
	{
	public:
		typedef struct tagDiskInfo {
			TxBlobString sSerialNumber;
			TxBlobString sModelNumber;
			TxBlobString toString() const
			{
				return this->sModelNumber + "," + this->sSerialNumber;
			}
		} TDiskInfo;
	private:
#pragma pack(push)
#pragma pack(1)
		typedef struct _GETVERSIONOUTPARAMS
		{
			BYTE bVersion;      // Binary driver version.   
			BYTE bRevision;     // Binary driver revision.   
			BYTE bReserved;     // Not used.   
			BYTE bIDEDeviceMap; // Bit map of IDE devices.   
			DWORD fCapabilities; // Bit mask of driver capabilities.   
			DWORD dwReserved[4]; // For future use.   
		} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;
		typedef struct _IDSECTOR
		{
			USHORT  wGenConfig;
			USHORT  wNumCyls;
			USHORT  wReserved;
			USHORT  wNumHeads;
			USHORT  wBytesPerTrack;
			USHORT  wBytesPerSector;
			USHORT  wSectorsPerTrack;
			USHORT  wVendorUnique[3];
			CHAR    sSerialNumber[20];
			USHORT  wBufferType;
			USHORT  wBufferSize;
			USHORT  wECCSize;
			CHAR    sFirmwareRev[8];
			CHAR    sModelNumber[40];
			USHORT  wMoreVendorUnique;
			USHORT  wDoubleWordIO;
			USHORT  wCapabilities;
			USHORT  wReserved1;
			USHORT  wPIOTiming;
			USHORT  wDMATiming;
			USHORT  wBS;
			USHORT  wNumCurrentCyls;
			USHORT  wNumCurrentHeads;
			USHORT  wNumCurrentSectorsPerTrack;
			ULONG   ulCurrentSectorCapacity;
			USHORT  wMultSectorStuff;
			ULONG   ulTotalAddressableSectors;
			USHORT  wSingleWordDMA;
			USHORT  wMultiWordDMA;
			BYTE    bReserved[128];
		} IDSECTOR, *PIDSECTOR;
		typedef struct _SRB_IO_CONTROL
		{
			ULONG HeaderLength;
			UCHAR Signature[8];
			ULONG Timeout;
			ULONG ControlCode;
			ULONG ReturnCode;
			ULONG Length;
		} SRB_IO_CONTROL, *PSRB_IO_CONTROL;
		typedef struct _MEDIA_SERAL_NUMBER_DATA {
			ULONG  SerialNumberLength;
			ULONG  Result;
			ULONG  Reserved[2];
			UCHAR  SerialNumberData[1];
		} MEDIA_SERIAL_NUMBER_DATA, *PMEDIA_SERIAL_NUMBER_DATA;
		typedef struct _rt_IdeDInfo_
		{
			BYTE IDEExists[4];
			BYTE DiskExists[8];
			WORD DisksRawInfo[8 * 256];
		} rt_IdeDInfo, *pt_IdeDInfo;
		typedef struct _rt_DiskInfo_
		{
			BOOL DiskExists;
			BOOL ATAdevice;
			BOOL RemovableDevice;
			WORD TotLogCyl;
			WORD TotLogHeads;
			WORD TotLogSPT;
			char SerialNumber[20];
			char FirmwareRevision[8];
			char ModelNumber[40];
			WORD CurLogCyl;
			WORD CurLogHeads;
			WORD CurLogSPT;
		} rt_DiskInfo;
#pragma pack(pop)

	private:
		//IOCTL commands   
		static const std::int32_t DFP_GET_VERSION = 0x00074080;
		static const std::int32_t DFP_SEND_DRIVE_COMMAND = 0x0007c084;
		static const std::int32_t DFP_RECEIVE_DRIVE_DATA = 0x0007c088;
		static const std::int32_t FILE_DEVICE_SCSI = 0x0000001b;
		static const std::int32_t IOCTL_SCSI_MINIPORT_IDENTIFY = ((FILE_DEVICE_SCSI << 16) + 0x0501);
		static const std::int32_t IOCTL_SCSI_MINIPORT = 0x0004D008;//  see NTDDSCSI.H for definition   
																   //Bits returned in the fCapabilities member of GETVERSIONOUTPARAMS    
		static const std::int32_t CAP_IDE_ID_FUNCTION = 1;  // ATA ID command supported   
		static const std::int32_t CAP_IDE_ATAPI_ID = 2;  // ATAPI ID command supported   
		static const std::int32_t CAP_IDE_EXECUTE_SMART_FUNCTION = 4;  // SMART commannds supported   
																	   //IDE
		static const std::int32_t IDE_ATAPI_IDENTIFY = 0xA1;
		static const std::int32_t IDE_ATA_IDENTIFY = 0xEC;
		static const std::int32_t MAX_IDE_DRIVES = 16;
		//
		static const std::int32_t SENDIDLENGTH = sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE;
	private:
		std::list<TDiskInfo> listDiskInfo;
		BOOL bOnlyMajor;
	private:
		static bool _gIsLegalSerialNumber(const TxBlobString &_sSerialNumber)
		{
			auto fn_isalnum = [](char _ch)->bool {
				return ('0' <= (_ch) && (_ch) <= '9')
					|| ('A' <= (_ch) && (_ch) <= 'Z')
					|| ('a' <= (_ch) && (_ch) <= 'z');
			};
			return ((_sSerialNumber.size() > 0 && fn_isalnum(_sSerialNumber.at(0)))
				|| (_sSerialNumber.size() > 19 && fn_isalnum(_sSerialNumber.at(19))));
		}
		static TxBlobString gConvertToString_(DWORD diskdata[256], int firstIndex, int lastIndex)
		{
			TxBlobString ret;
			for (int index = firstIndex; index <= lastIndex; index++)
			{
				char lc_chs[] = {
					(char)(diskdata[index] / 256),(char)(diskdata[index] % 256)
				};
				for (int ii = 0; ii < TxStdlib::arrayLength(lc_chs); ++ii)
					ret.append(&lc_chs[ii], 1);
			}
			return ret.trimLR("\r\n\t ");
		}
		void _gWriteByDiskData(int _drive, DWORD(&_diskdata)[256])
		{
			if (_diskdata[0] & 0x0040)
			{//固定硬盘
				TDiskInfo lcDiskInfo;
				lcDiskInfo.sSerialNumber = gConvertToString_(_diskdata, 10, 19);
				lcDiskInfo.sModelNumber = gConvertToString_(_diskdata, 27, 46);
				if (!this->isFinish_() && _gIsLegalSerialNumber(lcDiskInfo.sSerialNumber))
					this->listDiskInfo.push_back(lcDiskInfo);
			}
		}
		BOOL _DoIDENTIFY_(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
			PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
			PDWORD lpcbBytesReturned)
		{
			// Set up data structures for IDENTIFY command.   
			pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;
			pSCIP->irDriveRegs.bFeaturesReg = 0;
			pSCIP->irDriveRegs.bSectorCountReg = 1;
			//pSCIP -> irDriveRegs.bSectorNumberReg = 1;   
			pSCIP->irDriveRegs.bCylLowReg = 0;
			pSCIP->irDriveRegs.bCylHighReg = 0;
			// Compute the drive number.   
			pSCIP->irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);
			// The command can either be IDE identify or ATAPI identify.   
			pSCIP->irDriveRegs.bCommandReg = bIDCmd;
			pSCIP->bDriveNumber = bDriveNum;
			pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;
			return ::DeviceIoControl(hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
				(LPVOID)pSCIP,
				sizeof(SENDCMDINPARAMS) - 1,
				(LPVOID)pSCOP,
				sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
				lpcbBytesReturned, NULL);
		}
		void _ReadPhysicalDriveInNTWithAdminRights_()
		{
			for (int drive = 0; drive < MAX_IDE_DRIVES && !this->isFinish_(); drive++)
			{
				HANDLE lc_hPhysicalDriveIOCTL = 0;
				char driveName[256];
				sprintf_s(driveName, "\\\\.\\PhysicalDrive%d", drive);
				lc_hPhysicalDriveIOCTL = CreateFileA(driveName,
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
					OPEN_EXISTING, 0, NULL);
				if (lc_hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE&&lc_hPhysicalDriveIOCTL != NULL)
				{
					GETVERSIONOUTPARAMS VersionParams;
					DWORD cbBytesReturned = 0;
					memset((void*)&VersionParams, 0, sizeof(VersionParams));
					if (::DeviceIoControl(lc_hPhysicalDriveIOCTL, DFP_GET_VERSION,
						NULL,
						0,
						&VersionParams,
						sizeof(VersionParams),
						&cbBytesReturned, NULL))
					{
						if (VersionParams.bIDEDeviceMap > 0)
						{
							BYTE             bIDCmd = 0;
							SENDCMDINPARAMS  scip;
							bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;
							memset(&scip, 0, sizeof(scip));
							BYTE IdOutCmd[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1] = { 0 };
							memset(IdOutCmd, 0, sizeof(IdOutCmd));
							if (_DoIDENTIFY_(lc_hPhysicalDriveIOCTL,
								&scip,
								(PSENDCMDOUTPARAMS)&IdOutCmd,
								(BYTE)bIDCmd,
								(BYTE)drive,
								&cbBytesReturned))
							{
								DWORD diskdata[256];
								int ijk = 0;
								USHORT *pIdSector = (USHORT *)
									((PSENDCMDOUTPARAMS)IdOutCmd)->bBuffer;
								for (ijk = 0; ijk < 256; ijk++)
									diskdata[ijk] = pIdSector[ijk];
								_gWriteByDiskData(drive, diskdata);
							}
						}
					}
					::CloseHandle(lc_hPhysicalDriveIOCTL);
				}
			}
		}
		void _ReadPhysicalDriveInNTWithZeroRights_()
		{
			for (int drive = 0; drive < MAX_IDE_DRIVES && !this->isFinish_(); drive++)
			{
				HANDLE lc_hPhysicalDriveIOCTL = 0;
				char driveName[256];
				sprintf_s(driveName, "\\\\.\\PhysicalDrive%d", drive);
				lc_hPhysicalDriveIOCTL = CreateFileA(driveName, 0,
					FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
					OPEN_EXISTING, 0, NULL);
				if (lc_hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE&&lc_hPhysicalDriveIOCTL != NULL)
				{
					STORAGE_PROPERTY_QUERY query;
					DWORD cbBytesReturned = 0;
					memset((void *)& query, 0, sizeof(query));
					query.PropertyId = StorageDeviceProperty;
					query.QueryType = PropertyStandardQuery;
					char lc_buffer[10000] = { 0 };
					if (::DeviceIoControl(lc_hPhysicalDriveIOCTL, IOCTL_STORAGE_QUERY_PROPERTY,
						&query,
						sizeof(query),
						&lc_buffer,
						sizeof(lc_buffer),
						&cbBytesReturned, NULL))
					{
						STORAGE_DEVICE_DESCRIPTOR * descrip = (STORAGE_DEVICE_DESCRIPTOR *)&lc_buffer;
						TDiskInfo lcDiskInfo;
						lcDiskInfo.sSerialNumber = TxBlobString(&lc_buffer[descrip->SerialNumberOffset]).trimLR("\r\n\t ");
						lcDiskInfo.sModelNumber = TxBlobString(&lc_buffer[descrip->ProductIdOffset]).trimLR("\r\n\t ");
						if (!this->isFinish_() && _gIsLegalSerialNumber(lcDiskInfo.sSerialNumber) && !descrip->RemovableMedia)
							this->listDiskInfo.push_back(lcDiskInfo);
					}
					::CloseHandle(lc_hPhysicalDriveIOCTL);
				}
			}
		}
		void _ReadDrivePortsInWin9X_(void)
		{
			HANDLE VxDHandle = 0;
			pt_IdeDInfo pOutBufVxD = 0;
			DWORD lpBytesReturned = 0;
			BOOL status = ::SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
			rt_IdeDInfo info;
			pOutBufVxD = &info;
			ZeroMemory(&info, sizeof(info));
			VxDHandle = CreateFileA("\\\\.\\IDE21201.VXD", 0, 0, 0,
				0, FILE_FLAG_DELETE_ON_CLOSE, 0);
			if (VxDHandle != INVALID_HANDLE_VALUE)
			{
				::DeviceIoControl(VxDHandle, 1,
					0, 0, pOutBufVxD, sizeof(pt_IdeDInfo), &lpBytesReturned, 0);
				::CloseHandle(VxDHandle);
			}
			for (int i = 0; i < 8 && !this->isFinish_(); i++)
			{
				if ((pOutBufVxD->DiskExists[i]) && (pOutBufVxD->IDEExists[i / 2]))
				{
					DWORD diskinfo[256];
					for (int j = 0; j < 256; j++)
						diskinfo[j] = pOutBufVxD->DisksRawInfo[i * 256 + j];
					_gWriteByDiskData(i, diskinfo);
				}
			}
			::SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
		}
		void _ReadIdeDriveAsScsiDriveInNT_()
		{
			for (int controller = 0; controller < 16 && !this->isFinish_(); controller++)
			{
				char   driveName[256];
				sprintf_s(driveName, "\\\\.\\Scsi%d:", controller);
				HANDLE hScsiDriveIOCTL = CreateFileA(driveName,
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
					OPEN_EXISTING, 0, NULL);
				if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE)
				{
					for (int drive = 0; drive < 2; drive++)
					{
						char buffer[sizeof(SRB_IO_CONTROL) + SENDIDLENGTH];
						SRB_IO_CONTROL *p = (SRB_IO_CONTROL *)buffer;
						SENDCMDINPARAMS *pin =
							(SENDCMDINPARAMS *)(buffer + sizeof(SRB_IO_CONTROL));
						DWORD dummy;

						memset(buffer, 0, sizeof(buffer));
						p->HeaderLength = sizeof(SRB_IO_CONTROL);
						p->Timeout = 10000;
						p->Length = SENDIDLENGTH;
						p->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
						strncpy_s((char *)p->Signature, 9, "SCSIDISK", 8);

						pin->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
						pin->bDriveNumber = drive;
						if (DeviceIoControl(hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT,
							buffer,
							sizeof(SRB_IO_CONTROL) +
							sizeof(SENDCMDINPARAMS) - 1,
							buffer,
							sizeof(SRB_IO_CONTROL) + SENDIDLENGTH,
							&dummy, NULL))
						{
							SENDCMDOUTPARAMS *pOut =
								(SENDCMDOUTPARAMS *)(buffer + sizeof(SRB_IO_CONTROL));
							IDSECTOR *pId = (IDSECTOR *)(pOut->bBuffer);
							if (pId->sModelNumber[0])
							{
								DWORD diskdata[256];
								int ijk = 0;
								USHORT *pIdSector = (USHORT *)pId;

								for (ijk = 0; ijk < 256; ijk++)
									diskdata[ijk] = pIdSector[ijk];
								_gWriteByDiskData(controller * 2 + drive, diskdata);
							}
						}
					}
					::CloseHandle(hScsiDriveIOCTL);
				}
			}
		}
		CDiskInfo_()
		{
		}
		~CDiskInfo_()
		{
		}
		bool isFinish_()
		{
			return !!this->bOnlyMajor&&this->listDiskInfo.size() > 0;
		}
	public:
		static void getDriveInfo(std::list<TDiskInfo> *_listDiskInfo, bool _bOnlyMajor)
		{
			OSVERSIONINFOW version = { 0 };
			version.dwOSVersionInfoSize = sizeof(version);
			GetVersionExW(&version);
			CDiskInfo_ lc_disk;
			lc_disk.bOnlyMajor = !!_bOnlyMajor;
			if (version.dwPlatformId == VER_PLATFORM_WIN32_NT)
			{
				if (!lc_disk.isFinish_())
					lc_disk._ReadPhysicalDriveInNTWithAdminRights_();
				if (!lc_disk.isFinish_())
					lc_disk._ReadIdeDriveAsScsiDriveInNT_();
				if (!lc_disk.isFinish_())
					lc_disk._ReadPhysicalDriveInNTWithZeroRights_();
			}
			else
			{
				if (!lc_disk.isFinish_())
					lc_disk._ReadDrivePortsInWin9X_();
			}
			std::map<TxBlobString, TDiskInfo> lc_mapDiskInfo;
			char lc_sep_chs[4] = { 0 };
			TxBlobString lc_sep_str(lc_sep_chs, 1);
			for (auto iter = lc_disk.listDiskInfo.begin(); iter != lc_disk.listDiskInfo.end(); ++iter)
				lc_mapDiskInfo[iter->sModelNumber + lc_sep_str + iter->sSerialNumber] = *iter;
			_listDiskInfo->clear();
			for (auto iter = lc_mapDiskInfo.begin(); iter != lc_mapDiskInfo.end(); ++iter)
				_listDiskInfo->push_back(iter->second);
		}
	};
}
class TxSysRegisterEncryption
{
private:
	static std::string _GetPhyMacAddressByServiceName(const char* _strServiceName)
	{
		std::string lc_strServiceName("//./");
		lc_strServiceName.append(_strServiceName);

		HANDLE lc_hDev = CreateFileA(lc_strServiceName.c_str(), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		if (lc_hDev != INVALID_HANDLE_VALUE)
		{
			DWORD lc_inBuf = OID_802_3_PERMANENT_ADDRESS;
			BYTE lc_outMacBuf[256] = { 0 };
			DWORD   lc_bytesReturned = 0;

			if (::DeviceIoControl(lc_hDev, IOCTL_NDIS_QUERY_GLOBAL_STATS, (LPVOID)&lc_inBuf, sizeof(lc_inBuf),
				lc_outMacBuf, sizeof(lc_outMacBuf) / sizeof(lc_outMacBuf[0]) - 1, &lc_bytesReturned, NULL))
			{
				char lc_buffer[64] = { 0 };
				sprintf(lc_buffer, "%02X-%02X-%02X-%02X-%02X-%02X",
					(unsigned int)lc_outMacBuf[0], (unsigned int)lc_outMacBuf[1], (unsigned int)lc_outMacBuf[2],
					(unsigned int)lc_outMacBuf[3], (unsigned int)lc_outMacBuf[4], (unsigned int)lc_outMacBuf[5]);
				return std::string(lc_buffer);
			}
			::CloseHandle(lc_hDev);
		}
		return std::string();
	}
	static std::list<std::string> _allAdapterMac_(bool _bOnlyMajor)
	{
		std::set<std::string> ret;
		if (true)
		{//方式一
			std::vector<unsigned char> lc_iai_buffer;
			lc_iai_buffer.resize(sizeof(IP_ADAPTER_INFO), 0);
			ULONG lc_iaiSize = (ULONG)lc_iai_buffer.size();
			DWORD lc_dwResult = ::GetAdaptersInfo((IP_ADAPTER_INFO*)lc_iai_buffer.data(), &lc_iaiSize);
			if (lc_dwResult == ERROR_BUFFER_OVERFLOW)
			{
				lc_iai_buffer.resize(lc_iaiSize, 0);
				lc_dwResult = ::GetAdaptersInfo((IP_ADAPTER_INFO*)lc_iai_buffer.data(), &lc_iaiSize);
			}
			if (ERROR_SUCCESS == lc_dwResult)
			{
				for (IP_ADAPTER_INFO *lc_piai = (IP_ADAPTER_INFO*)lc_iai_buffer.data(); lc_piai != NULL; lc_piai = lc_piai->Next)
				{
					if (!_bOnlyMajor || (std::strstr(lc_piai->Description, "Wifi") == NULL
						&&std::strstr(lc_piai->Description, "Wireless") == NULL
						&&std::strstr(lc_piai->Description, "USB") == NULL
						&&std::strstr(lc_piai->Description, "VPN") == NULL))
					{
						std::string lc_mac = _GetPhyMacAddressByServiceName(lc_piai->AdapterName);
						if (lc_mac.length() > 0)
							ret.insert(lc_mac);
					}
				}
				if (_bOnlyMajor&&ret.size() > 0)
					return std::list<std::string>(ret.begin(), ret.end());
			}
		}
		if (true)
		{//方式二
			HKEY hKey_3 = NULL;
			if (::RegOpenKeyW(HKEY_LOCAL_MACHINE, L"SYSTEM\\ControlSet001\\services\\Tcpip\\Parameters\\Adapters", &hKey_3) == ERROR_SUCCESS)
			{
				char lc_szServiceName[MAX_PATH];
				DWORD lc_serviceNameSize = MAX_PATH;
				for (int i = 0; ::RegEnumKeyExA(hKey_3, i, lc_szServiceName, &lc_serviceNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS;
					++i, lc_serviceNameSize = MAX_PATH)
				{
					std::string lc_mac = _GetPhyMacAddressByServiceName(lc_szServiceName);
					if (lc_mac.length() > 0)
						ret.insert(lc_mac);
				}
				::RegCloseKey(hKey_3);
			}
			if (_bOnlyMajor&&ret.size() > 0)
				return std::list<std::string>(ret.begin(), ret.end());
		}
		return std::list<std::string>(ret.begin(), ret.end());
	}
public:
	static std::list<std::string> allDiskInfo()
	{
		std::list<std::string> ret;
		std::list<_TxPrivateNs_::CDiskInfo_::TDiskInfo> lcListDiskInfo;
		_TxPrivateNs_::CDiskInfo_::getDriveInfo(&lcListDiskInfo, false);
		for (auto iter = lcListDiskInfo.begin(); iter != lcListDiskInfo.end(); ++iter)
			ret.push_back(iter->toString());
		return ret;
	}
	static std::string majorDiskInfo()
	{
		std::list<_TxPrivateNs_::CDiskInfo_::TDiskInfo> lcListDiskInfo;
		_TxPrivateNs_::CDiskInfo_::getDriveInfo(&lcListDiskInfo, true);
		if (lcListDiskInfo.size() <= 0)
			return TxBlobString();
		return lcListDiskInfo.front().toString();
	}
	static std::string getCpuId()
	{
		char ret[32] = { 0 };
		std::array<int, 4> lc_cpu_i;
		__cpuid(lc_cpu_i.data(), 0);
		int lc_nIds = lc_cpu_i[0];
		if (lc_nIds > 1)
		{
			std::vector<std::array<int, 4>> lc_outData;
			lc_outData.reserve(lc_nIds);
			for (int i = 0; i <= lc_nIds; ++i)
			{
				__cpuidex(lc_cpu_i.data(), i, 0);
				lc_outData.push_back(lc_cpu_i);
			}
			sprintf(ret, "%08X%08X",
				(unsigned int)lc_outData[1][3], (unsigned int)lc_outData[1][0]);
		}
		return std::string(ret);
	}
	static std::string majorAdapterMac()
	{
		std::list<std::string> lc_ret = _allAdapterMac_(true);
		if (lc_ret.size() > 0)
			return lc_ret.front();
		return std::string();
	}
	static std::list<std::string> allAdapterMac()
	{
		return _allAdapterMac_(false);
	}
};


//--------------------------------------TxSysIocp--------------------------------------//
#define DefIocpTcpApiGetObjectOrgOverLappedPointer_(OrgClass,MemOverLapped,OverLappedPtr) \
	((OrgClass*)(((size_t)(OVERLAPPED*)OverLappedPtr)-((size_t)(OVERLAPPED*)&((OrgClass*)(NULL))->MemOverLapped)))
namespace TxSysIocp {
	class _My_IocpGlobalFunction_
	{
	public:
		template<class T, class TTT>
		inline static T buildDynamicCast(TTT *_ptr)
		{
			assert(_ptr == NULL || (dynamic_cast<T>(_ptr)) != NULL);
			return _ptr != NULL ? (T)_ptr : (T)NULL;
		}
		static void gIocpWsaWSAStartupInitDeal(int _refcount)
		{
			static TxSysAtomicLong lcRefSocketInitCount(0);
			if (_refcount > 0)
			{
				if (lcRefSocketInitCount.increment() == 1)
				{
					WSADATA lcWSAData = { 0 };
					int res = ::WSAStartup(MAKEWORD(2, 2), &lcWSAData);
					if (res != 0)
					{
						sysLogError("WSAStartup()执行失败,res=[{0}]").args(res);
					}
				}
			}
			else if (_refcount < 0)
			{
				if (lcRefSocketInitCount.decrement() == 0)
				{
					::WSACleanup();
				}
			}
			else if (lcRefSocketInitCount.getValue() != 0)
			{
				sysLogError("WSAStartup refer count not equal 0");
			}
			if (lcRefSocketInitCount.getValue() < 0)
			{
				sysLogError("WSAStartup refer count less than 0");
			}
		}
	};

	class MyRecordThreadIdMgr_ : private TxForbidDirectCopyObject
	{
	private:
		struct StructArry
		{
			DWORD dwThreadId;
			DWORD dwThreadIndex;
			inline bool operator<(const StructArry &_tem) { return this->dwThreadId < _tem.dwThreadId; }
		} *arry;
		int count;
		DWORD dwThreadStartIndex;
	public:
		MyRecordThreadIdMgr_()
		{
			this->dwThreadStartIndex = 0;
			this->arry = NULL;
			this->count = 0;
		}
		~MyRecordThreadIdMgr_()
		{
			this->reset(0);
		}
		void reset(int _count)
		{
			if (this->arry != NULL)
				delete[] this->arry;
			this->dwThreadStartIndex = 0;
			this->arry = NULL;
			this->count = 0;
			if (_count > 0)
			{
				this->count = _count;
				this->arry = new StructArry[this->count];
			}
		}
		void pushThisThread(DWORD _threadId)
		{
			this->arry[this->dwThreadStartIndex].dwThreadId = _threadId;
			this->arry[this->dwThreadStartIndex].dwThreadIndex = this->dwThreadStartIndex;
			this->dwThreadStartIndex++;
			if ((int)this->dwThreadStartIndex > (int)this->count)
				sysLogError("add thread is to many");
		}
		void settle()
		{
			if ((int)this->dwThreadStartIndex != this->count)
				sysLogError("thread count is wrong");
			std::sort(&this->arry[0], &this->arry[this->count]);
#ifndef NDEBUG
			for (int i = this->count - 1; (--i) >= 0;)
			{
				if (this->arry[i].dwThreadId == this->arry[i + 1].dwThreadId)
					sysLogError("thread id repeat");
			}
#endif
		}
		inline int getCurrentThreadIndex(DWORD _threadId) const
		{
			StructArry *pArry = this->arry;
			if (pArry[0].dwThreadId <= _threadId&&this->count > 0 && _threadId <= pArry[this->count].dwThreadId)
			{
				int l = 0, r = this->count;
				while (l != r)
				{
					int k = (l + r) >> 1;
					if (pArry[k].dwThreadId < _threadId) l = k + 1;
					else r = k;
				}
				if (pArry[l].dwThreadId == _threadId)
					return pArry[l].dwThreadIndex;
			}
			sysLogError("getCurrentThreadIndex() no this thread id. _threadId=[{0}]").args(_threadId);
#ifndef NDEBUG
			{
				DWORD arry_id[1024] = { 0 };
				int i = sizeof(arry_id) / sizeof(arry_id[0]);
				if (i > count) i = count;
				while (i--) arry_id[i] = this->arry[i].dwThreadId;
				int dfs = 0; dfs = dfs; (void)dfs;
			}
#endif
			return -1;
		}
	};

	struct MyIocpServiceAtmcVariate_
	{
		TxSysAtomicLong lAtmOverLappedAccretionCount;
		TxSysAtomicLong lAtmExistThreadCount;
		TxSysAtomicLong lAtmTriggerStopCount;
		TxSysAtomicLong lAtmThreadRunning;
	};

	class TxIocpService : private TxForbidDirectCopyObject
	{
	public:
		typedef bool(*FuncType)(OVERLAPPED *_obj, int _iNumberOfBytes, bool _bResultStatus);
		struct tagIocpOverlappedWorkBase_
		{
			OVERLAPPED mOVERLAPPED;
			FuncType func;
			void initialize(FuncType _func) { ::memset(&this->mOVERLAPPED, 0, sizeof(this->mOVERLAPPED)); this->func = _func; }
			void reset_overlapped() { ::memset(&this->mOVERLAPPED, 0, sizeof(this->mOVERLAPPED)); }
			void pushQueueFailureWork() { this->func(&this->mOVERLAPPED, 0, 0); }
		};
	private:
		//TxSysMutex mServiceQueueMutex;
		TxSysMutex mGlobalMutex;
		TxSysMutex mRecordThreadIdMgrMutex;
		MyRecordThreadIdMgr_ mRecordThreadIdMgr;
		HANDLE hIoCompletionPort;
		std::vector<std::shared_ptr<TxSysThread>> mListThread;
		void *__tem_1_;
		LONG bThreadRunSustainStatus;
		void *__tem_2_;
	public:
		MyIocpServiceAtmcVariate_ mMyIocpServiceAtmcVariate;
	public:
		void _associationSocket_(SOCKET _mSocket)
		{
			this->serviceQueueMutexLock();
			if (NULL == ::CreateIoCompletionPort((HANDLE)_mSocket, this->hIoCompletionPort, (u_long)0, 0))
				sysLogError("CreateIoCompletionPort[iocp-SOCKET] failure!");
			this->serviceQueueMutexUnlock();
		}
		void _associationSocket_(HANDLE _hSerialPort)
		{
			this->serviceQueueMutexLock();
			if (NULL == ::CreateIoCompletionPort(_hSerialPort, this->hIoCompletionPort, (u_long)0, 0))
				sysLogError("CreateIoCompletionPort[iocp-HANDLE] failure!");
			this->serviceQueueMutexUnlock();
		}
		static void _static_run_(void *_arg)
		{
			TxIocpService* pThis = ((TxIocpService*)_arg);
			pThis->mRecordThreadIdMgrMutex.lock();
			pThis->mRecordThreadIdMgrMutex.unlock();
			pThis->_run_();
		}
		void _run_()
		{
			this->onStartThreadEvent();
			this->mMyIocpServiceAtmcVariate.lAtmTriggerStopCount.increment();
			this->mMyIocpServiceAtmcVariate.lAtmExistThreadCount.increment();
			while (this->bThreadRunSustainStatus
				|| this->mMyIocpServiceAtmcVariate.lAtmOverLappedAccretionCount.getValue() != 0
				|| this->mMyIocpServiceAtmcVariate.lAtmTriggerStopCount.getValue() + 1
				!= this->mMyIocpServiceAtmcVariate.lAtmExistThreadCount.getValue())
			{
				DWORD ulNumberOfBytes = 0;
				OVERLAPPED *pLcOverlapped = NULL;
				ULONG_PTR ulLcCompletionKey = 0;
				BOOL bResultStatus = ::GetQueuedCompletionStatus(this->hIoCompletionPort,
					&ulNumberOfBytes, &ulLcCompletionKey, &pLcOverlapped, INFINITE);
				if (pLcOverlapped == NULL
					|| DefIocpTcpApiGetObjectOrgOverLappedPointer_(tagIocpOverlappedWorkBase_, mOVERLAPPED, pLcOverlapped)->func == NULL)
				{
					sysLogError("GetQueuedCompletionStatus() failure!");
				}
				if (pLcOverlapped != NULL)
				{
					if (!DefIocpTcpApiGetObjectOrgOverLappedPointer_(tagIocpOverlappedWorkBase_, mOVERLAPPED, pLcOverlapped)->func(
						pLcOverlapped, (int)ulNumberOfBytes, !!bResultStatus))
					{
						this->mMyIocpServiceAtmcVariate.lAtmTriggerStopCount.decrement();
					}
				}
			}
			if (this->mMyIocpServiceAtmcVariate.lAtmExistThreadCount.decrement() > 0)
				this->_triggerStop__();
			this->onFinishThreadEvent();
		}
		void _triggerStop__()
		{
			struct tagIocpOverlappedWork_Ext : public tagIocpOverlappedWorkBase_
			{
				TxIocpService *const pTxIocpService;
				tagIocpOverlappedWork_Ext(TxIocpService *_pTxIocpService) :pTxIocpService(_pTxIocpService)
				{
					TxSysDebugObjectMemory::malloc(this);
					this->pTxIocpService->mMyIocpServiceAtmcVariate.lAtmOverLappedAccretionCount.increment();
				}
				~tagIocpOverlappedWork_Ext()
				{
					this->pTxIocpService->mMyIocpServiceAtmcVariate.lAtmOverLappedAccretionCount.decrement();
					TxSysDebugObjectMemory::free(this);
				}
				static bool g_func(OVERLAPPED *_obj, int _iNumberOfBytes, bool _bResultStatus)
				{
					_iNumberOfBytes = _iNumberOfBytes; (void)_iNumberOfBytes; _bResultStatus = _bResultStatus; (void)_bResultStatus;
					tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext
						= DefIocpTcpApiGetObjectOrgOverLappedPointer_(tagIocpOverlappedWork_Ext, mOVERLAPPED, _obj);
					delete pIocpOverlappedWork_Ext;
					return false;
				}
			};
			tagIocpOverlappedWork_Ext *pTxIocpOverlappedWorkBase_Ext = new tagIocpOverlappedWork_Ext(this);
			pTxIocpOverlappedWorkBase_Ext->initialize(&tagIocpOverlappedWork_Ext::g_func);
			this->serviceQueueMutexLock();
			if (this->hIoCompletionPort == NULL
				|| !::PostQueuedCompletionStatus(this->hIoCompletionPort, NULL, NULL, &pTxIocpOverlappedWorkBase_Ext->mOVERLAPPED))
			{
				pTxIocpOverlappedWorkBase_Ext->pushQueueFailureWork();
				sysLogError("triggerStop()::PostQueuedCompletionStatus() failure");
			}
			this->serviceQueueMutexUnlock();
		}
		template<class TFUNC>
		void postTask(const TFUNC &_mfunc)
		{
			struct tagIocpOverlappedWork_Ext : public tagIocpOverlappedWorkBase_
			{
				TFUNC mfunc;
				TxIocpService *const pTxIocpService;
				tagIocpOverlappedWork_Ext(TxIocpService *_pTxIocpService, const TFUNC &_mfunc) :pTxIocpService(_pTxIocpService), mfunc(_mfunc)
				{
					TxSysDebugObjectMemory::malloc(this);
					this->pTxIocpService->mMyIocpServiceAtmcVariate.lAtmOverLappedAccretionCount.increment();
				}
				~tagIocpOverlappedWork_Ext()
				{
					this->pTxIocpService->mMyIocpServiceAtmcVariate.lAtmOverLappedAccretionCount.decrement();
					TxSysDebugObjectMemory::free(this);
				}
				static bool g_func(OVERLAPPED *_obj, int _iNumberOfBytes, bool _bResultStatus)
				{
					_iNumberOfBytes = _iNumberOfBytes; (void)_iNumberOfBytes; _bResultStatus = _bResultStatus; (void)_bResultStatus;
					tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext
						= DefIocpTcpApiGetObjectOrgOverLappedPointer_(tagIocpOverlappedWork_Ext, mOVERLAPPED, _obj);
					if (pIocpOverlappedWork_Ext == NULL)
						sysLogError("postTask()::tagIocpOverlappedWork_Ext::g_func() exception");
					else
					{
						pIocpOverlappedWork_Ext->mfunc();
						delete pIocpOverlappedWork_Ext;
					}
					return true;
				}
			};
			tagIocpOverlappedWork_Ext *pTxIocpOverlappedWorkBase_Ext = new tagIocpOverlappedWork_Ext(this, _mfunc);
			pTxIocpOverlappedWorkBase_Ext->initialize(&tagIocpOverlappedWork_Ext::g_func);
			this->serviceQueueMutexLock();
			if (this->hIoCompletionPort == NULL
				|| !::PostQueuedCompletionStatus(this->hIoCompletionPort, NULL, NULL, &pTxIocpOverlappedWorkBase_Ext->mOVERLAPPED))
			{
				pTxIocpOverlappedWorkBase_Ext->pushQueueFailureWork();
				sysLogError("postTask()::PostQueuedCompletionStatus() failure");
			}
			this->serviceQueueMutexUnlock();
		}
		inline void serviceQueueMutexLock()
		{
			//this->mServiceQueueMutex.lock();
		}
		inline void serviceQueueMutexUnlock()
		{
			//this->mServiceQueueMutex.unlock();
		}
	private:
		virtual void onStartThreadEvent() { }
		virtual void onFinishThreadEvent() { }
	public:
		TxIocpService() : hIoCompletionPort(NULL)
		{
			TxSysDebugObjectMemory::malloc(this);
			addRefWSAStartup();
			this->hIoCompletionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
			if (this->hIoCompletionPort == NULL)
			{
				sysLogError("CreateIoCompletionPort() create iocp object failure");
			}
		}
		virtual ~TxIocpService()
		{
			this->join();
			if (this->hIoCompletionPort != NULL)
			{
				if (!::CloseHandle(this->hIoCompletionPort))
					sysLogError("if (!::CloseHandle(this->hIoCompletionPort))执行失败。");
				this->hIoCompletionPort = NULL;
			}
			if (this->mMyIocpServiceAtmcVariate.lAtmOverLappedAccretionCount.getValue() != 0)
				sysLogError("when quit service, some OverLapped no clear!");
			releaseWSACleanup();
			TxSysDebugObjectMemory::free(this);
		}
		static void addRefWSAStartup() { _My_IocpGlobalFunction_::gIocpWsaWSAStartupInitDeal(1); }
		static void releaseWSACleanup() { _My_IocpGlobalFunction_::gIocpWsaWSAStartupInitDeal(-1); }
		static void checkWSAPerfectStatus() { _My_IocpGlobalFunction_::gIocpWsaWSAStartupInitDeal(0); }
		static int getOptimalThreadNumber()
		{
			int ret = 16;
			SYSTEM_INFO lc_si = { 0 };
			::GetSystemInfo(&lc_si);
			if ((int)lc_si.dwNumberOfProcessors > 0)
				ret = 2 * (int)lc_si.dwNumberOfProcessors;
			return ret;
		}
		void start(int _iThreadCount)
		{
			this->join();
			this->mGlobalMutex.lock();
			this->mListThread.clear();
			this->mListThread.resize(_iThreadCount);
			this->mMyIocpServiceAtmcVariate.lAtmThreadRunning.setValue(1);
			this->bThreadRunSustainStatus = 1;
			this->mRecordThreadIdMgrMutex.lock();
			this->mRecordThreadIdMgr.reset(_iThreadCount);
			for (int i = 0; i < (int)this->mListThread.size(); i++)
			{
				this->mListThread[i].reset(new TxSysThread());
				this->mListThread[i]->create([this]() {_static_run_(this); });
				this->mRecordThreadIdMgr.pushThisThread(this->mListThread[i]->getThreadId());
			}
			this->mRecordThreadIdMgr.settle();
			this->mRecordThreadIdMgrMutex.unlock();
			this->mGlobalMutex.unlock();
		}
		void join()
		{
			this->mGlobalMutex.lock();
			for (int i = 0; i < (int)this->mListThread.size(); i++)
			{
				this->mListThread[i]->join();
				this->mListThread[i]->release();
			}
			this->mListThread.clear();
			this->mRecordThreadIdMgrMutex.lock();
			this->mRecordThreadIdMgr.reset(0);
			this->mRecordThreadIdMgrMutex.unlock();
			this->mGlobalMutex.unlock();
		}
		void triggerStop()
		{
			if (this->bThreadRunSustainStatus)
				this->bThreadRunSustainStatus = 0;
			if (this->mMyIocpServiceAtmcVariate.lAtmThreadRunning.exchange(0) != 0)
				this->_triggerStop__();
		}
		int getCurrentThreadIndex() const
		{
			return this->mRecordThreadIdMgr.getCurrentThreadIndex(::GetCurrentThreadId());
		}
	};

	class TxIocpServiceSingleton : public TxSysIocp::TxIocpService
	{
	private:
		int iThreadCount;
	private:
		TxIocpServiceSingleton(int _threadCount)
		{
			this->iThreadCount = (_threadCount < 1 ? 1 : _threadCount);
			if (_threadCount <= 0)
			{
				sysLogError("_threadCount=[{0}]。And this->iThreadCount=[{1}]").args(
					_threadCount, this->iThreadCount);
			}
			__super::start(this->iThreadCount);
		}
		~TxIocpServiceSingleton()
		{
			__super::triggerStop();
			__super::join();
		}
		static TxIocpServiceSingleton* __init_env_(int _sign, int _threadCount)
		{
			struct TTT
			{
				TxIocpServiceSingleton *pobj;
			public:
				TTT(int _sign, int _threadCount)
				{
					this->pobj = NULL;
					if (_threadCount <= 0)
					{
						sysLogError("_threadCount=[{0}]。").args(_threadCount);
						_threadCount = 4;
					}
					if (_sign <= 0)
					{
						sysLogError("_sign=[{0}]。").args(_sign);
					}
					else this->pobj = new TxIocpServiceSingleton(_threadCount);
				}
				~TTT()
				{
					this->release_();
				}
				void release_()
				{
					if (this->pobj != NULL)
					{
						delete this->pobj;
						this->pobj = NULL;
						TxIocpService::checkWSAPerfectStatus();
					}
				}
			};
			static TTT ret(_sign, _threadCount);
			if (_sign < 0)
				ret.release_();
			return ret.pobj;
		}
	public:
		static void initialize(int _threadCount)
		{
			__init_env_(1, _threadCount);
		}
		static void uninitialize()
		{
			__init_env_(-1, 0);
		}
		static TxIocpServiceSingleton* getInstance()
		{
			return __init_env_(0, 0);
		}
		static inline int getMtu()
		{
			return 1440;
		}
		static inline int getWriteBufferSize()
		{
			return 1024 * 1024;
		}
		static int getThreadCount()
		{
			return getInstance()->iThreadCount;
		}
	};

	class TxNetAddrIn
	{
	private:
		SOCKADDR_IN addr;
	private:
		inline void _set_addr_zero_()
		{
			if (sizeof(this->addr) != 2 * sizeof(unsigned long long))
			{
				sysLogError("if (sizeof(this->addr) != 2 * sizeof(unsigned long long))");
				::memset(&this->addr, 0, sizeof(this->addr));
			}
			else
			{
				((unsigned long long*)&this->addr)[0] = 0;
				((unsigned long long*)&this->addr)[1] = 0;
			}
		}
	public:
		TxNetAddrIn()
		{
			this->_set_addr_zero_();
		}
		TxNetAddrIn(const SOCKADDR_IN &_addr)
		{
			this->setObject(_addr);
		}
		inline bool equal(const TxNetAddrIn &_temp) const
		{
			if (sizeof(SOCKADDR_IN) == 16)
			{
				return ((const std::uint64_t*)&this->addr)[0] == ((const std::uint64_t*)&_temp.addr)[0]
					&& ((const std::uint64_t*)&this->addr)[1] == ((const std::uint64_t*)&_temp.addr)[1];
			}
			static_assert(sizeof(SOCKADDR_IN) == 16, "");
			assert(false);
			return 0 == ::memcmp(&this->addr, &_temp.addr, sizeof(this->addr));
		}
		inline bool operator <(const TxNetAddrIn &_temp) const
		{
			if (sizeof(SOCKADDR_IN) == 16)
			{
				return ((const std::uint64_t*)&this->addr)[0] != ((const std::uint64_t*)&_temp.addr)[0] ?
					(((const std::uint64_t*)&this->addr)[0] < ((const std::uint64_t*)&_temp.addr)[0])
					: (((const std::uint64_t*)&this->addr)[1] < ((const std::uint64_t*)&_temp.addr)[1]);
			}
			static_assert(sizeof(SOCKADDR_IN) == 16, "");
			assert(false);
			return ::memcmp(&this->addr, &_temp.addr, sizeof(this->addr)) < 0;
		}
		void set(const char *_ip, int _port)
		{
			this->addr.sin_family = AF_INET;
			if (_ip != NULL) this->addr.sin_addr.s_addr = ::inet_addr(_ip);
			else this->addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
			this->addr.sin_port = ::htons((u_short)_port);
		}
		void setObject(const SOCKADDR_IN &_addr)
		{
			this->addr = _addr;
		}
		void setDefault()
		{
			this->_set_addr_zero_();
			this->addr.sin_family = AF_INET;
		}
		template<class T_CH, int N, class T_INT>
		bool get(T_CH(&_ip)[N], T_INT *_port) const
		{
			_ip[0] = 0;
			*_port = -1;
			if (this->addr.sin_family == AF_INET)
			{
				*_port = (T_INT)::ntohs(this->addr.sin_port);
				TxBlobString::txStrNCopy(_ip, ::inet_ntoa(this->addr.sin_addr));
				return true;
			}
			return false;
		}
		inline SOCKADDR_IN* getObject()
		{
			return &this->addr;
		}
		inline const SOCKADDR_IN* getObject() const
		{
			return &this->addr;
		}
		inline bool isValid() const
		{
			return this->addr.sin_family == AF_INET;
		}
	};

	struct tagIocpOverlappedWork_ : public TxIocpService::tagIocpOverlappedWorkBase_
	{
		TxIocpService *const pTxIocpService;
		tagIocpOverlappedWork_(TxIocpService *_pTxIocpService, TxIocpService::FuncType _func) :pTxIocpService(_pTxIocpService)
		{
			TxSysDebugObjectMemory::malloc(this);
			this->pTxIocpService->mMyIocpServiceAtmcVariate.lAtmOverLappedAccretionCount.increment();
			TxIocpService::tagIocpOverlappedWorkBase_::initialize(_func);
		}
		~tagIocpOverlappedWork_()
		{
			this->pTxIocpService->mMyIocpServiceAtmcVariate.lAtmOverLappedAccretionCount.decrement();
			TxSysDebugObjectMemory::free(this);
		}
		template<class L, class H>
		inline static void split_hight_low_num(L *_pl, H *_ph, unsigned long long _val)
		{
			static_assert((H)(1.5) == (H)(1) && (L)(1.5) == (L)(1), "");
			*_pl = (L)(_val);
			*_ph = (H)(_val >> (8 * sizeof(L)));
		}
		template<class L, class H>
		inline static unsigned long long merge_hight_low_num(L _l, H _h)
		{
			static_assert((H)(1.5) == (H)(1) && (L)(1.5) == (L)(1), "");
			return ((unsigned long long)_l) | (((unsigned long long)_h) << (8 * sizeof(L)));
		}
		template<class T>
		inline static T* getObjectOrgOverLappedPointer(OVERLAPPED *_pOVERLAPPED)
		{
#ifndef NDEBUG
			if (0) { tagIocpOverlappedWork_ *_ptr = (T*)(_pOVERLAPPED); _ptr = _ptr; (void)_ptr; }
#endif
			return (T*)((tagIocpOverlappedWork_*)(((size_t)_pOVERLAPPED) - (size_t)&((tagIocpOverlappedWork_*)NULL)->mOVERLAPPED));
		}
		inline void setOffset(unsigned long long _offset) {
			this->split_hight_low_num(&this->mOVERLAPPED.Offset, &this->mOVERLAPPED.OffsetHigh, _offset);
		}
	};

	class TxIocpSocketBasic : public std::enable_shared_from_this<TxIocpSocketBasic>, protected TxForbidDirectCopyObject
	{
	protected:
		TxSysMutex mutex;
		SOCKET mSocket;
		const int iTcp1_Udp2;
	public:
		TxIocpService *const pTxIocpService;
	protected:
		void _reopenSocket_no_mutex_()
		{
			if (this->mSocket == INVALID_SOCKET)
			{
				for (;;)
				{
					switch (this->iTcp1_Udp2)
					{
					case 1:
						this->mSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
						break;
					case 2:
						this->mSocket = ::WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
						break;
					default:
						sysLogError("参数错误。this->iTcp1_Udp2=[{0}]").args(this->iTcp1_Udp2);
						break;
					}
					if (this->mSocket != INVALID_SOCKET)
						break;
					sysLogError("WSASocket()执行失败。this->iTcp1_Udp2=[{0}]").args(this->iTcp1_Udp2);
					::Sleep(1000);
				}
				if (this->mSocket != INVALID_SOCKET)
					pTxIocpService->_associationSocket_(this->mSocket);
			}
		}
	public:
		TxIocpSocketBasic(TxIocpService *_pTxIocpService, int _iTcp1_Udp2) :pTxIocpService(_pTxIocpService), iTcp1_Udp2(_iTcp1_Udp2)
		{
			TxSysDebugObjectMemory::malloc(this);
			TxIocpService::addRefWSAStartup();

			this->mSocket = INVALID_SOCKET;
			this->_reopenSocket_no_mutex_();
		}
		virtual ~TxIocpSocketBasic()
		{
			this->close();
			TxIocpService::releaseWSACleanup();
			TxSysDebugObjectMemory::free(this);
		}
		inline SOCKET getRiskSocket() { return this->mSocket; }
		void lockObject()
		{
			this->mutex.lock();
		}
		void unlockObject()
		{
			this->mutex.unlock();
		}
		inline TxIocpService* getIocpService() const { return const_cast<TxIocpSocketBasic*>(this)->pTxIocpService; }
		void close()
		{
			this->lockObject();
			SOCKET s = this->mSocket;
			this->mSocket = INVALID_SOCKET;
			this->unlockObject();
			if (s != INVALID_SOCKET)
			{
				::shutdown(s, SD_BOTH);
				if (0 != ::closesocket(s))
					sysLogError("closesocket() failure");
			}
		}
	};

	class TxTcpTrmsSocket : public TxIocpSocketBasic
	{
	public:
		virtual void _vfReadExceptionEvent(std::shared_ptr<TxIocpSocketBasic> &_sp) = 0;
		virtual void _vfReadBufferEvent(std::shared_ptr<TxIocpSocketBasic> &_sp, const unsigned char *_data, int _size) = 0;
		virtual void _vfWriteBufferEvent(std::shared_ptr<TxIocpSocketBasic> &_sp, int _iSurplusSize) = 0;
	private:
		unsigned char pReadBuff[10 * 1024];
		TxMemoryChunk<5 * 1024, TxSysDebugObjectMemory> mWriteBuff;
		LONG __dsfdsdf__0;
		LONG lStandardReserve;
		LONG __dsfdsdf__1;
		LONG bWrittingSign;
		LONG __dsfdsdf__2;
	public:
		TxTcpTrmsSocket(TxIocpService *_pTxIocpService, int _mtu, int _iWriteBufferSize)
			: TxIocpSocketBasic(_pTxIocpService, 1), mWriteBuff(_mtu), lStandardReserve((LONG)_iWriteBufferSize)
		{
			TxSysDebugObjectMemory::malloc(this);
			this->bWrittingSign = 0;
		}
		void setSockOptNoDelay()
		{
			this->lockObject();
			BOOL lc_b_nodelay = TRUE;
			if (this->mSocket != INVALID_SOCKET)
			{
				if (0 != ::setsockopt(this->mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&lc_b_nodelay, sizeof(lc_b_nodelay)))
					sysLogError("if (0 != ::setsockopt(this->mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&lc_b_nodelay, sizeof(lc_b_nodelay)))调用失败");
			}
			this->unlockObject();
		}
		virtual ~TxTcpTrmsSocket() { TxSysDebugObjectMemory::free(this); }
		void setWriteBufferSize(int _iWriteBufferSize)
		{
			this->lockObject();
			this->lStandardReserve = (LONG)_iWriteBufferSize;
			this->unlockObject();
		}
		void asyncWrite(const void *_data, int _size)
		{
			if (_data == NULL || _size <= 0) return;
			_staticAsyncWrite((const unsigned char*)_data, _size, 0, this->shared_from_this(), NULL);
		}
		static void asyncWrite(std::shared_ptr<TxIocpSocketBasic> &_sp, const void *_data, int _size)
		{
			if (_data == NULL || _size <= 0 || dynamic_cast<TxTcpTrmsSocket*>(_sp.get()) == NULL)
			{
				sysLogError("if (_data == NULL || _size <= 0|| dynamic_cast<TxTcpTrmsSocket*>(_sp.get()))参数错误。_sp=[{0}],_data=[{1}],_size=[{2}]").args(
					(size_t)dynamic_cast<TxTcpTrmsSocket*>(_sp.get()), (size_t)_data, _size);
				return;
			}
			_staticAsyncWrite((const unsigned char*)_data, _size, 0, _sp, NULL);
		}
		void asyncWrite(const std::string &_data)
		{
			if (_data.length() <= 0) return;
			_staticAsyncWrite((const unsigned char*)_data.c_str(), (int)_data.size(), 0, this->shared_from_this(), NULL);
		}
		void asyncWrite(const TxBlobString &_data)
		{
			if (_data.length() <= 0) return;
			_staticAsyncWrite((const unsigned char*)_data.c_str(), (int)_data.size(), 0, this->shared_from_this(), NULL);
		}
		static void asyncWrite(std::shared_ptr<TxIocpSocketBasic> &_sp, const std::string &_data)
		{
			if (_data.length() <= 0)
				return;
			if (dynamic_cast<TxTcpTrmsSocket*>(_sp.get()) == NULL)
			{
				sysLogError("if (dynamic_cast<TxTcpTrmsSocket*>(_sp.get()) == NULL)");
				return;
			}
			_staticAsyncWrite((const unsigned char*)_data.c_str(), (int)_data.size(), 0, _sp, NULL);
		}
		static void asyncWrite(std::shared_ptr<TxIocpSocketBasic> &_sp, const TxBlobString &_data)
		{
			if (_data.length() <= 0)
				return;
			if (dynamic_cast<TxTcpTrmsSocket*>(_sp.get()) == NULL)
			{
				sysLogError("if (dynamic_cast<TxTcpTrmsSocket*>(_sp.get()) == NULL)");
				return;
			}
			_staticAsyncWrite((const unsigned char*)_data.c_str(), (int)_data.size(), 0, _sp, NULL);
		}
		static void _staticAsyncWrite(const unsigned char *_data, int _size, const int _iWriteCount, std::shared_ptr<TxIocpSocketBasic> &_sp, void *_orgOvrLpdPtr)
		{
			struct tagIocpOverlappedWork_Ext : public tagIocpOverlappedWork_
			{
				DWORD dwNumberOfBytesRecvd;
				DWORD dwOrgTotalSize;
				std::weak_ptr<TxIocpSocketBasic> wp;
				tagIocpOverlappedWork_Ext(std::shared_ptr<TxIocpSocketBasic> *_sp, TxIocpService *_pTxIocpService, DWORD _dwOrgTotalSize)
					:wp(*_sp), tagIocpOverlappedWork_(_pTxIocpService, g_func), dwOrgTotalSize(_dwOrgTotalSize)
				{
					this->dwNumberOfBytesRecvd = 0;
				}
				static bool g_func(OVERLAPPED *_obj, int _iNumberOfBytes, bool _bResultStatus)
				{
					tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext
						= tagIocpOverlappedWork_::getObjectOrgOverLappedPointer<tagIocpOverlappedWork_Ext>(_obj);
					std::shared_ptr<TxIocpSocketBasic> lc_sp(pIocpOverlappedWork_Ext->wp.lock());
					TxTcpTrmsSocket *pThis = _My_IocpGlobalFunction_::buildDynamicCast<TxTcpTrmsSocket*>(lc_sp.get());
					BOOL bLcDoGood = FALSE;
					if (pThis != NULL)
					{
						if (_bResultStatus&&_iNumberOfBytes > 0)
						{
							pThis->_vfWriteBufferEvent(lc_sp,
								(int)(pIocpOverlappedWork_Ext->dwOrgTotalSize - _iNumberOfBytes));
							_staticAsyncWrite(NULL, 0, _iNumberOfBytes, lc_sp, pIocpOverlappedWork_Ext);
							bLcDoGood = TRUE;
						}
					}
					if (!bLcDoGood)
						delete pIocpOverlappedWork_Ext;
					return true;
				}
			};
			tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext = (tagIocpOverlappedWork_Ext*)_orgOvrLpdPtr;
			BOOL bLcPushMsgSuc = FALSE;
			if (_sp)
			{
				TxTcpTrmsSocket *pThis = _My_IocpGlobalFunction_::buildDynamicCast<TxTcpTrmsSocket*>(_sp.get());
				WSABUF mLcBuf = { 0 };
				pThis->lockObject();
				if (_iWriteCount > 0)
				{
					pThis->mWriteBuff.popFront(_iWriteCount);
					if (pThis->lStandardReserve > 0)
						pThis->mWriteBuff.reserveStandardTail(pThis->lStandardReserve);
				}
				if (_size > 0)
					pThis->mWriteBuff.pushTail((const unsigned char*)_data, _size);
				if (!pThis->bWrittingSign || _iWriteCount > 0)//Write operation idle
				{
					mLcBuf.buf = (char*)pThis->mWriteBuff.getUnitDataPtr();
					mLcBuf.len = pThis->mWriteBuff.getUnitDataSize();
				}
				if (mLcBuf.len > 0)
				{
					if (pIocpOverlappedWork_Ext == NULL)
					{
						pIocpOverlappedWork_Ext = new tagIocpOverlappedWork_Ext(
							&_sp, pThis->pTxIocpService, (DWORD)pThis->mWriteBuff.getTotalSize());
					}
					DWORD dwFlags = 0;
					pThis->getIocpService()->serviceQueueMutexLock();
					bLcPushMsgSuc = (pThis->getRiskSocket() != INVALID_SOCKET
						&& (SOCKET_ERROR != ::WSASend(pThis->getRiskSocket(), &mLcBuf, 1,
							&pIocpOverlappedWork_Ext->dwNumberOfBytesRecvd, dwFlags, &pIocpOverlappedWork_Ext->mOVERLAPPED, NULL)
							|| ::WSAGetLastError() == ERROR_IO_PENDING));
					pThis->getIocpService()->serviceQueueMutexUnlock();
					pThis->bWrittingSign = !!bLcPushMsgSuc;
				}
				else
				{
					if (_iWriteCount > 0)
						pThis->bWrittingSign = FALSE;
				}
				pThis->unlockObject();
			}
			if (!bLcPushMsgSuc&&pIocpOverlappedWork_Ext != NULL)
				pIocpOverlappedWork_Ext->pushQueueFailureWork();
		}
		static void staticStartAsyncRead(std::shared_ptr<TxIocpSocketBasic> &_sp, void *_orgOvrLpdPtr)
		{
			struct tagIocpOverlappedWork_Ext : public tagIocpOverlappedWork_
			{
				DWORD dwNumberOfBytesRecvd;
				DWORD dwFlags;
				std::shared_ptr<TxIocpSocketBasic> sp;
				tagIocpOverlappedWork_Ext(std::shared_ptr<TxIocpSocketBasic> *_sp,
					TxIocpService *_pTxIocpService)
					:sp(*_sp), tagIocpOverlappedWork_(_pTxIocpService, g_func)
				{
					this->dwNumberOfBytesRecvd = 0;
					this->dwFlags = 0;
				}
				static bool g_func(OVERLAPPED *_obj, int _iNumberOfBytes, bool _bResultStatus)
				{
					tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext
						= tagIocpOverlappedWork_::getObjectOrgOverLappedPointer<tagIocpOverlappedWork_Ext>(_obj);
					TxTcpTrmsSocket *pThis = _My_IocpGlobalFunction_::buildDynamicCast<TxTcpTrmsSocket*>(
						pIocpOverlappedWork_Ext->sp.get());
					BOOL bSec = FALSE;
					if (pThis != NULL)
					{
						if (_iNumberOfBytes > 0 && _bResultStatus)
						{
							pThis->_vfReadBufferEvent(pIocpOverlappedWork_Ext->sp, pThis->pReadBuff, _iNumberOfBytes);
							staticStartAsyncRead(pIocpOverlappedWork_Ext->sp, pIocpOverlappedWork_Ext);
							bSec = TRUE;
						}
					}
					if (!bSec)
					{
						if (pThis != NULL)
						{
							pThis->close();
							pThis->_vfReadExceptionEvent(pIocpOverlappedWork_Ext->sp);
						}
						delete pIocpOverlappedWork_Ext;
					}
					return true;
				}
			};
			tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext = (tagIocpOverlappedWork_Ext*)_orgOvrLpdPtr;
			TxTcpTrmsSocket *pThis = _My_IocpGlobalFunction_::buildDynamicCast<TxTcpTrmsSocket*>(_sp.get());
			BOOL bLcPushMsgSuc = FALSE;
			if (pThis != NULL)
			{
				if (pIocpOverlappedWork_Ext == NULL)
					pIocpOverlappedWork_Ext = new tagIocpOverlappedWork_Ext(&_sp, pThis->pTxIocpService);
				WSABUF mLcBuf;
				mLcBuf.buf = (char*)pThis->pReadBuff;
				mLcBuf.len = sizeof(pThis->pReadBuff);
				pThis->lockObject();
				pThis->pTxIocpService->serviceQueueMutexLock();
				bLcPushMsgSuc = (pThis->getRiskSocket() != INVALID_SOCKET
					&& (SOCKET_ERROR != ::WSARecv(pThis->getRiskSocket(), &mLcBuf, 1, &pIocpOverlappedWork_Ext->dwNumberOfBytesRecvd,
						&pIocpOverlappedWork_Ext->dwFlags, &pIocpOverlappedWork_Ext->mOVERLAPPED, NULL)
						|| ::WSAGetLastError() == ERROR_IO_PENDING));
				pThis->pTxIocpService->serviceQueueMutexUnlock();
				pThis->unlockObject();
			}
			if (!bLcPushMsgSuc&&pIocpOverlappedWork_Ext != NULL)
			{
				pIocpOverlappedWork_Ext->pushQueueFailureWork();
			}
		}
	};

	class MyIocpTcpServerSocketBase_ : public TxIocpSocketBasic
	{
	public:
		MyIocpTcpServerSocketBase_(TxIocpService *_pTxIocpService) : TxIocpSocketBasic(_pTxIocpService, 1) { }
	public:
		virtual void onPeerSocketReadExceptionEvent(std::shared_ptr<TxSysIocp::TxIocpSocketBasic> &_sp) {
			(void)&_sp;
		}
		virtual void onPeerSocketReadBufferEvent(std::shared_ptr<TxSysIocp::TxIocpSocketBasic> &_sp, const unsigned char *_data, int _size) {
			(void)&_sp; (void)_data; (void)_size;
		}
		virtual void onPeerSocketWriteBufferEvent(std::shared_ptr<TxSysIocp::TxIocpSocketBasic> &_sp, int _iSurplusSize) {
			(void)&_sp; (void)_iSurplusSize;
		}
	};

	class TxTcpPeerSocket : public TxTcpTrmsSocket
	{
	private:
		std::shared_ptr<TxIocpSocketBasic> spServerSocket;
	private:
		virtual void onPeerReadExceptionEvent() { }
		virtual void onPeerReadBufferEvent(const unsigned char *_data, int _size) { (void)_data; (void)_size; }
		virtual void onPeerWriteBufferEvent(int _iSurplusSize) { (void)_iSurplusSize; }
	public:
		TxTcpPeerSocket(std::shared_ptr<TxSysIocp::TxIocpSocketBasic> &_spParentSocket, int _mtu, int _iWriteBufferSize = -1)
			:TxTcpTrmsSocket(_spParentSocket->pTxIocpService, _mtu, _iWriteBufferSize), spServerSocket(_spParentSocket)
		{
			TxSysDebugObjectMemory::malloc(this);
		}
		~TxTcpPeerSocket()
		{
			TxSysDebugObjectMemory::free(this);
		}
		inline std::shared_ptr<TxSysIocp::TxIocpSocketBasic>& getServerSocket() { return this->spServerSocket; }
		virtual void _vfReadExceptionEvent(std::shared_ptr<TxIocpSocketBasic> &_sp)
		{
			if (_sp)
			{
				MyIocpTcpServerSocketBase_ *pServerSocket = _My_IocpGlobalFunction_::buildDynamicCast<MyIocpTcpServerSocketBase_*>(this->spServerSocket.get());
				this->onPeerReadExceptionEvent();
				if (pServerSocket == NULL)
					assert(false);
				else pServerSocket->onPeerSocketReadExceptionEvent(_sp);
			}
		}
		virtual void _vfReadBufferEvent(std::shared_ptr<TxIocpSocketBasic> &_sp, const unsigned char *_data, int _size)
		{
			if (_sp)
			{
				MyIocpTcpServerSocketBase_* pServerSocket = _My_IocpGlobalFunction_::buildDynamicCast<MyIocpTcpServerSocketBase_*>(this->spServerSocket.get());
				this->onPeerReadBufferEvent(_data, _size);
				if (pServerSocket == NULL)
					assert(false);
				else pServerSocket->onPeerSocketReadBufferEvent(_sp, _data, _size);
			}
		}
		virtual void _vfWriteBufferEvent(std::shared_ptr<TxIocpSocketBasic> &_sp, int _iSurplusSize)
		{
			if (_sp)
			{
				MyIocpTcpServerSocketBase_* pServerSocket = _My_IocpGlobalFunction_::buildDynamicCast<MyIocpTcpServerSocketBase_*>(this->spServerSocket.get());
				this->onPeerWriteBufferEvent(_iSurplusSize);
				if (pServerSocket == NULL)
					assert(false);
				else pServerSocket->onPeerSocketWriteBufferEvent(_sp, _iSurplusSize);
			}
		}
	};

	class TxTcpServerSocket : public MyIocpTcpServerSocketBase_
	{
	private:
		LPFN_ACCEPTEX lpfnAcceptEx;
		LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockaddrs;
	private:
		virtual void onAcceptPeerSocketEvent(std::shared_ptr<TxIocpSocketBasic> &_sp, const SOCKADDR_IN *_addr) { (void)&_sp; (void)_addr; }
		virtual std::shared_ptr<TxSysIocp::TxIocpSocketBasic> onNewSocketEvent()
		{
			return std::make_shared<TxTcpPeerSocket>(this->shared_from_this(), 1440);
		}
	public:
		TxTcpServerSocket(TxSysIocp::TxIocpService *_pTxIocpService) :MyIocpTcpServerSocketBase_(_pTxIocpService)
		{
			TxSysDebugObjectMemory::malloc(this);
			if (true)
			{
				GUID mLcGuidAcceptEx = WSAID_ACCEPTEX;
				DWORD dwBytes = 0;
				this->lpfnAcceptEx = NULL;
				if (::WSAIoctl(this->getRiskSocket(), SIO_GET_EXTENSION_FUNCTION_POINTER,
					&mLcGuidAcceptEx, sizeof(mLcGuidAcceptEx),
					&this->lpfnAcceptEx, sizeof(this->lpfnAcceptEx),
					&dwBytes, NULL, NULL) == SOCKET_ERROR)
				{
					sysLogError("WSAIoctl AcceptEx(WSAID_ACCEPTEX) failure");
				}
			}
			if (true)
			{
				GUID mLcGuidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
				DWORD dwBytes = 0;
				if (::WSAIoctl(this->getRiskSocket(), SIO_GET_EXTENSION_FUNCTION_POINTER,
					&mLcGuidGetAcceptExSockaddrs, sizeof(mLcGuidGetAcceptExSockaddrs),
					&this->lpfnGetAcceptExSockaddrs, sizeof(this->lpfnGetAcceptExSockaddrs),
					&dwBytes, NULL, NULL) == SOCKET_ERROR)
				{
					sysLogError("WSAIoctl AcceptEx(WSAID_GETACCEPTEXSOCKADDRS) failure");
				}
			}
		}
		virtual ~TxTcpServerSocket()
		{
			TxSysDebugObjectMemory::free(this);
		}
		void asyncAccept__(void *_orgOvrLpdPtr = NULL)
		{
			struct tagIocpOverlappedWork_Ext : public tagIocpOverlappedWork_
			{
				char aBothPointAddrBuffer[2][16 + sizeof(struct sockaddr_in)];
				std::shared_ptr<TxIocpSocketBasic> sp;
				TxTcpServerSocket *const pIocpTcpServerSocket;
				DWORD dwBytesReceived;
				tagIocpOverlappedWork_Ext(TxTcpServerSocket *_pIocpTcpServerSocket)
					:tagIocpOverlappedWork_(_pIocpTcpServerSocket->pTxIocpService, g_func), pIocpTcpServerSocket(_pIocpTcpServerSocket)
				{
					memset(&this->aBothPointAddrBuffer, 0, sizeof(this->aBothPointAddrBuffer));
					this->dwBytesReceived = 0;
				}
				static bool g_func(OVERLAPPED *_obj, int _iNumberOfBytes, bool _bResultStatus)
				{
					(void)_iNumberOfBytes;
					tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext = tagIocpOverlappedWork_::getObjectOrgOverLappedPointer<tagIocpOverlappedWork_Ext>(_obj);
					TxTcpServerSocket *pThis = pIocpOverlappedWork_Ext->pIocpTcpServerSocket;
					BOOL bSec = FALSE;
					if (pThis != NULL)
					{
						bSec = TRUE;
						if (_bResultStatus)
						{
							struct sockaddr_in *lc_local_addr = NULL;
							struct sockaddr_in *lc_remote_addr = NULL;
							int lc_local_addr_len = sizeof(sockaddr_in);
							int lc_remote_addr_len = sizeof(sockaddr_in);
							if (pThis->lpfnGetAcceptExSockaddrs != NULL)
							{
								pThis->lpfnGetAcceptExSockaddrs(
									&pIocpOverlappedWork_Ext->aBothPointAddrBuffer[0][0], 0,
									sizeof(pIocpOverlappedWork_Ext->aBothPointAddrBuffer[0]),
									sizeof(pIocpOverlappedWork_Ext->aBothPointAddrBuffer[1]),
									(struct sockaddr**)&lc_local_addr, &lc_local_addr_len,
									(struct sockaddr**)&lc_remote_addr, &lc_remote_addr_len);
							}
							pThis->onAcceptPeerSocketEvent(pIocpOverlappedWork_Ext->sp, lc_remote_addr);
							if (pIocpOverlappedWork_Ext->sp)
								TxTcpTrmsSocket::staticStartAsyncRead((std::shared_ptr<TxIocpSocketBasic>)pIocpOverlappedWork_Ext->sp, NULL);
						}
						if (!_bResultStatus)
						{
							pThis->lockObject();
							SOCKET s = pThis->getRiskSocket();
							pThis->unlockObject();
							if (s == INVALID_SOCKET)
								bSec = FALSE;
						}
						if (bSec)
							pThis->asyncAccept__(pIocpOverlappedWork_Ext);
					}
					if (!bSec&&pIocpOverlappedWork_Ext)
					{
						delete pIocpOverlappedWork_Ext;
					}
					return true;
				}
			};

			tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext = (tagIocpOverlappedWork_Ext*)_orgOvrLpdPtr;
			if (pIocpOverlappedWork_Ext == NULL)
				pIocpOverlappedWork_Ext = new tagIocpOverlappedWork_Ext(this);
			pIocpOverlappedWork_Ext->sp = this->onNewSocketEvent();
			if (std::dynamic_pointer_cast<TxTcpPeerSocket>(pIocpOverlappedWork_Ext->sp) == NULL)
				sysLogError("onNewSocketEvent() must return object inherit TxTcpPeerSocket!");
			BOOL bLcPushMsgSuc = FALSE;

			this->lockObject();
			this->getIocpService()->serviceQueueMutexLock();
			bLcPushMsgSuc = (this->getRiskSocket() != INVALID_SOCKET
				&& (lpfnAcceptEx(
					this->getRiskSocket(),
					pIocpOverlappedWork_Ext->sp->getRiskSocket(),
					&pIocpOverlappedWork_Ext->aBothPointAddrBuffer[0][0],
					0,
					sizeof(pIocpOverlappedWork_Ext->aBothPointAddrBuffer[0]),
					sizeof(pIocpOverlappedWork_Ext->aBothPointAddrBuffer[1]),
					&pIocpOverlappedWork_Ext->dwBytesReceived,
					&pIocpOverlappedWork_Ext->mOVERLAPPED)
					|| ::WSAGetLastError() == ERROR_IO_PENDING));
			this->getIocpService()->serviceQueueMutexUnlock();
			this->unlockObject();

			if (!bLcPushMsgSuc&&pIocpOverlappedWork_Ext != NULL)
			{
				sysLogError("lpfnAcceptEx() failure");
				pIocpOverlappedWork_Ext->pushQueueFailureWork();
			}
		}
		bool listen(int _iPort)
		{
			//Initialization Socket
			struct sockaddr_in lcServerAddress = { 0 };
			lcServerAddress.sin_family = AF_INET;
			//You can choose to bind any of the available addresses, or a IP address that you specify
			lcServerAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
			lcServerAddress.sin_port = ::htons((u_short)_iPort);
			SOCKET s = this->getRiskSocket();

			// bind tcp server port
			if (s != INVALID_SOCKET
				&&SOCKET_ERROR != ::bind(s, (struct sockaddr *) &lcServerAddress, sizeof(lcServerAddress))
				&& SOCKET_ERROR != ::listen(s, SOMAXCONN))
			{
				this->asyncAccept__(NULL);
				return true;
			}
			return false;
		}
	};

	class TxTcpClientSocket : public TxTcpTrmsSocket
	{
	private:
		std::function<void(bool)> fnConnectEvent;
		std::function<void()> fnExceptionEvent;
		std::function<void(const unsigned char*, int)> fnReadEvent;
		std::function<void(int)> fnWriteEvent;
		std::function<void(void)> fnDestroyEvent;
		LPFN_CONNECTEX lpfnConnectEx;
	private:
		void _rebind_socket_nomutex_()
		{
			GUID mLcGuidConnectEx = WSAID_CONNECTEX;
			DWORD dwBytes = 0;
			this->lpfnConnectEx = NULL;
			SOCKET lc_s = this->getRiskSocket();
			if (SOCKET_ERROR == ::WSAIoctl(lc_s, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&mLcGuidConnectEx, sizeof(mLcGuidConnectEx),
				&this->lpfnConnectEx, sizeof(this->lpfnConnectEx), &dwBytes, 0, 0))
			{
				sysLogError("WSAIoctl failed ConnectEx()");
			}
			else
			{
				sockaddr_in local_addr = { 0 };
				local_addr.sin_family = AF_INET;
				while (SOCKET_ERROR == ::bind(lc_s, (sockaddr *)(&local_addr), sizeof(sockaddr_in)))
				{
					int err = (int)::WSAGetLastError();
					if (err == (int)(ERROR_IO_PENDING))
						break;
					else if (err == (int)(WSAENOBUFS)) ::Sleep(1000);
					else sysLogError("client bind(local) failed");
				}
			}
		}
	protected:
		virtual void onClientConnectEvent(bool _bResult)
		{
			if (this->fnConnectEvent)
				this->fnConnectEvent(_bResult);
		}
		virtual void onClientReadExceptionEvent()
		{
			if (this->fnExceptionEvent)
				this->fnExceptionEvent();
		}
		virtual void onClientReadBufferEvent(const unsigned char *_data, int _size)
		{
			if (this->fnReadEvent)
				this->fnReadEvent(_data, _size);
		}
		virtual void onClientWriteBufferEvent(int _iSurplusSize)
		{
			if (this->fnWriteEvent)
				this->fnWriteEvent(_iSurplusSize);
		}
	public:
		TxTcpClientSocket(TxSysIocp::TxIocpService *_pTxIocpService, int _mtu, int _iWriteBufferSize = -1)
			:TxTcpTrmsSocket(_pTxIocpService, _mtu, _iWriteBufferSize)
		{
			TxSysDebugObjectMemory::malloc(this);
			this->_rebind_socket_nomutex_();
		}
		virtual ~TxTcpClientSocket()
		{
			if (this->fnDestroyEvent)
				this->fnDestroyEvent();
			TxSysDebugObjectMemory::free(this);
		}
		void setConnectEvent(const std::function<void(bool)> &_fnConnect)
		{
			this->fnConnectEvent = _fnConnect;
		}
		void setExceptionEvent(const std::function<void()> &_fnException)
		{
			this->fnExceptionEvent = _fnException;
		}
		void setReadEvent(const std::function<void(const unsigned char*, int)> &_fnRead)
		{
			this->fnReadEvent = _fnRead;
		}
		void setWriteEvent(const std::function<void(int)> &_fnWrite)
		{
			this->fnWriteEvent = _fnWrite;
		}
		void asyncConnect(const char _strIp[], int _iPort)
		{
			staticAsyncConnect(_strIp, _iPort, this->shared_from_this());
		}
		void setDestroyEvent(const std::function<void()> &_fnDestroyEvent)
		{
			this->fnDestroyEvent = _fnDestroyEvent;
		}
		void asyncConnect(const wchar_t _strIp[], int _iPort)
		{
			char lc_strIp[32] = { 0 };
			for (int i = 0; _strIp[i] != 0; i++)
			{
				if (i > sizeof(lc_strIp) / sizeof(lc_strIp[0]) - 1)
					break;
				if (_strIp[i] == '.' || ('0' <= _strIp[i] && _strIp[i] <= '9'))
					lc_strIp[i] = (char)_strIp[i];
				else break;
			}
			staticAsyncConnect(lc_strIp, _iPort, this->shared_from_this());
		}
		static void staticAsyncConnect(const char _strIp[], int _iPort, std::shared_ptr<TxIocpSocketBasic> &_sp)
		{
			struct tagIocpOverlappedWork_Ext : public tagIocpOverlappedWork_
			{
				DWORD dwNumberOfBytesRecvd;
				std::shared_ptr<TxIocpSocketBasic> sp;
				tagIocpOverlappedWork_Ext(std::shared_ptr<TxIocpSocketBasic> *_sp)
					:tagIocpOverlappedWork_((*_sp)->pTxIocpService, g_func), sp(*_sp)
				{
					this->dwNumberOfBytesRecvd = 0;
				}
				static bool g_func(OVERLAPPED *_obj, int _iNumberOfBytes, bool _bResultStatus)
				{
					(void)_iNumberOfBytes;
					tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext
						= tagIocpOverlappedWork_::getObjectOrgOverLappedPointer<tagIocpOverlappedWork_Ext>(_obj);
					TxTcpClientSocket *pThis = (TxTcpClientSocket*)(pIocpOverlappedWork_Ext->sp.get());
					//if (!_bResultStatus)
					//	pThis->close();
					pThis->onClientConnectEvent(_bResultStatus);
					if (_bResultStatus)
						TxTcpTrmsSocket::staticStartAsyncRead(pIocpOverlappedWork_Ext->sp, NULL);
					delete pIocpOverlappedWork_Ext;
					return true;
				}
			};
			TxTcpClientSocket *pThis = (TxTcpClientSocket*)_sp.get();
			tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext = new tagIocpOverlappedWork_Ext(&_sp);
			sockaddr_in mLcPeerAddr = { 0 };
			mLcPeerAddr.sin_family = AF_INET;
			mLcPeerAddr.sin_addr.s_addr = inet_addr(_strIp);
			mLcPeerAddr.sin_port = ::htons((u_short)_iPort);
			pIocpOverlappedWork_Ext->dwNumberOfBytesRecvd = 0;
			BOOL bLcPushMsgSuc = FALSE;
			pThis->lockObject();
			pThis->getIocpService()->serviceQueueMutexLock();
			if (pThis->getRiskSocket() == INVALID_SOCKET)
			{
				pThis->_reopenSocket_no_mutex_();
				pThis->_rebind_socket_nomutex_();
			}
			bLcPushMsgSuc = pThis->lpfnConnectEx(
				pThis->getRiskSocket(),
				(sockaddr*)&mLcPeerAddr,
				sizeof(mLcPeerAddr),
				NULL,
				0,
				&pIocpOverlappedWork_Ext->dwNumberOfBytesRecvd,
				&pIocpOverlappedWork_Ext->mOVERLAPPED)
				|| ::WSAGetLastError() == ERROR_IO_PENDING;
			pThis->getIocpService()->serviceQueueMutexUnlock();
			pThis->unlockObject();
			if (!bLcPushMsgSuc)
			{
				if (pThis->getRiskSocket() != INVALID_SOCKET)
					sysLogError("lpfnConnectEx() failure");
				pIocpOverlappedWork_Ext->pushQueueFailureWork();
			}
		}
		virtual void _vfReadExceptionEvent(std::shared_ptr<TxIocpSocketBasic> &_sp)
		{
			TxTcpClientSocket *pThis = _My_IocpGlobalFunction_::buildDynamicCast<TxTcpClientSocket*>(_sp.get());
			if (pThis != NULL)
				pThis->onClientReadExceptionEvent();
		}
		virtual void _vfReadBufferEvent(std::shared_ptr<TxIocpSocketBasic> &_sp, const unsigned char *_data, int _size)
		{
			TxTcpClientSocket *pThis = _My_IocpGlobalFunction_::buildDynamicCast<TxTcpClientSocket*>(_sp.get());
			if (pThis != NULL)
				pThis->onClientReadBufferEvent(_data, _size);
		}
		virtual void _vfWriteBufferEvent(std::shared_ptr<TxIocpSocketBasic> &_sp, int _iSurplusSize)
		{
			TxTcpClientSocket *pThis = _My_IocpGlobalFunction_::buildDynamicCast<TxTcpClientSocket*>(_sp.get());
			if (pThis != NULL)
				pThis->onClientWriteBufferEvent(_iSurplusSize);
		}
	};
	//class CPackageWrap
	//{
	//public:
	//	typedef TxBlobString TPack;
	//public:
	//	bool push(const void *_data, int _size)
	//	{
	//		return true;
	//	}
	//	std::shared_ptr<TPack> pop(std::uint32_t *_pkgNumber)
	//	{
	//		(void)_pkgNumber;
	//		return std::shared_ptr<TPack>();
	//	}
	//	static std::uint32_t buildPkgNumber()
	//	{
	//		static boost::atomic<std::uint32_t> lc_atmValue(0);
	//		std::uint32_t ret;
	//		do {
	//			ret = ++lc_atmValue;
	//		} while (ret == 0);
	//		return ret;
	//	}
	//};
	//class CClientSockeGroup
	//{
	//private:
	//	struct tagClientData {
	//		TxObjectLifePeriod mLifePeriod;
	//		std::string ip;
	//	};
	//	struct tagClientSession {
	//		tagClientData mClientData;
	//		TxObjectLifePeriod::TxObjectEntity mEntityValid;
	//		std::weak_ptr<TxSysIocp::TxTcpClientSocket> wpSocket;
	//		boost::atomic<int> atmiWriteSurplusSize;
	//		CPackageWrap mReadPackage;
	//	};
	//	struct tagMsgRpdData
	//	{
	//		tagClientData mClientData;
	//		std::shared_ptr<CPackageWrap::TPack> rpdMsg;
	//	};
	//	struct tagRpdMsg {
	//		std::vector<tagMsgRpdData> vMsgRpdData;
	//		TxSysEvent mFinishEvent;
	//		const std::function<void(const std::vector<tagMsgRpdData>&)> *pfunc;
	//	};
	//private:
	//	std::vector<std::shared_ptr<tagClientSession>> vClientSession;
	//	TxSysEvent mThreadEvent;
	//	std::map<std::uint32_t, std::weak_ptr<tagRpdMsg>> msgRespond;
	//	TxSysMutex mMutex;
	//public:
	//	CClientSockeGroup()
	//	{
	//	}
	//	~CClientSockeGroup()
	//	{
	//		this->clear();
	//	}
	//	void clear()
	//	{
	//		std::list<std::weak_ptr<TxSysIocp::TxTcpClientSocket>> lcListSocket;
	//		this->mMutex.lock();
	//		for (auto iter = this->vClientSession.begin(); iter != this->vClientSession.end(); ++iter)
	//			lcListSocket.push_back((*iter)->wpSocket);
	//		this->vClientSession.clear();
	//		this->mMutex.unlock();
	//		while (lcListSocket.size() > 0)
	//		{
	//			for (auto iter = lcListSocket.begin(); iter != lcListSocket.end();)
	//			{
	//				std::shared_ptr<TxSysIocp::TxTcpClientSocket> lc_spSocket = iter->lock();
	//				if (!lc_spSocket)
	//					iter = lcListSocket.erase(iter);
	//				else
	//				{
	//					lc_spSocket->close();
	//					++iter;
	//				}
	//			}
	//			::Sleep(10);
	//		}
	//	}
	//	void postThreadTask(int _msec, const std::function<void(void)> &_mfunc)
	//	{
	//	}
	//	void request(const std::string &_data, int _wait_msec, const std::function<void(const std::vector<CClientSockeGroup::tagMsgRpdData>&)> _mfunc)
	//	{
	//		std::uint32_t lcPkgNumber = CPackageWrap::buildPkgNumber();
	//		tagMsgRpdData lcMsgRpdData;
	//		std::shared_ptr<tagRpdMsg> lcRpdMsg = std::make_shared<tagRpdMsg>();
	//		lcRpdMsg->pfunc = &_mfunc;
	//		this->mMutex.lock();
	//		lcRpdMsg->vMsgRpdData.resize(this->vClientSession.size());
	//		for (int i = 0; i < (int)this->vClientSession.size(); ++i)
	//			lcRpdMsg->vMsgRpdData[i].mClientData = this->vClientSession[i]->mClientData;
	//		auto iter = this->msgRespond.find(lcPkgNumber);
	//		if (iter != this->msgRespond.end())
	//		{
	//			std::shared_ptr<tagRpdMsg> lcOldRpdMsg = iter->second.lock();
	//			if (lcOldRpdMsg)
	//				lcOldRpdMsg->mFinishEvent.setEvent();
	//		}
	//		this->msgRespond[lcPkgNumber] = lcRpdMsg;
	//		this->mMutex.unlock();
	//
	//		lcRpdMsg->mFinishEvent.waitEvent(_wait_msec);
	//
	//		this->mMutex.lock();
	//		std::vector<tagMsgRpdData> lc_vMsgRpdData = lcRpdMsg->vMsgRpdData;
	//		this->msgRespond.erase(lcPkgNumber);
	//		this->mMutex.unlock();
	//
	//		(*lcRpdMsg->pfunc)(lc_vMsgRpdData);
	//	}
	//	void _socketConnectedFinish_(std::weak_ptr<tagClientSession> _wpClientSession, int _idx)
	//	{
	//		std::shared_ptr<tagClientSession> lc_spClientSession = _wpClientSession.lock();
	//		if (!lc_spClientSession)
	//			return;
	//		this->mMutex.lock();
	//		std::shared_ptr<TxSysIocp::TxTcpClientSocket> lc_spSocket = lc_spClientSession->wpSocket.lock();
	//		this->mMutex.unlock();
	//		lc_spSocket->setReadEvent([lc_spClientSession, this, _idx](const unsigned char *_data, int _size) {
	//			bool lc_set_event = false;
	//			lc_spClientSession->mReadPackage.push(_data, _size);
	//			for (;;)
	//			{
	//				std::uint32_t lc_pkgNumber = 0;
	//				std::shared_ptr<CPackageWrap::TPack> lc_pkg = lc_spClientSession->mReadPackage.pop(&lc_pkgNumber);
	//				if (!lc_pkg)
	//					break;
	//				this->mMutex.lock();
	//				if (lc_pkgNumber != 0)
	//				{
	//					auto find_iter = this->msgRespond.find(lc_pkgNumber);
	//					if (find_iter != this->msgRespond.end())
	//					{
	//						std::shared_ptr<tagRpdMsg> lc_spRpdMsg = find_iter->second.lock();
	//						if (lc_spRpdMsg)
	//						{
	//							if (_idx < (int)lc_spRpdMsg->vMsgRpdData.size())
	//								lc_spRpdMsg->vMsgRpdData[_idx].rpdMsg = lc_pkg;
	//						}
	//					}
	//				}
	//				this->mMutex.unlock();
	//				lc_set_event = true;
	//			}
	//			if (lc_set_event)
	//				this->mThreadEvent.setEvent();
	//		});
	//		lc_spSocket->setWriteEvent([lc_spClientSession](int _surpSize) {
	//			lc_spClientSession->atmiWriteSurplusSize = _surpSize;
	//		});
	//		lc_spSocket->setExceptionEvent([_wpClientSession, this, _idx]() {
	//			this->postThreadTask(1 * 1000, [_wpClientSession, this, _idx]() {
	//				this->_initSocket(_wpClientSession, _idx);
	//			});
	//		});
	//	}
	//	void _initSocket(std::weak_ptr<tagClientSession> _wpClientSession, int _idx)
	//	{
	//		std::shared_ptr<tagClientSession> lc_spClientSession = _wpClientSession.lock();
	//		if (!lc_spClientSession)
	//			return;
	//		std::shared_ptr<TxSysIocp::TxTcpClientSocket> lc_spSocket;
	//		for (;;)
	//		{
	//			this->mMutex.lock();
	//			lc_spSocket = lc_spClientSession->wpSocket.lock();
	//			this->mMutex.unlock();
	//			if (!lc_spSocket)
	//				break;
	//			assert(false);
	//			lc_spSocket->close();
	//			::Sleep(50);
	//		}
	//		lc_spSocket.reset(new TxSysIocp::TxTcpClientSocket(TxSysIocp::TxIocpServiceSingleton::getInstance(), 1440));
	//		this->mMutex.lock();
	//		lc_spClientSession->wpSocket = lc_spSocket;
	//		this->mMutex.unlock();
	//		lc_spSocket->setConnectEvent([_wpClientSession, this, _idx](bool _bConneted) {
	//			if (_bConneted)
	//			{//连接成功
	//				this->_socketConnectedFinish_(_wpClientSession, _idx);
	//			}
	//			else
	//			{
	//				this->postThreadTask(3 * 1000, [_wpClientSession, this, _idx]() {
	//					this->_initSocket(_wpClientSession, _idx);
	//				});
	//			}
	//		});
	//
	//		lc_spSocket->asyncConnect(lc_spClientSession->mClientData.ip.c_str(), 8806);
	//	}
	//	void setIp(const std::list<std::string> &_listIp)
	//	{
	//		this->clear();
	//		this->vClientSession.resize(_listIp.size());
	//		int lc_pos = 0;
	//		for (auto iter = _listIp.cbegin(); iter != _listIp.cend(); ++iter)
	//		{
	//			std::string lc_ip = *iter;
	//			std::shared_ptr<tagClientSession> lc_spClientSession = std::make_shared<tagClientSession>();
	//			this->vClientSession[lc_pos] = lc_spClientSession;
	//			lc_spClientSession->atmiWriteSurplusSize = 0;
	//			lc_spClientSession->mClientData.ip = lc_ip;
	//			lc_spClientSession->mClientData.mLifePeriod = TxObjectLifePeriod(&lc_spClientSession->mEntityValid);
	//			_initSocket(lc_spClientSession, lc_pos);
	//			++lc_pos;
	//		}
	//	}
	//};

	class TxUdpSocket : public TxIocpSocketBasic
	{
	private:
		unsigned char pReadBuff[15 * 1024];
		unsigned char __buff_tem[128];
		std::function<void()> fnExceptionEvent;
		std::function<void(const TxNetAddrIn&, const unsigned char*, int)> fnReadEvent;
		std::function<void(int)> fnWriteEvent;
		std::function<void(void)> fnDestroyEvent;
		std::list<std::vector<unsigned char>> listWriteBuffer;//sizeof(SOCKADDR_IN) + sizeof(WSABUF) + _size
		LONG __dsfdsdf__1;
		int iTotalSurplusSendSize;
		LONG __dsfdsdf__2;
		BOOL bWrittingSign;
		LONG __dsfdsdf__3;
		TxSysAtomicLong bAtmReading;
	private:
		static BOOL _staticStartUdpRead__(std::shared_ptr<TxIocpSocketBasic> &_sp, void *_orgOvrLpdPtr)
		{
			struct tagIocpOverlappedWork_Ext : public tagIocpOverlappedWork_
			{
				DWORD dwNumberOfBytesRecvd;
				DWORD dwFlags;
				TxNetAddrIn mNetAddr; char _temAddrOffset[8];
				INT iNetAddrSize;
				BOOL bSocketValid;

				std::shared_ptr<TxIocpSocketBasic> sp;
				tagIocpOverlappedWork_Ext(std::shared_ptr<TxIocpSocketBasic> *_sp,
					TxIocpService *_pTxIocpService)
					:sp(*_sp), tagIocpOverlappedWork_(_pTxIocpService, g_func)
				{
					this->dwNumberOfBytesRecvd = 0;
					this->dwFlags = 0;
					this->iNetAddrSize = sizeof(this->mNetAddr);
					this->bSocketValid = TRUE;
				}
				static bool g_func(OVERLAPPED *_obj, int _iNumberOfBytes, bool _bResultStatus)
				{
					tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext
						= tagIocpOverlappedWork_::getObjectOrgOverLappedPointer<tagIocpOverlappedWork_Ext>(_obj);
					TxUdpSocket *pThis = _My_IocpGlobalFunction_::buildDynamicCast<TxUdpSocket*>(pIocpOverlappedWork_Ext->sp.get());
					BOOL bSec = FALSE;
					if (pThis != NULL&&pIocpOverlappedWork_Ext->bSocketValid)
					{
						if (_iNumberOfBytes > 0 && _bResultStatus)
							pThis->_dealReadBufferEvent_(pIocpOverlappedWork_Ext->mNetAddr, pThis->pReadBuff, _iNumberOfBytes);
						_staticStartUdpRead__(pIocpOverlappedWork_Ext->sp, pIocpOverlappedWork_Ext);
						bSec = TRUE;
					}
					if (!bSec)
					{
						if (pThis != NULL)
						{
							pThis->close();
							pThis->_dealReadExceptionEvent_();
						}
						delete pIocpOverlappedWork_Ext;
					}
					return true;
				}
			};
			tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext = (tagIocpOverlappedWork_Ext*)_orgOvrLpdPtr;
			TxUdpSocket *pThis = _My_IocpGlobalFunction_::buildDynamicCast<TxUdpSocket*>(_sp.get());
			BOOL bLcPushMsgSuc = FALSE;
			if (pThis != NULL)
			{
				if (pIocpOverlappedWork_Ext == NULL)
				{
					pIocpOverlappedWork_Ext = new tagIocpOverlappedWork_Ext(&_sp, pThis->pTxIocpService);
				}
				WSABUF mLcBuf;
				mLcBuf.buf = (char*)pThis->pReadBuff;
				mLcBuf.len = sizeof(pThis->pReadBuff);
				pThis->lockObject();
				pThis->getIocpService()->serviceQueueMutexLock();
				bLcPushMsgSuc = (pThis->getRiskSocket() != INVALID_SOCKET
					&& (SOCKET_ERROR != ::WSARecvFrom(pThis->getRiskSocket(), &mLcBuf, 1,
						&pIocpOverlappedWork_Ext->dwNumberOfBytesRecvd, &pIocpOverlappedWork_Ext->dwFlags,
						(SOCKADDR*)pIocpOverlappedWork_Ext->mNetAddr.getObject(), &pIocpOverlappedWork_Ext->iNetAddrSize,
						&pIocpOverlappedWork_Ext->mOVERLAPPED, NULL)
						|| ::WSAGetLastError() == ERROR_IO_PENDING));
				pThis->getIocpService()->serviceQueueMutexUnlock();
				pThis->unlockObject();
			}
			if (!bLcPushMsgSuc&&pIocpOverlappedWork_Ext != NULL)
			{
				pIocpOverlappedWork_Ext->bSocketValid = FALSE;
				pIocpOverlappedWork_Ext->pushQueueFailureWork();
				pThis->bAtmReading.setValue(0);
				return FALSE;
			}
			return TRUE;
		}

		static void _staticStartUdpWrite__(const SOCKADDR_IN *_addr, const unsigned char *_data, int _size,
			std::shared_ptr<TxIocpSocketBasic> &_sp, void *_orgOvrLpdPtr, BOOL _bWriteOne)
		{
			struct tagIocpOverlappedWork_Ext : public tagIocpOverlappedWork_
			{
				std::weak_ptr<TxIocpSocketBasic> wp;
				DWORD dwNumberOfBytesRecvd;
				DWORD dwSurplusTotalSize;
				BOOL bOverlappedValid;
				tagIocpOverlappedWork_Ext(std::shared_ptr<TxIocpSocketBasic> *_sp, TxIocpService *_pTxIocpService, DWORD _dwSurplusTotalSize)
					:wp(*_sp), tagIocpOverlappedWork_(_pTxIocpService, g_func), dwSurplusTotalSize(_dwSurplusTotalSize)
				{
					this->dwNumberOfBytesRecvd = 0;
					this->bOverlappedValid = TRUE;
				}
				static bool g_func(OVERLAPPED *_obj, int _iNumberOfBytes, bool _bResultStatus)
				{
					TX_UNUSED(_iNumberOfBytes); TX_UNUSED(_bResultStatus);
					tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext
						= tagIocpOverlappedWork_::getObjectOrgOverLappedPointer<tagIocpOverlappedWork_Ext>(_obj);
					std::shared_ptr<TxIocpSocketBasic> lc_sp(pIocpOverlappedWork_Ext->wp.lock());
					TxUdpSocket *pThis = _My_IocpGlobalFunction_::buildDynamicCast<TxUdpSocket*>(lc_sp.get());
					BOOL bLcDoGood = FALSE;
					if (pThis != NULL)
					{
						if (pIocpOverlappedWork_Ext->bOverlappedValid)
						{
							pThis->_dealWriteBufferEvent((int)(pIocpOverlappedWork_Ext->dwSurplusTotalSize));
							_staticStartUdpWrite__(NULL, NULL, 0, lc_sp, pIocpOverlappedWork_Ext, TRUE);
							bLcDoGood = TRUE;
						}
					}
					if (!bLcDoGood)
						delete pIocpOverlappedWork_Ext;
					return true;
				}
			};

			tagIocpOverlappedWork_Ext *pIocpOverlappedWork_Ext = (tagIocpOverlappedWork_Ext*)_orgOvrLpdPtr;
			BOOL bLcPushMsgSuc = FALSE;
			if (_sp)
			{
				TxUdpSocket *pThis = _My_IocpGlobalFunction_::buildDynamicCast<TxUdpSocket*>(_sp.get());
				std::vector<unsigned char> lcVctWriteBuffer;
				if (_addr != NULL&&_data != NULL&&_size > 0)
				{
					lcVctWriteBuffer.resize(sizeof(SOCKADDR_IN) + sizeof(WSABUF) + _size);
					unsigned char *lc_data_ptr = lcVctWriteBuffer.data();
					*(SOCKADDR_IN*)lc_data_ptr = *_addr;
					WSABUF *lc_wsabuf = (WSABUF*)(lc_data_ptr + sizeof(SOCKADDR_IN));
					lc_wsabuf->buf = (char*)lc_data_ptr + sizeof(SOCKADDR_IN) + sizeof(WSABUF);
					lc_wsabuf->len = (u_long)_size;
					memcpy(lc_wsabuf->buf, _data, _size);
				}
				WSABUF lcUdpWsaBuf = { 0 };
				SOCKADDR_IN lcUdpAddrIn = { 0 };
				pThis->lockObject();
				if (lcVctWriteBuffer.size() > 0)
				{
					pThis->iTotalSurplusSendSize += (LONG)lcVctWriteBuffer.size() - (sizeof(SOCKADDR_IN) + sizeof(WSABUF));
					pThis->listWriteBuffer.push_back(std::vector<unsigned char>());
					pThis->listWriteBuffer.back().swap(lcVctWriteBuffer);
					((WSABUF*)((char*)pThis->listWriteBuffer.back().data() + sizeof(SOCKADDR_IN)))->buf
						= (char*)pThis->listWriteBuffer.back().data() + sizeof(SOCKADDR_IN) + sizeof(WSABUF);
				}
				while (pThis->listWriteBuffer.size() > 0)
				{
					if (_bWriteOne)
					{
						pThis->iTotalSurplusSendSize -= (int)pThis->listWriteBuffer.front().size() - (sizeof(SOCKADDR_IN) + sizeof(WSABUF));
						pThis->listWriteBuffer.pop_front();
					}
					if ((_bWriteOne || !pThis->bWrittingSign) && pThis->listWriteBuffer.size() > 0)//Write operation idle
					{
						lcUdpAddrIn = *(SOCKADDR_IN*)pThis->listWriteBuffer.front().data();
						lcUdpWsaBuf = *(WSABUF*)((char*)pThis->listWriteBuffer.front().data() + sizeof(SOCKADDR_IN));
					}
					if (lcUdpWsaBuf.len > 0)
					{
						if (pIocpOverlappedWork_Ext == NULL)
						{
							pIocpOverlappedWork_Ext = new tagIocpOverlappedWork_Ext(&_sp, pThis->pTxIocpService,
								(DWORD)pThis->iTotalSurplusSendSize - (DWORD)lcUdpWsaBuf.len);
						}
						DWORD dwFlags = 0;
						pThis->getIocpService()->serviceQueueMutexLock();
						bLcPushMsgSuc = (pThis->getRiskSocket() != INVALID_SOCKET
							&& (SOCKET_ERROR != ::WSASendTo(pThis->getRiskSocket(), &lcUdpWsaBuf, 1,
								&pIocpOverlappedWork_Ext->dwNumberOfBytesRecvd, dwFlags,
								(const struct sockaddr*)&lcUdpAddrIn, sizeof(SOCKADDR_IN),
								&pIocpOverlappedWork_Ext->mOVERLAPPED, NULL)
								|| ::WSAGetLastError() == ERROR_IO_PENDING));
						pThis->getIocpService()->serviceQueueMutexUnlock();
						if (bLcPushMsgSuc)
						{
							pThis->bWrittingSign = TRUE;
							break;
						}
						else
						{
							_bWriteOne = TRUE;
							continue;
						}
					}
					else
					{
						if (_bWriteOne)
							pThis->bWrittingSign = FALSE;
						break;
					}
				}
				pThis->unlockObject();
			}
			if (!bLcPushMsgSuc&&pIocpOverlappedWork_Ext != NULL)
			{
				pIocpOverlappedWork_Ext->bOverlappedValid = FALSE;
				pIocpOverlappedWork_Ext->pushQueueFailureWork();
			}
		}
		void _dealReadExceptionEvent_()
		{
			if (this->fnExceptionEvent)
				this->fnExceptionEvent();
		}
		void _dealReadBufferEvent_(const TxNetAddrIn &_addr, const unsigned char *_data, int _size)
		{
			if (this->fnReadEvent)
				this->fnReadEvent(_addr, _data, _size);
		}
		void _dealWriteBufferEvent(int _iSurplusSize)
		{
			if (this->fnWriteEvent)
				this->fnWriteEvent(_iSurplusSize);
		}
	public:
		TxUdpSocket(TxIocpService *_pTxIocpService) : TxIocpSocketBasic(_pTxIocpService, 2)
		{
			TxSysDebugObjectMemory::malloc(this);
			this->bAtmReading.setValue(0);
			this->iTotalSurplusSendSize = 0;
			this->bWrittingSign = FALSE;
		}
		~TxUdpSocket()
		{
			if (this->fnDestroyEvent)
				this->fnDestroyEvent();
			TxSysDebugObjectMemory::free(this);
		}
		void setExceptionEvent(const std::function<void()> &_fnException)
		{
			this->fnExceptionEvent = _fnException;
		}
		void setReadEvent(const std::function<void(const TxSysIocp::TxNetAddrIn&, const unsigned char*, int)> &_fnRead)
		{
			this->fnReadEvent = _fnRead;
		}
		void setWriteEvent(const std::function<void(int)> &_fnWrite)
		{
			this->fnWriteEvent = _fnWrite;
		}
		void setDestroyEvent(const std::function<void()> &_fnDestroyEvent)
		{
			this->fnDestroyEvent = _fnDestroyEvent;
		}
		bool start(int _port)
		{
			if (_port > 0 && _port < 65536)
			{
				TxNetAddrIn lc_addr;
				lc_addr.set(NULL, _port);
				return this->start(lc_addr);
			}
			return this->start();
		}
		bool start()
		{
			TxNetAddrIn lc_addr;
			lc_addr.setDefault();
			return this->start(lc_addr);
		}
		bool start(const TxNetAddrIn &_addr)
		{
			SOCKET ss = this->getRiskSocket();
			if (ss != INVALID_SOCKET)
			{
				SOCKADDR* p_lc_addr = (SOCKADDR*)_addr.getObject();
				if (SOCKET_ERROR != ::bind(ss, p_lc_addr, sizeof(*p_lc_addr)))
				{
					if (0 == this->bAtmReading.exchange(1))
						return !!_staticStartUdpRead__(this->shared_from_this(), NULL);
				}
			}
			return false;
		}
		void asyncWrite(const TxNetAddrIn &_addr, const void *_data, int _size)
		{
			if (_data == NULL || _size <= 0) return;
			_staticStartUdpWrite__(_addr.getObject(), (const unsigned char*)_data, _size, this->shared_from_this(), NULL, FALSE);
		}
		static void asyncWrite(std::shared_ptr<TxIocpSocketBasic> &_sp, const TxNetAddrIn &_addr, const void *_data, int _size)
		{
			if (_data == NULL || _size <= 0 || dynamic_cast<TxTcpTrmsSocket*>(_sp.get()) == NULL)
			{
				sysLogError("if (_data == NULL || _size <= 0|| dynamic_cast<TxTcpTrmsSocket*>(_sp.get()))参数错误。_sp=[{0}],_data=[{1}],_size=[{2}]").args(
					(size_t)dynamic_cast<TxTcpTrmsSocket*>(_sp.get()), (size_t)_data, _size);
				return;
			}
			_staticStartUdpWrite__(_addr.getObject(), (const unsigned char*)_data, _size, _sp, NULL, FALSE);
		}
		void asyncWrite(const TxNetAddrIn &_addr, const std::string &_data)
		{
			if (_data.length() <= 0) return;
			_staticStartUdpWrite__(_addr.getObject(), (const unsigned char*)_data.c_str(), (int)_data.size(), this->shared_from_this(), NULL, FALSE);
		}
		void asyncWrite(const TxNetAddrIn &_addr, const TxBlobString &_data)
		{
			if (_data.length() <= 0) return;
			_staticStartUdpWrite__(_addr.getObject(), (const unsigned char*)_data.c_str(), (int)_data.size(), this->shared_from_this(), NULL, FALSE);
		}
	};

}

/*
[English]
function: bool onWriteBufferEvent(int _iSurplusSize)
1. The functions maybe enter at the same time more than two threads：
2. When[_iSurplusSize] equals 0, And call[asyncWrite] here.Then before this[onWriteBufferEvent] operation is fully exited,
it is possible that another[onWriteBufferEvent] will arrive.

function : onAcceptPeerSocketEvent onSocketReadBufferEvent
1. These functions ensure that only one thread enters at the same time.

[TxTcpServerSocket] after close, there may be a residual new peer before accept success, so after close,
there is still likely to join the peer list success.So,
you should add a class variable that indicates that server is not available until server is ready to execute close,
and if you have accept peer later, you need to close it.

function : onSocketReadBufferEvent
Processing[onSocketReadBufferEvent] messages must wait until all local operations are completed and then send data back to the client.

[Chinese]
CmJvb2wgb25Xcml0ZUJ1ZmZlckV2ZW50KGludCBfaVN1cnBsdXNTaXplKeezu+WIl+WHveaVsOacieWPr+iDveS4pOadoee6v+eoi+WQjOaXtui/m+WFpe+8mgrlvZNfaVN1cnBsdXNTaXpl6L+U5ZueMOaXtu+8jOeEtuWQjuWcqOivpeWHveaVsOmHjOmdouWPiOiwg+eUqGFzeW5jV3JpdGXjgILpgqPkuYjlnKjor6Xmk43kvZzov5jmsqHlrozlhajpgIDlh7rliY3vvIzmnInlj6/og73lj6blpJbkuIDkuKpvbldyaXRlQnVmZmVyRXZlbnTliLDovr7jgIIKCm9uQWNjZXB0UGVlclNvY2tldEV2ZW50IG9uU29ja2V0UmVhZEJ1ZmZlckV2ZW50562J6L+Z5Lqb5Ye95pWw5bCx5L+d6K+B5ZCM5LiA5pe25Yi75Y+q5pyJ5LiA5p2h57q/56iL6L+b5YWl44CCCgpUeFRjcFNlcnZlclNvY2tldOWcqOaJp+ihjGNsb3Nl5LmL5ZCO77yM5Y+v6IO95pyJ5q6L55WZ55qEbmV3IHBlZXLkuYvliY3lsLFhY2NlcHTmiJDlip/vvIzmiYDku6XlnKhjbG9zZeS5i+WQjuS7jeeEtuWKoOWFpXBlZXLpk77ooajmiJDlip/jgIIK5omA5Lul5bqU6K+l5Yqg5LiA5Liq57G75Y+Y6YeP77yM5Zyoc2VydmVy5YeG5aSH5omn6KGMY2xvc2XkuYvliY3moIflv5dzZXJ2ZXLlt7Lnu4/kuI3lj6/nlKjvvIzlpoLmnpzmraTlkI7ov5hhY2NlcHQgcGVlcuWNs+mcgOaKiuS7luWFs+mXreWNs+WPr+OAggoK5aSE55CGb25Tb2NrZXRSZWFkQnVmZmVyRXZlbnTmtojmga/vvIzlv4XpobvnrYnliLDmiYDmnInmnKzlnLDmk43kvZznu5PmnZ/lkI7vvIzlho3lj5HmlbDmja7lm57lj5Hnu5nlrqLmiLfnq6/jgIIK
*/

//--------------------------------------TxSyslib--------------------------------------//
class TxSyslib : public _TxPrivateNs_::CSyslib
{
public:
	static void initEnv()
	{
		TxStdlib::initEnv();
		TxStdlib::executeOnce([]() {
			calcSystemCpuOccupyRate();
			getWorkingDirectory(NULL);
			getHighPrecisionMicrosecond();
			_TxPrivateNs_::_MySysTimerPreciseDelaySetInstance__::initEnv();
			TxSysFileSlice::initEnv();
		});
	}
};


#undef _Def_tx_sysMessageBoxDebugInfo___

