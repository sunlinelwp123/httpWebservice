var Hello = function() {

	var _basePath = '.';
	var _errFunc = function(data) {
		var options = {
			type : "danger",
			message : "ajax通讯异常"
		};
		console.log(options);
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
	var _ajaxHello = function(url, data, type, successfn, errorfn, dataType,
			async) {
		console.log("是否同步"+async);
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
		console.log("发送请求数据========" +  JSON.stringify(data));
		console.log("发送请求类型========" + type);
		console.log("发送请求数据类型========" + dataType);
		console.log("发送请求数类型========" + async+(async?"异步":"同步"));

		jQuery.ajax({
			type : type,
			contentType : _contentType,
			async : async,
			data : JSON.stringify(data),
			url : url,
			dataType : dataType,
			success : function(d) {
				successfn(d);
			},
			error : function(e) {
				errorfn(e);
			}
		});
	};
	var _ajaxPath = function(url, node) {
		var params = url.split("/");
		if(params.length <= 0){
			var options = {
				type : "danger",
				message : "url格式错误"
			};
		}
	    url = _basePath + "/rest/" + url;	
		//url = url + "?ran=" + Math.random();
		return url;
	};
	
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
		ajaxHello : function(url, data, type, successfn, errorfn, dataType,async) {
				_ajaxHello(_ajaxPath(url), data, type, successfn, errorfn, dataType, async);
		}
	}
}
();
	
	
	
	
	
	
	
