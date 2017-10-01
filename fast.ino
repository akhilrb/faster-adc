//Setting up the ADC unit and right adjusting the result
//because you're dropping two LSBs. Read bottom notes for
//more
ADMUX = _BV(REFS0) | _BV(ADLAR);  //INSTR1

//Try once with the above instruction and once with the following.
//Both do the same thing, I'm just not sure which one
//will be more efficient.
ADMUX = 0b01100000; //INSTR2

//Comment INSTR1 if using INSTR2

//The above register states are fixed and need not be changed
//so we *might* save some CPU cycles here
//by not calling analogRead repeatedly.

//Pin 5 goes High
PORTD |= 0b00100000;

//Start Conversion
ADCSRA |= _BV(ADSC);
while (bit_is_set(ADCSRA,ADSC));

//Read 8-bits (one register) and store directly.
//Otherwise, 10 bits are written, read,
//then LSBs are dropped, then stored.
uint8_t a = ADCH;

//Repeating for all variables
ADCSRA |= _BV(ADSC);
while (bit_is_set(ADCSRA,ADSC));
uint8_t b = ADCH;
ADCSRA |= _BV(ADSC);
while (bit_is_set(ADCSRA,ADSC));
uint8_t c = ADCH;
ADCSRA |= _BV(ADSC);
while (bit_is_set(ADCSRA,ADSC));
uint8_t d = ADCH;

//Pin 5 goes low
PORTD |= ~(0b00100000);

//SD related
dataFile.write((const uint8_t *)&a, sizeof(a));
dataFile.write((const uint8_t *)&b, sizeof(b));
dataFile.write((const uint8_t *)&c, sizeof(c));
dataFile.write((const uint8_t *)&d, sizeof(d));

//Pin 6 goes High
PORTD |= 0b01000000;

//Start Conversion
ADCSRA |= _BV(ADSC);
while (bit_is_set(ADCSRA,ADSC));

//Read 8-bits (one register) and store directly.
//Otherwise, 10 bits are written, read,
//then LSBs are dropped, then stored.
uint8_t a = ADCH;

//Repeating for all variables
ADCSRA |= _BV(ADSC);
while (bit_is_set(ADCSRA,ADSC));
uint8_t b = ADCH;
ADCSRA |= _BV(ADSC);
while (bit_is_set(ADCSRA,ADSC));
uint8_t c = ADCH;
ADCSRA |= _BV(ADSC);
while (bit_is_set(ADCSRA,ADSC));
uint8_t d = ADCH;

//Pin 6 goes low
PORTD |= ~(0b01000000);

//SD related
dataFile.write((const uint8_t *)&a, sizeof(a));
dataFile.write((const uint8_t *)&b, sizeof(b));
dataFile.write((const uint8_t *)&c, sizeof(c));
dataFile.write((const uint8_t *)&d, sizeof(d));
/*
I found in the datasheet that one conversion the first conversion
takes 25 clock cycles and all the rest take 13, unless the ADC is
disturbed and turned on again.
Considering 1 clock cycle = 62.5 ns
Total clock cycles = 25 + 13*7 = 116
Minimum time = 116*62.5 = 7250 ns

How this code is more efficient (according to my beliefs):
1. An analogRead always calls up the ADC unit (in case the pins had been
  defined as digital I/Os earlier), sets up the internal reference at
  Vcc, then sets the AD channel to A0, because the only info we send
  to the function is A0.
  The code above does that only once. So it latches to the Vcc reference,
  A0 analog channel, and the pins are defined as analog input only. Once and
  for all. You didn't use any other analog pin, which is why I think this
  code will create some difference.
2. The default setting of storing values in internal registers is
  left-adjusted. Since the ADC has a resolution of 10-bits, it takes 
  2 registers (ADCL and ADCH) to store any conversion. Now, keep in mind
  that ADCH will have the 2 MSBs and rest will be in ADCL. This is what
  left adjusted values mean, MSB towards left.
  Now when reading these values, a 2 byte int has to be used, which
  concatenates the ADCL bits to ADCH and then stores it in the variable.
  Right-adjusting the values fills the ADCH with 8 bits and ADCL with 2 bits,
  which have to be dropped ultimately, so there's no point reading them.
  Therefore we read only one register and directly dump that value to the
  variable.
*/