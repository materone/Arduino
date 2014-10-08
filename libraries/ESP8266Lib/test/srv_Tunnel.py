#!/usr/bin/env python 

import SocketServer 
from time import ctime
HOST = '192.168.1.7'
PORT = 8888 
ADDR = (HOST, PORT) 

class MyRequestHandler(SocketServer.BaseRequestHandler): 
    def handle(self): 
        print 'connected from:',self.client_address 
        while True: 
		print '%s' % self.request.recv(1024)
            	self.request.sendall('[%s] %s' % (ctime(),self.request.recv(1024))) 
	#self.request.sendall('[%s] i am Server' % (ctime()))

tcpServ = SocketServer.ThreadingTCPServer(ADDR, MyRequestHandler) 

print 'waiting for connection...'

tcpServ.serve_forever()
