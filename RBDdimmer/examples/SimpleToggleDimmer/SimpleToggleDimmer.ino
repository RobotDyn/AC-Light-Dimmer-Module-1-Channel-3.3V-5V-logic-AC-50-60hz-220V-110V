#include <RBDdimmer.h>

dimmerLamp dimmer1(4); //initialase port for dimmer: name(PinNumber);

void setup() {
  Serial.begin(9600);
  dimmer1.begin(TOGGLE_MODE, OFF); //dimmer initialisation: name.begin(MODE, STATE) 
  Serial.println("--- Toggle dimmer example ---");
  dimmer1.toggleSettings(0, 100); //Name.toggleSettings(MIN, MAX);
  dimmer1.setState(ON); // state: dimmer1.setState(ON/OFF);
  pinMode(14, INPUT);
}
void loop() {
  // put your main code here, to run repeatedly:

}
