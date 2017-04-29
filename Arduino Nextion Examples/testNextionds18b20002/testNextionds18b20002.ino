#include "NexText.h"
#include <OneWire.h>
#include "NexGauge.h"
#include <DallasTemperature.h>
#include "Wire.h"
#include "RTCLib.h"

OneWire  ds(7); 

#define at_serial Serial1

/*
 *******************************************************************
 * Temperature partial variable
 * 
 *******************************************************************
 */
uint32_t number = 0;
uint16_t sensor_pin = 7;    /* select the input pin for the Temperature */
uint16_t sensor_value = 0;   /* variable to store the value coming from the Temperature*/
unsigned long update = 1;
int32_t timer = -1;          /* timer for second */

NexText temperature = NexText(0, 1, "tem");
NexGauge pointer  = NexGauge(0, 2, "z0");
char buffer[10] = {0};
char buffer_temperature[10] = {0};
char buffer_temp[10] = {0};



NexTouch *nex_Listen_List[] = 
{
    
    NULL
};


void temperatureDisplay(void)
{
    byte i;
    byte present = 0;
    byte type_s = 0;
    byte data[12];
    byte addr[8];
    float celsius;

    ds.search(addr);

    ds.reset();
    ds.select(addr);
    ds.write(0x44,1);                       /* start conversion, with parasite power on at the end */

    delay(1000);                            /* maybe 750ms is enough, maybe not */
                                            /* we might do a ds.depower() here, but the reset will take care of it. */
    present = ds.reset();
    ds.select(addr);    
    ds.write(0xBE);                         /* Read Scratchpad */

    for ( i = 0; i < 9; i++) 
    {   /* we need 9 bytes */
        data[i] = ds.read();
    }
    /* convert the data to actual temperature*/
    unsigned int raw = (data[1] << 8) | data[0];
    if (type_s) 
    {
        raw = raw << 3;                         /* 9 bit resolution default */
        if (data[7] == 0x10) 
        {
            /* count remain gives full 12 bit resolution */
            raw = (raw & 0xFFF0) + 12 - data[6];
        }
    } 
    else 
    {
        byte cfg = (data[4] & 0x60);
        if (cfg == 0x00) raw = raw << 3; 
        else if (cfg == 0x20) raw = raw << 2; 
        else if (cfg == 0x40) raw = raw << 1; 
        /* default is 12 bit resolution, 750 ms conversion time */
    }
    celsius = (float)raw / 16.0;
    number = ((float)celsius/60)*180+30;
    pointer.setValue(number);

    memset(buffer, 0, sizeof(buffer));
    itoa(celsius, buffer, 10);
    temperature.setText(buffer);
     
    
}

void setup(void)
{
    nexInit();
  
}

void loop(void)
{
    
    temperatureDisplay();
        update += 0;
    nexLoop(nex_Listen_List);
    
  
    
    
}

