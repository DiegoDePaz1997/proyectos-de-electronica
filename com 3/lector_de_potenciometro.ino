//potenciometro pin A0
//Salidas pines 9,8,7,6

void setup() {
  pinMode(A0,INPUT);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  float voltaje= float(analogRead(A0))*5/1024;
  if(voltaje<0.5){
    digitalWrite(8,LOW);
    digitalWrite(7,LOW);
    digitalWrite(6,LOW);
    delay(300);
    digitalWrite(9,HIGH);    
    }
  else if(voltaje<1){
    digitalWrite(9,LOW);
    digitalWrite(7,LOW);
    digitalWrite(6,LOW);
    delay(300);
    digitalWrite(8,HIGH);
    }
  else if(voltaje<1.5){
    digitalWrite(9,LOW);
    digitalWrite(8,LOW);
    digitalWrite(6,LOW);
    delay(300);
    digitalWrite(7,HIGH);    
    }
  else if(voltaje<2){
    digitalWrite(9,LOW);
    digitalWrite(8,LOW);
    digitalWrite(7,LOW);
    delay(300);
    digitalWrite(6,HIGH);    
    }
    
  else{ 
    digitalWrite(9,LOW);
    digitalWrite(8,LOW);
    digitalWrite(7,LOW);
    digitalWrite(6,LOW);
    }


  
  Serial.println(voltaje);
  delay(500);
}
