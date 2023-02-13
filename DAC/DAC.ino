//pines a utilizar de salida
// pin 2,3,4,5,6,7,8,9


int periodo = 5;
void setup() {
  Serial.begin(9600); 
    for(int i =2; i<11; i++)
 {
     pinMode(i, OUTPUT);
 }
}

void loop() {
//descomentas la funcion que queres que se ejecute
senoidal();
//diente_de_sierra();

}



void senoidal()
{
  int o =0;
 while(true)
 {
  int  a =128*sin(o/(3.14*32))+128;
    Serial.print(128); 
    Serial.print(",");     
    Serial.print(a);
    Serial.print(","); 
    Serial.print(-128); 
    Serial.print(","); 
    Serial.print(analogRead(A0));
    Serial.println(",");   
  delay(periodo);

  for( int i =2; i<11; i++)
{
  int aux = a&0b00000001;

  if(aux == 1)
  {
    digitalWrite(i, HIGH);  
  }
  else
  {
   digitalWrite(i, LOW);  
  }
  a= a>>1;
}
o+=1;
}
}






void diente_de_sierra()
{
 for( int au =-128; au<127; au++)
{
  int a = au;
    Serial.print(128); 
    Serial.print(",");     
    Serial.print(a);
    Serial.print(","); 
    Serial.print(-128); 
    Serial.print(","); 
    Serial.print(analogRead(A0));
    Serial.println(",");  
  delay(periodo);

 for( int i =2; i<11; i++)
{
  int aux = a&0b00000001;

  if(aux == 1)
  {
    digitalWrite(i, HIGH);  
  }
  else
  {
   digitalWrite(i, LOW);  
  }
  a= a>>1;
  
}
}
  
}
