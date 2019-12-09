#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif


#include "wifi.h"

#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <String.h>
#include "ESPAsyncWebServer.h"
#include "html.h"
#include "AsyncJson.h"
#include "json.h"
#include "Sim800L.h"
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include "ready.h"

String ssid = "SMS-WifiConfig";
String pass = "";


AsyncWebServer Aserver(80);

const char* PARAM_MESSAGE = "message";

void wificfReqHandle(void)
{
  Aserver.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request){
    int n = WiFi.scanNetworks();
    String st;
    for (int i=0; i < n; ++i)
    {
      st+="<option value=\""+String(WiFi.SSID(i))+"\">"+String(WiFi.SSID(i))+"</option>";
    }
    request->send(200, "text/html", pageWifiA+st+pageWifiB);
  });
}
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/html", page404);
}
void wifiReqHandle(void)
{
  Aserver.on("/cf", HTTP_GET, [](AsyncWebServerRequest *request){
      ssid = request->getParam("ssid")->value();
      pass = request->getParam("pass")->value();
      request->send(200, "text/html", pageRestart);
      //request->redirect("http://"+WiFi.localIP()+"/wifi");
      vTaskDelay(2000/portTICK_PERIOD_MS);
      //Serial.println(ssid);
      //Serial.println(pass);
      //WiFi.end();
      WiFi.begin(ssid.c_str(), pass.c_str());
      //Serial.println("CONNECTING TO "+ssid);
      vTaskDelay(2000/portTICK_PERIOD_MS);
      while (WiFi.status() != WL_CONNECTED) {
//        //delay(500);        
      }
      //delay(5000);
      
      ESP.restart();
    });
}
bool busy=0;
void smsHandle(void)
{
//  Aserver.on("/sms", HTTP_GET, [](AsyncWebServerRequest *request){
//
//        String message;
//        if (request->hasParam("msg")) {
//            message = request->getParam("msg")->value();
//        } else {
//            message = "No message sent";
//        }
//        request->send(200, "text/plain", "Hello, GET: " + message);
//
//    });

  Aserver.on("/sms",HTTP_POST,[](AsyncWebServerRequest * request){},NULL,
  [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
      //String str = String((char*)data);
      while ((busy)||(getSIMReady())) vTaskDelay(10);
      setnotSIMReady();
      busy =1;
      String to;
      String text;
      int function;
      String messageId;
      jsonDeserialize((char*) data, to, text, function, messageId);
      if ((function==101)&&((to.length()==10)||(to.length()==13&&(to.indexOf("+84")!=-1)))/*&&(text.length()<=255)*/)
      {
        
        request->send(200,"text/plain",jsonSerialize(to,200,"SUCCESS",messageId));
        if (smsSend(to, text))
        {
            //vTaskDelay(200/portTICK_PERIOD_MS);
//            HTTPClient http;
//            http.begin("http:192.168.3.129/statusSMS"); //HTTP connection
//            int httpCode = http.POST(jsonSerialize(to,201,"SENT",messageId));
//            http.end();  
//---------------------------------------------
            Serial.println("SENT");
//            HTTPClient http;   
//            //vTaskDelay(200/portTICK_PERIOD_MS);
//            delay(200);
//            Serial.println("OK1");
//            http.begin("http://192.168.3.129:5003/statusSMS");  //Specify destination for HTTP request
//            //vTaskDelay(200/portTICK_PERIOD_MS);
//            delay(200);
//            Serial.println("OK2");
//            http.addHeader("Content-Type", "application/json");    
//            delay(200);
//            Serial.println("OK3");//Specify content-type header
//            //vTaskDelay(200/portTICK_PERIOD_MS);
//            int httpResponseCode = http.POST(jsonSerialize(to,201,"SENT",messageId));   //Send the actual POST request
//            delay(200);
//            Serial.println("OK4");
            //vTaskDelay(200/portTICK_PERIOD_MS);
            //http.end();  
        }
        else 
        {
//            vTaskDelay(200/portTICK_PERIOD_MS);
//---------------------------------------------
//            HTTPClient http;
//            http.begin("http://192.168.3.129/statusSMS"); //HTTP connection
//            int httpCode = http.POST(jsonSerialize(to,401,"CANT_SEND",messageId));
//            http.end();  
            
//            delay(200);
            Serial.println("CAN'T SEND");
//            HTTPClient http; 
//            vTaskDelay(200/portTICK_PERIOD_MS); 
//            http.begin("http://192.168.3.129:5003/statusSMS");  //Specify destination for HTTP request
//            vTaskDelay(200/portTICK_PERIOD_MS);
//            http.addHeader("Content-Type", "application/json");             //Specify content-type header
//            vTaskDelay(200/portTICK_PERIOD_MS);
//            int httpResponseCode = http.POST(jsonSerialize(to,401,"CANT_SEND",messageId));   //Send the actual POST request
//            vTaskDelay(200/portTICK_PERIOD_MS);
//            //http.end();  
//            udp.beginPacket("http://192.168.3.129/statusSMS", "5003");
//            udp.write(buffer, 11);
//            udp.endPacket();
        }
      }
      else
      {
        request->send(200,"text/plain",jsonSerialize(to,400,"BAD_REQUEST",messageId));
      }
      busy =0;
      setSIMReady();
//      Serial.println(to);
//      Serial.println(text);
//      Serial.println(function);
//      Serial.println(messageId);
      
  });


}

void checkbalanceHandle(void)
{
      Aserver.on("/check",HTTP_POST,[](AsyncWebServerRequest * request){},NULL,
      [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
      //String str = String((char*)data);
      
      int function;
      cmdjsonDeserialize((char*) data, function);
      if ((function==102))
      {
        long inc = balanceCheck();
        Serial.println(inc);
        //delay(200);
        if (inc >=0)
        {
            request->send(200,"text/plain","{\"status\": 200,\"balance\": \""+String(inc)+"\"}");
        }
        else
        {
            request->send(200,"text/plain","{\"status\": 500}");
        }
      }
      else
      {
        request->send(200,"text/plain","{\"status\": 500}");
      }

      
  });
}

void balanceincHandle(void)
{
      Aserver.on("/inc",HTTP_POST,[](AsyncWebServerRequest * request){},NULL,
      [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
      //String str = String((char*)data);
      
      int function;
      String seriNumber;
      incjsonDeserializz((char*) data,function, seriNumber);
      if ((function==103))
      {
        if (balanceInc(seriNumber))
        {
            request->send(200,"text/plain","{\"status\": 200}");
        }
        else
        {
            request->send(200,"text/plain","{\"status\": 500}");
        }
       
      }
      else
      {
        request->send(200,"text/plain","{\"status\": 500}");
      }    
  });
}


void ApMode(void)
{
  WiFi.mode(WIFI_MODE_AP);
  ssid += String(random(0xffff), HEX);
  WiFi.softAP(ssid.c_str(), pass.c_str());
  Serial.println("AP Mode");
}

bool wifiInit(void)
{
  WiFi.begin();
  long t = millis();
  while (WiFi.status() != WL_CONNECTED) {
      //delay(500);
      if (millis()-t>20000) 
      {
        ApMode();
        break;
      }
  }
  Serial.println("CONNECTED to: "+WiFi.SSID());
  if (!MDNS.begin("sms")) return 0;
  
//----------------------------------------------
//HTTP Req Handle
  wificfReqHandle();
  wifiReqHandle();
  smsHandle();
  balanceincHandle();
  checkbalanceHandle();
//----------------------------------------------  
  Aserver.onNotFound(notFound);
  Aserver.begin();
  MDNS.addService("http", "tcp", 80);
  return 1;
}
