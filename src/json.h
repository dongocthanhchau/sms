#ifndef _json__h_
#define _json__h_
void jsonDeserialize(char* json, String &to, String &text,int &function, String &messageId);
String jsonSerialize(String num,int id,String stt,String messageId);
void cmdjsonDeserialize(char *json, int &function);
void incjsonDeserializz(char *json, int &funtion, String &seriNumber);
void SPIFFSjsonDeserialize(const char*json, String &deviceID, String &ssid, String &password, String &ipaddress, String &host, String &port, String &username, String &pass, String &status, String &command, String &result);
String SPIFFSjsonSerialize(String deviceID, String ssid, String password, String ipaddress, String host, String port, String username, String pass, String status, String command, String result);

#endif
