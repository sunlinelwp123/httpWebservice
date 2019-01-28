/**
 * 复核功能JS
 */
var ReCheck=function(){
	//复核密码验证数据
    var _checkData = null;
    //事件对象
    var _eventObj = null;
    //事件默认click
    var _event = "click";
    
    $("#check_save",$("#index_checkModal")).unbind("click");//解绑原事件
	//人员复核弹出窗 确认处理
	$("#check_save",$("#index_checkModal")).click(function(e){
		e.preventDefault();
		var data={};
		$.each($("input",$("#check_form")),function(i,n){
			if (n.name != undefined && n.name != "" && n.name != null) {
				data[n.name] = n.value;
			}
		});
		//验证复核密码
		Sunline.ajaxRouter("recheck/uschck", data, "POST", 
			function(ret){
				 if(ret.retCode == "0000"){
					 _checkData=data;
					 $(_eventObj).trigger(_event);//触发原事件--重新提交
				 }else{
					 _checkData=null
					 $('.msg', $("#index_checkModal")).text(ret.retMsg);
					 $('.alert-success',$("#index_checkModal")).hide();
					 $('.alert-danger',$("#index_checkModal")).show();	
				 }
			}, 
			function(ret){
				_checkData=null
				$('.msg', $("#index_checkModal")).text("复核验证请求异常");
				$('.alert-success',$("#index_checkModal")).hide();
				$('.alert-danger',$("#index_checkModal")).show();			
			}, "json",true);
	});	
	
	/**
     *复核弹窗
	 */
	var _reCheckMode = function( type, obj, event){
		if(type === "p"){
			//复核人员复核
			$("#index_checkModal").modal("show");
		}else{
			//其他复核
			bootbox.alert("复核类型暂不支持");
		}
		_eventObj=obj;
		_event=event;
	}
	
	var _clearModal = function(){
		$('.alert-success',$("#index_checkModal")).hide();
		$('.alert-danger',$("#index_checkModal")).hide();	
		$('.msg', $("#index_checkModal")).text("");
		$('input[name="authus"]', $("#index_checkModal")).val("");
		$('input[name="authpw"]', $("#index_checkModal")).val("");
	}
	
	var _hideModal = function(){
		$("#index_checkModal").modal("hide");
	}
	
	//F8键盘事件
	$(document).keydown(function (event) {
		$('button[data-bb-handler="ok"]',$(".modal-dialog")).trigger("click");//隐藏原弹出窗。
	    if (event.keyCode === 119) {//F8  键盘码 119 
	    	$("#index_recheck").trigger("click");
	    };
	});
	
	var _getCheckData = function(input){
		if(!Sunline.isNull(_checkData)){
			$.each(_checkData,function(i,n){
				input[i]=n;
			});
		}
		_checkData = null;
		_clearModal();
	}
	
	var  _getClickEvent = function(func){
		$("#index_recheck").unbind("click");//解绑原事件
		$("#index_recheck").bind('click',function(){//绑定新事件
			      func();
		});
	}
	
	
	return {
		/**
		 * 人员复核,获取复核窗口 暂时支持柜员复核 "p"
		 * type 授权类型
		 * obj 授权后要执行事件对象,如 $("***")
		 * event 要执行的事件,如:'ciick'
		 */
		getCheckMode : function(type,obj,event){
			return _reCheckMode(type,obj,event);
		},
		/**
		 *  获取复核数据并赋值
		 */
		getCheckData : function(input){
			return _getCheckData(input);
		},
		/**
		 *  复核数据赋值
		 */
		setCheckData : function(checkData){
			_checkData=checkData;
		},
		/**
		 *  清除复核数据
		 */
		clearModal : function(){
			_clearModal();
		},
		/**
		 *  隐藏复核窗口
		 */
		hideModal : function(){
			_hideModal();
		},
		/**
		 *  获取绑定事件
		 *  func 绑定执行方法
		 */
		getClickEvent :function(func){
			_getClickEvent(func);
		}
	}
	
}();


