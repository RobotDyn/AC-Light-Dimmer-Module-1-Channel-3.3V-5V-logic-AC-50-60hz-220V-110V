#ifndef RBDDIMMER_H
#define RBDDIMMER_H

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

typedef enum
{
    NORMAL_MODE = 0,
    TOGGLE_MODE = 1
} DIMMER_MODE_typedef;

typedef enum
{
    OFF = false,
    ON = true
} ON_OFF_typedef;

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define DIMMER_TIMER 4
#define INT_vect INT4_vect   

#elif defined(__AVR_ATmega32U4__)
#define DIMMER_TIMER 3
#define INT_vect INT1_vect

#else
#define DIMMER_TIMER 2
#define INT_vect INT0_vect
#endif



#define ALL_DIMMERS 10

class dimmerLamp 
{         
    private:
        int current_num;
		int timer_num;
        bool toggle_state;
        int tog_current;
		
		void port_init(void);
		static void timer_init(void);
		static void ext_int_init(void);
		
    public:   
        uint16_t pulse_begin;
        int dimmer_pin;
        int tog_max;
        int tog_min;

        dimmerLamp(int user_dimmer_pin);
        void begin(DIMMER_MODE_typedef DIMMER_MODE, ON_OFF_typedef ON_OFF);
        void setPower(int power);
		int  getPower(void);
		void setState(ON_OFF_typedef ON_OFF);
        bool getState(void);
		void changeState(void);
        void setMode(DIMMER_MODE_typedef DIMMER_MODE);
        DIMMER_MODE_typedef getMode(void);
        void toggleSettings(int minValue, int maxValue);  
};

#endif
