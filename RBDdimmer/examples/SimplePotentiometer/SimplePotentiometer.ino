#include <RBDdimmer.h>

dimmerLamp dim4(4); //initialase port for dimmer: name(PinNumber);

int DIM4 = 0;

void setup() {
 dim4.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 

}

void loop() 
{
  DIM4 = map(analogRead(0), 1, 1024, 100, 0); // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
  Serial.println (DIM4); 
  dim4.setPower(DIM4); // name.setPower(0%-100%)
  int i =analogRead(0);
  Serial.println (i);
}
