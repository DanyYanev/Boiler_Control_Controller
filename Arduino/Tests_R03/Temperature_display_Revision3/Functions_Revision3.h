#pragma once

#include<iostream>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <doxygen.h>
#include <NexButton.h>
#include <NexCheckbox.h>
#include <NexConfig.h>
#include <NexCrop.h>
#include <NexDualStateButton.h>
#include <NexGauge.h>
#include <NexGpio.h>
#include <NexHardware.h>
#include <NexHotspot.h>
#include <NexNumber.h>
#include <NexObject.h>
#include <NexPage.h>
#include <NexPicture.h>
#include <NexProgressBar.h>
#include <NexRadio.h>
#include <NexRtc.h>
#include <NexScrolltext.h>
#include <NexSlider.h>
#include <NexText.h>
#include <NexTimer.h>
#include <Nextion.h>
#include <NexTouch.h>
#include <NexUpload.h>
#include <NexVariable.h>
#include <NexWaveform.h>

void TempUpdate(){
 
  //dbSerialPrintln("IN");
  BSensors.requestTemperatures();
  BTemp = BSensors.getTempCByIndex(0);
  BTempN.setValue(BTemp);
  
}

void Pic_Update(bool state, NexCrop button, int pageOn, int pageOff){
  uint32_t var;
  if(state)var = pageOn;
  else var = pageOff;
  button.setPic(var);
}

void UpdateSingleRelays(bool state, bool master, int relay1){  //Update Relay Function
  if(master){
    if(state == true){
      digitalWrite(relay1, LOW);
      //Serial.println("RELAY6 ON");
    }
    if(state == false){
      digitalWrite(relay1, HIGH);
      //Serial.println("RELAY6 OFF");
    }
  } else {
    digitalWrite(relay1, HIGH);
  }
}

void UpdateDoubleRelays(bool state, bool master, int relay1, int relay2){ //BOILER SOURCE
  if(master){
    if(state){
      digitalWrite(relay1, LOW);
      //Serial.println("RELAY1 ON");
      digitalWrite(relay2, HIGH);
      //Serial.println("RELAY2 OFF");
    } else{
      digitalWrite(relay1, HIGH);
      //Serial.println("RELAY1 OFF");
      digitalWrite(relay2, LOW);
      //Serial.println("RELAY2 ON");
    }
  } else {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
  }
}

void UpdateSourceK(){
  bool K = false;
  if(BoilerState){
    if(BoilerSource == 1) K = true;
  }
  if(HeatingState){
    if(HeatingSource){
      if(FloorPump || ConvPump || FloorConvPump) K = true;
    }
  }
  if(PoolPump) K = true;
  //dbSerialPrintln(K);
  UpdateSingleRelays(K, 1, RELAY8);
}

void UpdateSourceHP(){
  bool HP = false;
  if(HeatingState){
    if(!HeatingSource){ //if HeatPump source on
      if(FloorPump || ConvPump || FloorConvPump) HP = true;
    }
  }
  UpdateSingleRelays(HP, 1, RELAY7);
}

void UpdatePriority(){
  if(Priority && BoilerState && BoilerSource && HeatingState && HeatingSource && (FloorPump || ConvPump || FloorConvPump)){
      HeatingState = false;
      UpdateSingleRelays(FloorPump, HeatingState, RELAY3);
      UpdateSingleRelays(ConvPump, HeatingState, RELAY4);
      UpdateSingleRelays(FloorConvPump, HeatingState, RELAY5);
      UpdateSourceK();
      UpdateSourceHP();
      HeatingState = true;
  } else {
      UpdateSingleRelays(FloorPump, HeatingState, RELAY3);
      UpdateSingleRelays(ConvPump, HeatingState, RELAY4);
      UpdateSingleRelays(FloorConvPump, HeatingState, RELAY5);
      UpdateSourceK();
      UpdateSourceHP();
  }
}

/*
   BUTTONS component callback function.
*/ 

//PAGE CHANGING
void buttonBoilerPushCallback(void *ptr)  //GO TO PAGE BOILER
{
  dbSerialPrintln("buttonBoilerPushCallback");
  page1.show();
  Pic_Update(BoilerSource, buttonBoilerSource, 6, 4);
  Pic_Update(Priority, buttonPriority, 6, 4);
}

void buttonHeatingPushCallback(void *ptr)  //GO TO PAGE HEATING
{
  dbSerialPrintln("buttonHeatingPushCallback");
  page5.show();
  Pic_Update(HeatingSource, buttonHeatingSource, 8, 5);
  Pic_Update(FloorPump, buttonFloorPump, 8, 5);
  Pic_Update(ConvPump, buttonConvPump, 8, 5);
  Pic_Update(FloorConvPump, buttonFloorConvPump, 8, 5);
}

void buttonBack0PushCallback(void *ptr){  //GO BACK TO HOME PAGE
  dbSerialPrintln("Back to page 0");
  page0.show();
  TempUpdate();
  Pic_Update(PoolPump, buttonPoolPump, 7, 2);
  Pic_Update(BoilerState, buttonBoilerSwichC, 7, 2);
  Pic_Update(HeatingState, buttonHeatingSwichC, 7, 2);
}

//LOGISTIC FUNCTIONS
void buttonBoilerSwichCPushCallBack(void *ptr){ //BOILER ON OFF
  if(BoilerState)BoilerState = false;
  else BoilerState = true;
  
  dbSerialPrint("Boiler Swich");
  dbSerialPrint(BoilerState);
  
  UpdateDoubleRelays(BoilerSource, BoilerState, RELAY1, RELAY2);
  UpdatePriority();
  Pic_Update(BoilerState, buttonBoilerSwichC, 7, 2);
}

void buttonHeatingSwichCPushCallBack(void *ptr){ //HEATING ON OFF 
  if(HeatingState)HeatingState = false;
  else HeatingState = true;
  
  dbSerialPrint("Heating Swich");
  dbSerialPrint(HeatingState);
  
  UpdatePriority();
  
  Pic_Update(HeatingState, buttonHeatingSwichC, 7, 2);
}

void buttonPriorityPushCallBack(void *ptr){
  dbSerialPrintln("buttonPriorityPushCallback");

  if(Priority) Priority = false;
  else Priority = true;

  UpdatePriority();
  
  Pic_Update(Priority, buttonPriority, 6, 4);
}

void buttonBTempUpPushCallBack(void *ptr){
  dbSerialPrintln("buttonBTempUpPushCallBack");
  
  BTempSet++;
  BTempSetN.setValue(BTempSet);
}

void buttonBTempDownPushCallBack(void *ptr){
  dbSerialPrintln("buttonBTempDownPushCallBack");
  
  BTempSet--;
  BTempSetN.setValue(BTempSet);
}
 
void buttonBHistUpPushCallBack(void *ptr){
  dbSerialPrintln("buttonBHistUpPushCallBack");
  
  BHistSet++;
  BHistSetN.setValue(BHistSet);
}
  
void buttonBHistDownPushCallBack(void *ptr){
  dbSerialPrintln("buttonBHistDownPushCallBack");
  
  BHistSet--;
  BHistSetN.setValue(BHistSet);
}

void buttonHTempUpPushCallBack(void *ptr){
  dbSerialPrintln("buttonBTempUpPushCallBack");
  
  HTempSet++;
  HTempSetN.setValue(HTempSet);
}

void buttonHTempDownPushCallBack(void *ptr){
  dbSerialPrintln("buttonBTempDownPushCallBack");
  
  HTempSet--;
  HTempSetN.setValue(HTempSet);
}

void buttonBoilerSourceKSwapPushCallBack(void *ptr){   //BOILER SOURCE
  dbSerialPrintln("Boiler Source Swap to Kotel");

  BoilerSource = true;
  UpdateDoubleRelays(BoilerSource, BoilerState, RELAY1, RELAY2);
  
  UpdatePriority();
  
  Pic_Update(BoilerSource, buttonBoilerSource, 6, 4);
}

void buttonBoilerSourceHSwapPushCallBack(void *ptr){    //BOILER SOURCE
  dbSerialPrintln("Boiler Source Swap to Heater");

  BoilerSource = false;
  
  UpdateDoubleRelays(BoilerSource, BoilerState, RELAY1, RELAY2);
  UpdatePriority();
  
  Pic_Update(BoilerSource, buttonBoilerSource, 6, 4);
}

void buttonHeatingSourceKSwapPushCallBack(void *ptr){   //HEATING SOURCE
  dbSerialPrintln("Heating Source Swap to Kotel");

  HeatingSource = true;
  
  UpdatePriority();
  
  Pic_Update(HeatingSource, buttonHeatingSource, 8, 5);
}

void buttonHeatingSourceHPSwapPushCallBack(void *ptr){    //HEATING SOURCE
  dbSerialPrintln("Boiler Source Swap to El Pompa");

  HeatingSource = false;
  
  UpdatePriority();
  
  Pic_Update(HeatingSource, buttonHeatingSource, 8, 5);
}

void buttonPoolPumpPushCallback(void *ptr)   //POOL PUMP ON OFF
{
  dbSerialPrintln("PoolPumpPopCallback");
  
  if(PoolPump)PoolPump = false;
  else PoolPump = true;

  UpdateSingleRelays(PoolPump, 1, RELAY6);
  UpdateSourceK();
  
  Pic_Update(PoolPump, buttonPoolPump, 7, 2);
  
}

void buttonFloorPumpPushCallback(void *ptr)   //FLOOR PUMP ON OFF
{
  dbSerialPrintln("FloorPumpPopCallback");
  
  if(FloorPump)FloorPump = false;
  else FloorPump = true;
  
  UpdatePriority();
  
  Pic_Update(FloorPump, buttonFloorPump, 8, 5);
}

void buttonConvPumpPushCallback(void *ptr)    //CONV PUMP ON OFF
{
  dbSerialPrintln("ConvPumpPopCallback");
  
  if(ConvPump)ConvPump = false;
  else ConvPump = true;
  
  UpdatePriority();

  Pic_Update(ConvPump, buttonConvPump, 8, 5);
}

void buttonFloorConvPumpPushCallback(void *ptr)   //FLOOR CONV PUMP ON OFF
{
  dbSerialPrintln("FloorConvPopCallback");
  
  if(FloorConvPump)FloorConvPump = false;
  else FloorConvPump = true;
  
  UpdatePriority();
  
  Pic_Update(FloorConvPump, buttonFloorConvPump, 8, 5);
}

