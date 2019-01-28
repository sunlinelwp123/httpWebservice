var Sunline = function() {

	var _basePath = '';
	var _isDemo = false;

	var _errFunc = function(data) {
		var options = {
			type : "danger",
			message : "ajax通讯异常"
		};
		Metronic.alert(options);
	};
 
	/**
	 * 判断变量是否为空
	 * 
	 */
	var _isNull = function(param) {
		if (param == null || param == "" || typeof (param) == "undefined") {
			return true;
		} else {
			return false;
		}
	};

	/**
	 * ajax封装
	 * 
	 * url 表示请求资源 data 发送到服务器的数据，数组存储，如：{"date": new Date().getTime(), "state":
	 * 1} type POST-add,PUT-update,GET-query,DELTE,按照Restful标准,默认get successfn
	 * 成功回调函数 errorfn 失败回调函数 dataType
	 * 预期服务器返回的数据类型，常用的如：xml、html、json、text,默认为json async 默认值:
	 * true。默认设置下，所有请求均为异步请求。如果需要发送同步请求，请将此选项设置为 false。
	 * 注意，同步请求将锁住浏览器，用户其它操作必须等待请求完成才可以执行。
	 * 
	 */
	var _ajaxSun = function(url, data, type, successfn, errorfn, dataType,
			async) {
		console.log("shifoutongbu"+async);
		async = _isNull(async+"")? true : async;
		
		type = _isNull(type) ? "get" : type;
		dataType = _isNull(dataType) ? "json" : dataType;
		errorfn = _isNull(errorfn) ? _errFunc : errorfn;
		var _contentType = "application/json";
		if (dataType == "json") {
			_contentType = "application/json";
		} else if (dataType == "xml") {
			_contentType = "application/xml";
		} else if (dataType == "html") {
			_contentType = "text/html";
		} else if (dataType == "pdf") {
			_contentType = "application/pdf";
		} else if (dataType == "xsl") {
			_contentType = "application/vnd.ms-excel";
		}

		console.log("发送请求URL========" + url);
		console.log("发送请求数据========" + $.toJSON(data));
		console.log("发送请求类型========" + type);
		console.log("发送请求数据类型========" + dataType);
		console.log("发送请求数类型========" + async+(async?"异步":"同步"));

		jQuery.ajax({
			type : type,
			contentType : _contentType,
			async : async,
			data : $.toJSON(data),
			url : url,
			dataType : dataType,
			success : function(d) {
				// 拦截6006错误码：需要授权复核功能
				if(d!=null && d!=undefined && d.retCode=='6006'){
//					alert('需要复合');
					if(d.retMsg.substr(0,12)=='[Perm.E0016]'){
						bootbox.confirm("当前柜员对本交易操作权限不够,需要授权是否需要进行授权?", function (result) {  
							if(result) {  
								$('button[data-bb-handler="ok"]',$(".modal-dialog")).trigger("click");//隐藏原弹出窗。
								$("#index_checkModal").modal("show");
								//人员复核弹出窗 确认处理
								$("#check_save",$("#index_checkModal")).unbind("click");//解绑原事件
								$("#check_save",$("#index_checkModal")).bind('click',function(){
//									e.preventDefault();
									var checkData={};
									$.each($("input",$("#check_form")),function(i,n){
										if (n.name != undefined && n.name != "" && n.name != null) {
											checkData[n.name] = n.value;
										}
									});
									//验证复核密码
									Sunline.ajaxRouter("recheck/uschck", checkData, "POST", 
											function(ret){
										if(ret.retCode == "0000"){
											$.each(checkData,function(i,n){
												data[i]=n;
											});
											_ajaxSun(url, data, type, successfn, errorfn, dataType,async);
											$('.alert-success',$("#index_checkModal")).hide();
											$('.alert-danger',$("#index_checkModal")).hide();	
											$('.msg', $("#index_checkModal")).text("");
											$('input[name="authus"]', $("#index_checkModal")).val("");
											$('input[name="authpw1"]', $("#index_checkModal")).val("");
											$("#index_checkModal").modal("hide");
										}else{
											$('.msg', $("#index_checkModal")).text(ret.retMsg);
											$('.alert-success',$("#index_checkModal")).hide();
											$('.alert-danger',$("#index_checkModal")).show();	
										}
									}, 
									function(ret){
										$('.msg', $("#index_checkModal")).text("复核验证请求异常");
										$('.alert-success',$("#index_checkModal")).hide();
										$('.alert-danger',$("#index_checkModal")).show();			
									}, "json",true);
								});	
								
							}
							else{
								$('input[name="authus"]', $("#index_checkModal")).val("");
								$('input[name="authpw1"]', $("#index_checkModal")).val("");
								$("#index_checkModal").modal("hide");
							}
						});  
					}else{
						bootbox.alert(d.retMsg);
					}
				}else{
					successfn(d);
				}
			},
			error : function(e) {
				errorfn(e);
			}
		});
	};
	
	
	
		
	
	/**
	 * 字典获取封装
	 * type 为dict_type
	 * path 从非字典表中获取字典型 
	 * id 为字典ID
	 * text 为字典显示值
	 * etc:Sunline.getDict("userst")   字典表中获取字典
	 * 非字典表中获取字典
	 * etc:Sunline.getDict("jijin","/prod","prodId","prodName")  
	 * 从/prod中获取字典  字典值为 prodId, 显示值为prodName ,分类为jijin
	 */
	var _getDict = function(type, path, id, text) {
		var dict=[];
		var data = {};
		var dictName="dictName";
		var dictId="dictId";
		var dictUrl= _basePath+"/rest/dict";
		if(path!=null){
			dictUrl+=path;
			dictName=text;
			dictId=id;
		}
		dictUrl+="?ran=" + Math.random();
		data.dictType = type;
		jQuery.ajax({
			type : "post",
			contentType : "application/json",
			async : false,
			data : $.toJSON(data),
			url :dictUrl,
			dataType : "json",
			success : function(d) {
		      dict=d;						
			},
			error : function(e) {
			}
		});
		$.each(dict, function(i, n){
             n.text=n[dictName]+"["+n[dictId]+"]";
             n.id=n[dictId];            
             });		
		return dict;
	};

	var _ajaxPath = function(url, node) {
		var params = url.split("/");
		if(params.length <= 0){
			var options = {
				type : "danger",
				message : "url格式错误"
			};
			Metronic.alert(options);
		}
		
		if (_isDemo) {
			var func = params[params.length - 1];
			var module = params[params.length - 2];
			
			var prePath = _basePath + "/pages/" + module + "/data/";
			if (_isNull(node)) {
				url = prePath + func + ".json";
			} else {
				if (node.id === '#') {
					url = prePath + func + "Root.json";
				} else {
					url = prePath + func + "Child.json";
				}
			}
		}else{
			url = _basePath + "/rest/" + url;
		}
		url = url + "?ran=" + Math.random();
		return url;
	};
	/**
	  * 表单验证封装
      * editform 需要验证的表单
      * submitFn 表单提交方法
      * rule 字段验证规则 
	 */
	var Valid = function ( editform, submitFn, rule) {	
		var editerror = $('.alert-danger', editform);
		var editsuccess = $('.alert-success', editform);
		var validator = editform.validate({
				errorElement : 'span',
				errorClass : 'help-block help-block-error',
				focusInvalid : false,
				ignore : "",
				rules : rule,
				invalidHandler : function (event, validator) {
					editsuccess.hide();
					editerror.show();
					Metronic.scrollTo(editerror, -200);
				},
				errorPlacement : function (error, element) {
					element.parent().append(error);
				},
				highlight : function (element) {
					$(element).closest('.form-group').removeClass("has-success").addClass('has-error');
				},
				success : function (label, element) {
					var icon = $(element).parent('.input-icon').children('i');
					$(element).closest('.form-group').removeClass('has-error').addClass('has-success');
					icon.removeClass("fa-warning").addClass("fa-check");
				},
				submitHandler : function (form) {
					editerror.hide();
					editsuccess.hide();
					submitFn();
				}
			});
		return {
			resetForm : function () {
				validator.resetForm();
			},
			validator : validator,
			editform : editform
		}
	};
	/**
	 * 初始化验证器默认提示信息
	 */
	var _validator = function() {
		jQuery.extend(jQuery.validator.messages, {
			required : "此项为必填字段，请输入内容！",
			remote : "您输入的内容已存在，请重新输入！",
			email : "请输入正确格式的电子邮件",
			url : "请输入合法的网址",
			date : "请输入合法的日期",
			dateISO : "请输入合法的日期 (ISO).",
			number : "请输入合法的数字",
			digits : "只能输入整数",
			creditcard : "请输入合法的信用卡号",
			equalTo : "请再次输入相同的值",
			accept : "请输入拥有合法后缀名的字符串",
			maxlength : jQuery.validator.format("请输入一个 长度最多是 {0} 的字符串"),
			minlength : jQuery.validator.format("请输入一个 长度最少是 {0} 的字符串"),
			rangelength : jQuery.validator
					.format("请输入 一个长度介于 {0} 和 {1} 之间的字符串"),
			range : jQuery.validator.format("请输入一个介于 {0} 和 {1} 之间的值"),
			max : jQuery.validator.format("请输入一个最大为{0} 的值"),
			min : jQuery.validator.format("请输入一个最小为{0} 的值")
		});
	}
	/***************************************************************************
	 * 给出两个date 算出俩个间隔时间 返回值为一个数组 第一个值为天数，第二个值为小时数，第三个值为分钟数第四个值为秒数 [12,15,23,34]
	 * date1，date2都为毫秒数
	 **************************************************************************/
	var _getIntervalBetween = function(date1, date2) {
		var interval = date2 - date1;
		var days = Math.floor(interval / 1000 / 60 / 60 / 24);
		var hours = Math
				.floor((interval - days * 3600 * 24 * 1000) / 1000 / 60 / 60);
		var minuts = Math
				.floor((interval - days * 3600 * 24 * 1000 - hours * 3600 * 1000) / 1000 / 60);
		var seconds = Math.floor((interval - days * 3600 * 24 * 1000 - hours
				* 3600 * 1000 - minuts * 60 * 1000) / 1000);
		return [ days, hours, minuts, seconds ];
	}
	var _localPath =function(url){
		return  window.location.href = _basePath + "/path/" + url;	
	}
	return {
		/**
		 * 设置基准路径
		 * 
		 */
		setBasePath : function(path) {
			_basePath = path;
		},

		/**
		 * 获取基准路径
		 * 
		 */
		getBasePath : function() {
			return _basePath;
		},

		getIsDemo : function() {
			return _isDemo;
		},

		/**
		 * 获取URL变量
		 * 
		 */
		getURLParameter : function(paramName) {
			var searchString = window.location.search.substring(1), i, val, params = searchString
					.split("&");

			for (i = 0; i < params.length; i++) {
				val = params[i].split("=");
				if (val[0] == paramName) {
					return unescape(val[1]);
				}
			}
			return null;
		},

		/**
		 * 判断变量是否为空
		 * 
		 */
		isNull : function(param) {
			return _isNull(param);
		},
		/**
		 * 增加demo模式,如果为demo模式，处理url匹配到相应的json文件数据 url 发送请求的地址
		 * 命名规则：/services/rest/模块/功能
		 * 
		 */
		ajaxRouter : function(url, data, type, successfn, errorfn, dataType,
				async) {
			console.log(_basePath);
			type = _isNull(type) ? "get" : type;
			// 判断是否为demo模式
			// 如果是demo模式处理url重指向本地json文件
			if (_isDemo) {
				// 只处理查询类的交易
				if (type.toUpperCase() === "GET") {
					_ajaxSun(_ajaxPath(url), null, "GET", successfn, errorfn,
							"json", async);
				} else {
					var options = {
						message : "操作成功",
						closeInSeconds : 2
					};
					Metronic.alert(options);
					successfn();
				}
			} else {
				_ajaxSun(_ajaxPath(url), data, type, successfn, errorfn, dataType, async);
			}
		},

		/**
		 * 处理ajax请求 提供给第三方插件ajax请求url修改
		 */
		ajaxPath : function(url, node) {
			return _ajaxPath(url, node);
		},

		/**
		 * 初始化form表单样式和控制
		 */
		initForm : function() {
			// 初始化input maxlength插件
			$("input[maxlength]").maxlength({
				alwaysShow : true,
				limitReachedClass : "label label-danger",
				placement : "centered-right"
			});		
		},
		/**
		 * 初始化验证器
		 * 
		 * options 验证器规则定义参数
		 * 
		 */
		initValidator : function() {
			_validator();
		},
		/**
		 * 获取字典型 
		 * type 为字典分类
		 * path 从非字典表中获取字典型 
		 * id 为字典ID
		 * text 为字典显示值
		 */
		getDict : function(type, path, id, text) {
			return _getDict(type, path, id, text);
		},
		/**
		 * 页面跳转window
		 * url跳转路径
		 */
		localPath:function(url){
			return _localPath(url);
		},
		  /**
	      * 表单验证封装
	      * editform 需要验证的表单	// $("#kupdppb");
	      * submitFn 表单提交方法
	      * rule 字段验证规则
	      */
		getValidate:function(editform,submitFn,rule){
			return new Valid(editform,submitFn,rule);
		}
		
	};
}();