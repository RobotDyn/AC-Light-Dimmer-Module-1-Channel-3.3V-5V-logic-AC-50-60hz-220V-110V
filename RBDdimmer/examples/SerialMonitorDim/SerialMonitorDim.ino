#include <RBDdimmer.h>//

dimmerLamp dim4(4); //initialase port for dimmer: name(PinNumber);

int DIM4 = 0;
int vdim4 = 0;

void setup() {
 Serial.begin(9600); 
 dim4.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 
 Serial.println("Dimmer Program is starting...");
 Serial.println("Set value");
}

void printSpace(int val)
{
  if ((vdim4 / 100) == 0) Serial.print(" ");
  if ((vdim4 / 10) == 0) Serial.print(" ");
}

void loop() {
  int pre_val = DIM4;

  if (Serial.available())
  {
    int buf = Serial.parseInt();
    if (buf != 0) DIM4 = buf;
    delay(200);
  }
  dim4.setPower(DIM4); // setPower(0-100%);

  if (pre_val != DIM4)
  {

    Serial.print("% lampValue -> ");
    printSpace(DIM4);
    Serial.print(DIM4);

  }
  delay(50);

}
