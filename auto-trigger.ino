#define TIMER 5000

//this class will create objects that will update automatically every ms.
class elapsedMillis
{
private:
    unsigned long ms;
public:
    elapsedMillis(void) { ms = millis(); }
    elapsedMillis(unsigned long val) { ms = millis() - val; }
    elapsedMillis(const elapsedMillis &orig) { ms = orig.ms; }
    operator unsigned long () const { return millis() - ms; }
    elapsedMillis & operator = (const elapsedMillis &rhs) { ms = rhs.ms; return *this; }
    elapsedMillis & operator = (unsigned long val) { ms = millis() - val; return *this; }
    elapsedMillis & operator -= (unsigned long val)      { ms += val ; return *this; }
    elapsedMillis & operator += (unsigned long val)      { ms -= val ; return *this; }
    elapsedMillis operator - (int val) const           { elapsedMillis r(*this); r.ms += val; return r; }
    elapsedMillis operator - (unsigned int val) const  { elapsedMillis r(*this); r.ms += val; return r; }
    elapsedMillis operator - (long val) const          { elapsedMillis r(*this); r.ms += val; return r; }
    elapsedMillis operator - (unsigned long val) const { elapsedMillis r(*this); r.ms += val; return r; }
    elapsedMillis operator + (int val) const           { elapsedMillis r(*this); r.ms -= val; return r; }
    elapsedMillis operator + (unsigned int val) const  { elapsedMillis r(*this); r.ms -= val; return r; }
    elapsedMillis operator + (long val) const          { elapsedMillis r(*this); r.ms -= val; return r; }
    elapsedMillis operator + (unsigned long val) const { elapsedMillis r(*this); r.ms -= val; return r; }
};

elapsedMillis timer;

void setup()
{
    //general functionality
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    timer = 0;
}

void loop()
{
    ADMUX = 0b01100000; //INSTR2

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

    if(timer > TIMER)
        while(1);
}