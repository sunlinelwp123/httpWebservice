var Tree = function(){
	
	return {
		init: function(options){
			options = $.extend(true, {
                src : "",
                url : "",
                qrysrc : "",
                selectEvent : null,
                menu : null
            }, options);
			
			$(options.src).jstree({
				"core" : {
	                "themes" : {
	                    "responsive": false
	                }, 
	                // so that create works
	                "check_callback" : true,
	                'data' : {
	                    'url' : function (node) {
	                    	return Sunline.ajaxPath(options.url,node);
	                    },
	                    'data' : function (node) {
	                    	var data = {};
	                    	//获取根节点
	                    	if(node.id === '#'){
	                    		data = {'action': 'queryRootNodes'};
	                    	}else{
	                    		data = {'action': 'queryChildNodes',
	                    				'parent': node.id 
	                    			};
	                    	}
	                    	return data;
	                    }
	                }
	            },
	            "types" : {
	                "default" : {
	                    "icon" : "fa fa-folder icon-state-warning icon-lg"
	                },
	                "file" : {
	                    "icon" : "fa fa-file icon-state-warning icon-lg"
	                }
	            },
	            "plugins" : [ "types", "search", "wholerow" ,"contextmenu"],
	            "contextmenu" : options.menu
			}).bind("select_node.jstree", function(e,data){options.selectedEvent(e,data)});
			
			var to = false;
			$(options.qrysrc).keyup(function () {
				if(to) { clearTimeout(to); }
				to = setTimeout(function () {
					var v = $(options.qrysrc).val();
					$(options.src).jstree(true).search(v);
				}, 250);
			});
		}
	};
};