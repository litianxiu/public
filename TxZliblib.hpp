#pragma once

#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#define ZLIB_WINAPI

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <vector>

#include <boost/locale.hpp>

#include "TxStdlib.hpp"
#include "TxSyslib.hpp"
#include "TxBoostlib.hpp"

#include <zip.h>
#include <unzip.h>

namespace _TxPrivateNs_
{
	class TxZliblib
	{
	protected:
		static std::wstring _asciiToUnicode__(const std::string &_ascii)
		{
			std::string lcArryCharset[] = {
				std::string("utf8"),
				std::string("gbk"),
				std::string("big5"),
				std::string("gb2312"),
				std::string("gb18030"),
				std::string("usascii"),
			};
			if (_ascii.length() <= 0)
				return std::wstring();
			for (int i = 0; i < sizeof(lcArryCharset) / sizeof(lcArryCharset[0]); ++i)
			{
				try {
					std::wstring ret = boost::locale::conv::to_utf<wchar_t>(_ascii,
						lcArryCharset[i], boost::locale::conv::stop);
					if (ret.length() <= 0)
						continue;
					std::string lc_old_str = boost::locale::conv::from_utf<wchar_t>(ret,
						lcArryCharset[i], boost::locale::conv::stop);
					if (lc_old_str != _ascii)
						continue;
					return ret;
				}
				catch (...) {
				}
			}
			return TxSyslib::sysCodecvt(_ascii);
		}
		static bool _transStringToPathNode(std::vector<TxBlobWString> *_lcNodeName,
			const TxBlobWString &_wsZipFile, const std::string &_filePath)
		{
			_lcNodeName->clear();

			TxBlobWString lc_wfilepath = _asciiToUnicode__(_filePath);
			const wchar_t *pNamePos = lc_wfilepath.c_str();
			const wchar_t *pStartName = lc_wfilepath.c_str();
			const wchar_t *pEndName = lc_wfilepath.c_str() + lc_wfilepath.length();
			for (; pNamePos < pEndName; pNamePos++)
			{
				if ((*pNamePos) == L'/' || (*pNamePos) == L'\\')
				{
					if (pNamePos - pStartName > 0)
						_lcNodeName->push_back(TxBlobWString(pStartName, (int)(pNamePos - pStartName)));
					pStartName = pNamePos + 1;
				}
			}
			if (pNamePos - pStartName > 0)
				_lcNodeName->push_back(TxBlobWString(pStartName, (int)(pNamePos - pStartName)));
			if (_lcNodeName->size() <= 0)
			{
				sysLogError("TxZliblib::_transStringToPathNode()，ZIP压缩包【{0}】解压时，无法解析其内部zip目录。【{1}】"
				).args(_wsZipFile, _filePath);
				return false;
			}
			return true;
		}
	public:
		static bool unzip(const TxBlobWString &_wsZipFile, const char _psw[],
			const std::function<void(const std::vector<TxBlobWString> &_wNodeName, long long _llFileSize,
				const std::function<int(void*, int)> &_fn_read)> &_func)
		{
			int lc_err = 0;
			bool ret_false = false, ret_true = true;
			TxBlobString lcZipFileName = TxSyslib::sysCodecvt(_wsZipFile);
			lcZipFileName.replaceSelf('\\', '/');
			lcZipFileName.trimSelfRight("\\/");
			while (lcZipFileName.size() > 0 && lcZipFileName.backChar() == L'\\')
				lcZipFileName.pop_back(1);
			std::vector<TxBlobWString> lcNodeName;
			if (_psw != NULL&&_psw[0] == 0)
				_psw = NULL;
			unzFile uf = NULL;
			char lcInitFileName[(FILENAME_MAX) * 2 + 64]; lcInitFileName[0] = 0;
			bool bNeedCloseCurrentFile = false;
			do {
				ret_false = false;
				unz_global_info64 gi;
				if ((uf = ::unzOpen64(lcZipFileName.c_str())) == NULL)
				{
					sysLogWarn("TxZliblib::unzip(){::unzOpen64()}：打开zip压缩包失败。压缩包路径：【{0}】").args(lcZipFileName);
					break;
				}
				if (!_func)
				{
					sysLogError("TxZliblib::unzip(...,const std::function<int(void*, int)> &_fn_read)>&)：参数错误，回掉函数为空");
					ret_false = true;
					break;
				}
				lc_err = ::unzGetGlobalInfo64(uf, &gi);
				if (lc_err != (int)UNZ_OK)
				{
					sysLogError("TxZliblib::unzip(){::unzGetGlobalInfo64();}，返回错误，错误码={0}").args(lc_err);
					break;
				}
				bool second_sign = true;
				for (int iii = 1; iii <= gi.number_entry; iii++)
				{
					second_sign = false;
					unz_file_info64 lc_file_info = { 0 };
					lc_err = ::unzGetCurrentFileInfo64(uf, &lc_file_info, lcInitFileName,
						sizeof(lcInitFileName) / sizeof(lcInitFileName[0]) - 1, NULL, 0, NULL, 0);
					if (lc_err != (int)UNZ_OK)
					{
						sysLogError("TxZliblib::unzip(){::unzGetCurrentFileInfo64()}：函数返回失败。错误码：[{0}]"
						).args(lc_err);
						break;
					}
					if ((long long int)lc_file_info.uncompressed_size < 0)
					{
						sysLogError("TxZliblib::unzip()：uncompressed_size小于零。uncompressed_size={0}"
						).args(lc_file_info.uncompressed_size);
						break;
					}
					lc_err = ::unzOpenCurrentFilePassword(uf, _psw);
					if (lc_err != (int)UNZ_OK)
					{
						sysLogError("TxZliblib::unzip(){::unzOpenCurrentFilePassword();}，返回错误，错误码={0}"
						).args(lc_err);
						break;
					}
					bNeedCloseCurrentFile = true;
					if (!_transStringToPathNode(&lcNodeName, _wsZipFile, lcInitFileName))
					{
						ret_true = false;
					}
					else if (!!(lc_file_info.external_fa&FILE_ATTRIBUTE_DIRECTORY))//if directory
					{
						if (lc_file_info.uncompressed_size == 0)
						{
							_func(lcNodeName, -1, [](void*, int)->int {return 0; });
						}
						else
						{
							sysLogError("TxZliblib::unzip()：external_fa提示是目录，但是uncompressed_size又不等于零。external_fa={0},uncompressed_size={1}"
							).args(lc_file_info.external_fa, lc_file_info.uncompressed_size);
							ret_true = false;
						}
					}
					else
					{
						_func(lcNodeName, (long long)lc_file_info.uncompressed_size,
							[&lc_file_info, &uf](void *_buffer, int _size)->int {
							int lc_val = (int)::unzReadCurrentFile(uf, _buffer, _size);
							return lc_val < 0 ? 0 : lc_val;
						});
					}
					bNeedCloseCurrentFile = false;
					if (::unzCloseCurrentFile(uf) != UNZ_OK)
						break;
					int lc_un_res_val = ::unzGoToNextFile(uf);
					if ((iii < gi.number_entry&&lc_un_res_val != UNZ_OK)
						|| (iii == gi.number_entry&&lc_un_res_val == UNZ_OK))
					{
						break;
					}
					second_sign = true;
				}
				if (second_sign)
					ret_false = true;
			} while (0);
			if (bNeedCloseCurrentFile&&::unzCloseCurrentFile(uf) != UNZ_OK)
				ret_true = false;
			if (uf != NULL&&::unzClose(uf) != UNZ_OK) ret_true = false;
			return ret_true&&ret_false;
		}
		static bool unzipFile(const TxBlobWString &_wsZipFile, const TxBlobWString &_wsUnpackDir,
			const TxBlobWString &_wsTempkDir, const char _psw[],
			const std::function<bool()> &_loopFunc = []() {return true; })
		{
			TxBlobWString lcWsTempkDir = TxBoostlib::completePath(_wsTempkDir);
			lcWsTempkDir.replaceSelf(L'\\', L'/');
			lcWsTempkDir.trimSelfRight(L"\\/");
			if (lcWsTempkDir.length() > 0)
				lcWsTempkDir.append(L"\\");

			TxBlobWString lcUnpackDir_tem_ = TxBoostlib::completePath(_wsUnpackDir);
			lcUnpackDir_tem_.replaceSelf(L'\\', L'/');
			lcUnpackDir_tem_.trimSelfRight(L"\\/");
			if (lcUnpackDir_tem_.size() > 0 && lcUnpackDir_tem_.backChar() == L'\\')
				lcUnpackDir_tem_.pop_back(1);
			auto lcBuildPathFunc = [&lcUnpackDir_tem_](const std::vector<TxBlobWString> &_wNodeName)->TxBlobWString {
				TxBlobWString lc_ret = lcUnpackDir_tem_;
				for (int i = 0; i < (int)_wNodeName.size(); ++i)
					lc_ret.append(TxBlobWString(L"\\") + _wNodeName[i]);
				return lc_ret;
			};

			std::vector<unsigned char> v_read_chunk_buff;
			v_read_chunk_buff.resize(1024 * 1024 * 4);

			bool ret_true = true;

			bool upzip_sign = unzip(_wsZipFile, _psw, [&lcBuildPathFunc, &lcWsTempkDir, &v_read_chunk_buff, &ret_true, &_loopFunc](
				const std::vector<TxBlobWString> &_wNodeName, long long _llFileSize, const std::function<int(void*, int)> &_fn_read) {
				TxBlobWString lc_ws_path = lcBuildPathFunc(_wNodeName);
				if (_llFileSize < 0)
				{
					if (_loopFunc())
						TxBoostlib::createDirectories(lc_ws_path);
				}
				else
				{
					FILE *lc_fp = NULL;
					while (_loopFunc())
					{
						int lc_size = _fn_read(v_read_chunk_buff.data(), (int)v_read_chunk_buff.size());
						if (lc_size > 0)
						{
							if (lc_fp == NULL)
								lc_fp = TxSyslib::wFileOpen(lc_ws_path, "wb", false);
							if (lc_fp == NULL)
							{
								if (TxBoostlib::isExistPath(lc_ws_path))
								{
									TxBoostlib::deleteAllFile(lc_ws_path);
									if (TxBoostlib::isExistPath(lc_ws_path))
									{
										TxBlobWString lcWsNewFile = lcWsTempkDir + TxSyslib::generateGuid().utf8Codecvt();
										TxBoostlib::moveFile(lcWsNewFile, lc_ws_path);
									}
								}
								else
								{
									TxBlobWString lc_file_dir = TxBoostlib::fileDirectory(lc_ws_path);
									if (lc_file_dir.length() > 0)
										TxBoostlib::createDirectories(lc_file_dir.c_str());
								}
							}
							if (lc_fp == NULL)
								lc_fp = TxSyslib::wFileOpen(lc_ws_path, "wb");
							if (lc_fp == NULL)
							{
								ret_true = false;
								sysLogError("无法创建或打开文件:{0}.").args(lc_ws_path);
							}
							else ::fwrite(v_read_chunk_buff.data(), 1, lc_size, lc_fp);
						}
						else
						{
							break;
						}
					}
					if (lc_fp != NULL)
					{
						::fclose(lc_fp);
						lc_fp = NULL;
					}
				}
			});
			if (!upzip_sign)
			{
				sysLogError("非法解压文件:{0}").args(_wsZipFile);
			}
			return upzip_sign&&ret_true;
		}

		static TxBlobString uncompressData(const TxBlobString &_compressData, int _rawDataSize)
		{
			TxBlobString ret;
			ret.resize(_rawDataSize, false);
			uLong lc_dest_size = (uLong)ret.size();
			int lc_err_code = ::uncompress((unsigned char*)ret.data(), &lc_dest_size,
				(const unsigned char*)_compressData.data(), (uLong)_compressData.size());
			if (lc_err_code != (int)Z_OK)
			{
				sysLogError("TxZliblib::uncompressData()->{::uncompress()}调用失败，错误码【{0}】。"
				).args(lc_err_code);
				ret.clear();
			}
			if (lc_dest_size != _rawDataSize)
			{
				sysLogError("TxZliblib::uncompressData()：lc_dest_size不等于_rawDataSize。"
				).args(lc_err_code);
				ret.clear();
			}
			return ret;
		}
		static TxBlobString compressData(const TxBlobString &_rawData)
		{
			TxBlobString ret;
			ret.resize((int)::compressBound((uLong)_rawData.size()), false);
			uLong lc_dest_size = (uLong)ret.size();
			int lc_err_code = ::compress((unsigned char*)ret.data(), &lc_dest_size,
				(const unsigned char*)_rawData.data(), (uLong)_rawData.size());
			if (lc_err_code != (int)Z_OK)
			{
				sysLogError("TxZliblib::compressData()->{::compress()}调用失败，错误码【{0}】。"
				).args(lc_err_code);
				ret.clear();
			}
			else
			{
				ret.resize(lc_dest_size, true);
			}
			return ret;
		}
	};

	class TxZlibzip
	{
	private:
		TxBlobWString wsZipFileName;
		TxBlobString strPsw;
		zipFile zf;
		std::vector<unsigned char> vctMemBuffer;
	private:
		template<class T_FUNC>
		bool _addFile__(const TxBlobWString &_wNewFileName, const T_FUNC &_fn_read)
		{
			struct tag_mmm {
				const T_FUNC *p_func;
				static int g_read(void *_userdata, long long _pos, void *_buffer, int _size)
				{
					return (*((tag_mmm*)_userdata)->p_func)(_pos, _buffer, _size);
				}
			} mmm;
			mmm.p_func = &_fn_read;
			return this->_addItem__(_wNewFileName, 0, &tag_mmm::g_read, &mmm);
		}
		bool _addItem__(const TxBlobWString &_wNewFileName,
			int _bDir, int(*_read_fn)(void*, long long, void*, int), void *_fn_userdata)
		{
			int lc_err_code = 0;
			bool ret = true;
			if (this->zf == NULL || this->vctMemBuffer.size() <= 0)
				return ret;
			TxBlobString lcFileName = TxSyslib::sysCodecvt(_wNewFileName).trimLeft("\r\n\t \\/");
			lcFileName.replaceSelf('\\', '/');
			uLong lc_crc = ::crc32(0L, Z_NULL, 0);
			if (!_bDir)
			{
				long long lc_file_pos = 0;
				for (lc_file_pos = 0;;)
				{
					int lc_len = _read_fn(_fn_userdata, lc_file_pos,
						this->vctMemBuffer.data(), (int)this->vctMemBuffer.size());
					if (lc_len <= 0)
						break;
					lc_crc = ::crc32(lc_crc, (const Bytef*)this->vctMemBuffer.data(), lc_len);
					lc_file_pos = lc_file_pos + lc_len;
				}
				lc_err_code = ::zipOpenNewFileInZip3_64(this->zf, lcFileName.c_str(), NULL,
					NULL, 0, NULL, 0, NULL,
					Z_DEFLATED,
					Z_DEFAULT_COMPRESSION, 0,
					-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
					(this->strPsw.length() == 0 ? NULL : this->strPsw.c_str()),
					lc_crc, 1);
				if (lc_err_code != (int)ZIP_OK)
				{
					sysLogError("TxZlibzip::_addItem__()->{::zipOpenNewFileInZip3_64()}调用失败，错误码【{0}】，文件路径：【{1}】。"
					).args(lc_err_code, this->wsZipFileName);
					ret = false;
				}
				else
				{
					for (lc_file_pos = 0;;)
					{
						int lc_len = _read_fn(_fn_userdata, lc_file_pos, this->vctMemBuffer.data(), (int)this->vctMemBuffer.size());
						lc_err_code = ::zipWriteInFileInZip(this->zf, this->vctMemBuffer.data(), lc_len);
						if (lc_err_code != (int)ZIP_OK)
						{
							sysLogError("TxZlibzip::_addItem__()->{::zipWriteInFileInZip()}调用失败，错误码【{0}】，文件路径：【{1}】。"
							).args(lc_err_code, this->wsZipFileName);
							ret = false;
							break;
						}
						if (lc_len <= 0)
							break;
						lc_file_pos = lc_file_pos + lc_len;
					}
					lc_err_code = ::zipCloseFileInZip(this->zf);
					if (lc_err_code != (int)ZIP_OK)
					{
						sysLogError("TxZlibzip::_addItem__()->{::zipCloseFileInZip()}调用失败，错误码【{0}】，文件路径：【{1}】。"
						).args(lc_err_code, this->wsZipFileName);
						ret = false;
					}
				}
			}
			else
			{
				zip_fileinfo m_zip_fileinfo;
				::memset(&m_zip_fileinfo, 0, sizeof(m_zip_fileinfo));
				m_zip_fileinfo.external_fa = FILE_ATTRIBUTE_DIRECTORY;
				lc_err_code = ::zipOpenNewFileInZip3_64(this->zf, lcFileName.c_str(), &m_zip_fileinfo,
					NULL, 0, NULL, 0, NULL,
					Z_DEFLATED,
					Z_DEFAULT_COMPRESSION, 0,
					-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
					(this->strPsw.length() == 0 ? NULL : this->strPsw.c_str()),
					lc_crc, 0);
				if (lc_err_code != (int)ZIP_OK)
				{
					sysLogError("TxZlibzip::_addItem__()->{::zipOpenNewFileInZip3_64()}调用失败，错误码【{0}】，文件路径：【{1}】。"
					).args(lc_err_code, this->wsZipFileName);
					ret = false;
				}
				else
				{
					lc_err_code = ::zipCloseFileInZip(this->zf);
					if (lc_err_code != (int)ZIP_OK)
					{
						sysLogError("TxZlibzip::_addItem__()->{::zipCloseFileInZip()}调用失败，错误码【{0}】，文件路径：【{1}】。"
						).args(lc_err_code, this->wsZipFileName);
						ret = false;
					}
				}
			}
			return ret;
		}
	public:
		TxZlibzip()
		{
			this->zf = NULL;
		}
		virtual ~TxZlibzip()
		{
			this->close();
		}
		bool open(const TxBlobWString &_zipFileName,
			const TxBlobString &_strPsw = TxBlobString(), int _appendSign = APPEND_STATUS_CREATE)
		{
			this->close();
			this->strPsw = _strPsw.trimLR("\r\n\t ");
			this->wsZipFileName = _zipFileName;
			this->zf = ::zipOpen64(TxSyslib::sysCodecvt(this->wsZipFileName).c_str(), _appendSign);
			if (this->zf == NULL)
			{
				sysLogWarn("打开zip文件失败。文件路径：【{0}】").args(_zipFileName);
				return false;
			}
			enum { E_BUFFER_SIZE = 512 * 1024 };
			if (this->vctMemBuffer.size() < E_BUFFER_SIZE)
				this->vctMemBuffer.resize(E_BUFFER_SIZE);
			return true;
		}
		void close()
		{
			if (this->zf != NULL)
			{
				int lc_status = ::zipClose(this->zf, NULL);
				if (lc_status != (int)ZIP_OK)
				{
					sysLogError("TxZlibzip::close()->{::zipClose}调用失败，错误码【{0}】，文件路径：【{1}】。"
					).args(lc_status, this->wsZipFileName);
				}
				this->zf = NULL;
			}
			this->vctMemBuffer.clear();
		}
		bool addFileStream(const TxBlobWString &_wNewFileName, const TxBlobWString &_dataFile)
		{
			TxSysFile lc_sysFile;
			if (!lc_sysFile.open(_dataFile, "rb"))
			{
				sysLogError("if (!lc_sysFile.open(_dataFile, \"rb\"))打开文件失败。压缩包zip文件路径：【{0}】").args(this->wsZipFileName);
				return false;
			}
			long long ll_file_pos = 0;
			return this->_addFile__(_wNewFileName, [&lc_sysFile, &ll_file_pos](long long _pos, void *_rd_data, int _size)->int {
				if (_pos != ll_file_pos)
				{
					lc_sysFile.seek(_pos, SEEK_SET);
					ll_file_pos = _pos;
				}
				_size = TxStdlib::maximum(0, lc_sysFile.read(_rd_data, _size));
				ll_file_pos = ll_file_pos + (long long)_size;
				return _size;
			});
		}
		bool addFileStream(const TxBlobWString &_wNewFileName, const std::shared_ptr<TxSysFileSlice> &_spSysFileSlice)
		{
			return this->_addFile__(_wNewFileName, [&_spSysFileSlice](long long _pos, void *_rd_data, int _size)->int {
				return TxStdlib::maximum(0, _spSysFileSlice->read(_rd_data, _size, _pos));
			});
		}
		bool addFileStream(const TxBlobWString &_wNewFileName, const std::function<int(long long, void*, int)> &_fnRead)
		{
			return this->_addFile__(_wNewFileName, _fnRead);
		}
		bool addFileData(const TxBlobWString &_wNewFileName, const void *_buff, long long _buff_size)
		{
			if (_buff == NULL || _buff_size < 0)
			{
				sysLogError("TxZlibzip::addFile()参数错误。压缩包zip文件路径：【{0}】").args(this->wsZipFileName);
				return false;
			}
			return this->_addFile__(_wNewFileName, [_buff, _buff_size](long long _pos, void *_rd_data, int _size)->int {
				if (_pos >= _buff_size)
					return 0;
				if ((long long)_size > _buff_size - _pos)
					_size = (int)(_buff_size - _pos);
				std::memcpy(_rd_data, (const char*)_buff + _pos, _size);
				return _size;
			});
		}
		bool addFileData(const TxBlobWString &_wNewFileName, const std::string &_data)
		{
			return this->addFileData(_wNewFileName, _data.data(), _data.size());
		}
		bool addFileData(const TxBlobWString &_wNewFileName, const TxBlobString &_data)
		{
			return this->addFileData(_wNewFileName, _data.data(), _data.size());
		}
		bool addDirectory(const TxBlobWString &_wNewFileName)
		{
			return this->_addItem__(_wNewFileName, 1, NULL, 0);
		}
	};
}

typedef _TxPrivateNs_::TxZliblib TxZliblib;
typedef _TxPrivateNs_::TxZlibzip TxZlibzip;

