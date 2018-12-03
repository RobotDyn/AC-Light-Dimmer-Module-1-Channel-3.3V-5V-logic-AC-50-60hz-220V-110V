#include <RBDdimmer.h>

dimmerLamp dim4(4); //initialase port for dimmer: name(PinNumber);

int buttonRed = 0;

void setup() {
 dim4.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 
 
}

void loop() {
  buttonRed = digitalRead(14);
  
  if (buttonRed == 1)
  {
    delay(10);
    dim4.setState(ON); //name.setState(ON/OFF);
  }
  
  if (buttonRed == 0)
  {
    delay(10);
    dim4.setState(OFF); //name.setState(ON/OFF);
  }
  
}
