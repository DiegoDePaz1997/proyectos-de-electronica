/*
crear un con dispositivos de potencia una matriz de 6 filas y 4 columnas
dónde se pueda por medio de una computadora ingresar un texto y este
se muestre en la matriz.

se utilizaron 3 registros de desplazamiento contectados
en cascada, sus salidas van conectadas a cada src que a 
su vez van conectadas al circuito de las bombillas.

con esto se pueden utilizar solo 2 salidas del microcontrolador
en vez de 24, con esto a su vez es más fácil aislar la parte
de alto voltaje.

*/


//aqui se coloca el texto a mostrar
String cadena = " ";

String A = "100110011111100111110000";
String B = "011110010111100101110000";
String C = "111100010001000111110000";
String D = "011110011001100101110000";
String E = "111100010111000111110000";
String F = "000100010111000111110000";
String G = "111110011101000111100000";
String H = "100110011111100110010000";
String I = "011100100010001001110000";
String J = "001001010100010011110000";
String K = "100101010011010110010000";
String L = "111100010001000100010000";
String M = "100110011001111110010000";
String N = "100110011101101110010000";
String O = "111110011001100111110000";
String P = "000100010111100101110000";
String Q = "100001101001100101100000";
String R = "100110010111100101110000";
String S = "011110000110000111100000";
String T = "001000100010001001110000";
String U = "011010011001100110010000";
String V = "001001010101010101010000";
String W = "010101110101010101010000";
String X = "000010010110011010010000";
String Y = "001000100010001001010000";
String Z = "000011110010010011110000";





int retraso = 100;
int largo_cadena = 0;
void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(9600);  
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);
  
}

void loop() {


  if ( Serial.available() > 0)
  {

    Serial.println(Serial.available());    
    cadena = Serial.readString();    
    for (int i = 0; i< cadena.length(); i++)
    {
    busqueda(cadena[i]);
    delay(1000);     
    }
  }
 // digitalWrite(11, HIGH);//para apagar la ultima letra //no probado
  
  
}


byte UNO[]    ={0b00100110,0b10100010,0b00101111};
byte DOS[]   = {0b01101001,0b00010010,0b01001111};
byte TRES[]  = {0b01101001,0b00100010,0b10010110};
byte CUATRO[]= {0b10011001,0b11110001,0b00010001};
byte CINCO[] = {0b10011001,0b11110001,0b00010001};
byte SEIS[]  = {0b11111000,0b11111001,0b10011111};
byte SIETE[] = {0b11110001,0b00010001,0b00010001};
byte OCHO[]  = {0b00001111,0b10011111,0b10011111};
byte NUEVE[] = {0b11111001,0b11110001,0b10100110};
byte CERO[]  = {0b11111001,0b10011001,0b10011111};    









  //envia la cadena de bits a los registros
  void ciclo(String ar)
  {
    digitalWrite(11, HIGH);
  for(int i = 0; i<=A.length(); i++)//tiene que tener 1 mas del tama;o del string
  {
    if(ar[i] == '1')
    {
     digitalWrite(13, HIGH);
     delay(retraso);
     digitalWrite(12, HIGH);
     delay(retraso);
     digitalWrite(12, LOW);
     delay(retraso);     
     digitalWrite(13, LOW);     

    }
    else
    {
     digitalWrite(12, HIGH);
     delay(retraso);
     digitalWrite(12, LOW);     
    }
  }
  digitalWrite(11, LOW);
  }










  //solo sirve para llamar a la funcion
  //ciclo que envia los bits
  void busqueda(char caracter)
  {



    switch (caracter)
    {
     
    case 'A':
    ciclo(A);
    break;

    case 'B':
    ciclo(B);
    break;

    case 'C':
    ciclo(C);
    break;

    case 'D':
    ciclo(D);
    break;

    case 'E':
    ciclo(E);
    break;

    case 'F':
    ciclo(F);
    break;

    case 'G':
    ciclo(G);
    break;

    case 'H':
    ciclo(H);
    break;

    case 'I':
    ciclo(I);
    break;

    case 'J':
    ciclo(J);
    break;

    case 'K':
    ciclo(K);
    break;

    case 'L':
    ciclo(L);
    break;

    case 'M':
    ciclo(M);
    break;
            
    case 'N':
    ciclo(N);
    break;

    case 'O':
    ciclo(O);
    break;

    case 'P':
    ciclo(P);
    break;

    case 'Q':
    ciclo(Q);
    break;

    case 'R':
    ciclo(R);
    break;

    case 'S':
    ciclo(S);
    break;

    case 'T':
    ciclo(T);
    break;

    case 'U':
    ciclo(U);
    break;

    case 'V':
    ciclo(V);
    break;

    case 'W':
    ciclo(W);
    break;
    
    case 'X':
    ciclo(X);
    break;

    case 'Y':
    ciclo(Y);
    break;

    case 'Z':
    ciclo(Z);
    break; 


 /* 
         case '1':
    ciclo(uno);
    break;
   
      case '2':
    ciclo(dos);
    break;
    
      case '3':
    ciclo(tres);
    break;
    
      case '4':
    ciclo(cuatro);
    break;  
        
      case '5':
    ciclo(cinco);
    break;

      case '6':
    ciclo(seis);
    break;

      case '7':
    ciclo(siete);
    break;

      case '8':
    ciclo(ocho);
    break;

      case '9':
    ciclo(nueve);
    break;

      case '0':
    ciclo(cero);
    break;
 
  
*/



    
    default:
    break;
    }
  }
