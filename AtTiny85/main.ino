
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>

#define wemosTrigger 0
#define signalPin 1

#define WEMOS_ACTIVE_TIME 10 // percentage 0 to 100
// sleep bit patterns:
//  1 second:  0b000110
//  2 seconds: 0b000111
//  4 seconds: 0b100000
//  8 seconds: 0b100001
#define WATCHDOG_SLEEP_BITS 0b100001

ISR(WDT_vect) 
  {
  wdt_disable();  
  }

void myWatchdogEnable(const byte interval) 
  {
  noInterrupts();
  wdt_reset();
    
  MCUSR = 0;                          // reset various flags
  WDTCR |= 0b00011000;               // see docs, set WDCE, WDE
  WDTCR =  0b01000000 | interval;    // set WDIE, and appropriate delay
  
  ADCSRA &= ~_BV(ADEN);

  // Disable all modules
  ADCSRA &= ~ bit(ADEN); // disable the ADC
  power_all_disable();

  set_sleep_mode (SLEEP_MODE_PWR_DOWN); 
  sleep_bod_disable();
  interrupts();
  sleep_mode();     
  
  } 

void wakeWemos(){
  digitalWrite(signalPin, LOW);
  digitalWrite(wemosTrigger, LOW);
  delay(500);
  digitalWrite(wemosTrigger, HIGH);
}


void sleepWemos() {
  digitalWrite(signalPin, HIGH);
  digitalWrite(wemosTrigger, LOW);
  delay(500);
  digitalWrite(wemosTrigger, HIGH);
}

void waitWemosActiveTime() {
  for (int i = 0; i < WEMOS_ACTIVE_TIME; i++){
    myWatchdogEnable (WATCHDOG_SLEEP_BITS);
  }
}

void waitWemosSleepTime() {
  for (int i = 0; i < 100 - WEMOS_ACTIVE_TIME; i++){
    myWatchdogEnable (WATCHDOG_SLEEP_BITS); 
  }
}

void setup()
{
pinMode (wemosTrigger, OUTPUT);
pinMode(signalPin, OUTPUT);
}  

void loop(){
  wakeWemos();
  waitWemosActiveTime();
  sleepWemos();
  waitWemosSleepTime();
}  
