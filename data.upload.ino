#include <stdlib.h>
#include <SoftwareSerial.h>
#include <ESP8266.h>
#define WIFI_TX 9
#define WIFI_RX 8
int min=15;
int max=35;
#define SERIAL_TX_BUFFER_SIZE 2048
#define SERIAL_RX_BUFFER_SIZE 2018
String _comdata_wifi = "";
String wifiName = "SRL";
String wifiPassword ="12345678";
char tempChar;
boolean DebugStart = true;
boolean RST = true;
boolean CWMODE = true;
boolean WifiStart =true;
SoftwareSerial mywifi(WIFI_RX,WIFI_TX);
#include <dht11.h>                                               //引用dht11库文件，使得下面可以调用相关参数
#define DHT11PIN A2                                             //定义温湿度针脚号为2号引脚
dht11 DHT11;
int tem,hum,light;
#define EspSerial mywifi
ESP8266 wifi(&EspSerial);

#define HOST_NAME   "jjfaedp.hedevice.com"
#define DEVICEID   "505475713"//设备ID，改
#define PROJECTID "194070"//项目ID，改
#define HOST_PORT   (876)
String jsonToSend;
char  tem_c[7], hum_c[7], light_c[7] ;    //换成char数组传输
String postString;
String apiKey="jPbDdOGFkImaYZvfgb4aurN1asE=";//API密匙
char buf[10];

void updateSensorData() {
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
  
    Serial.print("create tcp ok\r\n");//tcp链接已经用AT指令的方式创建，具体格式参照官方文档

jsonToSend="{\"Temperature\":";
    dtostrf(tem,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"Humidity\":";
    dtostrf(hum,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"Light\":";
    dtostrf(light,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+="}";



   postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
    postString+="\r\n";


  const char *postArray = postString.c_str();                 //将str转化为char数组
  Serial.println(postArray);
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println("send success");   
     if (wifi.releaseTCP()) {                                 //释放TCP连接
        Serial.print("release tcp ok\r\n");
        } 
     else {
        Serial.print("release tcp err\r\n");
        }
      postArray = NULL;                                       //清空数组，等待下次传输数据
  
  } else {
    Serial.print("create tcp err\r\n");
  }
  
  delay(500);
}








void sendDebug(String cmd)
{
  Serial.print("指令输入：");

  mywifi.println(cmd);
  }
boolean connectWiFi(String Name,String Password)
  {
    String cmd="AT+CWJAP=\"";
    cmd+=Name;
    cmd+="\",\"";
    cmd+=Password;
    cmd+="\"";
    mywifi.println(cmd);
    delay(2000);
 }
 void getWifiSerialData(){
  while(mywifi.available()>0){
    tempChar = mywifi.read();
    delay(10);
    _comdata_wifi += char(tempChar);
    delay(10);
    }
}
void setup()
{
  mywifi.begin(9600);
  Serial.begin(9600);
  pinMode(DHT11PIN,OUTPUT);                       //定义输出口
    pinMode(10,OUTPUT);
    pinMode(11,OUTPUT);
    
  delay(500);
 
}
void loop()
{
  
  sendDebug("AT");
   getWifiSerialData();
  if(_comdata_wifi!=""&DebugStart)
  {
    Serial.println(_comdata_wifi);
    delay(1000);
    if(_comdata_wifi.indexOf("OK"))
    {
      Serial.println("收到OK，测试指令成功。开始连接WIFI");
      delay(1000);
      
      sendDebug("AT+RST");

      }else{
        Serial.println("未收到OK，测试指令不成功成功。无法连接WIFI");
      }
      _comdata_wifi=String("");
   }
  
    Serial.println(_comdata_wifi);
    delay(1000);
    if(_comdata_wifi.indexOf("OK"))
    {
      Serial.println("收到OK，重置指令成功。开始选择模式");
      delay(1000);
      sendDebug("AT+CWMODE=3");

      }else{
        Serial.println("未收到OK，重置指令不成功成功。无法连接WIFI");
      }
      _comdata_wifi=String("");
    
    Serial.println(_comdata_wifi);
    delay(1000);
    if(_comdata_wifi.indexOf("OK"))
    {
      Serial.println("收到OK，模式指令成功。开始连接WIFI");
      delay(1000);
        connectWiFi(wifiName,wifiPassword);

      }else{
        Serial.println("未收到OK，模式指令不成功成功。无法连接WIFI");
      }
      _comdata_wifi=String("");
  
   
   if(_comdata_wifi!="" & WifiStart)
   {
    Serial.println(_comdata_wifi);
    delay(1000);
    if(_comdata_wifi.indexOf("WIFI GOT IP")){
      Serial.println("接收到WIFI GOT IP,WIFI 连接成功");
      delay(1000);
      }else{
        Serial.println("WIFI 连接不成功");
        delay(1000);
        }
        _comdata_wifi=String("");
   }
  /* if(_comdata_wifi!="")
   {
    Serial.println(_comdata_wifi);
    delay(1000);
    if(_comdata_wifi.indexOf("OK")){
      Serial.println("开始");
      sendDebug("+++");
      delay(1000);
      }else{
        Serial.println("FAIL");
        delay(1000);
        }
        _comdata_wifi=String("");
   }*/
    if(_comdata_wifi!="")
   {
    Serial.println(_comdata_wifi);
    delay(1000);
    if(_comdata_wifi.indexOf("OK")){
      Serial.println("开始");
      sendDebug("AT+CIFSR");
      delay(1000);
      }else{
        Serial.println("FAIL");
        delay(1000);
        }
        _comdata_wifi=String("");
   }
    if(_comdata_wifi!="")
   {
    Serial.println(_comdata_wifi);
    delay(1000);
    if(_comdata_wifi.indexOf("OK")){
      Serial.println("开始");
      sendDebug("AT+CIPMUX=0");
      delay(1000);
      }else{
        Serial.println("FAIL");
        delay(1000);
        }
        _comdata_wifi=String("");
   }
    if(_comdata_wifi!="")
   {
    Serial.println(_comdata_wifi);
    delay(1000);
    if(_comdata_wifi.indexOf("OK")){
      Serial.println("开始");
      sendDebug("AT+CIPMODE=1");
      delay(1000);
      }else{
        Serial.println("FAIL");
        delay(1000);
        }
        _comdata_wifi=String("");
   }
    if(_comdata_wifi!="")
   {
    Serial.println(_comdata_wifi);
    delay(1000);
    if(_comdata_wifi.indexOf("OK")){
      Serial.println("开始");
      sendDebug("AT+CIPSTART=\"TCP\",\"jjfaedp.hedevice.com\",876\r\n");
      delay(1000);
      }else{
        Serial.println("FAIL");
        delay(1000);
        }
        _comdata_wifi=String("");
   }
    if(_comdata_wifi!="")
   {
    Serial.println(_comdata_wifi);
    delay(1000);
    if(_comdata_wifi.indexOf("OK")){
      Serial.println("开始");
      sendDebug("AT+CIPSEND\r\n");
      delay(1000);
      }else{
        Serial.println("FAIL");
        delay(1000);
        }
        _comdata_wifi=String("");
   }


   
   int chk = DHT11.read(DHT11PIN);                 //将读取到的值赋给chk

 tem=(float)DHT11.temperature;               //将温度值赋值给tem
 hum=(float)DHT11.humidity;                   //将湿度值赋给hum
Serial.print("Tempeature:");                        //打印出Tempeature:
Serial.println(tem);                                     //打印温度结果

Serial.print("Humidity:");                            //打印出Humidity:
Serial.print(hum);                                     //打印出湿度结果
Serial.println("%");                                 //打印出%

delay(1000);  
      light = analogRead(A3);   
   Serial.println(light);
  {
     if (tem>=min&&tem<max )
    { 
        digitalWrite(11,0);
        digitalWrite(10,1);
    }
    else if(tem>=max)
    {
        digitalWrite(11,1);
        digitalWrite(10,0);
    }
    else
        {digitalWrite(11,0);
        digitalWrite(10,0); }
  }
    
    delay(100);       //防止串口写入速度过快


    /*WIFI
    Serial.println("exit pass-through mode");//每次循环重新初始化ESP8266
mywifi.print("+++");
digitalWrite(4,HIGH);//发送一条指令，闪一下灯，说明有数据传输
delay(100);
digitalWrite(4,LOW);
delay(50);//根据不同种类的指令类型，要留出不同的时间等待完成
mywifi.print("AT+CWMODE=3\r\n");//设置模式3，STA+AP，其实我们只用了STA模式
digitalWrite(4,HIGH);
delay(100);
digitalWrite(4,LOW);
delay(100);
mywifi.print("AT+RST\r\n");//让模块复位，能在接收信息最下面看到 ready，说明模块复位成功。复位后就是STA+AP模式了
digitalWrite(4,HIGH);
delay(100);
digitalWrite(4,LOW);
delay(100);
mywifi.print("AT+CWJAP=\"ohfuck\",\"shalebaji\"\r\n");//输入你无线路由器的用户名和密码，一定要写正确，注意大小写
digitalWrite(4,HIGH);
delay(100);
digitalWrite(4,LOW);
delay(1000);*/




      /*TCP*/
/*mywifi.print("AT+CIFSR\r\n");//查询IP，因为有STA和AP两个模式，所以有两组MAC和IP，常见的是192.168.x.x局域网IP
digitalWrite(4,HIGH);
delay(100);
digitalWrite(4,LOW);
delay(100);
mywifi.print("AT+CIPMUX=0\r\n");//设置单链接，透传只能在单连接模式下进行，所以在建立连接之前一定要用（AT+CIPMUX=0 设置单连接）
digitalWrite(4,HIGH);
delay(100);
digitalWrite(4,LOW);
delay(100);
mywifi.print("AT+CIPMODE=1\r\n");//设置透传模式，ESP8266要求必须透传
digitalWrite(4,HIGH);
delay(100);
digitalWrite(4,LOW);
Serial.println("TCP connect begin");
mywifi.print("AT+CIPSTART=\"TCP\",\"183.230.40.33\",80\r\n");//创建TCP链接，具体格式参照官方文档
digitalWrite(4,HIGH);
delay(100);
digitalWrite(4,LOW);
delay(200);
Serial.println("TCP connect begin");
digitalWrite(4,HIGH);
delay(100);
digitalWrite(4,LOW);
delay(500);
mywifi.print("AT+CIPSEND\r\n");//启动透传发送，ESP8266上传准备完成，利用AT指令只能完成到这一步
digitalWrite(4,HIGH);
delay(100);
digitalWrite(4,LOW);
delay(50);*/
Serial.println("uploading data......");
updateSensorData();
delay(1000);


    
}
   
