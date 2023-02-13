#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        12 // 

#define NUMPIXELS 64// Popular NeoPixel ring size


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 15 // Time (in milliseconds) to pause between pixels

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif





  
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  int r=4,v=0,a=4;
 for(int i=0; i<8;i++)
 {
 fila(i,32-i*r,0,i*a);
  delay(DELAYVAL);
 columna(i,0,i*v+5,0);
 }
    pixels.clear(); 
  }



void fila(int num_fila,int rojo,int verde, int azul){
  int fila[]={0,8,16,24,32,40,48,56};
  
  for(int i=0; i<8; i++) { 
    pixels.setPixelColor(i+ fila[num_fila] , pixels.Color(rojo, verde, azul));
    pixels.show();  
    delay(DELAYVAL); 

  }
  pixels.show(); 
}


void columna(int num_columna,int rojo,int verde, int azul){
  int columna[]={0,1,2,3,4,5,6,7};
  for(int i=0; i<8; i++) { 
    pixels.setPixelColor(8*i+ columna[num_columna] , pixels.Color(rojo, verde, azul));
    pixels.show();  
   delay(DELAYVAL);
  }
  pixels.show(); 
}
