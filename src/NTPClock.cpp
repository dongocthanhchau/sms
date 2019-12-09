#include "NTPClock.h"
#include <NTPClient.h>
#include <WiFiUdp.h>


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 25200, 60000);

void NTPInit(void)
{
  timeClient.begin();
}
String getTime()
{
  timeClient.update();
  return String(timeClient.getFormattedTime());
}
