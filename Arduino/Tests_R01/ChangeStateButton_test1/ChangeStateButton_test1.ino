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

bool PoolPump = false;
/*
 * DECLARE NEXTION objects [page id:0,component id:1, component name: "q0"]. 
 */
NexPage page0    = NexPage(0, 0, "page0");
NexPage page1    = NexPage(1, 0, "page1");
NexPage page5    = NexPage(5, 0, "page5");

NexCrop buttonPoolPump = NexCrop(0, 10, "PoolPump");
NexButton buttonBoiler = NexButton(0, 8, "BoilerM");
NexButton buttonHeater = NexButton(0, 9, "HeaterM");
NexButton buttonBackB = NexButton(1, 2, "Back");
NexButton buttonBackH = NexButton(5, 2, "Back");

/*
 * REGISTER NEXTION objects to the touch event list.
 */
NexTouch *nex_listen_list[] =
{
  &buttonPoolPump,
  &buttonBoiler,
  &buttonHeater,
  //&buttonBSourceK,
  &buttonBackB,
  &buttonBackH,
  NULL
};

/*
   BUTTONS PAGES update function.
*/ 
void Pic_Update(bool state, NexCrop button, int pageOn, int pageOff){
  uint32_t var;
  if(state)var = pageOn;
  else var = pageOff;
  button.setPic(var);
}

/*
   BUTTONS component callback function.
*/ 
void buttonBoilerPushCallback(void *ptr)
{
  dbSerialPrintln("buttonBoilerPushCallback");
  page1.show();
}

void buttonBack0PushCallback(void *ptr){
  dbSerialPrintln("Back to page 0");
  page0.show();
  Pic_Update(PoolPump, buttonPoolPump, 7, 2);
}

void buttonHeaterPushCallback(void *ptr)
{
  dbSerialPrintln("buttonHeaterPushCallback");
  page5.show();
}

void buttonPoolPumpPushCallback(void *ptr)
{
  dbSerialPrintln("PoolPumpPopCallback");
  
  if(PoolPump)PoolPump = false;
  else PoolPump = true;

  if(PoolPump == true){
  digitalWrite(RELAY6, LOW);
  Serial.println("RELAY6 ON");
  }
  if(PoolPump == false){
  digitalWrite(RELAY6, HIGH);
  Serial.println("RELAY6 OFF");
  }

  Pic_Update(PoolPump, buttonPoolPump, 7, 2);
  
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

  Serial.println("Serial start");

  /*
   *Register the pop event callback function of the current button0 component. 
   */
  buttonPoolPump.attachPush(buttonPoolPumpPushCallback);
  buttonBoiler.attachPush(buttonBoilerPushCallback);
  buttonHeater.attachPush(buttonHeaterPushCallback);
  buttonBackB.attachPush(buttonBack0PushCallback);
  buttonBackH.attachPush(buttonBack0PushCallback);
  dbSerialPrintln("Setup done");
  
}

void loop() {
  
  // put your main code here, to run repeatedly:
  nexLoop(nex_listen_list);
}
 
  

