# faster-adc

This code aims at using the ADC unit of the ATMega328 more judiciously than the basic `analogRead()` function.<br>
The need arised when I had serious timing contrains that couldn't have been achieved reliably with Arduino alone.<br>
Channel A0 of the ADC had to be polled 8 times and all that data had to be stored in an SD card after dropping 2 LSBs from the reading.<br>
All this took 1.8 ms, and it wasn't that appreciable. I knew that this could be reduced without getting a better SD card, but I didn't know how I could do it.<br>
This is the attempt I made.

## How It Works:

I found in the datasheet that the first conversion takes 25 clock cycles and all the rest take 13, unless the ADC is disturbed and turned on again.<br>

Considering 1 clock cycle = 62.5 ns <br>
<br>
Total clock cycles = 25 + 13x7 = 116 <br>
<br>
Minimum time = 116x62.5 = 7250 ns<br>
<br>

#### Why is this more efficient? (according to my beliefs)

1. An analogRead always calls up the ADC unit (in case the pins had been defined as digital I/Os earlier), sets up the internal reference at Vcc, then sets the AD channel to A0, because the only info we send to the function is A0. <br>
  The code above does that only once. <br>
  So it latches to the Vcc reference, A0 analog channel, and the pins are defined as analog input only. Once and for all.<br>
2. The default setting of storing values in internal registers is left-adjusted. Since the ADC has a resolution of 10-bits, it takes 2 registers (ADCL and ADCH) to store any conversion. Now, keep in mind that ADCH will have the 2 MSBs and rest will be in ADCL. This is what left adjusted values mean, MSB towards left.<br>
  Now when reading these values, a 2 byte int has to be used, which concatenates the ADCL bits to ADCH and then stores it in the variable.<br>
  Right-adjusting the values fills the ADCH with 8 bits and ADCL with 2 bits, which have to be dropped ultimately, so there's no point reading them.<br>
  Therefore we read only one register and directly dump that value to the variable.<br>
