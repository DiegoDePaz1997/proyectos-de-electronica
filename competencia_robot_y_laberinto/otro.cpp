#include <Arduino.h>
#include <Servo.h>
//--------------------------------------------------Lista de motores y sensores------------------------------------------------------------//
int sensor1 = 12; //Izquierda
int sensor2 = 11; //Derecha
int mot1_1 = 3; //IN1
int mot1_2 = 5; //IN2
int mot2_1 = 6; //IN3
int mot2_2 = 7; //IN4
int ardu2 = 2; //Señal al arduino 2
int ardu1 = 8;// Señal Arduino 
//--------------------------------------------------Lista de variables auxiliares----------------------------------------------------------//
int val1 = 0;
int val2 = 0;
//---------------------------------------------------Señales Ardu2-------------------------------------------------------------------------//
int sig1 = 4;
int sig2 = 5;
//------------------------------------------------- Variables De Giro Activado por Sensor--------------------------------------------------//
int adonde = 0;

void setup() {
  pinMode(sensor1,INPUT);
  pinMode(sensor2,INPUT);
  pinMode(mot1_1,OUTPUT);
  pinMode(mot1_2,OUTPUT);
  pinMode(mot2_1,OUTPUT);
  pinMode(mot2_2,OUTPUT);  
  Serial.begin(9600);
  pinMode(sig1,INPUT);
  pinMode(sig2,OUTPUT);
  pinMode(ardu1,OUTPUT);   
  pinMode(ardu2,OUTPUT); 
}

//------------------------------------------------------------Rutina para caminar hacia adelante-------------------------------------------//
void adelante(){
    analogWrite(mot1_1,120);
    digitalWrite(mot1_2,LOW);
    analogWrite(mot2_1, 125);
    digitalWrite(mot2_2,LOW);  
}
//------------------------------------------------------------Rutina por si se desvía ------------------------------------------------------//
void giro_izquierda(){
    digitalWrite(mot1_1,LOW);
    digitalWrite(mot1_2,LOW);
    analogWrite(mot2_1,180); //Aca se varia giro
    digitalWrite(mot2_2,LOW);
}
//------------------------------------------------------------Rutina para caminar hacia atras-------------------------------------------//
void atras(){
    digitalWrite(mot1_1,LOW);
    analogWrite(mot1_2,170);
    digitalWrite(mot2_1,LOW);
    analogWrite(mot2_2,170);
}
void atras1(){
    digitalWrite(mot1_1,LOW);
    analogWrite(mot1_2,200);
    digitalWrite(mot2_1,LOW);
    analogWrite(mot2_2,200);
}
//------------------------------------------------------------Rutina por si se desvía derecha-------------------------------------------//
void giro_derecha(){
     digitalWrite(mot1_2,LOW);
     analogWrite(mot1_1,180); //Aca se varia giro
     digitalWrite(mot2_2,LOW);
     digitalWrite(mot2_1,LOW);
}

//----------------------------------------------------------Detencion-----------------------------------------------------------------------
void detencion(){
     digitalWrite(mot1_1,LOW);
     digitalWrite(mot1_2,LOW);
     digitalWrite(mot2_1,LOW);
     digitalWrite(mot2_2,LOW);
}
void giro1(){
     digitalWrite(mot1_1,LOW);
     digitalWrite(mot1_2,LOW);
     digitalWrite(mot2_1,LOW);
     digitalWrite(mot2_2,LOW);
     delay(100);
     digitalWrite(mot1_1,LOW);
     analogWrite(mot1_2,130);
     digitalWrite(mot2_1,LOW);
     analogWrite(mot2_2,130);
     delay(300);
     analogWrite(mot1_2,130);
     digitalWrite(mot1_1,LOW);
     digitalWrite(mot2_2,LOW);
     analogWrite(mot2_1,130);
     delay(300); //Tiempo de giro
}


//---------------------------------------------------------VUELTA----------------------------------------------------------------/
void vuelta(){
    digitalWrite(mot1_1,LOW);
    digitalWrite(mot1_2,HIGH);
    digitalWrite(mot2_1,HIGH);
    digitalWrite(mot2_2,LOW);
}





//----------------------------------------------------Colores------------------------------------------------------------------------------//
void colores(){
  int col = analogRead(A0); // Lectura analoga
  Serial.println(col); //Para leer el valor analogo
  
  if(col < 100){  //Cuando detecta el color amarillo
    Serial.println("BLANCO");
  }
  else if(col >= 200 && col <= 300){  //Cuando detecta el color amarillo
    Serial.println("AMARILLO");
    adonde = 1;
    detencion();
    digitalWrite(ardu2,HIGH);
    delay(1);
    digitalWrite(ardu2,LOW);
    delay(12500);
    atras();
    delay(300);
    vuelta();
    delay(1000);
    }
  else if(col >450){  //Cuando detecta negro
    Serial.println("NEGRO");
    adonde = 0;
    detencion();
    digitalWrite(ardu1,HIGH);
    delay(1);
    digitalWrite(ardu1,LOW);
    delay(12500);
    atras();
    delay(300);
    vuelta();
    delay(1200);
  }
}




void loop() {
  val1 = digitalRead(sensor1); //sensor derecho 1 al detectar negro 
  val2 = digitalRead(sensor2); //sensor izquierdo 1 al detectar negro 
  colores();
  if(val1 == HIGH and val2 == LOW){//Una rueda
    //Serial.println("Rueda 1");
    detencion();
    delay(500);
    atras1();
    delay(160);
    giro_izquierda();
    delay(280);
    ;
  }
  else if(val1 == LOW and val2 == HIGH){//Negra solo en una rueda
    //Serial.println("Rueda 2");
    detencion();
    delay(500);
    atras1();
    delay(160);
    giro_derecha();
    delay(1000);
    
  }
   else if(val1 == LOW and val2 == LOW){//blanco
     //Serial.println("Adelante");
     adelante();
  }
   else if(val1 == HIGH and val2 == HIGH){//blanco
     //Serial.println("Adelante");
     atras();
     delay(350);
     if (adonde == 0){
        giro_derecha();
        delay(1500); //Este giro es para el ingreso del carro para recoger la caja.
     }
     else if (adonde == 1){
        giro_izquierda();
        delay(1500); //Este giro es para el regreso del carro llevando la caja.
     }

  }
}


