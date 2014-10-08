#!/usr/bin/python
# Copyright (c) 2010-2013,All rights reserved,wangerqi@supermap.com.
# This shows a service of an MQTT subscriber.

import sys
import datetime
import socket, sys

#======================================================        
#MQTT Initialize.--------------------------------------
try:
    import paho.mqtt.client as mqtt
except ImportError:
    # If you have the module installed, just use "import paho.mqtt.client"
    import os
    import inspect
    cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile( inspect.currentframe() ))[0],"../src")))
    if cmd_subfolder not in sys.path:
        sys.path.insert(0, cmd_subfolder)
    import paho.mqtt.client as mqtt

#======================================================
def on_connect(mqttc, obj, rc):
    print("OnConnetc, rc: "+str(rc))

def on_publish(mqttc, obj, mid):
    print("OnPublish, mid: "+str(mid))

def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_log(mqttc, obj, level, string):
    print("Log:"+string)

def on_message(mqttc, obj, msg):
    curtime = datetime.datetime.now()
    strcurtime = curtime.strftime("%Y-%m-%d %H:%M:%S")
    print(strcurtime + ": " + msg.topic+" "+str(msg.qos)+" "+str(msg.payload))
    on_exec(str(msg.payload))

def on_exec(strcmd):
    print "Exec:",strcmd
    strExec = strcmd
    
#=======================================
#if __name__ == '__main__':
        
# If you want to use a specific client id, use
# mqttc = mqtt.Client("client-id")
# but note that the client id must be unique on the broker. Leaving the client
# id parameter empty will generate a random id for you.
mqttc = mqtt.Client()
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe

mqttc.on_log = on_log

#Using Mosquitto MQTT Borker.
#Local Server.
strBroker = "localhost"

#Lan Server.
strBroker = "192.168.12.25"

#public server by SMI.
#strBroker = "112.124.67.178"

#test server by eclipse funds.
#strBroker = "m2m.eclipse.org"

mqttc.connect(strBroker, 1883, 60)
mqttc.subscribe("/inode/info", 0)
mqttc.loop_forever()
  
