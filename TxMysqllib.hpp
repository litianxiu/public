#pragma once

#include "TxStdlib.hpp"
#include "TxSyslib.hpp"

#include <functional>
#include <vector>
#include <mysql.h>
#include <errmsg.h>

class TxMysqlWrap
{
public:
	class TxResultSet
	{
		friend TxMysqlWrap;
	private:
		std::vector<std::pair<TxBlobString, TxBlobString>> vctData;
	private:
		TxResultSet()
		{
		}
		~TxResultSet()
		{
		}
		void _resizeColumn__(int _colCount)
		{
			this->vctData.resize(_colCount);
		}
		void _setName__(int _idx, const TxBlobString &_name)
		{
			if (_idx < (int)this->vctData.size())
				this->vctData[_idx].first = _name;
			else
				assert(0);
		}
		void _setValue__(int _idx, const TxBlobString &_value)
		{
			if (_idx < (int)this->vctData.size())
				this->vctData[_idx].second = _value;
			else
				assert(0);
		}
	public:
		int columnCount() const
		{
			return (int)this->vctData.size();
		}
		TxBlobString getValue(const TxBlobString &_name) const
		{
			auto lc_arry = this->vctData.data();
			for (int i = (int)this->vctData.size(); i--;)
			{
				if (lc_arry[i].first == _name)
					return lc_arry[i].second;
			}
			return TxBlobString();
		}
	};
private:
	friend class TxResultSet;
	MYSQL m_mysql_tem_;
	MYSQL *p_mysql;
	MYSQL_STMT *p_stmt;
	bool bTransactionSign;
	bool bLockedSign;
	struct {
		TxBlobString buffer;
		TxBlobString bind;
	} args;
	struct {
		TxBlobString host;
		TxBlobString username;
		TxBlobString password;
		TxBlobString database;
		int port;
	} mOpenArgs;
	std::function<void(unsigned int _nErr, const char *_strErr)> funcExcptCallBack;
private:
	void assert_msg_(bool _btrue)
	{
		(void)_btrue; _btrue = _btrue;
		if (!_btrue)
		{
			if (this->funcExcptCallBack)
			{
				int lc_err_no = ::mysql_errno(this->p_mysql);
				const char *lc_err_str = ::mysql_error(this->p_mysql);
				if (lc_err_no != 0 || (lc_err_str != NULL&&lc_err_str[0] != 0))
				{
					if (lc_err_str == NULL)
						lc_err_str = "";
					this->funcExcptCallBack(lc_err_no, lc_err_str);
				}
			}
#ifndef NDEBUG
			const char *lc_err_str = NULL;
			unsigned int lc_err_no = 0;
			if (this->p_mysql != NULL)
			{
				lc_err_str = ::mysql_error(this->p_mysql);
				lc_err_no = ::mysql_errno(this->p_mysql);
			}
			assert(false);
			lc_err_str = NULL;
			lc_err_no = 0;
#endif
		}
	}
	bool __isConnected_()
	{
		if (this->p_mysql == NULL)
			return false;
		unsigned int lc_err_no = ::mysql_errno(this->p_mysql);
		switch (lc_err_no)
		{
		case CR_UNKNOWN_ERROR:
		case CR_SOCKET_CREATE_ERROR:
		case CR_CONNECTION_ERROR:
		case CR_CONN_HOST_ERROR:
		case CR_IPSOCK_ERROR:
		case CR_UNKNOWN_HOST:
		case CR_SERVER_GONE_ERROR:
		case CR_LOCALHOST_CONNECTION:
		case CR_WRONG_HOST_INFO:
		case CR_TCP_CONNECTION:
		case CR_SERVER_HANDSHAKE_ERR:
		case CR_SERVER_LOST:
		case CR_NAMEDPIPE_CONNECTION:
		case CR_NAMEDPIPEWAIT_ERROR:
		case CR_NAMEDPIPEOPEN_ERROR:
		case CR_NAMEDPIPESETSTATE_ERROR:
		case CR_WRONG_LICENSE:
			assert(false);
			return false;
			break;
		default:
			break;
		}
		return true;
	}
	bool _only_open_()
	{
		assert(this->p_mysql == NULL);
		this->p_mysql = &this->m_mysql_tem_;
		::memset(this->p_mysql, 0, sizeof(*this->p_mysql));
		::mysql_init(this->p_mysql);
		if (!::mysql_real_connect(this->p_mysql, this->mOpenArgs.host.c_str(), this->mOpenArgs.username.c_str(), this->mOpenArgs.password.c_str(),
			this->mOpenArgs.database.c_str(), this->mOpenArgs.port, NULL, 0))
		{
			this->assert_msg_(false);
			this->close();
			return false;
		}
		char lc_chs_sql[] = "SET NAMES 'utf8';";
		int lc_db_sign = ::mysql_real_query(this->p_mysql, lc_chs_sql, TxBlobString::txStringLength(lc_chs_sql));
		if (lc_db_sign != 0)
			this->assert_msg_(false);
		return true;
	}
	bool _transaction_(const std::function<bool(void)> &_mfunc, bool _b_recursive)
	{
		bool ret = false;
		this->assert_msg_(!this->bTransactionSign&&this->p_mysql != NULL);
		if (this->p_mysql == NULL)
			return false;
		if (this->executeSql("SET AUTOCOMMIT=0;"))
		{
			if (this->executeSql("BEGIN;"))
			{
				this->bTransactionSign = true;
				if (_mfunc())
				{//返回true提交数据
					if (!this->executeSql("COMMIT;"))
						this->assert_msg_(false);
				}
				else
				{//返回false数据回滚
					if (!this->executeSql("ROLLBACK;"))
						this->assert_msg_(false);
				}
				ret = true;
			}
			this->executeSql("SET AUTOCOMMIT=1;");
			this->bTransactionSign = false;
		}
		if (!ret)
		{
			this->assert_msg_(false);
			if (_b_recursive && !this->__isConnected_())
			{
				this->close();
				if (!this->_only_open_())
					return false;
				ret = this->_transaction_(_mfunc, false);
			}
		}
		return ret;
	}
	bool _db_atomic_lock_operation__(const TxBlobString &_tablename,
		const char *_swtReadWriteOp, const std::function<void()> &_mfunc, bool _b_recursive)
	{
		bool ret = false;
		this->assert_msg_(!this->bLockedSign);
		if (this->executeSql(TxBlobString("LOCK TABLES ") + _tablename + " " + _swtReadWriteOp + ";"))
		{
			this->bLockedSign = true;
			_mfunc();
			this->executeSql(TxBlobString("UNLOCK TABLES;"));
			this->bLockedSign = false;
			ret = true;
		}
		if (!ret)
		{
			this->assert_msg_(false);
			if (_b_recursive && !this->__isConnected_())
			{
				this->close();
				if (!this->_only_open_())
					return false;
				ret = this->_db_atomic_lock_operation__(_tablename, _swtReadWriteOp, _mfunc, false);
			}
		}
		this->assert_msg_(ret);
		return ret;
	}
public:
	TxMysqlWrap()
	{
		this->p_mysql = NULL;
		this->p_stmt = NULL;
		this->bLockedSign = false;
		this->bTransactionSign = false;
	}
	~TxMysqlWrap()
	{
		this->close();
	}
	void setExcptCallBack(const std::function<void(unsigned int _nErr, const char *_strErr)> &_func)
	{
		this->funcExcptCallBack = _func;
	}
	void close()
	{
		if (this->p_stmt != NULL)
		{
			::mysql_stmt_free_result(this->p_stmt);
			::mysql_stmt_close(this->p_stmt);
			this->p_stmt = NULL;
		}
		if (this->p_mysql != NULL)
		{
			::mysql_close(this->p_mysql);
			this->p_mysql = NULL;
		}
		this->args.buffer.clear();
		this->args.bind.clear();
		this->funcExcptCallBack = std::function<void(unsigned int _nErr, const char *_strErr)>();
	}
	void transaction(const std::function<bool(void)> &_mfunc)
	{
		this->_transaction_(_mfunc, true);
	}
	bool readLock(const TxBlobString &_tablename, const std::function<void()> &_mfunc)
	{
		return this->_db_atomic_lock_operation__(_tablename, "READ", _mfunc, true);
	}
	bool writeLock(const TxBlobString &_tablename, const std::function<void()> &_mfunc)
	{
		return this->_db_atomic_lock_operation__(_tablename, "WRITE", _mfunc, true);
	}
	bool open(const TxBlobString &_host, const TxBlobString &_username, const TxBlobString &_password, const TxBlobString &_database, int _port)
	{
		this->mOpenArgs.host = _host;
		this->mOpenArgs.username = _username;
		this->mOpenArgs.password = _password;
		this->mOpenArgs.database = _database;
		this->mOpenArgs.port = _port;
		this->close();
		return this->_only_open_();
	}
	bool beginStmt(const TxBlobString &_sql, bool _b_recursive = true)
	{
		if (this->p_stmt == NULL)
		{
			this->p_stmt = ::mysql_stmt_init(this->p_mysql);
		}
		else
		{
			::mysql_stmt_free_result(this->p_stmt);
			::mysql_stmt_reset(this->p_stmt);
		}
		this->args.buffer.clear();
		this->args.bind.clear();
		int lc_err = ::mysql_stmt_prepare(this->p_stmt, _sql.c_str(), TxBlobString::txStringLength(_sql.c_str()));
		if (lc_err != 0)
		{
			this->assert_msg_(false);
			if (_b_recursive && !this->__isConnected_())
			{
				this->close();
				if (!this->_only_open_())
					return false;
				return this->beginStmt(_sql, false);
			}
		}
		return lc_err == 0;
	}
	void bindParamFloat(long double _value)
	{
		double lc_value = (double)_value;
		MYSQL_BIND lc_bind = { 0 };
		lc_bind.buffer_type = MYSQL_TYPE_DOUBLE;
		lc_bind.buffer = (void*)(size_t)this->args.buffer.size();
		lc_bind.buffer_length = sizeof(lc_value);
		lc_bind.is_null = 0;
		this->args.bind.append((char*)&lc_bind, sizeof(lc_bind));
		this->args.buffer.append((char*)&lc_value, sizeof(lc_value));
	}
	void bindParamInt(long long _value)
	{
		long long lc_value = _value;
		MYSQL_BIND lc_bind = { 0 };
		lc_bind.buffer_type = MYSQL_TYPE_LONGLONG;
		lc_bind.buffer = (void*)(size_t)this->args.buffer.size();
		lc_bind.buffer_length = sizeof(lc_value);
		lc_bind.is_null = 0;
		this->args.bind.append((char*)&lc_bind, sizeof(lc_bind));
		this->args.buffer.append((char*)&lc_value, sizeof(lc_value));
	}
	void bindParamString(const TxBlobString &_value)
	{
		MYSQL_BIND lc_bind = { 0 };
		lc_bind.buffer_type = MYSQL_TYPE_STRING;
		lc_bind.buffer = (void*)(size_t)this->args.buffer.size();
		lc_bind.buffer_length = _value.size();
		lc_bind.is_null = 0;
		this->args.bind.append((char*)&lc_bind, sizeof(lc_bind));
		this->args.buffer.append(_value);
	}
	void bindParamBlob(const TxBlobString &_value)
	{
		MYSQL_BIND lc_bind = { 0 };
		lc_bind.buffer_type = MYSQL_TYPE_BLOB;
		lc_bind.buffer = (void*)(size_t)this->args.buffer.size();
		lc_bind.buffer_length = _value.size();
		lc_bind.is_null = 0;
		this->args.bind.append((char*)&lc_bind, sizeof(lc_bind));
		this->args.buffer.append(_value);
	}
	long long endStmt()
	{
		long long ret = -1;
		this->assert_msg_(this->p_stmt != NULL);
		if (this->p_stmt != NULL)
		{
			for (int i = this->args.bind.size() / sizeof(MYSQL_BIND); i--;)
			{
				MYSQL_BIND *p_lc_bind = (MYSQL_BIND*)this->args.bind.data() + i;
				p_lc_bind->buffer = (char*)this->args.buffer.data() + (size_t)p_lc_bind->buffer;
			}
			int lc_bind_count = (int)::mysql_stmt_param_count(this->p_stmt);
			if (lc_bind_count * sizeof(MYSQL_BIND) != (int)this->args.bind.size())
			{
				this->assert_msg_(false);
			}
			else if (0 == ::mysql_stmt_bind_param(this->p_stmt, (MYSQL_BIND*)this->args.bind.data())
				&& 0 == ::mysql_stmt_execute(this->p_stmt))
			{
				ret = (long long)::mysql_stmt_affected_rows(this->p_stmt);
			}
			else
			{
				this->assert_msg_(false);
			}
			this->args.buffer.clear();
			this->args.bind.clear();
		}
		return ret;
	}
	bool executeSql(const TxBlobString &_sql, const std::function<bool(const TxMysqlWrap::TxResultSet*)> &_mfunc
		= std::function<bool(const TxMysqlWrap::TxResultSet*)>(), bool _b_recursive = true)
	{
		bool ret = false;
		if (0 == ::mysql_real_query(this->p_mysql, _sql.c_str(), TxBlobString::txStringLength(_sql.c_str())))
		{
			TxResultSet lc_res;
			MYSQL_RES *lc_pResultSet = ::mysql_store_result(this->p_mysql);
			if (lc_pResultSet != NULL)
			{
				lc_res._resizeColumn__((int)::mysql_num_fields(lc_pResultSet));
				for (int iii = 0;; ++iii)
				{
					MYSQL_FIELD *lc_fd = ::mysql_fetch_field(lc_pResultSet);
					if (lc_fd == NULL)
						break;
					lc_res._setName__(iii, TxBlobString(lc_fd->name));
				}
				for (;;)
				{
					//获取逐个字段
					MYSQL_ROW lc_row = ::mysql_fetch_row(lc_pResultSet);
					unsigned long *lc_lens = ::mysql_fetch_lengths(lc_pResultSet);
					if (lc_row == NULL || lc_lens == NULL)
						break;
					for (int i = 0; i < (int)lc_res.columnCount(); ++i)
						lc_res._setValue__(i, TxBlobString(lc_row[i], (int)lc_lens[i]));
					if (_mfunc(&lc_res))
						break;
				}
				//释放结果集内容
				::mysql_free_result(lc_pResultSet);
			}
			ret = true;
		}
		else
		{
			if (_b_recursive && !this->__isConnected_())
			{
				this->close();
				if (!this->_only_open_())
					return false;
				return this->executeSql(_sql, _mfunc, false);
			}
			this->assert_msg_(false);
		}
		return ret;
	}
};
