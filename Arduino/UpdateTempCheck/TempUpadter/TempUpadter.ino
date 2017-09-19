#include <INextionCallback.h>
#include <INextionColourable.h>
#include <INextionFontStyleable.h>
#include <INextionNumericalValued.h>
#include <INextionStringValued.h>
#include <INextionTouchable.h>
#include <INextionWidget.h>
#include <Nextion.h>
#include <NextionButton.h>
#include <NextionCallbackFunctionHandler.h>
#include <NextionCrop.h>
#include <NextionDualStateButton.h>
#include <NextionGauge.h>
#include <NextionHotspot.h>
#include <NextionNumber.h>
#include <NextionPage.h>
#include <NextionPicture.h>
#include <NextionProgressBar.h>
#include <NextionSlider.h>
#include <NextionText.h>
#include <NextionTimer.h>
#include <NextionTypes.h>
#include <NextionVariableNumeric.h>
#include <NextionVariableString.h>
#include <NextionWaveform.h>

#include "Nextion.h"

NexNumber n2 = NexNumber(0, 10, "n2");

char buffer[100] = {0};

NexTouch *nex_listen_list[] =
{
&n2,
NULL
};

void n0PopCallback(void *ptr)
{
dbSerialPrintln("n0PopCallback");
n2.setValue(50);
}

void setup()
{
nexInit();
n2.attachPop(n0PopCallback);
int t1 = 0;
}

void loop(void)
{
n2.setValue(t1);
delay(2000);
t1++;
nexLoop(nex_listen_list);
}
