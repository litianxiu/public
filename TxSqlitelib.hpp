#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <list>
#include <memory>
#include <cstring>
#include <cstdint>

#include "TxStdlib.hpp"
#include "TxSyslib.hpp"
#include "TxBoostlib.hpp"
#include <sqlite3.h>


namespace _TxPrivateNs_ {
	class MySqlitelib;
	class MySqliteResultSet
	{
		friend MySqlitelib;
	private:
		const char* arryColumnName[32];
		size_t columnCount;
		sqlite3_stmt *pstmt;
	private:
		MySqliteResultSet()
		{
			this->_clear_();
		}
		~MySqliteResultSet()
		{
			this->_clear_();
		}
		void _reset_(sqlite3_stmt *_pstmt)
		{
			this->_clear_();
			if (_pstmt != NULL)
			{
				this->pstmt = _pstmt;
				//this->columnCount = ::sqlite3_column_count(this->pstmt);
				for (this->columnCount = 0;; ++this->columnCount)
				{
					if (this->columnCount >= (size_t)TxStdlib::arrayLength(this->arryColumnName))
						break;
					this->arryColumnName[this->columnCount] = ::sqlite3_column_name(this->pstmt, (int)this->columnCount);
					if (this->arryColumnName[this->columnCount] == NULL)
						break;
					if (this->arryColumnName[this->columnCount][0] == 0)
						break;
				}
			}
		}
		void _clear_()
		{
			this->columnCount = 0;
			this->pstmt = NULL;
		}
		template<class TFUNC>
		bool _getColumnValue_(const TxBlobString &_name, const TFUNC &_mfunc) const
		{
			if (this->columnCount <= 0 || this->pstmt == NULL)
				return false;
			for (int i = (int)this->columnCount; i--;)
			{
				if (_name.operator==(this->arryColumnName[i]))
					return _mfunc(i);
			}
			return false;
		}
	public:
		//Blob
		bool getValueBlob(TxBlobString *_blob, const TxBlobString &_name) const
		{
			_blob->clear();
			return this->_getColumnValue_(_name, [this, _blob](int _iCol)->bool {
				const char *lc_ptr = (const char*)::sqlite3_column_blob(this->pstmt, _iCol);
				if (lc_ptr == NULL)
					return true;
				int lc_len = ::sqlite3_column_bytes(this->pstmt, _iCol);
				if (lc_len < 0)
					return false;
				_blob->assign(lc_ptr, lc_len);
				return true;
			});
		}
		TxBlobString getValueBlobRet(const TxBlobString &_name,
			const TxBlobString &_defaultBlob = _TxPrivateNs_::TDefaultValRef<TxBlobString>::get_())
		{
			TxBlobString ret;
			if (this->getValueBlob(&ret, _name))
				return ret;
			if (&_defaultBlob == &_TxPrivateNs_::TDefaultValRef<TxBlobString>::get_())
				return ret;
			return _defaultBlob;
		}
		//Text
		bool getValueText(TxBlobString *_text, const TxBlobString &_name) const
		{
			_text->clear();
			return this->_getColumnValue_(_name, [this, _text](int _iCol)->bool {
				const char *lc_ptr = (const char*)::sqlite3_column_text(this->pstmt, _iCol);
				if (lc_ptr == NULL)
					return false;
				int lc_len = ::sqlite3_column_bytes(this->pstmt, _iCol);
				if (lc_len < 0)
					return false;
				_text->assign(lc_ptr, lc_len);
				return true;
			});
		}
		TxBlobString getValueTextRet(const TxBlobString &_name,
			const TxBlobString &_defaultText = _TxPrivateNs_::TDefaultValRef<TxBlobString>::get_())
		{
			TxBlobString ret;
			if (this->getValueText(&ret, _name))
				return ret;
			if (&_defaultText == &_TxPrivateNs_::TDefaultValRef<TxBlobString>::get_())
				return ret;
			return _defaultText;
		}
		//Double
		bool getValueDouble(double *_val, const TxBlobString &_name) const
		{
			return this->_getColumnValue_(_name, [this, _val](int _iCol)->bool {
				*_val = ::sqlite3_column_double(this->pstmt, _iCol);
				return true;
			});
		}
		double getValueDoubleRet(const TxBlobString &_name,
			const double &_defaultVal = _TxPrivateNs_::TDefaultValRef<double>::get_())
		{
			double ret = 0;
			if (this->getValueDouble(&ret, _name))
				return ret;
			if (&_defaultVal == &_TxPrivateNs_::TDefaultValRef<double>::get_())
				return ret;
			return _defaultVal;
		}
		//Int64
		bool getValueInt64(std::int64_t *_val, const TxBlobString &_name) const
		{
			return this->_getColumnValue_(_name, [this, _val](int _iCol)->bool {
				*_val = (std::int64_t)::sqlite3_column_int64(this->pstmt, _iCol);
				return true;
			});
		}
		std::int64_t getValueInt64Ret(const TxBlobString &_name,
			const std::int64_t &_defaultVal = _TxPrivateNs_::TDefaultValRef<std::int64_t>::get_())
		{
			std::int64_t ret = 0;
			if (this->getValueInt64(&ret, _name))
				return ret;
			if (&_defaultVal == &_TxPrivateNs_::TDefaultValRef<std::int64_t>::get_())
				return ret;
			return _defaultVal;
		}
		//int
		bool getValueInt(int *_val, const TxBlobString &_name) const
		{
			return this->_getColumnValue_(_name, [this, _val](int _iCol)->bool {
				*_val = ::sqlite3_column_int(this->pstmt, _iCol);
				return true;
			});
		}
		int getValueIntRet(const TxBlobString &_name,
			const int &_defaultVal = _TxPrivateNs_::TDefaultValRef<int>::get_())
		{
			int ret = 0;
			if (this->getValueInt(&ret, _name))
				return ret;
			if (&_defaultVal == &_TxPrivateNs_::TDefaultValRef<int>::get_())
				return ret;
			return _defaultVal;
		}
	};
	class MySqlitelib : private TxForbidDirectCopyObject
	{
	private:
		sqlite3 *p_sqlite3;
		sqlite3_stmt *pBindStmt;
		std::list<TxBlobString> recordBindBufStr;
		int iBindParamIdx;
		bool bDbDamageSign;
		bool b_db_transaction;
	private:
		void _clearBindStmt_()
		{
			if (this->pBindStmt != NULL)
			{
				int lc_resCode = ::sqlite3_finalize(this->pBindStmt);
				if (lc_resCode != SQLITE_OK)
					sysLogError("sqlite3_finalize()£¬´íÎóÂë:[{0}]").args(lc_resCode);
				this->pBindStmt = NULL;
			}
			this->iBindParamIdx = 0;
			this->recordBindBufStr.clear();
		}
	public:
		MySqlitelib()
		{
			this->p_sqlite3 = NULL;
			this->pBindStmt = NULL;
			this->bDbDamageSign = false;
			this->iBindParamIdx = 0;
			this->b_db_transaction = false;
			this->_clearBindStmt_();
			this->closeDb();
		}
		~MySqlitelib()
		{
			this->closeDb();
		}
		void closeDb()
		{
			this->b_db_transaction = false;
			this->_clearBindStmt_();
			if (this->p_sqlite3 != NULL)
			{
				::sqlite3_close(this->p_sqlite3);
				this->p_sqlite3 = NULL;
			}
		}
		void _checkDbDamageStatus(int _err)
		{
			switch (_err)
			{
			case SQLITE_NOTADB:
				this->bDbDamageSign = true;
				break;
			default:
				break;
			}
			if (_err != SQLITE_OK && _err != SQLITE_ROW && _err != SQLITE_DONE&&_err != SQLITE_ABORT)
				sysLogError("sqlite3_prepare()£¬´íÎóÂë:[{0}]").args(_err);
		}
		bool isDatabaseDamage() const
		{
			return this->bDbDamageSign;
		}
		bool openDb(const wchar_t *_filename, bool _safety_slow)
		{
			this->closeDb();
			TxBlobWString lc_file_dir = TxBoostlib::fileDirectory(_filename);
			if (!TxBoostlib::isDirectory(lc_file_dir))
			{
				TxBoostlib::deleteAllFile(lc_file_dir);
				TxBoostlib::createDirectories(lc_file_dir);
			}
			this->bDbDamageSign = false;
			this->_checkDbDamageStatus(::sqlite3_open16(_filename, &this->p_sqlite3));
			if (this->p_sqlite3 != NULL)
			{
				if (_safety_slow)
					this->exec_sql("PRAGMA synchronous = FULL; PRAGMA temp_store = MEMORY;", 1);
				else
					this->exec_sql("PRAGMA synchronous = OFF; PRAGMA temp_store = MEMORY;", 1);
				if (!this->bDbDamageSign)
				{
					this->exec_sql("select name from sqlite_master;", 1,
						[](int, char**, char**) { return true; });
				}
			}
			return this->p_sqlite3 != NULL;
		}
		bool sureDictionaryTable(const TxBlobString &_tableName, bool _bCheckExist)
		{
			if (_bCheckExist)
			{
				if (this->isExistTable(_tableName))
					return true;
			}
			TxBlobString lc_sql;
			lc_sql.compose("create table if not exists {0} (id TEXT PRIMARY KEY,data BLOB);").args(_tableName);
			return this->exec_sql(lc_sql, 1);
		}
		template<class TFUNC>
		bool selectDictionary(const TxBlobString &_tableName, const TxBlobString &_id, const TFUNC &_mFunc)
		{
			if (this->p_sqlite3 == NULL)
			{
				sysLogError("if (this->p_sqlite3 == NULL)Ö´ÐÐÊ§°Ü");
				return false;
			}
			TxBlobString lc_sql;
			lc_sql.compose("select * from {0} ").args(_tableName);
			if (_id.length() > 0)
				lc_sql.appendCompose(" where id=\'{0}\'").args(_id);
			return this->querySql(lc_sql,
				[this, &_mFunc](TxSqlitelib::TxResultSet *_retSet)->bool {
				TxBlobString lc_id, lc_blob;
				if (_retSet->getValueText(&lc_id, "id") && _retSet->getValueBlob(&lc_blob, "data"))
					_mFunc(lc_id, lc_blob);
				return true;
			});
		}
		bool readDictionary(TxBlobString *_value, const TxBlobString &_table, const TxBlobString &_key)
		{
			bool ret = false;
			this->selectDictionary(_table, _key,
				[&_value, &_key, &ret](const TxBlobString &_db_key, const TxBlobString &_db_value)->bool {
				if (_db_key == _key)
				{
					*_value = _db_value;
					ret = true;
					return false;
				}
				return true;
			});
			return ret;
		}
		bool writeDictionary(const TxBlobString &_table, const TxBlobString &_id, const TxBlobString &_data)
		{
			if (this->p_sqlite3 == NULL)
			{
				sysLogError("if (this->p_sqlite3 == NULL)Ö´ÐÐÊ§°Ü");
				return false;
			}
			if (this->b_db_transaction)
			{
				TxBlobString lc_sql;
				lc_sql.compose("replace info {0}(id,data) values(\'{1}',?);").args(_table, _id);
				for (int ii = 0;; ++ii)
				{
					if (this->beginStmt(lc_sql))
					{
						this->bindParamBlob(_data);
						if (this->endStmt())
							return true;
					}
					if (ii != 0)
						break;
					this->sureDictionaryTable(lc_sql, 0);
				}
			}
			else
			{
				TxBlobString lc_sql;
				bool lc_same_sign = false;
				bool lc_exist_sign = false;
				if (!this->selectDictionary(_table, _id,
					[this, &_id, &_data, &lc_same_sign, &lc_exist_sign](
						const TxBlobString &_db_key, const TxBlobString &_db_value)->bool {
					if (_db_key == _id)
					{
						lc_exist_sign = true;
						if (_db_value == _data)
							lc_same_sign = true;
						return false;
					}
					return true;
				})) {
					if (!lc_exist_sign)
						this->sureDictionaryTable(_table, 0);
				}
				if (lc_same_sign)
					return true;
				if (lc_exist_sign) lc_sql.compose("update {0} set data=? where id=\'{1}\';").args(_table, _id);
				else lc_sql.compose("insert into {0}(id,data) values(\'{1}',?);").args(_table, _id);
				if (this->beginStmt(lc_sql))
				{
					this->bindParamBlob(_data);
					if (this->endStmt())
						return true;
				}
			}
			return false;
		}
		bool deleteDictionary(const TxBlobString &_table, const TxBlobString &_key)
		{
			if (this->p_sqlite3 == NULL)
			{
				sysLogError("if (this->p_sqlite3 == NULL)Ö´ÐÐÊ§°Ü");
				return false;
			}
			TxBlobString lc_sql;
			lc_sql.compose("delete from {0} where id = \'{1}\' ;").args(_table, _key);
			return this->exec_sql(lc_sql, 1);
		}
		bool exec_sql(const TxBlobString &_sql, int _bAssert,
			const std::function<bool(int, char**, char**)> &_mfunc = std::function<bool(int, char**, char**)>())
		{
			int lc_resCode = SQLITE_OK;
			char *lc_err_msg = NULL;
			if (!_mfunc)
				this->_checkDbDamageStatus(lc_resCode = ::sqlite3_exec(this->p_sqlite3, _sql.c_str(), NULL, NULL, &lc_err_msg));
			else
			{
				struct TYY {
					const std::function<bool(int, char**, char**)> *p_fn;
					static int g_cb(void *_userdata, int _n, char **_value, char **_key) {
						if ((*((TYY*)_userdata)->p_fn)(_n, _key, _value))
							return 0;
						return -1;
					}
				} m_tyy;
				m_tyy.p_fn = &_mfunc;
				this->_checkDbDamageStatus(lc_resCode = ::sqlite3_exec(this->p_sqlite3, _sql.c_str(),
					&TYY::g_cb, &m_tyy, &lc_err_msg));
			}
			if (lc_resCode != SQLITE_OK&&lc_resCode != SQLITE_ABORT && !!_bAssert)
			{
				sysLogError("Ö´ÐÐÊý¾Ý¿âÓï¾äÊ§°Ü¡£sqlite3_exec()£¬´íÎóÂë:[{0}]£¬´íÎóÎÄ±¾:[{1}]£¬sql:[{2}]").args(
					lc_resCode, TxBlobString(lc_err_msg), _sql);
			}
			return lc_resCode == SQLITE_OK;
		}
		bool isExistTable(const TxBlobString &_tableName)
		{
			bool ret = false;
			TxBlobString lc_sql;
			lc_sql.compose("select name from sqlite_master where name=\'{0}\';").args(_tableName);
			this->exec_sql(lc_sql, 0, [&ret, &_tableName](int _n, char **_key, char **_value)->bool {
				for (int i = 0; i < _n; ++i)
				{
					if (std::strcmp(_key[i], "name") == 0 && std::strcmp(_value[i], _tableName.c_str()) == 0)
					{
						ret = true;
						return false;
					}
				}
				return true;
			});
			return ret;
		}
		bool transaction(const std::function<bool(void)> &_mfunc)
		{
			if (this->p_sqlite3 == NULL)
			{
				sysLogError("if (this->p_sqlite3 == NULL)Ö´ÐÐÊ§°Ü");
				return false;
			}
			if (this->exec_sql("begin", 1))
			{
				this->b_db_transaction = true;
				bool lc_sign = _mfunc();
				this->b_db_transaction = false;
				if (lc_sign)
					return this->exec_sql("commit", 1);
				this->exec_sql("rollback", 1);
			}
			return false;
		}
		template<class TFUNC>
		bool querySql(const TxBlobString &_sql, const TFUNC &_mfunc)
		{
			bool ret = false;
			if (this->p_sqlite3 == NULL)
			{
				sysLogError("if (this->p_sqlite3 == NULL)Ö´ÐÐÊ§°Ü");
				return false;
			}
			this->_clearBindStmt_();
			sqlite3_stmt *lc_pstmt = NULL;
			int lc_resCode = ::sqlite3_prepare(this->p_sqlite3, _sql.c_str(), _sql.length(), &lc_pstmt, NULL);
			this->_checkDbDamageStatus(lc_resCode);
			if (lc_resCode == SQLITE_OK&&lc_pstmt != NULL)
			{
				MySqliteResultSet lcSqlteRes;
				ret = true;
				for (;;)
				{
					lc_resCode = ::sqlite3_step(lc_pstmt);
					if (lc_resCode != SQLITE_ROW)
						break;
					lcSqlteRes._reset_(lc_pstmt);
					if (!_mfunc(&lcSqlteRes))
						break;
				}
			}
			if (lc_pstmt != NULL)
			{
				::sqlite3_finalize(lc_pstmt);
				lc_pstmt = NULL;
			}
			return ret;
		}
		bool beginStmt(const TxBlobString &_sql)
		{
			this->_clearBindStmt_();
			if (this->p_sqlite3 == NULL)
			{
				sysLogError("if (this->p_sqlite3 == NULL)Ö´ÐÐÊ§°Ü");
				return false;
			}
			this->iBindParamIdx = 0;
			this->recordBindBufStr.push_back(_sql);
			int lc_resCode = ::sqlite3_prepare(this->p_sqlite3,
				this->recordBindBufStr.back().c_str(), this->recordBindBufStr.back().length(), &this->pBindStmt, NULL);
			this->_checkDbDamageStatus(lc_resCode);
			if (lc_resCode == SQLITE_OK&&this->pBindStmt != NULL)
				return true;
			this->_clearBindStmt_();
			return false;
		}
		bool endStmt()
		{
			if (this->pBindStmt == NULL)
			{
				sysLogError("if(this->pBindStmt == NULL)£¬²ÎÊý´íÎó¡£");
				return false;
			}
			int lc_resCode = ::sqlite3_step(this->pBindStmt);
			this->_clearBindStmt_();
			if (lc_resCode != SQLITE_DONE)
			{
				sysLogError("sqlite3_step()£¬´íÎóÂë:[{0}]").args(lc_resCode);
				return false;
			}
			return true;
		}
		bool bindParamBlob(const TxBlobString &_value)
		{
			if (this->pBindStmt == NULL)
			{
				sysLogError("if(this->pBindStmt == NULL)£¬²ÎÊý´íÎó¡£");
				return false;
			}
			this->recordBindBufStr.push_back(_value);
			int lc_resCode = ::sqlite3_bind_blob(this->pBindStmt, ++this->iBindParamIdx,
				this->recordBindBufStr.back().data(), this->recordBindBufStr.back().size(), NULL);
			if (lc_resCode != SQLITE_OK)
			{
				sysLogError("sqlite3_bind_blob()£¬´íÎóÂë:[{0}]£¬this->iBindParamIdx:[{1}]").args(
					lc_resCode, this->iBindParamIdx);
				this->_clearBindStmt_();
				return false;
			}
			return true;
		}
		bool bindParamText(const TxBlobString &_value)
		{
			if (this->pBindStmt == NULL)
			{
				sysLogError("if(this->pBindStmt == NULL)£¬²ÎÊý´íÎó¡£");
				return false;
			}
			this->recordBindBufStr.push_back(_value);
			int lc_resCode = ::sqlite3_bind_text(this->pBindStmt, ++this->iBindParamIdx,
				this->recordBindBufStr.back().data(), this->recordBindBufStr.back().size(), NULL);
			if (lc_resCode != SQLITE_OK)
			{
				sysLogError("sqlite3_bind_text()£¬´íÎóÂë:[{0}]£¬this->iBindParamIdx:[{1}]").args(
					lc_resCode, this->iBindParamIdx);
				this->_clearBindStmt_();
				return false;
			}
			return true;
		}
		bool bindParamDouble(double _value)
		{
			if (this->pBindStmt == NULL)
			{
				sysLogError("if(this->pBindStmt == NULL)£¬²ÎÊý´íÎó¡£");
				return false;
			}
			int lc_resCode = ::sqlite3_bind_double(this->pBindStmt, ++this->iBindParamIdx, _value);
			if (lc_resCode != SQLITE_OK)
			{
				sysLogError("sqlite3_bind_double()£¬´íÎóÂë:[{0}]£¬this->iBindParamIdx:[{1}]").args(
					lc_resCode, this->iBindParamIdx);
				this->_clearBindStmt_();
				return false;
			}
			return true;
		}
		bool bindParamInt(int _value)
		{
			if (this->pBindStmt == NULL)
			{
				sysLogError("if(this->pBindStmt == NULL)£¬²ÎÊý´íÎó¡£");
				return false;
			}
			int lc_resCode = ::sqlite3_bind_int(this->pBindStmt, ++this->iBindParamIdx, _value);
			if (lc_resCode != SQLITE_OK)
			{
				sysLogError("sqlite3_bind_int()£¬´íÎóÂë:[{0}]£¬this->iBindParamIdx:[{1}]").args(
					lc_resCode, this->iBindParamIdx);
				this->_clearBindStmt_();
				return false;
			}
			return true;
		}
		bool bindParamInt64(std::int64_t _value)
		{
			if (this->pBindStmt == NULL)
			{
				sysLogError("if(this->pBindStmt == NULL)£¬²ÎÊý´íÎó¡£");
				return false;
			}
			int lc_resCode = ::sqlite3_bind_int64(this->pBindStmt, ++this->iBindParamIdx, _value);
			if (lc_resCode != SQLITE_OK)
			{
				sysLogError("sqlite3_bind_int64()£¬´íÎóÂë:[{0}]£¬this->iBindParamIdx:[{1}]").args(
					lc_resCode, this->iBindParamIdx);
				this->_clearBindStmt_();
				return false;
			}
			return true;
		}
	};
}

class TxSqlitelib
{
public:
	typedef _TxPrivateNs_::MySqliteResultSet TxResultSet;
private:
	std::shared_ptr<TxSysProcessMutex<false>> spProcessMutex;
	_TxPrivateNs_::MySqlitelib m_Sqlitelib;
public:
	TxSqlitelib()
	{
	}
	~TxSqlitelib()
	{
		this->close();
	}
	bool open(const TxBlobWString &_wFileName, bool _safety_slow,
		const std::function<void()> &_fnDbDamage = std::function<void()>())
	{
		bool ret = false;
		this->close();

		TxBlobWString lc_db_file = TxBoostlib::completePath(_wFileName);
		while (lc_db_file.length() > 0 && lc_db_file.backChar() == L'\\')
			lc_db_file.pop_back(1);

		if (lc_db_file.length() <= 0)
			return false;

		TxBlobString lc_proc_mutex_key = TxStdlib::calcMd5(
			TxBlobString("Global.TxSqliteKvdb.proc.mutex.Key.ext--")
			+ lc_db_file.toLowerCase().utf8Codecvt()).toLowerCase();

		if (this->spProcessMutex)
		{
			this->spProcessMutex->unlock();
			this->spProcessMutex.reset();
		}
		this->spProcessMutex = std::make_shared<TxSysProcessMutex<false>>(lc_proc_mutex_key);
		this->spProcessMutex->lock();

		for (int lc_times = 0;;)
		{
			ret = this->m_Sqlitelib.openDb(lc_db_file.c_str(), _safety_slow);
			if ((++lc_times) < 2 && this->m_Sqlitelib.isDatabaseDamage())
			{
				this->m_Sqlitelib.closeDb();
				ret = false;
				if (_fnDbDamage)
				{
					_fnDbDamage();
					break;
				}
				TxBoostlib::deleteAllFile(lc_db_file);
				sysLogError("É¾³ýÊý¾Ý¿â¡£lc_db_file=[{0}]").args(lc_db_file);
				continue;
			}
			break;
		}
		if (!ret)
		{
			sysLogError("´ò¿ªÊý¾Ý¿â´íÎó¡£");
			this->close();
		}
		return ret;
	}
	void close()
	{
		this->m_Sqlitelib.closeDb();
		if (this->spProcessMutex)
		{
			this->spProcessMutex->unlock();
			this->spProcessMutex.reset();
		}
	}
	bool isExistTable(const TxBlobString &_tableName)
	{
		return this->m_Sqlitelib.isExistTable(_tableName);
	}
	bool sureDictionaryTable(const TxBlobString &_tableName)
	{
		return this->m_Sqlitelib.sureDictionaryTable(_tableName, 1);
	}
	bool writeDictionary(const TxBlobString &_table, const TxBlobString &_id, const TxBlobString &_data)
	{
		return this->m_Sqlitelib.writeDictionary(_table, _id, _data);
	}
	template<class TFUNC>
	int selectAllDictionary(const TxBlobString &_tableName, const TFUNC &_mFunc)
	{
		return this->m_Sqlitelib.selectDictionary(_tableName, TxBlobString(), _mFunc);
	}
	bool readDictionary(TxBlobString *_value, const TxBlobString &_table, const TxBlobString &_key)
	{
		return this->m_Sqlitelib.readDictionary(_value, _table, _key);
	}
	TxBlobString readDictionaryRet(const TxBlobString &_table, const TxBlobString &_key)
	{
		TxBlobString ret;
		if (!this->readDictionary(&ret, _table, _key))
			ret.clear();
		return ret;
	}
	bool deleteDictionary(const TxBlobString &_table, const TxBlobString &_key)
	{
		return this->m_Sqlitelib.deleteDictionary(_table, _key);
	}
	bool execSQL(const TxBlobString &_sql,
		const std::function<bool(int, char**, char**)> &_mfunc = std::function<bool(int, char**, char**)>())
	{
		return this->m_Sqlitelib.exec_sql(_sql, 1, _mfunc);
	}
	bool transaction(const std::function<bool(void)> &_mfunc)
	{
		return this->m_Sqlitelib.transaction(_mfunc);
	}
	bool querySql(const TxBlobString &_sql, const std::function<bool(TxSqlitelib::TxResultSet*)> &_mfunc)
	{
		if (_mfunc)
			return this->m_Sqlitelib.querySql(_sql, _mfunc);
		return this->m_Sqlitelib.querySql(_sql, [](TxSqlitelib::TxResultSet*)->bool {return false; });
	}
	bool beginStmt(const TxBlobString &_sql)
	{
		return this->m_Sqlitelib.beginStmt(_sql);
	}
	bool endStmt()
	{
		return this->m_Sqlitelib.endStmt();
	}
	bool bindParamBlob(const TxBlobString &_value)
	{
		return this->m_Sqlitelib.bindParamBlob(_value);
	}
	bool bindParamText(const TxBlobString &_value)
	{
		return this->m_Sqlitelib.bindParamText(_value);
	}
	bool bindParamDouble(double _value)
	{
		return this->m_Sqlitelib.bindParamDouble(_value);
	}
	bool bindParamInt(int _value)
	{
		return this->m_Sqlitelib.bindParamInt(_value);
	}
	bool bindParamInt64(std::int64_t _value)
	{
		return this->m_Sqlitelib.bindParamInt64(_value);
	}
};

