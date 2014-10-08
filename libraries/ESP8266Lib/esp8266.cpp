/*=======================================================================
 ESP8266 WiFi Module by UART for Arduino.

 Author:OpenThings@163.com,
 Last Modified: 2014.09.25.

 获取最新版本：
	git clone https://git.oschina.net/supergis/ESP8266Lib.git
 在用于Arduino软件串口时，波特率只支持9600版本，并且需要独立供电（3.3V,300MA）。 
 =========================================================================*/

#include <Arduino.h>
#include "SoftSerial.h"
#include "esp8266.h"
//#define  espSerial Serial
SoftSerial espSerial; // RX, TX

esp8266::esp8266(void)
{
	//等待串口连接。只有 Leonardo 需要。
  	//while (!espSerial){
     	//}
}

bool esp8266::init(long bandrate,uint8_t rx,uint8_t tx)
{	
    espSerial.init(rx,tx);
    espSerial.begin(bandrate);
    
    //To Hard Reset the WiFi module,Connect to CP_RST.
    pinMode(pin_RESET, OUTPUT);		
    return true;
}

void esp8266::GetInfo()
{
    isOK();    delay(200);    
    Version(); delay(200);
    showMode();delay(200);
    showJAP(); delay(200);    
    showMux(); delay(200);
    showIP();  delay(200);
    Serial.println("\n-----Info Finished---------------");
}

//建立硬串口（USB）与软件串口的透传通道。
//模块调试和测试是非常有用哦！
//Transform between UART and SoftSerial.
void esp8266::bridge() 
{
    if (espSerial.available())
    {
        Serial.write(espSerial.read());
    }
    if (Serial.available())
    {
        espSerial.write(Serial.read());
    }
}

//RUN AT and Normal UART mode.
void esp8266::xCommander() 
{
    if (Serial.available()>0)
    {
        char a = Serial.read();
        data_uart = data_uart + a;
        if ((a=='\r')||(a=='\n'))        //one Phase finished.
        {
          onCommand();
          data_uart = "";
        }
        if (data_uart.length()>256) 	//Reset if too mush...
           data_uart = "";
    }
}

void esp8266::onCommand()
{
	Serial.print("UART: ");
	Serial.println(data_uart);
	
	String prefix = data_uart.substring(0,1) ;
	String strCMD =  data_uart.substring(1);
	Serial.print("CMD:");	
	Serial.println(prefix);
	Serial.println(strCMD);		

	if (prefix == String("@"))	//Send AT Msg.
	{	
		String strAT;
		if (data_uart.length()<=3)
			strAT = "AT\r\n";
		else
			strAT = "AT+" + strCMD;
		espSerial.print(strAT);
	}
	else if (prefix == String("^"))	//Send Control Msg.
	{	
	}	
	else if (prefix == String("~"))	//Send Data Msg.
	{	
		Send(0,strCMD);
	}	
	else
	{
		Serial.println("Unknow CMD: "+data_uart);
	}
}

bool esp8266::xReceiver() 
{
	if (espSerial.available()>0)
	{
		char a = espSerial.read();      //Serial.print(a);
		data_rev = data_rev + a;
		//Serial.write(a);				//for debug msg.

		if (data_rev.length()>data_max)
		{
		  data_rev = "";       			//if Data too more,reset receive buffer,data will lost.
		  Serial.println("#data_rev reset.");
		  return false;  
		}
		
		//Find a completed phase.
		if (data_rev.indexOf(T_OK) >= 0)
		  return true; 	
	}
	return false;
}

//Process Received data as "+IPD,0," will finish with "\r\nOK" and as is.
String esp8266::onReceive()
{
	int n1= data_rev.indexOf("D,");
	if (n1<0)
	{ 
		//onOther();
		data_rev = "" ;
		return "";   	//Non +IPD phase, just pass it.
	}
	//Serial.println(">>"+data_rev);
	
	n1 += 2;  
	int chID = data_rev.substring(n1,n1+1).toInt();
	int n2= data_rev.indexOf(":")+1;
	int nLen = data_rev.substring(n1+2 , n2).toInt();

	String data = data_rev.substring(n2,n2+nLen);
	Serial.println("D="+data);
	data_rev = "" ;
    return data;
}

//====================================================
//Quick Start a TCP Server.
void esp8266::startServer(String ap,String pwd,long nPort)
{
    Serial.print("\r\nSet WiFi Mode to 1...");
	configMode(1);	delay(200); 
	Serial.println("OK."); 

    Serial.print("\r\nSet AP...");  
	Serial.print(ap);
    configJAP(ap,pwd);    delay(5000);  
	Serial.println(" OK."); 
  
	Serial.print("\r\nStart Server at ");  
	Serial.print(nPort);	
	startServer(nPort);	delay(2000);
	Serial.println(" OK.");

	Serial.println("Current IP ");  
	showIP();	delay(200);
}

void esp8266::startClient(String ipServer,long nPort)
{
}

void esp8266::startClientUDP(String ipServer,long nPort)
{
}

//=========================
//发送AT指令。
void esp8266::runAT(String strAT)
{
    espSerial.print(strAT);
}

void esp8266::runAT_End(String strAT)
{
    espSerial.println(strAT);
}

//===================================
//处理返回值，通用函数。
//等待返回结果，此时输入会“Busy”，然后挂起。
String esp8266::waitData(String Tag1, String Tag2,String Tag3,String Tag4)
{
    String data;
    while (1)
    {
        if(espSerial.available()>0)
        {
            char a = espSerial.read();
            data = data + a;
	    timeLast = millis();	    
        }
	else
	{
	    timeFree = millis();
	}
	if ((timeFree>timeLast)&&(timeFree-timeLast)>timeInterval) break;
		
	//找到任何一个标识符即退出。
        if ((Tag1!="")&&(data.indexOf(Tag1) != -1)) break;
	if ((Tag2!="")&&(data.indexOf(Tag2) != -1)) break;
	if ((Tag3!="")&&(data.indexOf(Tag3) != -1)) break;
	if ((Tag4!="")&&(data.indexOf(Tag4) != -1)) break;	
    }
    return data;
}

//---------------------------------
//如果没有等到OK，会一直挂起。
//后续考虑：加入超时机制，超时自动退出，并重启模块。
String esp8266::waitOK()
{
    return waitData("OK",T_ERROR);
}

String esp8266::waitInput()
{
    return waitData(">","","","");
}

String esp8266::waitSendFinish()
{
    return waitData(T_SENDOK,T_ERROR,"","");
}


//==================================
String esp8266::isOK()
{
    runAT_End(AT);
    return "";
    /*
    if (waitData("OK","O")!="")  
        return "OK";
    else
        return "ERROR";
  */
}

/*===================================
 * esp8266 Firmware Version
 ===================================*/
String esp8266::Version(void)
{
    runAT_End(AT_GMR);
    return "";
    /*
    String str = waitData("OK","O");
    str = str.substring(0,7);
    return str ;
    */
}

/*===================================
 * 重启esp8266芯片
 ===================================*/
void esp8266::Reset(void)
{
    runAT_End(AT_RST);
    /*
    waitData("ready");
    */
}

//Using D_PIN_9 to RST,Signal LOW to reset.
void esp8266::HardReset(void)
{
    digitalWrite(pin_RESET, LOW);
    delay(1000);
    digitalWrite(pin_RESET, HIGH);
} 

/*********************************************
                   esp8266功能指令
 *********************************************
 
/*==============================
 查询目前esp8266模块的工作模式
===============================*/
String esp8266::showMode()
{
    runAT_End(AT_CWMODE_GET);  //发送AT指令
    return "";
    /*
    String data;
    data = waitData("OK","K");
    Serial.println("Mode:");
    Serial.println(data);
    if(data.indexOf("1")!=-1)
    {
        return "Station";
    }
    else if(data.indexOf("2")!=-1)
    {
        return "AP";
    }
    else if(data.indexOf("3")!=-1)
    {
        return "AP+Station";
    }
    return "";
    */
}

/*================================
 配置esp8266模块的工作模式(需要重启后方可生效)(done)
 ================================*/
void esp8266::configMode(int aMode)
{
    runAT(AT_CWMODE_SET);  //发送AT指令
    String a = String(aMode);
    a.trim();
    runAT_End(a);
    /*
    String data;
    data = waitData("no change");
    */
}

/*===================================
 * 返回（字符串）当前esp8266搜索到的接入点信息(只能在)
 * 加密类型、SSID、信号强度
 ===================================*/
String esp8266::showAP(void)
{
    runAT_End(AT_CWLAP);  //发送AT指令
    return "";
    /*
    String data;
    data = waitData("OK","error","","");
    if (data.indexOf("error")!=-1)
    {
        return "ERROR";
    }
    else
    {
        char head[4] = {0x0D,0x0A};                 //头部多余字符串
        char tail[7] = {0x0D,0x0A,0x0D,0x0A};       //尾部多余字符串
        data.replace("AT+ShowAP","");
        data.replace("done","");
        data.replace("+ShowAP","esp8266");
        data.replace(head,"");
        data.replace(tail,"");
        return data;
    }
    */
 }

/*===================================
 * 查询返回目前连接的esp8266接入点信息  （done）
===================================*/
String esp8266::showJAP(void)
{
    runAT_End(AT_CWJAP_GET);  //发送AT指令
    return "";
    /*
    String data;
    data = waitData("OK");
    
    char head[4] = {0x0D,0x0A};   //头部多余字符串
    char tail[7] = {0x0D,0x0A,0x0D,0x0A};        //尾部多余字符串
    data.replace("AT+JoinAP?","");
    data.replace("+JoinAP","AP");
    data.replace("done","");
    data.replace(head,"");
    data.replace(tail,"");
    
    return data;
    */
}

/*=======================================
 * 配置登陆网络需要的ssid名称以及密码(未验证，未测试)
 =======================================*/
void esp8266::configJAP(String ssid , String pwd)
{
    runAT(AT_CWJAP_SET);  
    runAT("\"");   runAT(ssid);    runAT("\"");      //"ssid"
    runAT(","); 
    runAT("\"");   runAT(pwd);    runAT_End("\"");   //"pwd"
    
    /*
    waitOK();
    */
}

/*=============================================
 * 退出目前登录的esp8266节点
 ============================================*/
void esp8266::quitAP(void)
{
    runAT_End(AT_CWQAP);
    /*
    waitOK();
    */
}

/*===============================================
 * AP模式下的网络配置
==============================================*/
String esp8266::showSAP()
{
    runAT_End(AT_CWSAP_GET);  //发送AT指令
    return "";
    /*
    String data;
    data = waitOK();
    char head[4] = {0x0D,0x0A};   //头部多余字符串
    char tail[7] = {0x0D,0x0A,0x0D,0x0A};        //尾部多余字符串
    data.replace("AT+SAP?","");
    data.replace("+SAP","mySAP");
    data.replace("done","");
    data.replace(head,"");
    data.replace(tail,"");
    return data;
    */
}

/*==============================================
 * 设置AP模式下的SSID 密码 信道 加密方式等信息  
 * 只能用于ap模式或者ap+Station模式下，重启芯片后生效.
================================================*/
void esp8266::configSAP(String ssid , String pwd , byte chl , byte ecn)
{
    runAT(AT_CWSAP_SET);                                //发送AT指令
    runAT("\"");    runAT(ssid);    runAT("\"");   //"ssid"
    runAT(",");
    runAT("\"");    runAT(pwd);    runAT("\"");   //"pwd"
    runAT(",");
    runAT(String(chl)); 
    runAT(",");
    runAT_End(String(ecn));
    /*   
    waitData("OK");
    */
}

/*********************************************
                   TPC/IP功能指令
 *********************************************/

/*==============================================
 * 获取目前的链接状态
 * <ID>  0-4
 * <type>  tcp or udp
 * <addr>  ip
 * <port>  port number
===============================================*/
String esp8266::showStatus(void)
{
    runAT_End(AT_CIPSTATUS);  //发送AT指令
    return "";
    /*
    String data;
    data = waitOK();
    char head[4] = {0x0D,0x0A};   //头部多余字符串
    char tail[7] = {0x0D,0x0A,0x0D,0x0A};        //尾部多余字符串
    data.replace("AT+ShowSTA","");
    data.replace("done","");
    data.replace(head,"");
    data.replace(tail,"");
    return data;
    */
}

/*============================================
 * 查询目前的链接模式（单链接or多链接）
 ============================================*/
String esp8266::showMux(void)
{
    runAT_End(AT_CIPMUX_GET);  //发送AT指令
    return "";
    /*
    String data;
    data = waitOK();
    
    char head[3]={0x0D,0x0A};   //将回显数据处理成需要的格式
    data.replace("AT+MUX?","");
    data.replace("+MUX","showMux");
    data.replace("done","");
    data.replace(head,"");
    return data;
    */
}

/*============================================
 * 设置链接模式（单链接or多链接）
==============================================*/
void esp8266::configMux(int aMux)
{
    runAT(AT_CIPMUX_SET);
    String a = String(aMux);
    a.trim();
    runAT_End(a);           //发送AT指令
    /*
    waitData("Link is builded");
    */
}


/*============================================
 * 建立tcp链接或者注册UDP端口号（单路模式）
 ===========================================*/
void esp8266::connectOne(String type, String addr, int port)
{
    runAT(AT_CIPSTART);
    type.toLowerCase();
    if (type==String("tcp"))
    {
        runAT("\"TCP\"");
    }
    else
    {
        runAT("\"UDP\"");
    }
    runAT(",");
    runAT("\"");    runAT(addr);    runAT("\"");
    runAT(",");
    runAT_End(String(port));
    /*
    waitData("ALREAY CONNECT");
    */
}

/*============================================
 * 建立tcp链接或者注册UDP端口号（多路模式）（0-4共计5路）
 =============================================*/
void esp8266::connectMux( byte id, String type, String addr, int port)
{
    runAT(AT_CIPSTART);
    runAT( "\"" );    runAT( String(id) );    runAT( "\"" );
    type.toLowerCase();
    if (type==String("tcp"))
    {
        runAT("\"TCP\"");
    }
    else
    {
        runAT("\"UDP\"");
    }
    runAT(",");
    runAT("\"");    runAT(addr);    runAT("\"");
    runAT(",");
    runAT_End(String(port));
    /*
    waitData("ALREAY CONNECT");
    */
}

/*==============================================
 * 发送数据(单路模式)
 ===============================================*/
void esp8266::Send(String strMsg)
{
    runAT(AT_CIPSEND);
    runAT_End(String(strMsg.length()));

    waitInput();            //等待发送状态就绪。
    runAT(strMsg);          //发送字符串。
    waitSendFinish();       //等待发送结束。
}

/*==============================================
 * 发送数据(多路路模式)
 ==============================================*/
void esp8266::Send(int id, String strMsg)
{
    runAT(AT_CIPSEND);
    runAT(String(id));
    runAT(",");
    runAT_End(String(strMsg.length()));

    waitInput();           //等待发送状态就绪。
    runAT( strMsg );       //发送字符串。
    waitSendFinish();      //等待发送结束。
}

/*============================================
 * 关闭TCP或UDP链接（单路模式）
 =============================================*/
void esp8266::closeOne(void)
{
    runAT_End(AT_CIPCLOSE);
    /*
    waitData("we must restart");
    */
}

/*============================================
 * 关闭TCP或UDP链接（多路模式）
==============================================*/
void esp8266::closeMux(byte id)
{
    runAT(AT_CIPCLOSEMUX);
    runAT_End(String(id));
    /*
    waitData("Link is not","Cant close");
    */
}

/*===========================================
 * 获取目前的本机IP地址
=============================================*/
String esp8266::showIP()
{
    runAT_End(AT_CIFSR);  //发送AT指令
    return "";
    /*
    String data;
    data = waitOK();
    return data;
    */
}

/*===========================================
 * 配置为服务器
=============================================*/
void esp8266::startServer(int port)
{	
    runAT(AT_CIPSERVER);  //发送AT指令
    runAT("1,");    runAT_End(String(port));
    /*
    waitData("Link is builded");
    */
}

void esp8266::stopServer(int port)
{	
    runAT(AT_CIPSERVER);  //发送AT指令
    runAT("0,");    runAT_End(String(port));
    /*
    waitData("Link is builded");
    */
}

