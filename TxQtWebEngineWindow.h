#pragma once

#include "TxStdlib.hpp"
#include "TxBoostlib.hpp"
#include "TxSyslib.hpp"
#include "TxQtlib.hpp"
#include "TxSqlitelib.hpp"

#include <map>
#include <memory>
#include <functional>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <boost/atomic.hpp>

#include <QtCore/QUrl>
#include <QtCore/QTimer>
#include <QtCore/QPoint>
#include <QtCore/QString>
#include <QtCore/QThread> 
#include <QtGui/QKeyEvent>
#include <QtGui/QClipboard>
#include <QtGui/QContextMenuEvent>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QtWidgets>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLabel>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineWidgets/QWebEnginePage>
#include <QtWebEngineWidgets/QWebEngineSettings>
#if QT_VERSION >= 0x050700
#include <QtWebEngineWidgets/QWebEngineContextMenuData>
#endif
#include <QtWebEngineWidgets/QWebEngineFullScreenRequest>
#include <QtWebEngineWidgets/QWebEngineHistory>
#include <QtWebChannel/QWebChannel>

#include <QtWebEngineWidgets/QWebEngineProfile>
#include <QtWebEngineCore/QWebEngineCookieStore>


namespace _TxPrivateNs_ {

	class QtWebChannelDataTransfer : public QObject
	{
		Q_OBJECT

			signals :
		void cpp_js(const QString &_text);
		public slots:
		void js_cpp(const QString &_text);
	private:
		std::function<void(const QString&)> js_cpp_cb_func;
	private:
		QtWebChannelDataTransfer(const QtWebChannelDataTransfer&);
	public:
		QtWebChannelDataTransfer(QWebChannel *_pWebChannel);
		~QtWebChannelDataTransfer();
		void emitCppToJs(const QString &_text);
		void setJsCppCallBack(const std::function<void(const QString&)> &_mfuc);
	};

	class MyWebEnginePageWrap_ : public QWebEnginePage
	{
	public :
		MyWebEnginePageWrap_(QWebEngineView *_parent);
		~MyWebEnginePageWrap_();
		virtual void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel _level,
			const QString &_message, int _lineNumber, const QString &_sourceID);
	};
}

class TxQtWebEngineView : public QWebEngineView
{
public:
	enum EnumWebResultCode
	{
		EWRC_ok = 0,
		EWRC_exception,
		EWRC_invalid_parameter,
		EWRC_error,
	};
private:
	std::list<QString> listLoadJsFile;
	TxObjectLifePeriod::TxObjectEntity mLifeEntity;
	_TxPrivateNs_::QtWebChannelDataTransfer *pQtWebChannelDataTransfer;
	_TxPrivateNs_::MyWebEnginePageWrap_ *pWebEnginePageWrap;
private:
	TxQtWebEngineView(TxQtWebEngineView&)
	{
		TxSysDebugObjectMemory::malloc(this);
		sysLogError("TxQtWebEngineView(TxQtWebEngineView&)不应该有此操作。");
	}
	const TxQtWebEngineView& operator=(TxQtWebEngineView&) const
	{
		sysLogError("const TxQtWebEngineView& operator=(TxQtWebEngineView&) const不应该有此操作。");
		return *(const TxQtWebEngineView*)NULL;
	}
	void init_channel_();
	void _loadExecJsFile_(const TxObjectLifePeriod &_lifePeriod,
		std::list<QString>::const_iterator _iter, const std::list<QString> &_listLoadJsFile);
	void _write_qtwrap_js_(const QUrl &_url, const QString &_qtwrapFile);
protected:
	virtual bool vfJsToCpp(const TxBoostMarkup<false> &_rqt_pt, const TxBlobWString &_json_str);
public:
	TxQtWebEngineView(QWidget *_parent);
	virtual ~TxQtWebEngineView();
	_TxPrivateNs_::MyWebEnginePageWrap_* _getWebEnginePageWrap_() { return this->pWebEnginePageWrap; }
	void emitCppToJs(const QString &_text);
	void setUrl(const QUrl &_url,
		const QString &_qtwrapFile = QString(),
		const std::list<QString> &_listLoadJsFile = std::list<QString>());
	void triggerWebAction(QWebEnginePage::WebAction _action);
	static void enableRemoteDebug(int _port);
	static void getRequestJsonField(TxBlobString *_method, TxBlobString *_userdata, const TxBoostMarkup<false> &_request_pt);
	static void putRespondJsonTree(TxBoostMarkup<false> *_respond_pt, const TxBlobString &_method, const TxBlobString &_userdata, EnumWebResultCode _ewrc);
	static void putRespondJsonTree(TxBoostMarkup<false> *_respond_pt, const TxBoostMarkup<false> &_request_pt, EnumWebResultCode _ewrc);
	TxObjectLifePeriod getLifePeriod();
	bool _dealJsToCpp_(const TxBoostMarkup<false> &_rqt_pt, const TxBlobWString &_json_str);
#if QT_VERSION >= 0x050700
	void contextMenuEvent(QContextMenuEvent *_event);
#endif
	void clearWebCache()
	{
		if (this->pWebEnginePageWrap != NULL)
		{
			QWebEngineHistory *lc_history=this->pWebEnginePageWrap->history();
			if (lc_history != NULL)
				lc_history->clear();

			QWebEngineProfile *lc_profile = this->pWebEnginePageWrap->profile();
			if (lc_profile != NULL)
			{
				lc_profile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
				lc_profile->clearHttpCache(); // 清理缓存
				lc_profile->clearAllVisitedLinks(); // 清理浏览记录
				QWebEngineCookieStore* lc_cookieStore = lc_profile->cookieStore();
				if (lc_cookieStore != NULL)
				{
					lc_cookieStore->deleteAllCookies();
					lc_cookieStore->deleteSessionCookies();
				}
			}
		}
	}
	//template<class TFUNC>//std::function<void(TxBoostMarkup<false>*)>
	//void threadProcessJavaScriptCallBack(const TFUNC &_mfunc)
	//{
	//	TxObjectLifePeriod lcLifePeriod(&this->mLifeEntity);
	//	TxQtWebEngineView* lc_webview = this;
	//	_TxPrivateNs_::_MyWebSingleton_::getInstance()->postThreadTask([lcLifePeriod, lc_webview, _mfunc]() {
	//		TxBoostMarkup<false> lc_rpd_pt;
	//		_mfunc(&lc_rpd_pt);
	//		if (!lc_rpd_pt.getBoostTree()->empty())
	//		{
	//			TxBlobWString lc_json_string = lc_rpd_pt.saveJsonString().utf8Codecvt();
	//			TxQtEnvironment::postTask([lcLifePeriod, lc_webview, lc_json_string]() {
	//				if (lcLifePeriod)
	//					lc_webview->emitCppToJs(QString::fromStdWString(lc_json_string));
	//			});
	//		}
	//	}));
	//}
};

namespace _TxPrivateNs_ {
	template<class WND>
	class MyWebEngineWindow : public TxQtWindow<WND>
	{
	private:
		QStackedWidget *uiStackedWidget;//栈UI=uiWebEngineContainerPage + uiStackedDisplayUserPage
		QWidget *uiWebEngineContainerPage;//web页面（总体）=uiWebPageTitlebarContainer+spWebEngineView
		QGridLayout *uiWebEngineContainerLyt;//uiWebEngineContainerLyt=uiWebEngineContainerPage
		QWidget *uiWebPageTitlebarContainer;//web页面的标题栏
		std::shared_ptr<TxQtWebEngineView> spWebEngineView;//浏览器
		QWidget *uiStackedDisplayUserPage;//用户自定义界面
		TxBlobString objectGuid;
	private:
		TxBlobString _getQtUiObjectName_()
		{
			return TxBlobString("N") + this->objectGuid + "Web";
		}
		void init_frame_ui_()
		{
			this->uiStackedWidget = new QStackedWidget(this->getContainerWidget());
			this->uiStackedDisplayUserPage = new QWidget();
			//加入用户自定义页面
			this->uiStackedWidget->addWidget(this->uiStackedDisplayUserPage);
			//加入web页面
			this->uiWebEngineContainerPage = new QWidget(this->uiStackedDisplayUserPage);
			this->uiWebEngineContainerPage->setObjectName(
				QString::fromStdWString(this->_getQtUiObjectName_().utf8Codecvt()));
			this->uiStackedWidget->addWidget(this->uiWebEngineContainerPage);
			//整理web页面布局
			this->uiWebEngineContainerLyt = new QGridLayout(this->uiWebEngineContainerPage);
			this->uiWebEngineContainerLyt->setSpacing(0);
			this->uiWebEngineContainerLyt->setContentsMargins(0, 0, 0, 0);
			//web页面加入标题栏
			this->uiWebPageTitlebarContainer = new QWidget(this->uiWebEngineContainerPage);
			this->uiWebEngineContainerLyt->addWidget(this->uiWebPageTitlebarContainer, 0, 0);
			this->uiWebPageTitlebarContainer->setVisible(false);
			//web页面加入浏览器
			this->spWebEngineView = this->_newTxQtWebEngineView_();
			this->uiWebEngineContainerLyt->addWidget(this->spWebEngineView.get(), 1, 0);
			//设置StackedWidget当前显示页面
			this->uiStackedWidget->setCurrentWidget(this->uiWebEngineContainerPage);

			QGridLayout *lc_lyt = new QGridLayout(this->getContainerWidget());
			lc_lyt->setMargin(0);
			lc_lyt->setSpacing(0);
			lc_lyt->addWidget(this->uiStackedWidget);
		}
		void init_ui_event_()
		{
			QObject::connect(this->spWebEngineView->_getWebEnginePageWrap_(),
				&QWebEnginePage::fullScreenRequested, [this](QWebEngineFullScreenRequest _request) {
				assert(false);
				(void)&_request;
				this->showFullScreen();
			});
			QObject::connect(this->spWebEngineView->_getWebEnginePageWrap_(),
				&QWebEnginePage::geometryChangeRequested, [this](const QRect &_geom) {
				assert(false);
				this->setGeometry(_geom);
			});
			QObject::connect(this->spWebEngineView->_getWebEnginePageWrap_(),
				&QWebEnginePage::windowCloseRequested, [this]() {
				this->close();
			});
		}
		std::shared_ptr<TxQtWebEngineView> _newTxQtWebEngineView_()
		{
			class TxQtWebEngineViewImpl : public TxQtWebEngineView
			{
			private:
				MyWebEngineWindow *const pThis;
			public:
				TxQtWebEngineViewImpl(MyWebEngineWindow *_pThis) :pThis(_pThis), TxQtWebEngineView(NULL)
				{
				}
				~TxQtWebEngineViewImpl()
				{
					this->getLifePeriod().destroy();
				}
				virtual bool vfJsToCpp(const TxBoostMarkup<false> &_rqt_pt, const TxBlobWString &_json_str)
				{
					return this->pThis->vfJsToCpp(_rqt_pt, _json_str);
				}
			};
			return std::make_shared<TxQtWebEngineViewImpl>(this);
		}
		void _setWebBackground_(const QMargins &_margins, const QColor *_color, const QString *_image_file)
		{
			TxBlobString lc_ts_qss;
			if (_color != NULL)
			{
				if (_color->alpha() >= 255)
				{
					lc_ts_qss.compose("#{0}{background:rgb({1},{2},{3});}").args(_getQtUiObjectName_(),
						_color->red(), _color->green(), _color->blue());
				}
				else
				{
					lc_ts_qss.compose("#{0}{background:rgba({1},{2},{3},{4});}").args(_getQtUiObjectName_(),
						_color->red(), _color->green(), _color->blue(), _color->alpha());
				}
			}
			else if (_image_file != NULL)
			{
				lc_ts_qss.compose("#{0}{border-image:url({1});}").args(_getQtUiObjectName_(),
					_image_file->toStdWString());
			}
			QString lc_qs_qss = QString::fromStdWString(lc_ts_qss.utf8Codecvt());
			if (lc_qs_qss != this->uiWebEngineContainerPage->styleSheet())
				this->uiWebEngineContainerPage->setStyleSheet(lc_qs_qss);
			if (_margins != this->uiWebEngineContainerLyt->contentsMargins())
				this->uiWebEngineContainerLyt->setContentsMargins(_margins);
		}
	protected:
		virtual bool vfJsToCpp(const TxBoostMarkup<false> &_rqt_pt, const TxBlobWString &_json_str) = 0;
//#ifndef NDEBUG
//		virtual void keyPressEvent(QKeyEvent *_keyEvent)
//		{
//			if (_keyEvent->key() == Qt::Key_F5 && (_keyEvent->modifiers()&Qt::ControlModifier))
//				this->triggerWebAction(QWebEnginePage::ReloadAndBypassCache);
//			return __super::keyPressEvent(_keyEvent);
//		}
//#endif
	public:
		MyWebEngineWindow(QWidget *_parent) :TxQtWindow<WND>(_parent)
		{
			TxQtEnvironment::assertMainThread();
			this->objectGuid = TxSyslib::generateGuid()
				+ TxBlobString::fromDatum(
				(unsigned int)((unsigned long long)TxBoostlib::randomUint64() % 100000000));
			this->init_frame_ui_();
			this->init_ui_event_();
		}
		~MyWebEngineWindow()
		{
			TxQtEnvironment::assertMainThread();
			this->getLifePeriod().destroy();
		}
		static void enableRemoteDebug(int _port)
		{
			TxQtWebEngineView::enableRemoteDebug(_port)
		}
		void setWebBackground(const QMargins &_margins, const QColor &_color)
		{
			return this->_setWebBackground_(_margins, &_color, NULL);
		}
		void setWebBackground(const QMargins &_margins, const QString &_image_file)
		{
			return this->_setWebBackground_(_margins, NULL, &_image_file);
		}
		void setUrl(const QUrl &_url,
			const QString &_qtwrapFile = QString(),
			const std::list<QString> &_listLoadJsFile = std::list<QString>())
		{
			this->spWebEngineView->setUrl(_url, _qtwrapFile, _listLoadJsFile);
		}
		void triggerWebAction(QWebEnginePage::WebAction _action)
		{
			return this->spWebEngineView->triggerWebAction(_action);
		}
		void emitCppToJs(const QString &_text)
		{
			this->spWebEngineView->emitCppToJs(_text);
		}
		void clearWebCache()
		{
			this->spWebEngineView->clearWebCache();
		}
		void selectStackedDisplayWebPage(bool _bWebPage)
		{
			TxQtEnvironment::assertMainThread();
			QWidget *lc_cur_page = _bWebPage ? (QWidget*)this->uiWebEngineContainerPage
				: (QWidget*)this->uiStackedDisplayUserPage;
			if (this->uiStackedWidget->currentWidget() != lc_cur_page)
				this->uiStackedWidget->setCurrentWidget(lc_cur_page);
		}
		bool isStackedDisplayWebPage() const
		{
			TxQtEnvironment::assertMainThread();
			return this->uiStackedWidget->currentWidget() == this->uiWebEngineContainerPage;
		}
		QWidget* getStackedDisplayUserPage() const
		{
			TxQtEnvironment::assertMainThread();
			return const_cast<QWidget*>(this->uiStackedDisplayUserPage);
		}
		QWidget* getWebPageTitlebarContainer() const
		{
			TxQtEnvironment::assertMainThread();
			return const_cast<QWidget*>(this->uiWebPageTitlebarContainer);
		}
		TxQtWebEngineView* getWebEngineView()
		{
			return this->spWebEngineView.get();
		}
	};
}

template<class WND>
class TxQtWebEngineWindow : public _TxPrivateNs_::MyWebEngineWindow<WND>
{
protected:
	struct {
		std::list<QRect> mListWebContain, mListWebEliminate;
	} mCaptionGeometry;
private:
	TxQtWebEngineWindow(const TxQtWebEngineWindow &)
	{
		sysLogError("TxQtWebEngineWindow(const TxQtWebEngineWindow &)不应该有此操作");
		TxSysDebugObjectMemory::malloc(this);
	}
protected:
	TxQtWebEngineWindow(QWidget *_parent) :_TxPrivateNs_::MyWebEngineWindow<WND>(_parent)
	{
		TxSysDebugObjectMemory::malloc(this);
	}
	virtual ~TxQtWebEngineWindow()
	{
		TxSysDebugObjectMemory::free(this);
		this->getLifePeriod().destroy();
	}
	virtual bool vfJsToCpp(const TxBoostMarkup<false> &_rqt_pt, const TxBlobWString &_json_str)
	{
		TxQtEnvironment::assertMainThread();
		TxBoostMarkup<false> lc_rpd_pt;
		TxBlobString lc_method, lc_userdata;
		TxQtWebEngineView::getRequestJsonField(&lc_method, &lc_userdata, _rqt_pt);

		bool ret_true = true;
		if (lc_method == "kernel.set_caption_geometry")
		{
			TxBoostMarkup<false> lc_arry_pt[] = {
				_rqt_pt.getPtreeChild("params.contain"),
				_rqt_pt.getPtreeChild("params.eliminate")
			};
			std::list<QRect>* lc_arry_rect[] = {
				&this->mCaptionGeometry.mListWebContain,
				&this->mCaptionGeometry.mListWebEliminate
			};
			for (int ii = 0; ii < 2; ++ii)
			{
				lc_arry_rect[ii]->clear();
				for (auto iter = lc_arry_pt[ii].getBoostTree()->begin();
					iter != lc_arry_pt[ii].getBoostTree()->end(); ++iter)
				{
					TxBoostMarkup<false> lc_elem_pt;
					lc_elem_pt.attach(&iter->second);
					int lc_left = lc_elem_pt.getValueString("left").toDatumRet(0);
					int lc_top = lc_elem_pt.getValueString("top").toDatumRet(0);
					int lc_width = lc_elem_pt.getValueString("width", false).toDatumRet(-1);
					int lc_height = lc_elem_pt.getValueString("height", false).toDatumRet(-1);
					if (lc_width > 0 && lc_height > 0)
						lc_arry_rect[ii]->push_back(QRect(QPoint(lc_left, lc_top), QSize(lc_width, lc_height)));
				}
			}
			TxQtWebEngineView::putRespondJsonTree(&lc_rpd_pt, _rqt_pt, TxQtWebEngineView::EWRC_ok);
		}
		else if (lc_method == "kernel.set_size")
		{
			int lc_width = _rqt_pt.getValueString("params.width", false).toDatumRet(-1);
			int lc_height = _rqt_pt.getValueString("params.height", false).toDatumRet(-1);
			float lc_ratio = _rqt_pt.getValueString("params.ratio", false).toDatumRet(-1.0f);
			TxQtWebEngineView::EnumWebResultCode lc_result_code = TxQtWebEngineView::EWRC_ok;
			if (lc_width > 0 && lc_height > 0)
			{
				this->centerWidget(QSize(lc_width, lc_height));
				lc_result_code = TxQtWebEngineView::EWRC_ok;
			}
			else if (lc_ratio > 0)
			{
				this->centerWidget(lc_ratio);
				lc_result_code = TxQtWebEngineView::EWRC_ok;
			}
			else
			{
				lc_result_code = TxQtWebEngineView::EWRC_invalid_parameter;
			}
			TxQtWebEngineView::putRespondJsonTree(&lc_rpd_pt, _rqt_pt, lc_result_code);
		}
		else if (lc_method == "kernel.set_geometry")
		{
			int lc_left = _rqt_pt.getValueString("params.left").toDatumRet(0);
			int lc_top = _rqt_pt.getValueString("params.top").toDatumRet(0);
			int lc_width = _rqt_pt.getValueString("params.width", false).toDatumRet(-1);
			int lc_height = _rqt_pt.getValueString("params.height", false).toDatumRet(-1);
			TxQtWebEngineView::EnumWebResultCode lc_result_code = TxQtWebEngineView::EWRC_ok;
			if (lc_width > 0 && lc_height > 0)
			{
				this->setGeometry(QRect(QPoint(lc_left, lc_top), QSize(lc_width, lc_height)));
				lc_result_code = TxQtWebEngineView::EWRC_ok;
			}
			else
			{
				lc_result_code = TxQtWebEngineView::EWRC_invalid_parameter;
			}
			TxQtWebEngineView::putRespondJsonTree(&lc_rpd_pt, _rqt_pt, lc_result_code);
		}
		else if (lc_method == "kernel.get_geometry")
		{
			QRect lc_rect = this->geometry();
			TxQtWebEngineView::putRespondJsonTree(&lc_rpd_pt, _rqt_pt, TxQtWebEngineView::EWRC_ok);
			lc_rpd_pt.putValue("params.left", lc_rect.left());
			lc_rpd_pt.putValue("params.top", lc_rect.top());
			lc_rpd_pt.putValue("params.width", lc_rect.width());
			lc_rpd_pt.putValue("params.height", lc_rect.height());
			lc_rpd_pt.putValue("params.right", lc_rect.right());
			lc_rpd_pt.putValue("params.bottom", lc_rect.bottom());
		}
		else if (lc_method == "kernel.set_state")
		{
			TxQtWebEngineView::EnumWebResultCode lc_result_code = TxQtWebEngineView::EWRC_ok;
			TxBlobString lc_state = _rqt_pt.getValueString("params.state");
			if (lc_state == "close")
				this->close();
			else if (lc_state == "maximize")
				this->showMaximized();
			else if (lc_state == "minimize")
				this->showMinimized();
			else if (lc_state == "restore")
				this->showNormal();
			else if (lc_state == "fullscreen")
				this->showFullScreen();
			else if (lc_state == "hide")
				this->hide();
			else if (lc_state == "show")
				this->show();
			else
			{
				assert(false);
				lc_result_code = TxQtWebEngineView::EWRC_invalid_parameter;
			}
			TxQtWebEngineView::putRespondJsonTree(&lc_rpd_pt, _rqt_pt, lc_result_code);
		}
		else if (lc_method == "kernel.get_state")
		{
			TxQtWebEngineView::putRespondJsonTree(&lc_rpd_pt, _rqt_pt, TxQtWebEngineView::EWRC_ok);
			lc_rpd_pt.putValue("params.visible", (this->isVisible() ? "true" : "false"));
			lc_rpd_pt.putValue("params.maximize", (this->isMaximized() ? "true" : "false"));
			lc_rpd_pt.putValue("params.minimize", (this->isMinimized() ? "true" : "false"));
			lc_rpd_pt.putValue("params.fullscreen", (this->isFullScreen() ? "true" : "false"));
		}
		else if (lc_method == "kernel.set_size_range")
		{
			int lc_min_width = _rqt_pt.getValueString("params.min_size.width", false).toDatumRet(-1);
			int lc_min_height = _rqt_pt.getValueString("params.min_size.height", false).toDatumRet(-1);
			int lc_max_width = _rqt_pt.getValueString("params.max_size.width", false).toDatumRet(-1);
			int lc_max_height = _rqt_pt.getValueString("params.max_size.height", false).toDatumRet(-1);
			TxQtWebEngineView::EnumWebResultCode lc_result_code = TxQtWebEngineView::EWRC_invalid_parameter;
			if (lc_min_width >= 0)
			{
				this->setMinimumWidth(lc_min_width);
				lc_result_code = TxQtWebEngineView::EWRC_ok;
			}
			if (lc_min_height >= 0)
			{
				this->setMinimumHeight(lc_min_height);
				lc_result_code = TxQtWebEngineView::EWRC_ok;
			}
			if (lc_max_width >= 0)
			{
				this->setMaximumWidth(lc_max_width);
				lc_result_code = TxQtWebEngineView::EWRC_ok;
			}
			if (lc_max_height >= 0)
			{
				this->setMaximumHeight(lc_max_height);
				lc_result_code = TxQtWebEngineView::EWRC_ok;
			}
			TxQtWebEngineView::putRespondJsonTree(&lc_rpd_pt, _rqt_pt, lc_result_code);
		}
		else
		{
			ret_true = false;
		}
		if (!lc_rpd_pt.getBoostTree()->empty())
			this->emitCppToJs(QString::fromStdWString(lc_rpd_pt.saveJsonString().utf8Codecvt()));
		if (ret_true)
			return true;
		return this->getWebEngineView()->_dealJsToCpp_(_rqt_pt, _json_str);
	}
	virtual int vfGetCaptionHeight()
	{
		TxQtEnvironment::assertMainThread();
		if (!this->isStackedDisplayWebPage())
			return this->height();
		if (this->mCaptionGeometry.mListWebContain.size() > 0)
			return 0;
		if (!__super::getWebPageTitlebarContainer()->isVisible())
			return 0;
		return this->getWebPageTitlebarContainer()->mapTo(this->getContainerWidget(),
			QPoint(0, __super::getWebPageTitlebarContainer()->height())).y();
	}
	virtual bool vfIsWindowCaption(const QPoint &_pt)
	{
		TxQtEnvironment::assertMainThread();
		if (!this->isStackedDisplayWebPage())
			return false;
		QPoint lc_web_pt = this->getWebEngineView()->mapFrom(this->getContainerWidget(), _pt);
		for (auto iter = this->mCaptionGeometry.mListWebContain.begin();; ++iter)
		{
			if (iter == this->mCaptionGeometry.mListWebContain.end())
				return false;
			if (iter->contains(lc_web_pt))
				break;
		}
		for (auto iter = this->mCaptionGeometry.mListWebEliminate.begin();
			iter != this->mCaptionGeometry.mListWebEliminate.end(); ++iter)
		{
			if (iter->contains(lc_web_pt))
				return false;
		}
		return true;
	}
};

