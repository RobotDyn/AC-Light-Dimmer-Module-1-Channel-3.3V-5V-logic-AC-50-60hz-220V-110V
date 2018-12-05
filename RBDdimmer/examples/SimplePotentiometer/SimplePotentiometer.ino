#include <RBDdimmer.h>

dimmerLamp dimmer(4); //initialase port for dimmer: name(PinNumber);

int outVal = 0;

void setup() {
  Serial.begin(9600);
  dimmer.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 
}

void loop() 
{
  outVal = map(analogRead(0), 1, 1024, 100, 0); // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
  Serial.println(outVal); 
  dimmer.setPower(outVal); // name.setPower(0%-100%)
}
