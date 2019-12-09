#include <Arduino.h>
#include <Wire.h>
#include <sim800.h>
#include "Sim800L.h"
#include <gprs.h>
#include "ready.h"

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define T_CALL

#if defined(T_CALL)
#define UART_TX                     27
#define UART_RX                     26
#define SIMCARD_RST                 5
#define SIMCARD_PWKEY               4
#define SIM800_POWER_ON             23
#else
#define UART_TX                     33
#define UART_RX                     34
#define SIMCARD_RST                 14
#define SIMCARD_PWKEY               15
#define SIM800_POWER_ON             4
#endif

#define UART_BANUD_RATE             9600

#define I2C_SDA                     21
#define I2C_SCL                     22

#define IP5306_ADDR                 0X75
#define IP5306_REG_SYS_CTL0         0x00

HardwareSerial hwSerial(1);
GPRS gprs(hwSerial, SIMCARD_PWKEY, SIMCARD_RST, SIM800_POWER_ON);

bool setPowerBoostKeepOn(int en)
{
    Wire.beginTransmission(IP5306_ADDR);
    Wire.write(IP5306_REG_SYS_CTL0);
    if (en)
        Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
    else
        Wire.write(0x35); // 0x37 is default reg value
    return Wire.endTransmission() == 0;
}

bool sim800Init(void)
{
  setnotSIMReady();
#if defined(T_CALL)
    Wire.begin(I2C_SDA, I2C_SCL);
    bool   isOk = setPowerBoostKeepOn(1);
#endif
    hwSerial.begin(UART_BANUD_RATE, SERIAL_8N1, UART_RX, UART_TX);
    if (!gprs.preInit()) {   
      //Init Failed
       return(0);   
    }
    
    int i = 3;
    while (i--) {
        hwSerial.print("AT+SPWM=0,1000,1\r\n");
        vTaskDelay(2000/portTICK_PERIOD_MS);
        hwSerial.print("AT+SPWM=0,0,0\r\n");
    }

    vTaskDelay(200/portTICK_PERIOD_MS);

    if (0 != gprs.init()) {
        //No SIM Card
        return(0);
    }

    // Switch audio channels
    hwSerial.print("AT+CHFA=1\r\n");
    vTaskDelay(5/portTICK_PERIOD_MS);
    hwSerial.print("AT+CRSL=100\r\n");
    vTaskDelay(5/portTICK_PERIOD_MS);
    hwSerial.print("AT+CLVL=100\r\n");
    vTaskDelay(5/portTICK_PERIOD_MS);
    hwSerial.print("AT+CLIP=1\r\n");
    vTaskDelay(2000/portTICK_PERIOD_MS);
    //Clear Buffer
    hwSerial.flush();
    //All Success
    hwSerial.setTimeout(100);
    setSIMReady();
    return(1);
}
bool isCallIn = false;
String getSim800(void)
{
  if (hwSerial.available()) {
        const char *s = hwSerial.readStringUntil('\n').c_str();
        return(s);
   }
}
bool balanceInc(String code)
{
  
  hwSerial.flush();
  setnotSIMReady();
  Serial.println("Processing...");
  
  hwSerial.print("AT+CUSD=1\r\n");
  vTaskDelay(100/portTICK_PERIOD_MS);
  hwSerial.setTimeout(100);
  int count=0;
  while (!hwSerial.find("OK"))
  {
    count++;
    vTaskDelay(150/portTICK_PERIOD_MS);
    if (count>20) return 0;
  }
  
  String str;
  hwSerial.print("AT+CUSD=1,\"*100*"+code+"#\"\r\n");
  vTaskDelay(2000/portTICK_PERIOD_MS);
  hwSerial.print("AT+CUSD=2\r\n");
  vTaskDelay(2000/portTICK_PERIOD_MS);
  if (!hwSerial.find("OK")) return 0;
  hwSerial.flush();
  setSIMReady();
  return 1;
}
long balanceCheck(void)
{
  hwSerial.flush();
  setnotSIMReady();
  Serial.println("Checking...");
  
  hwSerial.print("AT+CUSD=1\r\n");
  int count=0;
  hwSerial.setTimeout(100);
  while (!hwSerial.find("OK"))
  {
    count++;
    vTaskDelay(150/portTICK_PERIOD_MS);
    if (count>20) return -1;
  }
  
  String str;
  hwSerial.print("AT+CUSD=1,\"*101#\"\r\n");
  
  /////----------------------------------------------------
  //String Processing
  count=0;
  
  while (!hwSerial.find("TKG"))
  {
    count++;
    vTaskDelay(150/portTICK_PERIOD_MS);
    if (count>20) return -1;
  }
  
  if (hwSerial.available())
     {
        str = hwSerial.readString();
        str.remove(0, 2);
        str.remove(str.indexOf("d,"));
     }
   else return -1;  
  //Serial.println("Checking...");
  hwSerial.print("AT+CUSD=2\r\n");
  //Serial.println("Checking...");
  //String st = hwSerial.readString();
     //strncpy(str,str,strstr(str,"d,")
  //delay(500);
  //vTaskDelay(500/portTICK_PERIOD_MS);
  //Serial.println("Checking.OK..");
  count=0;
  hwSerial.setTimeout(100);
  while (!hwSerial.find("OK"))
  {
    count++;
    vTaskDelay(150/portTICK_PERIOD_MS);
    if (count>20) return -1;
  }
  hwSerial.flush();
  setSIMReady();
  return str.toInt();
  //--------------------------------------------------------
}
bool smsSend(String num, String msg)
{
  setnotSIMReady();
  //hwSerial.flush();
  delay(1000);
  if (num.indexOf("0")==0) num.remove(0, 1);
  if (num.indexOf("+84")==0) num.remove(0, 3);
  hwSerial.println("AT+CMGF=1"); // Configuring TEXT mode
  //Wait until OK 
  while (!hwSerial.find("OK"));
  //Send num data
  hwSerial.println("AT+CMGS=\"+84"+num+"\"");
  delay(200);
  //Send SMS data
  String st="";
  //Serial.println("SMS0");
  while (msg.length()>100)
  {
      st = msg.substring(0,100);
      //Serial.println(st);
      hwSerial.print(st);
      msg.remove(0,100);
      delay(200);  
  }
  hwSerial.print(msg);
  delay(200);

  //Send End Transmition
  //Serial.println("SMS1");
  hwSerial.write(26);
  while (!hwSerial.available())
      delay(1000);

      
  hwSerial.setTimeout(500);
  int count = 1;
  if (!hwSerial.find("OK")) 
  {
    //hwSerial.flush();
    setSIMReady();
    count++;
    if (count > 4) return 0;
  }
  else
  {
    //hwSerial.flush();
    setSIMReady();
    return 1;
  }
  //Serial.println("SMS2");
//  {
//    vTaskDelay(150/portTICK_PERIOD_MS);
//    count++;
//    if (count>20) return 0;
//  }
}
