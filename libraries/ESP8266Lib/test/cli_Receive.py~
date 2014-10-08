#!/usr/bin/env python 

from time import *
from socket import * 

HOST = '192.168.199.191'
#HOST = '192.168.1.7'
PORT = 8888
BUFSIZ = 1024 
ADDR = (HOST, PORT) 

tcpCliSock = socket(AF_INET, SOCK_STREAM) 
#tcpCliSock.setdefaulttimeout(20)
tcpCliSock.connect(ADDR) 

print "Receiving..."
while True: 	
	try:
		data = tcpCliSock.recv(BUFSIZ) 
		if data == "EXIT": 
		    break 
		data = ctime() + ": " + data
		print "Rev:",data.strip()
		#tcpCliSock.send("Hi: "+data);
		tcpCliSock.send("Data Received.");
	except	tcpCliSock.timeout as ex:
		print "Error:" + ex
		tcpCliSock.connect(ADDR) 
		print "Reconnected."
		
tcpCliSock.close()
print "TCP Client Closed."
