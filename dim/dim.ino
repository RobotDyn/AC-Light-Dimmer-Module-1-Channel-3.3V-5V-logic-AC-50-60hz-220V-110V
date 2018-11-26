#include <Dimmer.h> //ссыдка на видоизмененную библиотеку //первоначальная библиоетка https://github.com/circuitar/Dimmer
 
/** 
 * Use count mode to switch the load on and off only when the AC voltage crosses zero. In this 
 * mode, the power is controlled by turning the triac on only for complete (half) cycles of the AC 
 * sine wave. The power delivery is adjusted by counting the number of cycles that are activated. 
 * This helps controlling higher, slower response loads (e.g. resistances) without introducing 
 * any triac switching noise on the line. 
 * 
 * Copyright (c) 2016 Circuitar 
 * This software is released under the MIT license. See the attached LICENSE file for details. 
 */  

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
//для димминга потенциометром 
  ////////use for analog Potentiometer 
  dim5 = map(analogRead(0), 1, 1024, 100, 15); // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
  Serial.println (dim5); 
  dimmer5.set(dim5); // dimmer.set(0%-100%)
  int i =analogRead(0);
  Serial.println (1024-i);
  readButtonState();
  
  //dimmer4.set(mainLAMP, setLampState(stateL));  
  dimmer4.set(mainLAMP);
  RiseFallLamp(setLampState(stateL));
  delay(25);
  //Serial.println(setLamp);

/*цикл постепенного включения-выключения  */ 
//  dimmer.set(abs(val)); //dimmer.set(0%-100%)
//  val++;
//
//вывод значений димминга на компорт
  //Serial.print("lampValue -> ");
//  printSpace(abs(dimmer.getValue()));
  
  if (Serial.available()) 
  {
    Serial.flush();
    Serial.println(dimmer4.getValue());  
  }
///////вывод значений димминга на компорт
//  
//  if (val > maxValue) val = minValue;
//  delay(20);
/*цикл постепенного включения-выключения  */

/*управление через компорт 
  pre_val = val;
  
  if (Serial.available())
  {
    int buf = Serial.parseInt();
    if (buf != 0) val = buf;
    delay(200);
  }
  dimmer.set(abs(val), true);

  if (pre_val != val)
  {
    
    Serial.print("% lampValue -> ");
    printSpace(abs(dimmer.getValue()));
    Serial.print(dimmer.getValue());  

  }
  delay(50);
управление через компорт */
}
