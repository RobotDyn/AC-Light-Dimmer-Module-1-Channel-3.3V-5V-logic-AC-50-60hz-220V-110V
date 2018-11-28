#include <Dimmer.h> //ссыдка на видоизмененную библиотеку //первоначальная библиоетка https://github.com/circuitar/Dimmer

#define LAMPMAXVALUE 63
int buttonRed = 0; 
int buttonBlue = 0; 

Dimmer dimmer4 (4,  DIMMER_NORMAL);//Dimmer dimmer(PWM_PIN,  DIMMER_NORMAL) Z-C use D2 pin
Dimmer dimmer5 (5,  DIMMER_NORMAL);//Dimmer dimmer(PWM_PIN,  DIMMER_NORMAL) Z-C use D2 pin
int i = 0; 
int dim4 = 0; 
int dim5 = 0;
void setup() { 
  Serial.begin(9600); 
  dimmer4.begin(); 
  dimmer4.setMinimum(1);
  dimmer5.begin(); 
  dimmer5.setMinimum(1);
  Serial.println("Dimmer Program is starting...");
} 
int mainLAMP = 0;
int maxdim4 = 67;
int mindim4 = -maxdim4;
int vdim4 = mindim4;
int maxdim5 = 67;
int mindim5 = -maxdim5;
int vdim5 = mindim4;
bool setLamp = true;
int br = 1, bb = 1, stateL = 0, valLamp;

void printSpace(int val)
{
  if ((vdim4 / 100) == 0) Serial.print(" ");
  if ((vdim4 / 10) == 0) Serial.print(" ");
  if ((vdim5 / 100) == 0) Serial.print(" ");
  if ((vdim5 / 10) == 0) Serial.print(" ");
}

bool setLampState(int val)
{
  bool ret;
  if (val >= 1) ret = true;
  else ret = false;
  return ret;
}

void RiseFallLamp(bool RiseFallInt)
{
  if ((RiseFallInt == true) && (mainLAMP < LAMPMAXVALUE)) mainLAMP++;
  else if ((RiseFallInt != true) && (mainLAMP > 0)) mainLAMP--;
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

  dim5 = map(analogRead(0), 1, 1024, 100, 15); // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
  Serial.println (dim5); 
  dimmer5.set(dim5); // dimmer.set(0%-100%)
  int i =analogRead(0);
  Serial.println (1024-i);
  readButtonState();
    
  dimmer4.set(mainLAMP);
  RiseFallLamp(setLampState(stateL));
  delay(25);
 
  if (Serial.available()) 
  {
    Serial.flush();
    Serial.println(dimmer4.getValue());  
  }

}
