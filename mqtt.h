#ifndef _MQTT_H_
#define _MQTT_H_

void publishData(const char *topic,const char *serialData);
void MQTTsetup(void);
void MQTTloop(void);


#endif
