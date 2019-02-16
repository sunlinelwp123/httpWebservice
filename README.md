一个极其简单的HTTP服务器，用与个人学习使用。
实现了读取静态资源，AJAX动态数据交互以及落库。
ajax请求数据格式 application/json 暂时不支持表单提交和其他提交方式
动态请求通过cgi通用网关接口（common gateway interface）调用对应程序

服务器代码
include/t_net.h
webcore.c
t_net.c
go语言实现 http服务器代码httpservice.go

服务案例
/webapp/rest/user/...

TODO 
服务路由
session
dict定义
日志分级
配置文件



@AUTH LWP
