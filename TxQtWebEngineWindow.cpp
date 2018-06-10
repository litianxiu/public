#include "TxQtWebEngineWindow.h"


_TxPrivateNs_::QtWebChannelDataTransfer::QtWebChannelDataTransfer(QWebChannel *_pWebChannel) :QObject(_pWebChannel)
{
	TxSysDebugObjectMemory::malloc(this);
	TxQtEnvironment::assertMainThread();
	_pWebChannel->registerObject(QString::fromWCharArray(L"QtWebChannelDataTransfer"), this);
}

_TxPrivateNs_::QtWebChannelDataTransfer::QtWebChannelDataTransfer(const QtWebChannelDataTransfer&)
{
	assert(false);
	TxSysDebugObjectMemory::malloc(this);
}

_TxPrivateNs_::QtWebChannelDataTransfer::~QtWebChannelDataTransfer()
{
	TxSysDebugObjectMemory::free(this);
}
void _TxPrivateNs_::QtWebChannelDataTransfer::emitCppToJs(const QString &_text)
{
	TxQtEnvironment::assertMainThread();
	emit cpp_js(_text);
}
void _TxPrivateNs_::QtWebChannelDataTransfer::js_cpp(const QString &_text)
{
	TxQtEnvironment::assertMainThread();
	if (this->js_cpp_cb_func)
		this->js_cpp_cb_func(_text);
}
void _TxPrivateNs_::QtWebChannelDataTransfer::setJsCppCallBack(const std::function<void(const QString&)> &_mfuc)
{
	TxQtEnvironment::assertMainThread();
	this->js_cpp_cb_func = _mfuc;
}


_TxPrivateNs_::MyWebEnginePageWrap_::MyWebEnginePageWrap_(QWebEngineView *_parent) :QWebEnginePage(_parent)
{
	TxSysDebugObjectMemory::malloc(this);
}
_TxPrivateNs_::MyWebEnginePageWrap_::~MyWebEnginePageWrap_()
{
	TxSysDebugObjectMemory::free(this);
}
void _TxPrivateNs_::MyWebEnginePageWrap_::javaScriptConsoleMessage(JavaScriptConsoleMessageLevel _level,
	const QString &_message, int _lineNumber, const QString &_sourceID)
{
	switch (_level)
	{
	case InfoMessageLevel:
		sysLogInfo("_level=[{0}], _message=[{1}], _lineNumber=[{2}], _sourceID=[{3}]").args(
			_level, _message.toStdWString(), _lineNumber, _sourceID.toStdWString());
		break;
	case WarningMessageLevel:
		sysLogWarn("_level=[{0}], _message=[{1}], _lineNumber=[{2}], _sourceID=[{3}]").args(
			_level, _message.toStdWString(), _lineNumber, _sourceID.toStdWString());
		break;
	case ErrorMessageLevel:
	default:
		sysLogError("_level=[{0}], _message=[{1}], _lineNumber=[{2}], _sourceID=[{3}]").args(
			_level, _message.toStdWString(), _lineNumber, _sourceID.toStdWString());
		break;
	}
	return __super::javaScriptConsoleMessage(_level, _message, _lineNumber, _sourceID);
}


TxQtWebEngineView::TxQtWebEngineView(QWidget *_parent) :QWebEngineView(_parent)
{
	TxSysDebugObjectMemory::malloc(this);
	this->pWebEnginePageWrap=new _TxPrivateNs_::MyWebEnginePageWrap_(this);
	this->setPage(this->pWebEnginePageWrap);
	this->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
	this->init_channel_();
#if !(QT_VERSION >= 0x050700)
	this->setContextMenuPolicy(Qt::NoContextMenu);
#endif

	QObject::connect(this->pWebEnginePageWrap, &QWebEnginePage::loadFinished, [this](bool _ok) {
		if (_ok)
		{
			if (this->listLoadJsFile.size() > 0)
			{
				TxObjectLifePeriod lcLifePeriod = this->getLifePeriod();
				TxQtEnvironment::postTask([lcLifePeriod, this]() {
					if (lcLifePeriod)
						this->_loadExecJsFile_(lcLifePeriod, this->listLoadJsFile.cbegin(), this->listLoadJsFile);
				});
			}
		}
		else sysLogWarn("QWebEnginePage::loadFinished加载网页失败。");
	});
}
TxQtWebEngineView::~TxQtWebEngineView()
{
	TxSysDebugObjectMemory::free(this);
	this->getLifePeriod().destroy();
}
void TxQtWebEngineView::init_channel_()
{
	QWebChannel *lc_channel = new QWebChannel(this);
	this->pQtWebChannelDataTransfer = new _TxPrivateNs_::QtWebChannelDataTransfer(lc_channel);
	this->pQtWebChannelDataTransfer->setJsCppCallBack([this](const QString &_text) {
		TxQtEnvironment::assertMainThread();
		TxBoostMarkup<false> lc_rqt_pt;
		TxBlobWString lc_json_str = _text.toStdWString();
		if (!lc_rqt_pt.parseJsonString(lc_json_str.utf8Codecvt()))
		{
			sysLogError("Js回调，解析json数据失败。_text=【{0}】").args(_text.toStdWString());
		}
		else
		{
			TxBlobString lc_log_attr = lc_rqt_pt.getValueString("log");
			if (lc_log_attr.length() > 0 && (lc_log_attr.equalNoCase("true") || lc_log_attr.equalNoCase("1")))
				sysLogInfo("接收到JavaScript指令：{0}").args(lc_json_str);
			if (!this->vfJsToCpp(lc_rqt_pt, lc_json_str))
			{
				sysLogError("JS回调消息处理失败，无法识别指令。lc_json_str=【{0}】").args(lc_json_str);
				TxBoostMarkup<false> lc_rpd_pt;
				TxBlobString lc_method, lc_userdata;
				getRequestJsonField(&lc_method, &lc_userdata, lc_rqt_pt);
				if (lc_method.length() > 0 && lc_userdata.length() > 0)
				{
					putRespondJsonTree(&lc_rpd_pt, lc_rqt_pt, EWRC_exception);
					this->emitCppToJs(QString::fromStdWString(lc_rpd_pt.saveJsonString().utf8Codecvt()));
				}
			}
		}
	});
	this->pWebEnginePageWrap->setWebChannel(lc_channel);
}
void TxQtWebEngineView::_loadExecJsFile_(const TxObjectLifePeriod &_lifePeriod,
	std::list<QString>::const_iterator _iter, const std::list<QString> &_listLoadJsFile)
{
	if (!_lifePeriod)
		return;
	if (_iter == _listLoadJsFile.cend())
		return;
	for (auto lc_iter = _listLoadJsFile.cbegin();; ++lc_iter)
	{
		if (lc_iter == _listLoadJsFile.cend())
		{
			sysLogError("if (lc_iter == _listLoadJsFile.cend())执行异常。_iter->toStdWString()=【{0}】").args(
				_iter->toStdWString());
			return;
		}
		if (lc_iter == _iter)
		{
			break;
		}
	}
	TxBlobWString lc_ws_js;
	QFile lcfile(*_iter);
	if (!lcfile.open(QIODevice::ReadOnly))
	{
		sysLogError("if (!lcfile.open(QIODevice::ReadOnly))打开文件失败。_iter->toStdWString()=【{0}】").args(_iter->toStdWString());
	}
	else
	{
		QByteArray lcFileData = lcfile.readAll();
		lcfile.close();
		lc_ws_js = TxBlobString(TxBlobString(lcFileData.data(), lcFileData.size()).c_str()).utf8Codecvt();
	}
	if (lc_ws_js.length() <= 0)
		_loadExecJsFile_(_lifePeriod, ++_iter, _listLoadJsFile);
	else
	{
		this->pWebEnginePageWrap->runJavaScript(QString::fromStdWString(lc_ws_js), [_lifePeriod, _iter, &_listLoadJsFile, this](const QVariant &_v) {
			TX_UNUSED(_v);
			std::list<QString>::const_iterator lc_iter = _iter;
			_loadExecJsFile_(_lifePeriod, ++lc_iter, _listLoadJsFile);
		});
	}
}
void TxQtWebEngineView::_write_qtwrap_js_(const QUrl &_url, const QString &_qtwrapFile)
{
	if (_qtwrapFile.length() <= 0)
		return;
	QFile lcfile(_qtwrapFile);
	if (!lcfile.open(QIODevice::ReadOnly))
	{
		sysLogError("if (!lcfile.open(QIODevice::ReadOnly))打开文件失败。_qtwrapFile=【{0}】"
		).args(_qtwrapFile.toStdWString());
		return;
	}
	auto lc_langBase64Fn = [](const std::map<TxBlobString, QString> &_langMap)->TxBlobString {
		TxBoostMarkup<false> lc_pt;
		for (auto iter = _langMap.cbegin(); iter != _langMap.cend(); ++iter)
			lc_pt.putValue(iter->first, iter->second.toUtf8().toStdString());
		return TxStdlib::base64Encode(lc_pt.saveJsonString(), false);
	};
	QByteArray lcFileData = lcfile.readAll();
	lcfile.close();
	TxBlobString lc_jsFileData;
	lc_jsFileData.compose(
		TxBlobString(lcFileData.data(), lcFileData.size()).c_str()).args(
			lc_langBase64Fn(TxQtEnvironment::getLanguageData()));

	TxBlobWString lc_fileNodeName = _qtwrapFile.toStdWString();
	lc_fileNodeName.replaceSelf(L'\\', L'/');
	lc_fileNodeName.trimSelfRight(L"\\");
	lc_fileNodeName = TxBlobWString(std::wcsrchr(lc_fileNodeName.c_str(), L'\\')).trimLR(L"\\");
	if (lc_fileNodeName.length() <= 0)
		return;
	TxBlobWString lc_localFile = _url.toLocalFile().toStdWString();
	lc_localFile.replaceSelf(L'\\', L'/').trimSelfRight(L"\\/\r\n\t ");
	if (lc_localFile.length() <= 0)
		return;
	if (!TxBoostlib::isExistFile(lc_localFile))
		return;
	TxBlobWString lc_localQtwrapFile = TxBoostlib::fileDirectory(lc_localFile);
	if (lc_localQtwrapFile.length() <= 0)
		return;
	lc_localQtwrapFile.replaceSelf(L'\\', L'/');
	if (lc_localQtwrapFile.backChar() != L'\\')
		lc_localQtwrapFile.append(L"\\");
	lc_localQtwrapFile.append(lc_fileNodeName);
	if (lc_jsFileData == TxBoostlib::readFile(lc_localQtwrapFile))
		return;
	TxBoostlib::deleteAllFile(lc_localQtwrapFile);
	TxBoostlib::writeFile(lc_localQtwrapFile, lc_jsFileData);
}

void TxQtWebEngineView::setUrl(const QUrl &_url,
	const QString &_qtwrapFile,
	const std::list<QString> &_listLoadJsFile)
{
	TxQtEnvironment::assertMainThread();
	this->_write_qtwrap_js_(_url, _qtwrapFile);
	this->listLoadJsFile = _listLoadJsFile;
	__super::setUrl(_url);
}
void TxQtWebEngineView::triggerWebAction(QWebEnginePage::WebAction _action)
{
	TxQtEnvironment::assertMainThread();
	__super::triggerPageAction(_action);
}
void TxQtWebEngineView::getRequestJsonField(TxBlobString *_method, TxBlobString *_userdata, const TxBoostMarkup<false> &_request_pt)
{
	if (_method != NULL)
		*_method = _request_pt.getValueString("method");
	if (_userdata != NULL)
		*_userdata = _request_pt.getValueString("userdata", false);
}
 void TxQtWebEngineView::enableRemoteDebug(int _port)
 {
	 TxQtEnvironment::assertMainThread();
	(void)_port;
#ifndef NDEBUG
	static bool lc_enabled_web_engine_debug = false;
	if (lc_enabled_web_engine_debug)
		assert(false);
	else
	{
		lc_enabled_web_engine_debug = true;
		char lc_port_chs[64] = { 0 };
		::sprintf_s(lc_port_chs, sizeof(lc_port_chs) / sizeof(lc_port_chs[0]) - 1, "%d", (int)_port);
		::qputenv("QTWEBENGINE_REMOTE_DEBUGGING", lc_port_chs);
	}
#endif
}
void TxQtWebEngineView::putRespondJsonTree(TxBoostMarkup<false> *_respond_pt,
	const TxBlobString &_method, const TxBlobString &_userdata, EnumWebResultCode _ewrc)
{
	if (_method.length() <= 0) assert(false);
	else _respond_pt->putValue("method", _method);

	if (_userdata.length() > 0)
		_respond_pt->putValue("userdata", _userdata);

	TxBlobString lc_result;
	switch (_ewrc)
	{
	case TxQtWebEngineView::EWRC_ok:
		lc_result = "ok";
		break;
	case TxQtWebEngineView::EWRC_exception:
		lc_result = "unknown";
		break;
	case TxQtWebEngineView::EWRC_invalid_parameter:
		lc_result = "invalid_parameter";
		break;
	case TxQtWebEngineView::EWRC_error:
		lc_result = "error";
		break;
	default:
		assert(false);
		lc_result = "unknown";
		break;
	}
	_respond_pt->putValue("result", lc_result);
	if (!_respond_pt->isPathExist("params"))
		_respond_pt->putArrayTree("params", TxBoostMarkup<false>());
}
void TxQtWebEngineView::putRespondJsonTree(TxBoostMarkup<false> *_respond_pt,
	const TxBoostMarkup<false> &_request_pt, EnumWebResultCode _ewrc)
{
	TxBlobString lc_method, lc_userdata;
	getRequestJsonField(&lc_method, &lc_userdata, _request_pt);
	return putRespondJsonTree(_respond_pt, lc_method, lc_userdata, _ewrc);
}
TxObjectLifePeriod TxQtWebEngineView::getLifePeriod()
{
	return TxObjectLifePeriod(&this->mLifeEntity);
}
bool TxQtWebEngineView::vfJsToCpp(const TxBoostMarkup<false> &_rqt_pt, const TxBlobWString &_json_str)
{
	return this->_dealJsToCpp_(_rqt_pt, _json_str);
}
bool TxQtWebEngineView::_dealJsToCpp_(const TxBoostMarkup<false> &_rqt_pt, const TxBlobWString &_json_str)
{
	TxQtEnvironment::assertMainThread();
	TxBoostMarkup<false> lc_rpd_pt;
	TxBlobString lc_method, lc_userdata;
	TxQtWebEngineView::getRequestJsonField(&lc_method, &lc_userdata, _rqt_pt);
	bool ret_true = true;
	if (lc_method == "kernel.quit")
	{
		TxSysProcessPeriod::leave();
		qApp->quit();
		TxQtWebEngineView::putRespondJsonTree(&lc_rpd_pt, _rqt_pt, TxQtWebEngineView::EWRC_ok);
	}
	else if (lc_method == "kernel.get_screen_resolution")
	{
		TxQtWebEngineView::putRespondJsonTree(&lc_rpd_pt, _rqt_pt, TxQtWebEngineView::EWRC_ok);
		if (true) {
			QRect lc_screen_geometry = QApplication::desktop()->screenGeometry(this);
			if (lc_screen_geometry.width() <= 0 || lc_screen_geometry.height() <= 0)
				lc_screen_geometry = QApplication::desktop()->screenGeometry();
			lc_rpd_pt.putValue("params.screen_geometry.left", lc_screen_geometry.left());
			lc_rpd_pt.putValue("params.screen_geometry.top", lc_screen_geometry.top());
			lc_rpd_pt.putValue("params.screen_geometry.width", lc_screen_geometry.width());
			lc_rpd_pt.putValue("params.screen_geometry.height", lc_screen_geometry.height());
		}
		if (true) {
			QRect lc_available_geometry = QApplication::desktop()->availableGeometry(this);
			if (lc_available_geometry.width() <= 0 || lc_available_geometry.height() <= 0)
				lc_available_geometry = QApplication::desktop()->availableGeometry();
			lc_rpd_pt.putValue("params.available_geometry.left", lc_available_geometry.left());
			lc_rpd_pt.putValue("params.available_geometry.top", lc_available_geometry.top());
			lc_rpd_pt.putValue("params.available_geometry.width", lc_available_geometry.width());
			lc_rpd_pt.putValue("params.available_geometry.height", lc_available_geometry.height());
		}
	}
	else if (lc_method == "kernel.log")
	{
		TxBlobString lc_level = _rqt_pt.getValueString("params.level");
		TxBlobWString lc_text = _rqt_pt.getValueStringW("params.text");
		if (lc_level.equalNoCase("info") || lc_level.equalNoCase("information"))
			sysLogInfo("Javascript打印日志：{0}").args(lc_text);
		else if (lc_level.equalNoCase("warn") || lc_level.equalNoCase("warning"))
			sysLogWarn("Javascript打印日志：{0}").args(lc_text);
		else if (lc_level.equalNoCase("error") || lc_level.equalNoCase("fatal"))
			sysLogError("Javascript打印日志：{0}").args(lc_text);
		else
		{
			sysLogError("无法识别window.log的params.level参数。params.level=[{0}]").args(lc_level);
			sysLogInfo("Javascript打印日志：{0}").args(lc_text);
		}
		TxQtWebEngineView::putRespondJsonTree(&lc_rpd_pt, _rqt_pt, TxQtWebEngineView::EWRC_ok);
	}
	else if (lc_method == "kernel.web_action")
	{
		TxQtWebEngineView::EnumWebResultCode lc_result_code = TxQtWebEngineView::EWRC_ok;
		TxBlobString lc_action = _rqt_pt.getValueString("params.action");
		if (lc_action.equalNoCase("Back"))
			this->triggerWebAction(QWebEnginePage::Back);
		else if (lc_action.equalNoCase("Forward"))
			this->triggerWebAction(QWebEnginePage::Forward);
		else if (lc_action.equalNoCase("Reload"))
			this->triggerWebAction(QWebEnginePage::Reload);
		else if (lc_action.equalNoCase("Stop"))
			this->triggerWebAction(QWebEnginePage::Stop);
		else if (lc_action.equalNoCase("ReloadAndBypassCache"))
			this->triggerWebAction(QWebEnginePage::ReloadAndBypassCache);
		else if (lc_action.equalNoCase("Cut"))
			this->triggerWebAction(QWebEnginePage::Cut);
		else if (lc_action.equalNoCase("Copy"))
			this->triggerWebAction(QWebEnginePage::Copy);
		else if (lc_action.equalNoCase("Paste"))
			this->triggerWebAction(QWebEnginePage::Paste);
		else if (lc_action.equalNoCase("Undo"))
			this->triggerWebAction(QWebEnginePage::Undo);
		else if (lc_action.equalNoCase("Redo"))
			this->triggerWebAction(QWebEnginePage::Redo);
		else if (lc_action.equalNoCase("SelectAll"))
			this->triggerWebAction(QWebEnginePage::SelectAll);
		else if (lc_action.equalNoCase("PasteAndMatchStyle"))
			this->triggerWebAction(QWebEnginePage::PasteAndMatchStyle);
		else if (lc_action.equalNoCase("ExitFullScreen"))
			this->triggerWebAction(QWebEnginePage::ExitFullScreen);
		else if (lc_action.equalNoCase("RequestClose"))
			this->triggerWebAction(QWebEnginePage::RequestClose);
#if QT_VERSION >= 0x050700
		else if (lc_action.equalNoCase("Unselect"))
			this->triggerWebAction(QWebEnginePage::Unselect);
		else if (lc_action.equalNoCase("SavePage"))
			this->triggerWebAction(QWebEnginePage::SavePage);
		else if (lc_action.equalNoCase("ViewSource"))
			this->triggerWebAction(QWebEnginePage::ViewSource);
#endif
		else
		{
			sysLogError("无法识别window.web_action的params.action参数。params.action=[{0}]").args(lc_action);
			lc_result_code = TxQtWebEngineView::EWRC_invalid_parameter;
		}
		TxQtWebEngineView::putRespondJsonTree(&lc_rpd_pt, _rqt_pt, lc_result_code);
	}
	else
	{
		ret_true = false;
	}
	if (!lc_rpd_pt.getBoostTree()->empty())
		this->emitCppToJs(QString::fromStdWString(lc_rpd_pt.saveJsonString().utf8Codecvt()));
	return ret_true;
}
void TxQtWebEngineView::emitCppToJs(const QString &_text)
{
	TxQtEnvironment::assertMainThread();
	this->pQtWebChannelDataTransfer->emitCppToJs(_text);
}
#if QT_VERSION >= 0x050700
void TxQtWebEngineView::contextMenuEvent(QContextMenuEvent *_event)
{
	const QWebEngineContextMenuData& lcWebMenuData = this->pWebEnginePageWrap->contextMenuData();
	QAction* lc_act_cut = this->pWebEnginePageWrap->action(QWebEnginePage::Cut);
	QAction* lc_act_copy = this->pWebEnginePageWrap->action(QWebEnginePage::Copy);
	QAction* lc_act_unselect = this->pWebEnginePageWrap->action(QWebEnginePage::Unselect);
	QAction* lc_act_selectall = this->pWebEnginePageWrap->action(QWebEnginePage::SelectAll);
	QAction* lc_act_paste = this->pWebEnginePageWrap->action(QWebEnginePage::Paste);
	bool lc_select_text_sign = !lcWebMenuData.selectedText().isEmpty();
	lc_act_cut->setEnabled(lc_select_text_sign);
	lc_act_copy->setEnabled(lc_select_text_sign);
	lc_act_unselect->setEnabled(lc_select_text_sign);
	bool lc_clipboard_text_sign = false;
	for each(auto elem in QApplication::clipboard()->mimeData()->formats())
	{
		if (elem == QString::fromStdWString(L"text/plain"))
		{
			lc_clipboard_text_sign = true;
			break;
		}
	}
	lc_act_paste->setEnabled(lc_clipboard_text_sign);
	QList<QAction*> lc_list_act;
	if (lcWebMenuData.isContentEditable())
	{
		lc_list_act.push_back(lc_act_cut);
		lc_list_act.push_back(lc_act_copy);
		lc_list_act.push_back(lc_act_selectall);
		lc_list_act.push_back(lc_act_unselect);
		lc_list_act.push_back(lc_act_paste);
	}
	else if (lc_select_text_sign)
	{
		lc_list_act.push_back(lc_act_copy);
	}
	if (lc_list_act.size() > 0)
	{
		QMenu lc_menu(this);
		lc_menu.addActions(lc_list_act);
		lc_menu.exec(_event->globalPos());
	}
}
#endif

//JavaScript发送到C++
#if 0

//协议：窗口标题区域
{
	"method":"kernel.set_caption_geometry",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"log" : "true",
		"params" : {
		"contain":[
		{
			"left":"0",
				"top" : "0",
				"width" : "800",
				"height" : "30"
		}
		],
			"eliminate":[
		{
			"left":"600",
				"top" : "0",
				"width" : "20",
				"height" : "20"
		},
		{
			"left":"640",
			"top" : "0",
			"width" : "20",
			"height" : "20"
		}
			]
	}
}
//返回状态
{
	"method":"kernel.set_caption_geometry",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"result" : "ok"
}

//协议：设置窗口大小
{
	"method":"kernel.set_size",
		"log" : "true",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"params" : {
		"width" : "800",
			"height" : "640",
			"ratio" : "0.8"
	}
}
//[params.width,params.height]表示指定窗口尺寸
//[params.ratio]表示按分辨率比例缩放窗口尺寸，取值范围[0-1]
//[params.width,params.height]和[params.ratio]组合二选一
//返回状态
{
	"method":"kernel.set_size",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"result" : "ok"
}

//协议：设置窗口大小位置
{
	"method":"kernel.set_geometry",
		"log" : "true",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"params" : {
		"left" : "300",
			"top" : "300",
			"width" : "600",
			"height" : "400"
	}
}
//返回状态
{
	"method":"kernel.set_geometry",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"result" : "ok"
}


//协议：获取窗口大小位置
{
	"method":"kernel.get_geometry",
		"log" : "true",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA"
}
//返回状态
{
	"method":"kernel.get_geometry",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"result" : "ok",
		"params" : {
		"left" : "300",
			"top" : "300",
			"width" : "600",
			"height" : "400"
	}
}

//协议：退出程序
{
	"method":"kernel.quit",
		"log" : "true",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA"
}
//返回状态
{
	"method":"kernel.quit",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"result" : "ok"
}

//协议：设置窗口状态
{
	"method":"kernel.set_state",
		"log" : "true",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"params" : {
		"state":"close"
	}
}
//参数[state]字段可以为隐藏[hide]，显示[show]，关闭[close]，最大化[maximize]，最小化[minimize]，还原[restore]，全屏[fullscreen]
//返回状态
{
	"method":"kernel.set_state",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"result" : "ok"
}

//协议：获取窗口状态
{
	"method":"kernel.get_state",
		"log" : "true",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA"
}
//返回状态
{
	"method":"kernel.get_state",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"params" : {
		"visible":"true",
			"maximize" : "true",
			"minimize" : "true",
			"fullscreen" : "true"
	}
}

//协议：获取屏幕大小
{
	"method":"kernel.get_screen_geometry",
		"log" : "true",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA"
}
//返回状态
{
	"method":"kernel.get_screen_resolution",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"result" : "ok",
		"params" : {
		"screen_geometry":{
			"left":"0",
				"top" : "0",
				"width" : "1024",
				"height" : "768"
		},
			"available_geometry":{
				"left":"0",
					"top" : "0",
					"width" : "1024",
					"height" : "720"
			}
	}
}
//参数[params.screen_geometry]字段屏幕分辨率
//参数[params.screen_geometry]字段可用分辨率，除去任务栏

//协议：设置窗口尺寸范围
{
	"method":"kernel.set_size_range",
		"log" : "true",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"params" : {
		"min_size":{
			"width":"640",
				"height" : "480"
		},
			"max_size" : {
				"width":"640",
					"height" : "480"
			}
	}
}
//返回状态
{
	"method":"kernel.set_size_range",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"result" : "ok"
}

//协议：打印日志
{
	"method":"kernel.log",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"params" : {
		"level" : "info",
			"text" : "这里打印日志文本"
	}
}
//返回状态
{
	"method":"kernel.log",
		"userdata" : "D0261BBA-A125-4D3A-A7F8-64D7F9ADA5FA",
		"result" : "ok"
}

#endif

