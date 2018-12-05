#include <RBDdimmer.h>

#define LAMPMAXVALUE 100

dimmerLamp dimmer(4); //initialase port for dimmer: name(PinNumber);

int stateL = 0, valLamp;
int DIM4 = 0;
int mainLamp = 0;
int buttonRed = 0; 
int buttonBlue = 0; 
bool setLamp = true;

void setup() {  
  Serial.begin(9600);
  dimmer.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 
}

void RiseFallLamp(bool RiseFallInt)
{
  if ((RiseFallInt == true) && (mainLamp < LAMPMAXVALUE)) mainLamp++;
  else if ((RiseFallInt != true) && (mainLamp > 0)) mainLamp--;
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
  
  if (buttonRed < 1) stateL++;
  if (buttonBlue < 1) stateL--;
  if (stateL < 0) stateL = 0;
  if (stateL > 1) stateL = 1;
}

void loop() {
  readButtonState();
  dimmer.setPower(mainLAMP); // setPower(0-100%);
  RiseFallLamp(setLampState(stateL));
  delay(25);
}
