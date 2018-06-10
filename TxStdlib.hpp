#ifndef _PUBLIC_TX_STD_LIB_
#define _PUBLIC_TX_STD_LIB_

#if defined(WIN32) || defined(WIN64)//windows

#pragma		warning(disable:4996)

#ifndef		_WINSOCK_DEPRECATED_NO_WARNINGS
#define			_WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#ifndef		_CRT_SECURE_NO_WARNINGS
#define			_CRT_SECURE_NO_WARNINGS
#endif

#ifndef		_CRT_SECURE_NO_DEPRECATE
#define			_CRT_SECURE_NO_DEPRECATE
#endif

#ifndef		_SCL_SECURE_NO_WARNINGS
#define			_SCL_SECURE_NO_WARNINGS
#endif

#if			((_MSC_VER)>=(1800))	// VisualStudio 2013
#define			TXSTDCONSTEXPR_ constexpr
#else
#define			TXSTDCONSTEXPR_ 
#endif

#if			((_MSC_VER)>=(1900))	// VisualStudio 2015
#define			TXSTDALIGNAS_(N) alignas(N)
#else
#define			TXSTDALIGNAS_(N) __declspec(align(N))
#endif

#elif defined(LINUX) || defined(linux)//linux
#define		TXSTDCONSTEXPR_ constexpr
#define		TXSTDALIGNAS_(N) alignas(N)

#else//other
#define		TXSTDCONSTEXPR_ constexpr
#define		TXSTDALIGNAS_(N) alignas(N)

#endif


//summary
#define TXSTDCONSTEXPR		TXSTDCONSTEXPR_
#define TXSTDALIGNAS		TXSTDALIGNAS_
#define TXSTDALIGNAS_1		TXSTDALIGNAS_(1)
#define TXSTDALIGNAS_16		TXSTDALIGNAS_(16)


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <cctype>
#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <stdexcept>
#include <algorithm>
#include <type_traits>
#include <memory>
#include <codecvt>
#include <functional>
#include <typeinfo>
#include <set>
#include <sstream>
#include <map>
#include <array>
#include <exception>

#define TX_UNUSED(...) _TxPrivateNs_::MyClassFuncTmp_::unused_fn_(__VA_ARGS__)


//函数局部变量析构时调用
#define _Def_Defer_d282_4e06_9a5fecdc168e8467_Ini_(line,...)  __defer_d282_4e06_9a5fecdc168e8467_init_##line##__VA_ARGS__
#define _Def_Defer_d282_4e06_9a5fecdc168e8467_Obj_(line,...)  __defer_d282_4e06_9a5fecdc168e8467_Obj_##line##__VA_ARGS__
#define TX_DEFER(_fn,...) \
	auto _Def_Defer_d282_4e06_9a5fecdc168e8467_Ini_(__LINE__,__VA_ARGS__)(_fn); \
	_TxPrivateNs_::_TxDeferFuncGen__::_FuncClass__ _Def_Defer_d282_4e06_9a5fecdc168e8467_Obj_(__LINE__,__VA_ARGS__); \
	_TxPrivateNs_::_TxDeferFuncGen__::gen(_Def_Defer_d282_4e06_9a5fecdc168e8467_Ini_(__LINE__,__VA_ARGS__),\
	_Def_Defer_d282_4e06_9a5fecdc168e8467_Obj_(__LINE__,__VA_ARGS__));\
	TX_UNUSED(_Def_Defer_d282_4e06_9a5fecdc168e8467_Ini_(__LINE__,__VA_ARGS__), \
	_Def_Defer_d282_4e06_9a5fecdc168e8467_Obj_(__LINE__,__VA_ARGS__));


#pragma pack(push)
#pragma pack(1)
template<int N>
struct TXSTDALIGNAS_1 TxSizeMemory
{
	unsigned char _data_[N];
};
#pragma pack(pop)

template<int N> class TxEmptyObjectTmpl { };

//--------------------------------------TxForbidDirectCopyObject--------------------------------------//
class TxForbidDirectCopyObject
{
private:
	TxForbidDirectCopyObject(const TxForbidDirectCopyObject&)
	{
		assert([]()->bool {(void)("copy a object,but it is forbided copyable![a]"); return false; }());
	}
	TxForbidDirectCopyObject& operator=(const TxForbidDirectCopyObject&)
	{
		assert([]()->bool {(void)("copy a object,but it is forbided copyable![b]"); return false; }()); return *(TxForbidDirectCopyObject*)NULL;
	}
public:
	TxForbidDirectCopyObject() { }
};


namespace _TxPrivateNs_ {

	//--------------------------------------MyClassFuncTmp_--------------------------------------//
	class MyClassFuncTmp_
	{
	public:
		static inline void unused_fn_() { }
		template<class T0>
		static inline void unused_fn_(const T0&) { }
		template<class T0, class T1>
		static inline void unused_fn_(const T0&, const T1&) { }
		template<class T0, class T1, class T2>
		static inline void unused_fn_(const T0&, const T1&, const T2&) { }
		template<class T0, class T1, class T2, class T3>
		static inline void unused_fn_(const T0&, const T1&, const T2&, const T3&) { }
		template<class T0, class T1, class T2, class T3, class T4>
		static inline void unused_fn_(const T0&, const T1&, const T2&, const T3&, const T4&) { }
		template<class T0, class T1, class T2, class T3, class T4, class T5>
		static inline void unused_fn_(const T0&, const T1&, const T2&, const T3&, const T4&, const T5&) { }
		template<class T0, class T1, class T2, class T3, class T4, class T5, class T6>
		static inline void unused_fn_(const T0&, const T1&, const T2&, const T3&, const T4&, const T5&, const T6&) { }
		template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
		static inline void unused_fn_(const T0&, const T1&, const T2&, const T3&, const T4&, const T5&, const T6&, const T7&) { }
		template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
		static inline void unused_fn_(const T0&, const T1&, const T2&, const T3&, const T4&, const T5&, const T6&, const T7&, const T8&) { }
		template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
		static inline void unused_fn_(const T0&, const T1&, const T2&, const T3&, const T4&, const T5&, const T6&, const T7&, const T8&, const T9&) { }
	};

	//--------------------------------------_TxDeferFuncGen__--------------------------------------//
	class _TxDeferFuncGen__
	{
	public:
		class _FuncClass__
		{
			friend class _TxDeferFuncGen__;
		private:
			void *fn_ptr;
			void(*defer_fn)(void*);
		public:
			~_FuncClass__() { this->defer_fn(this->fn_ptr); }
		};
		template<class TFUNC>
		static void gen(TFUNC &_func, _FuncClass__ &_outFn)
		{
			class TT { public: static void defer_cb_(void *_fn) { (*(TFUNC*)_fn)(); } };
			_outFn.fn_ptr=&_func;
			_outFn.defer_fn = &TT::defer_cb_;
		}
	};

	//--------------------------------------TDefaultValRef--------------------------------------//
	template<class T_DATA>
	class TDefaultValRef
	{
	private:
		template<class T>//Class
		static void _init_defual_(T *_val, const TxEmptyObjectTmpl<0>&)
		{
			TX_UNUSED(_val);
		}
		template<class T>//Number
		static void _init_defual_(T *_val, const TxEmptyObjectTmpl<1>&)
		{
			*_val = (T)0;
		}
	public:
		static const T_DATA& get_()
		{
			struct TTT {
				unsigned char buffer_[sizeof(T_DATA) + sizeof(size_t)];
				TTT()
				{
					::memset(this->buffer_, 0, sizeof(this->buffer_));
					new ((T_DATA*)this->buffer_) T_DATA();
					_init_defual_((T_DATA*)this->buffer_, TxEmptyObjectTmpl<(int)!!(
						std::numeric_limits<T_DATA>::is_integer
						|| std::is_integral<T_DATA>::value
						|| std::is_floating_point<T_DATA>::value
						|| std::is_enum<T_DATA>::value
						|| std::is_pointer<T_DATA>::value
						)>());
				}
				~TTT()
				{
					((T_DATA*)this->buffer_)->~T_DATA();
				}
			};
			static TTT ret;
			return *(T_DATA*)ret.buffer_;
		}
	};

	//--------------------------------------CStdlibBase--------------------------------------//
	class CStdlibBase
	{
	public:
		static void initEnv()
		{
			hexUpperChar(0);
			hexLowerChar(0);
		}

		template<class T, int N>
		inline static TXSTDCONSTEXPR int arrayLength(const T(&)[N])
		{
			return N;
		}

		template<class T>
		inline static TXSTDCONSTEXPR const T& maximum(const T &_a, const T &_b)
		{
			return (_a) < (_b) ? (_b) : (_a);
		}
		template<class T>
		inline static TXSTDCONSTEXPR const T& maximum(const T &_a, const T &_b, const T &_c)
		{
			return maximum(_a, maximum(_b, _c));
		}
		template<class T>
		inline static TXSTDCONSTEXPR const T& maximum(const T &_a, const T &_b, const T &_c, const T &_d)
		{
			return maximum(maximum(_a, _b), maximum(_c, _d));
		}
		template<class T>
		inline static TXSTDCONSTEXPR const T& minimum(const T &_a, const T &_b)
		{
			return (_a) < (_b) ? (_a) : (_b);
		}
		template<class T>
		inline static TXSTDCONSTEXPR const T& minimum(const T &_a, const T &_b, const T &_c)
		{
			return minimum(_a, minimum(_b, _c));
		}
		template<class T>
		inline static TXSTDCONSTEXPR const T& minimum(const T &_a, const T &_b, const T &_c, const T &_d)
		{
			return minimum(minimum(_a, _b), minimum(_c, _d));
		}
		template<class T>
		inline static TXSTDCONSTEXPR const T& adjustRange(const T &_value, const T &_range1, const T &_range2)
		{
			return (_range1 < _range2) ?
				maximum(_range1, minimum(_value, _range2))
				: maximum(_range2, minimum(_value, _range1));
		}
		template<class T>
		inline static TXSTDCONSTEXPR T absolute(const T &_a)
		{
			return (_a) < (T)0 ? -(_a) : (_a);
		}
		template<class T1, class T2>
		inline static void memoryCopy(T1 &_dest, const T2 &_src)
		{
			static_assert(std::is_standard_layout<T1>::value&&std::is_standard_layout<T2>::value, "memoryCopy()");
			*(TxSizeMemory<sizeof(T1)>*)&_dest = *(TxSizeMemory<sizeof(T2)>*)&_src;
		}
		template<class T1, class T2, int N2>
		inline static void memoryCopy(T1 &_dest, T2(&_src)[N2])
		{
			static_assert(std::is_standard_layout<T1>::value&&std::is_standard_layout<T2>::value, "memoryCopy()");
			*(TxSizeMemory<sizeof(T1)>*)&_dest = *(TxSizeMemory<N2 * sizeof(T2)>*)_src;
		}
		template<class T1, int N1, class T2, int N2>
		inline static void memoryCopy(T1(&_dest)[N1], T2(&_src)[N2])
		{
			static_assert(std::is_standard_layout<T1>::value&&std::is_standard_layout<T2>::value, "memoryCopy()");
			*(TxSizeMemory<N1 * sizeof(T1)>*)_dest = *(TxSizeMemory<N2 * sizeof(T2)>*)_src;
		}
		template<class T1, int N1, class T2>
		inline static void memoryCopy(T1(&_dest)[N1], const T2 &_src)
		{
			static_assert(std::is_standard_layout<T1>::value&&std::is_standard_layout<T2>::value, "memoryCopy()");
			*(TxSizeMemory<N1 * sizeof(T1)>*)_dest = *(TxSizeMemory<sizeof(T2)>*)&_src;
		}

		inline static TXSTDCONSTEXPR char hexUpperChar(int _value)
		{
			return "0123456789ABCDEF"[_value & 0x0f];
		}
		inline static TXSTDCONSTEXPR char hexLowerChar(int _value)
		{
			return "0123456789abcdef"[_value & 0x0f];
		}
		static std::string hexUpperString(const std::string &_data, char _separator = 0)
		{
			std::string ret;
			if (_separator == 0)
			{
				ret.resize(_data.size() * 2);
				for (int i = (int)_data.size(); i--;)
				{
					ret[i * 2] = hexUpperChar((_data.data()[i] >> 4) & 0x0f);
					ret[i * 2 + 1] = hexUpperChar(_data.data()[i] & 0x0f);
				}
			}
			else
			{
				ret.resize(_data.size() * 3);
				for (int i = (int)_data.size(); i--;)
				{
					ret[i * 3] = hexUpperChar((_data.data()[i] >> 4) & 0x0f);
					ret[i * 3 + 1] = hexUpperChar(_data.data()[i] & 0x0f);
					ret[i * 3 + 2] = _separator;
				}
			}
			return ret;
		}
		static std::string hexLowerString(const std::string &_data, char _separator)
		{
			std::string ret;
			if (_separator == 0)
			{
				ret.resize(_data.size() * 2);
				for (int i = (int)_data.size(); i--;)
				{
					ret[i * 2] = hexLowerChar((_data.data()[i] >> 4) & 0x0f);
					ret[i * 2 + 1] = hexLowerChar(_data.data()[i] & 0x0f);
				}
			}
			else
			{
				ret.resize(_data.size() * 3);
				for (int i = (int)_data.size(); i--;)
				{
					ret[i * 3] = hexLowerChar((_data.data()[i] >> 4) & 0x0f);
					ret[i * 3 + 1] = hexLowerChar(_data.data()[i] & 0x0f);
					ret[i * 3 + 2] = _separator;
				}
			}
			return ret;
		}
	};


	//--------------------------------------My_Aes128_--------------------------------------//
	class My_Aes128_
	{//速度测试，加密+解密的性能，一秒钟1.2M+。然而用OpenSSL可达到389M每秒
	public:
		static void encode(std::string *_outData, const unsigned char _aes_psw[16], const void *_data, int _size)
		{
			return deal_encode_decode_(_outData, _aes_psw, _data, _size, &My_Aes128_::Cipher);
		}
		static void decode(std::string *_outData, const unsigned char _aes_psw[16], const void *_data, int _size)
		{
			return deal_encode_decode_(_outData, _aes_psw, _data, _size, &My_Aes128_::InvCipher);
		}
		static void initEnv()
		{
			My_Aes128_ mm(NULL);
			TX_UNUSED(mm);
		}
	private:
		My_Aes128_(const unsigned char* _key)
		{
			static TXSTDCONSTEXPR const unsigned char lc_sBox_And_invsBox[][256] = {
				{//   0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f 
					0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, //0
						0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, //1
						0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, //2
						0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, //3
						0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, //4
						0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, //5
						0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, //6
						0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, //7
						0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, //8
						0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, //9
						0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, //a
						0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, //b
						0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, //c
						0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, //d
						0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, //e
						0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16  //f
				},
				{//   0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
					0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, //0
						0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, //1
						0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, //2
						0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, //3
						0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, //4
						0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, //5
						0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, //6
						0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, //7
						0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, //8
						0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, //9
						0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, //a
						0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, //b
						0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, //c
						0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, //d
						0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, //e
						0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d  //f
					}
			};
			this->pSbox = lc_sBox_And_invsBox[0];
			this->pInvSbox = lc_sBox_And_invsBox[1];
			assert(this->pSbox[255] != 0 && this->pInvSbox[255] != 0 && std::is_unsigned<size_t>::value);
			if (_key != NULL)
				this->KeyExpansion(_key, w);
		}
		~My_Aes128_()
		{
		}
		static void deal_encode_decode_(std::string *_outData, const unsigned char *_aes_psw,
			const void *_data, int _size, unsigned char* (My_Aes128_::*_mem_func)(unsigned char* input))
		{
			enum { _E_AES_BUFFER_SIZE = 16, };
			if (_data == NULL || _size <= 0)
			{
				_outData->clear();
				return;
			}
			_outData->reserve(((_size + (_E_AES_BUFFER_SIZE - 1)) / _E_AES_BUFFER_SIZE)*_E_AES_BUFFER_SIZE);
			My_Aes128_ lc_aes(_aes_psw);
			unsigned char lc_buffer[_E_AES_BUFFER_SIZE];
			for (const char *lc_pos = (const char*)_data, *lc_end = (const char*)_data + _size;
				(size_t)lc_pos < (size_t)lc_end; lc_pos += _E_AES_BUFFER_SIZE)
			{
				if ((size_t)lc_pos + _E_AES_BUFFER_SIZE <= (size_t)lc_end)
				{
					::memcpy(lc_buffer, lc_pos, _E_AES_BUFFER_SIZE);
				}
				else
				{
					::memset(lc_buffer, 0, _E_AES_BUFFER_SIZE);
					::memcpy(lc_buffer, lc_pos, (size_t)(lc_end - lc_pos));
				}
				(lc_aes.*_mem_func)(lc_buffer);
				_outData->append((const char*)lc_buffer, (size_t)_E_AES_BUFFER_SIZE);
			}
		}
		unsigned char* Cipher(unsigned char* input)
		{
			unsigned char state[4][4];
			int i, r, c;

			for (r = 0; r < 4; r++)
			{
				for (c = 0; c < 4; c++)
				{
					state[r][c] = input[c * 4 + r];
				}
			}

			AddRoundKey(state, w[0]);

			for (i = 1; i <= 10; i++)
			{
				SubBytes(state);
				ShiftRows(state);
				if (i != 10)MixColumns(state);
				AddRoundKey(state, w[i]);
			}

			for (r = 0; r < 4; r++)
			{
				for (c = 0; c < 4; c++)
				{
					input[c * 4 + r] = state[r][c];
				}
			}

			return input;
		}

		unsigned char* InvCipher(unsigned char* input)
		{
			unsigned char state[4][4];
			int i, r, c;

			for (r = 0; r < 4; r++)
			{
				for (c = 0; c < 4; c++)
				{
					state[r][c] = input[c * 4 + r];
				}
			}

			AddRoundKey(state, w[10]);
			for (i = 9; i >= 0; i--)
			{
				InvShiftRows(state);
				InvSubBytes(state);
				AddRoundKey(state, w[i]);
				if (i)
				{
					InvMixColumns(state);
				}
			}

			for (r = 0; r < 4; r++)
			{
				for (c = 0; c < 4; c++)
				{
					input[c * 4 + r] = state[r][c];
				}
			}

			return input;
		}
	private:
		const unsigned char *pSbox;
		const unsigned char *pInvSbox;
		unsigned char w[11][4][4];
	private:
		void KeyExpansion(const unsigned char* key, unsigned char w[][4][4])
		{
			int i, j, r, c;
			unsigned char rc[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };
			for (r = 0; r < 4; r++)
			{
				for (c = 0; c < 4; c++)
				{
					w[0][r][c] = key[r + c * 4];
				}
			}
			for (i = 1; i <= 10; i++)
			{
				for (j = 0; j < 4; j++)
				{
					unsigned char t[4];
					for (r = 0; r < 4; r++)
					{
						t[r] = j ? w[i][r][j - 1] : w[i - 1][r][3];
					}
					if (j == 0)
					{
						unsigned char temp = t[0];
						for (r = 0; r < 3; r++)
						{
							t[r] = this->pSbox[t[(r + 1) % 4]];
						}
						t[3] = this->pSbox[temp];
						t[0] ^= rc[i - 1];
					}
					for (r = 0; r < 4; r++)
					{
						w[i][r][j] = w[i - 1][r][j] ^ t[r];
					}
				}
			}
		}
		unsigned char FFmul(unsigned char a, unsigned char b)
		{
			unsigned char bw[4];
			unsigned char res = 0;
			int i;
			bw[0] = b;
			for (i = 1; i < 4; i++)
			{
				bw[i] = bw[i - 1] << 1;
				if (bw[i - 1] & 0x80)
				{
					bw[i] ^= 0x1b;
				}
			}
			for (i = 0; i < 4; i++)
			{
				if ((a >> i) & 0x01)
				{
					res ^= bw[i];
				}
			}
			return res;
		}

		void SubBytes(unsigned char state[][4])
		{
			int r, c;
			for (r = 0; r < 4; r++)
			{
				for (c = 0; c < 4; c++)
				{
					state[r][c] = this->pSbox[state[r][c]];
				}
			}
		}
		void ShiftRows(unsigned char state[][4])
		{
			unsigned char t[4];
			int r, c;
			for (r = 1; r < 4; r++)
			{
				for (c = 0; c < 4; c++)
				{
					t[c] = state[r][(c + r) % 4];
				}
				for (c = 0; c < 4; c++)
				{
					state[r][c] = t[c];
				}
			}
		}
		void MixColumns(unsigned char state[][4])
		{
			unsigned char t[4];
			int r, c;
			for (c = 0; c < 4; c++)
			{
				for (r = 0; r < 4; r++)
				{
					t[r] = state[r][c];
				}
				for (r = 0; r < 4; r++)
				{
					state[r][c] = FFmul(0x02, t[r])
						^ FFmul(0x03, t[(r + 1) % 4])
						^ FFmul(0x01, t[(r + 2) % 4])
						^ FFmul(0x01, t[(r + 3) % 4]);
				}
			}
		}
		void AddRoundKey(unsigned char state[][4], unsigned char k[][4])
		{
			int r, c;
			for (c = 0; c < 4; c++)
			{
				for (r = 0; r < 4; r++)
				{
					state[r][c] ^= k[r][c];
				}
			}
		}
		void InvSubBytes(unsigned char state[][4])
		{
			int r, c;
			for (r = 0; r < 4; r++)
			{
				for (c = 0; c < 4; c++)
				{
					state[r][c] = this->pInvSbox[state[r][c]];
				}
			}
		}
		void InvShiftRows(unsigned char state[][4])
		{
			unsigned char t[4];
			int r, c;
			for (r = 1; r < 4; r++)
			{
				for (c = 0; c < 4; c++)
				{
					t[c] = state[r][(c - r + 4) % 4];
				}
				for (c = 0; c < 4; c++)
				{
					state[r][c] = t[c];
				}
			}
		}
		void InvMixColumns(unsigned char state[][4])
		{
			unsigned char t[4];
			int r, c;
			for (c = 0; c < 4; c++)
			{
				for (r = 0; r < 4; r++)
				{
					t[r] = state[r][c];
				}
				for (r = 0; r < 4; r++)
				{
					state[r][c] = FFmul(0x0e, t[r])
						^ FFmul(0x0b, t[(r + 1) % 4])
						^ FFmul(0x0d, t[(r + 2) % 4])
						^ FFmul(0x09, t[(r + 3) % 4]);
				}
			}
		}
	};


	//--------------------------------------My_Md5_--------------------------------------//
	class My_Md5_
	{
	private:
		static void _buildMd5__(uint8_t result[16], const uint8_t initial_msg[], int initial_len)
		{
			// leftrotate function definition
#define _MD5_LEFTROTATE__(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
#define _MD5_NUM32FROMMEM__(strp) \
	( \
	(((std::uint32_t)((std::uint8_t*)(strp))[0])<<0) \
	|(((std::uint32_t)((std::uint8_t*)(strp))[1])<<8) \
	|(((std::uint32_t)((std::uint8_t*)(strp))[2])<<16) \
	|(((std::uint32_t)((std::uint8_t*)(strp))[3])<<24) \
	)
#define _MD5_MEMBYNUM32__(strp,val32) \
	( \
	((std::uint8_t*)(strp))[0]=(std::uint8_t)(((val32)>>0)&0xff),\
	((std::uint8_t*)(strp))[1]=(std::uint8_t)(((val32)>>8)&0xff),\
	((std::uint8_t*)(strp))[2]=(std::uint8_t)(((val32)>>16)&0xff),\
	((std::uint8_t*)(strp))[3]=(std::uint8_t)(((val32)>>24)&0xff) \
	)

			// Constants are the integer part of the sines of integers (in radians) * 2^32.
			static TXSTDCONSTEXPR const std::uint32_t md5k[64] = {
				0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
				0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
				0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
				0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
				0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
				0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
				0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
				0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
				0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
				0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
				0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
				0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
				0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
				0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
				0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
				0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
			};

			// r specifies the per-round shift amounts
			static TXSTDCONSTEXPR const std::uint32_t md5r[] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
				5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
				4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
				6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };
			if (initial_len >= 0)
			{
				// Initialize variables - simple count in nibbles:
				std::uint32_t val0 = 0x67452301
					, val1 = 0xefcdab89
					, val2 = 0x98badcfe
					, val3 = 0x10325476;

				int new_len, offset;
				for (new_len = initial_len * 8 + 1; new_len % 512 != 448; new_len++)
					;
				new_len /= 8;

				std::vector<std::uint8_t> lc_vct_msg;
				lc_vct_msg.resize(new_len + 64, 0);
				// (we alloc also 64 extra bytes...)
				memcpy(lc_vct_msg.data(), initial_msg, initial_len);
				lc_vct_msg.data()[initial_len] = 128; // write the "1" bit

				_MD5_MEMBYNUM32__(lc_vct_msg.data() + new_len, 8 * initial_len);

				// Process the message in successive 512-bit chunks:
				//for each 512-bit chunk of message:
				for (offset = 0; offset < new_len; offset += (512 / 8))
				{
					// break chunk into sixteen 32-bit words w[j], 0 ≤ j ≤ 15
					std::uint32_t *w = (std::uint32_t *)(lc_vct_msg.data() + offset);
					// Initialize hash value for this chunk:
					std::uint32_t a = val0, b = val1, c = val2, d = val3, temp;
					// Main loop:
					std::uint32_t i;
					for (i = 0; i < 64; i++)
					{
						std::uint32_t f, g;
						if (i < 16)
						{
							f = (b & c) | ((~b) & d);
							g = i;
						}
						else if (i < 32)
						{
							f = (d & b) | ((~d) & c);
							g = (5 * i + 1) % 16;
						}
						else if (i < 48)
						{
							f = b ^ c ^ d;
							g = (3 * i + 5) % 16;
						}
						else
						{
							f = c ^ (b | (~d));
							g = (7 * i) % 16;
						}
						//uint32_t temp = d;
						temp = d;
						d = c;
						c = b;
						//a += f + md5k[i] + w[g];
						a += f + md5k[i] + _MD5_NUM32FROMMEM__(&w[g]);
						b = b + _MD5_LEFTROTATE__(a, md5r[i]);
						a = temp;
					}
					// Add this chunk's hash to val so far:
					val0 += a; val1 += b; val2 += c; val3 += d;
				}
				_MD5_MEMBYNUM32__(&result[0], val0);
				_MD5_MEMBYNUM32__(&result[4], val1);
				_MD5_MEMBYNUM32__(&result[8], val2);
				_MD5_MEMBYNUM32__(&result[12], val3);
			}
#undef _MD5_LEFTROTATE__
#undef _MD5_NUM32FROMMEM__
#undef _MD5_MEMBYNUM32__
		}
	public:
		static void generateMd5(unsigned char _md5[16], const void* _data, int _size)
		{
			_buildMd5__(_md5, (const uint8_t*)_data, _size);
		}
		static std::string generateMd5(const void* _data, int _size)
		{
			unsigned char lc_md5[16] = { 0 };
			generateMd5(lc_md5, _data, _size);
			return CStdlibBase::hexUpperString(std::string((const char*)lc_md5, CStdlibBase::arrayLength(lc_md5)));
		}
		static void initEnv()
		{
			My_Aes128_::initEnv();
			_buildMd5__(NULL, NULL, -1);
		}
	};

	//--------------------------------------My_Base64_--------------------------------------//
	class My_Base64_
	{
	public:
		static void initEnv()
		{
			std::string lc_str;
			Encode(&lc_str, (const unsigned char*)"", 0, true);
			Decode(&lc_str, (const char*)"", 0);
		}
		static void Encode(std::string *_out, const unsigned char* Data, int DataSize, bool _bNewLine)
		{
			_out->clear();
			_out->reserve((int)((DataSize + DataSize / 64) * 4 / 3));
			//编码表
			static const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
			//返回值
			int LineLength = 0;
			unsigned char lc_Tmp[4];
			for (int i = 0; i < (int)(DataSize / 3); i++)
			{
				*(TxSizeMemory<3>*)&lc_Tmp[0] = *(TxSizeMemory<3>*)Data;
				Data += 3;
				char lc_chs[] = {
					EncodeTable[lc_Tmp[0] >> 2],
					EncodeTable[((lc_Tmp[0] << 4) | (lc_Tmp[1] >> 4)) & 0x3F],
					EncodeTable[((lc_Tmp[1] << 2) | (lc_Tmp[2] >> 6)) & 0x3F],
					EncodeTable[lc_Tmp[2] & 0x3F]
				};
				_out->append(lc_chs, sizeof(lc_chs) / sizeof(lc_chs[0]));
				LineLength += 4;
				if (LineLength == 76)
				{
					if (_bNewLine)
						_out->append("\r\n");
					LineLength = 0;
				}
			}
			//对剩余数据进行编码
			int Mod = DataSize % 3;
			if (Mod == 1)
			{
				lc_Tmp[0] = *Data++;
				char lc_chs[] = {
					EncodeTable[(lc_Tmp[0] & 0xFC) >> 2],
					EncodeTable[((lc_Tmp[0] & 0x03) << 4)],
					'=',
					'='
				};
				_out->append(lc_chs, sizeof(lc_chs) / sizeof(lc_chs[0]));
			}
			else if (Mod == 2)
			{
				lc_Tmp[0] = *Data++;
				lc_Tmp[1] = *Data++;
				char lc_chs[] = {
					EncodeTable[(lc_Tmp[0] & 0xFC) >> 2],
					EncodeTable[((lc_Tmp[0] & 0x03) << 4) | ((lc_Tmp[1] & 0xF0) >> 4)],
					EncodeTable[((lc_Tmp[1] & 0x0F) << 2)],
					'='
				};
				_out->append(lc_chs, sizeof(lc_chs) / sizeof(lc_chs[0]));
			}
		}
		static bool Decode(std::string *_out, const char* Data, int DataSize)
		{
			//解码表
			static const char DecodeTable[] =
			{
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				62, // '+'
				0, 0, 0,
				63, // '/'
				52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
				0, 0, 0, 0, 0, 0, 0,
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
				13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
				0, 0, 0, 0, 0, 0,
				26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
				39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
			};
			_out->clear();
			if (Data == NULL || DataSize <= 0)
			{
				Data = NULL;
				DataSize = 0;
			}
			for (int i = DataSize; i--;)
			{
				if (((const unsigned char*)Data)[i] >= sizeof(DecodeTable) / sizeof(DecodeTable[0]))
					return false;
			}
			//返回值
			if (DataSize > 0)
				_out->reserve(DataSize * 3 / 4);
			for (int i = 0; i < DataSize;)
			{
				if (*Data != '\r' && *Data != '\n')
				{
					std::int32_t nValue = ((std::int32_t)DecodeTable[(*Data++)]) << 18;
					nValue += ((std::int32_t)DecodeTable[*Data++]) << 12;
					char lc_buff_[4];
					char *lc_pos_ptr = &lc_buff_[0];
					*(lc_pos_ptr++) = (nValue & 0x00FF0000) >> 16;
					if (*Data != '=')
					{
						nValue += DecodeTable[*Data++] << 6;
						*(lc_pos_ptr++) = (nValue & 0x0000FF00) >> 8;
						if (*Data != '=')
						{
							nValue += DecodeTable[*Data++];
							*(lc_pos_ptr++) = nValue & 0x000000FF;
						}
					}
					i += 4;
					_out->append(lc_buff_, lc_pos_ptr);
				}
				else// 回车换行,跳过
				{
					Data++;
					i++;
				}
			}
			return true;
		}
	};

	//--------------------------------------_MyUrlCode_--------------------------------------//
	class _MyUrlCode_
	{
	public:
		static void urlEncode(std::string *_dest, const std::string &_src)
		{
			const char* hex = "0123456789ABCDEF";
			_dest->clear();
			for (size_t i = 0; i < _src.size(); ++i)
			{
				unsigned char cc = (unsigned char)_src[i];
				if (((char)cc >= 'A' && (char)cc <= 'Z')
					|| ((char)cc >= 'a' && (char)cc <= 'z')
					|| ((char)cc >= '0' && (char)cc <= '9')
					|| (char)cc == '.'
					|| (char)cc == '_'
					|| (char)cc == '-'
					|| (char)cc == '*')
				{
					if (cc == ' ')
					{
						_dest->append("+", 1);
					}
					else
					{
						_dest->append((char*)&cc, 1);
					}
				}
				else
				{
					unsigned char c = static_cast<unsigned char>(_src[i]);
					_dest->append("%", 1);
					_dest->append(&hex[c / 16], 1);
					_dest->append(&hex[c % 16], 1);
				}
			}
		}
		static void urlDecode(std::string *_dest, const std::string &_src)
		{
			int hex = 0;
			for (size_t i = 0; i < _src.length(); ++i)
			{
				switch (_src[i])
				{
				case '+':
					_dest->append(" ");
					break;
				case '%':
					if (std::isxdigit(_src[i + 1]) && std::isxdigit(_src[i + 2]))
					{
						std::string hexStr = _src.substr(i + 1, 2);
						hex = strtol(hexStr.c_str(), 0, 16);
						if (!((hex >= 48 && hex <= 57) ||
							(hex >= 97 && hex <= 122) ||
							(hex >= 65 && hex <= 90) ||
							hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29
							|| hex == 0x2a || hex == 0x2b || hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f
							|| hex == 0x3A || hex == 0x3B || hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f
							))
						{
							char lc_ch[2] = { (char)(hex) };
							_dest->append(lc_ch, 1);
							i += 2;
						}
						else _dest->append("%", 1);
					}
					else {
						_dest->append("%", 1);
					}
					break;
				default:
					_dest->append(&_src[i], 1);
					break;
				}
			}
		}
	};

	//--------------------------------------CStdlib--------------------------------------//
	class CStdlib : public CStdlibBase
	{
	public:
		static void aes128Encode(std::string *_outData, const unsigned char(&_aes_psw)[16], const void *_data, int _size)
		{
			My_Aes128_::encode(_outData, _aes_psw, _data, _size);
		}
		static void aes128Decode(std::string *_outData, const unsigned char(&_aes_psw)[16], const void *_data, int _size)
		{
			My_Aes128_::decode(_outData, _aes_psw, _data, _size);
		}
		static std::string calcMd5(const void* _data, int _size)
		{
			return My_Md5_::generateMd5(_data, _size);
		}
		template<class T_DATA>
		static std::string calcMd5(const T_DATA &_data)
		{
			return My_Md5_::generateMd5(_data.c_str(), _data.size());
		}
		static void calcMd5(unsigned char(&_md5)[16], const void* _data, int _size)
		{
			My_Md5_::generateMd5(_md5, _data, _size);
		}
		template<class T_DATA>
		static void calcMd5(unsigned char(&_md5)[16], const T_DATA &_data)
		{
			My_Md5_::generateMd5(_md5, (const uint8_t*)_data.data(), _data.size());
		}
		static void urlEncode(std::string *_out, const std::string& _src)
		{
			_MyUrlCode_::urlEncode(_out, _src);
		}
		static void urlDecode(std::string *_out, const std::string& _src)
		{
			_MyUrlCode_::urlDecode(_out, _src);
		}
		static void base64Encode(std::string *_out, const void *_src_data, int _src_size, bool _bNewLine)
		{
			My_Base64_::Encode(_out, (const unsigned char*)_src_data, _src_size, _bNewLine);
		}
		static bool base64Decode(std::string *_out, const void *_src_data, int _src_size)
		{
			return My_Base64_::Decode(_out, (const char*)_src_data, _src_size);
		}
		template<class TFUNC>
		static void executeOnce(const TFUNC &_func)
		{
			class T_FUNC_OBJ { public: T_FUNC_OBJ(const TFUNC &_func) { _func(); } };
			static T_FUNC_OBJ lc_mm(_func); TX_UNUSED(lc_mm);
		}
		static void initEnv()
		{
			CStdlibBase::initEnv();
			executeOnce([]() {
				_TxPrivateNs_::My_Aes128_::initEnv();
				_TxPrivateNs_::My_Md5_::initEnv();
			});
		}
	};
}

//--------------------------------------TxThreadSafeVariant & TxThreadSafeBoolean--------------------------------------//
template<class T>
class TXSTDALIGNAS_16 TxThreadSafeVariant
{
private:
	size_t _tem1[2];
	TXSTDALIGNAS_16 T value;
	size_t _tem2[1];
public:
	TxThreadSafeVariant(T _value = 0)
	{
		this->_tem1[0] = this->_tem1[1] = this->_tem2[0] = 0;
		this->value = _value;
	}
	inline const TxThreadSafeVariant& operator=(T _value)
	{
		this->value = _value;
		return *this;
	}
	inline const TxThreadSafeVariant& operator=(const TxThreadSafeVariant &_tem)
	{
		this->value = _tem.value;
		return *this;
	}
	inline operator T () const
	{
		return this->value;
	}
};
typedef TxThreadSafeVariant<bool> TxThreadSafeBoolean;


//--------------------------------------TxMemoryPool--------------------------------------//

namespace _TxPrivateNs_ {
	namespace _NsPrvMemoryPool_ {
		struct _MyPrvMemoryPool_;
		enum { _Enum_MyPrvMemoryPool_ALIGN = 16 };
		typedef struct TXSTDALIGNAS_16 _tagMemoryPoolElemNode__ {
			struct _tagMemoryPoolElemNode__ *next;
			struct _MyPrvMemoryPool_ *host;
		} _TMemoryPoolElemNode__;
		struct MyPrvMemoryPoolSetBase
		{
		public:
			struct _MyPrvMemoryPool_ *listUser;//可用链表
			struct _MyPrvMemoryPool_ *listFull;//完全使用的链表
			struct _MyPrvMemoryPool_ *listEmpty;//不可用链表
			const size_t iElemSize;
			size_t iTotalValidCount;
		public:
			virtual void* vfMalloc() = 0;
			virtual void vfFree(_TMemoryPoolElemNode__ *_elemPtr, _MyPrvMemoryPool_ *_pMemoryPool) = 0;
			virtual void vfClearDirty() = 0;
		public:
			MyPrvMemoryPoolSetBase(size_t _iElemSize) :iElemSize(_iElemSize)
			{
				this->listEmpty = NULL;
				this->listFull = NULL;
				this->listUser = NULL;
				this->iTotalValidCount = 0;
			}
			~MyPrvMemoryPoolSetBase()
			{
				assert(this->listEmpty == NULL);
				assert(this->listFull == NULL);
				assert(this->listUser == NULL);
				assert(this->iTotalValidCount == 0);
			}
		};
		struct TXSTDALIGNAS_16 _MyPrvMemoryPool_
		{
			struct _MyPrvMemoryPool_ *next;
			struct _MyPrvMemoryPool_ *prev;
			_TMemoryPoolElemNode__ *listMemory;//可用内存链表
			_TMemoryPoolElemNode__ *bufferBgn;//申请内存块开始地址
			_TMemoryPoolElemNode__ *bufferEnd;//申请内存块结束地址
			_TMemoryPoolElemNode__ *bufferPos;//申请内存块可用地址
			MyPrvMemoryPoolSetBase *pPrvMemoryPoolSetBase;
			size_t iMallocFreeSum;//申请和释放内存计数器，用于检测可用内存是否满的
		public:
			void* malloc(size_t _elemSize)
			{
				void *ret = NULL;
				if (this->bufferPos != this->bufferEnd)
				{
					ret = this->bufferPos + 1;
					this->bufferPos->host = this;
					this->bufferPos->next = NULL;
					this->bufferPos = (_TMemoryPoolElemNode__*)((unsigned char*)this->bufferPos + (_elemSize + sizeof(_TMemoryPoolElemNode__)));
				}
				else if (this->listMemory != NULL)
				{
					ret = ((_TMemoryPoolElemNode__*)this->listMemory) + 1;
					this->listMemory->host = this;
					this->listMemory = this->listMemory->next;
				}
				else
				{
					assert(false);
				}
				if (ret != NULL)
				{
					++this->iMallocFreeSum;
					--pPrvMemoryPoolSetBase->iTotalValidCount;
#ifndef NDEBUG
					assert(_elemSize % sizeof(size_t) == 0);
					for (int i = 0; i < (int)(_elemSize / sizeof(size_t)); i++)
						((size_t*)ret)[i] = (size_t)i;
#endif
				}
				return ret;
			}
			inline bool isFull() const
			{
				assert((int)this->iMallocFreeSum >= 0);
				return this->iMallocFreeSum == 0;
			}
			inline bool isEmpty() const
			{
				assert((int)this->iMallocFreeSum >= 0);
				return this->listMemory == NULL&&this->bufferPos == this->bufferEnd;
			}
			static void moveHeadNode(struct _MyPrvMemoryPool_ **_fromListNode, struct _MyPrvMemoryPool_ **_toListHead)
			{
				struct _MyPrvMemoryPool_ *lc_tmp = (*_fromListNode);
				assert(lc_tmp != NULL);
				struct _MyPrvMemoryPool_ *lc_tmp_1 = (*_fromListNode) = lc_tmp->next;
				if (lc_tmp->prev != NULL)
					lc_tmp->prev->next = lc_tmp_1;
				if (lc_tmp_1 != NULL)
					lc_tmp_1->prev = lc_tmp->prev;

				lc_tmp_1 = lc_tmp->next = (*_toListHead);
				lc_tmp->prev = NULL;
				if (lc_tmp_1 != NULL)
					lc_tmp_1->prev = lc_tmp;
				(*_toListHead) = lc_tmp;
			}
		};
		template<int ELEM_SIZE, int ELEM_COUNT>
		class TXSTDALIGNAS_16 __MyPrvMemoryPoolSet_ : public MyPrvMemoryPoolSetBase
		{
		public:
			enum {
				ELEM_REAL_SIZE = ((ELEM_SIZE + _Enum_MyPrvMemoryPool_ALIGN - 1) / _Enum_MyPrvMemoryPool_ALIGN)*_Enum_MyPrvMemoryPool_ALIGN,
				ELEM_REAL_COUNT = ((ELEM_COUNT + _Enum_MyPrvMemoryPool_ALIGN - 1) / _Enum_MyPrvMemoryPool_ALIGN)*_Enum_MyPrvMemoryPool_ALIGN,
				THIS_SELF_SIZE = ((_Enum_MyPrvMemoryPool_ALIGN - 1 + sizeof(_MyPrvMemoryPool_)) / _Enum_MyPrvMemoryPool_ALIGN) * _Enum_MyPrvMemoryPool_ALIGN,
				ELEM_BUFFER_SIZE = THIS_SELF_SIZE + ELEM_REAL_COUNT * (ELEM_REAL_SIZE + sizeof(_TMemoryPoolElemNode__)),
			};
		private:
			struct _MyPrvMemoryPool_* newMemoryPool()
			{
				_MyPrvMemoryPool_ *ret;
				ret = (_MyPrvMemoryPool_*)::operator new(ELEM_BUFFER_SIZE);

				ret->listMemory = NULL;
				ret->bufferBgn = (_TMemoryPoolElemNode__*)(((unsigned char*)ret) + THIS_SELF_SIZE);
				ret->bufferEnd = (_TMemoryPoolElemNode__*)((unsigned char*)ret->bufferBgn
					+ ELEM_REAL_COUNT*(ELEM_REAL_SIZE + sizeof(_TMemoryPoolElemNode__)));
				ret->bufferPos = ret->bufferBgn;
				ret->iMallocFreeSum = 0;
				ret->pPrvMemoryPoolSetBase = this;
				ret->pPrvMemoryPoolSetBase->iTotalValidCount += ELEM_REAL_COUNT;
				ret->next = NULL;
				ret->prev = NULL;
				return ret;
			}
			void releaseMemoryPool(_MyPrvMemoryPool_ *_ptr)
			{
				::operator delete((void*)_ptr);
			}
		public:
			__MyPrvMemoryPoolSet_() :MyPrvMemoryPoolSetBase(ELEM_SIZE)
			{
				//this->p_init_buffer_ = (void*)this->init_buffer_;
				assert((sizeof(_TMemoryPoolElemNode__)) % 16 == 0 || (sizeof(_TMemoryPoolElemNode__)) == 2 * sizeof(void*));
			}
			~__MyPrvMemoryPoolSet_()
			{
				struct _MyPrvMemoryPool_ **lc_arryPool[] = {
					&this->listEmpty,
					&this->listFull,
					&this->listUser,
				};
				assert(this->listUser == NULL&&this->listEmpty == NULL);
				for (int i = CStdlibBase::arrayLength(lc_arryPool); i--;)
				{
					_MyPrvMemoryPool_ *lc_node = *lc_arryPool[i];
					while (lc_node != NULL)
					{
						_MyPrvMemoryPool_ *lc_tmp = lc_node->next;
						this->releaseMemoryPool(lc_node);
						lc_node = lc_tmp;
					}
					*lc_arryPool[i] = NULL;
				}
			}
			virtual void* vfMalloc()
			{
				void *ret = NULL;
				if (this->listUser != NULL)
				{
					ret = this->listUser->malloc(ELEM_REAL_SIZE);
					if (this->listUser->isEmpty())
						_MyPrvMemoryPool_::moveHeadNode(&this->listUser, &this->listEmpty);
				}
				else if (this->listFull != NULL)
				{
					_MyPrvMemoryPool_::moveHeadNode(&this->listFull, &this->listUser);
					return this->vfMalloc();
				}
				else
				{
					this->listUser = this->newMemoryPool();
					this->listUser->prev = NULL;
					this->listUser->next = NULL;
					return this->vfMalloc();
				}
				return ret;
			}
			template<int MAX_COUNT>
			void _clearByValidCount__()
			{
				while (this->iTotalValidCount >= MAX_COUNT&&this->listFull != NULL)
				{
					struct _MyPrvMemoryPool_ *lc_tmp = this->listFull;
					this->listFull = this->listFull->next;
					if (this->listFull != NULL)
						this->listFull->prev = NULL;
					this->releaseMemoryPool(lc_tmp);
					this->iTotalValidCount -= ELEM_REAL_COUNT;
				}
			}
			virtual void vfClearDirty()
			{
				return this->_clearByValidCount__<ELEM_REAL_COUNT>();
			}
			virtual void vfFree(_TMemoryPoolElemNode__ *_elemPtr, _MyPrvMemoryPool_ *_pMemoryPool)
			{
				--_pMemoryPool->iMallocFreeSum;
				++this->iTotalValidCount;

				if (_pMemoryPool->isEmpty())
				{
					if (this->listEmpty == _pMemoryPool)
					{
						_MyPrvMemoryPool_::moveHeadNode(&this->listEmpty, &this->listUser);
					}
					else
					{
						_MyPrvMemoryPool_ *pThisTmp = _pMemoryPool;
						_MyPrvMemoryPool_::moveHeadNode(&pThisTmp, &this->listUser);
					}
				}
				else if (_pMemoryPool->iMallocFreeSum == 0)
				{
					if (this->listEmpty == _elemPtr->host)
						this->listEmpty = NULL;
					else if (this->listUser == _elemPtr->host)
						this->listUser = NULL;
					else if (this->listFull == _elemPtr->host)
						this->listFull = NULL;
					_MyPrvMemoryPool_ *pThisTmp = _pMemoryPool;
					_MyPrvMemoryPool_::moveHeadNode(&pThisTmp, &this->listFull);
				}

				_elemPtr->next = _pMemoryPool->listMemory;
				_pMemoryPool->listMemory = _elemPtr;

				assert((int)_pMemoryPool->iMallocFreeSum >= 0);
				this->_clearByValidCount__<2 * ELEM_REAL_COUNT>();//此处清理有可能把_pMemoryPool对象清除掉
			}
		};
		template<class MUTEX_TYPE>
		class TXSTDALIGNAS_16 _tagPrvMemoryMgr_
		{
		private:
			template<int ELEM_SIZE> struct TXSTDALIGNAS_16 _CPoolSet_Base_
				: public __MyPrvMemoryPoolSet_<ELEM_SIZE, ((ELEM_SIZE >= 1024 ? 2 : 1) * 1024 * 1024 / ELEM_SIZE)> { };
			template<int N1, int N2> struct TXSTDALIGNAS_16 _CPoolSet_
				: public _CPoolSet_Base_ <(((1 << N1) + (1 << N2)) >> 1)> {};
		private:
			struct TXSTDALIGNAS_16 _tagPoolSetAlc {
				TXSTDALIGNAS_16 _CPoolSet_<3, 3> _m0;
				TXSTDALIGNAS_16 _CPoolSet_<4, 4> _m1;
				TXSTDALIGNAS_16 _CPoolSet_<4, 5> _m2;
				TXSTDALIGNAS_16 _CPoolSet_<5, 5> _m3;
				TXSTDALIGNAS_16 _CPoolSet_<5, 6> _m4;
				TXSTDALIGNAS_16 _CPoolSet_<6, 6> _m5;
				TXSTDALIGNAS_16 _CPoolSet_<6, 7> _m6;
				TXSTDALIGNAS_16 _CPoolSet_<7, 7> _m7;
				TXSTDALIGNAS_16 _CPoolSet_<7, 8> _m8;
				TXSTDALIGNAS_16 _CPoolSet_<8, 8> _m9;
				TXSTDALIGNAS_16 _CPoolSet_<8, 9> _m10;
				TXSTDALIGNAS_16 _CPoolSet_<9, 9> _m11;
				TXSTDALIGNAS_16 _CPoolSet_<9, 10> _m12;
				TXSTDALIGNAS_16 _CPoolSet_<10, 10> _m13;
				TXSTDALIGNAS_16 _CPoolSet_<10, 11> _m14;
				TXSTDALIGNAS_16 _CPoolSet_<11, 11> _m15;
				TXSTDALIGNAS_16 _CPoolSet_<11, 12> _m16;
				TXSTDALIGNAS_16 _CPoolSet_<12, 12> _m17;
				TXSTDALIGNAS_16 _CPoolSet_<12, 13> _m18;
				TXSTDALIGNAS_16 _CPoolSet_<13, 13> _m19;
			} _mPoolSetAlc;
			TXSTDALIGNAS_16 MyPrvMemoryPoolSetBase* arryPoolSet[20];
			//__MyPrvMemoryPoolSet_<E_MIN_SIZE, ((sizeof(void*)<=4?1:12) * 1024 * 1024 / E_MIN_SIZE)> minPoolMgr;
			//__MyPrvMemoryPoolSet_<E_MAX_SIZE, ((sizeof(void*) <= 4 ? 1 : 4) * 1024 * 1024 / E_MAX_SIZE)> maxPoolMgr;
			TXSTDALIGNAS_16 MUTEX_TYPE mMutex;
		public:
			_tagPrvMemoryMgr_()
			{
				MyPrvMemoryPoolSetBase* lcArryMemPoolSet[] = {
					&this->_mPoolSetAlc._m0,
					&this->_mPoolSetAlc._m1,
					&this->_mPoolSetAlc._m2,
					&this->_mPoolSetAlc._m3,
					&this->_mPoolSetAlc._m4,
					&this->_mPoolSetAlc._m5,
					&this->_mPoolSetAlc._m6,
					&this->_mPoolSetAlc._m7,
					&this->_mPoolSetAlc._m8,
					&this->_mPoolSetAlc._m9,
					&this->_mPoolSetAlc._m10,
					&this->_mPoolSetAlc._m11,
					&this->_mPoolSetAlc._m12,
					&this->_mPoolSetAlc._m13,
					&this->_mPoolSetAlc._m14,
					&this->_mPoolSetAlc._m15,
					&this->_mPoolSetAlc._m16,
					&this->_mPoolSetAlc._m17,
					&this->_mPoolSetAlc._m18,
					&this->_mPoolSetAlc._m19
				};
				static_assert(sizeof(this->arryPoolSet) == sizeof(lcArryMemPoolSet), "");
				memcpy(this->arryPoolSet, lcArryMemPoolSet, sizeof(this->arryPoolSet));
			}
			~_tagPrvMemoryMgr_()
			{
				this->clearDirty();
			}
			void clearDirty()
			{
				this->mMutex.lock();
				for (int i = 0; i < CStdlibBase::arrayLength(this->arryPoolSet); ++i)
					this->arryPoolSet[i]->vfClearDirty();
				this->mMutex.unlock();
			}
			void* malloc(size_t _size, size_t *_maxSize)
			{
				int lcL = 0, lcR = CStdlibBase::arrayLength(this->arryPoolSet);
				while (lcL < lcR)
				{
					int k = (lcL + lcR) >> 1;
					if (this->arryPoolSet[k]->iElemSize < _size)
						lcL = k + 1;
					else lcR = k;
				}
				void *ret = NULL;
				if (lcL >= CStdlibBase::arrayLength(this->arryPoolSet))
				{
					_TMemoryPoolElemNode__ *lcMemoryPoolElemNode = (_TMemoryPoolElemNode__*)
						(::operator new(_size + sizeof(_TMemoryPoolElemNode__)));
					lcMemoryPoolElemNode->host = NULL;
					lcMemoryPoolElemNode->next = NULL;
					ret = lcMemoryPoolElemNode + 1;
					if (_maxSize != NULL)
						*_maxSize = _size;
				}
				else
				{
					this->mMutex.lock();
					ret = this->arryPoolSet[lcL]->vfMalloc();
					this->mMutex.unlock();
					if (_maxSize != NULL)
						*_maxSize = this->arryPoolSet[lcL]->iElemSize;
				}
				return ret;
			}
			void free(void *_ptr)
			{
				_TMemoryPoolElemNode__ *lc_elemPtr = (_TMemoryPoolElemNode__*)(
					(unsigned char*)_ptr - sizeof(_TMemoryPoolElemNode__));
				_MyPrvMemoryPool_ *lc_pMemoryPool = lc_elemPtr->host;
				if (lc_pMemoryPool == NULL)
					::operator delete((void*)lc_elemPtr);
				else
				{
					this->mMutex.lock();
					lc_pMemoryPool->pPrvMemoryPoolSetBase->vfFree(lc_elemPtr, lc_pMemoryPool);
					this->mMutex.unlock();
				}
			}
		};
	}
}
template<class MUTEX_TYPE>
class TxMemoryPool
{
private:
	_TxPrivateNs_::_NsPrvMemoryPool_::_tagPrvMemoryMgr_<MUTEX_TYPE> mPrvMemoryMgr;
public:
	TxMemoryPool()
	{
	}
	~TxMemoryPool()
	{
	}
	void* malloc(size_t _size, size_t *_maxSize = NULL)
	{
		return this->mPrvMemoryMgr.malloc(_size, _maxSize);
	}
	void free(void *_ptr)
	{
		return this->mPrvMemoryMgr.free(_ptr);
	}
	void clearDirty()
	{
		return this->mPrvMemoryMgr.clearDirty();
	}
};


//--------------------------------------TxObjectLifePeriod--------------------------------------//
class TxObjectLifePeriod
{
public:
	class TxObjectEntity
	{
		friend class TxObjectLifePeriod;
	private:
		std::shared_ptr<size_t> sp_valid;
	public:
		TxObjectEntity() :sp_valid(std::make_shared<size_t>()) { *this->sp_valid = 1; }
		~TxObjectEntity() { }
		operator bool() const { return this->sp_valid&&*this->sp_valid == 1; }
		void destroy() { *this->sp_valid = 0; }
	};
private:
	std::weak_ptr<size_t> wp_valid;
public:
	TxObjectLifePeriod()
	{
	}
	TxObjectLifePeriod(TxObjectLifePeriod::TxObjectEntity *_entity) :wp_valid(_entity->sp_valid)
	{
	}
	void destroy()
	{
		std::shared_ptr<size_t> lc_sp_valid = this->wp_valid.lock();
		if (lc_sp_valid)
			*lc_sp_valid = 0;
	}
	operator bool() const
	{
		std::shared_ptr<size_t> lc_sp_valid = this->wp_valid.lock();
		if (!lc_sp_valid)
			return false;
		if ((*lc_sp_valid) != 1)
			return false;
		return true;
	}
};


//--------------------------------------TxBlobString & TxBlobWString--------------------------------------//
namespace _TxPrivateNs_
{
	template<int NSIZE> struct _TSizeSignedInt_ { };
	template<> struct _TSizeSignedInt_<1> { typedef std::int8_t type; };
	template<> struct _TSizeSignedInt_<2> { typedef std::int16_t type; };
	template<> struct _TSizeSignedInt_<4> { typedef std::int32_t type; };
	template<> struct _TSizeSignedInt_<8> { typedef std::int64_t type; };

	class _CTX_UTF8_TRANS_ : public std::codecvt_utf8<wchar_t>
	{
	private:
		unsigned char buffer_state_[sizeof(state_type) + sizeof(size_t)];
		state_type *p_state;
	public:
		_CTX_UTF8_TRANS_()
		{
			this->p_state = NULL;
			::memset(this->buffer_state_, 0, sizeof(this->buffer_state_));
			new ((state_type*)this->buffer_state_) state_type();
			this->p_state = (state_type*)this->buffer_state_;
		}
		~_CTX_UTF8_TRANS_()
		{
			this->p_state->~state_type();
			this->p_state = NULL;
		}
		static const _CTX_UTF8_TRANS_* _object_()
		{
			static _CTX_UTF8_TRANS_ ret;
			return &ret;
		}
		inline const state_type& get_empty_codecvt_state_() const
		{
			return *this->p_state;
		}
		result _trans_(state_type &_state,
			const char* _from, const char* _from_end, const char*& _from_next,
			wchar_t* _to, wchar_t* _to_end, wchar_t*& _to_next) const
		{
			return this->in(_state, _from, _from_end, _from_next, _to, _to_end, _to_next);
		}
		result _trans_(state_type &_state,
			const wchar_t* _from, const wchar_t* _from_end, const wchar_t*& _from_next,
			char* _to, char* _to_end, char*& _to_next) const
		{
			return this->out(_state, _from, _from_end, _from_next, _to, _to_end, _to_next);
		}
	};

	template<class T_BLOBSTR>
	class MyComposeParameter_
	{
	private:
		struct {
			unsigned char __buf[sizeof(T_BLOBSTR)];
		} aListString[10];
		std::function<void(const T_BLOBSTR&)> cb_func;
		T_BLOBSTR sFormat;
		int argsCount;
	public:
		MyComposeParameter_(const T_BLOBSTR &_fmt, const std::function<void(const T_BLOBSTR&)> &_cb_func)
		{
			this->cb_func = _cb_func;
			this->argsCount = 0;
			this->sFormat.assign(_fmt);
		}
		~MyComposeParameter_()
		{
			T_BLOBSTR lc_result_str;
			if (this->argsCount <= 0)
				lc_result_str = this->sFormat;
			else
			{
				auto lc_pStrPos = this->sFormat.c_str();
				auto lc_pStrEnd = this->sFormat.c_str() + this->sFormat.length();
				auto lc_pRecordCpyStart = lc_pStrPos; lc_pRecordCpyStart = NULL;
				for (;;)
				{
					if (lc_pStrPos + 2 < lc_pStrEnd&&lc_pStrPos[0] == '{'&&lc_pStrPos[2] == '}'
						&& '0' <= (int)lc_pStrPos[1] && (int)lc_pStrPos[1] < this->argsCount + '0')
					{
						if (lc_pRecordCpyStart != NULL)
						{
							lc_result_str.append(lc_pRecordCpyStart, (int)(lc_pStrPos - lc_pRecordCpyStart));
							lc_pRecordCpyStart = NULL;
						}
						lc_result_str.append(*(T_BLOBSTR*)&this->aListString[(unsigned int)lc_pStrPos[1] - '0']);
						lc_pStrPos += 3;
					}
					else if (lc_pStrPos >= lc_pStrEnd)
					{
						if (lc_pRecordCpyStart != NULL)
						{
							lc_result_str.append(lc_pRecordCpyStart, (int)(lc_pStrPos - lc_pRecordCpyStart));
							lc_pRecordCpyStart = NULL;
						}
						break;
					}
					else
					{
						if (lc_pRecordCpyStart == NULL)
							lc_pRecordCpyStart = lc_pStrPos;
						++lc_pStrPos;
					}
				}
			}
			this->cb_func(lc_result_str);
			for (int i = this->argsCount; i--;)
				((T_BLOBSTR*)&this->aListString[i])->~T_BLOBSTR();
		}
		void addParameter(const T_BLOBSTR &_arg)
		{
			if (this->argsCount < (int)(sizeof(this->aListString) / sizeof(this->aListString[0])))
				new ((T_BLOBSTR*)&this->aListString[this->argsCount++]) T_BLOBSTR(_arg);
			else assert(0);
		}
	};

	template<class T_BLOBSTR>
	class MyComposeTemporaryObjects_
	{
	private:
		std::shared_ptr<MyComposeParameter_<T_BLOBSTR>> spMyComposeParameter;
	public:
		MyComposeTemporaryObjects_(const std::shared_ptr<MyComposeParameter_<T_BLOBSTR>> &_spMyComposeParameter)
			:spMyComposeParameter(_spMyComposeParameter)
		{
		}
		template<class T_ARG>
		MyComposeTemporaryObjects_& operator%(const T_ARG &_arg)
		{
			this->spMyComposeParameter->addParameter(T_BLOBSTR::fromDatum(_arg));
			return *this;
		}
		MyComposeTemporaryObjects_& args()
		{
			return (*this);
		}
		template<class T0>
		MyComposeTemporaryObjects_& args(const T0 &_a0)
		{
			return (*this) % _a0;
		}
		template<class T0, class T1>
		MyComposeTemporaryObjects_& args(const T0 &_a0, const T1 &_a1)
		{
			return (*this) % _a0%_a1;
		}
		template<class T0, class T1, class T2>
		MyComposeTemporaryObjects_& args(const T0 &_a0, const T1 &_a1, const T2 &_a2)
		{
			return (*this) % _a0%_a1%_a2;
		}
		template<class T0, class T1, class T2, class T3>
		MyComposeTemporaryObjects_& args(const T0 &_a0, const T1 &_a1, const T2 &_a2, const T3 &_a3)
		{
			return (*this) % _a0%_a1%_a2%_a3;
		}
		template<class T0, class T1, class T2, class T3, class T4>
		MyComposeTemporaryObjects_& args(const T0 &_a0, const T1 &_a1, const T2 &_a2, const T3 &_a3, const T4 &_a4)
		{
			return (*this) % _a0%_a1%_a2%_a3%_a4;
		}
		template<class T0, class T1, class T2, class T3, class T4, class T5>
		MyComposeTemporaryObjects_& args(const T0 &_a0, const T1 &_a1, const T2 &_a2, const T3 &_a3, const T4 &_a4,
			const T5 &_a5)
		{
			return (*this) % _a0%_a1%_a2%_a3%_a4%_a5;
		}
		template<class T0, class T1, class T2, class T3, class T4, class T5, class T6>
		MyComposeTemporaryObjects_& args(const T0 &_a0, const T1 &_a1, const T2 &_a2, const T3 &_a3, const T4 &_a4,
			const T5 &_a5, const T6 &_a6)
		{
			return (*this) % _a0%_a1%_a2%_a3%_a4%_a5%_a6;
		}
		template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
		MyComposeTemporaryObjects_& args(const T0 &_a0, const T1 &_a1, const T2 &_a2, const T3 &_a3, const T4 &_a4,
			const T5 &_a5, const T6 &_a6, const T7 &_a7)
		{
			return (*this) % _a0%_a1%_a2%_a3%_a4%_a5%_a6%_a7;
		}
		template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
		MyComposeTemporaryObjects_& args(const T0 &_a0, const T1 &_a1, const T2 &_a2, const T3 &_a3, const T4 &_a4,
			const T5 &_a5, const T6 &_a6, const T7 &_a7, const T8 &_a8)
		{
			return (*this) % _a0%_a1%_a2%_a3%_a4%_a5%_a6%_a7%_a8;
		}
		template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
		MyComposeTemporaryObjects_& args(const T0 &_a0, const T1 &_a1, const T2 &_a2, const T3 &_a3, const T4 &_a4,
			const T5 &_a5, const T6 &_a6, const T7 &_a7, const T8 &_a8, const T9 &_a9)
		{
			return (*this) % _a0%_a1%_a2%_a3%_a4%_a5%_a6%_a7%_a8%_a9;
		}
	};

	template<class TypeChar, class TypeOpsChar,
		class TPeerTypeChar, class TPeerTypeOpsChar>
		class TxBlobTmplString
	{
	public:
		typedef MyComposeTemporaryObjects_<TxBlobTmplString> TxComposeParameter;
	private:
		typedef TxBlobTmplString<TPeerTypeChar, TPeerTypeOpsChar, TypeChar, TypeOpsChar> PeerStrongTmplStringType;
		typedef std::basic_string<TypeChar> TypeStdString;
		typedef std::basic_string<TPeerTypeChar> TPeerTypeStdString;
		friend class TxBlobTmplString<TPeerTypeChar, TPeerTypeOpsChar, TypeChar, TypeOpsChar>;
		enum { e_object_size = 64, e_tss_base_size = (e_object_size - 1 * sizeof(void*) - 2 * sizeof(int)) / (sizeof(TypeChar)), };
		TypeChar *dataPtr;
		int iLength;
		int iBuffMaxSize;
		TypeChar _mem_buff_temp_[e_tss_base_size];
	protected:
		inline static int __my_str_len(const char _str[])
		{
			if (_str == NULL || _str[0] == 0) return 0;
			return (int)::strlen(_str);
		}
		inline static int __my_str_len(const wchar_t _str[])
		{
			if (_str == NULL || _str[0] == 0) return 0;
			return (int)::wcslen(_str);
		}
		inline static char* __my_mem_chr(const char _str[], size_t _count, char _val)
		{
			return (char*)::memchr(_str, _val, _count);
		}
		inline static wchar_t* __my_mem_chr(const wchar_t _str[], size_t _count, wchar_t _val)
		{
			return (wchar_t*)::wmemchr(_str, _val, _count);
		}
		inline static void __my_mem_set(char _str[], size_t _count, char _val)
		{
			::memset(_str, (char)_val, _count);
		}
		inline static void __my_mem_set(wchar_t _str[], size_t _count, wchar_t _val)
		{
			::wmemset(_str, _val, _count);
		}
		inline static void __my_mem_cpy(char* _dest, const char* _src, size_t _count)
		{
			::memcpy(_dest, _src, _count * sizeof(_src[0]));
		}
		inline static void __my_mem_cpy(unsigned char* _dest, const unsigned char* _src, size_t _count)
		{
			::memcpy(_dest, _src, _count * sizeof(_src[0]));
		}
		inline static void __my_mem_cpy(wchar_t* _dest, const wchar_t* _src, size_t _count)
		{
			::memcpy(_dest, _src, _count * sizeof(_src[0]));
		}
		inline static int __my_vsn_printf(char *_dest, int _buf_size, const char* _format, va_list _vlist)
		{
			return ::vsnprintf(_dest, _buf_size, _format, _vlist);
		}
		inline static int __my_vsn_printf(wchar_t *_dest, int _buf_size, const wchar_t* _format, va_list _vlist)
		{
			return ::vswprintf(_dest, _buf_size, _format, _vlist);
		}
		inline static const char* __my_mem_chr(const char *_str, char _ch, int _iLength)
		{
			return (const char*)::memchr(_str, _ch, _iLength);
		}
		inline static const wchar_t* __my_mem_chr(const wchar_t *_str, wchar_t _ch, int _iLength)
		{
			return (const wchar_t*)::wmemchr(_str, _ch, _iLength);
		}
		inline void __constructor_init__()
		{
			class T_OBJ_ {
			private: char _buf_tem_[(true
				&& sizeof(TypeChar) == sizeof(TypeOpsChar)
				&& sizeof(TPeerTypeChar) == sizeof(TPeerTypeOpsChar)
				&& (sizeof(TypeChar) == sizeof(char) || sizeof(TypeChar) == sizeof(wchar_t))
				&& (sizeof(TPeerTypeChar) == sizeof(char) || sizeof(TPeerTypeOpsChar) == sizeof(wchar_t))
				&& sizeof(TypeChar) != sizeof(TPeerTypeChar)
				&& sizeof(TxBlobTmplString) == e_object_size
				&& (std::is_same<TypeStdString, std::string>::value || std::is_same<TypeStdString, std::wstring>::value)
				&& (std::is_same<TPeerTypeStdString, std::string>::value || std::is_same<TPeerTypeStdString, std::wstring>::value)
				&& true) ? sizeof(void*) : -1];
			public: T_OBJ_() { _CTX_UTF8_TRANS_::_object_(); }
			}; static T_OBJ_ _m_obj_; (void)_m_obj_;
			this->_mem_buff_temp_[0] = 0;
			this->dataPtr = this->_mem_buff_temp_;
			this->iLength = 0;
			this->iBuffMaxSize = e_tss_base_size;
		}
		void __clear__()
		{
			if (dataPtr != _mem_buff_temp_)
			{
				delete[] this->dataPtr;
				this->dataPtr = _mem_buff_temp_;
			}
			this->iLength = 0;
			this->iBuffMaxSize = e_tss_base_size;
			this->_mem_buff_temp_[0] = 0;
		}
		void __resize__(const int _iNewSize, const bool _bKeepOrgn)
		{
			if (_iNewSize <= 1)
				this->__clear__();
			else
			{
				if (_iNewSize > this->iBuffMaxSize || _iNewSize < (this->iBuffMaxSize >> 2))//重新分配内存
				{
					if (_iNewSize <= e_tss_base_size)
					{
						if (dataPtr != _mem_buff_temp_)
						{
							if (_bKeepOrgn)
								__my_mem_cpy(_mem_buff_temp_, dataPtr, TxStdlib::minimum<int>(_iNewSize, e_tss_base_size));
							delete[] this->dataPtr;
							this->dataPtr = _mem_buff_temp_;
						}
						this->iLength = _iNewSize - 1;
						this->iBuffMaxSize = e_tss_base_size;
					}
					else
					{
						int iLcBuffMaxTemSize = (this->iBuffMaxSize < _iNewSize ? this->iBuffMaxSize : e_tss_base_size);
						while ((iLcBuffMaxTemSize <<= 1) < _iNewSize);
						this->iBuffMaxSize = iLcBuffMaxTemSize;
						if (_bKeepOrgn)
						{
							TypeChar *pOld = this->dataPtr;
							this->dataPtr = new TypeChar[iLcBuffMaxTemSize];
							__my_mem_cpy(this->dataPtr, pOld, TxStdlib::minimum<int>(this->iLength + 1, _iNewSize));
							if (pOld != _mem_buff_temp_) delete[] pOld;
						}
						else
						{
							if (this->dataPtr != _mem_buff_temp_) delete[] this->dataPtr;
							this->dataPtr = new TypeChar[iLcBuffMaxTemSize];
						}
					}
				}
				this->iLength = _iNewSize - 1;
			}
		}
		inline bool _data_pointer_inside_(const void *_str) const
		{
			return (uintptr_t)this->dataPtr <= (uintptr_t)_str && (uintptr_t)_str < ((uintptr_t)(this->dataPtr + this->iBuffMaxSize));
		}
		void __assign__(const TypeChar *_str, int _iLen = -1)
		{
			if (_iLen < 0) _iLen = __my_str_len(_str);
			if (_iLen == 0 || _str == NULL)
			{
				assert(_str != NULL);
				this->__clear__();
			}
			else if (this->_data_pointer_inside_(_str))
				this->assign(TxBlobTmplString(_str, _iLen));
			else
			{
				this->__resize__(_iLen + 1, false);
				__my_mem_cpy(this->dataPtr, _str, _iLen);
				this->dataPtr[_iLen] = 0;
			}
		}
		void __append__(const TypeChar *_str, int _iLen = -1)
		{
			if (_iLen < 0) _iLen = __my_str_len(_str);
			if (this->_data_pointer_inside_(_str))
				this->append(TxBlobTmplString(_str, _iLen));
			else if (_str != NULL)
			{
				int iStartPos = this->iLength;
				this->__resize__(_iLen + this->iLength + 1, true);
				__my_mem_cpy(this->dataPtr + iStartPos, _str, _iLen);
				this->dataPtr[this->iLength] = 0;
			}
			else
			{
				assert(0);
			}
		}
		void __pop_back__(int _iDiscardCount)
		{
			if (_iDiscardCount >= this->iLength)
				this->__clear__();
			else if (_iDiscardCount > 0)
			{
				this->__resize__(this->iLength + 1 - _iDiscardCount, true);
				this->dataPtr[this->iLength] = 0;
			}
		}
		static inline bool _equal_char_no_case_(int _ch1, int _ch2)
		{
			return (('A' <= (_ch1) && (_ch1) <= 'Z') ? ((_ch1)+('a' - 'A')) : (_ch1))
				== (('A' <= (_ch2) && (_ch2) <= 'Z') ? ((_ch2)+('a' - 'A')) : (_ch2));
		}
		bool __no_case_compare_equal_(const TypeChar *_str, int _iLen = -1) const
		{
			if (_iLen < 0) _iLen = __my_str_len(_str);
			if (_iLen != this->iLength) return false;
			for (int pos = _iLen; pos--;)
			{
				if (!_equal_char_no_case_((int)this->dataPtr[pos], (int)_str[pos]))
					return false;
			}
			return true;
		}
		int __compare_value_(const TypeChar *_str, int _iLen = -1) const
		{
			if (_iLen < 0) _iLen = __my_str_len(_str);
			if (this->iLength < _iLen) return -1;
			else if (this->iLength > _iLen) return 1;
			int _ret = ::memcmp(this->dataPtr, _str, _iLen * sizeof(TypeChar));
			if (_ret < 0) return -1;
			else if (_ret > 0) return 1;
			return 0;
		}
		int _format__(int _bClearOrg, const TypeChar fmt[], va_list _args)
		{
			TypeChar buffer[384 / sizeof(TypeChar) + 4];
			TypeChar *pBuf = &buffer[0];
			pBuf[0] = 0;
			int res = __my_vsn_printf(pBuf, (sizeof(buffer) / sizeof(buffer[0])) - 4, fmt, _args);
			if (res < 0)
			{
				res = __my_vsn_printf(NULL, 0, fmt, _args);
				if (res > 0)
				{
					pBuf = new TypeChar[res + 2];
					int iRetTemp = __my_vsn_printf(pBuf, res + 1, fmt, _args);
					assert(iRetTemp == res); (void)iRetTemp;
				}
			}
			if (res < 0) res = 0;
			if (_bClearOrg) this->assign(pBuf, res);
			else this->append(pBuf, res);
			if ((void*)pBuf != (void*)buffer)
				delete[] pBuf;
			return res;
		}
		bool _conversion_copy_(const TPeerTypeChar *_str, const int _iLen)
		{
			TypeChar lc_s_buffer[384 / sizeof(TypeChar)];
			const TPeerTypeChar *lc_s_end = _str + (_iLen < 0 ? __my_str_len(_str) : _iLen);
			_CTX_UTF8_TRANS_::state_type lc_state = _CTX_UTF8_TRANS_::_object_()->get_empty_codecvt_state_();
			this->__clear__();
			while (_str != lc_s_end)
			{
				TypeChar *lc_out_next_ptr = NULL;
				switch (_CTX_UTF8_TRANS_::_object_()->_trans_(lc_state, _str, lc_s_end, _str,
					lc_s_buffer, lc_s_buffer + sizeof(lc_s_buffer) / sizeof(lc_s_buffer[0]), lc_out_next_ptr))
				{
				case _CTX_UTF8_TRANS_::partial:
				case _CTX_UTF8_TRANS_::ok:
					if ((size_t)lc_s_buffer < (size_t)lc_out_next_ptr)
						this->__append__(lc_s_buffer, (int)(lc_out_next_ptr - lc_s_buffer));
					else
					{
						assert(false);
						this->clear();
						return false;
					}
					break;
				case _CTX_UTF8_TRANS_::noconv:
					assert(false);//Never test the situation
					if (_str < lc_s_end)
					{
						int lc_org_len = this->iLength;
						this->__resize__(this->iLength + (int)(lc_s_end - _str), true);
						TypeChar *lc_ptr = this->dataPtr + lc_org_len;
						while (_str != lc_s_end)
							*(lc_ptr++) = (TypeChar)*(_str++);
					}
					else
					{
						assert(false);
						this->clear();
					}
					return false;
					break;
				default:
					assert(false);
					this->clear();
					return false;
					break;
				}
			}
			return true;
		}
		static TxComposeParameter _g_compose_(
			const TxBlobTmplString &_compose, const std::function<void(const TxBlobTmplString&)> &_mfunc)
		{
			//return TxComposeParameter(std::shared_ptr<MyComposeParameter_<TxBlobTmplString>>(
			//	new MyComposeParameter_<TxBlobTmplString>(_compose, _mfunc)));
			return TxComposeParameter(std::make_shared<MyComposeParameter_<TxBlobTmplString>>(_compose, _mfunc));
		}
	public:
		TxBlobTmplString() { __constructor_init__(); }
		TxBlobTmplString(const TypeChar *_str, int _iLen = -1) { __constructor_init__(); if (_str != NULL) this->assign(_str, _iLen); }
		TxBlobTmplString(const TypeOpsChar *_str, int _iLen = -1) { __constructor_init__(); if (_str != NULL) this->assign(_str, _iLen); }
		TxBlobTmplString(const TypeStdString &_temp) { __constructor_init__(); this->assign(_temp.c_str(), (int)_temp.length()); }
		TxBlobTmplString(const TxBlobTmplString &_temp) { __constructor_init__(); this->assign(_temp); }
		~TxBlobTmplString() { this->__clear__(); }
		TxBlobTmplString clone() const { return *this; }
		void assign(const TypeChar *_str, int _iLen = -1) { this->__assign__(_str, _iLen); }
		void assign(const TypeOpsChar *_str, int _iLen = -1) { this->__assign__((const TypeChar*)_str, _iLen); }
		void assign(const TxBlobTmplString &_tem) { this->__assign__(_tem.dataPtr, _tem.iLength); }
		void assign(const TypeStdString &_tem) { this->__assign__(_tem.c_str(), (int)_tem.length()); }
		void append(const TypeChar *_str, int _iLen = -1) { this->__append__(_str, _iLen); }
		void append(const TypeOpsChar *_str, int _iLen = -1) { this->__append__((const TypeChar*)_str, _iLen); }
		void append(const TxBlobTmplString &_tem) { this->__append__(_tem.dataPtr, _tem.iLength); }
		void append(const TypeStdString &_tem) { this->__append__(_tem.c_str(), (int)_tem.length()); }
		void assign(const PeerStrongTmplStringType &_tem) { return this->assign(_tem.c_str(), _tem.length()); }
		void assign(const TPeerTypeStdString &_tem) { return this->assign(_tem.c_str(), _tem.length()); }
		void assign(const TPeerTypeChar *_str, int _iLen = -1) { this->_conversion_copy_(_str, _iLen); }
		void append(const PeerStrongTmplStringType &_tem) { this->append(_tem.c_str(), _tem.length()); }
		void append(const TPeerTypeStdString &_tem) { this->append(_tem.c_str(), (int)_tem.length()); }
		void append(const TPeerTypeChar *_str, int _iLen = -1)
		{
			TxBlobTmplString lc_app_str;
			lc_app_str._conversion_copy_(_str, _iLen);
			this->__append__(lc_app_str.c_str(), lc_app_str.length());
		}
		void resize(const int _iSize, const bool _bKeepOrgn)
		{
			this->__resize__(_iSize + 1, _bKeepOrgn);
			this->dataPtr[this->iLength] = 0;
		}
		int compare(const TypeChar *_str, int _iLen = -1) const
		{
			return this->__compare_value_(_str, _iLen);
		}
		int compare(const TxBlobTmplString &_tem) const
		{
			return this->__compare_value_(_tem.data(), _tem.iLength);
		}
		bool equalNoCase(const TypeChar *_str, int _iLen = -1) const
		{
			return this->__no_case_compare_equal_(_str, _iLen);
		}
		bool equalNoCase(const TxBlobTmplString &_tem) const
		{
			return this->__no_case_compare_equal_(_tem.data(), _tem.iLength);
		}
		void pop_back(int _iDiscardCount) { this->__pop_back__(_iDiscardCount); }
		void clear() { this->__clear__(); }
		inline TypeChar* data() const { return const_cast<TypeChar*>(this->dataPtr); }
		inline const TypeChar* c_str() const { return this->dataPtr; }
		inline int length() const { return this->iLength; }
		inline int size() const { return this->iLength; }
		inline TypeChar* begin() { return this->dataPtr; }
		inline TypeChar* end() { return this->dataPtr + this->iLength; }
		inline const TypeChar* cbegin() const { return this->dataPtr; }
		inline const TypeChar* cend() const { return this->dataPtr + this->iLength; }
		TypeStdString toStdString() const { return TypeStdString(this->dataPtr, this->iLength); }
		operator TypeStdString() const { return TypeStdString(this->dataPtr, this->iLength); }
		TxBlobTmplString& operator=(const TxBlobTmplString &_tem) { this->assign(_tem); return *this; }
		TxBlobTmplString& operator=(const TypeStdString &_tem) { this->assign(_tem); return *this; }
		TxBlobTmplString& operator=(const TypeChar *_str) { this->assign(_str); return *this; }
		TxBlobTmplString& operator=(const TypeOpsChar *_str) { this->assign(_str); return *this; }
		TxBlobTmplString& operator+=(const TypeStdString &_tem) { this->append(_tem); return *this; }
		TxBlobTmplString& operator+=(const TxBlobTmplString &_tem) { this->append(_tem); return *this; }
		TxBlobTmplString& operator+=(const TypeChar *_str) { this->append(_str); return *this; }
		TxBlobTmplString& operator+=(const TypeOpsChar *_str) { this->append(_str); return *this; }
		TxBlobTmplString operator+(const TxBlobTmplString &_tem) const { TxBlobTmplString ret(*this); ret.append(_tem); return ret; }
		TxBlobTmplString operator+(const TypeStdString &_tem) const { TxBlobTmplString ret(*this); ret.append(_tem); return ret; }
		TxBlobTmplString operator+(const TypeChar *_str) const { TxBlobTmplString ret(*this); ret.append(_str); return ret; }
		TxBlobTmplString operator+(const TypeOpsChar *_str) const { TxBlobTmplString ret(*this); ret.append(_str); return ret; }
		bool operator==(const TxBlobTmplString &_tem) const { return this->__compare_value_(_tem.dataPtr, _tem.iLength) == 0; }
		bool operator==(const TypeChar _str[]) const { return this->__compare_value_(_str) == 0; }
		bool operator!=(const TxBlobTmplString &_tem) const { return this->__compare_value_(_tem.dataPtr, _tem.iLength) != 0; }
		bool operator!=(const TypeChar _str[]) const { return this->__compare_value_(_str) != 0; }
		bool operator<(const TxBlobTmplString &_tem) const { return this->__compare_value_(_tem.dataPtr, _tem.iLength) < 0; }
		bool operator<(const TypeChar _str[]) const { return this->__compare_value_(_str) < 0; }
		inline TypeChar& operator[](int _idx)
		{
			enum { _ESIZETDIVTCHARINT = (sizeof(size_t)) / (sizeof(TypeChar)), };
			static TypeChar lc_tmp_chs[(_ESIZETDIVTCHARINT < 4 ? 4 : _ESIZETDIVTCHARINT) * 2] = { 0 };
			assert(0 <= _idx&&_idx < this->iLength&&lc_tmp_chs[0] == 0);
			return (0 <= _idx&&_idx < this->iLength) ? this->dataPtr[_idx] : (lc_tmp_chs[0] = 0, lc_tmp_chs[0]);
		}
		inline TypeChar operator[](int _idx) const
		{
			assert(0 <= _idx&&_idx < this->iLength);
			return (0 <= _idx&&_idx < this->iLength) ? this->dataPtr[_idx] : (TypeChar)0;
		}
		TxBlobTmplString left(int _nCount) const
		{
			return TxBlobTmplString(this->dataPtr, TxStdlib::minimum<int>(_nCount, this->iLength));
		}
		TxBlobTmplString right(int _nCount) const
		{
			if (_nCount <= 0) return TxBlobTmplString();
			else if (_nCount >= this->iLength) return *this;
			return TxBlobTmplString(this->dataPtr + this->iLength - _nCount, _nCount);
		}
		TxBlobTmplString middle(int _nStart, int _nCount) const
		{
			if (_nStart >= this->iLength || _nCount <= 0) return TxBlobTmplString();
			return TxBlobTmplString(this->dataPtr + _nStart, TxStdlib::minimum<int>(_nCount, this->iLength - _nStart));
		}
		TxBlobTmplString& lowerCaseSelf()
		{
			for (int pos = this->iLength; pos--;)
			{
				int ch = this->dataPtr[pos];
				if (ch >= 'A'&&ch <= 'Z')
					this->dataPtr[pos] += 'a' - 'A';
			}
			return *this;
		}
		TxBlobTmplString& upperCaseSelf()
		{
			for (int pos = this->iLength; pos--;)
			{
				int ch = this->dataPtr[pos];
				if (ch >= 'a'&&ch <= 'z')
					this->dataPtr[pos] += 'A' - 'a';
			}
			return *this;
		}
		TxBlobTmplString toLowerCase() const
		{
			TxBlobTmplString ret(*this);
			ret.lowerCaseSelf();
			return ret;
		}
		TxBlobTmplString toUpperCase() const
		{
			TxBlobTmplString ret(*this);
			ret.upperCaseSelf();
			return ret;
		}
		TxBlobTmplString& replaceSelf(TypeChar target, TypeChar sample1)
		{
			for (int pos = this->iLength; pos--;)
			{
				if (this->dataPtr[pos] == sample1)
					this->dataPtr[pos] = target;
			}
			return *this;
		}
		TxBlobTmplString& replaceSelf(TypeChar target, TypeChar sample1, TypeChar sample2)
		{
			for (int pos = this->iLength; pos--;)
			{
				int ch = this->dataPtr[pos];
				if (ch == (int)sample1 || ch == (int)sample2)
					this->dataPtr[pos] = target;
			}
			return *this;
		}
		TxBlobTmplString& replaceSelf(TypeChar target, TypeChar sample1, TypeChar sample2, TypeChar sample3)
		{
			for (int pos = this->iLength; pos--;)
			{
				int ch = this->dataPtr[pos];
				if (ch == (int)sample1 || ch == (int)sample2 || ch == (int)sample3)
					this->dataPtr[pos] = target;
			}
			return *this;
		}
		int format(const TypeChar fmt[], ...)
		{
			va_list lc_args;
			va_start(lc_args, fmt);
			int res = this->_format__(true, fmt, lc_args);
			va_end(lc_args);
			return res;
		}
		int vformat(const TypeChar fmt[], va_list _args)
		{
			return this->_format__(true, fmt, _args);
		}
		int appendFormat(const TypeChar fmt[], ...)
		{
			va_list lc_args;
			va_start(lc_args, fmt);
			int res = this->_format__(false, fmt, lc_args);
			va_end(lc_args);
			return res;
		}
		TxBlobTmplString trimLeft(const TypeChar _str[]) const
		{
			int _strLen = __my_str_len(_str);
			const TypeChar *p = this->dataPtr - 1;
			while (__my_mem_chr(_str, *(++p), _strLen) != NULL);
			return TxBlobTmplString(p);
		}
		TxBlobTmplString trimRight(const TypeChar _str[]) const
		{
			int _strLen = __my_str_len(_str);
			const TypeChar *pStart = this->dataPtr;
			const TypeChar *p = this->dataPtr + this->iLength;
			while ((--p) >= pStart&&__my_mem_chr(_str, *p, _strLen) != NULL);
			return TxBlobTmplString(pStart, (int)(p - pStart + 1));
		}
		TxBlobTmplString trimLR(const TypeChar _str[]) const
		{
			const TypeChar *p;
			int _strLen = __my_str_len(_str);
			for (p = this->dataPtr - 1; __my_mem_chr(_str, *(++p), _strLen) != NULL;);
			const TypeChar *pStart = p;
			for (p = this->dataPtr + this->iLength; (--p) >= pStart&&__my_mem_chr(_str, *p, _strLen) != NULL;);
			return TxBlobTmplString(pStart, (int)(p - pStart + 1));
		}
		TxBlobTmplString& trimSelfRight(const TypeChar _str[])
		{
			int _strLen = __my_str_len(_str);
			TypeChar *pStart = this->dataPtr;
			TypeChar *p = this->dataPtr + this->iLength;
			while ((--p) >= pStart&&__my_mem_chr(_str, *p, _strLen) != NULL);
			this->pop_back(this->iLength - (int)(p - pStart + 1));
			return *this;
		}
		void fillZero()
		{
			memset(this->dataPtr, 0, (this->iLength + 1) * sizeof(*this->dataPtr));
		}
		inline TypeChar frontChar() const
		{
			return this->iLength > 0 ? this->dataPtr[0] : 0;
		}
		inline TypeChar backChar() const
		{
			return this->iLength > 0 ? this->dataPtr[this->iLength - 1] : 0;
		}
		inline TypeChar at(unsigned int _index) const
		{
			assert((int)_index < this->iLength);
			return (int)_index < this->iLength ? this->dataPtr[_index] : 0;
		}
		inline void setAt(unsigned int _index, TypeChar _ch)
		{
			if ((int)_index >= this->iLength)
				assert(false);
			else this->dataPtr[_index] = _ch;
		}
	public:
		PeerStrongTmplStringType utf8Codecvt() const
		{//Unicode转utf8再转Unicode性能，50M每秒
			PeerStrongTmplStringType ret;
			ret._conversion_copy_(this->dataPtr, this->iLength);
			return ret;
		}
		static TxComposeParameter compose(
			const TxBlobTmplString &_compose, const std::function<void(const TxBlobTmplString&)> &_mfunc)
		{
			return _g_compose_(_compose, _mfunc);
		}
		TxComposeParameter compose(const TxBlobTmplString &_compose)
		{
			this->clear();
			return _g_compose_(_compose, [this](const TxBlobTmplString &_str) {
				this->__assign__(_str.dataPtr, _str.iLength);
			});
		}
		TxComposeParameter appendCompose(const TxBlobTmplString &_compose)
		{
			return _g_compose_(_compose, [this](const TxBlobTmplString &_str) {
				this->__append__(_str.dataPtr, _str.iLength);
			});
		}
	public:
		std::list<TxBlobTmplString> splitSegment(const TypeChar *_sample0, const TypeChar *_sample1 = NULL, const TypeChar *_sample2 = NULL, const TypeChar *_sample3 = NULL,
			const TypeChar *_sample4 = NULL, const TypeChar *_sample5 = NULL, const TypeChar *_sample6 = NULL, const TypeChar *_sample7 = NULL) const
		{
			std::list<TxBlobTmplString> ret;
			struct tagSpSeg {
				const TypeChar *sample;
				int len;
				tagSpSeg() { this->len = 0; this->sample = NULL; }
				void set(const TypeChar *_sample) { this->len = ((this->sample = _sample) == NULL ? 0 : (int)__my_str_len(_sample)); }
				bool operator<(const tagSpSeg &_tem) const { return this->len > _tem.len; }
			} arg[8];
			arg[0].set(_sample0);
			arg[1].set(_sample1);
			arg[2].set(_sample2);
			arg[3].set(_sample3);
			arg[4].set(_sample4);
			arg[5].set(_sample5);
			arg[6].set(_sample6);
			arg[7].set(_sample7);
			std::sort(&arg[0], &arg[sizeof(arg) / sizeof(arg[0])]);

			int lcStartPos = 0;
			for (int pos = 0; pos < this->iLength; pos++)
			{
				for (int i = 0; i < sizeof(arg) / sizeof(arg[0]); i++)
				{
					if (arg[i].len <= 0) break;
					if (arg[i].len <= this->iLength - pos&&::memcmp(&this->dataPtr[pos], arg[i].sample, arg[i].len) == 0)
					{
						if (pos > lcStartPos)
						{
							ret.push_back(TxBlobTmplString());
							ret.back().assign(&this->dataPtr[lcStartPos], pos - lcStartPos);
						}
						lcStartPos = (pos += arg[i].len);
						if (arg[i].len > 0)
							--pos;
						break;
					}
				}
			}
			if (this->iLength > lcStartPos)
			{
				ret.push_back(TxBlobTmplString());
				ret.back().assign(&this->dataPtr[lcStartPos], this->iLength - lcStartPos);
			}
			return ret;
		}
	public:
		template<int N> inline static bool txStrNCopy(TypeChar(&_dest)[N], const TypeChar _src[])
		{
			bool ret = true; assert(N > 0);
			int len = __my_str_len((TypeChar*)_src) + 1;
			if (len > N) ret = false, len = N - 1, _dest[N - 1] = 0;
			__my_mem_cpy(_dest, _src, len); return ret;
		}
		template<int N> inline static bool txStrNCopy(TypeChar(&_dest)[N], const TypeOpsChar _src[])
		{
			bool ret = true; assert(N > 0);
			int len = __my_str_len((TypeChar*)_src) + 1;
			if (len > N) ret = false, len = N - 1, _dest[N - 1] = 0;
			__my_mem_cpy(_dest, _src, len); return ret;
		}
		template<int N> inline static bool txStrNCopy(TypeOpsChar(&_dest)[N], const TypeChar _src[])
		{
			bool ret = true; assert(N > 0);
			int len = __my_str_len((TypeChar*)_src) + 1;
			if (len > N) ret = false, len = N - 1, _dest[N - 1] = 0;
			__my_mem_cpy(_dest, _src, len); return ret;
		}
		template<int N> inline static bool txStrNCopy(TypeOpsChar(&_dest)[N], const TypeOpsChar _src[])
		{
			bool ret = true; assert(N > 0);
			int len = __my_str_len((TypeChar*)_src) + 1;
			if (len > N) ret = false, len = N - 1, _dest[N - 1] = 0;
			__my_mem_cpy(_dest, _src, len); return ret;
		}
		template<int N> inline static bool txSnprintf(TypeChar(&_dest)[N], const TypeChar *format, ...)
		{
			assert(N > 0);
			va_list args;
			va_start(args, format);
			int ret = __my_vsn_printf((TypeChar*)_dest, N - 1, format, args);
			va_end(args);
			_dest[N - 1] = 0;
			return ret >= 0;
		}
		template<int N> inline static bool txSnprintf(TypeOpsChar(&_dest)[N], const TypeChar *format, ...)
		{
			assert(N > 0);
			va_list args;
			va_start(args, format);
			int ret = __my_vsn_printf((TypeChar*)_dest, N - 1, format, args);
			va_end(args);
			_dest[N - 1] = 0;
			return ret >= 0;
		}
		static int txStringLength(const TypeChar _str[]) { return __my_str_len(_str); }
	public:
		template<class RET>
		RET toDatumRet(const RET &_defaultVal = TDefaultValRef<RET>::get_()) const
		{
			RET ret;
			if (!this->toDatum(ret) && (&TDefaultValRef<RET>::get_() != &_defaultVal))
				ret = _defaultVal;
			return ret;
		}
		template<class NumType>
		static bool _toDatumNumbTrans_(std::basic_istringstream<TypeChar> &_iss, NumType &out_num, const TxEmptyObjectTmpl<0>&)
		{
			bool res = false;
			if (1 == sizeof(NumType))
				res = (_iss >> (*(std::int8_t*)&out_num)).operator bool();
			else if (2 == sizeof(NumType))
				res = (_iss >> (*(std::int16_t*)&out_num)).operator bool();
			else if (4 == sizeof(NumType))
				res = (_iss >> (*(std::int32_t*)&out_num)).operator bool();
			else if (8 == sizeof(NumType))
				res = (_iss >> (*(std::int64_t*)&out_num)).operator bool();
			else assert(false);
			return res;
		}
		template<class NumType>
		static bool _toDatumNumbTrans_(std::basic_istringstream<TypeChar> &_iss, NumType &out_num, const TxEmptyObjectTmpl<1>&)
		{
			bool res = false;
			if (_iss >> out_num)
				res = true;
			return res;
		}
		template<class NumType>
		bool toDatum(NumType &out_num, int offset = 0) const
		{
			if ((unsigned int)offset >= (unsigned int)this->iLength)
				return false;
			std::basic_istringstream<TypeChar> lc_iss(TypeStdString(
				this->dataPtr + offset, this->iLength - offset));
			if (std::numeric_limits<NumType>::max_digits10 > 0)
				lc_iss.precision(std::numeric_limits<NumType>::max_digits10);
			return _toDatumNumbTrans_<NumType>(lc_iss, out_num, TxEmptyObjectTmpl<(int)!(
				std::is_enum<NumType>::value
				|| std::is_same<NumType, bool>::value
				|| std::is_same<NumType, const bool>::value
				|| std::is_same<NumType, bool&>::value
				|| std::is_same<NumType, const bool&>::value)>());
		}
		bool toDatum(bool &out_num, int offset = 0) const
		{
			bool res = false;
			if (offset <= this->iLength)
			{
				int lc_val = (!!out_num);
				if (this->toDatum(lc_val, offset))
					out_num = !!lc_val;
			}
			return res;
		}
		template<int N>
		bool toDatum(TypeChar(&out_num)[N], int offset = 0) const
		{
			bool res = false;
			if (offset <= this->iLength)
				res = txStrNCopy(out_num, this->dataPtr + offset);
			else out_num[0] = 0;
			return res;
		}
		template<int N>
		bool toDatum(TypeOpsChar(&out_num)[N], int offset = 0) const
		{
			bool res = false;
			if (offset <= this->iLength)
				res = txStrNCopy(out_num, this->dataPtr + offset);
			else out_num[0] = 0;
			return res;
		}
		bool toDatum(TxBlobTmplString& out_str, int offset = 0) const
		{
			bool res = false;
			if (offset <= this->iLength)
			{
				out_str = (this->dataPtr + offset);
				res = true;
			}
			else
			{
				out_str.clear();
			}
			return res;
		}
		bool toDatum(TPeerTypeStdString& out_str, int offset = 0) const
		{
			bool res = false;
			if (offset <= this->iLength)
			{
				PeerStrongTmplStringType mLcTem;
				mLcTem.assign(this->dataPtr + offset);
				out_str.assign(mLcTem.c_str(), mLcTem.iLength);
				res = true;
			}
			else
			{
				out_str.clear();
			}
			return res;
		}
		bool toDatum(PeerStrongTmplStringType& out_str, int offset = 0) const
		{
			bool res = false;
			if (offset <= this->iLength)
			{
				out_str.assign(this->dataPtr + offset);
				res = true;
			}
			else
			{
				out_str.clear();
			}
			return res;
		}
		bool toDatum(TypeStdString& out_str, int offset = 0) const
		{
			bool res = false;
			if (offset <= this->iLength)
			{
				out_str = (this->dataPtr + offset);
				res = true;
			}
			else
			{
				out_str.clear();
			}
			return res;
		}
	public:
		template<class NumType>
		static TxBlobTmplString fromDatum(const NumType &num)
		{
			std::basic_ostringstream<TypeChar> lc_oss;
			if (std::numeric_limits<NumType>::max_digits10 > 0)
				lc_oss.precision(std::numeric_limits<NumType>::max_digits10);
			if (std::is_enum<NumType>::value
				|| std::is_same<NumType, bool>::value
				|| std::is_same<NumType, const bool>::value
				|| std::is_same<NumType, bool&>::value
				|| std::is_same<NumType, const bool&>::value)
			{
				if (1 == sizeof(NumType))
					lc_oss << (*(const std::int8_t*)&num);
				else if (2 == sizeof(NumType))
					lc_oss << (*(const std::int16_t*)&num);
				else if (4 == sizeof(NumType))
					lc_oss << (*(const std::int32_t*)&num);
				else if (8 == sizeof(NumType))
					lc_oss << (*(const std::int64_t*)&num);
				else assert(false);
			}
			else lc_oss << num;
			return lc_oss.str();
		}

		static TxBlobTmplString fromDatum(const TypeOpsChar *num) { return TxBlobTmplString(num); }
		static TxBlobTmplString fromDatum(const TypeChar *num) { return TxBlobTmplString(num); }
		static TxBlobTmplString fromDatum(TypeOpsChar *num) { return TxBlobTmplString(num); }
		static TxBlobTmplString fromDatum(TypeChar *num) { return TxBlobTmplString(num); }
		static TxBlobTmplString fromDatum(const TypeStdString &num) { return TxBlobTmplString(num); }
		static TxBlobTmplString fromDatum(TypeStdString &num) { return TxBlobTmplString(num); }
		static TxBlobTmplString fromDatum(const TxBlobTmplString &num) { return num; }
		static TxBlobTmplString fromDatum(TxBlobTmplString &num) { return num; }

		static TxBlobTmplString fromDatum(const TPeerTypeChar *num) { TxBlobTmplString ret; ret.assign(num); return ret; }
		static TxBlobTmplString fromDatum(const TPeerTypeOpsChar *num) { TxBlobTmplString ret; ret.assign(num); return ret; }
		static TxBlobTmplString fromDatum(TPeerTypeChar *num) { TxBlobTmplString ret; ret.assign(num); return ret; }
		static TxBlobTmplString fromDatum(TPeerTypeOpsChar *num) { TxBlobTmplString ret; ret.assign(num); return ret; }
		static TxBlobTmplString fromDatum(const PeerStrongTmplStringType &num) { TxBlobTmplString ret; ret.assign(num); return ret; }
		static TxBlobTmplString fromDatum(PeerStrongTmplStringType &num) { TxBlobTmplString ret; ret.assign(num); return ret; }
		static TxBlobTmplString fromDatum(const TPeerTypeStdString &num) { TxBlobTmplString ret; ret.assign(num); return ret; }
		static TxBlobTmplString fromDatum(TPeerTypeStdString &num) { TxBlobTmplString ret; ret.assign(num); return ret; }
	};
}

typedef _TxPrivateNs_::TxBlobTmplString<
	char, unsigned char,
	wchar_t, _TxPrivateNs_::_TSizeSignedInt_<sizeof(wchar_t)>::type
> TxBlobString;

typedef _TxPrivateNs_::TxBlobTmplString<
	wchar_t, _TxPrivateNs_::_TSizeSignedInt_<sizeof(wchar_t)>::type,
	char, unsigned char
> TxBlobWString;

//int main()
//{
//	TxBlobString sss;
//	sss.compose(" 512.3 sfd{1},{0},{2},{0},{3},").args(L"jlskf",1231223,TxBlobWString::fromDatum(15222.01)).args("abc");
//	sss.appendFormat("sfd%d,%s,		",123456789,"abc");
//	TxBlobString sss1=TxBlobWString::fromDatum(sss).trimLR(L"	 ").utf8Codecvt();
//	double val=sss.toDatumRet<double>();
//	int64_t val64=sss.toDatumRet<int>();
//	printf("%g,%lld,%s\n",(double)val,(long long int)val64,sss.c_str());
//	return 0;
//}


//--------------------------------------TxSingletonTmpl--------------------------------------//
template<class T, bool B_ASSERT = true, int BORDER_SIZE = 256>
class TxSingletonTmpl : private TxForbidDirectCopyObject
{
private:
	class T_EX : public T
	{
	public:
		T_EX() {}
		~T_EX() {}
	};
	class tagSingleObject_
	{
	private:
		friend T;
		friend TxSingletonTmpl<T>;
	private:
		TXSTDALIGNAS_16 unsigned char reserve_header[BORDER_SIZE];
		TXSTDALIGNAS_16 unsigned char obj_buffer[((sizeof(T_EX) + sizeof(void*) + 7) / 8) * 8];
		TXSTDALIGNAS_16 unsigned char reserve_tail[BORDER_SIZE];
		TXSTDALIGNAS_16 T_EX *object_ptr;
	public:
		tagSingleObject_(int _sign)
		{
			assert(!B_ASSERT || _sign > 0);
			TX_UNUSED(_sign);
			memset(this->reserve_header, 0, sizeof(this->reserve_header));
			memset(this->obj_buffer, 0, sizeof(this->obj_buffer));
			memset(this->reserve_tail, 0, sizeof(this->reserve_tail));
			*(void**)this->reserve_tail = *(void**)this->reserve_header = (void*)this;
			this->object_ptr = (T_EX*)this->obj_buffer;
			new (this->object_ptr) T_EX();
		}
		~tagSingleObject_()
		{
			assert(!B_ASSERT
				|| (this->object_ptr == NULL
					&& (*(void**)this->reserve_header == (void*)this)
					&& (*(void**)this->reserve_tail == (void*)this)));
			this->destructor();
		}
		void destructor()
		{
			if (this->object_ptr != NULL)
			{
				this->object_ptr->~T_EX();
				this->object_ptr = NULL;
			}
		}
		inline T* get_ptr() const
		{
			return const_cast<T*>((const T*)this->object_ptr);
		}
	};
private:
	friend T;
	friend tagSingleObject_;
private:
	TxSingletonTmpl(const TxSingletonTmpl&)
	{
		assert(false);
	}
	static T* _init_super_env_(int _sign)
	{
		static tagSingleObject_ ret(_sign);
		if (_sign < 0)
			ret.destructor();
		return ret.get_ptr();
	}
public:
	TxSingletonTmpl()
	{
	}
	static void initialize()
	{
		_init_super_env_(1);
	}
	static void uninitialize()
	{
		_init_super_env_(-1);
	}
	static T* object()
	{
		return _init_super_env_(0);
	}
};


//--------------------------------------TxSliceFileEntirety--------------------------------------//
class TxSliceFileEntirety
{
private:
	class MyInt128 {
	private:
		std::uint64_t llIdentInt64[2];
		friend class TxSliceFileEntirety;
	private:
		const MyInt128& _incr()
		{
			if ((++this->llIdentInt64[0]) >= (((std::uint64_t)~(std::uint64_t)(0)) >> 1) - 1)
			{
				this->llIdentInt64[0] = 0;
				++this->llIdentInt64[1];
			}
			return *this;
		}
		MyInt128(std::uint64_t _high, std::uint64_t _low)
		{
			this->llIdentInt64[0] = _low;
			this->llIdentInt64[1] = _high;
		}
	public:
		MyInt128()
		{
			this->reset();
		}
		inline void reset()
		{
			this->llIdentInt64[0] = 0;
			this->llIdentInt64[1] = 0;
		}
		inline bool operator<(const MyInt128 &_tmp) const {
			return this->llIdentInt64[1] != _tmp.llIdentInt64[1] ?
				(bool)(this->llIdentInt64[1] < _tmp.llIdentInt64[1])
				: (bool)(this->llIdentInt64[0] < _tmp.llIdentInt64[0]);
		}
		inline bool isEmpty() const
		{
			return this->llIdentInt64[0] == 0 && this->llIdentInt64[1] == 0;
		}
	};
	struct tagFileUserdData_ {
		MyInt128 mIdent;
		std::uint64_t llFileStart;
		std::uint64_t llFilePos;//从0开始
		std::uint64_t llFileSize;
		std::uint64_t llFileEnd;
		const MyInt128& getIdent() const {
			return this->mIdent;
		}
	};
	struct tagFileIdleData_ {
		std::uint64_t llFileStart;
		std::uint64_t llFileEnd;
		MyInt128 getSizePosTo128() const {
			return MyInt128(this->llFileEnd - this->llFileStart, this->llFileStart);
		}
	};
public:
	enum EORGSK {
		ORIGIN_SEEK_SET = SEEK_SET,
		ORIGIN_SEEK_CUR = SEEK_CUR,
		ORIGIN_SEEK_END = SEEK_END,
	};
	typedef MyInt128 TFileIdent;
private:
	std::map<MyInt128, tagFileUserdData_> mapUserdIdentFile;//已经使用
	std::map<MyInt128, tagFileIdleData_> mapIdleSizePosFile;//未分配，size-pos，可用的
	std::map<std::uint64_t, tagFileIdleData_> mapIdleStartKeyFile;//未分配，start，可用的
	std::map<std::uint64_t, tagFileIdleData_> mapIdleEndKeyFile;//未分配，end，可用的
	MyInt128 mRecordIncrIdent;
	std::uint64_t llTotalFileEndPos;
private:
	virtual int vfFileRead(std::uint64_t _pos, void *_buffer, int _size) = 0;
	virtual bool vfFileWrite(std::uint64_t _pos, const void *_buffer, int _size) = 0;
	virtual void vfFileResize(std::uint64_t _fileSize) = 0;
	virtual void vfError(const std::wstring &_err) = 0;
	int _doFileRead(std::uint64_t _pos, void *_buffer, int _size)
	{
		if ((std::int64_t)_pos < 0 || _size < 0 || _buffer == NULL)
		{
			this->vfError(L"_doFileRead(): if((std::int64_t)_pos<0||_size<0||_buffer==NULL)");
			return 0;
		}
		if (_pos + _size > this->llTotalFileEndPos)
		{
			this->vfError(L"_doFileRead(): if(_pos+_size>this->llTotalFileEndPos)");
			return 0;
		}
		return this->vfFileRead(_pos, _buffer, _size);
	}
	bool _doFileWrite(std::uint64_t _pos, const void *_buffer, int _size)
	{
		if ((std::int64_t)_pos < 0 || _size < 0 || _buffer == NULL)
		{
			this->vfError(L"_doFileWrite(): if((std::int64_t)_pos<0||_size<0||_buffer==NULL)");
			return 0;
		}
		if (_pos + _size > this->llTotalFileEndPos)
		{
			this->vfError(L"_doFileWrite(): if(_pos+_size>this->llTotalFileEndPos)");
			return 0;
		}
		return this->vfFileWrite(_pos, _buffer, _size);
	}
	void _doFileResize(std::uint64_t _fileSize)
	{
		this->llTotalFileEndPos = _fileSize;
		this->vfFileResize(_fileSize);
	}
	const TFileIdent& _generateFileIncrIdent_()
	{
		return this->mRecordIncrIdent._incr();
	}
protected:
	TxSliceFileEntirety()
	{
		this->mRecordIncrIdent.reset();
		this->mRecordIncrIdent._incr();
		this->llTotalFileEndPos = 0;
	}
	~TxSliceFileEntirety()
	{
	}
	TFileIdent do_create(std::uint64_t _fileSize)
	{
		tagFileUserdData_ lcUserdData;
		if (_fileSize > 1024 * 1024 * (std::uint64_t)1024 * 1024 * 1024 || _fileSize <= 0)//1024*1024*1G
		{
			lcUserdData.mIdent.reset();
			assert(_fileSize == 0);
		}
		else
		{
			lcUserdData.mIdent = this->_generateFileIncrIdent_();
			const std::uint64_t lc_fileAlignSize = ((_fileSize + 255) / (std::uint64_t)256)*(std::uint64_t)256;
			auto iter = this->mapIdleSizePosFile.upper_bound(MyInt128(_fileSize, 0));
			if (iter != this->mapIdleSizePosFile.end())
			{
				tagFileIdleData_ lcIdleMem = iter->second;
				lcUserdData.llFileStart = lcIdleMem.llFileStart;
				lcUserdData.llFileSize = _fileSize;
				lcUserdData.llFilePos = 0;
				lcUserdData.llFileEnd = lcIdleMem.llFileStart + lc_fileAlignSize;

				this->mapIdleSizePosFile.erase(iter);
				assert(this->mapIdleStartKeyFile.find(lcIdleMem.llFileStart) != this->mapIdleStartKeyFile.end());
				this->mapIdleStartKeyFile.erase(lcIdleMem.llFileStart);
				assert(this->mapIdleEndKeyFile.find(lcIdleMem.llFileEnd) != this->mapIdleEndKeyFile.end());
				this->mapIdleEndKeyFile.erase(lcIdleMem.llFileEnd);
			}
			else
			{
				lcUserdData.llFileStart = this->llTotalFileEndPos;
				lcUserdData.llFileSize = _fileSize;
				lcUserdData.llFilePos = 0;
				lcUserdData.llFileEnd = lcUserdData.llFileStart + lc_fileAlignSize;
				this->_doFileResize(lcUserdData.llFileEnd);
			}
			this->mapUserdIdentFile[lcUserdData.mIdent] = lcUserdData;
		}
		return lcUserdData.mIdent;
	}
	bool do_release(const TFileIdent &_fileIdent)
	{
		if (_fileIdent.isEmpty())
			return false;
		auto userd_iter = this->mapUserdIdentFile.find(_fileIdent);
		if (userd_iter == this->mapUserdIdentFile.end())
			return false;

		tagFileIdleData_ lc_newIdleData;
		lc_newIdleData.llFileStart = userd_iter->second.llFileStart;
		lc_newIdleData.llFileEnd = userd_iter->second.llFileEnd;
		this->mapUserdIdentFile.erase(userd_iter);

		auto iter_end_map = this->mapIdleEndKeyFile.find(lc_newIdleData.llFileStart);
		if (iter_end_map != this->mapIdleEndKeyFile.end())
		{
			this->mapIdleStartKeyFile.erase(iter_end_map->second.llFileStart);
			this->mapIdleSizePosFile.erase(iter_end_map->second.getSizePosTo128());
			lc_newIdleData.llFileStart = iter_end_map->second.llFileStart;
			this->mapIdleEndKeyFile.erase(iter_end_map);
		}

		auto iter_start_map = this->mapIdleStartKeyFile.find(lc_newIdleData.llFileEnd);
		if (iter_start_map != this->mapIdleStartKeyFile.end())
		{
			this->mapIdleEndKeyFile.erase(iter_start_map->second.llFileEnd);
			this->mapIdleSizePosFile.erase(iter_start_map->second.getSizePosTo128());
			lc_newIdleData.llFileEnd = iter_start_map->second.llFileEnd;
			this->mapIdleStartKeyFile.erase(iter_start_map);
		}

		if (lc_newIdleData.llFileEnd == this->llTotalFileEndPos)
		{
			this->_doFileResize(lc_newIdleData.llFileStart);
		}
		else
		{
			this->mapIdleStartKeyFile[lc_newIdleData.llFileStart] = lc_newIdleData;
			this->mapIdleEndKeyFile[lc_newIdleData.llFileEnd] = lc_newIdleData;
			this->mapIdleSizePosFile[lc_newIdleData.getSizePosTo128()] = lc_newIdleData;
		}
		return true;
	}
	bool do_read_all(TxBlobString *_outData, const TFileIdent &_fileIdent)
	{
		auto iter = this->mapUserdIdentFile.find(_fileIdent);
		if (iter == this->mapUserdIdentFile.end())
			return false;
		_outData->resize((int)iter->second.llFileSize, false);
		return _outData->size() == this->_doFileRead(iter->second.llFileStart, _outData->data(), _outData->size());
	}
	int do_read(const TFileIdent &_fileIdent, void *_buffer, int _size, long long _pos = -1)
	{
		auto iter = this->mapUserdIdentFile.find(_fileIdent);
		if (iter == this->mapUserdIdentFile.end())
			return 0;
		if (_pos < 0)
			_pos = iter->second.llFilePos;
		if ((std::uint64_t)_pos + (std::uint64_t)_size > iter->second.llFileSize)
			_size = (int)(iter->second.llFileSize - _pos);
		iter->second.llFilePos = (std::uint64_t)(_pos + _size);
		return this->_doFileRead((std::uint64_t)_pos + iter->second.llFileStart, _buffer, _size);
	}
	bool do_write(const TFileIdent &_fileIdent, const void *_buffer, int _size, long long _pos = -1)
	{
		bool ret_true = true;
		auto iter = this->mapUserdIdentFile.find(_fileIdent);
		if (iter == this->mapUserdIdentFile.end())
			return 0;
		if (_pos < 0)
			_pos = iter->second.llFilePos;
		if ((std::uint64_t)_pos + (std::uint64_t)_size > iter->second.llFileSize)
		{
			_size = (int)(iter->second.llFileSize - _pos);
			ret_true = false;
		}
		iter->second.llFilePos = (std::uint64_t)(_pos + _size);
		if (!this->_doFileWrite((std::uint64_t)_pos + iter->second.llFileStart, _buffer, _size))
			ret_true = false;
		return ret_true;
	}
	bool do_seek(const TFileIdent &_fileIdent, long long _offset, TxSliceFileEntirety::EORGSK _origin)
	{
		auto iter = this->mapUserdIdentFile.find(_fileIdent);
		if (iter == this->mapUserdIdentFile.end())
		{
			assert(false);
			return false;
		}
		switch (_origin)
		{
		case ORIGIN_SEEK_SET:
			iter->second.llFilePos = (std::uint64_t)_offset;
			break;
		case ORIGIN_SEEK_CUR:
			iter->second.llFilePos = (std::uint64_t)((std::int64_t)iter->second.llFilePos + (std::int64_t)_offset);
			break;
		case ORIGIN_SEEK_END:
			iter->second.llFilePos = (std::uint64_t)((std::int64_t)iter->second.llFileSize + (std::int64_t)_offset);
			break;
		default:
			assert(false);
			return false;
			break;
		}
		if ((std::int64_t)iter->second.llFilePos < 0)
			iter->second.llFilePos = 0;
		else if (iter->second.llFilePos > iter->second.llFileSize)
			iter->second.llFilePos = iter->second.llFileSize;
		return true;
	}
};


//--------------------------------------TxMemoryChunk--------------------------------------//
namespace _TxPrivateNs_ {
	class CEmptyDebugObjectMemory
	{
	public:
		static inline void malloc(void*) { }
		static inline void free(void*) { }
		static void checkPerfect() { }
	};
}
template<int BUFFERSIZE, class TDEBUGMEMORY = _TxPrivateNs_::CEmptyDebugObjectMemory>
class TxMemoryChunk : private TxForbidDirectCopyObject
{
protected:
	struct StructDataInfo
	{
		unsigned char *pBuffer;
		struct StructDataInfo *next;
		int iStartPos;
		int iEndPos;
		int iMaxSize;
	};
	StructDataInfo *head;
	StructDataInfo *tail;
	unsigned char __tem_buffer__[BUFFERSIZE];
	int b__tem_buffer_valid_;
	int iTotalSize;
	const int iUnitMaxSize;
private:
	StructDataInfo* _malloc_(int *_outSize, size_t _size)
	{
		unsigned char *ret = NULL;
		if (this->b__tem_buffer_valid_&&_size < sizeof(this->__tem_buffer__))
		{
			this->b__tem_buffer_valid_ = 0;
			ret = __tem_buffer__;
			*_outSize = (int)sizeof(this->__tem_buffer__);
		}
		else
		{
			*_outSize = (int)((_size + sizeof(long long) - 1) / sizeof(long long)) * sizeof(long long);
			ret = new unsigned char[*_outSize];
			TDEBUGMEMORY::malloc((StructDataInfo*)ret);
		}
		return (StructDataInfo*)ret;
	}
	void _free_(StructDataInfo *_ptr)
	{
		if ((unsigned char*)_ptr == (unsigned char*)this->__tem_buffer__)
			this->b__tem_buffer_valid_ = 1;
		else
		{
			TDEBUGMEMORY::free(_ptr);
			delete[](unsigned char*)_ptr;
		}
	}
public:
	TxMemoryChunk(int _iUnitMaxSize) :iUnitMaxSize(_iUnitMaxSize)
	{
		TDEBUGMEMORY::malloc(this);
		this->head = this->tail = NULL;
		this->iTotalSize = 0;
		this->b__tem_buffer_valid_ = 1;
	}
	~TxMemoryChunk()
	{
		this->clear();
		TDEBUGMEMORY::free(this);
	}
	inline int getTotalSize() const
	{
		return this->iTotalSize;
	}
	inline unsigned char* getUnitDataPtr()
	{
		assert(this->iTotalSize >= 0);
		return this->head != NULL ? ((unsigned char*)this->head->pBuffer + this->head->iStartPos) : NULL;
	}
	inline int getUnitDataSize() const
	{
		assert(this->head == NULL || this->head->iEndPos >= this->head->iStartPos);
		return this->head != NULL ? (this->head->iEndPos - this->head->iStartPos) : 0;
	}
	void pushTail(const unsigned char *_arry, int _iCount)
	{
		assert(_iCount >= 0);
		iTotalSize += _iCount;
		if (this->tail == NULL || (this->tail->iMaxSize - this->tail->iEndPos < _iCount))
		{
			int irsize;
			StructDataInfo *elem = this->_malloc_(&irsize, (this->iUnitMaxSize > _iCount ? this->iUnitMaxSize : _iCount) + sizeof(StructDataInfo));
			irsize -= sizeof(StructDataInfo);
			elem->pBuffer = ((unsigned char*)elem) + sizeof(StructDataInfo);
			elem->next = NULL;
			elem->iStartPos = 0;
			elem->iEndPos = 0;
			elem->iMaxSize = irsize;
			if (this->tail == NULL) this->head = elem;
			else this->tail->next = elem;
			this->tail = elem;
		}
		memcpy((char*)this->tail->pBuffer + this->tail->iEndPos, _arry, _iCount);
		this->tail->iEndPos += _iCount;
	}
	void clear()
	{
		if (this->iTotalSize != 0)
		{
			StructDataInfo *elem = this->head;
			while (elem != NULL)
			{
				StructDataInfo *ptr = elem->next;
				this->_free_(elem);
				elem = ptr;
			}
			this->head = this->tail = NULL;
			this->iTotalSize = 0;
		}
		this->b__tem_buffer_valid_ = 1;
	}
	void popFront(int _iCount)
	{
		if (_iCount >= this->iTotalSize)
			this->clear();
		else if (_iCount > 0)
		{
			this->iTotalSize -= _iCount;
			while (_iCount > 0)
			{
				StructDataInfo* pLcElem = this->head;
				int iLcLen = pLcElem->iEndPos - pLcElem->iStartPos;
				if (iLcLen <= _iCount)
				{
					this->head = pLcElem->next;
					this->_free_(pLcElem);
					if (this->head == NULL) this->tail = NULL;
				}
				else pLcElem->iStartPos += _iCount;
				_iCount -= iLcLen;
			}
		}
	}
	int popGetFront(void *_buffer, int _size)
	{
		if (_size <= 0) return 0;
		int lc_sur_size = _size;
		while (lc_sur_size > 0)
		{
			int lc_unit_size = this->getTotalSize();
			if (lc_unit_size > lc_sur_size)
				lc_unit_size = lc_sur_size;
			else if (lc_unit_size <= 0)
				break;
			memcpy(_buffer, this->getUnitDataPtr(), lc_unit_size);
			lc_sur_size -= lc_unit_size;
			this->popFront(lc_unit_size);
		}
		return _size - lc_sur_size;
	}
	void reserveStandardTail(int _iMaxSize)
	{
		assert(_iMaxSize > 0);
		if (this->head == NULL) return;
		while (this->iTotalSize > _iMaxSize&&this->head->next != NULL)
		{
			assert(([]()->bool {static long long llllll = 0; return (llllll++) != 0; })());
			StructDataInfo* pLcElem = this->head;
			this->iTotalSize -= (pLcElem->iEndPos - pLcElem->iStartPos);
			this->head = pLcElem->next;
			this->_free_(pLcElem);
		}
	}
};


//--------------------------------------TxCryptoAlgorithm--------------------------------------//
class TxCryptoAlgorithm {
public:
	class TxCrc32
	{
	private:
		std::uint32_t val_crc;
	private:
		static void _g_pushData(TxCrc32 *_pThis, const unsigned char *_data, int _size)
		{
			class MyCrc32Gfunc
			{
			private:
				std::uint32_t m_CRC32[256];
			public:
				MyCrc32Gfunc()
				{
					assert(sizeof(this->m_CRC32[0]) == 4);
					for (int i = 0; i < (int)(sizeof(m_CRC32) / sizeof(m_CRC32[0])); i++)
					{
						std::uint32_t crc = i;
						for (int j = 0; j < 8; j++)
						{
							if (crc & 1) crc = (crc >> 1) ^ 0xEDB88320;
							else crc = crc >> 1;
						}
						m_CRC32[i] = crc;
					}
				}
				inline const std::uint32_t* getCrc32Tabel() const
				{
					return this->m_CRC32;
				}
				static std::uint32_t build_crc32(const std::uint32_t *_crc_tabel, const unsigned char *_buf, int _len, std::uint32_t _init_crc32)
				{
					while (_len >= 16)
					{
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[0])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[1])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[2])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[3])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[4])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[5])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[6])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[7])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[8])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[9])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[10])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[11])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[12])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[13])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[14])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[15])] ^ (_init_crc32 >> 8);
						_buf += 16;
						_len -= 16;
					}
					while (_len >= 4)
					{
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[0])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[1])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[2])] ^ (_init_crc32 >> 8);
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ _buf[3])] ^ (_init_crc32 >> 8);
						_buf += 4;
						_len -= 4;
					}
					while ((--_len) >= 0)
					{
						_init_crc32 = _crc_tabel[((_init_crc32 & 0xFF) ^ (*(_buf++)))] ^ (_init_crc32 >> 8);
					}
					return _init_crc32;
				}
			};
			static const MyCrc32Gfunc __tem_;
			if (_data != NULL&&_size > 0 && _pThis != NULL)
			{
				_pThis->val_crc = MyCrc32Gfunc::build_crc32(__tem_.getCrc32Tabel(), _data, _size, _pThis->val_crc);
			}
		}
	public:
		TxCrc32()
		{
			initEnv();
			this->reset();
		}
		static void initEnv()
		{
			_g_pushData(NULL, NULL, 0);
		}
		inline void reset()
		{
			this->val_crc = (std::uint32_t)(-1);
		}
		void pushData(const void *_data, int _size)
		{
			_g_pushData(this, (const unsigned char*)_data, _size);
		}
		inline std::uint32_t getUint32Value() const
		{
			return ~this->val_crc;
		}
		inline void getCrc32Buffer(void *_out_crc32)
		{
			std::uint32_t lc_crc = ~this->val_crc;
			((unsigned char*)_out_crc32)[0] = (unsigned char)(lc_crc >> 24);
			((unsigned char*)_out_crc32)[1] = (unsigned char)(lc_crc >> 16);
			((unsigned char*)_out_crc32)[2] = (unsigned char)(lc_crc >> 8);
			((unsigned char*)_out_crc32)[3] = (unsigned char)(lc_crc >> 0);
		}
		inline void getCrc32Bytes(unsigned char(&_crc)[4])
		{
			return getCrc32Buffer(_crc);
		}
		inline void getCrc32Bytes(char(&_crc)[4])
		{
			return getCrc32Buffer(_crc);
		}
		inline void setFromUint32Value(std::uint32_t _crc32)
		{
			this->val_crc = (~_crc32);
		}
		inline void setFromCrc32Buffer(const void *_in_crc)
		{
			this->setFromUint32Value(
				(((std::uint32_t)(((unsigned char*)_in_crc)[0])) << 24)
				| (((std::uint32_t)(((unsigned char*)_in_crc)[1])) << 16)
				| (((std::uint32_t)(((unsigned char*)_in_crc)[2])) << 8)
				| (((std::uint32_t)(((unsigned char*)_in_crc)[3])) << 0));
		}
		inline void setFromCrc32Bytes(const unsigned char(&_crc)[4])
		{
			this->setFromCrc32Buffer(_crc);
		}
		inline void setFromCrc32Bytes(const char(&_crc)[4])
		{
			this->setFromCrc32Buffer(_crc);
		}
	};
public:
	static void initEnv()
	{
		TxCrc32::initEnv();
	}
};


//--------------------------------------TxSerializeArchive--------------------------------------//
class TxSerializeArchive
{
public:
	class TxSuccessException { };
	typedef enum _EnumDataStreamType__ {
		EDST_package = 1,
		EDST_unpack = 2,
	} EnumDataStreamType;
protected:
	TxBlobString dataBuffer;
	unsigned int dataPos;
	const EnumDataStreamType etDataStreamType;
private:
	static void _throw_exception_(bool _bSuccess)
	{
		if (_bSuccess) throw(TxSuccessException());
		else throw(std::runtime_error("serialize exception"));
	}
private:
	template<class NumType>
	static void _bind_pack_stream_(TxBlobString *_outBytes, const NumType &_num)
	{
		if (std::is_integral<NumType>::value || std::is_enum<NumType>::value || std::is_floating_point<NumType>::value)
		{
			if (sizeof(NumType) == sizeof(std::uint8_t) && sizeof(NumType) == 1)
			{
				std::uint8_t lc_buff[] = { (std::uint8_t)sizeof(NumType),
					*(const std::uint8_t*)&_num };
				_outBytes->append((const char*)lc_buff, sizeof(lc_buff));
			}
			else if (sizeof(NumType) == sizeof(std::uint16_t))
			{
				std::uint8_t lc_buff[] = { (std::uint8_t)sizeof(NumType),
					(std::uint8_t)((*(const std::uint16_t*)&_num) >> 8),
					(std::uint8_t)((*(const std::uint16_t*)&_num) >> 0) };
				_outBytes->append((const char*)lc_buff, sizeof(lc_buff));
			}
			else if (sizeof(NumType) == sizeof(std::uint32_t))
			{
				std::uint32_t a1 = *(const std::uint32_t*)&_num;
				std::uint8_t lc_buff[] = { (std::uint8_t)sizeof(NumType),
					(std::uint8_t)(a1 >> 24),
					(std::uint8_t)(a1 >> 16),
					(std::uint8_t)(a1 >> 8),
					(std::uint8_t)(a1 >> 0) };
				_outBytes->append((const char*)lc_buff, sizeof(lc_buff));
			}
			else if (sizeof(NumType) == sizeof(std::uint64_t))
			{
				std::uint32_t a1 = (std::uint32_t)((*(const std::uint64_t*)&_num) >> 32);
				std::uint32_t a2 = (std::uint32_t)((*(const std::uint64_t*)&_num) >> 0);
				std::uint8_t lc_buff[] = { (std::uint8_t)sizeof(NumType),
					(std::uint8_t)(a1 >> 24),
					(std::uint8_t)(a1 >> 16),
					(std::uint8_t)(a1 >> 8),
					(std::uint8_t)(a1 >> 0),
					(std::uint8_t)(a2 >> 24),
					(std::uint8_t)(a2 >> 16),
					(std::uint8_t)(a2 >> 8),
					(std::uint8_t)(a2 >> 0) };
				_outBytes->append((const char*)lc_buff, sizeof(lc_buff));
			}
			else assert(false);
		}
		else assert(false);
	}
	static void _bind_pack_stream_(TxBlobString *_outBytes, const TxBlobString &_num)
	{
		std::uint32_t lc_size = (std::uint32_t)_num.size();
		if (lc_size <= ((std::uint32_t)0xff) - 1)
		{
			std::uint8_t lc_buff = (std::uint8_t)lc_size;
			_outBytes->append((const char*)&lc_buff, 1);
		}
		else
		{
			std::uint8_t lc_buff[] = {
				(std::uint8_t)0xff,
				(std::uint8_t)(lc_size >> 24),
				(std::uint8_t)(lc_size >> 16),
				(std::uint8_t)(lc_size >> 8),
				(std::uint8_t)(lc_size >> 0)
			};
			_outBytes->append((const char*)lc_buff, sizeof(lc_buff));
		}
		_outBytes->append(_num);
	}
	static void _bind_pack_stream_(TxBlobString *_outBytes, const TxBlobWString &_num)
	{
		_bind_pack_stream_(_outBytes, _num.utf8Codecvt());
	}

	template<class NumType>
	void _pop_unpack_stream_(NumType *_outNum)
	{
		if (std::is_integral<NumType>::value || std::is_enum<NumType>::value || std::is_floating_point<NumType>::value)
		{
			const std::uint8_t *ptr = (std::uint8_t*)this->dataBuffer.data() + this->dataPos;
			if ((unsigned int)sizeof(NumType) != (unsigned int)(*ptr) || sizeof(NumType) + 1 > (int)(this->dataBuffer.size() - (int)this->dataPos))
			{
				assert(false);
				_throw_exception_(false);
			}
			if (sizeof(NumType) == sizeof(std::uint8_t) && sizeof(NumType) == 1)
			{
				*(std::uint8_t*)_outNum = ptr[1];
			}
			else if (sizeof(NumType) == sizeof(std::uint16_t))
			{
				*(std::uint16_t*)_outNum = ((std::uint16_t)ptr[1] << 8) | ((std::uint16_t)ptr[2] << 0);
			}
			else if (sizeof(NumType) == sizeof(std::uint32_t))
			{
				*(std::uint32_t*)_outNum = ((std::uint32_t)ptr[1] << 24) | ((std::uint32_t)ptr[2] << 16)
					| ((std::uint32_t)ptr[3] << 8) | ((std::uint32_t)ptr[4] << 0);
			}
			else if (sizeof(NumType) == sizeof(std::uint64_t))
			{
				*(std::uint64_t*)_outNum =
					(((std::uint64_t)(((std::uint32_t)ptr[1] << 24) | ((std::uint32_t)ptr[2] << 16) | ((std::uint32_t)ptr[3] << 8) | ((std::uint32_t)ptr[4] << 0))) << 32)
					| (std::uint64_t)(((std::uint32_t)ptr[5] << 24) | ((std::uint32_t)ptr[6] << 16) | ((std::uint32_t)ptr[7] << 8) | ((std::uint32_t)ptr[8] << 0));
			}
			else assert(false);
			this->dataPos += (int)sizeof(NumType) + 1;
		}
		else assert(false);
	}
	void _pop_unpack_stream_(TxBlobString *_outNum)
	{
		const std::uint8_t *ptr = (std::uint8_t*)this->dataBuffer.data() + this->dataPos;
		int iLcValidLen = (int)this->dataBuffer.size() - (int)this->dataPos;
		unsigned int i_head = (unsigned int)(*ptr);
		if (i_head == (unsigned int)0xff)
		{
			if (iLcValidLen < 5)
				_throw_exception_(false);
			i_head = (((std::uint32_t)ptr[1]) << 24)
				| (((std::uint32_t)ptr[2]) << 16)
				| (((std::uint32_t)ptr[3]) << 8)
				| (((std::uint32_t)ptr[4]) << 0);
			if ((std::uint32_t)iLcValidLen < i_head + 5 || i_head % sizeof(char) != 0)
				_throw_exception_(false);
			this->dataPos += i_head + 5;
			_outNum->assign((char*)&ptr[5], i_head / sizeof(char));
		}
		else
		{
			if (iLcValidLen < (int)i_head + 1 || i_head % sizeof(char) != 0) _throw_exception_(false);
			this->dataPos += i_head + 1;
			_outNum->assign((char*)&ptr[1], i_head / sizeof(char));
		}
	}
	void _pop_unpack_stream_(TxBlobWString *_outNum)
	{
		TxBlobString lc_outNum;
		this->_pop_unpack_stream_(&lc_outNum);
		*_outNum = lc_outNum.utf8Codecvt();
	}
public:
	TxSerializeArchive(TxSerializeArchive::EnumDataStreamType _etDataStreamType)
		:dataPos(0), etDataStreamType(_etDataStreamType)
	{
		assert(sizeof(std::uint8_t) == 1);
		assert(sizeof(std::uint16_t) == 2);
		assert(sizeof(std::uint32_t) == 4);
		assert(sizeof(std::uint64_t) == 8);
	}
	virtual ~TxSerializeArchive() { }
	inline TxSerializeArchive::EnumDataStreamType getDataStreamType() const
	{
		return this->etDataStreamType;
	}
	void setStreamBuffer(const TxBlobString &_data)
	{
		this->dataPos = 0;
		this->dataBuffer = _data;
	}
	void setStreamBuffer(const void *_data, std::size_t _size)
	{
		this->dataPos = 0;
		this->dataBuffer.assign((const char*)_data, (int)_size);
	}
	const TxBlobString& getStreamBuffer() const
	{
		return this->dataBuffer;
	}
	const unsigned char* getStreamData() const
	{
		return (const unsigned char*)this->dataBuffer.data();
	}
	int getStreamSize() const
	{
		return (int)this->dataBuffer.size();
	}
	template<class NumType>
	void bindObject(NumType &_num)
	{
		switch (this->etDataStreamType)
		{
		case EDST_package:
			_bind_pack_stream_(&this->dataBuffer, _num);
			break;
		case EDST_unpack:
			if ((unsigned int)this->dataPos == (unsigned int)this->dataBuffer.size())
				_throw_exception_(true);
			_pop_unpack_stream_(&_num);
			break;
		default:
			assert(false);
			break;
		}
	}
	//void block()
	//{
	//	TxBlobString lc_str_tem;
	//	this->bindObject(lc_str_tem);
	//	(void)&lc_str_tem;
	//}
	template<class TFUNC>
	void block(const TFUNC &_mfunc)
	{
		TxSerializeArchive lcSerializeArchive(this->etDataStreamType);
		switch (this->etDataStreamType)
		{
		case EDST_package:
			_mfunc(&lcSerializeArchive);
			this->bindObject(lcSerializeArchive.dataBuffer);
			break;
		case EDST_unpack:
			try {
				this->bindObject(lcSerializeArchive.dataBuffer);
				_mfunc(&lcSerializeArchive);
			}
			catch (TxSuccessException&) {
#ifndef NDEBUG
				int aaa = 0;
				++aaa;
#endif
			}
			break;
		default:
			assert(false);
			break;
		}
	}
};


//--------------------------------------TxShellPackage--------------------------------------//
//全包：crc32,num2
//分包：head2,crc4,len2
class TxShellPackage
{
private:
	enum { e_max_unit_size = 1024, e_head_ch0 = (unsigned int)0xac, e_head_ch1 = (unsigned int)0x58, };
	struct tagUnitData
	{
		unsigned char data[e_max_unit_size];
		int dataSize;
		unsigned int crc32;
		int lenght16;
		int bCompleteSign;
		enum {
			e_null = 0,
			e_head0,
			e_head1,
			e_crc0,
			e_crc1,
			e_crc2,
			e_crc3,
			e_len0,
			e_len1,
			e_data0,
		} eStatus;
		inline void reset() { this->eStatus = e_head0; }
		//inline void operator=(const tagUnitData&) { }
		//tagUnitData(const tagUnitData&) { }
		//tagUnitData() { }
		//void assign(const tagUnitData &_tem) { ::memcpy(this,&_tem,sizeof(*this)); }
	} mRecordUnitData;
	std::list<tagUnitData> listUnitData;
	int iCompleteCount;
private:
	static void _build_my_crc32_len(unsigned char _crc32[], const void *_data, int _len, const void *_data2 = NULL, int _len2 = 0)
	{
		if (_crc32 != NULL)
		{
			TxCryptoAlgorithm::TxCrc32 m_crc32_obj;
			m_crc32_obj.setFromUint32Value(_len + _len2);
			if (_data != NULL&&_len > 0)
				m_crc32_obj.pushData(_data, _len);
			if (_data2 != NULL&&_len2 > 0)
				m_crc32_obj.pushData(_data2, _len2);
			m_crc32_obj.getCrc32Buffer(_crc32);
		}
	}
	void _pop_result_(void(*_cb)(void*, unsigned int, const unsigned char*, int), void *_userdata)
	{
		if (this->listUnitData.size() > 0 && this->iCompleteCount > 0)
		{
			unsigned char __lc_buff[6 * 1024];
			unsigned char *pBuffPtr = &__lc_buff[0];
			int iBuffSize = 0;
			int iLcStrSize = 0;
			--this->iCompleteCount;
			for (std::list<tagUnitData>::iterator iter = this->listUnitData.begin();; iter++)
			{
				iLcStrSize += iter->dataSize;
				if (iter->bCompleteSign) break;
			}
			if (iLcStrSize > (int)sizeof(__lc_buff))
				pBuffPtr = new unsigned char[iLcStrSize + 32];
			for (std::list<tagUnitData>::iterator iter = this->listUnitData.begin();;)
			{
				assert(iBuffSize + iter->dataSize <= iLcStrSize);
				assert(iter->dataSize > 0);
				memcpy(&pBuffPtr[iBuffSize], iter->data, iter->dataSize);
				iBuffSize += iter->dataSize;
				int bLcCompleteSign = iter->bCompleteSign;
				iter = this->listUnitData.erase(iter);
				if (bLcCompleteSign)
					break;
			}
			assert((int)(iBuffSize) == (int)iLcStrSize);
			assert((int)(iBuffSize) >= (int)8);
			unsigned int lc_crc32 = 0;
			_build_my_crc32_len((unsigned char*)&lc_crc32, pBuffPtr + 4, iBuffSize - 4);
			if (*(unsigned int*)&lc_crc32 == *(unsigned int*)&pBuffPtr[0] && iBuffSize >= 8)
			{//校验通过
				_cb(_userdata,
					(((unsigned int)pBuffPtr[4]) << 24) | (((unsigned int)pBuffPtr[5]) << 16) | (((unsigned int)pBuffPtr[6]) << 8) | (((unsigned int)pBuffPtr[7]) << 0),
					pBuffPtr + 8, iBuffSize - 8);
			}
			else
				assert(false);
			if (pBuffPtr != &__lc_buff[0])
				delete[] pBuffPtr;
		}
	}
	void _pushData__(const unsigned char *_data, int _count)
	{
		for (bool lc_force_loop = false; lc_force_loop || _count > 0;)
		{
			lc_force_loop = false;
			switch (this->mRecordUnitData.eStatus)
			{
			case tagUnitData::e_head0:
				if ((unsigned int)*_data == e_head_ch0)
				{
					this->mRecordUnitData.eStatus = tagUnitData::e_head1;
					_data++;
					_count--;
				}
				else
				{
					assert(([]()->bool {static long long llllll = 0; return (llllll++) != 0; })());
					const unsigned char *pS = (const unsigned char*)::memchr(_data + 1, tagUnitData::e_head0, _count - 1);
					if (pS != NULL)
					{
						_count -= (int)(pS - _data);
						_data = pS;
					}
					else
					{
						_data += _count;
						_count = 0;
					}
				}
				break;
			case tagUnitData::e_head1:
				if ((unsigned int)*_data == e_head_ch1)
				{
					this->mRecordUnitData.eStatus = tagUnitData::e_crc0;
					_data++;
					_count--;
				}
				else
				{
					assert(false);
					this->mRecordUnitData.reset();
				}
				break;
			case tagUnitData::e_crc0:
				this->mRecordUnitData.crc32 = ((unsigned short)*_data) << 24;
				this->mRecordUnitData.eStatus = tagUnitData::e_crc1;
				_data++;
				_count--;
				break;
			case tagUnitData::e_crc1:
				this->mRecordUnitData.crc32 |= ((unsigned int)*_data) << 16;
				this->mRecordUnitData.eStatus = tagUnitData::e_crc2;
				_data++;
				_count--;
				break;
			case tagUnitData::e_crc2:
				this->mRecordUnitData.crc32 |= ((unsigned int)*_data) << 8;
				this->mRecordUnitData.eStatus = tagUnitData::e_crc3;
				_data++;
				_count--;
				break;
			case tagUnitData::e_crc3:
				this->mRecordUnitData.crc32 |= ((unsigned int)*_data);
				this->mRecordUnitData.eStatus = tagUnitData::e_len0;
				_data++;
				_count--;
				break;
			case tagUnitData::e_len0:
				this->mRecordUnitData.lenght16 = ((unsigned int)*_data) << 8;
				this->mRecordUnitData.eStatus = tagUnitData::e_len1;
				_data++;
				_count--;
				break;
			case tagUnitData::e_len1:
				this->mRecordUnitData.lenght16 |= ((unsigned int)*_data);
				this->mRecordUnitData.eStatus = tagUnitData::e_data0;
				_data++;
				_count--;
				this->mRecordUnitData.dataSize = 0;
				if ((int)this->mRecordUnitData.lenght16 > (int)e_max_unit_size)
				{
					assert(false);
					this->mRecordUnitData.reset();
				}
				else
				{
					lc_force_loop = true;
				}
				break;
			case tagUnitData::e_data0:
				if (true)
				{
					int iLcUnitSize = this->mRecordUnitData.lenght16 - this->mRecordUnitData.dataSize;
					if (iLcUnitSize > _count) iLcUnitSize = _count;
					assert(iLcUnitSize >= 0);
					if (iLcUnitSize > 0)
						memcpy(&this->mRecordUnitData.data[this->mRecordUnitData.dataSize], _data, iLcUnitSize);
					_data += iLcUnitSize;
					_count -= iLcUnitSize;
					this->mRecordUnitData.dataSize += iLcUnitSize;
					if (this->mRecordUnitData.lenght16 <= this->mRecordUnitData.dataSize)
					{
						unsigned char lc_crc32[4] = { 0 };
						_build_my_crc32_len(lc_crc32, this->mRecordUnitData.data, this->mRecordUnitData.lenght16);
						if (((((unsigned int)lc_crc32[0]) << 24)
							| (((unsigned int)lc_crc32[1]) << 16)
							| (((unsigned int)lc_crc32[2]) << 8)
							| (((unsigned int)lc_crc32[3]) << 0))
							== (unsigned int)this->mRecordUnitData.crc32)
						{
							if (this->mRecordUnitData.lenght16 <= 0)
							{
								if (this->listUnitData.size() > 0)
								{
									if (!this->listUnitData.back().bCompleteSign)
									{
										this->iCompleteCount++;
										this->listUnitData.back().bCompleteSign = 1;
									}
								}
							}
							else
							{
								this->mRecordUnitData.bCompleteSign = 0;
								this->listUnitData.push_back(this->mRecordUnitData);
							}
						}
						else
						{
							assert(([]()->bool {static long long llllll = 0; return (llllll++) != 0; })());
						}
						this->mRecordUnitData.reset();
					}
				}
				break;
			default:
				assert(false);
				break;
			}
		}
	}
	static int _build_package_node(unsigned char _out_pack_data[],
		const unsigned char *_data, int _count, const unsigned char *_data2, int _count2)
	{
		//head
		_out_pack_data[0] = (unsigned char)e_head_ch0;
		_out_pack_data[1] = (unsigned char)e_head_ch1;
		//crc32
		_build_my_crc32_len(&_out_pack_data[2], _data, _count, _data2, _count2);
		//length
		_out_pack_data[6] = (unsigned char)((unsigned int)(_count + _count2) >> 8);
		_out_pack_data[7] = (unsigned char)((unsigned int)(_count + _count2) >> 0);
		if (_count > 0) memcpy(&_out_pack_data[8], _data, _count);
		if (_count2 > 0) memcpy(&_out_pack_data[_count + 8], _data2, _count2);
		return _count + _count2 + 8;
	}
	static void _pack_(unsigned int _pkg_num, const unsigned char *_data, int _count, void(*_cb)(void*, const unsigned char*, int), void *_userdata)
	{
		//crc32--global
		unsigned char g_crc32_pkgnumber[8] = { 0 };
		int g_crc32_pkgnumber_count = sizeof(g_crc32_pkgnumber);
		g_crc32_pkgnumber[4] = (unsigned char)(_pkg_num >> 24);
		g_crc32_pkgnumber[5] = (unsigned char)(_pkg_num >> 16);
		g_crc32_pkgnumber[6] = (unsigned char)(_pkg_num >> 8);
		g_crc32_pkgnumber[7] = (unsigned char)(_pkg_num >> 0);
		_build_my_crc32_len(g_crc32_pkgnumber, &g_crc32_pkgnumber[4], 4, _data, _count);
		unsigned char __lc_buff[6 * 1024];
		unsigned char *org_buff = &__lc_buff[0];
		int iLcCalSizeII = _count + sizeof(g_crc32_pkgnumber);
		iLcCalSizeII = iLcCalSizeII + ((iLcCalSizeII + e_max_unit_size - 1) / e_max_unit_size) * 8 + 8;
		if ((int)sizeof(__lc_buff) < iLcCalSizeII)
			org_buff = new unsigned char[iLcCalSizeII + 32];
		unsigned char *pBuffPtr = org_buff;
		while (_count > 0)
		{
			int iLcUnitSize = g_crc32_pkgnumber_count + _count;
			if (iLcUnitSize > e_max_unit_size)
				iLcUnitSize = e_max_unit_size;
			iLcUnitSize -= g_crc32_pkgnumber_count;
			//build data node
			pBuffPtr += _build_package_node(pBuffPtr, g_crc32_pkgnumber, g_crc32_pkgnumber_count, _data, iLcUnitSize);
			_data += iLcUnitSize;
			_count -= iLcUnitSize;
			g_crc32_pkgnumber_count = 0;
		}
		//build empty node
		pBuffPtr += _build_package_node(pBuffPtr, NULL, 0, NULL, 0);
		assert((int)(pBuffPtr - org_buff) == iLcCalSizeII);
		_cb(_userdata, org_buff, (int)(pBuffPtr - org_buff));
		if (org_buff != (&__lc_buff[0]))
			delete[] org_buff;
	}
public:
	TxShellPackage()
	{
		TxCryptoAlgorithm::initEnv();
		this->iCompleteCount = 0;
		this->mRecordUnitData.reset();
		this->listUnitData.clear();
	}
	~TxShellPackage()
	{
	}
	void pushData(const void *_data, int _count)
	{
		return this->_pushData__((const unsigned char*)_data, _count);
	}
	void pushData(const TxBlobString &_data)
	{
		return this->_pushData__((const unsigned char*)_data.data(), (int)_data.size());
	}
	void popResult(void(*_cb)(void*, unsigned int, const unsigned char*, int), void *_userdata)
	{
		return this->_pop_result_(_cb, _userdata);
	}
	template<class TFUNC>
	void popResult(TFUNC &_mFunc)
	{
		class MFR
		{
			TFUNC *const pFunc;
		public:
			MFR(TFUNC *_pFunc) :pFunc(_pFunc) { }
			static void _g_call_back_pop_result(void *_userdata, unsigned int _pkg_num, const unsigned char *_data, int _size)
			{
				return (*((MFR*)_userdata)->pFunc)(_pkg_num, _data, _size);
			}
		} m_MFR(&_mFunc);
		return this->_pop_result_(&MFR::_g_call_back_pop_result, &m_MFR);
	}
	bool popResultString(TxBlobString *_outData, unsigned int *_pkg_num)
	{
		struct MFR
		{
			TxBlobString *outData;
			unsigned int *numPkg;
			bool *ret;
		public:
			static void _g_call_back_pop_result(void *_userdata, unsigned int _pkg_num, const unsigned char *_data, int _size)
			{
				*((MFR*)_userdata)->ret = true;
				*((MFR*)_userdata)->numPkg = _pkg_num;
				((MFR*)_userdata)->outData->assign((const char*)_data, _size);
			}
		} m_MFR;
		bool ret = false;
		m_MFR.outData = _outData;
		m_MFR.ret = &ret;
		m_MFR.numPkg = _pkg_num;
		this->_pop_result_(&MFR::_g_call_back_pop_result, &m_MFR);
		return ret;
	}
	std::shared_ptr<TxBlobString> popResultSharedString(unsigned int *_numPkg)
	{
		struct MFR
		{
			std::shared_ptr<TxBlobString> *ret;
			unsigned int *numPkg;
		public:
			static void _g_call_back_pop_result(void *_userdata, unsigned int _pkg_num, const unsigned char *_data, int _size)
			{
				((MFR*)_userdata)->ret->reset(new TxBlobString());
				(*((MFR*)_userdata)->ret)->assign((const char*)_data, _size);
				*((MFR*)_userdata)->numPkg = _pkg_num;
			}
		} m_MFR;
		std::shared_ptr<TxBlobString> ret;
		m_MFR.ret = &ret;
		m_MFR.numPkg = _numPkg;
		this->_pop_result_(&MFR::_g_call_back_pop_result, &m_MFR);
		return ret;
	}
	static void pack(std::uint32_t _num_pkg, const void *_data, int _count, void(*_cb)(void*, const unsigned char*, int), void *_userdata)
	{
		return _pack_(_num_pkg, (const unsigned char*)_data, _count, _cb, _userdata);
	}
	static void pack(std::uint32_t _num_pkg, const TxBlobString &_data, void(*_cb)(void*, const unsigned char*, int), void *_userdata)
	{
		return _pack_(_num_pkg, (const unsigned char*)_data.data(), (int)_data.size(), _cb, _userdata);
	}
	template<class TFUNC>
	static void pack(std::uint32_t _num_pkg, const void *_data, int _count, TFUNC &_mFunc)
	{
		class MFR
		{
			TFUNC *const pFunc;
		public:
			MFR(TFUNC *_pFunc) :pFunc(_pFunc) { }
			static void _g_call_back_pop_result(void *_userdata, const unsigned char *_data, int _size)
			{
				return (*((MFR*)_userdata)->pFunc)(_data, _size);
			}
		} m_MFR(&_mFunc);
		return _pack_(_num_pkg, (const unsigned char*)_data, _count, &MFR::_g_call_back_pop_result, &m_MFR);
	}
	template<class TFUNC>
	static void pack(std::uint32_t _num_pkg, const TxBlobString &_data, TFUNC &_mFunc)
	{
		return pack(_num_pkg, (const void*)_data.data(), (int)_data.size(), _mFunc);
	}
	static void pack(TxBlobString *_outData, std::uint32_t _num_pkg, const void *_data, int _count)
	{
		struct MFR
		{
			TxBlobString *outData;
			static void _g_call_back_pop_result(void *_userdata, const unsigned char *_data, int _size)
			{
				((MFR*)_userdata)->outData->assign((const char*)_data, _size);
			}
		} m_MFR;
		m_MFR.outData = _outData;
		return _pack_(_num_pkg, (const unsigned char*)_data, _count, &MFR::_g_call_back_pop_result, &m_MFR);
	}
	static void pack(TxBlobString *_outData, std::uint32_t _num_pkg, const TxBlobString &_data)
	{
		return pack(_outData, _num_pkg, (const void*)_data.data(), (int)_data.size());
	}
};

class TxStdlib : public _TxPrivateNs_::CStdlib
{
public:
	static void initEnv()
	{
		_TxPrivateNs_::CStdlib::initEnv();
		executeOnce([]() {
			_TxPrivateNs_::My_Aes128_::initEnv();
			_TxPrivateNs_::My_Md5_::initEnv();
			TxCryptoAlgorithm::initEnv();
			_TxPrivateNs_::_CTX_UTF8_TRANS_::_object_();
			TxBlobWString(L" ").utf8Codecvt().utf8Codecvt();
		});
	}
	static TxBlobString hexUpperString(const TxBlobString &_data, char _separator = 0)
	{
		return _TxPrivateNs_::CStdlib::hexUpperString(_data, _separator);
	}
	static TxBlobString hexLowerString(const TxBlobString &_data, char _separator)
	{
		return _TxPrivateNs_::CStdlib::hexLowerString(_data, _separator);
	}
	static TxBlobString calcMd5(const void* _data, int _size)
	{
		return _TxPrivateNs_::CStdlib::calcMd5(_data, _size);
	}
	template<class T_DATA>
	static TxBlobString calcMd5(const T_DATA &_data)
	{
		return _TxPrivateNs_::CStdlib::calcMd5(_data.c_str(), _data.size());
	}
	template<class T_DATA>
	static TxBlobString aes128Encode(const std::array<unsigned char, 16> &_aes_psw, const T_DATA &_data)
	{
		std::string ret;
		_TxPrivateNs_::CStdlib::aes128Encode(&ret, _aes_psw.data(), _data.data(), (int)_data.size());
		return ret;
	}
	template<class T_DATA>
	static TxBlobString aes128Encode(const unsigned char(&_aes_psw)[16], const T_DATA &_data)
	{
		std::string ret;
		_TxPrivateNs_::CStdlib::aes128Encode(&ret, _aes_psw, _data.data(), (int)_data.size());
		return ret;
	}
	template<class T_DATA>
	static TxBlobString aes128Decode(const std::array<unsigned char, 16> &_aes_psw, const T_DATA &_data)
	{
		std::string ret;
		_TxPrivateNs_::CStdlib::aes128Decode(&ret, _aes_psw.data(), _data.data(), (int)_data.size());
		return ret;
	}
	template<class T_DATA>
	static TxBlobString aes128Decode(const unsigned char(&_aes_psw)[16], const T_DATA &_data)
	{
		std::string ret;
		_TxPrivateNs_::CStdlib::aes128Decode(&ret, _aes_psw, _data.data(), (int)_data.size());
		return ret;
	}
	static TxBlobString base64Encode(const TxBlobString& _src, bool _bNewLine)
	{
		std::string ret;
		_TxPrivateNs_::CStdlib::base64Encode(&ret, _src.data(), _src.size(), _bNewLine);
		return ret;
	}
	static TxBlobString base64Decode(const TxBlobString& _src)
	{
		std::string ret;
		_TxPrivateNs_::CStdlib::base64Decode(&ret, _src.data(), _src.size());
		return ret;
	}
	static TxBlobString urlEncode(const TxBlobString& _text)
	{
		std::string ret;
		_TxPrivateNs_::CStdlib::urlEncode(&ret, _text.toStdString());
		return ret;
	}
	static TxBlobString urlDecode(const TxBlobString& _src)
	{
		std::string ret;
		_TxPrivateNs_::CStdlib::urlDecode(&ret, _src.toStdString());
		return ret;
	}
};

#endif

//
//#include <thread>
//#include <mutex>
//char buff_t[52400];
//TxBlobString g_str;
//std::mutex g_mutex;
//
//void __cb_thread_init_(void *_arg1, void *_arg2)
//{
//	TX_UNUSED(_arg1);
//	TX_UNUSED(_arg2);
//	srand((unsigned int)time(NULL));
//	for (int i = 0; i < (int)sizeof(buff_t); i++)
//		buff_t[i] = rand() % 4 == 0 ? 0 : (char)rand();
//}
//
//TxBlobString _getString()
//{
//	int i1 = rand() % sizeof(buff_t);
//	int i2 = rand() % sizeof(buff_t);
//	if (i1 > i2)
//	{
//		int a = i1;
//		i1 = i2;
//		i2 = a;
//	}
//	return TxBlobString(&buff_t[i1], (int)(&buff_t[i2] - &buff_t[i1]));
//}
//
//void __cb_thread_append_(void *_arg1, void *_arg2)
//{
//	TX_UNUSED(_arg1);
//	TX_UNUSED(_arg2);
//	for (;;)
//	{
//		TxBlobString str;
//		str.append(_getString());
//		str.append(_getString());
//		str.assign(_getString());
//		str.append(_getString());
//		str.append(_getString());
//		str.append(_getString());
//		str.append(_getString());
//		g_mutex.lock();
//		g_str.append(str);
//		if (g_str.size() > 1024 * 1024)
//			g_str.clear();
//		g_mutex.unlock();
//		str.append(_getString());
//		str.append(_getString());
//	}
//}
//
//void g_test_pack(void *, void *)
//{
//	srand((unsigned int)time(NULL));
//	for (int ttp = 0; ttp < 1024; ++ttp)
//	{
//		unsigned int nnnn;
//		TxShellPackage mPkg;
//		TxBlobString org_str = _getString();
//		org_str.append("abcdefg");
//		TxBlobString str_sri;
//		TxShellPackage::pack(&str_sri, -1, org_str);
//		int len1 = rand() % (str_sri.size());
//		int len2 = str_sri.size() - len1; if (len2 > 0) len2 = rand() % len2;
//		int len3 = str_sri.size() - len1 - len2; if (len3 > 0) len3 = rand() % len3;
//		int len4 = str_sri.size() - len1 - len2 - len3; if (len4 > 0) len4 = rand() % len4;
//		int len5 = str_sri.size() - len1 - len2 - len3 - len4;
//		int ookk = 0;
//		for (ookk = 0; ookk < rand() % 10 + 1; ookk++)
//		{
//			mPkg.pushData(str_sri.data(), len1);
//			mPkg.pushData(str_sri.data() + len1, len2);
//			mPkg.pushData(str_sri.data() + len1 + len2, len3);
//			mPkg.pushData(str_sri.data() + len1 + len2 + len3, len4);
//			mPkg.pushData(str_sri.data() + len1 + len2 + len3 + len4, len5);
//		}
//		while (ookk--)
//		{
//			TxBlobString outOOORG;
//			if (!mPkg.popResultString(&outOOORG, &nnnn))
//				assert(false);
//			if (outOOORG.size() != org_str.size() || memcmp(outOOORG.data(), org_str.data(), org_str.size()) != 0)
//			{
//				int adfs = 0;
//				assert(false);
//				TX_UNUSED(adfs);
//			}
//		}
//	}
//}
//
//int main()
//{
//	printf("ffdsfds-start\n");
//	std::vector<std::shared_ptr<std::thread>> vThread;
//	{
//		vThread.push_back(std::shared_ptr<std::thread>(new std::thread([](){__cb_thread_init_(NULL, NULL); })));
//	}
//	{
//		for (int i = 0; i < 10; i++)
//		{
//			vThread.push_back(std::shared_ptr<std::thread>(new std::thread([](){g_test_pack(NULL, NULL); })));
//		}
//	}
//	{
//		for (int i = 0; i < (int)vThread.size(); i++)
//			vThread[i]->join();
//	}
//	printf("ffdsfds-end\n");
//	return 0;
//}
//
