/*
codigo de un robot que recorre un laberinto, llega al centro donde
detecta un cubo, lo toma, se da la vuelva y sale del laberinto de 
manera autonoma

el robot fue echo por por nosotros, los datos de este no los 
documentamos
*/



#include <Arduino.h>
#include <Servo.h>

//definiciones de inicializacion
#define sensor_derecha  11  //pin 11 no sirve
#define sensor_izquieda  12
#define sensor_de_enmedio A0
int val1 =  0;
int val2 =  0;
#define mot1_1  3
#define mot1_2  5
#define mot2_1  6
#define mot2_2  7


//funcion de giro
//variable que toma en cuenta si se comienza o ya se llego al cubo
//ayudara a saber a que lado se va a girar
#define giro_a_der 0
#define giro_a_izq 1
byte lado_giro = giro_a_der;  //esta cambia cuando se llegua al cubo




//definiciones del swich
#define tratamiento_datos_    0
#define detec_der_   1
#define detec_izq_ 2
#define detec_ambos_     3
#define selector         5
#define dato_del_promedio   6
#define sin_deteccion_ambos 7
#define sens_color_negro_o_amarillo 8


//tiempo de sensadas
#define retraso_entre_sensadas 6
#define retraso_por_deteccion 30


//definiciones de nombramiento
#define sensor_der 0
#define sensor_izq 1
#define negro 1
#define blanco 0


// tiempos y fuerza motor
#define tiempo_de_correccion 50
#define tiempo_correccion_giro 1000 //
#define tiempo_atras 150
#define tiempo_de_limbo 350


// variables de motor
#define fuerza_norm  130  //velocidad normal
#define fuerza_excep_max  200 //fuerza cuando gira max
#define fuerza_excep_min 80//fuerza cuando gira min
#define fuerza_atras 100  

//variables de correccion 
#define fuerza_media 120
#define extra_fuerza_motor  60 //valor del lado de el extra velocidad izquierda y derecha

//variables para el sensor de color
#define val_medio_col_amarillo  512      //es donde cae el amarillo
#define val_medio_col_negro  1000       // donde cae el negro
#define mas_menos 5                    //se suma y resta a val_medio_color
#define amarillo 0                    //negro es 1
bool es_negro_o_amarillo = negro;




byte datos_sensor [] = {0,0};       //toma inicialmente los datos de los sensores
byte vec_datos [] = {0,0,0, 0,0,0}; // guarda las muestras de datos_sensor
byte vec_prom [] = {0,0};           // en vase a vec_datos se tienen los valores a utilizar
int vec_sens_color [] = {0,0,0}; //datos sensor de color

byte desicion_va_a = tratamiento_datos_;
byte cont_prom =0;
byte cont_prom_color =0;




//modifica velocidad de ambos motores  valores positivos y negativos hasta 255
void velocidad_izq_der(int izquierda, int derecha ){

 
  if(derecha>=0){// avanza hacia adelante ruedas derecha
    analogWrite(mot1_1,derecha);
    analogWrite(mot1_2,0);
  }
  else{// hacia atras
    derecha *=-1;
    analogWrite(mot1_1,0);
    analogWrite(mot1_2,derecha);   
  }
    

  
  if(izquierda>=0){// avanza hacia adelante ruedas izquierda
    analogWrite(mot2_1,izquierda);
    analogWrite(mot2_2,0);
  }
  else{
    izquierda*=-1;
    analogWrite(mot2_1,0);
    analogWrite(mot2_2,izquierda);   
  }
}




// tratamiento de datos
void sensado(){
 datos_sensor[sensor_der] = digitalRead(sensor_derecha); //sensor derecho        alto/1 cuando estan en negro
 datos_sensor[sensor_izq]= digitalRead(sensor_izquieda); //sensor izquierdo
}




byte complemento_promedio(byte vector_sensor_N){ // si vector_sen_N es 0 no pasa al segundo vector de vec_datos
    byte offset = 0;
    if(vector_sensor_N == 1){
      offset = 3;
    }

    int n = 0;
    for(byte i=0; i<3; i++){
      if(vec_datos[i + offset] ==blanco){
          n-=1;
        }
        else{
          n+=1;
        } 
}
    if(n>0){
      return negro;
    }
    else{
      return blanco;
    }

}




bool prom_sens_colores(){   //posible conficto por manejo de int en vez de float---------------por division------------
    int sumatoria = 0;
    for(byte i=0; i<3; i++){
      sumatoria += vec_sens_color[i];
    }
    int promedio_color = sumatoria/3;
    Serial.println(promedio_color);

    if(((val_medio_col_amarillo - mas_menos) <  promedio_color) and  (promedio_color < (val_medio_col_amarillo + mas_menos))){
      es_negro_o_amarillo = amarillo;
      return true;
    }

    if(((val_medio_col_negro - mas_menos) < promedio_color) and (promedio_color < (val_medio_col_negro + mas_menos))){
      es_negro_o_amarillo = negro;
      return true;
    }


      return false;
}




byte promedio(){
  vec_datos[cont_prom] = datos_sensor[sensor_der];
  vec_datos[cont_prom + 3] = datos_sensor[sensor_izq];
  vec_sens_color[cont_prom] = analogRead(sensor_de_enmedio);


  if(cont_prom == 2){
/*
  if(prom_sens_colores()){
    velocidad_izq_der(0, 0);
    for(byte i = 0; i<3; i++){    //senso dos veces para estar seguro de que es amarilla la deteccion
      vec_sens_color[i] = analogRead(sensor_de_enmedio);
      delay(retraso_entre_sensadas);
    }
    if(prom_sens_colores()){
    cont_prom = 0;
    return sens_color_negro_o_amarillo;
    }    
  }

*/
    cont_prom = 0;
    vec_prom[sensor_der] = complemento_promedio(sensor_der);
    vec_prom[sensor_izq] = complemento_promedio(sensor_izq);
    return selector;
  } 


  cont_prom +=1;
  return tratamiento_datos_;
}



  bool cond_giro = false; //
void tiempo_de_corector(byte sensor_a_espera, bool true_or_false, int tiempo = 0){  //complemento a funciones de correccion y giro
  byte vec [] = {0,0,0   ,0,0,0};
  byte sen_der = 0;
  byte sen_izq = 0;
  int tiem = millis();
  while(true){   

    for(byte i=0; i<3; i++){
    sensado();
    vec[i] = datos_sensor[sensor_izq];
    vec[i + 3] = datos_sensor[sensor_der];                          //complemento de selector_de_accion
    delay(retraso_entre_sensadas);
    }
 
   int n = 0;
   int N = 0;
    for(byte i=0; i<3; i++){
      if(vec[i] ==blanco){
          n-=1;
        }
        else{
          n+=1;
        } 

      if(vec[i + 3] ==blanco){
          N-=1;
        }
        else{
          N+=1;
        } 
}

    if(n>0){//de sensor izquierda
      sen_izq = negro;
    }
    else{
       sen_izq = blanco;
    }

    if(N>0){//de sensor derecha
      sen_der = negro;
    }
    else{
       sen_der = blanco;
    }

    Serial.print(sen_izq);
    Serial.print(" ");
    Serial.print(sen_der);
    Serial.print(" ");
    Serial.println("se enciclo");
    Serial.print("sensor a espera =");
    Serial.print(sensor_a_espera);
    Serial.print(" bool =");
    Serial.println(true_or_false);

    cont_prom = 0;

    if(sensor_a_espera == sensor_der and  sen_der == true_or_false ){
      cond_giro =true;
      break;//condicion de salida del bucle
    }

   if(sensor_a_espera == sensor_izq and sen_izq == true_or_false){
     cond_giro = true;
     break;
   }


   if(tiempo!=0){
     if(tiem == tiempo){
        break;
        }
   }
    }
  }
  

//funcion de giro
//tiene en cuenta los sensores 
//si tiene que girar a la derecha, tiene en cuenta el sensor derecho para parar de girar
void giro_a_izq_der(byte llegue_al_cubo){
    byte nn = 0;
    while(nn< 5){


    if(llegue_al_cubo == giro_a_der){
    velocidad_izq_der(fuerza_excep_min, -fuerza_excep_max); 
    delay(2*tiempo_de_limbo);
    velocidad_izq_der(fuerza_excep_max, -fuerza_excep_min);     
    delay(tiempo_de_limbo); 
    }
    else{//giro_a_izq
    velocidad_izq_der(-fuerza_excep_max, fuerza_excep_min); 
    delay(tiempo_de_limbo);
    velocidad_izq_der(fuerza_excep_min, fuerza_excep_max);     
    delay(tiempo_de_limbo); 

    }
    nn+=1;

    }
}




byte var_de_vector_anterior =0; //guarda el estado del  vector sensor para la segunda sensada
void func_selector(){

   if(vec_prom[sensor_izq] == 0 and vec_prom[sensor_der] == 0){ desicion_va_a = sin_deteccion_ambos; }
    else if(vec_prom[sensor_izq] == 0 and vec_prom[sensor_der] == 1){desicion_va_a = detec_der_;}
    else if(vec_prom[sensor_izq] == 1 and vec_prom[sensor_der] == 0){desicion_va_a = detec_izq_;}
    else if(vec_prom[sensor_izq] == 1 and vec_prom[sensor_der] == 1){desicion_va_a = detec_ambos_;}

    else{ desicion_va_a = tratamiento_datos_;}// error si entra aqui

}




void selector_de_accion(){

    if(var_de_vector_anterior == detec_der_){

        if(vec_prom[sensor_izq] == blanco and vec_prom[sensor_der] == negro){ 
                velocidad_izq_der(fuerza_media, fuerza_media + extra_fuerza_motor); 
                tiempo_de_corector(sensor_der, blanco); //corrector en funcion a deteccion de sensores              
                Serial.println("correccion de derecha");
          }

        if(vec_prom[sensor_izq] == blanco and vec_prom[sensor_der] == blanco){
                velocidad_izq_der(-fuerza_atras, -fuerza_atras);
                tiempo_de_corector(sensor_der, negro);
                tiempo_de_corector(sensor_der, blanco);
                velocidad_izq_der(fuerza_media, fuerza_media + extra_fuerza_motor);
                delay(tiempo_de_correccion); 
                Serial.println("desborde de derecha");        
          }

        if((vec_prom[sensor_izq] == negro and vec_prom[sensor_der] == negro) or (vec_prom[sensor_izq] == negro and vec_prom[sensor_der] == blanco) ){
                velocidad_izq_der(0, 0);
                delay(tiempo_correccion_giro);
                velocidad_izq_der(-fuerza_atras, -fuerza_atras);//------------------------------------
                delay(tiempo_atras);                
                giro_a_izq_der(lado_giro);
                Serial.println("pared enfrente der");
          }
    }
    else{// para el sensor izquierdo

        if(vec_prom[sensor_izq] == negro and vec_prom[sensor_der] == blanco){ 
                    velocidad_izq_der(fuerza_media + extra_fuerza_motor, fuerza_media); 
                    tiempo_de_corector(sensor_izq, blanco);
                    Serial.println("correccion izquierda");    
          }

        if(vec_prom[sensor_izq] == blanco and vec_prom[sensor_der] == blanco){
                    velocidad_izq_der(-fuerza_atras, -fuerza_atras);
                    tiempo_de_corector(sensor_izq, negro);
                     tiempo_de_corector(sensor_izq, blanco);
                    velocidad_izq_der(fuerza_media + extra_fuerza_motor, fuerza_media);
                    delay(tiempo_de_correccion); 
                    Serial.println("desborde de izquierda");     
          }

        if((vec_prom[sensor_izq] == negro and vec_prom[sensor_der] == negro) or (vec_prom[sensor_izq] == blanco and vec_prom[sensor_der] == negro) ){
                    velocidad_izq_der(0, 0);
                    delay(tiempo_correccion_giro);//--------------------------------
                    velocidad_izq_der(-fuerza_atras, -fuerza_atras);//------------------------------------
                    delay(tiempo_atras);
                    giro_a_izq_der(lado_giro);//funcion de giro izquieda o derecha
                    Serial.println("pared enfrente izq");
          }
    }

}




void cond_de_vector_anterior(byte deteccion_izq_o_der){
          if(var_de_vector_anterior != 0){
            selector_de_accion(); 
            var_de_vector_anterior = 0;      
        }
        else{
            var_de_vector_anterior = deteccion_izq_o_der;     
        }
}




void setup() {
  pinMode(sensor_derecha,INPUT);
  pinMode(sensor_izquieda,INPUT);
  pinMode(sensor_de_enmedio, INPUT);
  pinMode(mot1_1,OUTPUT);
  pinMode(mot1_2,OUTPUT);
  pinMode(mot2_1,OUTPUT);
  pinMode(mot2_2,OUTPUT);  
  Serial.begin(9600);   
}





void loop() {
 // int coln = analogRead(A0); // Lectura analoga


switch (desicion_va_a){

  case tratamiento_datos_:
        sensado();
        delay(retraso_entre_sensadas);
        desicion_va_a = promedio(); // tratamiento_datos_ o selector_ o _sens_color_negro_o_amarillo
  break;


  case sens_color_negro_o_amarillo:
       if(es_negro_o_amarillo == amarillo){
        //rutina para la toma del cubo----------------------------------------------------------------  
        lado_giro = giro_a_izq;        
        }

        if(es_negro_o_amarillo == negro){ 
        /*rutina para soltar el cubo y dar la vuelta
        usar el hecho de iniciar en negro y finalizar en amarillo para saber que esta iniciando o
        finalizando el viaje al centro de la tierra 
        puede que detecte el negro creyendo que tiene que soltar el cubo iniciando el viaje*/
        lado_giro = giro_a_der;
        }
        desicion_va_a = tratamiento_datos_;
  break;


  case selector:  //elige una funcion de cambio de velocidad inicial 
    func_selector();
      //  if(var_de_vector_anterior==0){delay(15);}//---------------------------------------------
        Serial.print(vec_prom[sensor_izq]);    
        Serial.println(vec_prom[sensor_der]);
  break;


  case sin_deteccion_ambos:
        if(var_de_vector_anterior != 0){
            cond_de_vector_anterior(0);                  
        }
        else{
          velocidad_izq_der(fuerza_norm, fuerza_norm);
          var_de_vector_anterior = 0;      
          desicion_va_a = tratamiento_datos_;
          Serial.println("avance");               
        }
  break;


  case detec_ambos_:
        if(var_de_vector_anterior != 0){
          cond_de_vector_anterior(0);      
        }
        else{
        velocidad_izq_der(0, 0); //correciones aqui asdfasdfasdfasdfasdfsadfasd
        delay(tiempo_correccion_giro);//--------------------------------
        velocidad_izq_der(-fuerza_atras, -fuerza_atras);//------------------------------------
        delay(tiempo_atras);
        giro_a_izq_der(lado_giro);
        desicion_va_a = tratamiento_datos_;
        Serial.println("girar a algun lado");
        }
  break;


  case detec_der_: //
        velocidad_izq_der(fuerza_media, fuerza_media);
        delay(retraso_por_deteccion);
        cond_de_vector_anterior(detec_der_);//funcion de seleccion de accion despues de segunda sensada 
        desicion_va_a = tratamiento_datos_;
  break;


  case detec_izq_:
        velocidad_izq_der(fuerza_media, fuerza_media);
        delay(retraso_por_deteccion);
        cond_de_vector_anterior(detec_izq_);
        desicion_va_a = tratamiento_datos_;
 break;
   


  default:
  break;
}

}


