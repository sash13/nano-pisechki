#include "main.h"

char i = 0, ii, k, idx, _delay = 0, say;

void goSleep(void)
{
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    TCNT0 = 255 - 31;    //* Reset timer ((128000/8)0.002)-1 = 31
    TCCR0B |= (1<<CS01); //* clk/8

    sleep_mode();        //* go to sleep
    //* return
    sleep_disable();
}

void pixel(uint8_t ii)
{
    char temp1 = pgm_read_byte(&(led_mask[ii/2]));
    char temp2 = pgm_read_byte(&(led_state[ii]));
    DDRB  |= temp1;  //* Configure pins for output
    PORTB |= temp2;  //* Write pins state

    goSleep();
    //_delay_ms(DELAY);

    PORTB &= ~temp2; 
    DDRB &= ~temp1; //* Configure pins for input
}

#ifdef LED_TEST
void pixel_test(uint8_t ii) //* Duplicate becouse gcc create strange overhead 
                            //* binary when pass 2 vars to function
{
    char temp1 = pgm_read_byte(&(led_mask[ii/2]));
    char temp2 = pgm_read_byte(&(led_state[ii]));
    DDRB  |= temp1;  //* Configure pins for output
    PORTB |= temp2;  //* Write pins state
    _delay_ms(250);
    PORTB &= ~temp2; 
    DDRB &= ~temp1; //* Configure pins for input
}
#endif

#ifdef MESSAGE
void print_char(uint8_t ch) 
{
    idx = 0;
    say = ch  - 'A';

    while(_delay<DELAY_OBJ)
    {
        for(i = 0; i<3; i++)                  //* Index over byte
        {
            char temp1 = pgm_read_byte(&(_chars[say][i]));     //* Load byte
            uint8_t shift = (i==2)?4:8;       //* Get shift for byte or half byte
            for(k = 0; k<shift; k++)
            {
                if((temp1 >> k & 0x1))        //* Check if bit set
                {
                    pixel(idx);    
                }
                idx++;                        //* Index of LED
            }
        }
        idx = 0;
        _delay++;
    }
    _delay =0;
}
#endif

int main(void)
{         
    DDRB &= ~LED_BIT;                         //* Init pins for input (Z - state)
  
    TCCR0A &= ~((1<<COM0A1) | (1<<COM0A0));   //* Normal timer mode
    TIMSK0 |= (1<<TOIE0);                     //* enable overflow interrupt

    sei();

    while (1)
    {
        #ifdef LED_TEST
        //* Run bits from 1 to 20
        for(i = 0; i<20; i++)
        {
            pixel_test(i);
        }
        //pixel(1);
        //* Run bits from 20 to 1
        for(i = 19 ; i>=0; i--)
        {
            pixel_test(i);
        } 
        #endif
        #ifdef H_LINES
        //* Horizontal lines
        for(i = 0; i<5; i++)
        {
            while(_delay<DELAY_OBJ)             //* Reapeat couple time for visualize multiled
            {
                for(k = 0; k<4; k++)
                {
                    idx = i*4+k;                //* Correct index
                    pixel(idx); 
                }
                _delay++;
            }
            _delay = 0;                         //* Clean counter of repeats
        }
        #endif
        _delay = 0;
        #ifdef V_LINES
        //* Vertical lines
        for(i = 0; i<4; i++)
        {
            while(_delay<DELAY_OBJ)
            {
                for(k = 0; k<5; k++)
                {
                    idx = i+k*4;
                    pixel(idx);  
                }
                _delay++;
            }
            _delay = 0; 
        }
        #endif
        _delay = 0; 
        #ifdef LED_ALL
        //* All pins show up!
        while(_delay<DELAY_OBJ)
        {
            for(i = 0; i<20; i++)
            {
                pixel(i);   
            }
            _delay++;
        }
        _delay = 0; 
        #endif
        #ifdef MESSAGE
        for(ii = 0; ii<MESSAGE_LEN; ++ii)
        {
            char letter = pgm_read_byte(&(message[ii]));
            if(letter == ' ')                               //* check if space
            {
                letter = 'A' + 26;                          //* correct index for synthetic space char
            }
            print_char(letter);
        }
        #endif
    }
}

//* interrupt when timer counter overflow
ISR(TIM0_OVF_vect)
{
    TCCR0B &= ~(1<<CS01); //* disable clk/8    
}
