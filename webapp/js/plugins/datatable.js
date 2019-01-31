/***
Wrapper/Helper Class for datagrid based on jQuery Datatable Plugin
 ***/
var Datatable = function () {
	var tableOptions; // main options
	var dataTable; // datatable object api
	var _dataTable; //datatable object
	var table; // actual table jquery object
	var tableContainer; // actual table container object
	var tableWrapper; // actual table wrapper jquery object
	var tableInitialized = false;
	var ajaxParams = {}; // set filter mode
	var the;

	var countSelectedRecords = function () {
		var selected = $('tbody > tr > td:nth-child(1) input[type="checkbox"]:checked', table).length;
		var text = tableOptions.dataTable.language.metronicGroupActions;
		if (selected > 0) {
			$('.table-group-actions > span', tableWrapper).text(text.replace("_TOTAL_", selected));
		} else {
			$('.table-group-actions > span', tableWrapper).text("");
		}
	};

	return {
		//main function to initiate the module
		init : function (options) {

			if (!$().dataTable) {
				return;
			}

			the = this;

			// default settings
			options = $.extend(true, {
					src : "", // actual table
					filterApplyAction : "filter",
					filterCancelAction : "filter_cancel",
					resetGroupActionInputOnSuccess : true,
					loadingMessage : '加载中...',
					dataTable : {
						"dom" : "<'row'<'col-md-8 col-sm-12'pli><'col-md-4 col-sm-12'<'table-group-actions pull-right'>>r><'table-scrollable't><'row'<'col-md-8 col-sm-12'pli><'col-md-4 col-sm-12'>>", // datatable layout
						"pageLength" : 10, // default records per page
						"lengthMenu" : [
							[10, 20],
							[10, 20]
						],
						"language" : { // language settings
							// metronic spesific
							"metronicGroupActions" : "_TOTAL_ 记录被选择:  ",
							"metronicAjaxRequestGeneralError" : "不能完成请求，请检查网络链接",

							// data tables spesific
							"lengthMenu" : "<span class='seperator'>|</span>显示 _MENU_ 条记录",
							"info" : "<span class='seperator'>|</span>共 _TOTAL_ 条记录  <<当前第_PAGE_ 页,共 _PAGES_页>>",
							"infoEmpty" : "没有数据可显示",
							"emptyTable" : "没有可用记录",
							"zeroRecords" : "抱歉， 没有找到",
							"infoFiltered" : "(从 _MAX_ 条记录中过滤)",
							"paginate" : {
								"previous" : "上页",
								"next" : "下页",
								"last" : "尾页",
								"first" : "首页",
								"page" : "第",
								"pageOf" : "页  总页数"
							}
						},

						//"orderCellsTop": true,
						"pagingType" : "bootstrap_extended", // pagination type(bootstrap, bootstrap_full_number or bootstrap_extended)
						"autoWidth" : false, // disable fixed width and enable fluid table
						"processing" : false, // enable/disable display message box on record load
						"serverSide" : true, // enable/disable server side ajax loading

						"ajax" : { // define ajax settings
							"url" : "", // ajax URL
							"type" : "POST", // request type
							"timeout" : 50000,
							"data" : function (data) { // add request parameters before submit
								$.each(ajaxParams, function (key, value) {
									data[key] = value;
								});
								/*Metronic.blockUI({
									message : tableOptions.loadingMessage,
									target : tableContainer,
									overlayColor : 'none',
									cenrerY : true,
									boxed : true
								});*/
								return JSON.stringify(data);
							},
							"dataSrc" : function (res) { // Manipulate the data returned from the server
								if (res.customActionMessage) {
									Metronic.alert({
										type : (res.customActionStatus == 'OK' ? 'success' : 'danger'),
										icon : (res.customActionStatus == 'OK' ? 'check' : 'warning'),
										message : res.customActionMessage,
										container : tableWrapper,
										place : 'prepend'
									});
								}

								if (res.customActionStatus) {
									if (tableOptions.resetGroupActionInputOnSuccess) {
										$('.table-group-action-input', tableWrapper).val("");
									}
								}

								if ($('.group-checkable', table).length === 1) {
									$('.group-checkable', table).attr("checked", false);
									$.uniform.update($('.group-checkable', table));
								}

								if (tableOptions.onSuccess) {
									tableOptions.onSuccess.call(undefined, the);
								}

								/*Metronic.unblockUI(tableContainer);*/

								return res.data;
							},
							"error" : function (data) { // handle general connection errors
								if (tableOptions.onError) {
									tableOptions.onError.call(undefined, the);
								}
								Metronic.alert({
									type : 'danger',
									icon : 'warning',
									message : tableOptions.dataTable.language.metronicAjaxRequestGeneralError,
									container : tableWrapper,
									place : 'prepend'
								});

								/*Metronic.unblockUI(tableContainer);*/
							}
						},

						"drawCallback" : function (oSettings) { // run some code on table redraw
							if (tableInitialized === false) { // check if table has been initialized
								tableInitialized = true; // set table initialized
								table.show(); // display table
							}
							/*Metronic.initUniform($('input[type="checkbox"]', table));*/ // reinitialize uniform checkboxes on each table reload
							countSelectedRecords(); // reset selected records indicator
						}
					}
				}, options);

			tableOptions = options;

			//判断是否为demo模式，默认datatable是请求到服务器上，在没有服务器的情况下，加载blockui会一直堵塞，所以demo模式下修改serverSide变量为false
			//tableOptions.dataTable.serverSide = false;

			// create table's jquery object
			table = $(options.src);
			tableContainer = table.parents(".table-container");

			// apply the special class that used to restyle the default datatable
			var tmp = $.fn.dataTableExt.oStdClasses;

			$.fn.dataTableExt.oStdClasses.sWrapper = $.fn.dataTableExt.oStdClasses.sWrapper + " dataTables_extended_wrapper";
			$.fn.dataTableExt.oStdClasses.sFilterInput = "form-control input-small input-sm input-inline";
			$.fn.dataTableExt.oStdClasses.sLengthSelect = "form-control input-xsmall input-sm input-inline";

			// initialize a datatable
			//dataTable = table.DataTable(options.dataTable);
			_dataTable = table.dataTable(options.dataTable);
			dataTable = _dataTable.api();
			// revert back to default
			$.fn.dataTableExt.oStdClasses.sWrapper = tmp.sWrapper;
			$.fn.dataTableExt.oStdClasses.sFilterInput = tmp.sFilterInput;
			$.fn.dataTableExt.oStdClasses.sLengthSelect = tmp.sLengthSelect;

			// get table wrapper
			tableWrapper = table.parents('.dataTables_wrapper');

			// build table group actions panel
			if ($('.table-actions-wrapper', tableContainer).length === 1) {
				$('.table-group-actions', tableWrapper).html($('.table-actions-wrapper', tableContainer).html()); // place the panel inside the wrapper
				$('.table-actions-wrapper', tableContainer).remove(); // remove the template container
			}
			// handle group checkboxes check/uncheck
			$('.group-checkable', table).change(function () {
				var set = $('tbody > tr > td:nth-child(1) input[type="checkbox"]', table);
				var checked = $(this).is(":checked");
				$(set).each(function () {
					$(this).attr("checked", checked);
				});
				$.uniform.update(set);
				countSelectedRecords();
			});

			// handle row's checkbox click
			table.on('change', 'tbody > tr > td:nth-child(1) input[type="checkbox"]', function () {
				countSelectedRecords();
			});

			// handle filter submit button click
			table.on('click', '.filter-submit', function (e) {
				e.preventDefault();
				the.submitFilter();
			});
			// handle filter cancel button click
			table.on('click', '.filter-cancel', function (e) {
				e.preventDefault();
				the.resetFilter();
			});

		},

		submitFilter : function () {
			the.setAjaxParam("action", tableOptions.filterApplyAction);

			// get all typeable inputs
			$('textarea.form-filter, select.form-filter, input.form-filter:not([type="radio"],[type="checkbox"])', table).each(function () {
				the.setAjaxParam($(this).attr("name"), $(this).val());
			});

			// get all checkboxes
			$('input.form-filter[type="checkbox"]:checked', table).each(function () {
				the.addAjaxParam($(this).attr("name"), $(this).val());
			});

			// get all radio buttons
			$('input.form-filter[type="radio"]:checked', table).each(function () {
				the.setAjaxParam($(this).attr("name"), $(this).val());
			});

			dataTable.ajax.reload();
		},

		resetFilter : function () {
			$('textarea.form-filter, select.form-filter, input.form-filter', table).each(function () {
				$(this).val("");
			});
			$('input.form-filter[type="checkbox"]', table).each(function () {
				$(this).attr("checked", false);
			});
			the.clearAjaxParams();
			the.addAjaxParam("action", tableOptions.filterCancelAction);
			dataTable.ajax.reload();
		},

		getSelectedRowsCount : function () {
			return $('tbody > tr > td:nth-child(1) input[type="checkbox"]:checked', table).length;
		},

		getSelectedRows : function () {
			var rows = [];
			$('tbody > tr > td:nth-child(1) input[type="checkbox"]:checked', table).each(function () {
				rows.push($(this).parent().parent().parent().parent());
			});

			return rows;
		},

		setAjaxParam : function (name, value) {
			ajaxParams[name] = value;
		},

		addAjaxParam : function (name, value) {
			if (!ajaxParams[name]) {
				ajaxParams[name] = [];
			}

			skip = false;
			for (var i = 0; i < (ajaxParams[name]).length; i++) { // check for duplicates
				if (ajaxParams[name][i] === value) {
					skip = true;
				}
			}

			if (skip === false) {
				ajaxParams[name].push(value);
			}
		},

		clearAjaxParams : function (name, value) {
			ajaxParams = {};
		},

		getDataTable : function () {
			return dataTable;
		},

		getTableWrapper : function () {
			return tableWrapper;
		},

		gettableContainer : function () {
			return tableContainer;
		},

		getTable : function () {
			return table;
		},

		bindTableDelete : function (dataKey, confirmMessage, retfunc) {
			confirmMessage = confirmMessage == "" ? "确定要删除该条数据 ?" : confirmMessage;
			table.on('click', '.delete', function (e) {
				e.preventDefault();
				var delObj = $(this);
				bootbox.confirm(confirmMessage, function (result) {
					if (!result) {
						return;
					}
					var data = {};
					var dataValue = delObj.attr("data-src").split(",");
					for (var i = 0; i < dataKey.length; i++) {
						data[dataKey[i]] = dataValue[i];
					}
					var nRow = delObj.parents('tr')[0];
					jQuery.ajax({
						type : "DELETE",
						contentType : "application/json",
						async : false,
						data : $.toJSON(data),
						url : tableOptions.dataTable.ajax.url,
						dataType : "json",
						success : function (d) {
							if (d.retCode != '0000') {
								bootbox.alert(d.retMsg);
								return;
							}
							bootbox.alert("删除成功", function () {
								_dataTable.fnDeleteRow(nRow);
							});
						}
					});
				});
			});
		},
		bindTableEdit : function (dataKey, editForm) {
			table.on('click', '.edit', function (e) {
				//获取选中行数据
				e.preventDefault();
				var editObj = $(this);
				var data = {};
				var dataValue = editObj.attr("data-src").split(",");
				for (var i = 0; i < dataKey.length; i++) {
					Object.defineProperty(data, dataKey[i], {
						value : dataValue[i]
					});
				}
				var nRowA = $(editObj.parents('tr')[0]).find("td");
				editForm(nRowA);
			});
		},
		/**
		 * 增加数据表格中的事件
		 * selector:时间对象，写法同jQuery 选择器
		 * event:事件类型，如 click ,dbclick,mouseenter等
		 * dataKey：主键
		 * handler:事件触发的方法，data:主键ID值
		 */
		addBindEvent : function (selector, event, dataKey, handler) {
			table.on(event, selector, function (e) {
				//获取选中行数据
				e.preventDefault();
				var editObj = $(this);
				var data = {};
				var dataValue = editObj.attr("data-src").split(",");
				for (var i = 0; i < dataKey.length; i++) {
					Object.defineProperty(data, dataKey[i], {
						value : dataValue[i]
					});
				}
				//console.info(dataKey);
				var nRowA = $(editObj.parents('tr')[0]).find("td");
				//console.log($(nRowA[0]).text());
				handler(data);
			});
		},
		bindSettingEdit : function (event, dataKey, editForm) {
			table.on('click', event, function (e) {
				//获取选中行数据
				e.preventDefault();
				var editObj = $(this);
				var data = {};
				var dataValue = editObj.attr("data-src").split(",");
				for (var i = 0; i < dataKey.length; i++) {
					Object.defineProperty(data, dataKey[i], {
						value : dataValue[i]
					});
				}
				var nRowA = $(editObj.parents('tr')[0]).find("td");
				editForm(nRowA);
			});
		}
	};
};
