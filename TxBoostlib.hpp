#ifndef _PUBLIC_TX_BOOST_LIB_
#define _PUBLIC_TX_BOOST_LIB_


#include "TxStdlib.hpp"

#include <assert.h>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <exception>

#include <boost/crc.hpp>
#include <boost/random.hpp>
#include <boost/locale.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>


//--------------------------------------TxBoostlib--------------------------------------// //Œ¥≤‚ ‘
class TxBoostlib
{
public:
	static bool deleteAllFile(const TxBlobWString &_w_file_or_dir)
	{
		TxBlobWString lc_w_file_or_dir = completePath(_w_file_or_dir.trimLR(L"\r\n\t \\/"));
		if (lc_w_file_or_dir.length() <= 0)
		{
			assert(false);
			return false;
		}
		boost::system::error_code m_err_code;
		boost::filesystem::wpath lcPath(lc_w_file_or_dir.c_str());
		if (!boost::filesystem::exists(lcPath, m_err_code))
			return true;
		boost::filesystem::wpath lcParentPath = lcPath.parent_path();
		if (lcParentPath.empty() || lcPath.generic_wstring() == lcParentPath.generic_wstring())
		{
			assert(false);
			return false;
		}
		boost::filesystem::remove_all(boost::filesystem::wpath(lc_w_file_or_dir.c_str()), m_err_code);
		if (!boost::filesystem::exists(lcPath, m_err_code))
			return true;
		return false;
	}
	static bool createDirectories(const TxBlobWString &_wdir)
	{
		if (_wdir.length() <= 0)
			return false;
		boost::system::error_code m_err_code;
		boost::filesystem::wpath mLcPath(_wdir.c_str());
		if (boost::filesystem::is_directory(mLcPath, m_err_code))
			return true;
		boost::filesystem::create_directories(mLcPath, m_err_code);
		if (boost::filesystem::is_directory(mLcPath, m_err_code))
			return true;
		return false;
	}
	static bool isDirectory(const TxBlobWString &_wdir)
	{
		boost::system::error_code m_err_code;
		boost::filesystem::wpath mLcPath(_wdir.c_str());
		if (boost::filesystem::is_directory(mLcPath, m_err_code))
			return !m_err_code;
		return false;
	}
	static bool isExistFile(const TxBlobWString &_wdir)
	{
		boost::system::error_code m_err_code;
		boost::filesystem::wpath mLcPath(_wdir.c_str());
		if (boost::filesystem::is_regular_file(mLcPath, m_err_code))
			return !m_err_code;
		return false;
	}
	static bool isExistPath(const TxBlobWString &_wdir)
	{
		boost::system::error_code m_err_code;
		boost::filesystem::wpath mLcPath(_wdir.c_str());
		if (boost::filesystem::exists(mLcPath, m_err_code))
			return !m_err_code;
		return false;
	}
	static bool makeSureClearDirectories(const TxBlobWString &_path)
	{
		typedef struct {
			static bool _as_check_error_(const boost::system::error_code &_error_code)
			{
				std::string lc_msg = _error_code.message();
				TX_UNUSED(lc_msg);
				return !_error_code;
			}
			static bool _clear_dir__(const boost::filesystem::wpath &_path)
			{
				boost::filesystem::directory_iterator path_begin(_path);
				boost::filesystem::directory_iterator path_end;
				bool ret_true = true;
				for (; path_begin != path_end; ++path_begin)
				{
					boost::system::error_code lc_error_code;
					if (boost::filesystem::is_directory(*path_begin))
					{
						if (!_clear_dir__(*path_begin))
							ret_true = false;
						boost::filesystem::remove_all(*path_begin, lc_error_code);
					}
					else
					{
						boost::filesystem::remove_all(*path_begin, lc_error_code);
					}
					assert(_as_check_error_(lc_error_code));
					if (lc_error_code)
						ret_true = false;
				}
				return ret_true;
			}
		}TTT;
		boost::filesystem::wpath lc_path(TxBlobWString(_path).trimSelfRight(L"\\/").replaceSelf(L'\\', L'/').toStdString());
		if (boost::filesystem::is_directory(lc_path))
			return TTT::_clear_dir__(lc_path);
		boost::system::error_code lc_error_code;
		boost::filesystem::remove_all(lc_path, lc_error_code);
		lc_error_code.clear();
		boost::filesystem::create_directories(lc_path, lc_error_code);
		assert(TTT::_as_check_error_(lc_error_code));
		return !lc_error_code;
	}
	static bool moveFile(const TxBlobWString &_newFile, const TxBlobWString &_oldFile)
	{
		boost::system::error_code m_err_code;
		boost::filesystem::wpath lc_newPath(_newFile.c_str());
		boost::filesystem::wpath lc_oldPath(_oldFile.c_str());
		boost::filesystem::rename(lc_oldPath, lc_newPath, m_err_code);
		if (!m_err_code)
			return true;
		if (!lc_newPath.has_parent_path())
			return false;
		if (!boost::filesystem::exists(lc_newPath))
			boost::filesystem::create_directories(lc_newPath.parent_path(), m_err_code);
		else boost::filesystem::remove_all(lc_newPath, m_err_code);
		boost::filesystem::rename(lc_oldPath, lc_newPath, m_err_code);
		return !m_err_code;
	}
	//static bool copyPath(const TxBlobWString &_dstFile, const TxBlobWString &_srcFile)
	//{
	//	bool bLcDir = false;
	//	bool bLcFile = false;
	//	if (isDirectory(_srcFile))
	//		bLcDir = true;
	//	else if (isExistFile(_srcFile))
	//		bLcFile = true;
	//	else
	//	{
	//		assert(0);
	//		return false;
	//	}

	//	boost::system::error_code lc_err_code;
	//	boost::filesystem::wpath lc_newPath(_dstFile.c_str());
	//	boost::filesystem::wpath lc_oldPath(_srcFile.c_str());
	//	if (bLcDir)
	//		boost::filesystem::copy_directory(lc_oldPath, lc_newPath, lc_err_code);
	//	else if (bLcFile)
	//		boost::filesystem::copy_file(lc_oldPath, lc_newPath, lc_err_code);
	//	if (!lc_err_code)
	//		return true;
	//	if (!lc_newPath.has_parent_path())
	//		return false;
	//	if (!boost::filesystem::exists(lc_newPath, lc_err_code))
	//		boost::filesystem::create_directories(lc_newPath.parent_path(), lc_err_code);
	//	else boost::filesystem::remove_all(lc_newPath, lc_err_code);
	//	if (bLcDir)
	//		boost::filesystem::copy_directory(lc_oldPath, lc_newPath, lc_err_code);
	//	else if (bLcFile)
	//		boost::filesystem::copy_file(lc_oldPath, lc_newPath, lc_err_code);
	//	return !lc_err_code;
	//}
	static TxBlobWString completePath(const TxBlobWString &_path)
	{
		TxBlobWString ret = _path;
		ret.replaceSelf(L'\\', L'/');
		try
		{
			bool lc_b_have_path_tail = false;
			if (ret.backChar() == L'\\')
			{
				lc_b_have_path_tail = true;
			}
			else
			{
				lc_b_have_path_tail = false;
				ret.append(L"\\");
			}

			boost::filesystem::wpath lc_path(ret.toStdString());
			ret = boost::filesystem::system_complete(lc_path).generic_wstring();
			ret.replaceSelf(L'\\', L'/');

			if (lc_b_have_path_tail)
			{
				if (ret.backChar() != L'\\')
					ret.append(L"\\");
			}
			else
			{
				while (ret.size() > 0 && ret.backChar() == L'\\')
					ret.pop_back(1);
			}
		}
		catch (...) {
			ret.clear();
		}
		return ret;
	}
	static long long fileSize(const TxBlobWString &_file)
	{
		boost::system::error_code m_err_code;
		boost::filesystem::wpath mLcPath(_file.c_str());
		long long ret = boost::filesystem::file_size(mLcPath, m_err_code);
		return !m_err_code ? ret : -1;
	}
	static bool resizeFile(const TxBlobWString &_file, long long _llFileSize)
	{
		boost::system::error_code m_err_code;
		boost::filesystem::wpath mLcPath(_file.c_str());
		boost::filesystem::resize_file(mLcPath, _llFileSize, m_err_code);
		return !m_err_code;
	}
	static TxBlobWString fileDirectory(const TxBlobWString &_file)
	{
		assert(_file.length() > 0 && _file.backChar() != L'\\'&&_file.backChar() != L'/');
		TxBlobWString ret;
		try {
			ret = boost::filesystem::wpath(std::wstring(_file.c_str())).parent_path().generic_wstring();
			if (ret.length() > 0)
			{
				ret.replaceSelf(L'\\', L'/');
				if (ret.backChar() != L'\\')
					ret.append(L"\\");
			}
		}
		catch (...) {}
		return ret;
	}
	static TxBlobWString pathExtension(const TxBlobWString &_path)
	{
		TxBlobWString ret;
		try {
			ret = boost::filesystem::wpath(std::wstring(_path.c_str())).extension().generic_wstring();
		}
		catch (...) {}
		return ret.trimLR(L"\r\n\t .");
	}
	static bool readFile(TxBlobString *_buffer, const TxBlobWString &_filename,
		unsigned long long _ullMaxSize = 16 * 1024 * 1024)
	{
		boost::filesystem::fstream lc_fs;
		bool ret = false;
		lc_fs.open(boost::filesystem::wpath(_filename.toStdString()), std::ios_base::in | std::ios_base::binary | std::ios_base::ate);
		if (!lc_fs.is_open())
			ret = false;
		else
		{
			long long lc_size = lc_fs.tellg();
			if (lc_size >= (long long)_ullMaxSize)
				ret = false;
			else
			{
				_buffer->resize((int)lc_size, false);
				lc_fs.seekg(0);
				lc_fs.read(_buffer->data(), _buffer->size());
				ret = true;
			}
		}
		lc_fs.close();
		if (!ret)
			_buffer->clear();
		return true;
	}
	static TxBlobString readFile(const TxBlobWString &_filename, unsigned long long _ullMaxSize = 16 * 1024 * 1024)
	{
		TxBlobString ret;
		readFile(&ret, _filename, _ullMaxSize);
		return ret;
	}
	static bool writeFile(const TxBlobWString &_filename, const TxBlobString &_buffer)
	{
		boost::filesystem::fstream lc_fs;
		auto lc_open_stream_fn = [&lc_fs, &_filename]()->bool {
			lc_fs.open(boost::filesystem::wpath(_filename.toStdString()), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
			return lc_fs.is_open();
		};
		bool ret = false, b_lc_open = false;
		if (lc_open_stream_fn())
			b_lc_open = true;
		else
		{
			lc_fs.close();
			deleteAllFile(_filename);
			if (lc_open_stream_fn())
				b_lc_open = true;
		}
		if (!b_lc_open)
			ret = false;
		else
		{
			lc_fs.write(_buffer.c_str(), _buffer.size());
			ret = true;
		}
		lc_fs.close();
		return true;
	}

	static std::uint64_t randomUint64()
	{
		class CRandom
		{
		private:
			boost::mutex mMutex;
			boost::mt19937 rng;
			boost::uniform_int<> ui;
			std::uint64_t uiIndexNum;
		private:
			std::uint64_t _randomValue_NoMutex_()
			{
				std::uint64_t ret = ((((std::uint64_t)(std::uint16_t)this->ui(rng)) << 0)
					| (((std::uint64_t)(std::uint16_t)this->ui(rng)) << 16)
					| (((std::uint64_t)(std::uint16_t)this->ui(rng)) << 32)
					| (((std::uint64_t)(std::uint16_t)this->ui(rng)) << 48))
					^ (this->uiIndexNum += 1000003) ^ (std::uint64_t)::time(NULL);
				return ret;
			}
		public:
			CRandom() :rng((std::uint32_t)::time(NULL) + (std::uint32_t)(std::size_t)&randomUint64 + (std::uint32_t)(std::size_t)&rng),
				ui(0, (unsigned short)-1), uiIndexNum((std::uint64_t)(size_t)&ui + (std::uint64_t)::time(NULL))
			{
				this->mMutex.lock();
				for (int i = (int)((this->uiIndexNum + this->_randomValue_NoMutex_()) % 11); i < 16 + 12; i++)
					this->_randomValue_NoMutex_();
				this->mMutex.unlock();
			}
			~CRandom()
			{
			}
			std::uint64_t randomValue()
			{
				this->mMutex.lock();
				std::uint64_t ret = this->_randomValue_NoMutex_();
				this->mMutex.unlock();
				return ret;
			}
		};
		static CRandom m_Random;
		return m_Random.randomValue();
	}
	static void initEnv()
	{
		TxStdlib::executeOnce([]() {
			TxStdlib::initEnv();
			randomUint64();
		});
	}
};


//--------------------------------------TxBoostUniqueSharedLock--------------------------------------// //Œ¥≤‚ ‘
class TxBoostUniqueSharedLock : private TxForbidDirectCopyObject
{
private:
	boost::shared_mutex rwmutex;
public:
	TxBoostUniqueSharedLock() { }
	virtual ~TxBoostUniqueSharedLock() { }
	template<class TFUNC>
	void sharedLock(const TFUNC& _mFunc)
	{
		boost::shared_lock<boost::shared_mutex>  lc_rdlock(rwmutex);
		TX_UNUSED(lc_rdlock);
		_mFunc();
	}
	template<class TFUNC>
	void uniqueLock(const TFUNC& _mFunc)
	{
		boost::unique_lock<boost::shared_mutex>  lc_wtlock(rwmutex);
		TX_UNUSED(lc_wtlock);
		_mFunc();
	}
};


//--------------------------------------TxBoostFileStream--------------------------------------// //Œ¥≤‚ ‘
class TxBoostFileStream
{
private:
	boost::filesystem::fstream m_fs;
	std::ios_base::openmode m_openmode;
public:
	TxBoostFileStream()
	{
		this->m_openmode = std::ios_base::openmode(0);
	}
	~TxBoostFileStream()
	{
		this->close();
	}

	//read//std::ios_base::in | std::ios_base::binary | std::ios_base::ate
	//write//std::ios_base::out | std::ios_base::binary | std::ios_base::trunc
	bool open(const TxBlobWString &_filename,
		std::ios_base::openmode _mode = std::ios_base::binary | std::ios_base::in | std::ios_base::out)
	{
		bool ret = false;
		this->close();
		try {
			this->m_fs.open(boost::filesystem::wpath(_filename.toStdString()), _mode);
			ret = this->m_fs.is_open();
		}
		catch (...) {
			ret = false;
		}
		if (ret)
			this->m_openmode = _mode;
		return ret;
	}
	bool is_open() const
	{
		return this->m_fs.is_open();
	}
	void close()
	{
		try {
			this->m_fs.close();
		}
		catch (...) {
			assert(false);
		}
		this->m_openmode = std::ios_base::openmode(0);
	}
	long long tell()
	{
		if (this->m_openmode&std::ios_base::out)
			return (long long)this->m_fs.tellp();
		else if (this->m_openmode&std::ios_base::in)
			return (long long)this->m_fs.tellg();
		assert(false);
		return 0;
	}
	bool seek(std::ios_base::seekdir _way, long long _position)
	{
		bool ret = false;
		try {
			if (this->m_openmode&std::ios_base::out)
			{
				this->m_fs.seekp(_position, _way);
				ret = true;
			}
			else if (this->m_openmode&std::ios_base::in)
			{
				this->m_fs.seekg(_position, _way);
				ret = true;
			}
			else
			{
				assert(false);
				ret = false;
			}
		}
		catch (...) {
			assert(false);
			ret = false;
		}
		return ret;
	}
	bool flush()
	{
		bool ret = false;
		try {
			this->m_fs.flush();
			ret = true;
		}
		catch (...) {
			assert(false);
			ret = false;
		}
		return ret;
	}
	int read(void *_buffer, int _size)
	{
		int ret = 0;
		try {
			this->m_fs.read((char*)_buffer, _size);
			ret = (int)this->m_fs.gcount();
		}
		catch (...) {
			assert(false);
			ret = 0;
		}
		return ret;
	}
	bool write(const void *_buffer, int _size)
	{
		bool ret = false;
		try {
			this->m_fs.write((const char*)_buffer, _size);
			ret = true;
		}
		catch (...) {
			assert(false);
			ret = false;
		}
		return ret;
	}
	bool eof() const
	{
		return this->m_fs.eof();
	}
};


//--------------------------------------TxBoostJsonPacket--------------------------------------//
class TxBoostJsonPacket
{
private:
	TxBlobString mBuffer;
public:
	TxBoostJsonPacket()
	{
	}
	~TxBoostJsonPacket()
	{
	}
	void push(const void *_data, int _size)
	{
		if (_data != NULL&&_size > 0)
			this->mBuffer.append((const char*)_data, _size);
	}
	template<class T_DATA>
	void push(const T_DATA &_data)
	{
		this->push(_data.data(), _data.size());
	}
	bool pop(boost::property_tree::ptree *_out)
	{
		const char *lc_bgn_ptr = (const char*)this->mBuffer.data();
		const char *lc_end_ptr = (const char*)this->mBuffer.data() + this->mBuffer.size();
		_out->clear();
		for (;;)
		{
			while (lc_bgn_ptr != lc_end_ptr&&lc_bgn_ptr[0] == 0)
				++lc_bgn_ptr;
			if (lc_bgn_ptr == lc_end_ptr || _out == NULL)
				break;
			else
			{
				const char *lc_find_ptr = (const char*)std::memchr(lc_bgn_ptr, 0, (int)(lc_end_ptr - lc_bgn_ptr));
				if (lc_find_ptr == NULL)
					break;
				else
				{
					std::istringstream lc_data_stream(std::string(lc_bgn_ptr, lc_find_ptr));
					try {
						boost::property_tree::read_json(lc_data_stream, *_out);
						_out = NULL;
					}
					catch (...) {
						assert(0);
					}
					lc_bgn_ptr = lc_find_ptr + 1;
				}
			}
		}
		if (lc_bgn_ptr != this->mBuffer.data())
		{
			if (lc_bgn_ptr == lc_end_ptr) this->mBuffer.clear();
			else this->mBuffer.assign(lc_bgn_ptr, (int)(lc_end_ptr - lc_bgn_ptr));
		}
		if (_out != NULL)
			_out->clear();
		return _out == NULL;
	}
	static TxBlobString serializer(const boost::property_tree::ptree &_data_pt)
	{
		try {
			std::ostringstream ret;
			boost::property_tree::write_json(ret, _data_pt, false);
			return TxBlobString("\0", 1) + ret.str() + TxBlobString("\0", 1);
		}
		catch (...) {
		}
		return TxBlobString();
	}
};


//--------------------------------------TxBoostCrc32--------------------------------------//
class TxBoostCrc32
{
private:
	boost::crc_32_type m_crc32;
private:
	static void _static_process_hexstring_(std::string *_out, const unsigned char *_buffer, int _size)
	{
		TxBoostCrc32 lc_crc32;
		lc_crc32.process(_buffer, _size);
		*_out = lc_crc32.result_hexstring();
	}
	static void _static_process_hexstring_(std::string *_out, const char *_buffer, int _size)
	{
		_static_process_hexstring_(_out, (const unsigned char*)_buffer, _size);
	}
public:
	void process(const void *_buffer, int _size)
	{
		return this->m_crc32.process_bytes(_buffer, _size);
	}
	template<class T_DATA>
	void process(const T_DATA &_data)
	{
		return this->process(_data.data(), _data.size());
	}
	std::string result_hexstring() const
	{
		char lc_res[24] = { 0 };
		unsigned  int lc_val = this->result_uint32();
		sprintf_s(lc_res, sizeof(lc_res) / sizeof(lc_res[0]) - 1, "%08X", lc_val);
		return std::string(lc_res);
	}
	boost::uint32_t result_uint32() const
	{
		return this->m_crc32.checksum();
	}
	static std::string static_process_hexstring(const void *_buffer, int _size)
	{
		std::string ret;
		_static_process_hexstring_(&ret, (const char*)_buffer, _size);
		return ret;
	}
	template<class T_DATA>
	static std::string static_process_hexstring(const T_DATA &_data)
	{
		std::string ret;
		_static_process_hexstring_(&ret, _data.data(), _data.size());
		return ret;
	}
};


//--------------------------------------TxBoostMarkup--------------------------------------//
template<bool B_MUTEX>
class TxBoostMarkup : private TxForbidDirectCopyObject
{
	friend class TxBoostMarkup<!B_MUTEX>;
private:
	typedef boost::mutex T_MUTEX_TYPE;
	boost::property_tree::ptree m_pt_tem_;
	boost::property_tree::ptree *pTree;
	char buff_mutex__tem__[B_MUTEX ? sizeof(T_MUTEX_TYPE) : sizeof(long long)];
private:
	inline void _constructor_()
	{
		this->pTree = &this->m_pt_tem_;
		if (B_MUTEX)
			new ((T_MUTEX_TYPE*)&this->buff_mutex__tem__[0]) T_MUTEX_TYPE();
	}
	inline void _destructor_()
	{
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
	bool _loadMarkupTextFromData_(const void *_data, int _size, int _i_json1_xml2_ini3, bool _b_assert = false)
	{
		bool ret = false;
		if (_size < 2)
			return ret;
		this->_lock_obj();
		try {
			this->pTree->clear();
			std::istringstream lc_data_stream(std::string((const char*)_data, (std::string::size_type)_size));
			switch (_i_json1_xml2_ini3)
			{
			case 1:
				boost::property_tree::read_json(lc_data_stream, *this->pTree);
				break;
			case 2:
				boost::property_tree::read_xml(lc_data_stream, *this->pTree,
					boost::property_tree::xml_parser::trim_whitespace
					| boost::property_tree::xml_parser::no_comments);
				break;
			case 3:
				boost::property_tree::read_ini(lc_data_stream, *this->pTree);
				break;
			default:
				assert(0);
				break;
			}
			ret = true;
		}
		catch (std::exception &_expt) {
#ifndef NDEBUG
			std::string jklsdf = _expt.what();
#endif
			assert(!_b_assert);
			TX_UNUSED(_b_assert);
			TX_UNUSED(_expt);
			this->pTree->clear();
			ret = false;
		}
		this->_unlock_obj();
		return ret;
	}
	TxBlobString _saveMarkupTextToString_(int _i_json1_xml2_ini3, bool _pretty) const
	{
		std::ostringstream lc_file_data;
		this->_lock_obj();
		try {
			switch (_i_json1_xml2_ini3)
			{
			case 1:
				boost::property_tree::write_json(lc_file_data, *this->pTree, _pretty);
				break;
			case 2:
				if (_pretty)
				{
					boost::property_tree::write_xml(lc_file_data, *this->pTree,
						boost::property_tree::xml_writer_settings<std::string>('\t', 1));
				}
				else
				{
					boost::property_tree::write_xml(lc_file_data, *this->pTree);
				}
				break;
			case 3:
				boost::property_tree::write_ini(lc_file_data, *this->pTree);
				break;
			default:
				assert(0);
				break;
			}
		}
		catch (...) {
			assert(0);
		}
		this->_unlock_obj();
		TxBlobString ret(lc_file_data.str());
		while (ret.length() > 0 && (ret.backChar() == '\r' || ret.backChar() == '\n') && !_pretty)
			ret.pop_back(1);
		return ret;
	}
public:
	TxBoostMarkup()
	{
		this->_constructor_();
	}
	TxBoostMarkup(const boost::property_tree::ptree &_pt)
	{
		this->_constructor_();
		*this->pTree = _pt;
	}
	TxBoostMarkup(const TxBoostMarkup<true> &_tem)
	{
		this->_constructor_();
		this->copy(_tem);
	}
	TxBoostMarkup(const TxBoostMarkup<false> &_tem)
	{
		this->_constructor_();
		this->copy(_tem);
	}
	TxBoostMarkup& attach(boost::property_tree::ptree *_pt)
	{
		if (_pt != NULL) this->pTree = _pt;
		else this->pTree = &this->m_pt_tem_;
		return *this;
	}
	TxBlobWString dataStringW() const
	{
		return this->dataString().utf8Codecvt();
	}
	TxBlobString dataString() const
	{
		this->_lock_obj();
		TxBlobString ret = this->pTree->data();
		this->_unlock_obj();
		return ret;
	}
	void dettach()
	{
		this->attach(NULL);
	}
	~TxBoostMarkup()
	{
		this->_destructor_();
	}
	TxBoostMarkup& operator=(const TxBoostMarkup<true> &_tem)
	{
		this->copy(_tem);
		return *this;
	}
	TxBoostMarkup& operator=(const TxBoostMarkup<false> &_tem)
	{
		this->copy(_tem);
		return *this;
	}
	inline boost::property_tree::ptree* getBoostTree() const
	{
		assert(!B_MUTEX);
		return const_cast<TxBoostMarkup*>(this)->pTree;
	}
	void copy(const TxBoostMarkup<true> &_obj)
	{
		this->_lock_obj();
		*this->pTree = *_obj.pTree;
		this->_unlock_obj();
	}
	void copy(const TxBoostMarkup<false> &_obj)
	{
		this->_lock_obj();
		*this->pTree = *_obj.pTree;
		this->_unlock_obj();
	}

	bool parseJsonFile(const TxBlobWString &_filename, bool _b_assert = false)
	{
		return this->parseJsonString(TxBoostlib::readFile(_filename), _b_assert);
	}
	bool parseJsonString(const TxBlobString &_utf8_data, bool _b_assert = false)
	{
		return this->parseJsonString(_utf8_data.c_str(), _utf8_data.length(), _b_assert);
	}
	bool parseJsonString(const void *_utf8_data, int _utf8_len, bool _b_assert = false)
	{
		return this->_loadMarkupTextFromData_(_utf8_data, _utf8_len, 1, _b_assert);
	}
	TxBlobString saveJsonString(bool _pretty = false) const
	{
		return this->_saveMarkupTextToString_(1, _pretty);
	}
	bool saveJsonFile(const TxBlobWString &_filename, bool _pretty = false) const
	{
		return TxBoostlib::writeFile(_filename, this->saveJsonString(_pretty));
	}

	bool parseXmlFile(const TxBlobWString &_filename, bool _b_assert = false)
	{
		return this->parseXmlString(TxBoostlib::readFile(_filename), _b_assert);
	}
	bool parseXmlString(const TxBlobString &_utf8_data, bool _b_assert = false)
	{
		return this->parseXmlString(_utf8_data.c_str(), _utf8_data.length(), _b_assert);
	}
	bool parseXmlString(const void *_utf8_data, int _utf8_len, bool _b_assert = false)
	{
		return this->_loadMarkupTextFromData_(_utf8_data, _utf8_len, 2, _b_assert);
	}
	TxBlobString saveXmlString(bool _pretty = true) const
	{
		return this->_saveMarkupTextToString_(2, _pretty);
	}
	bool saveXmlFile(const TxBlobWString &_filename, bool _pretty = true) const
	{
		return TxBoostlib::writeFile(_filename, this->saveXmlString(_pretty));
	}

	bool parseIniFile(const TxBlobWString &_filename, bool _b_assert = false)
	{
		return this->parseIniString(TxBoostlib::readFile(_filename), _b_assert);
	}
	bool parseIniString(const TxBlobString &_utf8_data, bool _b_assert = false)
	{
		return this->parseIniString(_utf8_data.c_str(), _utf8_data.length(), _b_assert);
	}
	bool parseIniString(const void *_utf8_data, int _utf8_len, bool _b_assert = false)
	{
		return this->_loadMarkupTextFromData_(_utf8_data, _utf8_len, 3, _b_assert);
	}
	TxBlobString saveIniString() const
	{
		return this->_saveMarkupTextToString_(3, false);
	}
	bool saveIniFile(const TxBlobWString &_filename, bool _pretty = true) const
	{
		return TxBoostlib::writeFile(_filename, this->saveIniString(_pretty));
	}
public:
	template<class NumType>
	bool putValue(const TxBlobString &_path, const NumType &_value)
	{
		bool ret = false;
		this->_lock_obj();
		try {
			if (std::is_integral<NumType>::value || std::is_enum<NumType>::value)//integer
			{
				if (sizeof(_value) <= sizeof(int) || sizeof(_value) <= sizeof(unsigned int))
				{
					if (std::is_signed<NumType>::value)//Signed
						this->pTree->put<int>(std::string(_path.c_str(), _path.size()), (int)_value);
					else//Unsigned
						this->pTree->put<unsigned int>(std::string(_path.c_str(), _path.size()), (unsigned int)_value);
				}
				else
				{
					this->pTree->put<NumType>(std::string(_path.c_str(), _path.size()), _value);
				}
				ret = true;
			}
			else if (std::is_floating_point<NumType>::value)
			{
				this->pTree->put<NumType>(std::string(_path.c_str(), _path.size()), _value);
				ret = true;
			}
			else assert(false);
		}
		catch (...) {
			ret = false;
		}
		assert(ret);
		this->_unlock_obj();
		return ret;
	}
	bool _putValue_(const TxBlobString &_path, const std::string &_value)
	{
		bool ret = false;
		this->_lock_obj();
		try
		{
			this->pTree->put(std::string(_path.c_str(), _path.length()), _value);
			ret = true;
		}
		catch (...) {
			ret = false;
		}
		assert(ret);
		this->_unlock_obj();
		return ret;
	}
	bool putValue(const TxBlobString &_path, const std::string &_value)
	{
		return this->_putValue_(_path, _value);
	}
	bool putValue(const TxBlobString &_path, const std::wstring &_value)
	{
		return this->_putValue_(_path, TxBlobWString(_value).utf8Codecvt());
	}
	bool putValue(const TxBlobString &_path, const char *_value)
	{
		return this->_putValue_(_path, std::string(_value));
	}
	bool putValue(const TxBlobString &_path, const wchar_t *_value)
	{
		return this->_putValue_(_path, TxBlobWString(_value).utf8Codecvt());
	}
	bool putValue(const TxBlobString &_path, wchar_t *_value)
	{
		return this->_putValue_(_path, TxBlobWString(_value).utf8Codecvt());
	}
	bool putValue(const TxBlobString &_path, char *_value)
	{
		return this->_putValue_(_path, _value);
	}
	bool putValue(const TxBlobString &_path, std::wstring &_value)
	{
		return this->_putValue_(_path, TxBlobWString(_value).utf8Codecvt());
	}
	bool putValue(const TxBlobString &_path, std::string &_value)
	{
		return this->_putValue_(_path, _value);
	}
	bool putValue(const TxBlobString &_path, const TxBlobWString &_value)
	{
		return this->_putValue_(_path, _value.utf8Codecvt());
	}
	bool putValue(const TxBlobString &_path, const TxBlobString &_value)
	{
		return this->_putValue_(_path, _value);
	}
	bool putValue(const TxBlobString &_path, TxBlobWString &_value)
	{
		return this->_putValue_(_path, _value.utf8Codecvt());
	}
	bool putValue(const TxBlobString &_path, TxBlobString &_value)
	{
		return this->_putValue_(_path, _value);
	}
public:
	template<class NumType>
	bool _getTransValue_(NumType *_value, const std::string &_path, const TxEmptyObjectTmpl<0>&) const
	{
		bool ret = true;
		if (std::is_signed<NumType>::value || std::is_enum<NumType>::value)
		{//Signed
			if (sizeof(_value) <= sizeof(int))
				*_value = (NumType)this->pTree->get<int>(_path);
			else if (sizeof(_value) <= sizeof(long long))
				*_value = (NumType)this->pTree->get<long long>(_path);
			else if (sizeof(_value) <= sizeof(long long int))
				*_value = (NumType)this->pTree->get<long long int>(_path);
			else ret = false;
		}
		else
		{//Unsigned
			if (sizeof(_value) <= sizeof(unsigned int))
				*_value = (NumType)this->pTree->get<unsigned int>(_path);
			else if (sizeof(_value) <= sizeof(unsigned long long))
				*_value = (NumType)this->pTree->get<unsigned long long>(_path);
			else if (sizeof(_value) <= sizeof(unsigned long long int))
				*_value = (NumType)this->pTree->get<unsigned long long int>(_path);
			else ret = false;
		}
		assert(ret);
		return ret;
	}
	template<class NumType>
	bool _getTransValue_(NumType *_value, const std::string &_path, const TxEmptyObjectTmpl<1>&) const
	{
		bool ret = true;
		if (sizeof(NumType) <= sizeof(float))
			*_value = (NumType)this->pTree->get<float>(_path);
		else if (sizeof(NumType) <= sizeof(double))
			*_value = (NumType)this->pTree->get<double>(_path);
		else if (sizeof(NumType) <= sizeof(long double))
			*_value = (NumType)this->pTree->get<long double>(_path);
		else ret = false;
		assert(ret);
		return ret;
	}
	template<class NumType>
	bool getValue(NumType *_value, const TxBlobString &_path, bool _b_assert = false) const
	{
		bool ret = false;
		this->_lock_obj();
		try {
			ret = this->_getTransValue_(_value, std::string(_path.c_str(), _path.size()),
				TxEmptyObjectTmpl<!!(std::is_floating_point<NumType>::value)>());
		}
		catch (...) {
			ret = false;
		}
		assert(!_b_assert || ret);
		TX_UNUSED(_b_assert);
		this->_unlock_obj();
		return ret;
	}
	bool _getValue_(TxBlobString *_value, const TxBlobString &_path, bool _b_assert = false) const
	{
		bool ret = false;
		this->_lock_obj();
		try {
			*_value = this->pTree->get<std::string>(std::string(_path.c_str(), _path.size()));
			ret = true;
		}
		catch (...) {
			assert(!_b_assert);
			TX_UNUSED(_b_assert);
			ret = false;
		}
		this->_unlock_obj();
		return ret;
	}
	bool getValue(std::string *_value, const TxBlobString &_path, bool _b_assert = false) const
	{
		TxBlobString lc_val;
		if (this->_getValue_(&lc_val, _path, _b_assert))
		{
			*_value = lc_val;
			return true;
		}
		return false;
	}
	bool getValue(std::wstring *_value, const TxBlobString &_path, bool _b_assert = false) const
	{
		TxBlobString lc_val;
		if (this->_getValue_(&lc_val, _path, _b_assert))
		{
			*_value = lc_val.utf8Codecvt();
			return true;
		}
		return false;
	}
	bool getValue(TxBlobString *_value, const TxBlobString &_path, bool _b_assert = false) const
	{
		return this->_getValue_(_value, _path, _b_assert);
	}
	bool getValue(TxBlobWString *_value, const TxBlobString &_path, bool _b_assert = false) const
	{
		TxBlobString str;
		if (this->_getValue_(&str, _path, _b_assert))
		{
			*_value = str.utf8Codecvt();
			return true;
		}
		return false;
	}
	TxBlobString getValueString(const TxBlobString &_path, bool _b_assert = false) const
	{
		TxBlobString ret;
		this->_getValue_(&ret, _path, _b_assert);
		return ret;
	}
	TxBlobWString getValueStringW(const TxBlobString &_path, bool _b_assert = false) const
	{
		TxBlobString ret;
		this->_getValue_(&ret, _path, _b_assert);
		return ret.utf8Codecvt();
	}
	TxBoostMarkup getPtreeChild(const TxBlobString &_path, bool _b_assert = false) const
	{
		TxBoostMarkup ret;
		ret.pTree->clear();
		this->_lock_obj();
		try {
			*ret.pTree = this->pTree->get_child(std::string(_path.c_str(), _path.size()));
		}
		catch (...) {
			assert(!_b_assert);
			TX_UNUSED(_b_assert);
		}
		this->_unlock_obj();
		return ret;
	}
	bool addPtreeChild(const TxBlobString &_path, const boost::property_tree::ptree &_sub_pt)
	{
		bool ret = false;
		this->_lock_obj();
		try {
			this->pTree->add_child(std::string(_path.c_str(), _path.size()), _sub_pt);
			ret = true;
		}
		catch (...) {
			assert(false);
		}
		this->_unlock_obj();
		return ret;
	}
	bool addPtreeChild(const TxBlobString &_path, const TxBoostMarkup<true> &_sub_pt)
	{
		return this->addPtreeChild(_path, *_sub_pt.pTree);
	}
	bool addPtreeChild(const TxBlobString &_path, const TxBoostMarkup<false> &_sub_pt)
	{
		return this->addPtreeChild(_path, *_sub_pt.pTree);
	}
	bool putPtreeChild(const TxBlobString &_path, const boost::property_tree::ptree &_sub_pt)
	{
		bool ret = false;
		this->_lock_obj();
		try {
			this->pTree->put_child(std::string(_path.c_str(), _path.size()), _sub_pt);
			ret = true;
		}
		catch (...) {
			assert(false);
		}
		this->_unlock_obj();
		return ret;
	}
	bool putPtreeChild(const TxBlobString &_path, const TxBoostMarkup<true> &_sub_pt)
	{
		return this->putPtreeChild(_path, *_sub_pt.pTree);
	}
	bool putPtreeChild(const TxBlobString &_path, const TxBoostMarkup<false> &_sub_pt)
	{
		return this->putPtreeChild(_path, *_sub_pt.pTree);
	}
	bool putArrayTree(const TxBlobString &_path, const boost::property_tree::ptree &_arry_elem)
	{
		bool ret_true = true;
		this->_lock_obj();
		try {
			this->pTree->get_child(std::string(_path.c_str(), _path.size())).push_back(std::make_pair("", _arry_elem));
		}
		catch (...) {
			try {
				boost::property_tree::ptree lc_arry_pt;
				lc_arry_pt.push_back(std::make_pair("", _arry_elem));
				this->pTree->put_child(std::string(_path.c_str(), _path.size()), lc_arry_pt);
			}
			catch (...) {
				assert(false);
				ret_true = false;
			}
		}
		this->_unlock_obj();
		return ret_true;
	}
	bool putArrayTree(const TxBlobString &_path, const TxBoostMarkup<true> &_arry_elem)
	{
		return this->putArrayTree(_path, *_arry_elem.pTree);
	}
	bool putArrayTree(const TxBlobString &_path, const TxBoostMarkup<false> &_arry_elem)
	{
		return this->putArrayTree(_path, *_arry_elem.pTree);
	}
	bool isPathExist(const TxBlobString &_path) const
	{
		bool ret = false;
		this->_lock_obj();
		if (!this->pTree->empty())
		{
			try {
				this->pTree->get<std::string>(std::string(_path.c_str(), _path.size()));
				ret = true;
			}
			catch (...) {
				ret = false;
			}
		}
		this->_unlock_obj();
		return ret;
	}
	bool deletePtreeNode(const TxBlobString &_path, bool _b_assert = false)
	{
		bool ret = false;
		const char *lc_ptr = std::strrchr(_path.c_str(), '.');
		this->_lock_obj();
		try {
			if (lc_ptr == NULL) this->pTree->erase(std::string(_path.c_str(), _path.size()));
			else this->pTree->get_child(std::string(_path.c_str(), lc_ptr)).erase(lc_ptr + 1);
			ret = true;
		}
		catch (...) {
			assert(!_b_assert);
			TX_UNUSED(_b_assert);
			ret = false;
		}
		this->_unlock_obj();
		return ret;
	}
	void mutex(const std::function<void(boost::property_tree::ptree*)> &_func)
	{
		this->_lock_obj();
		_func(&this->pt);
		this->_unlock_obj();
	}
	void clear()
	{
		this->_lock_obj();
		this->pTree->clear();
		this->_unlock_obj();
	}
};


//--------------------------------------TxJsonArchive--------------------------------------//
class TxJsonArchive : private TxForbidDirectCopyObject
{
private:
	TxBoostMarkup<false> *const pt;
	const int i_in1_out2;
private:
	TxJsonArchive(TxBoostMarkup<false> *_pt, int _i_in1_out2) :pt(_pt), i_in1_out2(_i_in1_out2)
	{
	}
	~TxJsonArchive()
	{
	}
	template<class OBJECT_TYPE, int N>
	TxJsonArchive& _bindArray_fix_N_tmpl_(const TxBlobString &_name, OBJECT_TYPE *_arry_obj)
	{
		switch (this->i_in1_out2)
		{
		case 1://in
			this->pt->deletePtreeNode(_name);
			for (int i = 0; i < N; ++i)
			{
				TxBoostMarkup<false> lc_elem_pt;
				TxJsonArchive lc_elem_archive(&lc_elem_pt, this->i_in1_out2);
				_arry_obj[i].archive(&lc_elem_archive);
				this->pt->putArrayTree(_name, lc_elem_pt);
			}
			break;
		case 2://out
			if (true)
			{
				TxBoostMarkup<false> lc_arry_pt = this->pt->getPtreeChild(_name);
				auto lc_arry_pt_iter = lc_arry_pt.getBoostTree()->begin();
				auto lc_arry_pt_iter_end = lc_arry_pt.getBoostTree()->end();
				for (int i = 0; i < N; ++i)
				{
					TxBoostMarkup<false> lc_elem_pt;
					if (lc_arry_pt_iter != lc_arry_pt_iter_end)
						lc_elem_pt.attach(&(lc_arry_pt_iter++)->second);
					TxJsonArchive lc_elem_archive(&lc_elem_pt, this->i_in1_out2);
					_arry_obj[i].archive(&lc_elem_archive);
				}
			}
			break;
		default://“Ï≥£
			assert(false);
			break;
		}
		return *this;
	}
	template<class OBJECT_TYPE, class LIST_TMPL_TYPE>
	TxJsonArchive& _bindArray_std_tmpl_(const TxBlobString &_name, LIST_TMPL_TYPE &_manyObj)
	{
		switch (this->i_in1_out2)
		{
		case 1://in
			this->pt->deletePtreeNode(_name);
			for (auto iter = _manyObj.begin(); iter != _manyObj.end(); ++iter)
			{
				TxBoostMarkup<false> lc_elem_pt;
				TxJsonArchive lc_elem_archive(&lc_elem_pt, this->i_in1_out2);
				iter->archive(&lc_elem_archive);
				this->pt->putArrayTree(_name, lc_elem_pt);
			}
			break;
		case 2://out
			if (true)
			{
				_manyObj.clear();
				TxBoostMarkup<false> lc_arry_pt = this->pt->getPtreeChild(_name);
				auto lc_arry_pt_iter_end = lc_arry_pt.getBoostTree()->end();
				for (auto lc_arry_pt_iter = lc_arry_pt.getBoostTree()->begin();
					lc_arry_pt_iter != lc_arry_pt_iter_end; ++lc_arry_pt_iter)
				{
					TxBoostMarkup<false> lc_elem_pt;
					lc_elem_pt.attach(&lc_arry_pt_iter->second);
					TxJsonArchive lc_elem_archive(&lc_elem_pt, this->i_in1_out2);
					_manyObj.push_back(OBJECT_TYPE());
					_manyObj.back().archive(&lc_elem_archive);
				}
			}
			break;
		default://“Ï≥£
			assert(false);
			break;
		}
		return *this;
	}
public:
	inline bool isParseOperation() const
	{
		return this->i_in1_out2 == 2;
	}
	inline TxBoostMarkup<false>* getBoostMarkup()
	{
		return this->pt;
	}
	template<class OBJECT_TYPE>
	static TxBlobString toString(const OBJECT_TYPE &_object, const TxBlobString &_org_jsonStr = TxBlobString(),
		bool _pretty = false)
	{
		TxBoostMarkup<false> lcBoostMarkup;
		if (_org_jsonStr.length() > 0)
			lcBoostMarkup.parseJsonString(_org_jsonStr);
		TxJsonArchive lc_object(&lcBoostMarkup, 1);
		const_cast<OBJECT_TYPE*>(&_object)->archive(&lc_object);
		return lcBoostMarkup.saveJsonString(_pretty);
	}
	template<class OBJECT_TYPE>
	static TxBlobWString toWString(const OBJECT_TYPE &_object, const TxBlobWString &_org_jsonStr = TxBlobWString(),
		bool _pretty = false)
	{
		return toString(_object, _org_jsonStr.utf8Codecvt(), _pretty).utf8Codecvt();
	}
	template<class OBJECT_TYPE>
	static bool parseString(OBJECT_TYPE *_object, const TxBlobString &_jsonStr)
	{
		TxBoostMarkup<false> lcBoostMarkup;
		if (!lcBoostMarkup.parseJsonString(_jsonStr))
			return false;
		TxJsonArchive lc_object(&lcBoostMarkup, 2);
		_object->archive(&lc_object);
		return true;
	}
	template<class OBJECT_TYPE>
	static bool parseString(OBJECT_TYPE *_object, const TxBlobWString &_jsonStr)
	{
		return parseString(_object, _jsonStr.utf8Codecvt());
	}
	template<class VALUE_TYPE>
	TxJsonArchive& bindValue(const TxBlobString &_name, VALUE_TYPE &_value)
	{
		switch (this->i_in1_out2)
		{
		case 1://in
			this->pt->putValue(_name, _value);
			break;
		case 2://out
			this->pt->getValue(&_value, _name);
			break;
		default://“Ï≥£
			assert(false);
			break;
		}
		return *this;
	}
	template<class OBJECT_TYPE>
	TxJsonArchive& bindObject(const TxBlobString &_name, OBJECT_TYPE &_object)
	{
		TxBoostMarkup<false> lc_sub_pt = this->pt->getPtreeChild(_name);
		TxJsonArchive lc_archive(&lc_sub_pt, this->i_in1_out2);
		switch (this->i_in1_out2)
		{
		case 1://in
			_object.archive(&lc_archive);
			this->pt->putPtreeChild(_name, *lc_archive.pt);
			break;
		case 2://out
			_object.archive(&lc_archive);
			break;
		default://“Ï≥£
			assert(false);
			break;
		}
		return *this;
	}
	template<class OBJECT_TYPE, int N>
	TxJsonArchive& bindArrayObject(const TxBlobString &_name, OBJECT_TYPE(&_arry_obj)[N])
	{
		return this->_bindArray_fix_N_tmpl_<OBJECT_TYPE, N>(_name, _arry_obj);
	}
	template<class OBJECT_TYPE, int N>
	TxJsonArchive& bindArrayObject(const TxBlobString &_name, std::array<OBJECT_TYPE, N> &_arry_obj)
	{
		return this->_bindArray_fix_N_tmpl_<OBJECT_TYPE, N>(_name, _arry_obj.data());
	}
	template<class OBJECT_TYPE>
	TxJsonArchive& bindArrayObject(const TxBlobString &_name, std::list<OBJECT_TYPE> &_listObj)
	{
		return this->_bindArray_std_tmpl_<OBJECT_TYPE>(_name, _listObj);
	}
	template<class OBJECT_TYPE>
	TxJsonArchive& bindArrayObject(const TxBlobString &_name, std::vector<OBJECT_TYPE> &_vectorObj)
	{
		return this->_bindArray_std_tmpl_<OBJECT_TYPE>(_name, _vectorObj);
	}
};

#endif
