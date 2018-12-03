#include <RBDdimmer.h>

dimmerLamp dimmer1(4); //initialase port for dimmer: name(PinNumber);

void setup() {
  Serial.begin(9600);
  dimmer1.begin(NORMAL_MODE, OFF); //dimmer initialisation: name.begin(MODE, STATE) 
  Serial.println("--- Simple dimmer example ---");
  dimmer1.setPower(50); // setPower(0-100%);
  dimmer1.setState(ON); // setState(ON/OFF);
  pinMode(5, INPUT);
}
void loop() {
  
  if (digitalRead(5) == 1) 
  {
    dimmer1.changeState(); //changes state on the opposite: name.changeState(); 
    Serial.print("Dimmer state: ");
    Serial.println(dimmer1.getState());
  }
  delay(1000);
  
}
