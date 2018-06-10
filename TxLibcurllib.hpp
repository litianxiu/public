#pragma once

#include "TxStdlib.hpp"
#include "TxSyslib.hpp"

extern "C"
{
#define BUILDING_LIBCURL
#include <curl/curl.h>
#undef BUILDING_LIBCURL
}
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"wldap32.lib")
#pragma comment(lib, "advapi32.lib")
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <memory>
#include <functional>

namespace _TxPrivateNs_ {
	class CLibcurlBase
	{
	public:
		typedef enum {
			EMLCBT_NULL = 0,
			EMLCBT_ERROR,
			EMLCBT_PROGRESSFUNCTION,
			EMLCBT_READFUNCTION,
			EMLCBT_WRITEFUNCTION,
			EMLCBT_CHUNK_BGN_FUNCTION,
			EMLCBT_CHUNK_END_FUNCTION,
		} EnumMsgLibcurlCallBackType_;
		typedef struct {
			EnumMsgLibcurlCallBackType_ cmd;
			union
			{
				struct {
					struct {
						size_t size;
						unsigned char *buffer;
					} inData;
					struct {
						size_t dataSize;
					} outData;
				}m_READFUNCTION;
				struct {
					struct {
						unsigned char *buffer;
						int size;
					} inData;
					struct {
					} outData;
				}m_WRITEFUNCTION;
				struct {
					struct {
						float fUploadRatio;		// 0.0 - 1.0
						float fDownloadRatio;	// 0.0 - 1.0
						int iUploadSpeed;		// B/s
						int iDownloadSpeed;	// B/s
					} inData;
					struct {
					} outData;
				}m_PROGRESSFUNCTION;
				struct {
					struct {
						int iDir1File2;
						const char *filename;
						long long iFileSize;
					} inData;
					struct {
						bool bSkip;
					} outData;
				} m_CHUNK_BGN_FUNCTION;
				struct {
					struct {
					} inData;
					struct {
					} outData;
				}m_CHUNK_END_FUNCTION;
			};
		} TCallBackParams;
	protected:
		enum EnumObjectType {
			eObjectType_Null = 0,
			eObjectType_FileDownload,
			eObjectType_FtpWildcardDownload,
			eObjectType_FtpUpload,
			eObjectType_HttpPost,
			eObjectType_HttpGet,
			eObjectType_MailSend,
		};
		CURL *curl;
		std::function<bool(TCallBackParams*)> fn_cb;
		double dDownloadStartPos;
		double dUploadStartPos;
		struct tagPostMsgInfo {
			TxBlobString name;
			TxBlobString value;
			CURLformoption formoption;
			tagPostMsgInfo(const TxBlobString &_name, const TxBlobString &_value, CURLformoption _formoption)
			{
				this->name = _name;
				this->value = _value;
				this->formoption = _formoption;
			}
		};
		std::list<tagPostMsgInfo> listPostInfo;
		std::list<TxBlobString> list_http_header;
		std::list<TxBlobString> list_mailReceiver;//邮件接受者
		TxBlobString post_fields;
		int iMsgCbResError;//错误状态。0代表正常,1代表返回错误，2代表执行错误回掉消息。
		int iNoDataTimeOutSecond;
		struct {
			double dNowDownloaded;
			double dNowUploaded;
			std::time_t lst_time;
			int progress_count;
		} mNetExceptionInfo;
		bool bIgnoreSslVerify;
	private:
		static size_t _read_callback_(void *buffer, size_t size, size_t nmemb, void *userp)
		{
			TCallBackParams msg = { EMLCBT_NULL };
			size_t ret = 0;
			CLibcurlBase *pThis = (CLibcurlBase*)userp;
			msg.cmd = EMLCBT_READFUNCTION;
			auto obj = &msg.m_READFUNCTION;
			obj->inData.buffer = (unsigned char*)buffer;
			obj->inData.size = nmemb;
			if (!pThis->_cbMsgLibcurlCallBackFunc_(msg))
				ret = CURL_READFUNC_ABORT;
			else ret = obj->outData.dataSize;
			return ret;
		}
		static size_t _write_callback_(char *buffer, size_t size, size_t nmemb, void *userp)
		{
			size_t ret = 0;
			TCallBackParams msg = { EMLCBT_NULL };
			msg.cmd = EMLCBT_WRITEFUNCTION;
			CLibcurlBase *pThis = (CLibcurlBase*)userp;
			auto obj = &msg.m_WRITEFUNCTION;
			obj->inData.buffer = (unsigned char*)buffer;
			obj->inData.size = nmemb;
			if (!pThis->_cbMsgLibcurlCallBackFunc_(msg))
				ret = CURLE_WRITE_ERROR;
			else ret = nmemb;
			return ret;
		}
		static int _prog_cb_PROGRESSFUNCTION(void *userp, double _dTotalToDownload, double _dNowDownloaded, double _dTotalToUpload, double _dNowUploaded)
		{
			CLibcurlBase *pThis = (CLibcurlBase*)userp;
			double dLcUpLoadSpeedValue = 0, __oooii_12_ = 0, dLcDownLoadSpeedValue = 0, __oooii_12_3_ = 0; (void)__oooii_12_; (void)__oooii_12_3_;
			TCallBackParams msg = { EMLCBT_NULL };
			msg.cmd = EMLCBT_PROGRESSFUNCTION;
			auto obj = &msg.m_PROGRESSFUNCTION;
			if (_dTotalToDownload > 1e-8)
			{
				obj->inData.fDownloadRatio = TxStdlib::adjustRange(
					(float)((_dNowDownloaded + pThis->dDownloadStartPos) / (_dTotalToDownload + pThis->dDownloadStartPos)),
					0.0f, 0.99999f);
				//if (obj->inData.fDownloadRatio < 0) obj->inData.fDownloadRatio = 0;
				//if (obj->inData.fDownloadRatio >= 1) obj->inData.fDownloadRatio = 0.99999f;
			}
			else if (_dTotalToUpload > 1e-8)
			{
				obj->inData.fUploadRatio = TxStdlib::adjustRange(
					(float)((_dNowUploaded + pThis->dUploadStartPos) / (_dTotalToUpload + pThis->dUploadStartPos)),
					0.0f, 0.99999f);
				//if (obj->inData.fUploadRatio < 0) obj->inData.fUploadRatio = 0;
				//if (obj->inData.fUploadRatio >= 1) obj->inData.fUploadRatio = 0.99999f;
			}
			if (CURLE_OK != ::curl_easy_getinfo(pThis->curl, CURLINFO_SPEED_UPLOAD, &dLcUpLoadSpeedValue))
				dLcUpLoadSpeedValue = 0;
			if (CURLE_OK != curl_easy_getinfo(pThis->curl, CURLINFO_SPEED_DOWNLOAD, &dLcDownLoadSpeedValue))
				dLcDownLoadSpeedValue = 0;
			obj->inData.iUploadSpeed = (int)(dLcUpLoadSpeedValue + 0.5);
			obj->inData.iDownloadSpeed = (int)(dLcDownLoadSpeedValue + 0.5);
			bool lc_res_sign = pThis->_cbMsgLibcurlCallBackFunc_(msg);

			//判断连接失效
			if (pThis->mNetExceptionInfo.dNowDownloaded != _dNowDownloaded || pThis->mNetExceptionInfo.dNowUploaded != _dNowUploaded)
			{
				pThis->mNetExceptionInfo.dNowDownloaded = _dNowDownloaded;
				pThis->mNetExceptionInfo.dNowUploaded = _dNowUploaded;
				//pThis->mNetExceptionInfo.lst_time = lc_cur_time;
				pThis->mNetExceptionInfo.progress_count = 0;
			}
			else
			{
				std::time_t lc_cur_time = { 0 };
				::time(&lc_cur_time);
				if (std::difftime(lc_cur_time, pThis->mNetExceptionInfo.lst_time) > (double)0.95)
				{
					pThis->mNetExceptionInfo.lst_time = lc_cur_time;
					++pThis->mNetExceptionInfo.progress_count;
				}
				if (pThis->iNoDataTimeOutSecond > 0 && pThis->mNetExceptionInfo.progress_count > pThis->iNoDataTimeOutSecond)
				{
					sysLogError("libcur长时间没传输数据，所以退出操作。time:[{0}]").args(pThis->iNoDataTimeOutSecond);
					assert(false);
					lc_res_sign = false;
				}
			}
			return lc_res_sign ? 0 : -1;
		}
		static long _cb_CHUNK_BGN_FUNCTION(struct curl_fileinfo *finfo, CLibcurlBase *userp, int remains)
		{
			TCallBackParams msg = { EMLCBT_NULL };
			long ret = CURL_CHUNK_BGN_FUNC_OK;
			CLibcurlBase *pThis = (CLibcurlBase*)userp;
			msg.cmd = EMLCBT_CHUNK_BGN_FUNCTION;
			auto obj = &msg.m_CHUNK_BGN_FUNCTION;
			switch (finfo->filetype) {
			case CURLFILETYPE_DIRECTORY:
				obj->inData.iDir1File2 = 1;
				//printf(" DIR\n");
				break;
			case CURLFILETYPE_FILE:
				obj->inData.iDir1File2 = 2;
				obj->inData.filename = finfo->filename;
				obj->inData.iFileSize = finfo->size;
				break;
			default:
				msg.cmd = EMLCBT_NULL;
				break;
			}
			if (!pThis->_cbMsgLibcurlCallBackFunc_(msg))
				ret = CURL_CHUNK_BGN_FUNC_FAIL;
			else if (obj->outData.bSkip) ret = CURL_CHUNK_BGN_FUNC_SKIP;
			return ret;
		}
		static long _cb_CHUNK_END_FUNCTION(CLibcurlBase *userp)
		{
			TCallBackParams msg = { EMLCBT_NULL };
			long ret = CURL_CHUNK_END_FUNC_OK;
			CLibcurlBase *pThis = (CLibcurlBase*)userp;
			msg.cmd = EMLCBT_CHUNK_END_FUNCTION;
			auto obj = &msg.m_CHUNK_END_FUNCTION;
			if (!pThis->_cbMsgLibcurlCallBackFunc_(msg))
				ret = CURL_CHUNK_END_FUNC_FAIL;
			return ret;
		}
		bool _cbMsgLibcurlCallBackFunc_(TCallBackParams &_mTCallBackParams)
		{
			if ((void*)this == NULL || _mTCallBackParams.cmd == EMLCBT_NULL)
			{
				sysLogErrorW(L"if ((void*)this == NULL || this->cb == NULL || _mTCallBackParams.cmd == EMLCBT_NULL)参数错误。"
					L"this=【{0}】,_mTCallBackParams.cmd=【{1}】").args((size_t)this, _mTCallBackParams.cmd);
				return false;
			}
			if (this->iMsgCbResError == 0 || (this->iMsgCbResError == 1 && _mTCallBackParams.cmd == EMLCBT_ERROR))
			{
				if (this->fn_cb)
				{
					if (!this->fn_cb(&_mTCallBackParams))
						this->iMsgCbResError = 1;
				}
				if (_mTCallBackParams.cmd == EMLCBT_ERROR)
					this->iMsgCbResError = 2;
			}
			return this->iMsgCbResError == 0;
		}
	protected:
		static void __initialize_libcurl_env_()
		{
			class TTT {
			public:
				TTT() {
					CURLcode lc_res = ::curl_global_init(CURL_GLOBAL_ALL);
					if (lc_res != CURLE_OK)
					{
						sysLogError("CLibcurlBase封装库，curl_global_init()调用失败。错误代码：[{0}]").args(lc_res);
						assert(false);
						lc_res = ::curl_global_init(CURL_GLOBAL_DEFAULT);
						if (lc_res != CURLE_OK)
							assert(false);
					}
					else
					{
						sysLogInfo("CLibcurlBase封装库，curl_global_init()调用成功");
					}
				}
				~TTT() {
					::curl_global_cleanup();
					if (TxSysLog::object())
						sysLogInfo("CLibcurlBase封装库，调用curl_global_cleanup()");
				}
			};
			static TTT _init_tem_obj; TX_UNUSED(_init_tem_obj);
		}
		void reset_init(EnumObjectType _objType)
		{
			this->dDownloadStartPos = 0;
			this->dUploadStartPos = 0;
#ifndef NDEBUG
			::curl_easy_setopt(this->curl, CURLOPT_VERBOSE, 1L);
#endif
			::curl_easy_setopt(this->curl, CURLOPT_NOSIGNAL, 1L);
			::curl_easy_setopt(this->curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
			::curl_easy_setopt(this->curl, CURLOPT_READFUNCTION, _read_callback_);
			::curl_easy_setopt(this->curl, CURLOPT_READDATA, this);
			::curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, _write_callback_);
			::curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, this);
			::curl_easy_setopt(this->curl, CURLOPT_NOPROGRESS, 0L);
			::curl_easy_setopt(this->curl, CURLOPT_PROGRESSFUNCTION, _prog_cb_PROGRESSFUNCTION);
			::curl_easy_setopt(this->curl, CURLOPT_PROGRESSDATA, this);
			::curl_easy_setopt(this->curl, CURLOPT_CHUNK_BGN_FUNCTION, _cb_CHUNK_BGN_FUNCTION);
			::curl_easy_setopt(this->curl, CURLOPT_CHUNK_END_FUNCTION, _cb_CHUNK_END_FUNCTION);
			::curl_easy_setopt(this->curl, CURLOPT_CHUNK_DATA, this);

			switch (_objType)
			{
			case eObjectType_FileDownload:
				::curl_easy_setopt(this->curl, CURLOPT_FTPSSLAUTH, (LONG)CURLFTPAUTH_DEFAULT);
				::curl_easy_setopt(this->curl, CURLOPT_WILDCARDMATCH, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_UPLOAD, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_POST, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 0L);
				break;
			case eObjectType_FtpWildcardDownload:
				::curl_easy_setopt(this->curl, CURLOPT_FTPSSLAUTH, (LONG)CURLFTPAUTH_DEFAULT);
				::curl_easy_setopt(this->curl, CURLOPT_UPLOAD, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_POST, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_WILDCARDMATCH, 1L);
				break;
			case eObjectType_FtpUpload:
				::curl_easy_setopt(this->curl, CURLOPT_FTPSSLAUTH, (LONG)CURLFTPAUTH_DEFAULT);
				::curl_easy_setopt(this->curl, CURLOPT_WILDCARDMATCH, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_POST, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_UPLOAD, 1L);
				break;
			case eObjectType_HttpPost:
				::curl_easy_setopt(this->curl, CURLOPT_FTPSSLAUTH, (LONG)CURLFTPAUTH_DEFAULT);
				::curl_easy_setopt(this->curl, CURLOPT_WILDCARDMATCH, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_UPLOAD, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_POST, 1L);
				break;
			case eObjectType_HttpGet:
				::curl_easy_setopt(this->curl, CURLOPT_FTPSSLAUTH, (LONG)CURLFTPAUTH_DEFAULT);
				::curl_easy_setopt(this->curl, CURLOPT_WILDCARDMATCH, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_UPLOAD, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_POST, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);
				break;
			case eObjectType_MailSend:
				::curl_easy_setopt(this->curl, CURLOPT_FTPSSLAUTH, (LONG)CURLFTPAUTH_SSL);
				::curl_easy_setopt(this->curl, CURLOPT_WILDCARDMATCH, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_POST, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_UPLOAD, 1L);
				break;
			default:
				assert(0);
				break;
			}
		}
	public:
		CLibcurlBase()
		{
			__initialize_libcurl_env_();
			this->bIgnoreSslVerify = false;
			this->setTimeoutSecond(10);
			this->curl = ::curl_easy_init();
		}
		virtual ~CLibcurlBase()
		{
			::curl_easy_cleanup(this->curl);
		}
		void setTimeoutSecond(int _iTimeOutSecond)
		{
			this->iNoDataTimeOutSecond = _iTimeOutSecond;
		}
		void setUrl(const char *_url)
		{
			::curl_easy_setopt(this->curl, CURLOPT_URL, _url);
		}
		void setMailSender(const char *_senderMail)
		{
			::curl_easy_setopt(this->curl, CURLOPT_MAIL_FROM, _senderMail);
		}
		void addMailReceiver(const char *_receiverMail)
		{
			if (_receiverMail != NULL)
			{
				if (_receiverMail[0] != 0)
					this->list_mailReceiver.push_back(_receiverMail);
			}
		}
		void setUsername(const char *_username)
		{
			::curl_easy_setopt(this->curl, CURLOPT_USERNAME, _username);
		}
		void setPassword(const char *_password)
		{
			::curl_easy_setopt(this->curl, CURLOPT_PASSWORD, _password);
		}
		void setRange(long long int _offset, long long int _filesize)
		{
			if (_offset >= 0 && _filesize >= 0)
			{
				this->dDownloadStartPos = (double)_offset;
				this->dUploadStartPos = (double)_offset;
				TxBlobString lc_range_str;
				lc_range_str.compose("{0}-{1}").args((long long int)_offset, (long long int)(_offset + _filesize - 1));
				::curl_easy_setopt(this->curl, CURLOPT_RANGE, lc_range_str.c_str());
			}
			else if (_offset >= 0)
			{
				this->dDownloadStartPos = (double)_offset;
				this->dUploadStartPos = (double)_offset;
				::curl_easy_setopt(this->curl, CURLOPT_RESUME_FROM_LARGE, (curl_off_t)_offset);
			}
		}
		void setFileSize(long long _file_size)
		{
			::curl_easy_setopt(this->curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)_file_size);
		}
		TxBlobString urlEncode(const TxBlobString &_data)
		{
			TxBlobString ret;
			if (this->curl == NULL)
			{
				sysLogError("if (this->curl == NULL)参数错误。");
				return ret;
			}
			char *lc_ptr = ::curl_easy_escape(this->curl, _data.data(), (int)_data.size());
			if (lc_ptr == NULL)
			{
				sysLogError("char *lc_ptr = ::curl_easy_escape(this->curl, _data.data(), (int)_data.size());返回错误。_data=[{0}]").args(_data);
			}
			else
			{
				ret.assign(lc_ptr);
			}
			if (lc_ptr != NULL)
				::curl_free(lc_ptr);
			return ret;
		}
		TxBlobString urlDecode(const TxBlobString &_data)
		{
			TxBlobString ret;
			if (this->curl == NULL)
			{
				sysLogError("if (this->curl == NULL)参数错误。");
				return ret;
			}
			int lc_size = 0;
			char *lc_ptr = ::curl_easy_unescape(this->curl, _data.data(), (int)_data.size(), &lc_size);
			if (lc_ptr == NULL || lc_size <= 0)
			{
				sysLogError("char *lc_ptr = ::curl_easy_unescape(this->curl, _data.data(), (int)_data.size(), &lc_size);返回错误。_data=[{0}]").args(_data);
			}
			else
			{
				ret.assign(lc_ptr, lc_size);
			}
			if (lc_ptr != NULL)
				::curl_free(lc_ptr);
			return ret;
		}
		void formaddPostFile(const TxBlobString &_name, const TxBlobWString &_file)
		{
			this->listPostInfo.push_back(tagPostMsgInfo(_name, _file.utf8Codecvt(), CURLFORM_FILE));
		}
		void formaddPostContent(const TxBlobString &_name, const TxBlobString &_content)
		{
			this->listPostInfo.push_back(tagPostMsgInfo(_name, _content, CURLFORM_COPYCONTENTS));
		}
		void setPostFields(const TxBlobString &_post_fields)
		{
			this->post_fields = _post_fields;
		}
		//"Accept:application/json" //"Content-Type:application/json" //"charset:utf-8"
		void addHttpHeader(const TxBlobString &_header)
		{
			this->list_http_header.push_back(_header);
		}
		void ignoreSslVerify()
		{
			this->bIgnoreSslVerify = true;
		}
		bool perform(const std::function<bool(TCallBackParams*)> &_fn_cb)
		{
			this->fn_cb = _fn_cb;
			this->iMsgCbResError = 0;

			//设置http header
			struct curl_slist *lc_headers = NULL;
			for (auto iter = this->list_http_header.begin(); iter != this->list_http_header.end(); ++iter)
				lc_headers = ::curl_slist_append(lc_headers, iter->c_str());
			if (lc_headers != NULL)
				::curl_easy_setopt(curl, CURLOPT_HTTPHEADER, lc_headers);

			//邮件接收者
			struct curl_slist *lc_mailReceiver = NULL;
			for (auto iter = this->list_mailReceiver.begin(); iter != this->list_mailReceiver.end(); ++iter)
				lc_mailReceiver = ::curl_slist_append(lc_mailReceiver, iter->c_str());
			if (lc_mailReceiver != NULL)
				::curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, lc_mailReceiver);

			if (this->bIgnoreSslVerify)
			{
				::curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYPEER, 0L);
				::curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYHOST, 0L);
			}

			//设置http post语句
			struct curl_httppost *lc_post_bgn = NULL;
			struct curl_httppost *lc_post_end = NULL;
			for (auto iter = this->listPostInfo.cbegin(); iter != listPostInfo.cend(); ++iter)
			{
				CURLformoption lc_formoption = CURLFORM_COPYCONTENTS;
				CURLFORMcode lc_code = ::curl_formadd(&lc_post_bgn, &lc_post_end,
					CURLFORM_COPYNAME, iter->name.c_str(),
					iter->formoption, iter->value.c_str(), CURLFORM_END);
				assert(lc_code == CURL_FORMADD_OK);
				(void)lc_code; lc_code = lc_code;
			}
			if (lc_post_bgn != NULL || lc_post_end != NULL)
				::curl_easy_setopt(this->curl, CURLOPT_HTTPPOST, lc_post_bgn);
			if (this->post_fields.length() > 0)
			{
				::curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, (const char*)this->post_fields.data());
				::curl_easy_setopt(this->curl, CURLOPT_POSTFIELDSIZE, (long)this->post_fields.size());
			}
			//初始化定时检测器
			::time(&this->mNetExceptionInfo.lst_time);
			this->mNetExceptionInfo.progress_count = 0;
			this->mNetExceptionInfo.dNowDownloaded = 0;
			this->mNetExceptionInfo.dNowUploaded = 0;

			//执行URL
			CURLcode res = ::curl_easy_perform(this->curl);
			if (res != CURLE_OK)
			{
				TCallBackParams lc_msg = { EMLCBT_NULL };
				lc_msg.cmd = EMLCBT_ERROR;
				this->_cbMsgLibcurlCallBackFunc_(lc_msg);
				sysLogWarn("CLibcurlBase封装库，curl_easy_perform()调用失败，错误码(CURLcode)：[{0}]").args(res);
			}

			this->bIgnoreSslVerify = false;

			//清理http post信息
			if (lc_post_bgn != NULL)
				::curl_formfree(lc_post_bgn);
			this->listPostInfo.clear();
			this->post_fields.clear();

			//清理http header
			if (lc_headers != NULL)
				::curl_slist_free_all(lc_headers);
			this->list_http_header.clear();

			//清理邮件接收者
			if (lc_mailReceiver != NULL)
				::curl_slist_free_all(lc_mailReceiver);
			this->list_mailReceiver.clear();

			::curl_easy_reset(this->curl);
			this->fn_cb = std::function<bool(TCallBackParams*)>();
			//assert(res == CURLE_OK);
			return res == CURLE_OK&&this->iMsgCbResError == 0;
		}
	};

	class TxLibcurlWrap : public CLibcurlBase
	{
	public:
		void setPostHttpJsonForm(const TxBlobString &_json_data)
		{
			this->addHttpHeader("Accept:application/json");
			this->addHttpHeader("Content-Type:application/json");
			this->addHttpHeader("Charset:utf-8");
			this->setPostFields(_json_data);
		}
		static long long getRemoteFileSize(const char* _url)
		{
			struct TTT {
				static size_t _get_file_size(void *ptr, size_t size, size_t nmemb, void *stream) {
					(void)ptr; (void)size; (void)nmemb; (void)stream;
					return (size_t)(size * nmemb);
				}
			};
			CLibcurlBase::__initialize_libcurl_env_();
			long long ret = -1;
			double d_filesize = 0.0;
			CURL* lc_curl = ::curl_easy_init();
			if (lc_curl != NULL)
			{
				::curl_easy_setopt(lc_curl, CURLOPT_URL, _url);
				::curl_easy_setopt(lc_curl, CURLOPT_NOSIGNAL, 1L);
				::curl_easy_setopt(lc_curl, CURLOPT_NOBODY, 1L);
				::curl_easy_setopt(lc_curl, CURLOPT_FOLLOWLOCATION, 1L);
				::curl_easy_setopt(lc_curl, CURLOPT_HEADERFUNCTION, &TTT::_get_file_size);
				::curl_easy_setopt(lc_curl, CURLOPT_SSL_VERIFYPEER, 0L);
				::curl_easy_setopt(lc_curl, CURLOPT_SSL_VERIFYHOST, 0L);
				CURLcode lc_sign = ::curl_easy_perform(lc_curl);
				if (lc_sign == CURLE_OK)
					lc_sign = curl_easy_getinfo(lc_curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &d_filesize);
				::curl_easy_cleanup(lc_curl);
				if (lc_sign == CURLE_OK)
					ret = (long long)(d_filesize + 0.5);
			}
			return ret;
		}
	public:
		//_url="http://192.168.1.226:8888/KaoShiXinXi.aspx?TYPE=0"
		//先调用formaddPost**或者set_post_fields()函数
		bool performHttpPost(const char _url[],
			const std::function<bool(TxLibcurlWrap::TCallBackParams*)> &_cb = std::function<bool(TCallBackParams*)>())
		{
			CLibcurlBase::reset_init(eObjectType_HttpPost);
			CLibcurlBase::setUrl(_url);
			return CLibcurlBase::perform(_cb);
		}
		//_url="http://ceshi.com:8383/test.asp?test=string"
		bool performHttpGet(const char _url[],
			const std::function<bool(TxLibcurlWrap::TCallBackParams*)> &_cb = std::function<bool(TCallBackParams*)>())
		{
			CLibcurlBase::reset_init(eObjectType_HttpGet);
			CLibcurlBase::setUrl(_url);
			return CLibcurlBase::perform(_cb);
		}
		//_url="ftp://DeviceInfo:DeviceInfo@192.168.1.226:21/abc/while-uploading.txt"
		bool performFtpUpload(const char _url[], long long _file_size,
			const std::function<bool(TxLibcurlWrap::TCallBackParams*)> &_cb = std::function<bool(TCallBackParams*)>())
		{
			CLibcurlBase::reset_init(eObjectType_FtpUpload);
			CLibcurlBase::setFileSize(_file_size);
			CLibcurlBase::setUrl(_url);
			return CLibcurlBase::perform(_cb);
		}
		//url="ftp://DeviceInfo:DeviceInfo@192.168.1.226/soft_system/update_info/123456.zip"
		bool performFileDownload(const char _url[], long long int _offset, long long int _filesize,
			const std::function<bool(TxLibcurlWrap::TCallBackParams*)> &_cb)
		{
			CLibcurlBase::reset_init(eObjectType_FileDownload);
			CLibcurlBase::setUrl(_url);
			CLibcurlBase::setRange(_offset, _filesize);
			return CLibcurlBase::perform(_cb);
		}
		//url="ftp://DeviceInfo:DeviceInfo@192.168.1.226/soft_system/update_info/*.zip"
		bool performFtpWildcardDownload(const char _url[], const std::function<bool(TxLibcurlWrap::TCallBackParams*)> &_cb)
		{
			CLibcurlBase::reset_init(eObjectType_FtpWildcardDownload);
			CLibcurlBase::setUrl(_url);
			return CLibcurlBase::perform(_cb);
		}
		bool performSendMail(const TxBlobString &_senderUrl, const TxBlobString &_senderMail,
			const TxBlobString &_senderUsername, const TxBlobString &_senderPassword,
			const std::list<TxBlobString> &_listReceiver, const TxBlobString &_subjectText, const TxBlobString &_contentData)
		{
			CLibcurlBase::reset_init(eObjectType_MailSend);
			CLibcurlBase::setUrl(_senderUrl.c_str());
			CLibcurlBase::setMailSender(_senderMail.c_str());
			CLibcurlBase::setUsername(_senderUsername.c_str());
			CLibcurlBase::setPassword(_senderPassword.c_str());
			for (auto iter = _listReceiver.cbegin(); iter != _listReceiver.cend(); ++iter)
			{
				if (iter->length() > 0)
					CLibcurlBase::addMailReceiver(iter->c_str());
			}

			TxBlobString lc_to_mail;
			TxBlobString lc_cc_mail_str;
			for (auto iter = _listReceiver.cbegin(); iter != _listReceiver.cend(); ++iter)
			{
				if (iter == _listReceiver.cbegin()) lc_to_mail = *iter;
				else lc_cc_mail_str.appendCompose("Cc: {0}\r\n").args(*iter);
			}

			TxBlobString lc_uploadData;
			lc_uploadData.compose("To: {0}\r\n"
				"From: {1}\r\n"
				"{2}"
				"Subject: {3}\r\n" "\r\n"
				"{4}").args(lc_to_mail, _senderMail, lc_cc_mail_str, _subjectText, _contentData);

			const unsigned char *lc_begin_ptr = (const unsigned char*)lc_uploadData.data();
			const unsigned char *lc_end_ptr = lc_begin_ptr + lc_uploadData.size();
			return CLibcurlBase::perform([&lc_begin_ptr, lc_end_ptr](TCallBackParams *_cbParam)->bool {
				if (_cbParam->cmd == CLibcurlBase::EMLCBT_READFUNCTION)
				{
					auto lc_ptr = &_cbParam->m_READFUNCTION;
					lc_ptr->outData.dataSize = TxStdlib::minimum((int)lc_ptr->inData.size, (int)(lc_end_ptr - lc_begin_ptr));
					if (lc_ptr->outData.dataSize > 0)
					{
						std::memcpy(lc_ptr->inData.buffer, lc_begin_ptr, lc_ptr->outData.dataSize);
						lc_begin_ptr += lc_ptr->outData.dataSize;
					}
				}
				return true;
			});
		}
	};
}

typedef _TxPrivateNs_::TxLibcurlWrap TxLibcurlWrap;

