/*
es un dispensador de pastillas
partes
 -pantalla lcd
 -3 botones
 -1 buzzer
 -1 motor de vibracion
 -1 modulo de reloj
 -14 compartimentos con luz led en el fondo

 -se le puede asignar hasta 14 pastillas diferentes
 -tiene un menu para setear la alarma y las posiciones de las pastillas
  a tomar se iluminan para ingresarlas 
 -al sonar una larma se iluminaran las posiciones de las pastillas 

-dependiendo de cuantos grupos de pastillas se necesite tomar y
 cada cuanto se deban de tomar se asignaran de manera automatica
 cuanto de cada grupo ingresar al dispensador para que se acaben de
 manera más homogenea
 ejemplo
      --grupo1 = por 1 semana cada 12 horas
      --grupo2 = por 3 dias cada 6 horas
      se eligiran el doble de pastillas del grupo2 
      luego se eliminarán automáticamente cuando pasen los 3 días
      y le pedirá al usuario que ingrese las pastillas iluminando 
      todos los compartimentos.

-cuando falte 1 pastilla por tomar se avisará cada cierto tiempo de que
 se debe de rellenar el dispensador


*/




//botones  boton_opciones_N pin 2          boton_pulsos pin 3   boton_inversor=7
//registros de desplazamiento  pin4 =reloj chip_pin_12_y_11   pin5 = data chip_pin_14

//motor de vibracion  luz y sonido pin 9

// coneccion de pantalla lcd y reloj de tiempo real i2c

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h> 

LiquidCrystal_I2C lcd (0x27, 16, 2);
DS3231 clock;
byte cont_de_salida = 0; // regresa al estado por defecto si alcanza el 4
//variables
volatile boolean boton_opciones_N = false;
volatile boolean boton_pulsos= false;
boolean borrar = true; // condicion para borrar
byte cons = 1;
long delaiy = 0;
long delaiy_comun = 0; // si no se hace nada en los proximos 15 seg regresa a estado 6
//
byte estado_actual = 6;                               //cada numero permite estar en un solo estado

byte boton_pulsos_conteo_comun = 0;                  //se utiliza en todo el programa controla el boton de conteo
byte grupo_N_pastillas = 0;                         //son la cantidad de grupos que se va a tomar
 

                   
float lista_grupos_pastillas[7] = {0,0,0,0,0,0,0};//va a almacenar inicialmente las horas
                                                  //va terminar almacenando cuantos espacios requiere cada grupo
byte  espacios_restantes_pastillas[7] = {0,0,0,0,0,0,0}; //  decrece cuando toca una pastilla 


 
byte hora_inicio[2] = {0,0};                  //a esta hora se asignaron las alarmas                    
byte horas_de_los_grupos[7]     = {0,0,0,0,0,0,0};// mantiene un registro de las horas
byte horas_mas_offset [7]     = {0,0,0,0,0,0,0};   //actualiza las horas



int contador = 0;// para evitar doble pusacion, no usar

// cosas del reloj
bool century = false;
bool h12Flag;
bool pmFlag;


//para la funcion de demostracion
int tiempo = 0;
 int diferencia= 0;










void setup() {
  lcd.init(); // inicializa
  lcd.backlight();  
  lcd.clear(); 

  Wire.begin();

  attachInterrupt(0, boton_opciones, LOW);//FALLING es cuando pasa de alto a bajo
  attachInterrupt(1, boton_cont, LOW); //LOW es cuando el pin esta en bajo
                                      //CHANGE dada vez que cambia de valor
  Serial.begin(9600);

  pinMode(5, OUTPUT);//datos    registros de desplazamiento
  pinMode(4, OUTPUT);//reloj

  pinMode(7, INPUT);// boton para borrar datos

  pinMode(9, OUTPUT); // alarmas
  
  recorrido_de_byte(16, 0); // apaga todos las suces

}






void loop() {

switch(estado_actual)
{

  case 0:
    lcd.clear();
    estado_actual = estado_opciones_0();
    break;
  
  case 1:
    lcd.clear();// se ingresan la cantidad de grupos a tomar
    estado_actual = estado1();  // agregar confirmacion o repeticion?
    break;

  case 2:
    lcd.clear();
    estado_actual = estado2(); // se ingresan los periodos de cada grupo, cada 8H cada 6H ...
    break;

  case 3:
    lcd.clear();// se iluminan los espacios para ingresar las medicinas
    estado_actual = estado3();
    break;



  case 5://por ahora solo funciona este, si quieren pasar a los otros poner estado_actual =1
    lcd.clear();
    estado_actual = estado_fecha_5();
    break;

  case 6:
    lcd.clear();
    estado_actual = proxima_pastilla_6();


    
    


  //estos estados se van a estar verificando a cada rato
  // tiempo
  // rellenado de medicina





  default:
    estado_actual=0;
}
}



byte estado_opciones_0(){  //inicio
  byte n;
  char flecha = " Coo";
  byte contador1=0;
  delaiy_comun = millis();
  while(true){
  
  if(abs(millis()-delaiy_comun) >15000){// retorno si no se hace nada por 15 segs
    boton_pulsos_conteo_comun = 0;
    contador1 = 0;
    return 6;
  }

    
  //cambio de fecha
    if(boton_pulsos==true){//aumenta en 1 el contador por cada pulsacion
    delaiy_comun = millis();
    boton_pulsos_conteo_comun+=1;
    contador1=0;
    if(boton_pulsos_conteo_comun == 2){boton_pulsos_conteo_comun=0;}
    boton_pulsos = false;
    }

    if(boton_opciones_N == true){
      delaiy_comun = millis();
      contador1+=1;
      boton_opciones_N = false;      
      
      if(contador1== 2){   
        if(boton_pulsos_conteo_comun== 0){
              contador1==0;
              boton_pulsos_conteo_comun = 0;
              return 5;
         } 
        else{
              boton_pulsos_conteo_comun = 0;
              contador1==0;
              return 1;
      }       
    }
    }

      lcd.setCursor(0, 0);
      lcd.print("Cam de fecha");
      if(boton_pulsos_conteo_comun==0){lcd.print(" Co");} else{lcd.print("   ");}

      lcd.setCursor(0, 1);
      lcd.print("in pastillas"); 
      if(boton_pulsos_conteo_comun==1){lcd.print(" Co");} else{lcd.print("   ");}





  }
}




byte estado1() // ingreso de # de grupos para tomar
{
  delaiy_comun = millis();
  while(true)
  {
  if(abs(millis()-delaiy_comun) >15000 or cont_de_salida==4 ){// retorno si no se hace nada por 15 segs
    boton_pulsos_conteo_comun = 0;                          // tambien retorna si se llega a 4 cont_de_salida
    cont_de_salida = 0;
    return 6;
  }

    
      if(digitalRead(6)==0 and abs(millis()-delaiy_comun) >500){//suma o resta con los botones
          cont_de_salida+=1;
          delaiy_comun = millis();
          cons*=-1; 
      }


 
      
    if(boton_opciones_N == true){
    cont_de_salida = 0;
    delaiy_comun = millis();
    boton_opciones_N = false;      
    if(boton_pulsos_conteo_comun>0){
      boton_pulsos_conteo_comun = 0;
      break;
      }     
    boton_pulsos_conteo_comun = 0;
    }



    if(boton_pulsos==true){//aumenta en 1 el contador por cada pulsacion
    cont_de_salida = 0;
    delaiy_comun = millis();
    boton_pulsos_conteo_comun+=cons;
    if(boton_pulsos_conteo_comun == 255){boton_pulsos_conteo_comun=0;}
    boton_pulsos = false;
    }

    if(boton_pulsos_conteo_comun==8){boton_pulsos_conteo_comun=0;}//resetea contador al llegar a 7
 

    grupo_N_pastillas = boton_pulsos_conteo_comun;
    impresion_letras("grupos de pasti", "llas a tomar =", boton_pulsos_conteo_comun);
 
  } 
    return 2;
}



byte estado2() // ingreso de cada cuanto se toman las pastillas
{
  delaiy_comun = millis();
  byte cont_acendente =0;// aumenta solo si se pulsa el boton, para pasar al siguiente grupo de pastillas
  while(true)
  {


      if(abs(millis()-delaiy_comun) >15000 or cont_de_salida==4 ){// retorno si no se hace nada por 15 segs
        boton_pulsos_conteo_comun = 0;                          // tambien retorna si se llega a 4 cont_de_salida
        cont_de_salida = 0;
        return 6;
      }

    
      if(digitalRead(6)==0 and abs(millis()-delaiy_comun) >500){//suma o resta con los botones
          cont_de_salida+=1;
          delaiy_comun = millis();
          cons*=-1; 
      }



    
   
    if(boton_pulsos==true){//lleva el conteo de las horas de cada pasilla
      delaiy_comun = millis();
      cont_de_salida = 0;
      boton_pulsos_conteo_comun+=cons;
      boton_pulsos = false;
      if(boton_pulsos_conteo_comun==255){boton_pulsos_conteo_comun=0;}
      }


  
    if(boton_opciones_N==true){//pasa a la siguiente opcion de colocacion de horario de pastilla
      delaiy_comun = millis();
      cont_de_salida = 0;
      if( boton_pulsos_conteo_comun>0 or cons!=1){  
      cont_acendente+=cons;
      if(cont_acendente==255){cont_acendente=0;}
      boton_pulsos_conteo_comun=0;        
      }
      boton_opciones_N = false;
      }  


  
    if(cont_acendente>=grupo_N_pastillas){// resetea y sale
      cont_acendente = 0;
      boton_pulsos_conteo_comun=0;
      
      break;
      } 

    lista_grupos_pastillas[cont_acendente] = boton_pulsos_conteo_comun;  
        
    impresion_letras("cada cuanto se","toma grupo" ,cont_acendente + 1 );
    lcd.print(" ="); 
    lcd.print(boton_pulsos_conteo_comun);
    lcd.print("H");  
  }
     for(int i= 0; i<grupo_N_pastillas ; i++)
     {
       horas_de_los_grupos[i] = lista_grupos_pastillas[i];
     }

     

  
  return 3;
}



byte estado3()  // se asignan posiciones a las pastillas
{ 
  programador_de_espacios(); //lista_grupos_pastillas[7] da la cantidad de espacios
  byte on_of = 0;
  byte con = 10;
  boton_pulsos_conteo_comun = 1; 
  while(true)
  {
   //tratamiento de botones  
    if(boton_pulsos == true or boton_opciones_N == true){
    boton_pulsos_conteo_comun += 1;
    boton_pulsos = false;
    boton_opciones_N = false;
    if(boton_pulsos_conteo_comun > grupo_N_pastillas ){boton_pulsos_conteo_comun=0; break; }
    }
    impresion_letras("coloque el grupo" , "#" ,boton_pulsos_conteo_comun);


      //el corrimiento tiene desfasaje
    if(con != boton_pulsos_conteo_comun){//condicion para encendido y apagado de leds
     con = boton_pulsos_conteo_comun;
   
    //control del encendido a partir del    boton_pulsos_conteo_comun
     byte CERO= 0b10000000; 
     CERO = CERO>> boton_pulsos_conteo_comun-1;   

    //envio de cadena a registro de desplazamiento   
    for(byte i = 0; i<grupo_N_pastillas; i++){
      if(CERO ==0b10000000){//enciende un grupo
      on_of = 1;
      CERO = CERO<<1;
      }
    else{                 //apaga los demas grupos
      on_of = 0;
      CERO = CERO<<1;
    }
    
      byte n = lista_grupos_pastillas[i];
      if(i == grupo_N_pastillas - 1){n+=1;}
      recorrido_de_byte(n, on_of);
    }
    }   
  }
  
  for(byte i =0; i<7; i++){
   espacios_restantes_pastillas[i] = lista_grupos_pastillas[i]; //parte nueva del estado 7 esta linea estaba mal
                                                               // pase como 6 horas buscando el error

}
for(byte i=0; i<grupo_N_pastillas; i++){
    horas_mas_offset[i]=horas_de_los_grupos[i] + clock.getHour(h12Flag, pmFlag);
  if(horas_mas_offset[i]>23){horas_mas_offset[i]-=24;}
}

  hora_inicio[0] = clock.getHour(h12Flag, pmFlag);
  hora_inicio[1] = clock.getMinute();




  
  recorrido_de_byte(16, 0);  
  return 0 ;
}



byte estado_fecha_5(){  // se cambian las las fechas y horas
  byte pasos_fecha = 0;
  boolean bol= true;
  delaiy_comun = millis();
  boton_pulsos_conteo_comun = 20;
    while(true){
      
      //----------------------------------------------------------
      if(abs(millis()-delaiy_comun) >15000 or cont_de_salida==4 ){// retorno si no se hace nada por 15 segs
        boton_pulsos_conteo_comun = 0;                          // tambien retorna si se llega a 4 cont_de_salida
        cont_de_salida = 0;
        return 6;
      }

    
      if(digitalRead(6)==0 and abs(millis()-delaiy_comun) >500){//suma o resta con los botones
          cont_de_salida+=1;
          delaiy_comun = millis();
          cons*=-1; 
      }
      //----------------------------------------------------------
    
    if(boton_pulsos==true){//aumenta en 1 el contador por cada pulsacion
        delaiy_comun = millis();
        cont_de_salida = 0;
        lcd.clear(); 
        boton_pulsos_conteo_comun+=cons;// cambio
        if(boton_pulsos_conteo_comun==30 and pasos_fecha==0){boton_pulsos_conteo_comun=20;}
        if(boton_pulsos_conteo_comun==13 and pasos_fecha==1){boton_pulsos_conteo_comun=1;}
        if(boton_pulsos_conteo_comun==32 and pasos_fecha==2){boton_pulsos_conteo_comun=1;}
        if(boton_pulsos_conteo_comun==25 and pasos_fecha==3){boton_pulsos_conteo_comun=0;}
        if(boton_pulsos_conteo_comun==60 and pasos_fecha==4){boton_pulsos_conteo_comun=0;}
        if(boton_pulsos_conteo_comun==255                  ){boton_pulsos_conteo_comun=0;}///////
        boton_pulsos = false;
    }




    
    if(boton_opciones_N == true){
        delaiy_comun = millis();
        cont_de_salida = 0;
        pasos_fecha+=cons; 
         if(pasos_fecha==255){pasos_fecha=0;}///////
        lcd.clear(); 
        if(pasos_fecha>0){
          boton_pulsos_conteo_comun = 0;
          }
        else{ 
          boton_pulsos_conteo_comun = 20;
          }   
        boton_opciones_N = false;
           
        if( pasos_fecha >5){pasos_fecha=0; boton_pulsos_conteo_comun = 0; break;}
    }

    if(pasos_fecha== 0){
         lcd.setCursor(0, 1);
         lcd.print("Año = ");
         lcd.print(boton_pulsos_conteo_comun);
         clock.setYear(boton_pulsos_conteo_comun);  
    }
    else if(pasos_fecha== 1){
         lcd.setCursor(0, 1);
         lcd.print("Mes = ");
         lcd.print(boton_pulsos_conteo_comun);
         clock.setMonth(boton_pulsos_conteo_comun);
      } 
    else if(pasos_fecha== 2){
         lcd.setCursor(0, 1);
         lcd.print("Dia = ");
         lcd.print(boton_pulsos_conteo_comun);
         clock.setDate(boton_pulsos_conteo_comun);
      } 
    else if(pasos_fecha== 3){
         lcd.setCursor(0, 1);
         lcd.print("Horas = ");
         lcd.print(boton_pulsos_conteo_comun);
         clock.setHour(boton_pulsos_conteo_comun);
      }  
    else if(pasos_fecha== 4){
         lcd.setCursor(0, 1);
         lcd.print("Minutos = ");
         lcd.print(boton_pulsos_conteo_comun);
         clock.setMinute(boton_pulsos_conteo_comun);
      }       
 
      lcd.setCursor(0, 0);
      lcd.print(clock.getYear(), DEC);
      lcd.print("/");
      lcd.print(clock.getMonth(century), DEC);
      lcd.print("/");
      lcd.print(clock.getDate(), DEC);
      lcd.print(" ");
      lcd.print(clock.getHour(h12Flag, pmFlag), DEC); //24-hr
      lcd.print(":");
      lcd.print(clock.getMinute(), DEC);
      bol=false;
 
    if(pasos_fecha==5){return 0;}
}

}




byte proxima_pastilla_6(){
byte numero1 =0;
byte numero2 =0;
  while(true){
    if(boton_pulsos==true){numero1+=1;}
    if(boton_opciones_N==true){numero2+=1;}
    
    if(boton_pulsos==true or boton_opciones_N==true){//aumenta en 1 el contador por cada pulsacion
    boton_pulsos_conteo_comun+=1;
    boton_pulsos = false;
    boton_opciones_N = false;
    }
    if(boton_pulsos_conteo_comun==4){
      numero1 =0;
      numero2 = 0;
      boton_pulsos_conteo_comun=0;
      return 0;
    }
    byte menor = horas_mas_offset[0];
    for(byte i = 0; i<grupo_N_pastillas; i++){

      if(menor > horas_mas_offset[i] ){
        menor = horas_mas_offset[i];
      }     
    }
    
         lcd.setCursor(0, 0);
         lcd.print("Proxima alarma ");
         lcd.setCursor(0, 1);
         lcd.print("   ");
         lcd.print(menor);
         lcd.print("H : ");
         lcd.print(hora_inicio[1]);
         lcd.print("min");
         lcd.setCursor(15, 1);
         lcd.print(boton_pulsos_conteo_comun);


     estado_toca_medicina_7();   //alarmas     


    if(numero1 == 2 and numero2 == 1){   
        demostracion();      
         numero1 =0;
         numero2 =0;
         boton_pulsos_conteo_comun=0;
  }  

    
}
}



void demostracion(){ //funcion para demostrar las capacidades del pastillero
    
     byte menor = horas_mas_offset[0];  
     byte nos   = 0; 
    for(byte i = 0; i<grupo_N_pastillas; i++){
      nos = i;
      if(menor > horas_mas_offset[nos] and horas_mas_offset[nos]>0){//toma proxima alarma
        menor = horas_mas_offset[nos];
      }   
    }
        
    if(menor>0){ 
     clock.setMinute(hora_inicio[1] -1);// cambia la hora hasta 7 segundos antes de la alarma
     clock.setHour(menor);
     clock.setSecond(53);
    }
}





byte estado_toca_medicina_7(){ //por medio de alarmas visuales auditivas se alerta de la toma
    boolean condicion = false;
    byte posicion = 0;
    byte on_of = 0;
    byte inis = 0;


      
      for(byte i= 0; i<grupo_N_pastillas; i++){
        inis = i;

        if(horas_mas_offset[inis]== clock.getHour(h12Flag, pmFlag) and hora_inicio[1]== clock.getMinute() and condicion == false)
        { //enciende alarma
            analogWrite(9,150);

            encendido_luz_unica(inis);//funcion que encienda la luz adecuada           
            espacios_restantes_pastillas[inis]-=1;
            horas_mas_offset[inis]+=horas_de_los_grupos[inis];
            if( horas_mas_offset[inis]>23){
              horas_mas_offset[inis]-=24;
              } 
               condicion =true;
               break;
            }
      }

      





      if(condicion==true){
       while(true){ 
        //verifica que queden espacios de pastillas
          for(byte i =0; i<grupo_N_pastillas; i++){
            if(espacios_restantes_pastillas[i]==0){
              for(byte i=0; i<200; i+=10){
                analogWrite(9,i);
                delay(50);
              }
               analogWrite(9,40);  
            }
          }

        
        if((boton_opciones_N == true or boton_pulsos==true)){// apaga la alarma       
        analogWrite(9, 0);
        boton_opciones_N = false;
        boton_pulsos = false;
        condicion = false;
        // recorre el byte y enciende o no los leds en funcion de si es 1, 0
        recorrido_de_byte(14, 0);
        boton_pulsos = false;
        boton_opciones_N = false;
        boton_pulsos_conteo_comun==0; 
        delay(200);       
        break;
      }
      }
    }

}





void encendido_luz_unica(byte posicion){  //posicion es hasta donde sumara
//control del encendido a partir de posicion
    byte suma =0;
           
      for(byte i = 0; i < posicion; i++){
        suma += lista_grupos_pastillas[i];
      }
      suma += espacios_restantes_pastillas[posicion];

      encendido_unitario(suma - 1);
    }
  



byte asignacion_de_procc_alarmas(){
  for(byte i= 0; i<7; i++){
      horas_de_los_grupos[i] += hora_inicio[0];  
  }
  
}






//programa cuantos espacios va a requerir cada grupo de pastillas
void programador_de_espacios() {
byte cond = 0;
byte acumulador_MUL = 1;  
byte numero = 2;
byte lista_grupos[7] = {0,0,0,0,0,0,0};
for(byte i=0; i<grupo_N_pastillas; i++)
{
 lista_grupos[i] = lista_grupos_pastillas[i];
}

while(numero <24)
{
  cond = 1;
  for(byte i=0; i<grupo_N_pastillas; i++)
  {
    if(lista_grupos[i]==1){
      continue;
      }
    

    if(lista_grupos[i]%numero==0){
      cond &= 0;
      lista_grupos[i]=lista_grupos[i]/numero;
    }
    else{cond &= 1;}
  }
   numero+= cond;
   if(cond==0){ acumulador_MUL= numero*acumulador_MUL;}
} 

numero=0;

for(byte i=0; i<grupo_N_pastillas; i++)// tratamiento de las horas
{
  lista_grupos[i]=acumulador_MUL/lista_grupos_pastillas[i];
  numero+=lista_grupos[i];
}

for(byte i=0; i<grupo_N_pastillas; i++)//asigna espacios a cada grupo de pastillas
{
  float m =14*float(lista_grupos[i])/numero;
  if( m> int(m) + 0.5){   
    lista_grupos_pastillas[i] = int(m) + 1;
  }
  else{
     lista_grupos_pastillas[i] = int(m);
  }

}
numero =0;
for(byte i=0; i<grupo_N_pastillas; i++)//verifica que hay 14 pastillas
{
  numero+=lista_grupos_pastillas[i];
}

byte numero2 = numero;
byte in =0;
if(numero>14)// si hay mas de 14 pastillas elimina una
{
numero =0;  
for(byte i=0; i<grupo_N_pastillas; i++)
{
 if(numero< lista_grupos_pastillas[i]){
  numero=lista_grupos_pastillas[i];
  in = i;
  }
}  
lista_grupos_pastillas[in]-=1;
}




if(numero2<14){//verifica y arregla el problema de que haya menos de 14 passtillas
  while(true){
  numero = lista_grupos_pastillas[0];
  in = 0;    
  for(byte i=0; i<grupo_N_pastillas; i++){//da la posicion del comparimento que menos tiene
  if(numero> lista_grupos_pastillas[i]){
    numero = lista_grupos_pastillas[i];
    in = i;
  }
  }  
  lista_grupos_pastillas[in]+=1;
  
  numero2 = 0;
  for(byte i=0; i<grupo_N_pastillas; i++)//verifica que hay 14 pastillas
{
  numero2+=lista_grupos_pastillas[i];
}

  if(numero2==14){
     break;
  }
  
  }
}
}

void impresion_letras(String n, String m, byte o)
{
      lcd.setCursor(0, 0);
      lcd.print(n);
      lcd.setCursor(0, 1);
      lcd.print(m); 
      lcd.print(o);   
}




//-------------------------------------------------------
// recorre el byte y enciende o no los leds en funcion de si es 1, 0
void recorrido_de_byte(byte cant_espacios, byte on_of)
{
  for(int i= 0; i<cant_espacios; i++)
  {
  ciclo(on_of);
  }
}





void encendido_unitario(byte posicion){ 
  byte on_of = 0;
  
  for(byte i = 0; i<=14; i++){
   if(posicion == i){
    on_of = 1;
    }
   else {
    on_of = 0;
    }

  ciclo(on_of);
  }
}


//funcion de encendido y apagado de los registros
//de desplazamiento 
  void ciclo(byte ar)
  {
    if(ar == 1)
    {
     digitalWrite(5, HIGH); //datos
     delay(1);
     digitalWrite(4, HIGH); //reloj
     delay(1);
     digitalWrite(4, LOW);
     delay(1);     
     digitalWrite(5, LOW);     
    }
    else
    {
     digitalWrite(4, HIGH);
     delay(1);
     digitalWrite(4, LOW); 
    } 
     delay(1);    
  }





//-------------------------------------------------------
//funciones de las interrupciones

void boton_opciones(){
  if(millis() - contador>100)
  {
  boton_opciones_N=true;
  contador = millis();
  }
  if(millis()==0){contador=0;}  
}


void boton_cont(){
  if(millis() - contador>100)
  {
  boton_pulsos=true;
  contador = millis();
  }
  if(millis()==0){contador=0;}  

}
