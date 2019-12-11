
#include "src\Sim800L.h"
#include "src\wifi.h"
#include "src\blynk.h"
#include "src\mqtt.h"
#include "src\NTPClock.h"

int ledDelay = 500/portTICK_PERIOD_MS;

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

// define tasks
void TaskBlink( void *pvParameters );
void TaskWifi( void *pvParameters );
void TaskButton( void *pvParameters );
void TaskSim800( void *pvParameters );
void setup() {
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  //disableCore0WDT();
  //disableCore1WDT();
  // Setup Task
  xTaskCreatePinnedToCore(
    TaskSim800
    ,  "Sim800"   
    ,  40960  
    ,  NULL
    ,  1 
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
    
  xTaskCreatePinnedToCore(
    TaskBlink
    ,  "Blink"   
    ,  1024  
    ,  NULL
    ,  2  
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskWifi
    ,  "Wifi"
    ,  40960  
    ,  NULL
    ,  1 
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskButton
    ,  "Button"
    ,  1024  
    ,  NULL
    ,  2  
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  
}

void loop()
{
  // Empty. Things are done in Tasks.
  if (millis()>43199997)
    {
      Serial.println("IT'S "+getTime());
      Serial.println("RESETTING, PLEASE WAIT...");
      ESP.restart();
    }
   vTaskDelay(2500/portTICK_PERIOD_MS);
   String state = "{\"status\":\"Running\",\"time\":" +  getTime() + "}";
   
   publishData("/STATUS",state.c_str());
   reconnect();
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  
  pinMode(WIFI_LED, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    blinkLed(WIFI_LED);   
    vTaskDelay(ledDelay);  
  }
}

void TaskWifi(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  Serial.println(wifiInit()?"WIFI Done":"WIFI FAIL");
  
  MQTTsetup();
  NTPInit();
  for (;;)
  {
    MQTTloop();
    vTaskDelay(2000/portTICK_PERIOD_MS);
    //Nothing Here
  }
}
void TaskSim800(void *pvParameters)
{
  (void) pvParameters;
  Serial.println(sim800Init()?"SIM 800 Done":"SIM 800 FAIL");
  ledDelay = 5000/portTICK_PERIOD_MS; 
  for (;;)
  {
    
    //balanceInc("315329942286398");
    //vTaskDelay(5000/portTICK_PERIOD_MS);
    //Serial.println(balanceCheck());
    //vTaskDelay(5000/portTICK_PERIOD_MS);
    //Nothing else
  }
}
void TaskButton(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  pinMode(BUTTON, INPUT_PULLUP);
  int t=-1;
  for (;;) // A Task shall never return or exit.
  {
    //Serial.println(analogRead(BUTTON));
    if (analogRead(BUTTON)<10)
    { 
    t-=-1;
    }else t=-1;
    
    vTaskDelay(10/portTICK_PERIOD_MS);
    if (t>RESET_TIME*100) 
    {
      Serial.println("LONG CLICK");
      ApMode();
      t=-1;
      ledDelay = 100/portTICK_PERIOD_MS;
      vTaskDelay(1000/portTICK_PERIOD_MS);
    }
//    else if (t>10) 
//    {
//      Serial.println("SHORT CLICK");
//      //Do Something
//    }
    
  }
}
