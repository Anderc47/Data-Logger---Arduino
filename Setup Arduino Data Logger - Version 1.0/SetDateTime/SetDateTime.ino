#include <DS3231_Simple.h>

DS3231_Simple Clock;

#define SERIAL_BAUD 9600


void setup() {
  
  
  Serial.begin(SERIAL_BAUD);
  Clock.begin();
  
  Serial.println();
  Serial.println();
}

void loop() 
{ 
  // Create a variable to hold the data 
  DateTime MyTimestamp;
  
  // Load it with the date and time you want to set, for example
  // Example is set to August 4th 2022 at 13:07:30 
  //Please set parameters to current time +10 sec upload time
  MyTimestamp.Day    = 23;
  MyTimestamp.Month  = 8;
  MyTimestamp.Year   = 22; 
  MyTimestamp.Hour   = 14;
  MyTimestamp.Minute = 21;
  MyTimestamp.Second = 30;
  
  // Then write it to the clock
  Clock.write(MyTimestamp);
  
  // And use it, we will read it back for example...  
  Serial.print("The time has been set to: ");
  Clock.printTo(Serial);
  Serial.println();
  
  // Remember, once you set the time, the clock remembers it and keeps
  // running even if you reset or turn off your Arduino, as long as
  // the clock has battery power.
  
  Serial.print("End Of Program (RESET to run again)");
  while(1);
}
