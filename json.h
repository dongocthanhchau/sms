#ifndef _json_h_
#define _json_h_
void jsonDeserialize(char* json, String &to, String &text,int &function, String &messageId);
String jsonSerialize(String num,int id,String stt,String messageId);
void cmdjsonDeserialize(char *json, int &function);
void incjsonDeserializz(char *json, int &funtion, String &seriNumber);
#endif
