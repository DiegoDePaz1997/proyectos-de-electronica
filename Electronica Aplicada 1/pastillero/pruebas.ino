#include <DS3231.h> 
#include <Wire.h>
DS3231 clock;

bool century= false;
bool h12Flag;
bool pmFlag;
byte year=1;
byte month=1;
byte date;
byte dow;
byte hour=23;
byte minute=0;
byte second=0;



void setup() {
  Serial.begin(9600);
  Wire.begin();

  
}



void loop() {

      delay(500);
      Serial.print(clock.getYear(), DEC);
      Serial.print("-");
      Serial.print(clock.getMonth(century), DEC);
      Serial.print("-");
      Serial.print(clock.getDate(), DEC);
      Serial.print(" ");
      Serial.print(clock.getHour(h12Flag, pmFlag), DEC); //24-hr
      Serial.print(":");
      Serial.print(clock.getMinute(), DEC);
      Serial.print(":");
      Serial.println(clock.getSecond(), DEC);
    




  //  clock.setYear(year);
  //  clock.setMonth(month);
 //   clock.setDate(date);
 //   clock.setDoW(dow);
    clock.setHour(hour);
    clock.setMinute(minute);
   // clock.setSecond(second);


  
    
  }










      
    
