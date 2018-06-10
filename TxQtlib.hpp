#pragma once

#include "TxStdlib.hpp"
#include "TxSyslib.hpp"
#include "TxBoostlib.hpp"

#include <functional>
#include <set>
#include <map>
#include <assert.h>

#include <QtCore/QDir>
#include <QtCore/QUuid>
#include <QtCore/QRect>
#include <QtCore/QEvent>
#include <QtCore/QtMath>
#include <QtCore/QMutex>
#include <QtCore/QThread>
#include <QtCore/QObject>
#include <QtCore/QFileInfo>
#include <QtCore/QLibraryInfo>
#include <QtCore/QTranslator>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonParseError>
#include <QtCore/QTimer>
#include <QtCore/QIODevice>
#include <QtCore/QFile>
#include <QtCore/QByteArray>
#include <QtCore/QStandardPaths>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtCore/QBuffer>
#include <QtGui/QImage>
#include <QtGui/QMovie>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QFrame>
#include <QtWidgets/QSlider>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTableWidget>


namespace _TxPrivateNs_ {
	class CQtlibBase
	{
	public:
		inline static void assertMainThread()
		{
#ifndef NDEBUG
			struct TTT {
				const Qt::HANDLE dw_main_thread_id;
				TTT() :dw_main_thread_id(QThread::currentThreadId()) { }
			};
			static TTT ttt;
			bool lc_sign = true;
			if (lc_sign&&ttt.dw_main_thread_id != QThread::currentThreadId())
			{
				lc_sign = false;
				QMessageBox::critical(NULL, QString::fromStdWString(L"error"),
					QString::fromStdWString(L"TxQtlib::assertMainThread()"), QMessageBox::Ok);
				lc_sign = true;
			}
#endif
		}
		static void initEnv()
		{
			assertMainThread();
		}
	};

	class _MyQtWindowGlobal
	{
	private:
		long long _tmp1_[1];
		long lMsgBoxRefCount;
		long long _tmp2_[1];
		std::map<TxBlobWString, TxBlobWString> map_wap_file;
		long long _tmp3_[1];
		QMutex mMutex;
	public:
		_MyQtWindowGlobal()
		{
			this->lMsgBoxRefCount = 0;
		}
		~_MyQtWindowGlobal()
		{
			if (this->lMsgBoxRefCount != 0)
			{
				sysLogError("if (this->lMsgBoxRefCount != 0)参数错误。this->iMsgBoxCount=【{0}】").args(this->lMsgBoxRefCount);
			}
		}
		static _MyQtWindowGlobal* getInstance()
		{
			static _MyQtWindowGlobal ret;
			return &ret;
		}
		int messageBoxRefCount(int _refMsgBoxCount)
		{
			this->mMutex.lock();
			this->lMsgBoxRefCount += (long)_refMsgBoxCount;
			int ret = (int)this->lMsgBoxRefCount;
			this->mMutex.unlock();
			return ret;
		}
		void mapWavFile(TxBlobWString *_local_wav_file, const TxBlobWString &_temp_dir_, const TxBlobWString &_wav_qt_res_file)
		{
			this->mMutex.lock();
			auto find_iter = this->map_wap_file.find(_wav_qt_res_file);
			if (find_iter != this->map_wap_file.end())
				*_local_wav_file = find_iter->second;
			if (_local_wav_file->length() <= 0)
			{
				*_local_wav_file = _temp_dir_.trimRight(L"\r\n\t \\") + L"\\"
					+ TxSyslib::generateGuid().utf8Codecvt() + L".wav";
				this->map_wap_file[_wav_qt_res_file] = *_local_wav_file;
			}
			if (!TxBoostlib::isExistFile(*_local_wav_file))
			{
				QFile lcfile(QString::fromWCharArray(_wav_qt_res_file.c_str()));
				if (!lcfile.open(QIODevice::ReadOnly))
				{
					sysLogError("if (!lcfile.open(QIODevice::ReadOnly))打开文件失败。_wav_qt_res_file=【{0}】").args(_wav_qt_res_file);
				}
				else
				{
					QByteArray lcFileData = lcfile.readAll();
					lcfile.close();
					TxBoostlib::writeFile(*_local_wav_file, TxBlobString(lcFileData.data(), lcFileData.size()));
				}
			}
			this->mMutex.unlock();
		}
	};

	class CQtlib : public CQtlibBase
	{
	private:
		template<class Wnd>
		static void _start_loop_dlg_(int _msec, int *_loopSign, const std::function<bool(Wnd*)> &_loopFn, const std::weak_ptr<Wnd> &_wnd)
		{
			std::shared_ptr<Wnd> lc_spWnd = _wnd.lock();
			if (lc_spWnd&&_loopFn&&_msec >= 0)
			{
				if ((*_loopSign) == 0 || !_loopFn(lc_spWnd.get()))
				{
					(*_loopSign) = 0;
					lc_spWnd->close();
				}
				QTimer::singleShot(_msec, [_loopFn, _wnd, _msec, _loopSign]() {
					_start_loop_dlg_(_msec, _loopSign, _loopFn, _wnd);
				});
			}
		}
		static QMessageBox::StandardButton _messagebox__(QWidget *_parent,
			const std::function<void(QMessageBox*)> &_initFn, const std::function<bool(QMessageBox*)> &_loopFn)
		{
			_MyQtWindowGlobal::getInstance()->messageBoxRefCount(1);
			std::shared_ptr<QMessageBox> lc_dlg = std::make_shared<QMessageBox>(_parent);
			lc_dlg->setTextFormat(Qt::PlainText);
			if (_initFn)
				_initFn(lc_dlg.get());
			int lc_loopSign = 1;
			_start_loop_dlg_<QMessageBox>(300, &lc_loopSign, _loopFn, lc_dlg);
			QMessageBox::StandardButton ret = (QMessageBox::StandardButton)lc_dlg->exec();
			if (_MyQtWindowGlobal::getInstance()->messageBoxRefCount(-1) < 0)
			{
				sysLogError("参数异常。_MyQtWindowGlobal::getInstance()->messageBoxRefCount(0)=【{0}】").args(
					_MyQtWindowGlobal::getInstance()->messageBoxRefCount(0));
			}
			return ret;
		}
		template<class WND>
		static void _centerWidget__(WND *_w, double _dScreenSizeRatio, const QSize &_wnd_size, QWidget *_screen_widget)
		{
			QDesktopWidget *lc_desktop = QApplication::desktop();
			QRect lcScreenRect(0, 0, 0, 0);
			lcScreenRect = lc_desktop->availableGeometry(_screen_widget);
			if (lcScreenRect.width() <= 0 || lcScreenRect.height() <= 0)
				lcScreenRect = lc_desktop->availableGeometry();
			if (lcScreenRect.width() <= 0 || lcScreenRect.height() <= 0)
				lcScreenRect = QRect(QPoint(0, 0), _wnd_size);
			if (lcScreenRect.width() <= 0 || lcScreenRect.height() <= 0)
				lcScreenRect = QRect(QPoint(0, 0), QSize(1024, 720));

			QSize lc_wnd_size = _wnd_size;
			if (_dScreenSizeRatio >= 0)
			{
				lc_wnd_size = QSize((int)(lcScreenRect.width()*_dScreenSizeRatio + 0.5f),
					(int)(lcScreenRect.height()*_dScreenSizeRatio + 0.5f));
			}

			QRect lcRealGeom(QPoint(lcScreenRect.left() + (lcScreenRect.width() - lc_wnd_size.width()) / 2,
				lcScreenRect.top() + (lcScreenRect.height() - lc_wnd_size.height()) / 2), lc_wnd_size);
			if (_w->geometry() != lcRealGeom)
				_w->setGeometry(lcRealGeom);
		}
		template<class WND>
		static inline QWidget* _transQWidgetPtr_(WND *_w)
		{
			return _w;
		}
	public:
		static QMessageBox::StandardButton messagebox(QWidget *_parent, const QMessageBox::StandardButtons &_stdBtns,
			const QString &_text, const QString &_title = QString(), QMessageBox::Icon _msg_icon = QMessageBox::Information,
			const QMessageBox::StandardButton &_defaultButton = QMessageBox::NoButton,
			const std::function<bool(QMessageBox*)> &_loopFn = std::function<bool(QMessageBox*)>())
		{
			return _messagebox__(_parent, [_stdBtns, _text, _title, _msg_icon, _defaultButton](QMessageBox *_msgBox) {
				_msgBox->setIcon(_msg_icon);
				_msgBox->setWindowFlags(_msgBox->windowFlags() | Qt::WindowStaysOnTopHint);
				_msgBox->setStandardButtons(_stdBtns);
				_msgBox->setText(_text);
				if (_title.length() > 0)
					_msgBox->setWindowTitle(_title);
				if (_defaultButton != QMessageBox::NoButton)
					_msgBox->setDefaultButton(_defaultButton);
			}, _loopFn);
		}
		static QMessageBox::StandardButton messagebox(QWidget *_parent, const std::function<void(QMessageBox*)> &_initFn,
			const std::function<bool(QMessageBox*)> &_loopFn = std::function<bool(QMessageBox*)>())
		{
			return _messagebox__(_parent, _initFn, _loopFn);
		}
		static int getMessageBoxCount()
		{
			return _MyQtWindowGlobal::getInstance()->messageBoxRefCount(0);
		}
		template<class WND>
		static void centerWidget(WND *_w, const QSize &_wnd_size, QWidget *_screen_widget = NULL)
		{
			TxQtEnvironment::assertMainThread();
			return _centerWidget__(_w, -1.0, _wnd_size, (_screen_widget == NULL ? _transQWidgetPtr_(_w) : _screen_widget));
		}
		template<class WND>
		static void centerWidget(WND *_w, QWidget *_screen_widget = NULL)
		{
			TxQtEnvironment::assertMainThread();
			return _centerWidget__(_w, -1.0, _w->size(), (_screen_widget == NULL ? _transQWidgetPtr_(_w) : _screen_widget));
		}
		template<class WND>
		static void centerWidget(WND *_w, double _dScreenSizeRatio, QWidget *_screen_widget = NULL)
		{
			TxQtEnvironment::assertMainThread();
			return _centerWidget__(_w, _dScreenSizeRatio, _w->size(), (_screen_widget == NULL ? _transQWidgetPtr_(_w) : _screen_widget));
		}

		//ui界面操作
		template<class WND>
		static void setVisible(WND *_w, bool _visible)
		{
			if (_w->isVisible() != _visible)
				_w->setVisible(_visible);
		}
		static void setEnabled(QWidget *_w, bool _enabled)
		{
			if (_w->isEnabled() != _enabled)
				_w->setEnabled(_enabled);
		}
		template<class WND>
		static void setGeometry(WND *_w, const QRect &_rect)
		{
			if (_w->geometry() != _rect)
				_w->setGeometry(_rect);
		}
		template<class WND>
		static bool setText(WND *_w, const QString &_text)
		{
			if (_w->text().compare(_text) != 0)
			{
				_w->setText(_text);
				return true;
			}
			return false;
		}
		template<class WND>
		static bool setPlainText(WND *_w, const QString &_text)
		{
			if (_w->toPlainText().compare(_text) != 0)
			{
				_w->setPlainText(_text);
				return true;
			}
			return false;
		}
		template<class WND, class VALUE>
		static void setValue(WND *_w, const VALUE &_val)
		{
			if (_w->value() != _val)
				_w->setValue(_val);
		}
		template<class WND>
		static void setCurrentWidget(WND *_w, QWidget *_curWidget)
		{
			if (_w->currentWidget() != _curWidget)
				_w->setCurrentWidget(_curWidget);
		}
		template<class WND>
		static void setChecked(WND *_w, bool _checked)
		{
			if ((!!_w->isChecked()) ^ (!!_checked))
				_w->setChecked(_checked);
		}
		static void setItemVisible(QListWidgetItem *_w, bool _visible)
		{
			if (_w->isHidden() == _visible)
				_w->setHidden(!_visible);
		}
		template<class WND>
		static bool setElidedText(WND *_w, const QString &_text)
		{
			QMargins lcContentsMargins = _w->contentsMargins();
			QString lcElidedText = _w->fontMetrics().elidedText(_text, Qt::ElideRight,
				_w->width() - lcContentsMargins.left() - lcContentsMargins.right(),
				Qt::TextShowMnemonic | Qt::TextWrapAnywhere);
			if (_w->text().compare(lcElidedText) != 0)
			{
				_w->setText(lcElidedText);
				return true;
			}
			return false;
		}
		template<class WND>
		static WND* createCenterChildWidget(QWidget *_parent, const QMargins &_margins = QMargins(0, 0, 0, 0))
		{
			QGridLayout *lc_lyt = new QGridLayout(_parent);
			lc_lyt->setContentsMargins(_margins);
			lc_lyt->setSpacing(0);
			WND *ret = new WND(_parent);
			lc_lyt->addWidget(ret);
			return ret;
		}
		static void loadAppStyleSheet(const QString &_filename)
		{
			QFile lc_file(_filename);
			if (lc_file.open(QIODevice::ReadOnly))
			{
				QString lc_qss_data = QString::fromUtf8(lc_file.readAll());
				if (lc_qss_data != qApp->styleSheet())
					qApp->setStyleSheet(lc_qss_data);
			}
			else assert(false);
		}
		static void playSoundFromQtResource(const TxBlobWString &_temp_dir_, const TxBlobWString &_wav_qt_res_file)
		{
			if (_temp_dir_.length() <= 0 || _wav_qt_res_file.length() <= 0)
			{
				sysLogError("if (_temp_dir_.length() <= 0 || _wav_qt_res_file.length() <= 0)参数错误。_temp_dir_=【{0}】。_wav_qt_res_file=【{1}】"
				).args(_temp_dir_, _wav_qt_res_file);
			}
			else
			{
				TxBlobWString lc_local_file;
				_MyQtWindowGlobal::getInstance()->mapWavFile(&lc_local_file, _temp_dir_, _wav_qt_res_file);
				if (lc_local_file.length() > 0)
				{
					if (!::PlaySoundW(lc_local_file.c_str(), NULL, SND_NOWAIT))
					{
						sysLogErrorW(L"if (!::PlaySoundW(lc_local_file.c_str(), NULL, SND_NOWAIT))返回错误。"
							L"lc_local_file=【{0}】,_wav_qt_res_file=【{1}】,_temp_dir_=【{2}】").args(lc_local_file, _wav_qt_res_file, _temp_dir_);
					}
				}
			}
		}
		static void playLableGif(QWidget *_widget, const QString &_gif_path)
		{
			class MyMovie :public QMovie
			{
			private:
				int iTimerId;
				QLabel *label;
				bool b_play_status;
			public:
				MyMovie(QLabel *_lable) :QMovie(_lable), label(_lable)
				{
					this->b_play_status = false;
					this->iTimerId = __super::startTimer(1000);
				}
				~MyMovie()
				{
					this->killTimer(this->iTimerId);
				}
				void my_start()
				{
					if (this->b_play_status)
						return;
					this->b_play_status = true;
					__super::start();
				}
				void my_stop()
				{
					if (!this->b_play_status)
						return;
					this->b_play_status = false;
					__super::stop();
				}
				virtual void timerEvent(QTimerEvent *_event)
				{
					if (this->iTimerId == _event->timerId())
					{
						if (label->isVisible())
							this->my_start();
						else this->my_stop();
					}
					return __super::timerEvent(_event);
				}
			};
			QLabel *lc_label = dynamic_cast<QLabel*>(_widget);
			if (lc_label == NULL)
			{
				lc_label = new QLabel(_widget);
				QGridLayout *lc_lyt = new QGridLayout(_widget);
				lc_lyt->setMargin(0);
				lc_lyt->addWidget(lc_label);
			}
			if (lc_label->movie() != NULL)
			{
				lc_label->movie()->stop();
				delete lc_label->movie();
			}
			MyMovie *lc_movie = new MyMovie(lc_label);
			lc_label->setMovie(lc_movie);

			lc_movie->my_stop();
			lc_label->setScaledContents(true);
			lc_movie->setFileName(_gif_path);
			lc_movie->my_start();
		}
		static bool generateDesktopShortcut(const QString &_app_path, const QString &_desktop_name)
		{
			bool ret = QFile::link(_app_path,
				QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/").append(
					_desktop_name).append(QString::fromStdWString(L".lnk")));
			assert(ret);
			return ret;
		}
		static void gClearGridLayout(QLayout *_layout)
		{
			if (_layout == NULL)
				return;
			for (int iii = _layout->count(); iii--;)
			{
				QLayoutItem *lc_lyt_item = _layout->itemAt(iii);
				if (lc_lyt_item == NULL)
					assert(false);
				else
				{
					gClearGridLayout(dynamic_cast<QLayout*>(lc_lyt_item));
					QWidget *lc_www = lc_lyt_item->widget();
					if (lc_www != NULL)
					{
						_layout->removeWidget(lc_www);
						delete lc_www;
					}
					_layout->removeItem(lc_lyt_item);
				}
			}
		}
		static void gClearStackedWidget(QStackedWidget *_stackedWidget)
		{
			for (int ii = _stackedWidget->count(); ii--;)
			{
				QWidget *lc_widget_item = _stackedWidget->widget(ii);
				if (lc_widget_item == NULL)
					assert(false);
				else
				{
					_stackedWidget->removeWidget(lc_widget_item);
					delete lc_widget_item;
				}
			}
		}
		static void gClearTabWidget(QTabWidget *_tabWidget)
		{
			for (int ii = _tabWidget->count(); ii--;)
			{
				QWidget *lc_widget = _tabWidget->widget(ii);
				_tabWidget->removeTab(ii);
				if (lc_widget == NULL)assert(false);
				else delete lc_widget;
			}
			assert(_tabWidget->count() == 0);
		}
		static void gClearTableWidget(QTableWidget *_tableWidget)
		{
			for (int yy = (int)_tableWidget->rowCount(); yy--;)
			{
				for (int xx = (int)_tableWidget->colorCount(); xx--;)
				{
					QWidget *lc_widget = _tableWidget->cellWidget(yy, xx);
					if (lc_widget != NULL)
					{
						_tableWidget->removeCellWidget(yy, xx);
						delete lc_widget;
					}
					QTableWidgetItem *lc_item = _tableWidget->item(yy, xx);
					if (lc_item != NULL)
						delete lc_item;
				}
				_tableWidget->removeRow(yy);
			}
		}
		static void initEnv()
		{
			TxSyslib::initEnv();
			TxBoostlib::initEnv();
			CQtlibBase::initEnv();
			_MyQtWindowGlobal::getInstance();
		}
	};

	class _MyTranslator_ : public QTranslator
	{
	private:
		std::map<TxBlobString, QString> mapLanguageData;
		QString emptyQString;
		TxBlobString ts_language_key;
		std::list<std::shared_ptr<QTranslator>> listSysTranslator;
	protected:
		virtual QString translate(const char *_context, const char *_sourceText, const char *_disambiguation, int _n) const
		{
			//此处会有多线程进入
			QString ret;
			TxBlobString lc_lang_local_key = this->_match_qt_local_lang_(_sourceText);
			if (lc_lang_local_key.length() > 0)
				ret = this->_find_from_language_key_(lc_lang_local_key);
			if (ret.length() <= 0)
			{
				if (_sourceText != NULL && std::strlen(_sourceText) >= 3 && _sourceText[0] == '[')
				{
					const char *lc_sep_ptr_pos = std::strchr(_sourceText, ']');
					if (lc_sep_ptr_pos != NULL)
						ret = this->_find_from_language_key_(TxBlobString(_sourceText + 1, (int)(lc_sep_ptr_pos - (_sourceText + 1))));
				}
			}
			if (ret.length() <= 0)
				ret = __super::translate(_context, _sourceText, _disambiguation, _n);
			return ret;
		}
	private:
		const QString& _find_from_language_key_(const TxBlobString &_key) const
		{
			auto find_iter = this->mapLanguageData.find(_key);
			if (find_iter == this->mapLanguageData.end())
			{
				sysLogError("if (find_iter == this->mapLanguageData.end())找不到语言key。_key=[{0}]").args(_key);
				return this->emptyQString;
			}
			return find_iter->second;
		}
		static TxBlobString _match_qt_local_lang_(const TxBlobString &_sourceText_key)
		{
			struct T_QT_KEY_LAN {
				std::map<TxBlobString, TxBlobString> data;
				T_QT_KEY_LAN() {
					const char* lc_qt_key_lang[][8] = {
						{ "LGG_qt_redo", "redo" },
						{ "LGG_qt_undo", "undo" },
						{ "LGG_qt_cut", "cut", "cu&t" },
						{ "LGG_qt_copy", "copy" },
						{ "LGG_qt_paste", "paste" },
						{ "LGG_qt_delete", "delete" },
						{ "LGG_qt_select_all", "select all" },
						{ "LGG_qt_unselect", "unselect" },
						{ "LGG_qt_stop", "stop" },
						{ "LGG_qt_reload", "reload" },
						{ "LGG_qt_forward", "forward" },
						{ "LGG_qt_back", "back" },
						{ "LGG_qt_save", "save" },
						{ "LGG_qt_yes", "yes" },
						{ "LGG_qt_no", "no" },
						{ "LGG_qt_ok", "ok" },
						{ "LGG_qt_cancel", "cancel" },
					};
					for (int y = 0; y < sizeof(lc_qt_key_lang) / sizeof(lc_qt_key_lang[0]); ++y)
					{
						for (int x = 1; x < sizeof(lc_qt_key_lang[0]) / sizeof(lc_qt_key_lang[0][0]); ++x)
						{
							if (lc_qt_key_lang[y][x] == NULL)
								break;
							this->data[TxBlobString(lc_qt_key_lang[y][x])] = TxBlobString(lc_qt_key_lang[y][0]);
						}
					}
				}
			};
			static T_QT_KEY_LAN lc_mQtKeyLan;
			auto lc_qt_key_lan_find_iter
				= lc_mQtKeyLan.data.find(TxBlobString(_sourceText_key).toLowerCase().trimLR("\r\n\t &"));
			if (lc_qt_key_lan_find_iter != lc_mQtKeyLan.data.end())
				return lc_qt_key_lan_find_iter->second;
			return TxBlobString();
		}
		void _loadJsonToMapData_(const QString &_languageFile)
		{
			_TxPrivateNs_::CQtlib::assertMainThread();
			QFile lc_file(_languageFile);
			if (!lc_file.open(QIODevice::ReadOnly))
			{
				sysLogError("if(!lc_file.open(QIODevice::ReadOnly | QIODevice::Text))打开语言文件失败。errorString=[{0}], _languageFile=[{1}]").args(
					lc_file.errorString().toStdWString(), _languageFile.toStdWString());
			}
			else
			{
				QJsonParseError lcJsonParseError;
				QByteArray lcJsonBytes = lc_file.readAll();
				lc_file.close();
				QJsonDocument lcJsonDocument = QJsonDocument::fromJson(lcJsonBytes, &lcJsonParseError);
				if (lcJsonParseError.error != QJsonParseError::NoError)
				{
					sysLogError("if (lcJsonParseError.error() != QJsonParseError::NoError)解析语言json文件失败。lcJsonParseError=[{0}]。json=【{1}】").args(
						lcJsonParseError.errorString().toStdWString(), lcJsonBytes.toStdString());
				}
				else
				{
					QJsonObject lcRootJsonObject = lcJsonDocument.object();
					QStringList lcListLangKeys = lcRootJsonObject.keys();
					for (auto iter = lcListLangKeys.begin(); iter != lcListLangKeys.end(); ++iter)
					{
						QJsonValue lcLangValue = lcRootJsonObject[*iter];
						if (!lcLangValue.isObject())
							sysLogError("if (!lcLangValue.isObject())参数错误。lcLangValue.type()=[{0}]").args(lcLangValue.type());
						else
						{
							QJsonValue lcLangContent = lcLangValue.toObject()[QString::fromStdWString(this->ts_language_key.utf8Codecvt())];
							if (!lcLangContent.isString())
								sysLogError("if (!lcLangContent.isString())参数错误。lcLangContent.type()=[{0}]").args(lcLangContent.type());
							else
							{
								this->mapLanguageData[TxBlobWString(iter->toStdWString()).utf8Codecvt()]
									= lcLangContent.toString();
							}
						}
					}
				}
			}
		}
		void _loadSysTranslator()
		{
			_TxPrivateNs_::CQtlib::assertMainThread();
			QString lc_file_suffix;
			if (this->ts_language_key.equalNoCase("chinese"))
				lc_file_suffix = QString::fromStdWString(L"_zh_CN.qm");
			else if (this->ts_language_key.equalNoCase("english"))
				lc_file_suffix = QString::fromStdWString(L"_en.qm");
			else
			{
				assert(false);
				lc_file_suffix = QString::fromStdWString(L"_en.qm");
			}

			for each(QFileInfo mfi in QDir(QLibraryInfo::location(QLibraryInfo::TranslationsPath)).entryInfoList())
			{
				QString lc_path = mfi.absoluteFilePath();
				if (mfi.isFile() && lc_path.length() > lc_file_suffix.length()
					&& lc_path.right(lc_file_suffix.length()).toLower() == lc_file_suffix.toLower())
				{
					std::shared_ptr<QTranslator> lc_spTranslator = std::make_shared<QTranslator>();
					if (lc_spTranslator->load(lc_path))
					{
						if (mfi.fileName().toLower() == (QString::fromStdWString(L"qt") + lc_file_suffix).toLower())
							this->listSysTranslator.push_back(lc_spTranslator);
						else
							this->listSysTranslator.push_front(lc_spTranslator);
					}
				}
			}

			qApp->installTranslator(this);
			for (auto iter = this->listSysTranslator.begin(); iter != this->listSysTranslator.end(); ++iter)
				qApp->installTranslator(iter->get());
		}
	private:
		_MyTranslator_(const _MyTranslator_&)
		{
			TxSysDebugObjectMemory::malloc(this);
			sysLogError("_MyTranslator_(const _MyTranslator_&)不应该有此操作");
		}
	public:
		_MyTranslator_()
		{
			_TxPrivateNs_::CQtlib::assertMainThread();
			this->_match_qt_local_lang_("");
			TxSysDebugObjectMemory::malloc(this);
		}
		~_MyTranslator_()
		{
			TxSysDebugObjectMemory::free(this);
		}
		void init(const TxBlobString &_languageKey, const QString &_languageFile)
		{
			_TxPrivateNs_::CQtlib::assertMainThread();
			if (this->ts_language_key.length() > 0
				|| _languageKey.length() <= 0
				|| _languageFile.length() <= 0)
			{
				assert(false);
			}
			this->ts_language_key = _languageKey;
			this->_loadJsonToMapData_(_languageFile);
			this->_loadSysTranslator();
		}
		const QString& text(const TxBlobString &_key)
		{
			return this->_find_from_language_key_(_key);
		}
		const std::map<TxBlobString, QString>& getLanguageData() const
		{
			return this->mapLanguageData;
		}
	};

	class _MyAutoPrompt_ : public QFrame
	{
	private:
		enum { timer_unit_msec = 80, };
		QTextEdit *pCenterUi;
		int iTimerId;
		int iDisplayTimeMsec;
		QString emptyTitle;
		std::function<void()> fnDestructor;
	private:
		virtual void timerEvent(QTimerEvent *_event)
		{
			if (_event->timerId() == this->iTimerId)
			{
				if (this->iDisplayTimeMsec > 0)
				{
					this->iDisplayTimeMsec -= timer_unit_msec;
				}
				else if (this->isVisible())
				{
					const int lc_delta_height = 2;
					QRect lc_rect = this->geometry();
					this->setGeometry(QRect(lc_rect.topLeft() + QPoint(0, lc_delta_height), lc_rect.size() - QSize(0, lc_delta_height)));
					if (this->geometry().height() == lc_rect.height())
						this->setVisible(false);
				}
			}
			return __super::timerEvent(_event);
		}
		virtual bool event(QEvent *_event)
		{
			switch (_event->type())
			{
			case QEvent::Close:
				this->setVisible(false);
				_event->ignore();
				return true;
				break;
			default:
				break;
			}
			return __super::event(_event);
		}
	private:
		_MyAutoPrompt_(const _MyAutoPrompt_&)
		{
			sysLogError("_MyAutoPrompt_(const _MyAutoPrompt_&)不应该有此操作。");
			TxSysDebugObjectMemory::malloc(this);
		}
	public:
		_MyAutoPrompt_(QWidget *_parent, const std::function<void()> &_fnDestructor) :QFrame(_parent), fnDestructor(_fnDestructor)
		{
			_TxPrivateNs_::CQtlib::assertMainThread();
			TxSysDebugObjectMemory::malloc(this);
			this->setWindowFlags((this->windowFlags() | Qt::Drawer | Qt::WindowStaysOnTopHint)
				&~(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowFullscreenButtonHint));

			this->emptyTitle = QString::fromWCharArray(L"					");
			QVBoxLayout* lc_global_lyt = new QVBoxLayout(this);
			//添加水平伸缩杆
			lc_global_lyt->addItem(new QSpacerItem(40, 8, QSizePolicy::Expanding, QSizePolicy::Minimum));
			//添加文本按钮
			this->pCenterUi = new QTextEdit(this);
			lc_global_lyt->addWidget(this->pCenterUi);
			//添加水平伸缩杆
			lc_global_lyt->addItem(new QSpacerItem(40, 8, QSizePolicy::Expanding, QSizePolicy::Minimum));
			this->setStyleSheet(QString::fromWCharArray(L""
				L"*{border:none; background-color: white; color:black; font: bold 11pt \"Arial\";}"));
			this->pCenterUi->setReadOnly(true);
			this->pCenterUi->setAlignment(Qt::AlignCenter);
			this->pCenterUi->verticalScrollBar()->setVisible(false);
			this->pCenterUi->horizontalScrollBar()->setVisible(false);
			this->iTimerId = this->startTimer(timer_unit_msec);
			this->setMinimumHeight(2);
			this->iDisplayTimeMsec = 0;
			this->setVisible(false);
			this->setContextMenuPolicy(Qt::NoContextMenu);
			this->pCenterUi->setContextMenuPolicy(Qt::NoContextMenu);
		}
		~_MyAutoPrompt_()
		{
			TxSysDebugObjectMemory::free(this);
			_TxPrivateNs_::CQtlib::assertMainThread();
			this->killTimer(this->iTimerId);
			if (this->fnDestructor)
				this->fnDestructor();
		}
		void setPromptInformation(int _iDisplayTimeMsec, const QString &_text, const QString &_title)
		{
			_TxPrivateNs_::CQtlib::assertMainThread();
			QString lc_title = _title;
			if (lc_title.length() <= 0)
				lc_title = this->emptyTitle;
			if (lc_title != this->windowTitle())
				this->setWindowTitle(lc_title);
			if (this->pCenterUi->toPlainText() != _text)
				this->pCenterUi->setPlainText(_text);
			this->iDisplayTimeMsec = _iDisplayTimeMsec;
			assert(_iDisplayTimeMsec > 0);
		}
		void showPrompt(bool _bfull)
		{
			_TxPrivateNs_::CQtlib::assertMainThread();
			QSize lc_size;
			if (_bfull) lc_size = QApplication::desktop()->geometry().size();
			else lc_size = QApplication::desktop()->availableGeometry().size();
			this->setGeometry(QRect(QPoint(lc_size.width() - 300, lc_size.height() - 200), QPoint(lc_size.width(), lc_size.height())));
			this->setVisible(true);
		}
		void showPrompt(const QRect &_rect)
		{
			_TxPrivateNs_::CQtlib::assertMainThread();
			this->setGeometry(_rect);
			this->setVisible(true);
		}
	};
}

//--------------------------------------TxQtEnvironment--------------------------------------//
class TxQtEnvironment : public QObject, public TxSingletonTmpl<TxQtEnvironment>
{
private:
	QEvent::Type iEventPostTaskId;
	std::list<std::shared_ptr<std::function<void(void)>>> mListTask;
	std::shared_ptr<_TxPrivateNs_::_MyTranslator_> spTranslator;
	struct {
		std::map<TxBlobString, QString> emptyMapKeyLang;
		QString emptyQString;
	} emptyObject;
	_TxPrivateNs_::_MyAutoPrompt_ *ptrAutoPrompt;
	QMutex mMutex;
private:
	virtual bool event(QEvent *_event)
	{
		if (_event->type() == this->iEventPostTaskId)
			this->_safe_dealTask();
		return __super::event(_event);
	}
	void _safe_dealTask()
	{
		std::list<std::shared_ptr<std::function<void(void)>>> lc_ListTask;
		this->mMutex.lock();
		lc_ListTask.swap(this->mListTask);
		this->mListTask.clear();
		this->mMutex.unlock();
		for (auto iter = lc_ListTask.begin(), iter_end = lc_ListTask.end(); iter != iter_end; ++iter)
			(**iter)();
	}
	template<class TFUNC>
	void _safe_postTask(const TFUNC &_mfunc)
	{
		auto lc_fn = std::make_shared<std::function<void(void)>>();
		*lc_fn = _mfunc;
		this->mMutex.lock();
		this->mListTask.push_back(lc_fn);
		this->mMutex.unlock();
		QApplication::postEvent(this, new QEvent(this->iEventPostTaskId));
	}
	void _installTranslator_(const TxBlobString &_languageKey, const QString &_languageFile)
	{
		this->mMutex.lock();
		if (this->spTranslator)
			assert(false);
		else
		{
			this->spTranslator = std::make_shared<_TxPrivateNs_::_MyTranslator_>();
			this->spTranslator->init(_languageKey, _languageFile);
		}
		this->mMutex.unlock();
	}
	const std::map<TxBlobString, QString>& _getLanguageData_() const
	{
		if (!this->spTranslator)
		{
			sysLogError("if (!this->spTranslator)参数错误");
			return emptyObject.emptyMapKeyLang;
		}
		return this->spTranslator->getLanguageData();
	}
	const QString& _translate_(const TxBlobString &_key)
	{
		if (!this->spTranslator)
		{
			sysLogError("if (!this->spTranslator)参数错误，_key=【{0}】").args(_key);
			return emptyObject.emptyQString;
		}
		return this->spTranslator->text(_key);
	}
	void _showPrompt_(QWidget *_parent, int _iDisplayTimeMsec, const QString &_text, const QString &_title, bool _showFull)
	{
		assertMainThread();
		if (this->ptrAutoPrompt == NULL)
		{
			this->ptrAutoPrompt = new _TxPrivateNs_::_MyAutoPrompt_(_parent, []() {
				assertMainThread();
				TxQtEnvironment *pThis = object();
				if (pThis != NULL)
					pThis->ptrAutoPrompt = NULL;
			});
		}
		if (this->ptrAutoPrompt->parent() != _parent)
			this->ptrAutoPrompt->setParent(_parent);
		this->ptrAutoPrompt->setPromptInformation(_iDisplayTimeMsec, _text, _title);
		this->ptrAutoPrompt->showPrompt(_showFull);
	}
protected:
	TxQtEnvironment()
	{
		_TxPrivateNs_::CQtlib::initEnv();
		_TxPrivateNs_::_MyQtWindowGlobal::getInstance();
		assertMainThread();
		enum { lc_eEventPostTaskId = QEvent::User + 168, };
		this->iEventPostTaskId = (QEvent::Type)QEvent::registerEventType(lc_eEventPostTaskId);
		assert(this->iEventPostTaskId == lc_eEventPostTaskId);
	}
	~TxQtEnvironment()
	{
		assertMainThread();
		this->_safe_dealTask();
		if (this->ptrAutoPrompt != NULL)
		{
			delete this->ptrAutoPrompt;
			this->ptrAutoPrompt = NULL;
		}
	}
public:
	inline static void assertMainThread()
	{
		_TxPrivateNs_::CQtlib::assertMainThread();
	}

	static void installTranslator(const TxBlobString &_languageKey, const QString &_languageFile)
	{
		assertMainThread();
		return object()->_installTranslator_(_languageKey, _languageFile);
	}
	static const std::map<TxBlobString, QString>& getLanguageData()
	{
		return object()->_getLanguageData_();
	}
	static const QString& translate(const TxBlobString &_key)
	{
		return object()->_translate_(_key);
	}
	template<class TFUNC>
	static void postTask(const TFUNC &_mfunc)
	{
		return object()->_safe_postTask(_mfunc);
	}
	static void showPrompt(bool _showFull, int _iDisplayTimeMsec, const QString &_text, QWidget *_parent = NULL)
	{
		assertMainThread();
		object()->_showPrompt_(_parent, _iDisplayTimeMsec, _text, QString(), _showFull);
	}
};


//--------------------------------------TxQtWindow--------------------------------------//
template<class WND>
class TxQtWindow : public WND
{
private:
	enum {
		E_WM_NCHITTEST = 0x0084,
		E_WM_NCMOUSEMOVE = 0x00A0,
		E_WM_NCLBUTTONDOWN = 0x00A1,
		E_WM_NCLBUTTONUP = 0x00A2,
		E_HTCLIENT = 1,
		E_HTCAPTION = 2,
		E_HTLEFT = 10,
		E_HTRIGHT = 11,
		E_HTTOP = 12,
		E_HTTOPLEFT = 13,
		E_HTTOPRIGHT = 14,
		E_HTBOTTOM = 15,
		E_HTBOTTOMLEFT = 16,
		E_HTBOTTOMRIGHT = 17,
		E_HTBORDER = 18,
	};
private:
	TxObjectLifePeriod::TxObjectEntity mLifeEntity;
	QWidget *uiContainerWidget;
	QGridLayout *uiContainerLayout;
	QPoint m_DragTitleBarPoint;
	QPoint ptTouchCaptionMousePos;//鼠标触碰时窗口位置
	QPoint m_invalidPoint;//不可用的点值
private:
	bool is_fixed_window_() const
	{
		return this->windowState()&(Qt::WindowMaximized | Qt::WindowFullScreen);
	}
	int get_window_border_L_() const
	{
		return this->is_fixed_window_() ? 0 : 8;
	}
protected:
	virtual int vfGetCaptionHeight() { return 0; }
	virtual bool vfIsWindowCaption(const QPoint &_pt) { (void)&_pt; return false; }
	virtual void vfTouchCaptionEvent() {}//鼠标轻轻点碰标题
protected:
	virtual void mousePressEvent(QMouseEvent *_mouseEvent)
	{
		QRect lc_center_w_rect = this->uiContainerWidget->geometry();
		if (lc_center_w_rect.contains(_mouseEvent->pos()) && !this->is_fixed_window_())
		{
			this->ptTouchCaptionMousePos = _mouseEvent->globalPos();
			int lcCaptionHeight = this->vfGetCaptionHeight();
			if (0 < lcCaptionHeight&&_mouseEvent->pos().y() <= lcCaptionHeight + lc_center_w_rect.top())
				this->m_DragTitleBarPoint = _mouseEvent->pos();
		}
		return __super::mousePressEvent(_mouseEvent);
	}
	virtual void mouseReleaseEvent(QMouseEvent *_mouseEvent)
	{
		QPoint lc_deltaTouchXY = this->ptTouchCaptionMousePos - _mouseEvent->globalPos();
		if (TxStdlib::absolute(lc_deltaTouchXY.x()) <= 1 && TxStdlib::absolute(lc_deltaTouchXY.y()) <= 1)
		{
			this->ptTouchCaptionMousePos = this->m_invalidPoint;
			this->vfTouchCaptionEvent();
		}
		else this->ptTouchCaptionMousePos = this->m_invalidPoint;
		this->m_DragTitleBarPoint = this->m_invalidPoint;
		return __super::mouseReleaseEvent(_mouseEvent);
	}
	virtual void mouseMoveEvent(QMouseEvent *_mouseEvent)
	{
		if (this->ptTouchCaptionMousePos != this->m_invalidPoint)
		{
			QPoint lc_deltaTouchXY = this->ptTouchCaptionMousePos - QCursor::pos();
			if (TxStdlib::absolute(lc_deltaTouchXY.x()) > 2 || TxStdlib::absolute(lc_deltaTouchXY.y()) > 2)
				this->ptTouchCaptionMousePos = this->m_invalidPoint;
		}
		if (this->m_DragTitleBarPoint != this->m_invalidPoint)
			this->setGeometry(QRect(_mouseEvent->globalPos() - this->m_DragTitleBarPoint, this->geometry().size()));
		return __super::mouseMoveEvent(_mouseEvent);
	}
	virtual void paintEvent(QPaintEvent *_paintEvent)
	{
		QPainter lc_painter(this);
		lc_painter.setRenderHint(QPainter::Antialiasing, true);

		const int lc_border = this->uiContainerLayout->margin();

		for (int i = 0; i <= lc_border; i++)
		{
			QPainterPath lc_pen_path;
			lc_pen_path.setFillRule(Qt::WindingFill);
			lc_pen_path.addRect(i, i, this->width() - i * 2, this->height() - i * 2);
			lc_painter.setPen(QColor(0, 0, 0, (int)(qPow(i* 16.5f / (float)lc_border, 1.4f)) + 0.5f));
			lc_painter.drawPath(lc_pen_path);
		}

		QPainterPath lc_fill_path;
		lc_fill_path.setFillRule(Qt::WindingFill);
		lc_fill_path.addRect(lc_border, lc_border, this->width() - lc_border * 2, this->height() - lc_border * 2);
		lc_painter.fillPath(lc_fill_path, QBrush(Qt::white));

		lc_painter.end();
	}
	//virtual void paintEvent(QPaintEvent *_paintEvent)
	//{
	//	const int lc_border = this->uiContainerLayout->margin();
	//	QSize lc_glbSize = this->geometry().size();
	//	if (this->mPageCache.iBorderRadius != this->iBorderRadius
	//		|| this->mPageCache.mBorderColor != this->mBorderColor
	//		|| this->mPageCache.image.size() != lc_glbSize
	//		|| this->mPageCache.iBorderWidth != lc_border)
	//	{//画图片缓存
	//		this->mPageCache.iBorderRadius = TxStdlib::maximum(0, this->iBorderRadius);
	//		this->mPageCache.mBorderColor = this->mBorderColor;
	//		this->mPageCache.iBorderWidth = lc_border;
	//		this->mPageCache.image = QImage(lc_glbSize, QImage::Format_RGBA8888);
	//		//this->mPageCache.image = QPixmap(lc_glbSize);
	//		//this->mPageCache.pixmap.fill(Qt::transparent);

	//		QPainter lc_pix_painter(&this->mPageCache.image);
	//		lc_pix_painter.setRenderHint(QPainter::Antialiasing, true);
	//		for (int ii = 0; ii <= lc_border; ++ii)
	//		{
	//			//设置颜色以及透明度
	//			if (ii == lc_border&&this->mPageCache.mBorderColor.alpha() != 0)
	//				lc_pix_painter.setPen(this->mPageCache.mBorderColor);
	//			else
	//			{
	//				int lc_alpha = (int)((qPow(ii* 16.5f / (float)lc_border, 1.4f)) + 0.5f);
	//				lc_pix_painter.setPen(QColor(0, 0, 0, lc_alpha));
	//			}
	//			QVector<QLine> lc_line;
	//			//top line
	//			lc_line.push_back(QLine(QPoint(ii + this->mPageCache.iBorderRadius, ii),
	//				QPoint(lc_glbSize.width() - ii - this->mPageCache.iBorderRadius, ii)));
	//			//bottom line
	//			lc_line.push_back(QLine(QPoint(ii + this->mPageCache.iBorderRadius, lc_glbSize.height() - ii),
	//				QPoint(lc_glbSize.width() - ii - this->mPageCache.iBorderRadius, lc_glbSize.height() - ii)));
	//			//left line
	//			lc_line.push_back(QLine(QPoint(ii, ii + this->mPageCache.iBorderRadius),
	//				QPoint(ii, lc_glbSize.height() - ii - this->mPageCache.iBorderRadius)));
	//			//right line
	//			lc_line.push_back(QLine(QPoint(lc_glbSize.width() - ii, ii + this->mPageCache.iBorderRadius),
	//				QPoint(lc_glbSize.width() - ii, lc_glbSize.height() - ii - this->mPageCache.iBorderRadius)));
	//			lc_pix_painter.drawLines(lc_line);
	//			if (this->mPageCache.iBorderRadius > 0)
	//			{
	//				QSize lc_arc_size(this->mPageCache.iBorderRadius * 2, this->mPageCache.iBorderRadius * 2);
	//				lc_pix_painter.drawArc(QRect(
	//					QPoint(lc_glbSize.width() - lc_arc_size.width() - ii, ii),
	//					lc_arc_size), 0 * 16, 90 * 16);//top right arc
	//				lc_pix_painter.drawArc(QRect(QPoint(ii, ii),
	//					lc_arc_size), 90 * 16, 90 * 16);//top left arc
	//				lc_pix_painter.drawArc(QRect(QPoint(ii, lc_glbSize.height() - lc_arc_size.height() - ii),
	//					lc_arc_size), 180 * 16, 90 * 16);//bottom left arc
	//				lc_pix_painter.drawArc(QRect(
	//					QPoint(lc_glbSize.width() - lc_arc_size.width() - ii,
	//						lc_glbSize.height() - lc_arc_size.height() - ii),
	//					lc_arc_size), 270 * 16, 90 * 16);//bottom left arc
	//			}
	//			lc_pix_painter.end();
	//		}
	//	}
	//	if (true)
	//	{//显示缓存
	//		QPainter lc_wnd_painter(this);
	//		lc_wnd_painter.drawImage(0, 0, this->mPageCache.image);
	//		lc_wnd_painter.end();
	//	}
	//}
	virtual bool event(QEvent *_event)
	{
		if (_event->type() == QEvent::WindowStateChange)
		{
			int lc_border = this->get_window_border_L_();
			if (this->uiContainerLayout->margin() != lc_border)
				this->uiContainerLayout->setMargin(lc_border);
		}
		return __super::event(_event);
	}
	virtual bool nativeEvent(const QByteArray &_eventType, void *_message, long *_result)
	{
#pragma pack(push)
#pragma pack(1)
		typedef struct {
			void* hwnd;				//HWND   hwnd;
			unsigned int message;	//UINT   message;
									//WPARAM wParam;
									//LPARAM lParam;
									//DWORD  time;
									//POINT  pt;
		} T_MY_MSG;
#pragma pack(pop)
		TxQtEnvironment::assertMainThread();
		auto lc_fnIsWindowCaption = [this](const QPoint &_mouseGlbPos)->bool {
			QSize lc_center_w_size = this->uiContainerWidget->size();
			QPoint lc_center_w_pt = this->uiContainerWidget->mapFromGlobal(_mouseGlbPos);
			return 0 <= lc_center_w_pt.x() && lc_center_w_pt.x() < lc_center_w_size.width()
				&& 0 <= lc_center_w_pt.y() && lc_center_w_pt.y() < lc_center_w_size.height()
				&& this->vfIsWindowCaption(lc_center_w_pt);
		};
		if ((unsigned int)((T_MY_MSG*)_message)->message == (unsigned int)E_WM_NCLBUTTONDOWN)
		{
			QPoint lcCursorPoint = QCursor::pos();
			if (lc_fnIsWindowCaption(lcCursorPoint))
				this->ptTouchCaptionMousePos = lcCursorPoint;
		}
		else if ((unsigned int)((T_MY_MSG*)_message)->message == (unsigned int)E_WM_NCLBUTTONUP)
		{
			if (this->ptTouchCaptionMousePos != this->m_invalidPoint)
			{
				QPoint lc_deltaTouchXY = this->ptTouchCaptionMousePos - QCursor::pos();
				if (TxStdlib::absolute(lc_deltaTouchXY.x()) <= 1 && TxStdlib::absolute(lc_deltaTouchXY.y()) <= 1)
				{
					this->ptTouchCaptionMousePos = this->m_invalidPoint;
					this->vfTouchCaptionEvent();
				}
				else this->ptTouchCaptionMousePos = this->m_invalidPoint;
			}
		}
		else if ((unsigned int)((T_MY_MSG*)_message)->message == (unsigned int)E_WM_NCMOUSEMOVE)
		{
			if (this->ptTouchCaptionMousePos != this->m_invalidPoint)
			{
				QPoint lc_deltaTouchXY = this->ptTouchCaptionMousePos - QCursor::pos();
				if (TxStdlib::absolute(lc_deltaTouchXY.x()) > 2 || TxStdlib::absolute(lc_deltaTouchXY.y()) > 2)
					this->ptTouchCaptionMousePos = this->m_invalidPoint;
			}
		}
		else if ((unsigned int)((T_MY_MSG*)_message)->message == (unsigned int)E_WM_NCHITTEST && !this->is_fixed_window_())
		{
			long lc_result = E_HTCLIENT;
			QPoint lcCursorPoint = QCursor::pos();
			if (lc_fnIsWindowCaption(lcCursorPoint))
			{
				lc_result = E_HTCAPTION;
				QPoint lc_deltaTouchXY = this->ptTouchCaptionMousePos - QCursor::pos();
				if (TxStdlib::absolute(lc_deltaTouchXY.x()) <= 1 && TxStdlib::absolute(lc_deltaTouchXY.y()) <= 1
					&& !(qApp->mouseButtons()&Qt::LeftButton))
				{
					this->ptTouchCaptionMousePos = this->m_invalidPoint;
					this->vfTouchCaptionEvent();
				}
				else this->ptTouchCaptionMousePos = this->m_invalidPoint;
			}
			else if (this->uiContainerLayout->margin() > 0)
			{
				QPoint lc_wnd_pt = this->mapFromGlobal(lcCursorPoint);
				QRect lc_inner_rect = this->uiContainerWidget->geometry();
				if (lc_wnd_pt.x() <= lc_inner_rect.left())
				{
					if (lc_wnd_pt.y() <= lc_inner_rect.top())
					{
						if (this->minimumSize() != this->maximumSize())
							lc_result = E_HTTOPLEFT;
						else lc_result = E_HTBORDER;
					}
					else if (lc_wnd_pt.y() >= lc_inner_rect.bottom())
					{
						if (this->minimumSize() != this->maximumSize())
							lc_result = E_HTBOTTOMLEFT;
						else lc_result = E_HTBORDER;
					}
					else
					{
						if (this->minimumWidth() != this->maximumWidth())
							lc_result = E_HTLEFT;
						else lc_result = E_HTBORDER;
					}
				}
				else if (lc_wnd_pt.x() >= lc_inner_rect.right())
				{
					if (lc_wnd_pt.y() <= lc_inner_rect.top())
					{
						if (this->minimumSize() != this->maximumSize())
							lc_result = E_HTTOPRIGHT;
						else lc_result = E_HTBORDER;
					}
					else if (lc_wnd_pt.y() >= lc_inner_rect.bottom())
					{
						if (this->minimumSize() != this->maximumSize())
							lc_result = E_HTBOTTOMRIGHT;
						else lc_result = E_HTBORDER;
					}
					else
					{
						if (this->minimumWidth() != this->maximumWidth())
							lc_result = E_HTRIGHT;
						else lc_result = E_HTBORDER;
					}
				}
				else
				{
					if (lc_wnd_pt.y() <= lc_inner_rect.top())
					{
						if (this->minimumHeight() != this->maximumHeight())
							lc_result = E_HTTOP;
						else lc_result = E_HTBORDER;
					}
					else if (lc_wnd_pt.y() >= lc_inner_rect.bottom())
					{
						if (this->minimumHeight() != this->maximumHeight())
							lc_result = E_HTBOTTOM;
						else lc_result = E_HTBORDER;
					}
				}
			}
			*_result = lc_result;
			return true;
		}
		return __super::nativeEvent(_eventType, _message, _result);
	}
private:
	TxQtWindow(const TxQtWindow&)
	{
		sysLogError("TxQtWindow(const TxQtWindow&)不应该有此操作。");
		TxSysDebugObjectMemory::malloc(this);
	}
public:
	TxQtWindow(QWidget *_parent, const Qt::WindowFlags &_windowContainFlags = Qt::WindowFlags(),
		const Qt::WindowFlags &_windowEliminateFlags = Qt::WindowFlags()) :WND(_parent)
	{
		TxQtEnvironment::assertMainThread();
		TxSysDebugObjectMemory::malloc(this);
		this->m_DragTitleBarPoint = this->ptTouchCaptionMousePos = this->m_invalidPoint = QPoint(-65535 / 2 + 7, -65535 / 2 + 5);
		this->setWindowFlags((_windowContainFlags | Qt::FramelessWindowHint | Qt::Window)&(~_windowEliminateFlags));

		this->setAttribute(Qt::WA_TranslucentBackground, true);

		QWidget *lc_CentralWidget = this;
		QMainWindow *lc_MainWindow_Temp = dynamic_cast<QMainWindow*>(this);
		if (lc_MainWindow_Temp != NULL)
		{
			lc_CentralWidget = new QWidget(this);
			lc_MainWindow_Temp->setCentralWidget(lc_CentralWidget);
		}
		this->uiContainerLayout = new QGridLayout(lc_CentralWidget);
		this->uiContainerLayout->setSpacing(0);
		this->uiContainerWidget = new QWidget(lc_CentralWidget);
		this->uiContainerLayout->addWidget(this->uiContainerWidget);
		this->uiContainerLayout->setMargin(this->get_window_border_L_());
	}
	virtual ~TxQtWindow()
	{
		TxQtEnvironment::assertMainThread();
		this->getLifePeriod().destroy();
		TxSysDebugObjectMemory::free(this);
	}
	TxObjectLifePeriod getLifePeriod()
	{
		return TxObjectLifePeriod(&this->mLifeEntity);
	}
	QWidget* getContainerWidget()
	{
		TxQtEnvironment::assertMainThread();
		return this->uiContainerWidget;
	}
	void centerWidget(const QSize &_wnd_size, QWidget *_screen_widget = NULL)
	{
		TxQtEnvironment::assertMainThread();
		return _TxPrivateNs_::CQtlib::centerWidget(this, _wnd_size, _screen_widget);
	}
	void centerWidget(QWidget *_screen_widget = NULL)
	{
		TxQtEnvironment::assertMainThread();
		return _TxPrivateNs_::CQtlib::centerWidget(this, _screen_widget);
	}
	void centerWidget(double _dScreenSizeRatio, QWidget *_screen_widget = NULL)
	{
		TxQtEnvironment::assertMainThread();
		return _TxPrivateNs_::CQtlib::centerWidget(this, _dScreenSizeRatio, _screen_widget);
	}
};

class TxQtlib : public _TxPrivateNs_::CQtlib
{
public:
	static void initEnv()
	{
		assertMainThread();
		_TxPrivateNs_::_MyQtWindowGlobal::getInstance();
		__super::initEnv();
	}
};


