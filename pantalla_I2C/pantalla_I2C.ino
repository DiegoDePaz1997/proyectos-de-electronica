#include <Wire.h>
#include <Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>

#define ancho 128
#define alto 64

#define oled_reset 4
Adafruit_SSD1306 oled(ancho,alto,&Wire,oled_reset);

void setup(){
  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  
}



void loop(){
  
  oled.clearDisplay();
  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  oled.print("hola han pasado: ");
  oled.setCursor(10,30);
  oled.setTextSize(2);
  oled.print(millis()/1000);
  oled.print(" seg.");
  oled.display();
  oled.clearDisplay();
}
