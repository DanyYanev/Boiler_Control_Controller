
#include <Nextion.h>
/*#include "NexText.h"
  #include "NexPage.h"
  #include "NexPicture.h"
  #include "NexGauge.h"
  #include "NexButton.h"
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <string.h>
#include "Arduino.h"
#include "Wire.h"

// Temperature Sensor
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS A0
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

 //Nextion component for page:home
 //NexText temperature = NexText(0, 7, "tem");
 
//Temperature partial variable
uint32_t number = 0;
uint16_t sensor_pin = A0;    /* select the input pin for the Temperature */
uint16_t sensor_value = 0;   /* variable to store the value coming from the Temperature*/
unsigned long update = 1;
int32_t timer = -1;          /* timer for second */

//Buffer zone of various variables
char buffer[10] = {0};
char buffer_temp[10] = {0};
char buffer_temperature[10] = {0};

//void  refreshHome(void)
//temperature.setText(buffer_temperature);

void temperatureDisplay(void)
{
  sensors.requestTemperatures(); // Send the command to get temperatures
  float celsius = sensors.getTempCByIndex(0);
  number = ((float)celsius / 60) * 180 + 30;
  pointer.setValue(number);
  memset(buffer, 0, sizeof(buffer));
  itoa(celsius, buffer, 10);
  temperature.setText(buffer);
}

void setup() {
  // put your setup code here, to run once:

}

void loop() 
  // put your main code here, to run repeatedly:

{
  if (millis() > update+ 5000)
  {
    temperatureDisplay();
    update = millis();
  }
  nexLoop(nex_Listen_List);
}
