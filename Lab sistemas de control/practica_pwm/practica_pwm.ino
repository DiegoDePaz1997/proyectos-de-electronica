boolean condicion=true;
byte pwm= 64;
boolean boton_1= false; 
boolean boton_2= false;
byte cont =0;
void setup() {
  // put your setup code here, to run once:
  pinMode(6,INPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(A0,INPUT);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  boolean x= digitalRead(6);
  float porcentaje= 100*float(pwm)/256;
  cont+=1;
    if(cont==10) {
      Serial.print(porcentaje);
      Serial.println(" %,  ");


      cont=0;        
    }
    delay(100);
    


  
  if(x)
  {

    delay(1000);
    //potenciometro
    int analogo= analogRead(0);
    if(analogo<50)
    {
     pwm=64;
    }
    else if(analogo<100)
    {
     pwm=128;
    }
    else
    {
     pwm=192;
    }
  analogWrite(5,pwm);    
  }

  
  else
  {
  //botones
    if(boton_1 and boton_2 ==true){condicion=true;}
    boton_1=  digitalRead(3);  
    boton_2=  digitalRead(2); 
    delayMicroseconds(200);
    boton_1=  digitalRead(3);  
    boton_2=  digitalRead(2); 

    if(condicion==true and boton_1==false)
    {
      pwm+=64;
      if(pwm>192 or pwm==0){pwm=192;}
    }

    if(condicion==true and boton_2==false)
    {
      pwm-=64;
      if(pwm<64){pwm=64;}

    }


    analogWrite(5,pwm);

    if(boton_1==false or  boton_2==false){condicion= false;}
  }

}
