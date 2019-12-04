#include <WiFi.h>
#include <PubSubClient.h>
#include "Sim800L.h"
#include "json.h"
#include "mqtt.h"


const char* mqtt_server = "cretabase.kbvision.tv";
#define mqtt_port 1883
#define MQTT_USER "minh"
#define MQTT_PASSWORD "123"
#define MQTT_SMS_RESULT "/result"
#define MQTT_CMD "/cmd"
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("connected");
      client.subscribe(MQTT_CMD);
    } else {
      Serial.print(".");
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
  //Serial.println(data);
      String to;
      String text;
      int function;
      String messageId;
      char* dat = &data[0];
      jsonDeserialize(dat, to, text, function, messageId);
      if ((function==101)&&((to.length()==10)||(to.length()==13&&(to.indexOf("+84")!=-1)))/*&&(text.length()<=255)*/)
      {
        publishData(MQTT_SMS_RESULT,jsonSerialize(to,200,"SUCCESS",messageId).c_str());
        if (smsSend(to, text))
        {
            publishData(MQTT_SMS_RESULT,jsonSerialize(to,201,"SENT",messageId).c_str());
            Serial.println("SENT");
        }
        else 
        {
            publishData(MQTT_SMS_RESULT,jsonSerialize(to,401,"CANT_SENT",messageId).c_str());
            Serial.println("CAN'T SEND");
        }
      }
      else
      {
        publishData(MQTT_SMS_RESULT,jsonSerialize(to,400,"BAD_REQUEST",messageId).c_str());
      }
}
void MQTTsetup() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(SMScallback);
  reconnect();
}

void MQTTloop(void)
{
 client.loop();
}
