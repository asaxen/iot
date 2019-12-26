/* ATtiny85 Sony NEX/Alpha Remote Control

   David Johnson-Davies - www.technoblogy.com - 12th April 2015
   ATtiny85 @ 1 MHz (internal oscillator; BOD disabled)
   
   CC BY 4.0
   Licensed under a Creative Commons Attribution 4.0 International license: 
   http://creativecommons.org/licenses/by/4.0/
   Tutorial: http://www.technoblogy.com/show?VFT
   
*/

#include <avr/sleep.h>

// Pin assignments
const int IRout = 1;   // OC0B
const int Shutter = 4;
const int Video = 2;
const int LED = 3;

// Remote control
const int Address = 0x1E3A;
const int ShutterCode = 0x2D;
const int TwoSecsCode = 0x37;
const int VideoCode = 0x48;

// IR transmitter **********************************************

const int top = 24;    // 1000000/25 = 40kHz
const int match = 18;  // pulses with approx 25% mark/space ratio

// Set up Timer/Counter0 to output PCM on OC0B
void SetupPCM () {
  TCCR0A = 3<<COM0B0 | 3<<WGM00; // Inverted output on OC0B and Fast PWM
  TCCR0B = 1<<WGM02 | 1<<CS00;   // Fast PWM and divide by 1
  OCR0A = top;                   // 40kHz
  OCR0B = top;                   // Keep output low
}

// Set up Pin Change interrupts on button pins
void SetupPinChange () {
  PCMSK = 1<<Shutter | 1<<Video;
  // Enable interrupt
  GIMSK = GIMSK | 1<<PCIE;
}

void Pulse (int carrier, int gap) {
  int count = carrier;
  OCR0B = match;  // Generate pulses  
  for (char i=0; i<2; i++) {
    for (int c=0; c<count; c++) {
      do ; while ((TIFR & 1<<TOV0) == 0);
      TIFR = 1<<TOV0;
    }
  count = gap;
  OCR0B = top;
  }
}

void SendSony (unsigned long code) {
  TCNT0 = 0;             // Start counting from 0
  // Send Start pulse
  Pulse(96, 24);
  // Send 20 bits
  for (int Bit=0; Bit<20; Bit++) {
    if (code & ((unsigned long) 1<<Bit)) Pulse(48, 24); else Pulse(24, 24);
  }
}

// Transmit code and light LED
void Transmit (int address, int command) {
  unsigned long code = (unsigned long) address<<7 | command;
  digitalWrite(LED, 1);
  SendSony(code);
  //delay(11);
  Pulse(0,440);
  SendSony(code);
  digitalWrite(LED, 0);
}

// Setup **********************************************
  
void setup() {
  TIMSK = 0;
  pinMode(Shutter, INPUT_PULLUP);
  pinMode(Video, INPUT_PULLUP);
  pinMode(IRout, OUTPUT);
  pinMode(LED, OUTPUT);
  // Disable ADC to save power
  ADCSRA &= ~(1<<ADEN);
  SetupPCM();
  SetupPinChange();
}

void loop() {
  // Come here after pin change interrupt wakes us from sleep
  if (!digitalRead(Shutter)) Transmit(Address, ShutterCode);
  else if (!digitalRead(Video)) Transmit(Address, VideoCode);
  delay(500);
}