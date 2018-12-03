
#include "Arduino.h"
#include "RBDdimmer.h"
#include <util/atomic.h>

#define _TCNT(X) TCNT ## X
#define TCNT(X) _TCNT(X)
#define _TCCRxA(X) TCCR ## X ## A
#define TCCRxA(X) _TCCRxA(X)
#define _TCCRxB(X) TCCR ## X ## B
#define TCCRxB(X) _TCCRxB(X)
#define _TIMSKx(X) TIMSK ## X
#define TIMSKx(X) _TIMSKx(X)
/* if defined chip with  8-bit timer */
#define _OCRxA(X) OCR ## X ## A
#define OCRxA(X) _OCRxA(X)
/* if defined chip with 16-bit timer */
#define _OCRxAH(X) OCR ## X ## AH
#define OCRxAH(X) _OCRxAH(X)
/* ********************************* */
#define _OCRxAL(X) OCR ## X ## AL
#define OCRxAL(X) _OCRxAL(X)

#define _TIFRx(X) TIFR ## X
#define TIFRx(X) _TIFRx(X)
#define _TIMER_COMPA_VECTOR(X) TIMER ## X ## _COMPA_vect
#define TIMER_COMPA_VECTOR(X) _TIMER_COMPA_VECTOR(X)


/*****
 * for AVR chip with
 * 8-bit timer
 *****/
#if DIMMER_TIMER == 2
#define TCCRxA_VALUE 0x02 // CTC mode
#define TCCRxB_VALUE 0x02 // Prescaler = 8 => Cycle = 0.5us @ 16MHz
#define OCRxA_VALUE 0x18

//#define OCRxA_VALUE 0x17
// CTC mode, Prescaler = 8 => Cycle = 0.5us @ 16MHz

/*****
 * for AVR_ATmega1280/2560 chip
 * 16-bit timer
 *****/
#elif (DIMMER_TIMER == 1 || DIMMER_TIMER == 4 || DIMMER_TIMER == 5)
#define TCCRxA_VALUE 0x00 // CTC mode
#define OCRxAH_VALUE 0x00
#define OCRxAL_VALUE 0x18
// CTC mode, Prescaler = 64 => Cycle = 4us @ 16MHz

/*****
 * for AVR_ATmega32U4 chip
 * 16-bit timer
 *****/
#elif (DIMMER_TIMER == 3 )
#define TCCRxA_VALUE 0x00 // CTC mode
#define OCRxAH_VALUE 0x00
#define OCRxAL_VALUE 0x18

// CTC mode, Prescaler = 64 => Cycle = 4us @ 16MHz

#endif

int dim_tim[10];
int dim_max[10];


//int dim_begin = 90;
int pulseWidth = 1;
volatile int current_dim = 0;
int all_dim = 3;
int rise_fall = true;
static int tog;
static int max_tim_for_tog = 1;
char user_zero_cross = '0';

static int toggleCounter = 0;
static int toggleReload = 25;

static dimmerLamp* dimmer[ALL_DIMMERS];
volatile int dimOutPin[ALL_DIMMERS];
volatile int zeroCross[ALL_DIMMERS];
volatile DIMMER_MODE_typedef dimMode[ALL_DIMMERS];
volatile ON_OFF_typedef dimState[ALL_DIMMERS];
volatile int dimCounter[ALL_DIMMERS];
static uint16_t dimPulseBegin[ALL_DIMMERS];
volatile uint16_t togMax[ALL_DIMMERS];
volatile uint16_t togMin[ALL_DIMMERS];
volatile bool togDir[ALL_DIMMERS];

dimmerLamp::dimmerLamp(int user_dimmer_pin):
	dimmer_pin(user_dimmer_pin)
{
	current_dim++;
	dimmer[current_dim-1] = this;
	current_num = current_dim-1;
	timer_num = DIMMER_TIMER;
	toggle_state = false;
	
	dimPulseBegin[current_dim-1] = 1;
	dimOutPin[current_dim-1] = user_dimmer_pin;
	dimCounter[current_dim-1] = 0;
	zeroCross[current_dim-1] = 0;
	dimMode[current_dim-1] = NORMAL_MODE;
	togMin[current_dim-1] = 0;
	togMax[current_dim-1] = 1;
}

void dimmerLamp::port_init(void)
{
	DDRB |= (1 << PORTB4);
	DDRD  &= ~(1 << PORTD2);
	PORTD |= (1<<PORTD2);
} 
 
void dimmerLamp::timer_init(void)
{
	TCCRxA(DIMMER_TIMER) &= ~(0xFF); // clean TCCRxA register
	TCCRxB(DIMMER_TIMER) &= ~(0xFF); // clean TCCRxB register

	TCCRxA(DIMMER_TIMER) = TCCRxA_VALUE;
	TCCRxB(DIMMER_TIMER) = TCCRxB_VALUE;
	TIMSKx(DIMMER_TIMER) = 0x02; ; //устанавливаем бит разрешения прерывания 1ого счетчика по совпадению с OCR1A(H и L)
	
	if	(DIMMER_TIMER == 2)
	{
		TCCRxB(DIMMER_TIMER) = 0x0A;
		OCRxA(DIMMER_TIMER) = OCRxA_VALUE;	
	}
	else if	(DIMMER_TIMER == 1 || DIMMER_TIMER == 4 || DIMMER_TIMER == 5)
	{
		TCCRxB(DIMMER_TIMER) |= ((1 << WGM12)|(1 << CS11)|(1 << CS10));
		OCRxAH(DIMMER_TIMER) = OCRxAH_VALUE;
		OCRxAL(DIMMER_TIMER) = OCRxAL_VALUE;
	}
	else if	(DIMMER_TIMER == 3)
	{
		TCCRxB(DIMMER_TIMER) |= ((1 << WGM12)|(1 << CS11)|(1 << CS10));
		OCRxAH(DIMMER_TIMER) = OCRxAH_VALUE;
		OCRxAL(DIMMER_TIMER) = OCRxAL_VALUE;
	}
	TIMSKx(DIMMER_TIMER) |= (1 << TOIE1); //timer interrupt enable
}

void dimmerLamp::ext_int_init(void) 
{ 
	EICRA &= ~0xFF; 
	
	if	(DIMMER_TIMER == 2)
	{
		EIMSK |= (1 << INT0); 
		EICRA |= (1 << ISC01)|(1 << ISC00);//0b00001100 	
	}
	else if	(DIMMER_TIMER == 1 || DIMMER_TIMER == 4 || DIMMER_TIMER == 5)
	{
		EIMSK |= (1 << INT4); 
		EICRB |= (1 << ISC41)|(1 << ISC40);//0b00001100 
	}
	else if	(DIMMER_TIMER == 3)
	{
		EIMSK |= (1 << INT1); 
		EICRA |= (1 << ISC11)|(1 << ISC10);//0b00001100 		
	}
}


void dimmerLamp::begin(DIMMER_MODE_typedef DIMMER_MODE, ON_OFF_typedef ON_OFF)
{
	dimMode[this->current_num] = DIMMER_MODE;
	dimState[this->current_num] = ON_OFF;
	port_init();
	pinMode(dimmer_pin, OUTPUT);
	timer_init();
	ext_int_init();	
}

void dimmerLamp::setPower(int power)
{
	if (power >= 90) 
	{
		power = 90;
		dimPulseBegin[this->current_num] = power;
	}
	if (0 <= power < 90) 
	{
		dimPulseBegin[this->current_num] = power + 5;
	}	
	delay(1);
}

int dimmerLamp::getPower(void)
{
	return dimPulseBegin[this->current_num];
}

void dimmerLamp::setState(ON_OFF_typedef ON_OFF)
{
	dimState[this->current_num] = ON_OFF;
}

bool dimmerLamp::getState(void)
{
	bool ret;
	if (dimState[this->current_num] == ON) ret = true;
	else ret = false;
	return ret;
}

void dimmerLamp::changeState(void)
{
	if (dimState[this->current_num] == ON) dimState[this->current_num] = OFF;
	else 
		dimState[this->current_num] = ON;
}

DIMMER_MODE_typedef dimmerLamp::getMode(void)
{
	return dimMode[this->current_num];
}

void dimmerLamp::setMode(DIMMER_MODE_typedef DIMMER_MODE)
{
	dimMode[this->current_num] = DIMMER_MODE;
}

void dimmerLamp::toggleSettings(int minValue, int maxValue)
{
	if (maxValue > 99) 
	{
    	maxValue = 99;
	}
	if (minValue < 5) 
	{
    	minValue = 5;
	}
	dimMode[this->current_num] = TOGGLE_MODE;
	togMin[this->current_num] = minValue;
	togMax[this->current_num] = maxValue;

	toggleReload = 25;
}


 
ISR(INT_vect)
{
	for (int i = 0; i < current_dim; i++ ) 
		if (dimState[i] == ON) zeroCross[i] = 1;
}


static int k;
ISR (TIMER_COMPA_VECTOR(DIMMER_TIMER))
{	
	toggleCounter++;
	for (k = 0; k < current_dim; k++)
	{
		
		if (zeroCross[k] == 1 )
		{
			dimCounter[k]++;
			
			if (dimMode[k] == TOGGLE_MODE)
			{
			/*****
			 * TOGGLE DIMMING MODE
			 *****/
			if (dimPulseBegin[k] >= togMax[k]) 	
			{
				// if reach max dimming value 
				togDir[k] = false;	// downcount				
			}
			if (dimPulseBegin[k] <= togMin[k])
			{
				// if reach min dimming value 
				togDir[k] = true;	// upcount
			}

			if (toggleCounter == toggleReload) 
			{
				if (togDir[k] == true) dimPulseBegin[k]++;
				else dimPulseBegin[k]--;
			}
			}
			
				/*****
				 * DEFAULT DIMMING MODE (NOT TOGGLE)
				 *****/
			if (dimCounter[k] >= dimPulseBegin[k] )
			{
				digitalWrite(dimOutPin[k], HIGH);							
			}

			if (dimCounter[k] >=  (dimPulseBegin[k] + pulseWidth) )
			{
				digitalWrite(dimOutPin[k], LOW);
				zeroCross[k] = 0;
				dimCounter[k] = 0;
			}
		}
	
	}			
	if (toggleCounter >= toggleReload) toggleCounter = 1;
	TIFRx(DIMMER_TIMER) |= ((1<<OCF1B)|(1<<OCF1A));	
}
