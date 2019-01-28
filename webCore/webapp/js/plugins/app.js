
var UC_PATH_ = "http://uc.tmooc.cn"; 
var TTS_MYTTS_URL = "http://tts.tmooc.cn"; 


window.loginSuccessCallback.add(fn);
window.loginSuccessCallback = $.Callbacks();
 window.logoutSuccessCallback.add(fn);
window.logoutSuccessCallback = $.Callbacks();



$.ajaxSetup({
    headers: {
        'X-Requested-With': 'XMLHttpRequest'
    },
    xhrFields: {
        withCredentials: true
    },
    cache: false
});

$.fn.cusTab = function(opt) {
    var tabs_ = this.find('[data-module="tab"]');
    tabs_.on('click', function() {
        if($(this).hasClass('active')) {
            return false;
        }
        tabs_.removeClass('active');
        $(this).addClass('active');
        var target_ = $('#' + $(this).data('target'));
        target_.addClass('active');
        target_.siblings('.tab-item').removeClass('active');
        if($.type(opt) == 'object' && $.type(opt.callback) == 'function') {
            opt.callback(tabs_, target_);
        }

    });
};

function getCookie(name) {
    var arr, reg = new RegExp("(^| )" + name + "=([^;]*)(;|$)");
    if(arr = document.cookie.match(reg)) {
        return unescape(arr[2]);
    } else {
        return '';
    }
}

function get_hash(strName) {
    var strHref = window.location.hash;
    var intPos = strHref.indexOf('#');
    var strRight = strHref.substr(intPos + 1);
    var arrTmp = strRight.split('&');
    for(var i = 0; i < arrTmp.length; i++) {
        var arrTemp = arrTmp[i].split('=');
        if(arrTemp[0].toUpperCase() == strName.toUpperCase() && arrTemp[1]) {
            if(i === arrTmp.length - 1) {
                var sIndex = arrTemp[1].indexOf('#');
                if(sIndex !== -1) {
                    arrTemp[1] = arrTemp[1].substring(0, sIndex);
                }
            }
            return arrTemp[1];
        }
    }
    return '';
}

function set_hash(str) {
    if(str && typeof(str) == "string") {
        window.location.hash = '#' + str;
    }
}


function cusmot_select_fun(obj, callback) {
    var cus_sel_box = (typeof(obj) == 'string') ? $('#' + obj) : obj,
        cus_sel_list = cus_sel_box.find('ul.custom_sel_list'),
        cus_sel_val = cus_sel_box.find('input.custom_sel_val'),
        cus_sel_text = cus_sel_box.find('.custom_sel_text');
    cus_sel_val.val('');
    cus_sel_list.on('click.cusselect', 'li.option', function() {
        cus_sel_text.text($(this).text());
        cus_sel_val.val($(this).attr('ord_id'));
        if(callback && typeof(callback) == 'function') {
            callback($(this).attr('ord_id'), cus_sel_box);
        }
    });
    cus_sel_box.on('click', function() {
        if(cus_sel_list.is(':hidden')) {
            $(document).one('mousedown.order', function(e) {
                var sel_cur_box = $(e.target).closest(cus_sel_box);
                if(sel_cur_box.length > 0) {
                    return;
                }
                cus_sel_list.hide();
            });
            cus_sel_list.show();
        } else {
            $(document).off('mousedown.order');
            cus_sel_list.hide();
        }
    });
}

$(document).on('click', '.js-closewin-btn', function() {
    $.colorbox && $.colorbox.close();
});
$("#js_loginout").click(function() {
    logout_fn();
    return false;
});

function logout_fn() { 
    $.get(UC_PATH_ + '/user/exitLogin', function(res) {
        if(res.code == 1) {
            if(window.IS_UC_PAGE_) { 
                location.href = UC_PATH_ + '/login/jumpTologin';
                getUserLoginStatus();
            } else {
                getUserLoginStatus(); 
                window.logoutSuccessCallback.fire(res);
            }
        } else {
            layer.msg(res.msg);
        }
    });
    return false;
}


function checkType(str, type, obj, form) {
    switch(type) {
        case 'required':
            return(str != '');
        case 'mail':
            return /^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$/.test(str);
        case 'phone':
            return /^((?!1{11})1\d{10}|09\d{8})$/.test(str);
        case 'mail_phone':
            return /^(\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*|(?!1{11})1\d{10}|09\d{8})$/.test(str);
        case 'code':
            return(function() {
                var num = obj.attr('cuslength') || 1;
                var str_ = '^[0-9]{' + num + '}$';
                var reg = new RegExp(str_);
                return reg.test(str);
            })();
        case 'code_en':
            return(function() {
                var num = obj.attr('cuslength') || 1;
                var str_ = '^[0-9a-zA-Z]{' + num + '}$';
                var reg = new RegExp(str_);
                return reg.test(str);
            })();
        case 'chinese':
            return /^[\u4E00-\u9FA5]+$/.test(str);
        case 'ch_and_en':
            return /^[a-zA-Z\u4E00-\u9FA5]+$/.test(str);
        case 'nickname':
            return(function() {
                var num = obj.attr('cuslength') || '4,12';
                var str_ = '^[[0-9a-zA-Z_\u4E00-\u9FA5]{' + num + '}$';
                var reg = new RegExp(str_);
                return reg.test(str);
            })();
       
        case 'qq':
            /*QQ*/
            return /^[0-9]{5,11}$/.test(str);
        case 'pwd':
            return /^[0-9a-zA-Z_]{6,18}$/.test(str);
        case 'equalTo':
            return(function() {
                var tar = obj.attr('equalTo');
                return(str === $.trim(form.find('#' + tar).val()));
            })();
        case 'specialchars':
            return !/[\-\_\,\!\|\~\`\(\)\#\$\%\^\&\*\{\}\:\;\"\<\>\?]/g.test(str);
        default:
            return true;
    }
}

function validateForm(form_id, opt) {
    var form_ = $('#' + form_id);
    if(!form_.length) {
        return false;
    }
    var fnObj = $.extend(true, {
        checkform: function(obj, val, type) {
            if(type == 'select' || type == 'radio_checkbox') {
                obj.off('change.cusrule').on('change.cusrule', function() {
                    fnObj.errtips($(this), $.trim(this.value), $(this).attr('cusrule'));
                });
            } else {
                obj.off('blur.cusrule').on('blur.cusrule', function() {
                    fnObj.errtips($(this), $.trim(this.value), $(this).attr('cusrule'));
                });
            }
            return this.errtips(obj, val, type);
        },
        form: function() {
            var param = true;
            form_.find('input[cusrule], select[cusrule], textarea[cusrule], div[cusrule]').each(function(i, n) {
                var obj = $(n);
                var cur = fnObj.checkform(obj, $.trim(this.value), obj.attr('cusrule'));
                param = (param && cur);
            });
            return param;
        },
        errtips: function(obj, val, type) {
            var res = checkType(val, type, obj, form_);
            !res ? obj.closest('.form-inp-area').addClass('error') : obj.closest('.form-inp-area').removeClass('error');
            return res;
        }
    }, opt);

    return fnObj;
}

$('#login_xxw').click(function() {
    if(window.IS_UC_PAGE_) { 
        location.href = UC_PATH_ + '/login/jumpTologin';
    } else {
        toLoginWinFn();
    }
    return false;
});

function toLoginWinFn() {
    $.colorbox({
        speed: 0,
        inline: true,
        overlayClose: false,
        close: false,
        href: "#login_boxxxw",
        onOpen: function() {
            $('#js_login_form').find('.error').removeClass('error');
            $('.js-md-180417-login-cj').find('.js-log-type:first').show()
                .siblings('.js-log-type').hide();
            $('#js_account_pm, #js_password, #js_imgcode').val('');
            $('#js_submit_login').data('isclick', false).html('鐧诲綍');
        }
    });
    refreshValidateImg(); 
}

$('#js_init_dropmenu').mouseenter(function() {
    $(this).find('.drop-menu-list').removeAttr('style').stop().slideDown(250);
}).mouseleave(function() {
    $(this).find('.drop-menu-list').stop().slideUp(250);
});

$(function() {
    /*if($('.md2018041201lty').length != 0) {
 *         js_init_sign();
 *             }*/
    $("#js_sign_up").on('mouseenter', function() {
        $.colorbox({
            speed: 0,
            inline: true,
            overlayClose: false,
            close: false,
            href: "#sign_uplty",
            onOpen: function() {
                reset_form();

                refreshValidateImg('js_validate_img_lty');
            },
            onComplete: function() {
                js_init_sign();
            }
        });
    });
    function reset_form() {
        var obj_ = $("#sign-formlty");
        obj_[0].reset();
        obj_.find('.custom_sel_text').html('璇烽€夋嫨');
        obj_.find('.custom_sel_val').val('');
        obj_.find('.form-inp-area').removeClass('error');
    }
    $('#js_comback').click(function() {
        $('body,html').animate({
            scrollTop: 0
        }, 500);
    });
    function js_init_sign() {
        if($("#sign_uplty").data('is_jsinited')) {
            return false;
        }
        $("#sign_uplty").data('is_jsinited', true);
        function limitLength(obj, num) {
            var val_input = obj.val(),
                val_length = val_input.length;
            $("#js_total_lty").text(num - Number(val_length));
            if(val_length > num) {
                obj.val(val_input.substring(0, num));
                $("#js_total_lty").text('0');
            } else {
                return;
            }
        }
        $("#remark").keyup(function() {
            limitLength($(this), 255);
        });
        function js_struc_data(d) {
            var html_ = "";
            $.each(d, function(i) {
                html_ += '<li class="option" ord_id="' + d[i].id + '">' + d[i].name + '</li>';
            })
            return html_;
        };

        $.ajax({
            type: "post",
            url: UC_PATH_ + "/tmoocCategory/queryTop",
            dataType: "json",
            data: {},
            success: function(d) {
                if(d.list != "") {
                    $("#js_subject").find('.custom_sel_list').html(js_struc_data(d));
                }
            }
        });
        cusmot_select_fun($("#js_course"), function(id, par_obj) {
            par_obj.find('.custom_sel_val').trigger('blur.cusrule'); 
        });
        cusmot_select_fun($("#js_subject"), function(id, par_obj) {
            par_obj.find('.custom_sel_val').trigger('blur.cusrule');
            $("#js_course").find('.custom_sel_text').html('璇烽€夋嫨');
            $("#js_course").find('.custom_sel_val').val('');
            var pid;
            var pid = $("#subject").val();
            $.ajax({
                type: "post",
                url: UC_PATH_ + "/tmoocCategory/query",
                dataType: "json",
                data: {
                    parentId: pid
                },
                success: function(d) {
                    if(d.list != "") {
                        $("#js_course").find('.custom_sel_list').html(js_struc_data(d));
                    }
                }
            });
        });
        $.ajax({
            type: "post",
            url: UC_PATH_ + "/applicationForm/findProvince",
            dataType: "json",
            data: {},
            success: function(d) {
                if(d.list != '') {
                    $("#js_province").find('.custom_sel_list').html(js_struc_data(d.list));
                }
            }

        });
        cusmot_select_fun($("#js_city"), function(id, par_obj) {
            par_obj.find('.custom_sel_val').trigger('blur.cusrule'); 
        });
        cusmot_select_fun($("#js_province"), function(id, par_obj) {
            par_obj.find('.custom_sel_val').trigger('blur.cusrule'); /
            $("#js_city").find('.custom_sel_text').html('璇烽€夋嫨');
            $("#js_city").find('.custom_sel_val').val('');
            var pid = $.trim($('#province').val());
            $.ajax({
                type: "post",
                url: UC_PATH_ + "/applicationForm/findCityInProvince",
                dataType: "json",
                data: {
                    provinceId: pid
                },
                success: function(d) {
                    $("#js_city").find('.custom_sel_list').html(js_struc_data(d.list));

                }
            });
        });
        $("#js_getPhoneCode").click(function(){
            var t_=$(this);
            var phone_=$.trim($("#telephone").val());
            var img_code=$.trim($("#js_imgCode_lty").val());
            var v_phone=validateForm('sign-formlty').checkform($("#telephone"),phone_,'phone');
            var v_img_code=validateForm('sign-formlty').checkform($("#js_imgCode_lty"),img_code,'code_en');
            if(v_phone&&v_img_code){
                sendValidateCode({
                   obj:t_,
                   url:UC_PATH_+"/user/sendPhoneCode",
                   data:{
                       phone:phone_,
                       imgCode:img_code
                   }
                });
            }
        });
        
        $('#sign_uplty .js-validate-img-lty').click(function() {
            var $img = $(this).closest('.form-inp-area').find('img');
            refreshValidateImg($img.attr('id'));
        });


        function save() {
            var telephone = $.trim($('#telephone').val());
            var qq = $.trim($('#qq').val());
            var name = $.trim($('#name').val());
            var cityId = $.trim($("#city").val());
            var cityName = $.trim($('#js_city .custom_sel_text').text());
            var categoryId = $.trim($("#course").val());
            var categoryName = $.trim($('#js_course .custom_sel_text').text());
            var remark = $.trim($('#remark').val());
            var imgCode=$.trim($("#js_imgCode_lty").val());
            var phoneCode=$.trim($("#js_phoneCode_lty").val());
            if($("#sign_up_source").length!=0){
                var chanel=$.trim($("#sign_up_source").val());
            }else{
                var chanel='10331002';
            }
            var that_ = $("#js_sign_btn");
            that_.prop("disabled", true);
            $.ajax({
                type: "post",
                url: UC_PATH_ + "/applicationForm/save",
                dataType: "json",
                data: {
                    telephone: telephone,
                    qq: qq,
                    name: name,
                    cityId: cityId,
                    cityName: cityName,
                    categoryId: categoryId,
                    categoryName: categoryName,
                    remark: remark,
                    validateCode:phoneCode,
                    imgCode:imgCode,
                    signUpSource:chanel
                },
                success: function(d) {
                    if(d.code == '1') {
                        layer.msg('浜茬埍鐨勭敤鎴凤紝鎮ㄥ凡缁忕敵璇锋垚鍔燂紒');
                        $.colorbox.close();
                    }else if(d.code == '-1'){
                        layer.msg('涓氬姟寮傚父');
                    }else{
                      
                        refreshValidateImg('js_validate_img_lty');
                        layer.msg(d.msg);
                    }
                    that_.prop("disabled", false);
                },
                error: function() {
                    that_.prop("disabled", false);
                }
            });
        }

        $("#js_sign_btn").click(function() {
            if(validateForm('sign-formlty').form()) {
                save();
            };
        });
    }

});

function requestParam(strName) {
    var strHref = location.search;
    var intPos = strHref.indexOf('?');
    if(intPos === -1) {
        return '';
    }
    var strRight = strHref.substr(intPos + 1);
    var arrTmp = strRight.split('&');
    for(var i = 0; i < arrTmp.length; i++) {
        var arrTemp = arrTmp[i].split('=');
        if(arrTemp[0].toUpperCase() == strName.toUpperCase()) {
            if(i === arrTmp.length - 1) {
                var sIndex = arrTemp[1].indexOf('#');
                if(sIndex !== -1) {
                    arrTemp[1] = arrTemp[1].substring(0, sIndex);
                }
            }
            return arrTemp[1];
        }
    }
    return '';
}


function refreshValidateImg(ele) {
    $('#' + (ele || 'js_validate_img')).attr("src", UC_PATH_ + "/validateCode?t=" + Math.random());
}

function sendValidateCode(opt) {
    if($.type(opt) != 'object' || !opt.obj.length) {
        return false;
    }
    var $obj = opt.obj;
    if(!$obj.data('issend')) {
        var second = opt.time || 59;
        var s_time = setInterval(function() {
            if(second <= 1) {
                $obj.prop('disabled', false).data('issend', false).text(opt.txt || '鑾峰彇鍔ㄦ€佺爜');
                clearInterval(s_time);
                return false;
            } else {
                $obj.text('锛�' + second-- + 's锛�');
            }
        }, 1000);

        $.post(opt.url, opt.data, function(res) {
            if(res.code != 1) {
                $obj.prop('disabled', false).data('issend', false).text(opt.txt || '鑾峰彇鍔ㄦ€佺爜');
                clearInterval(s_time);
                layer.msg(res.msg);
            }
            opt.callback && opt.callback(res);
        });
        $obj.prop('disabled', true).data('issend', true);
    }
}

function initLoginFn() {
 
    $('.js-validate-img').click(function() {
        refreshValidateImg();
    });

    $('.js-md-180417-login-cj').on('click', '.js-log-type-cut', function() {
        var p_ = $(this).closest('.js-log-type');
        p_.hide().siblings('.js-log-type').show();

        var sib_ = p_.siblings('.js-log-type');
        if(sib_.data('logtype') == 3) {
            var state;
            if(window.IS_UC_PAGE_) {
                state = window.IS_UC_PAGE_;
            } else {
                var local = location.href;
                state = encodeURIComponent(encodeURIComponent(local));
            }
            if(window.WxLogin){
                new WxLogin({
                    self_redirect: false,
                    id: "js_wx_code_log",
                    appid: "wx3478ef3576df5109",
                    scope: "snsapi_login",
                    redirect_uri: encodeURIComponent("http://www.tmooc.cn/touc" + "/login/wxLogin"),
                    state: state,
                    style: "black",
                    href: ""
                });
            }else{
                $('#js_wx_code_log').html('<p style="font-size: 16px; padding-top: 50px;">寰俊鍒濆鍖栧け璐ワ紝璇烽噸璇曪紒</p>');
            }

        }
        setTimeout(function() {
            $.colorbox && $.colorbox.resize();
        }, 10);
    });
    $('#js_submit_login').click(function() {
        if(validateForm('js_login_form').form()) {
            var t_ = $(this);

            var loginNmae = $.trim($('#js_account_pm').val());
            var password = $.trim($('#js_password').val());
            var imgcode = $.trim($('#js_imgcode').val());
            var log_type; 
            var flag_ = true;
            if(checkType(loginNmae, 'mail')) {
                log_type = 1;
            } else if(checkType(loginNmae, 'phone')) {
                log_type = 0;
            }

            if(!t_.data('isclick')) {
                t_.data('isclick', true).html('鐧诲綍涓�...');

                $.post(UC_PATH_ + "/login/loginTimes", {
                    loginName: loginNmae,
                    accountType: log_type
                }, function(res) {
                    if(res.code == 1) {
                        if(res.obj >= 3) {
                            if(!$('#js_validate_img_area').is(':visible')) {
                                $('#js_validate_img_area').show().find('#js_imgcode').attr('cusrule', 'code_en').attr('cuslength', 4);
                                $.colorbox.resize();
                                flag_ = false;
                                refreshValidateImg();
                            }
                            t_.data('isclick', false).html('鐧诲綍');
                        }
                        if(flag_) {
                     
                            $.post(UC_PATH_ + "/login", {
                                loginName: loginNmae,
                                password: MD5(password),
                                imgCode: imgcode,
                                accountType: log_type
                            }, function(res) {
                         
                            
                                if(res.code == 1 || res.code == 2) {
                                    if(window.IS_UC_PAGE_) { 
                                        location.href = UC_PATH_ + "/userCenter/toUserCenterPage";
                                        return false;
                                    } else { 
                                        layer.msg('鐧诲綍鎴愬姛');
                                        getUserLoginStatus(); 
                                        window.loginSuccessCallback.fire(res); 
                                    }
                                    if(res.code == 2) {

                                        $.colorbox({
                                            speed: 0,
                                            inline: true,
                                            overlayClose: false,
                                            close: false,
                                            href: "#js_yz_video"
                                        });
                                    } else {
                                        $.colorbox && $.colorbox.close();
                                    }
                                } else if(res.code == -8009) {

                                    $('#js_account_pm').before('<a href="http://uc.tmooc.cn/login/jumpToActiveMailobx" style="position: absolute;top: -17px;font-size: 12px;">璇ヨ处鍙锋湭婵€娲伙紝<span style="color:red">鐐瑰嚮鍘绘縺娲�</span></a>');
                                } else if(res.code == -9001) {

                                    layer.msg(res.msg);
                                } else {
                                    layer.msg(res.msg);
                                    refreshValidateImg();
                                }
                                t_.data('isclick', false).html('鐧诲綍');

                            });
                        }
                    } else {
                        t_.data('isclick', false).html('鐧诲綍');
                        layer.msg(res.msg);
                    }
                });
            }

        }
    });
    $('#js_password, #js_imgcode').keypress(function(e) {
        if(e.keyCode === 13) {
            $('#js_submit_login').trigger('click');
        }
    });
}

function initRegisterFn() {
    refreshValidateImg();
    $('.js-validate-img').click(function() {
        var $img = $(this).closest('.form-inp-area').find('img');
        refreshValidateImg($img.attr('id'));
    });

    $('#md_2018040401_ll').cusTab({
        callback: function(tabs_, target_) {
            if(target_.attr('id') == 'md_2018040401_ll_tab2') {
                refreshValidateImg('js_validate_img_m');
            } else {
                refreshValidateImg();
            }
        }
    });

    function register(obj, type, loginName, password, nickName, validateCode) {
        if(!obj.data('issend')) {
            $.post(UC_PATH_ + "/user/registerAccount", {
                loginName: loginName,
                password: MD5(password),
                nickName: nickName,
                validateCode: validateCode,
                accountType: type
            }, function(res) {
                if(res.code != 1) {
                    obj.data('issend', false);
                    refreshValidateImg((type == 1 && 'js_validate_img_m'));
                    layer.msg(res.msg);
                } else {
                    if(window.IS_UC_PAGE_) { 
                        layer.msg('娉ㄥ唽鎴愬姛,3s鍚庤烦杞櫥褰曢〉');
                        setTimeout(function() {
                            location.href = UC_PATH_ + '/login/jumpTologin';
                        }, 3000);
                    } else {
                        $.colorbox && $.colorbox.close();
                        getUserLoginStatus(); 
                    }
                }
                window.registerSuccessCallback && window.registerSuccessCallback(res);
            });
            obj.data('issend', true);
        }
    }

    $('#js_DynamicCodePhone').click(function() {
        var t_ = $(this);
        var reg_account = $.trim($('#js_account_phone').val());
        var img_code = $.trim($('#js_imgcode_phone').val());

        var v_account = validateForm('js_reg_phone_form').checkform($('#js_account_phone'), reg_account, 'phone');
        var v_imgCode = validateForm('js_reg_phone_form').checkform($('#js_imgcode_phone'), img_code, 'code_en');

        if(v_account && v_imgCode) {
            sendValidateCode({
                obj: t_,
                url: UC_PATH_ + "/user/sendPhoneCode",
                data: {
                    phone: reg_account,
                    imgCode: img_code
                }
            });
        }
    });

    $('#js_DynamicCodeEmail').click(function() {
        var t_ = $(this);
        var reg_account = $.trim($('#js_account_mail').val());
        var img_code = $.trim($('#js_imgcode_mail').val());

        var v_account = validateForm('js_reg_mail_form').checkform($('#js_account_mail'), reg_account, 'mail');
        var v_imgCode = validateForm('js_reg_mail_form').checkform($('#js_imgcode_mail'), img_code, 'code_en');

        if(v_account && v_imgCode) {
            sendValidateCode({
                obj: t_,
                url: UC_PATH_ + "/user/sendEmail",
                data: {
                    email: reg_account,
                    imgCode: img_code
                }
            });
        }
    });

    $('#js_submit_reg_phone').click(function() {
        if(!$('#js_form_user_regyx').is(':checked')){
            layer.msg('璇烽槄璇诲苟鍕鹃€夊悓鎰忕敤鎴锋敞鍐屽崗璁紒');
            return false;
        }
        if(validateForm('js_reg_phone_form').form()) {
            var account = $.trim($('#js_account_phone').val());
            var pwd = $.trim($('#js_pwd_phone2').val());
            var img_code = $.trim($('#js_imgcode_phone').val());
            var dy_code = $.trim($('#js_dy_code_phone').val());

            register($(this), 0, account, pwd, '', dy_code);
        }
    });

    $('#js_submit_reg_mail').click(function() {
        if(!$('#js_form_user_regyx').is(':checked')){
            layer.msg('璇烽槄璇诲苟鍕鹃€夊悓鎰忕敤鎴锋敞鍐屽崗璁紒');
            return false;
        }
        if(validateForm('js_reg_mail_form').form()) {
            var account = $.trim($('#js_account_mail').val());
            var name = $.trim($('#js_name_mail').val());
            var pwd = $.trim($('#js_pwd_mail2').val());
            var img_code = $.trim($('#js_imgcode_mail').val());
            var dy_code = $.trim($('#js_dy_code_mail').val());

            register($(this), 1, account, pwd, name, dy_code);
        }
    });
    $('#js_dy_code_phone').keypress(function(e) {
        if(e.keyCode === 13) {
            $('#js_submit_reg_phone').trigger('click');
        }
    });
    $('#js_dy_code_mail').keypress(function(e) {
        if(e.keyCode === 13) {
            $('#js_submit_reg_mail').trigger('click');
        }
    });
}

$('#js_go_ynote').click(function(event) {
    window.open("http://inote.tmooc.cn" + "/login/loginnow?sessionId=" + getCookie("TMOOC-SESSION"));
    return false;
});

function getUserLoginStatus() { 
    var login_stateObj = $('#login_statelty');
    if(login_stateObj.length == 0) {
        return false;
    }
    $.getJSON(UC_PATH_ + '/userValidate/getUserInfo', function(d) {
        if(d && d.code == '1') { 
            login_stateObj.find('.logined').show();
            login_stateObj.find('.no-login').hide();
            $("#tobbar_username").text(d.obj.nickName).attr('href', UC_PATH_ + "/userCenter/toUserCenterPage")
                .attr('target', function() {
                    if(window.IS_UC_PAGE_) { 
                        return '_self';
                    }
                    return '_blank';
                });
            if(d.obj.pictureUrl) {
                $("#tobbar_userimg").attr('src', d.obj.pictureUrl);
            } else {
                $("#tobbar_userimg").attr('src', "http://cdn.tmooc.cn/tmooc-web/css/img/user-head.jpg");
            }
            $("#tobbar_userimg").parent('a').attr('href', UC_PATH_ + "/userCenter/toUserCenterPage")
                .attr('target', function() {
                    if(window.IS_UC_PAGE_) { 
                        return '_self';
                    }
                    return '_blank';
                });

            if(d.obj.userChannel && d.obj.userChannel == '10061007') {
                login_stateObj.find('#js_isshow_tts').show();
            }else{
                login_stateObj.find('#js_isshow_tts').hide();
            }
           check_old_sp_user();
        } else {
            login_stateObj.find('.logined').hide();
            login_stateObj.find('.no-login').show();
            $("#tobbar_username").text('');
            $("#tobbar_userimg").attr('src', "http://cdn.tmooc.cn/tmooc-web/css/img/user-head.jpg");
        }
    });
}

function check_old_sp_user() { 
    var flag_ = false;
    $.getJSON(UC_PATH_ + '/userCenter/getUserBuy', function(d) {
        if(d && d.code == '1' && d.obj.status == '0') { 
            layer.confirm('<div style="text-indent: 28px;">鐢变簬TMOOC缃戠珯鍗囩骇鏀圭増锛屽嚭浜庤绋嬫湇鍔′綋楠屾晥鏋滄彁鍗囷紝閮ㄥ垎鏃х珯璇剧▼鏈仛杩佺Щ锛堝寘鎷儴鍒嗕粯璐硅绋嬶級銆傞拡瀵逛粯璐硅绋嬬殑宸茶喘瀛﹀憳锛屼负寮ヨˉ鎮ㄧ殑璐拱鎹熷け锛孴MOOC浠ユ偍鍦ㄦ棫绔欐墍璐绋嬪弻鍊嶄环鍊肩殑浼氬憳鏃堕暱浣滀负琛ュ伩锛屾偍鍦ㄧ偣鍑诲厬鎹㈠悗鍗冲彲鑾峰緱鐩稿簲鐨勪細鍛樻椂闀裤€�<br> <p style="text-indent: 28px">鎰熻阿鎮ㄧ殑鐞嗚В涓庢敮鎸侊紝绁濇偍鍦═MOOC瀛︿範鎰夊揩锛�</p> <br> <p class="text-right">TMOOC 2018骞�6鏈�12鏃�</p></div>', {
                btn: ['宸插悓鎰忓苟纭鍏戞崲', '鍙栨秷'],
                area:['500px','auto']
            }, function(index) {
                if(!flag_){
                    flag_ = true;
                    $.getJSON(UC_PATH_ + '/userCenter/updateUserBuy', function(dd) {
                        if(dd && dd.code =='1'){
                            layer.msg('鍏戞崲鎴愬姛锛�');
                            layer.close(index);
                        }else{
                            layer.msg('鍏戞崲澶辫触锛佽鑱旂郴瀹㈡湇锛�');
                        }
                        flag_ = false;
                    });
                }
            });
        }
    });
}

function openPayStatusFn() { 
    $.colorbox({
        speed: 0,
        inline: true,
        overlayClose: false,
        close: false,
        href: "#md_2018040423_ll"
    });
}

function checkTtsUser() {
    $.ajax({
        type: "post",
        url: UC_PATH_ + "/user/checkTtsUser",
        dataType: "json",
        async: false,
        success: function(data) {
            if(data && data.code == 1) {
               
                window.open(TTS_MYTTS_URL + "/user/myTTS?sessionId=" + encodeURI(data.obj) + "&date=" + (encodeURI(new Date())));
            } else {
          
                window.location.href = 'http://www.tmooc.cn/ttspages/online-class/web/TMOOC/index.shtml';
            }
        }
    });
    return false;
}


$(function() {
    var url_ = window.location.pathname.replace('index.shtml', '');
    $.each($("#js_slide_nav a"), function() {
        var href_ = $(this).attr("href");
        if(href_) {
            href_ = /www.tmooc.cn(\/(\w+?\/)?)?/.exec(href_);
            if(href_ && href_[1] && href_[1] == url_) {
                $(this).parent().addClass('active');
                return false;
            }
        }
    });
});



