#include "Arduino.h"
#include "ArduinoJson.h"
#include "json.h"

void jsonDeserialize(char* json, String &to, String &text,int &function, String &messageId)
{
  const size_t capacity = JSON_OBJECT_SIZE(10) + 500;
  DynamicJsonDocument doc(capacity);
  //const char* json = "{\"from\":\"sender_name String\",\"to\":\"recv_number String\",\"text\":\"message_content String\",\"function\":101,\"messageId\":\"String\"}";
  deserializeJson(doc, json);
  const char* t = doc["to"]; // "recv_number String"
  to = t;
  const char* te = doc["text"]; // "message_content String"
  text = te;
  int func = doc["function"]; // 101
  function = func;
  const char* m = doc["messageId"]; // "String"
  messageId = m;
}

String jsonSerialize(String num,int id,String stt,String messageId)
{
  const size_t capacity = JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5);
  DynamicJsonDocument doc(capacity);

  JsonArray messages = doc.createNestedArray("messages");

  JsonObject messages_0 = messages.createNestedObject();
  
  messages_0["to"] = num;//-----------------

  JsonObject messages_0_status = messages_0.createNestedObject("status");
  messages_0_status["id"] = id;//----------------
  messages_0_status["name"] = stt;//------------------
  messages_0["messageId"] = messageId;//----------
  String dat;
  serializeJson(doc, dat);
  return dat;
}
void cmdjsonDeserialize(char *json, int &function)
{
  const size_t capacity = JSON_OBJECT_SIZE(2) + 20;
  DynamicJsonDocument doc(capacity);
  //const char* json = "{\"function\":102}";
  deserializeJson(doc, json);
  function = doc["function"]; // 102
}
void incjsonDeserializz(char *json, int &function, String &seriNumber)
{
  const size_t capacity = JSON_OBJECT_SIZE(4) + 60;
  DynamicJsonDocument doc(capacity);
  //const char* json = "{\"seriNumber\":\"String\",\"function\":103}";
  deserializeJson(doc, json);
  const char* seri = doc["seriNumber"]; // "String"
  seriNumber = String(seri);
  function = doc["function"]; // 103
}

void SPIFFSjsonDeserialize(const char*json, String &deviceID, String &ssid, String &password, String &ipaddress, String &host, String &port, String &username, String &pass, String &status, String &command, String &result)
{
  const size_t capacity = JSON_OBJECT_SIZE(2) + 3*JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(10) + 250;
  DynamicJsonDocument doc(capacity);

  //const char* json = "{\"device\":{\"deviceID\":\"12DWR\"},\"wifi\":{\"ssid\":\"YOURSOLUTION\",\"password\":\"yoursolution\",\"ipaddress\":\"192.168.3.127\"},\"mqtt\":{\"host\":\"cretabase.kbvision.tv\",\"port\":\"1883\",\"username\":\"leiioxwr\",\"password\":\"98NW8KaE_FCH\",\"topic\":{\"status\":\"/status\",\"command\":\"/cmd\",\"result\":\"/result\"}}}";

  deserializeJson(doc, json);

  const char* device_deviceID = doc["device"]["deviceID"]; // "12DWR"
  deviceID = String(device_deviceID);
  
  JsonObject wifi = doc["wifi"];
  const char* wifi_ssid = wifi["ssid"]; // "YOURSOLUTION"
  ssid = String(wifi_ssid);
  const char* wifi_password = wifi["password"]; // "yoursolution"
  password = String (wifi_password);
  const char* wifi_ipaddress = wifi["ipaddress"]; // "192.168.3.127"
  ipaddress = String(wifi_ipaddress);

  JsonObject mqtt = doc["mqtt"];
  const char* mqtt_host = mqtt["host"]; // "cretabase.kbvision.tv"
  host = String(mqtt_host);
  const char* mqtt_port = mqtt["port"]; // "1883"
  port = String(mqtt_port);
  const char* mqtt_username = mqtt["username"]; // "leiioxwr"
  username = String(mqtt_username);
  const char* mqtt_password = mqtt["password"]; // "98NW8KaE_FCH"
  pass = String(mqtt_password);
  
  JsonObject mqtt_topic = mqtt["topic"];
  const char* mqtt_topic_status = mqtt_topic["status"]; // "/status"
  status = String(mqtt_topic_status);
  const char* mqtt_topic_command = mqtt_topic["command"]; // "/cmd"
  command = String(mqtt_topic_command);
  const char* mqtt_topic_result = mqtt_topic["result"]; // "/result"
  result = String(mqtt_topic_result);
}
String SPIFFSjsonSerialize(String deviceID, String ssid, String password, String ipaddress, String host, String port, String username, String pass, String status, String command, String result)
{
  const size_t capacity = JSON_OBJECT_SIZE(2) + 3*JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(10);
  DynamicJsonDocument doc(capacity);  

  JsonObject device = doc.createNestedObject("device");
  device["deviceID"] = deviceID;//"12DWR";

  JsonObject wifi = doc.createNestedObject("wifi");
  wifi["ssid"] = ssid;//"YOURSOLUTION";
  wifi["password"] = password;//"yoursolution";
  wifi["ipaddress"] = ipaddress;//"192.168.3.127";

  JsonObject mqtt = doc.createNestedObject("mqtt");
  mqtt["host"] = host;//"cretabase.kbvision.tv";
  mqtt["port"] = port;//"1883";
  mqtt["username"] = username;//"leiioxwr";
  mqtt["password"] = pass;//"98NW8KaE_FCH";

  JsonObject mqtt_topic = mqtt.createNestedObject("topic");
  mqtt_topic["status"] = status;//"/status";
  mqtt_topic["command"] = command;//"/cmd";
  mqtt_topic["result"] = result;//"/result";
  
  String dat;
  serializeJson(doc, dat);
  return dat;
//serializeJson(doc, Serial);
}
