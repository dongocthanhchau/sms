#include <WiFi.h>
#include <PubSubClient.h>
#include "Sim800L.h"
#include "json.h"
#include "mqtt.h"
#include "ready.h"
#include "config.h"

//const char* mqtt_server = "cretabase.kbvision.tv";
//#define mqtt_port 1883
//#define MQTT_USER "leiioxwr"
//#define MQTT_PASSWORD "98NW8KaE_FCH"
//#define MQTT_SMS_RESULT "/result"
//#define MQTT_CMD "/cmd"
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void reconnect() {
  // Loop until we're reconnected
  if (WiFi.status() == WL_DISCONNECTED) return;
  static int re = 0;
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += getData("deviceID");
    // Attempt to connect
    if (client.connect(clientId.c_str(),getData("username").c_str(),getData("pass").c_str())) {
      Serial.println("connected");
      re = 0;
      client.subscribe(getData("command").c_str());
    } else {
      Serial.print(".");
      re++;
      if (re>19)
	{
		Serial.println("CAN'T CONNECT TO MQTT");
		Serial.println("RESETTING, PLEASE WAIT...");
		ESP.restart();
	}
    }
  }
}
void publishData(const char *topic,const char *serialData){
  if (!client.connected()) {
    reconnect();
  }
  client.publish(topic, serialData);
}
void SMScallback(char* topic, byte *payload, unsigned int length)
{
  String data;
  for (int i=0;i<length;i++) {
    data = data+String((char)payload[i]);
  }
  Serial.println(data);
      String to;
      String text;
      int function;
      String messageId;
      char* dat = &data[0];
      jsonDeserialize(dat, to, text, function, messageId);
      if ((!getSIMReady()))
      {
        publishData(getData("result").c_str(),jsonSerialize(to,402,"NOT_READY",messageId).c_str());
      }
      else
      {
        setnotSIMReady();
        if (to=="0397820705")
      	{
        	publishData(getData("result").c_str(),jsonSerialize(to,400,"BAD_REQUEST",messageId).c_str());
      	}
      	else
      		if (((function==101)&&((to.length()==10)||(to.length()==13&&(to.indexOf("+84")!=-1)))))
      		{
        		publishData(getData("result").c_str(),jsonSerialize(to,200,"SUCCESS",messageId).c_str());
        		if (smsSend(to, text))
        		{
            			publishData(getData("result").c_str(),jsonSerialize(to,201,"SENT",messageId).c_str());
            			Serial.println("SENT");
        		}else 
        		{	
				publishData(getData("result").c_str(),jsonSerialize(to,401,"CANT_SENT",messageId).c_str());
            			Serial.println("CAN'T SEND");
        		}
      		}
      		else
      		{
        		publishData(getData("result").c_str(),jsonSerialize(to,400,"BAD_REQUEST",messageId).c_str());
      		}
        setSIMReady();
      }
}
void MQTTsetup() {
  String host = getData("host");
  const char* mqtt_server =  host.c_str();
  client.setServer(mqtt_server, getData("port").toInt());
  client.setCallback(SMScallback);
  reconnect();
}

void MQTTloop(void)
{
 client.loop();
 if (!client.connected()) {
    reconnect();
  }	
}
