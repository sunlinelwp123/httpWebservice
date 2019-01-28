;(function($) {
	//精度判断，即小数点后面有几位
	$.validator.addMethod(
		"precision", 
		function(value, element, param) {
			if (this.optional(element) || value == null || value == "") {
				return true;
			}
			if (parseFloat(value) == NaN) {
				return false;
			}
			if (parseFloat(param) == NaN) {
				return false;
			}
			var idx = value.lastIndexOf(".");
			if (idx < 0) {
				return true;
			}
			var cnt = value.length - idx - 1;
			if (cnt <= parseFloat(param)) {
				return true;
			}
			return false;
		}, 
		$.validator.format("小数位数不能超过{0}")
	);
	//介于两个值中间，包含边界值
	$.validator.addMethod(
		"between", 
		function(value, element, param) {
			if (parseFloat(value) == NaN) {
				return false;
			}
			if (parseFloat(param[0]) == NaN) {
				return false;
			}
			if (parseFloat(param[1]) == NaN) {
				return false;
			}
			return this.optional(element) || ( parseFloat(value) >= parseFloat(param[0]) && parseFloat(value) <= parseFloat(param[1]) );
		}, 
		$.validator.format("必须大于等于{0}小于等于{1}")
	);
	//大于等于某个值
	$.validator.addMethod(
		"gteq", 
		function(value, element, param) {
			if (parseFloat(value) == NaN) {
				return false;
			}
			if (parseFloat(param) == NaN) {
				return false;
			}
			return this.optional(element) || (parseFloat(value) >= parseFloat(param));
		}, 
		$.validator.format("必须大于等于{0}")
	);
	//大于某个值
	$.validator.addMethod(
		"gt", 
		function(value, element, param) {
			if (parseFloat(value) == NaN) {
				return false;
			}
			if (parseFloat(param) == NaN) {
				return false;
			}
			return this.optional(element) || (parseFloat(value) > parseFloat(param));
		}, 
		$.validator.format("必须大于{0}")
	);
	//小于等于某个值
	$.validator.addMethod(
		"lteq", 
		function(value, element, param) {
			if (parseFloat(value) == NaN) {
				return false;
			}
			if (parseFloat(param) == NaN) {
				return false;
			}
			return this.optional(element) || (parseFloat(value) <= parseFloat(param));
		}, 
		$.validator.format("必须小于等于{0}")
	);
	//小于某个值
	$.validator.addMethod(
		"lt", 
		function(value, element, param) {
			if (parseFloat(value) == NaN) {
				return false;
			}
			if (parseFloat(param) == NaN) {
				return false;
			}
			return this.optional(element) || (parseFloat(value) < parseFloat(param));
		}, 
		$.validator.format("必须小于{0}")
	);
	// 中文字两个字节
	$.validator.addMethod(
		"brangelength", 
		function(value, element, param) {
		    var length = $.trim(value).replace(/[^\x00-\xff]/g,"**").length;
		    return this.optional(element) || ( length >= param[0] && length <= param[1] );   
		}, 
		$.validator.format("长度介于{0}和{1}之间")
	);
	// 中文字两个字节
	$.validator.addMethod(
		"bminlength", 
		function(value, element, param) {
			var length = $.trim(value).replace(/[^\x00-\xff]/g,"**").length;
		    return this.optional(element) || ( length >= param );   
		}, 
		$.validator.format("长度最少是{0}")
	);
	// 中文字两个字节
	$.validator.addMethod(
		"bmaxlength", 
		function(value, element, param) {
			var length = $.trim(value).replace(/[^\x00-\xff]/g,"**").length;
		    return this.optional(element) || ( length <= param );   
		}, 
		$.validator.format("长度最多是{0}")
	);
	// 中文字三个字节
	$.validator.addMethod(
		"urangelength", 
		function(value, element, param) {
			var length = encodeURIComponent($.trim(value)).replace(/%[A-F\d]{2}/ig,"*").length;
		    return this.optional(element) || ( length >= param[0] && length <= param[1] );   
		}, 
		$.validator.format("长度介于{0}和{1}之间")
	);
	// 中文字三个字节
	$.validator.addMethod(
		"uminlength", 
		function(value, element, param) {
			var length = encodeURIComponent($.trim(value)).replace(/%[A-F\d]{2}/ig,"*").length;
		    return this.optional(element) || ( length >= param );   
		}, 
		$.validator.format("长度最少是{0}")
	);
	// 中文字三个字节
	$.validator.addMethod(
		"umaxlength", 
		function(value, element, param) {
			var length = encodeURIComponent($.trim(value)).replace(/%[A-F\d]{2}/ig,"*").length;
		    return this.optional(element) || ( length <= param );   
		}, 
		$.validator.format("长度最多是{0}")
	);
	// 固定长度，中文字三个字节
	$.validator.addMethod(
		"ufixedlength", 
		function(value, element, param) {
			var length = encodeURIComponent($.trim(value)).replace(/%[A-F\d]{2}/ig,"*").length;
		    return this.optional(element) || ( length <= param && length >=param );   
		}, 
		$.validator.format("长度必须是{0}")
	);
	//是否存在
	$.validator.addMethod(
		"exists", 
		function(value, element, param) {
			var size = $(param[1]).size();
		    return size >= param[0];   
		},
		$.validator.format("至少需要{0}个要素")
	);
	//日期验证
	$.validator.addMethod(
	  "mydate", 
	  function(value, element) {
		  if (this.optional(element)) {
			  return true;
		  }
		  if (! /^\d{4}[0-1]{1}[0-9]{1}[0-3]{1}[0-9]{1}$/.test(value)) {
			  return false;
		  }
		  try {
			  var d1 = new Date(value.substr(0, 4),parseFloat(value.substr(4, 2))-1, value.substr(6, 2));
			  if (/Invalid|NaN/.test(d1)) {
				  return false;
			  }
			  //alert(d1.getFullYear() +"-"+ parseFloat(value.substr(0, 4)) +";"+d1.getMonth() +"-"+ parseFloat(value.substr(4, 2)) +";"+ d1.getDate() +"-"+ parseFloat(value.substr(6, 2)));
			  //即使mm、dd部分不合法，比如超出范围，仍然可以正常构造对象，javascript会直接进位处理，因此需要额外比对处理前和处理后的日期是否一致
			  if (d1.getFullYear() != parseFloat(value.substr(0, 4)) || (d1.getMonth()+1) != parseFloat(value.substr(4, 2)) || d1.getDate() != parseFloat(value.substr(6, 2))) {
				  return false;
			  }
		  } catch (e) {
			  return false;
		  }
		  return true;
	 }, 
	 $.validator.format("非法日期格式(yyyyMMdd)")
	);
	//只能是字母([a-zA-Z])
	$.validator.addMethod("mustletter", 
		function(value, element) {
			return this.optional(element) || /^[a-z]([a-zA-Z]+$)/.test(value);
		}, 
		$.validator.format("请输入大小写字母")
	);
	//中文验证
	$.validator.addMethod(
	  "cnname", 
	  function(value, element) {
		return this.optional(element) || /^[\u4e00-\u9fa5]+$/.test(value);
	 }, 
	 $.validator.format("非法中文字符(含空格)")
	);
	//身份证验证
	$.validator.addMethod("id_no", 
		function(value, element, param) {
			return this.optional(element) || IdCardValidate(value);
		}, 
		$.validator.format("身份证号码不合法")
	);
	//金额验证（最多两位小数），大于0
	$.validator.addMethod(
		"money_gt0", 
		function(value, element, param) {
			return this.optional(element) || (/^\d{1,15}(\.\d{1,2})?$/.test(value) && value > 0);
		}, 
		$.validator.format("请输入大于0的金额")
	);
	//金额验证（最多两位小数），大于等于0
	$.validator.addMethod(
		"money_gteq0", 
		function(value, element, param) {
			return this.optional(element) || /^\d{1,15}(\.\d{1,2})?$/.test(value);
		}, 
		$.validator.format("请输入大于等于0的金额（最多两位小数）")
	);
	//金额验证（最多两位小数），正负0
	$.validator.addMethod(
		"money", 
		function(value, element, param) {
			return this.optional(element) || /^-?\d{1,15}(\.\d{1,2})?$/.test(value);
		}, 
		$.validator.format("非法的金额格式")
	);
	
})(jQuery);