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
/*
 * DEFINE INPUTS/OUTPUTS 
 */
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

uint32_t BTemp = 0;
uint32_t BTempSet = 0;
uint32_t BHistSet = 0;
uint32_t HTempSet = 0;
int i = 0;

bool PoolPump = false;
bool FloorPump = false;
bool ConvPump = false;
bool FloorConvPump = false;
bool BoilerSource = false;
bool BoilerState = false;
bool HeatingSource = false;
bool HeatingState = false;
bool Priority = false;

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
  TempUpdate();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(millis() % 10000 == 0){
    TempUpdate();
  }
   
  nexLoop(nex_listen_list);
  
}
 
  

