#include "Arduino.h"
#include "ArduinoJson.h"


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
