#!/usr/bin/python
# -*- coding: UTF-8 -*-
# Copyright (c) 2014,OpenThings Projects.
# 通过串口进行WiFi模块参数配置，需要USB转TTL连接模块。

import sys
import os
import time
import serial 

/*
==================================
ESP8266作为IPSERVER:
模块：
设为STA模式：AT+CWMODE=1
连接到路由器：AT+CWJAP="HiWiFi","12345678"
查询分配的IP地址：AT+CIFSR
设为多重连接允许：AT+CIPMUX=1
建立IP服务器：AT+CIPSERVER=1,8888

测试IPClient(Python)：
python tcpTimer.py

收到的数据：
+IPD,1,Length:Data

OK
===================================
*/
portDevice = '/dev/ttyUSB0'		#Arduino连接的端口。
baudrate = 9600

AP = "NETGEAR71"				#无线接入点SSID。
PWD = "shinyphoenix155"			#密码。
portIPServer = 8888				#模块IP服务器的端口。
SrvClient = "192.168.199.199"	#客户端连接的远程地址。
portClient = 9999				#客户端连接的远程端口。

#while True: 
print "WiFi AT Configuration Tool. V1.0"
print "Created by OpenThings Project,2014-09-29."

def getResult():
	while (ser.inWaiting()>0)
		sData = ser.readline()
		print sData

ser = serial.Serial(portDevice,baudrate)

ser.writeline("@")
getResult()

ser.writeline("@CWMODE=1")
getResult()

ser.writeline("@CWJAP="+"\""+AP+"\","+"\""+PWD+"\"")
getResult()

ser.writeline("@CIPMUX=1")
getResult()

ser.writeline("@CIPSERVER=1,"+portIPServer)
getResult()

ser.writeline("@CIFSR")
getResult()

#配置为客户端访问模式。
#ser.writeline("@CIPSTART=")
#ser.writeline("@CIFSR")

ser.close()
print "Serial port closed."


