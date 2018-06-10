#pragma once

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/ossl_typ.h>
#include <openssl/buffer.h>
#include <openssl/crypto.h>
#include <string>
#include <string.h>
#include <vector>
#include <array>
#include <algorithm>

#include "TxSyslib.hpp"


class TxOpensslBase64
{
private:
	static bool _my_BIO_write__(BIO *_bio, const void *_data, int _size)
	{
		int lc_sign = BIO_write(_bio, _data, _size);
		if (lc_sign > 0)
			return true;
		sysLogError("BIO_write(_bio, _data, _size);·µ»Ø´íÎó¡£lc_sign=¡¾{0}¡¿").args(lc_sign);
		if (BIO_should_retry(_bio))
			return _my_BIO_write__(_bio, _data, _size);//Do something to handle the retry
		return false;//Handle failed read here
	}
	static int _my_BIO_read__(BIO *_bio, void *_data, int _size)
	{
		int lc_sign = BIO_read(_bio, _data, _size);
		if (lc_sign > 0)
			return lc_sign;
		sysLogError("BIO_read(_bio, _data, _size)·µ»Ø´íÎó¡£lc_sign=¡¾{0}¡¿").args(lc_sign);
		if (lc_sign == 0)
			return -1;//Handle closed connection
		if (BIO_should_retry(_bio))
			return _my_BIO_read__(_bio, _data, _size);//Do something to handle the retry
		return -1;
	}
	static bool _my_BIO_flush__(BIO *_bio)
	{
		int lc_sign = BIO_flush(_bio);
		if (lc_sign > 0)
			return true;
		sysLogError("BIO_flush(_bio)·µ»Ø´íÎó¡£lc_sign=¡¾{0}¡¿").args(lc_sign);
		if (BIO_should_retry(_bio))
			return _my_BIO_flush__(_bio);//Do something to handle the retry
		return false;
	}
	static BIO* _new_base64_bio_(bool _new_line)
	{
		BIO *ret = BIO_new(BIO_f_base64());
		if (!_new_line)
			BIO_set_flags(ret, BIO_FLAGS_BASE64_NO_NL);
		return ret;
	}
	static void _encode__(TxBlobString *_out, const void *_data, int _size, bool _new_line)
	{
		_out->clear();
		if (_data != NULL&&_size > 0)
		{
			BIO *lc_b64 = BIO_push(_new_base64_bio_(_new_line), BIO_new(BIO_s_mem()));
			if (_my_BIO_write__(lc_b64, _data, _size))
			{
				if (_my_BIO_flush__(lc_b64))
				{
					BUF_MEM *lc_bptr = NULL;
					BIO_get_mem_ptr(lc_b64, &lc_bptr);
					if (lc_bptr != NULL&&lc_bptr->data != NULL && (int)lc_bptr->length > 0)
						_out->assign((const char*)lc_bptr->data, (int)lc_bptr->length);
				}
			}
			BIO_free_all(lc_b64);
		}
	}
	static void _decode__(TxBlobString *_out, const void *_data, int _size)
	{
		if (_data != NULL&&_size > 0)
		{
			_out->resize(_size, false);
#ifndef NDEBUG
			TxBlobString lc_tsOrgString((const char*)_data, _size);
#endif
			BIO *lc_bmem = BIO_push(_new_base64_bio_(::memchr(_data, '\n', _size) != 0),
				BIO_new_mem_buf(const_cast<void*>(_data), _size));
			if (lc_bmem != NULL)
			{
				int lc_len = _my_BIO_read__(lc_bmem, _out->data(), (int)_out->size());
				if (lc_len < 0)
					lc_len = 0;
				_out->resize(lc_len, true);
				BIO_free_all(lc_bmem);
			}
#ifndef NDEBUG
			if (lc_tsOrgString != TxBlobString((const char*)_data, _size))
				sysLogError("if (lc_tsOrgString == TxBlobString((const char*)_data, _size))Âß¼­´íÎó");
#endif
		}
		else
		{
			_out->clear();
		}
	}
public:
	static TxBlobString encode(const void *_data, int _size, bool _new_line)
	{
		TxBlobString ret;
		_encode__(&ret, _data, _size, _new_line);
		return ret;
	}
	static TxBlobString encode(const TxBlobString &_data, bool _new_line)
	{
		TxBlobString ret;
		_encode__(&ret, _data.data(), _data.size(), _new_line);
		return ret;
	}
	static TxBlobString decode(const void *_data, int _size)
	{
		TxBlobString ret;
		_decode__(&ret, _data, _size);
		return ret;
	}
	static TxBlobString decode(const TxBlobString &_data)
	{
		TxBlobString ret;
		_decode__(&ret, _data.data(), _data.size());
		return ret;
	}
};


class TxOpensslMd5
{
public:
	enum { MD5_BYTE_SIZE = MD5_DIGEST_LENGTH, };
private:
	std::array<unsigned char, MD5_BYTE_SIZE> result;
	MD5_CTX mMd5Ctx;
	long bValid;
public:
	TxOpensslMd5()
	{
		this->result.fill(0);
		this->bValid = 0;
	}
	~TxOpensslMd5()
	{
		this->final();
	}
	void initial()
	{
		if (this->bValid)
			::OPENSSL_cleanse(&this->mMd5Ctx, sizeof(this->mMd5Ctx));
		::MD5_Init(&this->mMd5Ctx);
		this->bValid = 1;
	}
	void update(const void *_data, int _size)
	{
		if (this->bValid)
			::MD5_Update(&this->mMd5Ctx, _data, _size);
	}
	const std::array<unsigned char, MD5_BYTE_SIZE>& final()
	{
		if (this->bValid)
		{
			::MD5_Final(this->result.data(), &this->mMd5Ctx);
			::OPENSSL_cleanse(&this->mMd5Ctx, sizeof(this->mMd5Ctx));
			this->bValid = 0;
		}
		return this->result;
	}
};

class TxOpensslSha1
{
public:
	enum { SHA1_BYTE_SIZE = SHA_DIGEST_LENGTH, };
private:
	std::array<unsigned char, SHA1_BYTE_SIZE> result;
	SHA_CTX mSha1Ctx;
	long bValid;
public:
	TxOpensslSha1()
	{
		this->result.fill(0);
		this->bValid = 0;
	}
	~TxOpensslSha1()
	{
		if (this->bValid)
			::OPENSSL_cleanse(&this->mSha1Ctx, sizeof(this->mSha1Ctx));
		this->bValid = 0;
	}
	void initial()
	{
		if (this->bValid)
			::OPENSSL_cleanse(&this->mSha1Ctx, sizeof(mSha1Ctx));
		::SHA1_Init(&this->mSha1Ctx);
		this->bValid = 1;
	}
	void update(const void *_data, int _size)
	{
		if (this->bValid)
			::SHA1_Update(&this->mSha1Ctx, _data, _size);
	}
	const std::array<unsigned char, SHA1_BYTE_SIZE>& final()
	{
		if (this->bValid)
		{
			::SHA1_Final(this->result.data(), &this->mSha1Ctx);
			::OPENSSL_cleanse(&this->mSha1Ctx, sizeof(this->mSha1Ctx));
			this->bValid = 0;
		}
		return this->result;
	}
};

class TxOpensslAes
{
private:
	AES_KEY mAesKey;
	TxBlobString remain;
	TxBlobString result;
	std::function<void(const unsigned char*, int)> mfunc;
	const long bEncodeSign;
public:
	TxOpensslAes(bool _bEncode, bool _bDecode) :bEncodeSign(_bEncode&&!_bDecode)
	{
		assert(_bEncode^_bDecode);
		AES_KEY lc_aes_key = { 0 };
		this->mAesKey = lc_aes_key;
	}
	virtual ~TxOpensslAes()
	{
		assert(this->remain.size() == 0);
	}
	bool initial(const TxBlobString &_psw, const std::function<void(const unsigned char*, int)> &_mfunc)
	{
		this->mfunc = _mfunc;
		if (this->mfunc && (_psw.size() == 16 || _psw.size() == 24 && _psw.size() == 32)
			&& (this->bEncodeSign ? (::AES_set_encrypt_key) : (::AES_set_decrypt_key))
			((const unsigned char*)_psw.data(), _psw.size() * 8, &this->mAesKey) >= 0)
		{
			return true;
		}
		assert(0);
		return false;
	}
	bool initial(const std::array<unsigned char, 16> &_psw, const std::function<void(const unsigned char*, int)> &_mfunc)
	{
		return this->initial(TxBlobString(_psw.data(), (int)_psw.size()), _mfunc);
	}
	bool initial(const std::array<unsigned char, 24> &_psw, const std::function<void(const unsigned char*, int)> &_mfunc)
	{
		return this->initial(TxBlobString(_psw.data(), (int)_psw.size()), _mfunc);
	}
	bool initial(const std::array<unsigned char, 32> &_psw, const std::function<void(const unsigned char*, int)> &_mfunc)
	{
		return this->initial(TxBlobString(_psw.data(), (int)_psw.size()), _mfunc);
	}
	void final()
	{
		if (this->remain.size() > 0)
		{
			char lc_zero[AES_BLOCK_SIZE] = { 0 };
			this->update(lc_zero, sizeof(lc_zero) - this->remain.size());
		}
	}
	void update(const TxBlobString& _data)
	{
		this->update(_data.data(), _data.size());
	}
	void update(const std::string& _data)
	{
		this->update(_data.data(), (int)_data.size());
	}
	void update(const void *_data, int _size)
	{
		this->remain.append((const unsigned char*)_data, _size);
		if (this->result.size() < this->remain.size())
			this->result.resize(this->remain.size(), false);
		::memset(this->result.data(), 0, this->remain.size());
		int lc_index = 0, lc_valid_size = this->remain.size() - AES_BLOCK_SIZE;
		for (lc_index = 0; lc_index <= lc_valid_size; lc_index += AES_BLOCK_SIZE)
		{
			(this->bEncodeSign ? (::AES_encrypt) : (::AES_decrypt))
				((const unsigned char*)this->remain.data() + lc_index,
				(unsigned char*)this->result.data() + lc_index, &this->mAesKey);
		}
		if (lc_index > 0)
		{
			if (this->mfunc)
				this->mfunc((const unsigned char*)this->result.data(), lc_index);
			this->remain = TxBlobString(this->remain.data() + lc_index, this->remain.size() - lc_index);
		}
	}
};

class TxOpensslAesEncode : public TxOpensslAes{ public:TxOpensslAesEncode() :TxOpensslAes(true, false){} };
class TxOpensslAesDecode : public TxOpensslAes{ public:TxOpensslAesDecode() :TxOpensslAes(false, true){} };



