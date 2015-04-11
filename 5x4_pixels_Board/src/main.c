#include "main.h"

char i = 0, ii, k, idx, _delay = 0, say;

void pixel(uint8_t ii)
{
    char temp1 = pgm_read_byte(&(led_mask[ii/2]));
    char temp2 = pgm_read_byte(&(led_state[ii]));
    DDRB  |= temp1;  //* Configure pins for output
    PORTB |= temp2;  //* Write pins state
    _delay_ms(DELAY);
    PORTB &= ~temp2; 
    DDRB &= ~temp1; //* Configure pins for input
}

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

void print_char(uint8_t ch) 
{
    idx = 0;
    say = ch  - 'A';

    while(_delay<DELAY_OBJ)
    {
        for(i = 0; i<3; i++)                  //* Index over byte
        {
            uint8_t shift = (i==2)?4:8;       //* Get shift for byte or half byte
            for(k = 0; k<shift; k++)
            {
                char temp1 = pgm_read_byte(&(_chars[say][i]));     //* Check if bit set
                if((temp1 >> k & 0x1))
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

int main()
{             
  
  DDRB &= ~LED_BIT;                         //* Init pins for input (Z - state)

  while (1)
  {
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
    //* Horizontal lines
    for(i = 0; i<5; i++)
    {
        while(_delay<DELAY_OBJ)             //* Reapeat couple time for visualize multiled
        {
            for(k = 0; k<4; k++)
            {
                idx = i*4+k;            //* Correct index
                pixel(idx); 
            }
            _delay++;
        }
        _delay = 0;                         //* Clean counter of repeats
    }
    _delay = 0;
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

    for(ii = 0; ii<MESSAGE_LEN; ++ii)
    {
        print_char(pgm_read_byte(&(message[ii])));
    }

  }
}
