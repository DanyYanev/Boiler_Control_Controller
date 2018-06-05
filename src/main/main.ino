#include <EEPROM.h>
//#include <doxygen.h>
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
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

#include "JsonBuilder.h"
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
#define BTEMP_SET_EE 32
#define BHIST_SET_EE 74
#define HTEMP_SET_EE 106
 
int RELAY1 = 30;
int RELAY2 = 32;
int RELAY3 = 34;
int RELAY4 = 36;
int RELAY5 = 38;
int RELAY6 = 40;
int RELAY7 = 42;
int RELAY8 = 44;
int RELAY9 = 46;
int RELAY10 = 48;
int RELAY11 = 50;
int RELAY12 = 52;

int RESET_PIN = 12;

uint32_t BTemp;
uint32_t BTempSet;
uint32_t BHistSet;
uint32_t HTemp;
uint32_t HTempSet;

int HState;
int HTimeOut;

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
bool HInMotion;
bool BCharge;

/*
 * DECLARE NEXTION objects [page id:0,component id:1, component name: "q0"]. 
 */
JsonBuilder JB("12345", "token");
 
OneWire BSensor(A0);
OneWire HSensor(A1);

DallasTemperature BSensors(&BSensor);
DallasTemperature HSensors(&HSensor);

NexPage page0 = NexPage(0, 0, "page0");
NexPage page1 = NexPage(1, 0, "page1");
NexPage page2 = NexPage(2, 0, "page2");
NexPage page5 = NexPage(5, 0, "page5");

NexNumber BTempN = NexNumber(0, 4, "BTemp");
NexNumber HTempN = NexNumber(0, 5, "HTemp");
NexNumber BTempSetN = NexNumber(1, 4, "BTempSet");
NexNumber BHistSetN = NexNumber(1, 3, "BHistSet");
NexNumber HTempSetN = NexNumber(5, 6, "HTempSet");

NexCrop buttonPoolPump = NexCrop(0, 8, "PoolPump");
NexCrop buttonBoilerSource = NexCrop(1, 10, "BSource"); // Crop on Page 1 For source pic swich
NexCrop buttonHeatingSource = NexCrop(5, 2, "HSource"); // Crop on Page 5 for source pic swich
NexCrop buttonBoilerSwichC = NexCrop(0, 9, "BoilerSwichC");
NexCrop buttonHeatingSwichC = NexCrop(0, 10, "HeatingSwichC");
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

NexButton buttonBoiler = NexButton(0, 6, "BoilerM");
NexButton buttonHeating = NexButton(0, 7, "HeatingM"); 
NexButton buttonBSourceK = NexButton(1, 11, "BSourceK");
NexButton buttonBSourceH = NexButton(1, 12, "BSourceH");
NexButton buttonHSourceK = NexButton(5, 4, "HSourceK");
NexButton buttonHSourceHP = NexButton(5, 5, "HSourceHP");
NexButton buttonBackB = NexButton(1, 2, "Back");
NexButton buttonBackS = NexButton(2, 4, "Back");
NexButton buttonBackH = NexButton(5, 3, "Back");
NexButton buttonReset = NexButton(2, 5, "Reset");



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
  &buttonReset,
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
  pinMode(RESET_PIN, OUTPUT);
  
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


  digitalWrite(RESET_PIN, HIGH);
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
  //digitalWrite(RESET_PIN, HIGH);
  nexInit();

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

  BCharge = 0;
  HTimeOut = 0;
  HInMotion = 0;

  Serial.begin(115200);
  Serial3.begin(9600);
  BSensors.begin();
  HSensors.begin();

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
  buttonReset.attachPush(buttonResetPushCallback); 
  
  Serial.println("Setup done");
  //TempUpdate();
  buttonBack0PushCallback(NULL); //TempUpdate inside
  
}

unsigned long currentMillis = millis();
unsigned long lastBoilerMillis = currentMillis;
unsigned long lastHeatingMillis = currentMillis;
unsigned long lastJBMillis = currentMillis;


void loop() {

  currentMillis = millis();

  if ((unsigned long)(currentMillis - lastBoilerMillis) >= 60000) { //every minute 60000 = 60s
    TempUpdate();

    lastBoilerMillis = currentMillis;
  }

  if ((unsigned long)(currentMillis - lastJBMillis) >= 5000) {
    if(JB.getAlternated()){
      Serial3.println(JB.getJson());
    }

    lastJBMillis = currentMillis;
  }


  if ((unsigned long)(currentMillis - lastHeatingMillis) >= 3000) {
    Thermosthat();

    lastHeatingMillis = currentMillis;
  }

  nexLoop(nex_listen_list);

}


