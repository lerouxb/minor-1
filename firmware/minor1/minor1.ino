#include "notes.h"

/*
 Generating a tone using DDS
*/

unsigned int Acc;
unsigned int Pulse = 0;
int PulseAmount = 1;
unsigned int MinPulse = 65536 / 10;
unsigned int MaxPulse = 65536 - 65536/10;
unsigned int Note = 857; // Middle C

void setup() {
  
  // Enable 64 MHz PLL and use as source for Timer1
  PLLCSR = 1<<PCKE | 1<<PLLE;     

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                     // Timer interrupts OFF
  TCCR1 = 1<<CS10;               // 1:1 prescale
  GTCCR = 1<<PWM1B | 2<<COM1B0;  // PWM B, clear on match

  pinMode(PIN_PB4, OUTPUT);            // Enable PWM output pin

  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3<<WGM00;             // Fast PWM
  TCCR0B = 1<<WGM02 | 2<<CS00;   // 1/8 prescale
  TIMSK = 1<<OCIE0A;             // Enable compare match, disable overflow
  OCR0A = 49;                    // Divide by 50
  
  
  //Serial.begin(9600);
}

void loop() {

  unsigned int value = analogRead(A3); // 0 to 1023

  for (int i=0; i<4; ++i) {
    value += analogRead(A3); // 0 to 1023
  }
  value = value/5;
  
  //Serial.println(value); 

  unsigned int midpoint = 755;
  int key = -1;

  for (int i=0; i<13; ++i) {
      if (value > midpoint - 10 && value < midpoint + 10) {
        key = i;
        break;
      }
      midpoint += 20; 
  }
  
  //Serial.println(key);  


  if (key == -1) {
    pinMode(PIN_PB4, INPUT);
  }
  else {
    pinMode(PIN_PB4, OUTPUT);
    Note = notes[38 - 24 + key]; // this should be 39 but then it is too high. weird.
  }

}

// ADCSRA&=(~(1<<ADEN))


ISR(TIMER0_COMPA_vect) {

  // forwards/backwards
  if (PulseAmount > 0) {
    if (Pulse + PulseAmount > MaxPulse) {
      PulseAmount = -PulseAmount;
    }
  }
  else  {
    if (Pulse + PulseAmount < MinPulse) {
     PulseAmount = -PulseAmount;
    }
  }
  Pulse += PulseAmount; // assuming PulseAmount == 1 and we're going end to end (which we're not): 65556 / 20000 == ~3.25hz
  
  Acc = Acc + Note;
  //OCR1B = (Acc >> 8) & 0x80; // square: take the top bit of the accumulator
  //OCR1B = Acc >> 8;// saw: top 8 bits of the accumulator
  
  if (Acc < Pulse) {
    OCR1B = 255;
  }
  else {
    OCR1B = 0;
  }
  
  // triangle
  /*
  signed char temp, mask;
  Acc = Acc + Note;
  temp = Acc >> 8;
  mask = temp >> 7;
  //OCR1B = temp ^ mask;
  temp = temp ^ mask;
  OCR1B = temp << 1;
  */
}
