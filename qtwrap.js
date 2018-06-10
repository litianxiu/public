if (typeof ($Qt) != 'object') {
    $Qt = {};
}

$Qt.Base64Encode = function (input) {
    var rv;
    rv = encodeURIComponent(input);
    rv = unescape(rv);
    rv = window.btoa(rv);
    return rv;
}
$Qt.Base64Decode = function (input) {
    var rv;
    rv = window.atob(input);
    rv = escape(rv);
    rv = decodeURIComponent(rv);
    return rv;
}

$Qt.Language = JSON.parse($Qt.Base64Decode("{0}"));
$Qt._Language_fn_ = function (_text) {
    var ret = '';
    if (typeof (_text) != 'string') {
        return ret;
    }
    if (_text.length <= 0) {
        return ret;
    }
    var lc_key_str = '';
    if (_text.charAt(0) == '[') {
        var lc_find_pos = _text.indexOf(']');
        if (lc_find_pos > 1) {
            lc_key_str = _text.substr(1, lc_find_pos - 1);
        }
    }
    if (lc_key_str.length <= 0) {
        return _text;
    }
    ret = $Qt.Lang[lc_key_str];
    if (typeof (ret) != 'string') {
        return _text;
    }
    return ret;
}
$Qt.Translate = function (_translate_class_name) {
    var lc_arryTranslateUiTextAttr = ['textContent', 'title', 'value', 'innerText'];
    if (typeof (_translate_class_name) != 'string' || _translate_class_name.length <= 0)
        _translate_class_name = 'tx_multi_language_translate';
    var lc_list_ui_node = document.getElementsByClassName(_translate_class_name);
    for (var yy = 0; yy < lc_list_ui_node.length; yy++) {
        var lc_elem_ui = lc_list_ui_node[yy];
        if (typeof (lc_elem_ui) == 'object') {
            for (var tt = 0; tt < lc_arryTranslateUiTextAttr.length; tt++) {
                var str = lc_elem_ui[lc_arryTranslateUiTextAttr[tt]];
                if (typeof (str) == 'string') {
                    var new_str = $Qt._Language_fn_(str, false);
                    if (str != new_str)
                        lc_elem_ui[lc_arryTranslateUiTextAttr[tt]] = new_str;
                }
            }
        }
    }
}

if (typedef($Qt._uniqueString_) != 'object') {
    $Qt._uniqueString_ = {
        chsSample: "",
        autoIncIntLow: 0,
        autoIncIntHigh: 0
    }
}
$Qt.UniqueString = function () {
    var res = '';
    var lc_chsSample = $Qt._uniqueString_.chsSample;
    var lc_chsSampleLen = lc_chsSample.length;
    if (lc_chsSampleLen <= 0) {
        $Qt._uniqueString_chsSample_ = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
        return $Qt.UniqueString();
    }

    var lc_val_L = $Qt._uniqueString_.autoIncIntLow;
    $Qt._uniqueString_.autoIncIntLow++;
    do {
        var lc_surp = lc_val_L % lc_chsSampleLen;
        lc_val_L = (lc_val_L - lc_surp) / lc_chsSampleLen;
        res += lc_chsSample[lc_surp];
    } while (lc_val_L > 0);

    if (res.length > 5) {
        $Qt._uniqueString_.autoIncIntLow = 0;
        $Qt._uniqueString_.autoIncIntHigh++;
        return $Qt.UniqueString();
    }

    return res + $Qt._uniqueString_.autoIncIntHigh.toString(10);
}

if (typedef($Qt._timerOutInfo_) != 'object') {
    $Qt._timerOutInfo_ = {};
}
$Qt._timerOutInfo_.timerCb=function(timer_guid) {
    var lcTimerInfo = $Qt._timerOutInfo_;
    if (typeof (lcTimerInfo[timer_guid]) == 'object') {
        lcTimerInfo[timer_guid].func(lcTimerInfo[timer_guid].args);
    }
    delete lcTimerInfo[timer_guid];
}
$Qt.TimerOut = function (_msec, _args, _func) {
    var lcTimerInfo = $Qt._timerOutInfo_;
    for (; ;) {
        var timer_guid = $Qt.UniqueString();
        if (typeof (lcTimerInfo[timer_guid]) != 'object') {
            lcTimerInfo[timer_guid] = new Object();
            lcTimerInfo[timer_guid].args = _args;
            lcTimerInfo[timer_guid].func = _func;
            setTimeout('$Qt._timerOutInfo_.timerCb(\'' + timer_guid + '\')', _msec);
            break;
        }
    }
}



