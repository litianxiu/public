if (typeof ($Qt) != 'object') {
    $Qt = {};
}

//初始化
(function () {
    if (typeof ($Qt._channel_) != 'object') {
        $Qt._channel_ = {
            transfer: {},
            MessageFunction: {},
            DirtyUserdata1: new Array(),
            DirtyUserdata2: new Array(),
            _SendToCppObject_: function (_json_obj, _fn_callback) {
                if (_fn_callback) {
                    _json_obj.userdata = $Qt.UniqueString();
                    $Qt._channel_.MessageFunction[_json_obj.userdata] = _fn_callback;
                    var len2 = $Qt._channel_.DirtyUserdata2.length;
                    $Qt._channel_.DirtyUserdata2[len2] = _json_obj.userdata;
                    if (len2 + 1 >= 8 * 1024) {
                        for (var ii = 0; ii < $Qt._channel_.DirtyUserdata1.length; ++ii) {
                            delete $Qt._channel_.MessageFunction[
                               $Qt._channel_.DirtyUserdata1[ii]];
                        }
                        $Qt._channel_.DirtyUserdata1 = $Qt._channel_.DirtyUserdata2;
                        $Qt._channel_.DirtyUserdata2 = new Array();
                    }
                }
                $Qt._channel_.transfer.js_cpp(JSON.stringify(_json_obj));
            }
        };
        new QWebChannel(qt.webChannelTransport, function (_channel) {
            $Qt._channel_.transfer = _channel.objects.QtWebChannelDataTransfer;
            $Qt._channel_.transfer.cpp_js.connect(function (_message) {
                var json_obj = JSON.parse(_message);
                if (json_obj) {
                    if (!json_obj.params) {
                        json_obj.params = {};
                    }
                    if (json_obj.userdata && typeof (json_obj.userdata) == 'string' && json_obj.userdata.length > 0) {
                        var msg_rpd = {
                            m_callback: $Qt._channel_.MessageFunction[json_obj.userdata];
                    };
                    if (typeof (msg_rpd.m_callback) == 'function') {
                        msg_rpd.__proto__ = $Qt;
                        msg_rpd.m_callback(json_obj);
                    }
                    delete $Qt._channel_.MessageFunction[json_obj.userdata];
                }
                if (typeof (window.OnQWebChannelMessage) == 'function') {
                    window.OnQWebChannelMessage(json_obj);
                }
            });
            if (typeof (window.OnQWebChannelLoadFinish) == 'function') {
                window.OnQWebChannelLoadFinish();
            }
        });
    }
})();

$Qt.SetCaptionGeometry=function (_args, _msg_cb) {
    _qwebchannelwrap_Window_SetCaptionGeometry(_args, _args.arry_contain_id, _args.arry_eliminate_id, _msg_cb);
};
$Qt.SetSize=function (_args, _msg_cb) {
    _qwebchannelwrap_Window_SetSize(_args, _args.width, _args.height, _args.ratio, _msg_cb);
};
$Qt.SetGeometry=function (_args, _msg_cb) {
    _qwebchannelwrap_Window_SetGeometry(_args, _args.left, _args.top, _args.width, _args.height, _msg_cb);
};
$Qt.GetGeometry=function (_args, _msg_cb) {
    _qwebchannelwrap_Window_GetGeometry(_args, _msg_cb);
};
$Qt.SetState=function (_args, _msg_cb) {
    _qwebchannelwrap_Window_SetState(_args, _args.state, _msg_cb);
};
$Qt.GetScreenResolution=function (_args, _msg_cb) {
    _qwebchannelwrap_Window_GetScreenResolution(_args, _msg_cb);
};
$Qt.SetSizeRange=function (_args, _msg_cb) {
    _qwebchannelwrap_Window_SetSizeRange(_args,
        _args.min_width, _args.min_height, _args.max_width, _args.max_height, _msg_cb);
};
$Qt.GetState=function (_args, _msg_cb) {
    _qwebchannelwrap_Window_GetState(_args, _msg_cb);
};
$Qt.WebAction=function (_args, _msg_cb) {
    _qwebchannelwrap_Window_WebAction(_args, _args.action, _msg_cb);
};


function _qwebchannelwrap_Window_SetCaptionGeometry(_args, _arry_contain_id, _arry_eliminate_id, _msg_cb) {
    var fn_arry_rect = function (_ui_id_arry) {
        var ret = new Array(_ui_id_arry.length);
        for (var i = 0; i < _ui_id_arry.length; ++i) {
            var lc_ui_node = document.getElementById(_ui_id_arry[i]);
            ret[i] = new Object();
            ret[i].left = lc_ui_node.offsetLeft;
            ret[i].top = lc_ui_node.offsetTop;
            ret[i].width = lc_ui_node.offsetWidth;
            ret[i].height = lc_ui_node.offsetHeight;
        }
        return ret;
    }
    var json_obj = {
        method: "window.set_caption_geometry",
        params: {}
    };
    if (_args.log)
        json_obj.log = _args.log;
    if (_arry_contain_id) {
        json_obj.params.contain = fn_arry_rect(_arry_contain_id);
    }
    if (_arry_eliminate_id) {
        json_obj.params.eliminate = fn_arry_rect(_arry_eliminate_id);
    }
    Qt._channel_.SendToCppObject(json_obj, _msg_cb);
}

function _qwebchannelwrap_Window_SetSize(_args, _width, _height, _ratio, _msg_cb) {
    var json_obj = {
        method: "window.set_size",
        params: {}
    };
    if (_args.log)
        json_obj.log = _args.log;
    if (_width && _height && _width > 0 && _height > 0) {
        json_obj.params.width = _width;
        json_obj.params.height = _height;
    } else if (_ratio && _ratio > 0) {
        json_obj.params.ratio = _ratio;
    }
    Qt._channel_.SendToCppObject(json_obj, _msg_cb);
}

function _qwebchannelwrap_Window_SetGeometry(_args, _left, _top, _width, _height, _msg_cb) {
    var json_obj = {
        method: "window.set_geometry",
        params: {
            left: _left,
            top: _top,
            width: _width,
            height: _height,
        }
    };
    if (_args.log)
        json_obj.log = _args.log;
    Qt._channel_.SendToCppObject(json_obj, _msg_cb);
}

function _qwebchannelwrap_Window_GetGeometry(_args, _msg_cb) {
    var json_obj = {
        method: "window.get_geometry"
    };
    if (_args.log)
        json_obj.log = _args.log;
    Qt._channel_.SendToCppObject(json_obj, _msg_cb);
}

function _qwebchannelwrap_Window_SetState(_args, _state, _msg_cb) {
    var json_obj = {
        method: "window.set_state",
        params: {
            state: _state
        }
    };
    if (_args.log)
        json_obj.log = _args.log;
    Qt._channel_.SendToCppObject(json_obj, _msg_cb);
}

function _qwebchannelwrap_Window_GetScreenResolution(_args, _msg_cb) {
    var json_obj = {
        method: "window.get_screen_resolution",
        log: _args.log
    };
    Qt._channel_.SendToCppObject(json_obj, _msg_cb);
}

function _qwebchannelwrap_Window_SetSizeRange(_args, _min_width, _min_height, _max_width, _max_height, _msg_cb) {
    var json_obj = {
        method: "window.set_size_range",
        params: {
            min_size: {},
            max_size: {}
        }
    };

    if (_args.log)
        json_obj.log = _args.log;

    if (_min_width && _min_width > 0)
        json_obj.params.min_size.width = _min_width;
    if (_min_height && _min_height > 0)
        json_obj.params.min_size.height = _min_height;
    if (_max_width && _max_width > 0)
        json_obj.params.max_size.width = _max_width;
    if (_max_height && _max_height > 0)
        json_obj.params.max_size.height = _max_height;

    Qt._channel_.SendToCppObject(json_obj, _msg_cb);
}

function _qwebchannelwrap_Window_GetState(_args, _msg_cb) {
    var json_obj = {
        method: "window.get_state"
    };
    if (_args.log)
        json_obj.log = _args.log;
    Qt._channel_.SendToCppObject(json_obj, _msg_cb);
}

function _qwebchannelwrap_Window_Log(_args, _level, _text, _msg_cb) {
    var json_obj = {
        method: "window.log",
        params: {
            level: _level,
            text: _text
        }
    };
    if (_args.log)
        json_obj.log = _args.log;

    if (_level == 'warn' || _level == 'warning' || _level == 'error' || _level == 'fatal')
        console.assert(false, _text);
    else console.log(_text);

    Qt._channel_.SendToCppObject(json_obj, _msg_cb);
}

function _qwebchannelwrap_Window_WebAction(_args, _action, _msg_cb) {
    var json_obj = {
        method: "window.web_action",
        params: {
            action: _action
        }
    };
    if (_args.log)
        json_obj.log = _args.log;
    Qt._channel_.SendToCppObject(json_obj, _msg_cb);
}

function _qwebchannelwrap_Window_SetStore(_args, _type, _key, _value, _msg_cb) {
    var json_obj = {
        method: "window.set_store",
        params: {
            store_type: _type,
            key: _key,
            value: _value
        }
    };
    if (_args.log)
        json_obj.log = _args.log;
    Qt._channel_.SendToCppObject(json_obj, _msg_cb);
}

function _qwebchannelwrap_Window_GetStore(_args, _type, _key, _msg_cb) {
    var json_obj = {
        method: "window.get_store",
        params: {
            store_type: _type,
            key: _key
        }
    };
    if (_args.log)
        json_obj.log = _args.log;
    Qt._channel_.SendToCppObject(json_obj, _msg_cb);
}


