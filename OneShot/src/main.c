// Clock speed 128 kHz
// Timer: prescaler 1/8
// PWM ~68Hz
#define F_CPU  128000
#include <stdio.h>
#include <avr/io.h>                                 
#include <util/delay.h>
#include <avr/interrupt.h>

#define SETBIT(P, BIT)	((P) |= (1<<(BIT)))
#define CLEARBIT(P,BIT)	((P) &= ~(1<<(BIT)))
#define CHECKBIT(P,BIT) ((P) & (1<<BIT)) 

#define RED_LED		PIN2
#define GRN_LED		PIN1
#define BLUE_LED	PIN0
#define LAST		2

#define DELAY		20

char led_state[3] = {RED_LED, GRN_LED, BLUE_LED};

char led_pwm[3] = {0, 0, 0};
char led_idx = 0;
char k, i, on = 0;

int main (void) 
{
    DDRB |= (1<<RED_LED) | (1<<GRN_LED) | (1<<BLUE_LED);

	TIMSK0 |= 1<<OCIE0B;      
	TCCR0B |= 1<<CS01;

	sei();

	while(1)
	{
		led_pwm[on] = 255-1;
		for(k = 0; k<255-1; k++)
		{
			if(on != 0) led_pwm[0] = k;
			if(on != 1) led_pwm[1] = k;
			if(on != 2) led_pwm[2] = k;
			_delay_ms(DELAY);
		}
		on++;
		if(on == 3) on = 0;
  	}

	return 0;
}

ISR(TIM0_COMPB_vect)
{
	TCCR0B &= 0b11111000;     //disable counter
	if(led_idx == 0)
	{
		CLEARBIT(PORTB, led_state[LAST]);
		SETBIT(PORTB, led_state[led_idx]);
	}
	else
	{
		CLEARBIT(PORTB, led_state[led_idx-1]);
		SETBIT(PORTB, led_state[led_idx]);
	}

	OCR0B = led_pwm[led_idx];

	led_idx++;
	if(led_idx == 3) led_idx = 0;

	TCNT0 = 0;
	TCCR0B |= 1<<CS01;
}
