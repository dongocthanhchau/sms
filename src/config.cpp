#include "SPIFFS.h"
#include "json.h"

void spiffBegin(void)
{
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}
String getSPIFF(String fileloc)
{
  //Serial.println("GOT DATA");
  String st;
  File file = SPIFFS.open(fileloc);
  if(!file){
    Serial.println("Failed to open file for reading");
    return "";
  }
  while(file.available()){
    st += (char)file.read();
  }
  file.close();
  return st;
}
void writeSPIFF(String fileloc, String st)
{
  File file = SPIFFS.open(fileloc, FILE_WRITE);
  file.print(st);
  file.close();
}
static String st,deviceID, ssid, password, ipaddress, host, port, username, pass, status, command, result;

void changeData(String data, String dataChange)
{
    if (data=="deviceID")
        deviceID=dataChange;
    else if (data =="ssid")
        ssid=dataChange;
    else if (data =="password")
        password=dataChange;
    else if (data =="ipaddress")
        ipaddress=dataChange;
    else if (data =="host")
        host=dataChange;
    else if (data =="port")
        port=dataChange;
    else if (data =="username")
        username=dataChange;
    else if (data =="pass")
        pass=dataChange; 
    else if (data =="status")
        status=dataChange;
    else if (data =="command")
        command=dataChange; 
    else if (data =="result")
        result=dataChange;
    writeSPIFF("/conf.txt",SPIFFSjsonSerialize(deviceID, ssid, password, ipaddress, host, port, username, pass, status, command, result));
    
}
String getData(String data)
{
  
  
  if ((st=="")||(st.indexOf(data)<0))
  {
    st = getSPIFF("/conf.txt");
    if (st.indexOf(data)<0) return""; 
    SPIFFSjsonDeserialize(st.c_str(), deviceID, ssid, password, ipaddress, host, port, username, pass, status, command, result);
  }
  
    if (data=="deviceID")
        return(deviceID);
    else if (data =="ssid")
        return(ssid);
    else if (data =="password")
        return(password);
    else if (data =="ipaddress")
        return(ipaddress);
    else if (data =="host")
        return(host);
    else if (data =="port")
        return(port);
    else if (data =="username")
        return(username);
    else if (data =="pass")
        return(pass); 
    else if (data =="status")
        return(status);
    else if (data =="command")
        return(command); 
    else if (data =="result")
        return(result);
    else 
    return "";
}
