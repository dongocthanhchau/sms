#ifndef _MQTT__H_
#define _MQTT__H_

void publishData(const char *topic,const char *serialData);
void reconnect();
void MQTTsetup(void);
void MQTTloop(void);


#endif
