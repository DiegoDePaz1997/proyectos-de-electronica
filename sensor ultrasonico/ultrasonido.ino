float dis_sensor;
float V_sonido;
int H=400;
int contador=20;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(2,OUTPUT);
pinMode(3,INPUT);
pinMode(4,OUTPUT);
pinMode(8,INPUT); 
pinMode(A5,INPUT);
pinMode(A4,INPUT); 

V_sonido= (331.1+(0.606*25))/1000;
}

void loop() {
if(digitalRead(A4)==0){
  H+=10;
  Serial.print("altura cambio a: ");
  Serial.println(H);
  delay(1000);
}
if(digitalRead(A5)==0){
  H-=10;
  Serial.print("altura cambio a: ");
  Serial.println(H);
  delay(1000);
}


  
digitalWrite(2,HIGH);
delayMicroseconds(10);
digitalWrite(2,LOW);
dis_sensor=pulseIn(3,HIGH)*V_sonido/2;


if(contador==0){
contador=20;
Serial.println("------------------");
Serial.print("altura del tanque= ");
Serial.print(H);
Serial.println("mm");
Serial.print("Distancia del sensor= ");
Serial.print(dis_sensor);
Serial.println("mm");
Serial.print("Nivel= ");
Serial.print(H-dis_sensor);
Serial.println("mm");
Serial.print("% de llenado= ");
Serial.print(100*(H-dis_sensor)/H);
Serial.println("%");
Serial.println("------------------");
}


if((H-dis_sensor)/H<=0.4){
  digitalWrite(8,HIGH);
}
else{
  digitalWrite(8,LOW);
}
if((H-dis_sensor)/H>=0.8){
  digitalWrite(4,HIGH);
}
else{
  digitalWrite(4,LOW);
}

contador-=1;
delay(100);




}
