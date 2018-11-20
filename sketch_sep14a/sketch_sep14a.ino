#include <Dimmer.h>

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

#include "Dimmer.h"

Dimmer dimmer(4,  DIMMER_NORMAL);//Dimmer dimmer(PWM_PIN,  DIMMER_NORMAL)
int i = 0;
int value = 0;

void setup() {
  Serial.begin(9600);
  dimmer.begin();
}

void loop() {
  //uncomment for auto dimmer
// if (i == 0)
//  {
//    for (value=0; value<80; value ++)
//    {
//      dimmer.set(value);
//      Serial.print("Power: ");
//      Serial.print(value);
//      Serial.println("%");   
//      delay(50); 
//    }
//    i=1;   
//  }
//   if (i == 1)
//  {
//    for (value = 80; value > 1; value --)
//    {
//      dimmer.set(value);
//      Serial.print("Power: ");
//      Serial.print(value);
//      Serial.println("%");   
//      delay(50); 
//    }
//    i=0;   
//  }
  ////////use for analog Potentiometer
  value = map(analogRead(0), 1, 720, 70, 0);
  Serial.println (value);
  dimmer.set(value);

}