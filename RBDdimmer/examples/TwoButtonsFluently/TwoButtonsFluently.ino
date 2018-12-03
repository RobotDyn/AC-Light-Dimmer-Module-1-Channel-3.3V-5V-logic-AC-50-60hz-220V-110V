#include <RBDdimmer.h>

#define LAMPMAXVALUE 100

dimmerLamp dim4(4); //initialase port for dimmer: name(PinNumber);

int br = 1, bb = 1, stateL = 0, valLamp;
int DIM4 = 0;
int mainLAMP = 0;
int buttonRed = 0; 
int buttonBlue = 0; 
bool setLamp = true;

void setup() {
  
  Serial.begin(9600);
  dim4.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 
  
 }

void RiseFallLamp(bool RiseFallInt)
{
  if ((RiseFallInt == true) && (mainLAMP < LAMPMAXVALUE)) mainLAMP++;
  else if ((RiseFallInt != true) && (mainLAMP > 0)) mainLAMP--;
}

bool setLampState(int val)
{
  bool ret;
  if (val >= 1) ret = true;
  else ret = false;
  return ret;
}

void readButtonState()
{
  buttonRed = digitalRead(14);
  buttonBlue = digitalRead(15);
  
  if (buttonRed < br) stateL++;
  if (buttonBlue < bb) stateL--;
  if (stateL < 0) stateL = 0;
  if (stateL > 1) stateL = 1;
}

void loop() {
  readButtonState();
  
  dim4.setPower(mainLAMP); // setPower(0-100%);
  RiseFallLamp(setLampState(stateL));
  delay(25);
}
