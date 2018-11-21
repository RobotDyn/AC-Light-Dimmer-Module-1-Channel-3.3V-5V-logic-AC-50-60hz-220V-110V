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
 
Dimmer dimmer(4,  DIMMER_NORMAL);//Dimmer dimmer(PWM_PIN,  DIMMER_NORMAL) Z-C use D2 pin
int i = 0; 
int value = 0; 
 
void setup() { 
  Serial.begin(9600); 
  dimmer.begin(); 
  dimmer.setMinimum(1);
  Serial.println("Dimmer Program is starting...");
} 

int maxValue = 100;
int minValue = -maxValue;
int val = minValue;

void printSpace(int val)
{
  if ((val / 100) == 0) Serial.print(" ");
  if ((val / 10) == 0) Serial.print(" ");
}

void loop() { 
/*для димминга потенциометром 
  ////////use for analog Potentiometer 
  value = map(analogRead(0), 0, 1024, 100, 0); // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
  Serial.println (value); 
  dimmer.set(value); // dimmer.set(0%-100%)
*/

/*цикл постепенного включения-выключения  */ 
  dimmer.set(abs(val)); //dimmer.set(0%-100%)
  val++;

/////вывод значений димминга на компорт
  Serial.print("% lampValue -> ");
  printSpace(abs(dimmer.getValue()));
  Serial.print(dimmer.getValue());  
/////вывод значений димминга на компорт
  
  if (val > maxValue) val = minValue;
  delay(50);
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
