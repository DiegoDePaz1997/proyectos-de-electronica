/*
sistema que verifica la temperatura de las personas ('si tenían covid')
  superaban el umbral---> no se les dejaba pasar y activaba una alarma
  sino ----> pasaban a la cabina de rociado 

  para el rociado existían dos botes uno pequeño que era el 
  principal y otro mayor, secundario
  cuando el pequeño llegaba a cieto humbral se activaba una bomba
  que la llenaba con el líquido del segundo tanque

*/



#include <Wire.h>
#include <Adafruit_MLX90614.h>  // pines  sda a4 scl a5



//Altura de los tanques (Cm)
float H_N[]={40, 40};



//estado anterior del tanque
//si al realizar una nueva medicion esta sobrepasa el +-10% se toma otra
//medicion para asegurar que no hubo error, asi hasta 4 veces
//si la diferencia mayor al 10% persiste se toma al 5 intento
float estado []={0,0};
//variables de conteo
byte del_ultrasonido=0;
byte del_temperatura=0;
//velocidad del sonido
float V_sonido= ((331.1+(0.606*25))/10000)/2;// cm/s


//Para el sensor de temperatura
Adafruit_MLX90614 mlx=Adafruit_MLX90614();
boolean condicion= false;
byte cont_tem_alarma=0;
boolean aceptar_temperatura= false;
byte cont_para_pasar=0;
boolean led_verde=false;
boolean cny70= false;


bool alarma_n = true; // condicion para el buzzer
bool R = true; // es para un temporizador 



void setup() {
//-----------------------------------------------------------
//configuracion de pines de los sensores de ultrasonido
//#1
pinMode(2,OUTPUT); //echo entrada 
pinMode(3,INPUT);  //output       
//#2
pinMode(4,OUTPUT); //echo entrada  
pinMode(5,INPUT); // output   

//pines para las bombas  tanque principal (1_galon) cuando llegue al 30% debe llenarce al 100%
pinMode(6,OUTPUT); //control de bomba #1 

//pines alarmas
pinMode(7,OUTPUT); //luz amarilla y       alarma audible buzzer


//luz roja y alarma del sensor de temperatura
pinMode(10,OUTPUT); //luz roja  y          alarma audible buzzer

//luz verde y   bomba sprays 
pinMode(9,OUTPUT);
//bomba accionadora de sprays pin 8
pinMode(8,OUTPUT);




//cny70 para verificacion que la persona ingreso a la cabina
pinMode(11,INPUT);

//pin 12 alarma buzzer
pinMode(12, OUTPUT);

//-----------------------------------------------------------


//puerto serial
Serial.begin(9600);
//-----------------------------------------------------------
//sensor de temperatura
mlx.begin();

}

    

void loop() {

//saco el promedio de 4 sensadas para mejorar la fiabilidad 
del_ultrasonido+=1;
if(del_ultrasonido==3)
{
  float senn1=100*(H_N[0] - medicion_distancia_sensor(0))/H_N[0];
  float senn2=100*(H_N[1] - medicion_distancia_sensor(1))/H_N[1];
  
  if(senn1< 30*0.75)digitalWrite(6,HIGH);//llenado de tanque 1
  if(senn1>100*0.75)digitalWrite(6,LOW); // detencion de llenado de tanque 1

  if(senn2< 20*0.75){digitalWrite(7,HIGH); digitalWrite(12,HIGH); alarma_n=true;}//alarma visual y audible activa al 20%
  if(senn2>23*0.75){      // desactivacion alarmas
    digitalWrite(7,LOW); 
    if(alarma_n){
     digitalWrite(12, LOW);
     alarma_n = false;  
    }
    
    }  
  
  del_ultrasonido=0;  
 

}



  
  float med= mlx.readObjectTempC();
  //sensor de temperatura
  
  //encendido de alarmas por tener mas de 38C
  if(med>=38){ digitalWrite(10,HIGH); digitalWrite(12,HIGH); alarma_n=false; aceptar_temperatura== false; condicion=true;cont_tem_alarma=25; digitalWrite(9,LOW); digitalWrite(8,LOW);} //tiempo de alarmas por covid
   //condiciones para apagar las alarmas
  if(condicion){ cont_tem_alarma-=1;}
  if(condicion==true and cont_tem_alarma==0){
    condicion=false;digitalWrite(10,LOW);
        if(!alarma_n){
         digitalWrite(12, LOW);
         alarma_n = true;  
    }
    }



  //secuencia para el paso de la persona, a cumplir
  if(med>=32 and med<38)    aceptar_temperatura=true;
  if(digitalRead(11)==true and cny70==false and aceptar_temperatura==true)  cny70=true;
  if(digitalRead(11)==false and cny70==false and aceptar_temperatura==true)  digitalWrite(9, HIGH);

  
  //si la persona ingresa
  if(aceptar_temperatura and led_verde==false and cny70==true and condicion==false ) { digitalWrite(9,HIGH); digitalWrite(8,HIGH); led_verde=true;cont_para_pasar=0;}
 
  if(led_verde==true)cont_para_pasar+=1;
  if(led_verde==true and cont_para_pasar==50){digitalWrite(9,LOW); digitalWrite(8,LOW);cont_para_pasar=0;aceptar_temperatura=false;led_verde=false;cny70=false;}
 

 //si la persona no ingresa a la cabina
  if(aceptar_temperatura==true and led_verde==false)cont_para_pasar+=1;
  if(cont_para_pasar==60){aceptar_temperatura=false; cont_para_pasar=0; digitalWrite(9, LOW); aceptar_temperatura=false;}


 





delay(100);
}





//para n=0 es el sensor 1 de 1galon,  n=1 es el segundo sensor  5galones     
float medicion_distancia_sensor(byte n)
{
  float pulse;
//medicion sensor
for(int i=0; i<3; i++)//si hay una rariacion de +-15% se repite la medicion, hata 3 veces
{
  digitalWrite(2+ 2*n, HIGH);
  delayMicroseconds(10);
  digitalWrite(2+ 2*n,LOW);
  pulse=pulseIn(3+ 2*n,HIGH);
  if(pulse<estado[n]*1.15 and pulse>estado[n]*0.85)
  {
    estado[n]=pulse;
    return pulse*V_sonido; 
  }
  delay(2);  
}
  estado[n]=pulse;
  return pulse*V_sonido;

}
