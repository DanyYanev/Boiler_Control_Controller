#include <EEPROM.h>
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
//#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//#include <LiquidCrystal.h>

#include "Functions_Revision3.h"
/*
 * DEFINE INPUTS/OUTPUTS 
 */
#define POOL_PUMP_EE 0
#define FLOOR_PUMP_EE 1
#define CONV_PUMP_EE 2
#define FLOOR_CONV_PUMP_EE 3
#define BOILER_SOURCE_EE 4
#define BOILER_STATE_EE 5
#define HEATING_SOURCE_EE 6
#define HEATING_STATE_EE 7
#define PRIORITY_EE 8
#define BTEMP_EE 10
#define BTEMP_SET_EE 32
#define BHIST_SET_EE 74
#define HTEMP_SET_EE 106
 
int RELAY1 = 31;
int RELAY2 = 33;
int RELAY3 = 35;
int RELAY4 = 37;
int RELAY5 = 39;
int RELAY6 = 41;
int RELAY7 = 43;
int RELAY8 = 45;
int RELAY9 = 47;
int RELAY10 = 49;
int RELAY11 = 51;
int RELAY12 = 53;

uint32_t BTemp;
uint32_t BTempSet;
uint32_t BHistSet;
uint32_t HTemp;
uint32_t HTempSet;

bool PoolPump;
bool FloorPump;
bool ConvPump;
bool FloorConvPump;
bool BoilerSource;
bool BoilerState;
bool BoilerPic;
bool HeatingSource;
bool HeatingState;
bool HeatingPic;
bool Priority;

/*
 * DECLARE NEXTION objects [page id:0,component id:1, component name: "q0"]. 
 */
OneWire BSensor(A0);

DallasTemperature BSensors(&BSensor);

NexPage page0 = NexPage(0, 0, "page0");
NexPage page1 = NexPage(1, 0, "page1");
NexPage page2 = NexPage(2, 0, "page2");
NexPage page5 = NexPage(5, 0, "page5");

NexNumber BTempN = NexNumber(0, 6, "BTemp");
NexNumber BTempSetN = NexNumber(1, 4, "BTempSet");
NexNumber BHistSetN = NexNumber(1, 3, "BHistSet");
NexNumber HTempSetN = NexNumber(5, 6, "HTempSet");

NexCrop buttonPoolPump = NexCrop(0, 10, "PoolPump");
NexCrop buttonBoilerSource = NexCrop(1, 10, "BSource"); // Crop on Page 1 For source pic swich
NexCrop buttonHeatingSource = NexCrop(5, 2, "HSource"); // Crop on Page 5 for source pic swich
NexCrop buttonBoilerSwichC = NexCrop(0, 11, "BoilerSwichC");
NexCrop buttonHeatingSwichC = NexCrop(0, 12, "HeatingSwichC");
NexCrop buttonPriority = NexCrop(1, 9, "Priority");
NexCrop buttonFloorPump = NexCrop(5, 9, "FloorPump");
NexCrop buttonConvPump = NexCrop(5, 10, "ConvPump");
NexCrop buttonFloorConvPump = NexCrop(5, 11, "FloorConvPump");

NexCrop buttonBTempUp = NexCrop(1, 6, "BTempUp");
NexCrop buttonBTempDown = NexCrop(1, 5, "BTempDown");
NexCrop buttonBHistUp = NexCrop(1, 8, "BHistUp");
NexCrop buttonBHistDown = NexCrop(1, 7, "BHistDown");
NexCrop buttonHTempUp = NexCrop(5, 8, "HTempUp");
NexCrop buttonHTempDown = NexCrop(5, 7, "HTempDown");

NexButton buttonBoiler = NexButton(0, 8, "BoilerM");
NexButton buttonHeating = NexButton(0, 9, "HeatingM"); 
NexButton buttonBSourceK = NexButton(1, 11, "BSourceK");
NexButton buttonBSourceH = NexButton(1, 12, "BSourceH");
NexButton buttonHSourceK = NexButton(5, 4, "HSourceK");
NexButton buttonHSourceHP = NexButton(5, 5, "HSourceHP");
NexButton buttonBackB = NexButton(1, 2, "Back");
NexButton buttonBackS = NexButton(2, 4, "Back");
NexButton buttonBackH = NexButton(5, 3, "Back");



/*
 * REGISTER NEXTION objects to the touch event list.
 */
NexTouch *nex_listen_list[] =
{
  &buttonBoilerSwichC,
  &buttonHeatingSwichC,
  &buttonPriority,
  &buttonBTempUp,
  &buttonBTempDown,
  &buttonBHistUp,
  &buttonBHistDown,
  &buttonHTempUp,
  &buttonHTempDown,
  &buttonPoolPump,
  &buttonFloorPump,
  &buttonConvPump,
  &buttonFloorConvPump,
  &buttonBoiler,
  &buttonHeating, 
  &buttonBSourceK,
  &buttonBSourceH,
  &buttonHSourceK,
  &buttonHSourceHP,
  &buttonBackB,
  &buttonBackS,
  &buttonBackH,
  NULL
};

/*
   BUTTONS PAGES update function.
*/ 


void setup() {
  // put your setup code here, to run once:
   /*
   *OUTPUT setup. 
   */
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(RELAY5, OUTPUT);
  pinMode(RELAY6, OUTPUT);
  pinMode(RELAY7, OUTPUT);
  pinMode(RELAY8, OUTPUT);
  pinMode(RELAY9, OUTPUT);
  pinMode(RELAY10, OUTPUT);
  pinMode(RELAY11, OUTPUT);
  pinMode(RELAY12, OUTPUT);
  
   /*
   *OUTPUT start setup. 
   */
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);
  digitalWrite(RELAY5, HIGH);
  digitalWrite(RELAY6, HIGH);
  digitalWrite(RELAY7, HIGH);
  digitalWrite(RELAY8, HIGH);
  digitalWrite(RELAY9, HIGH);
  digitalWrite(RELAY10, HIGH);
  digitalWrite(RELAY11, HIGH);
  digitalWrite(RELAY12, HIGH);
  nexInit();

  EEPROM.get(BTEMP_EE, BTemp);
  EEPROM.get(BTEMP_SET_EE, BTempSet);
  EEPROM.get(BHIST_SET_EE, BHistSet);
  EEPROM.get(HTEMP_SET_EE, HTempSet);

  EEPROM.get(POOL_PUMP_EE, PoolPump);
  EEPROM.get(FLOOR_PUMP_EE, FloorPump);
  EEPROM.get(CONV_PUMP_EE, ConvPump);
  EEPROM.get(FLOOR_CONV_PUMP_EE, FloorConvPump);
  EEPROM.get(BOILER_SOURCE_EE, BoilerSource);
  EEPROM.get(BOILER_STATE_EE, BoilerPic);
  EEPROM.get(HEATING_SOURCE_EE, HeatingSource);
  EEPROM.get(HEATING_STATE_EE, HeatingPic);
  EEPROM.get(PRIORITY_EE, Priority);

  Serial.begin(115200);
  BSensors.begin();

  Serial.println("Serial start");

  /*
   *Register the pop event callback function of the current component. 
   */
  buttonBoilerSwichC.attachPush(buttonBoilerSwichCPushCallBack);
  buttonHeatingSwichC.attachPush(buttonHeatingSwichCPushCallBack);
  buttonPriority.attachPush(buttonPriorityPushCallBack);
  buttonBTempUp.attachPush(buttonBTempUpPushCallBack);
  buttonBTempDown.attachPush(buttonBTempDownPushCallBack);
  buttonBHistUp.attachPush(buttonBHistUpPushCallBack);
  buttonBHistDown.attachPush(buttonBHistDownPushCallBack);
  buttonHTempUp.attachPush(buttonHTempUpPushCallBack);
  buttonHTempDown.attachPush(buttonHTempDownPushCallBack);
    
  buttonPoolPump.attachPush(buttonPoolPumpPushCallback);
  buttonFloorPump.attachPush(buttonFloorPumpPushCallback);
  buttonConvPump.attachPush(buttonConvPumpPushCallback);
  buttonFloorConvPump.attachPush(buttonFloorConvPumpPushCallback);
  
  buttonBoiler.attachPush(buttonBoilerPushCallback);
  buttonHeating.attachPush(buttonHeatingPushCallback);
  
  buttonBSourceK.attachPush(buttonBoilerSourceKSwapPushCallBack);
  buttonBSourceH.attachPush(buttonBoilerSourceHSwapPushCallBack);
  
  buttonHSourceK.attachPush(buttonHeatingSourceKSwapPushCallBack);
  buttonHSourceHP.attachPush(buttonHeatingSourceHPSwapPushCallBack);
  
  buttonBackB.attachPush(buttonBack0PushCallback);
  buttonBackS.attachPush(buttonBack0PushCallback);
  buttonBackH.attachPush(buttonBack0PushCallback);
  
  dbSerialPrintln("Setup done");
  //TempUpdate();
  buttonBack0PushCallback(NULL); //TempUpdate inside
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(millis() % 10000 == 0){
    TempUpdate();
  }
   
  nexLoop(nex_listen_list);
  
}
 
  

