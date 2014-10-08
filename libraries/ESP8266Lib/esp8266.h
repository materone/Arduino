/*=======================================================================
 ESP8266 WiFi Module by UART for Arduino.

 Author:OpenThings@163.com,
 Last Modified: 2014.09.25.

 获取最新版本：
	git clone https://git.oschina.net/supergis/ESP8266Lib.git
 在用于Arduino软件串口时，波特率只支持9600版本，并且需要独立供电（3.3V,300MA）。 
 =========================================================================*/

#ifndef __ESP8266SERIAL_H__
#define __ESP8266SERIAL_H__
#include <Arduino.h>
#include "SoftSerial.h"

#define    OPEN          0
#define    WEP           1
#define    WAP_PSK       2
#define    WAP2_PSK      3
#define    WAP_WAP2_PSK  4

#define    TCP     1
#define    tcp     1
#define    UDP     0
#define    udp     0

#define    OPEN    1
#define    CLOSE   0

#define    STA     1
#define    AP      2
#define    AT+STA  3

#define SERIAL_TX_BUFFER_SIZE 128
#define SERIAL_RX_BUFFER_SIZE 128

//===================================================
#define  ESP8266_AT    //Select Model of Module.

#define T_OK            "OK"
#define T_OK_END         "\r\nOK"
#define AT 				"AT"		//OK：测试是否正常运行。
#define AT_GMR		"AT+GMR"	//OK：查看固件版本。

/*===================================================
 ESP8266,官方AT指令表：
*/
#ifdef ESP8266_AT
#define AT_RST		"AT+RST"	//OK,ready：重新启动。

//返回信息标识码
#define T_OK 			"OK"
#define T_DONE			"done"
#define T_ERROR			"error"
#define T_LINKOK		"link is builded"
#define T_ONE			"one"
#define T_LINKED		"linked"
#define T_NOLINK		"link is not"
#define T_CANTCLOSE		"Cant close"
#define T_RESTART		"we must restart"
#define T_UNLINK		"unlink"
#define T_SENDOK		"OK"
#define T_NOT			"not"
#define T_CONNECTED		"ALEAY CONNECT"
#define T_LINKTYPEERROR	"Linktype ERROR"
#define T_NOCHANGE		"no change"
#define T_READY			"ready!!"

//连接到无线接入点
#define AT_CWMODE_GET	"AT+CWMODE?"	//OK：选择WiFi模式，1为网卡，2为AP，3为网卡＋AP。
#define AT_CWMODE_SET	"AT+CWMODE="	//1/2/3=>OK
#define AT_CWMODE_STA	"AT+CWMODE=1"	//1/2/3=>OK
#define AT_CWMODE_AP	"AT+CWMODE=2"	//1/2/3=>OK
#define AT_CWMODE_ALL	"AT+CWMODE=3"	//1/2/3=>OK
#define	AT_CWJAP_GET	"AT+CWJAP?"	//OK：当前连接到的AP。
#define AT_CWLAP		"AT+CWLAP"	//OK：列出可用的AP。
#define AT_CWJAP_SET	"AT+CWJAP="	//"SSID","PWD"＝>OK：连接无线接入点。
#define AT_CWQAP		"AT+CWQAP"	//OK：退出与AP的连接。

//作为接入点，设置信息。
#define AT_CWSAP_GET	"AT+CWSAP?"	//AP接入点参数查询。
#define AT_CWSAP_SET	"AT+CWSAP="	//"SSID","PWD",6,4=>OK：作为AP的接入点参数。

//建立TCP/IP网络连接。 
#define AT_CIPSTART		"AT+CIPSTART="	//"UDP","IP",Port=>OK：建立TCP/UDP连接。
#define AT_CIFSR  		"AT+CIFSR"	//获取本模块的IP地址。
#define AT_CIPSTATUS	"AT+CIPSTATUS"	//OK，获得当前连接状态。

//发送数据。
#define AT_CIPSEND		"AT+CIPSEND="	//count=>">"：设置发送区宽度。
// +IPD 接收到的网络数据。
//    1)单路连接时 (+CIPMUX=0) +IPD,<len>:<data>
//    2)多路连接时 (+CIPMUX=1) +IPD,<id>,<len>:<data>

//断开连接。					// ">"Msg => SEND OK：发送字符。
#define AT_CIPCLOSE	        "AT+CIPCLOSE"	//OK,Unlink：断开连接。
#define AT_CIPCLOSEMUX  	"AT+CIPCLOSE="	//OK,Unlink：断开连接。

//建立IPServer，接收外部客户端连接。
#define AT_CIPMUX_GET	"AT+CIPMUX?"	  //OK：查询多连接设置.
#define AT_CIPMUX_SET	"AT+CIPMUX="	//OK：启用多连接，后续连接时需指明连接ID
#define AT_CIPMUX0		"AT+CIPMUX=0"	//OK：启用多连接，后续连接时需指明连接ID.
#define AT_CIPMUX1		"AT+CIPMUX=1"	//OK：启用多连接，后续连接时需指明连接ID
#define AT_CIPSERVER	"AT+CIPSERVER="	                //0/1，Port>OK：设置为服务器。
#define AT_CIPSERVER_DEFAULT	"AT+CIPSERVER=1,8888"	//0/1，Port>OK：设置为服务器。
#define AT_CIPMODE		"AT+CIPMODE="	//0/1=>OK：设置模块传输模式，1为透传。
#define AT_CIPSTO		"AT+CIPSTO="	//T(0~28800)=>OK：设置服务器超时时间。
#endif
//===================================

/*==================================================== 
 透传模式：
 AT+CIPSTART="UDP","IP",Port
 AT+CIPMODE=1=>OK,
 AT＋CIPSEND >
 发送单个数据包+++退出透传模式。
=====================================================*/

/*===================================================
 ESP8266,Ocrobot版，AT指令表：
*/
#ifdef ESP8266_AT_OCROBOT
#define AT_RST		"AT+Reset"	//OK,ready：重新启动。

//返回信息标识码
#define T_DONE			"done"
#define T_ERROR			"RROR"
#define T_LINKOK		"Link is builded"
#define T_ONE			"one"
#define T_LINKED		"Linked"
#define T_NOLINK		"Link is not"
#define T_CANTCLOSE		"Cant close"
#define T_RESTART		"we must restart"
#define T_UNLINK		"Unlink"
#define T_SENDOK		"SEND OK"
#define T_NOT			"not"
#define T_CONNECTED		"ALEAY CONNECT"
#define T_LINKTYPEERROR	"Linktyp ERROR"
#define T_NOCHANGE		"nochange"
#define T_READY			"OCROBOT WIFI ready!!!"

//连接到无线接入点
#define AT_CWMODE_GET	"AT+MODE?"	//OK：选择WiFi模式，1为网卡，2为AP，3为网卡＋AP。
#define AT_CWMODE_SET	"AT+MODE="	//1/2/3=>OK
#define	AT_CWJAP_GET	"AT+JoinAP?"	//OK：当前连接到的AP。
#define AT_CWLAP		"AT+ShowAP"	//OK：列出可用的AP。
#define AT_CWJAP_SET	"AT+JoinAP="	//"SSID","PWD"＝>OK：连接无线接入点。
#define AT_CWQAP		"AT+ExtAP"	//OK：退出与AP的连接。

//作为接入点，设置信息。
#define AT_CWSAP		"AT+SAP?"	//查询当前AP的配置。
#define AT_CWSAP		"AT+SAP="	//"SSID","PWD",6,4=>OK：作为AP的接入点参数。

//建立TCP/IP网络连接。 
#define AT_CIPSTART		"AT+NewSTA="	//"UDP","IP",Port=>OK：建立TCP/UDP连接。
#define AT_CIFSR		"AT+ShowIP"	//获取本模块的IP地址。
#define AT_CIPSTATUS	"AT+ShowSTA"	//OK，获得当前连接状态。

//发送数据。
#define AT_CIPSEND		"AT+Update="	//count=>">"：设置发送区宽度。
// +IPD 接收到的网络数据。
//    1)单路连接时 (+CIPMUX=0) +IPD,<len>:<data>
//    2)多路连接时 (+CIPMUX=1) +IPD,<id>,<len>:<data>

//断开连接。					// ">"Msg => SEND OK：发送字符。
#define AT_CIPCLOSE		"AT+CLOSE"	//OK,Unlink：断开连接。
#define AT_CIPCLOSE_MUX	"AT+CLOSE="	//OK,Unlink：断开连接。

//建立IPServer，接收外部客户端连接。
#define AT_CIPMUX_GET	"AT+MUX?"	//OK：查询多连接设置.
#define AT_CIPMUX		"AT+MUX="	//OK：启用多连接，后续连接时需指明连接ID.
#define AT_CIPSERVER	"AT+Server"	//0/1，Port>OK：设置为服务器。
#define AT_CIPMODE		"AT+CIPMODE="	//0/1=>OK：设置模块传输模式，1为透传。
#define AT_CIPSTO		"AT+CIPSTO="	//T(0~28800)=>OK：设置服务器超时时间。
#endif
//=======================================

class esp8266
{
  public:
    unsigned long timeFree,timeLast;
    const static unsigned long timeInterval = 10000;
    const static int pin_ENABLE = 8;
    const static int pin_RESET = 9;
    String data_rev,data_uart;
    const static int data_max = 128;
    
     esp8266(void);
	//String begin(void);
     //SoftSerial espSerial;
    bool init(long bandrate,uint8_t rx=10,uint8_t tx=11);
    void GetInfo();
    void bridge();
    void xCommander();
 	void onCommand();
 	bool xReceiver();
    String onReceive();       
 
    void startClient(String ipServer,long nPort);
    void startClientUDP(String ipServer,long nPort);
    void startServer(String ap,String pwd,long nPort=8888);
    
    //=====================
    bool sendAT(String strAT);
    
    //=====================
    void runAT(String strAT);
    void runAT_End(String strAT);
    
    String waitData(String Tag1, String Tag2="",String Tag3="",String Tag4="");    
    String waitOK();
    String waitInput();
    String waitSendFinish();
    
    /*=================esp8266serial功能指令=================*/
    String isOK();
    String Version(void);
    void Reset(void);    //重启模块
    void HardReset(void);

    String showMode(void);   //查询模块目前模式
    void configMode(int a);   //设置模块的模式
    String showAP(void);   //返回esp8266serial列表
    String showJAP(void);  //返回目前连接的esp8266serial接入点信息
    void configJAP(String ssid , String pwd);    //配置登陆esp8266serial接入点
    void quitAP(void);    //退出当前esp8266serial连接
    
    //显示ap模式下的模块的ssid 密码 通道号 以及加密方式
    String showSAP(void);
    //配置ap模式下的网络参数 ssid 密码 通道号 以及加密方式
    void configSAP(String ssid , String pwd , byte chl , byte ecn);

    /*================TCP/IP指令================*/
    String showStatus(void);    //查询链接状态
    String showMux(void);       //查询目前的链接模式（单链接or多链接）
    void configMux(int a);    //设置链接模式（单链接【0】or多链接【1】）
    //创建 tcp或者udp链接（单路模式)
    void connectOne(String type, String addr, int port);   
    //创建 tcp或者udp链接（多路模式)（id:0-4） 0号可以连接客户端和服务器，其他只能连接服务器
    void connectMux(byte id, String type, String addr, int port); 
    void Send(String str);  //单路模式发送数据
    void Send(int id, String str);  //多路模式发送数据
    void closeOne(void);   //关闭tcp或udp （单路模式）
    void closeMux(byte id); //关闭tcp货udp （多路模式）
    String showIP();    //显示本机目前获取的ip地址
    
    void startServer(int port=8888);  //配置服务器
    void stopServer(int port=8888);  //配置服务器

};

#endif

