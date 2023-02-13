#include <Arduino.h>
#include "mov.h"
#include "Datos.h"



//motores
#define mot_der_1 3
#define mot_der_2 5
#define mot_izq_1 6
#define mot_izq_2 7
#define der true
#define izq false
bool giro = der;

//sensores
#define sensor_derecha 11
#define sensor_izquierda 12
#define sensor_enmedio A0

//variables de la sensada continua
byte dato_der [2] ;
byte dato_izq [2] ;
byte dato_med [2] ;
byte n_ = 0; //van a ir cambiando el actual por el anterior y el anterior por el actual, de los vectores
byte N_ = 1;


mov M;
Datos D;
//para toma de datos de la funcion de la clase
#define moda_izq 0
#define moda_der 1
#define prom_color 2




void save_data(byte i){// i =val_actual  i = val_anterior
dato_der [i] = D.datos(moda_der);
dato_izq [i] = D.datos(moda_izq);
dato_med [i] = D.datos(prom_color);  
}




//funcion para seleccion
 void estado(byte dat_izq_n, byte dat_der_n, byte dat_izq_N, byte dat_der_N){


   if(dat_izq_n == blanco and dat_der_n == blanco){

        if(dat_izq_N == negro and dat_der_N == blanco){M.correc_izq();}
        else if(dat_izq_N == blanco and dat_der_N == negro){M.correc_der();}
        else if(dat_izq_N == negro and dat_der_N == negro){M.giro_izq_der(giro);}
        else{M.adelante();}
   }


   else if(dat_izq_n == negro and dat_der_n == blanco){

        if(dat_izq_N == negro and dat_der_N == blanco){M.correc_izq();}
        else if(dat_izq_N == blanco and dat_der_N == blanco){M.adelante();}
        else if(dat_izq_N == blanco and dat_der_N == negro){M.giro_izq_der(giro);}
        else{M.giro_izq_der(giro);}
   }


   else if(dat_izq_n == blanco and dat_der_n == negro){

        if(dat_izq_N == blanco and dat_der_N == negro){M.correc_der();}
        else if(dat_izq_N == blanco and dat_der_N == blanco){M.adelante();}
        else if(dat_izq_N == negro and dat_der_N == blanco){M.giro_izq_der(giro);}
        else{M.giro_izq_der(giro);}
   }

   else{
      M.giro_izq_der(giro);
   }
 }





void setup() {
  pinMode(sensor_derecha,INPUT);
  pinMode(sensor_izquierda,INPUT);
  pinMode(sensor_enmedio, INPUT);
  pinMode(mot_der_1,OUTPUT);
  pinMode(mot_der_2,OUTPUT);
  pinMode(mot_izq_1,OUTPUT);
  pinMode(mot_izq_2,OUTPUT);  
  //Serial.begin(9600);   

D.sensado();
save_data(0);
D.sensado();
save_data(1);  
}




void loop() {

//n_ == comienza como anterior     N_ == comienza como actual actual
estado(dato_izq[0], dato_der[0], dato_izq[1], dato_der[1]);
D.sensado();
save_data(0);
estado(dato_izq[1], dato_der[1], dato_izq[0], dato_der[0]);
D.sensado();
save_data(1);

  
}






