#include "DHT.h"

#define DHTPIN 2  
#define DHTTYPE DHT22 

float temperature = 0.0f;
float humidity = 0.0f;

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  Serial.begin(9600);
  dht.begin();  
}

void loop() {

  readSensor();
  sendHumidityToNextion();
  sendTemperatureToNextion();
  delay(2000);
}

void readSensor()
{
 humidity = dht.readHumidity();
 temperature = dht.readTemperature();
}

void sendHumidityToNextion()
{
  String command = "humidity.txt=\""+String(humidity)+"\"";
  Serial.print(command);
  endNextionCommand();
}

void sendTemperatureToNextion()
{
  String command = "temperature.txt=\""+String(temperature,1)+"\"";
  Serial.print(command);
  endNextionCommand();
}

void endNextionCommand()
{
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

