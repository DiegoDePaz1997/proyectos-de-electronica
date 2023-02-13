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

String cadena = " ";
int retraso = 1;
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

byte A[] = {0b10011001,0b11111001,0b11110000};
byte B[] = {0b01111001,0b01111001,0b01110000};
byte C[] = {0b11110001,0b00010001,0b11110000};
byte D[] = {0b01111001,0b10011001,0b01110000};
byte E[] = {0b11110001,0b01110001,0b11110000};
byte F[] = {0b00010001,0b01110001,0b11110000};
byte G[] = {0b11111001,0b11010001,0b11100000};
byte H[] = {0b10011001,0b11111001,0b10010000};
byte I[] = {0b01110010,0b00100010,0b01110000};
byte J[] = {0b00100101,0b01000100,0b11110000};
byte K[] = {0b10010101,0b00110101,0b10010000};
byte L[] = {0b11110001,0b00010001,0b00010000};
byte M[] = {0b10011001,0b10011111,0b10010000};
byte N[] = {0b10011001,0b11011011,0b10010000};
byte O[] = {0b11111001,0b10011001,0b11110000};
byte P[] = {0b00010001,0b01111001,0b01110000};
byte Q[] = {0b10000110,0b10011001,0b01100000};
byte R[] = {0b10011001,0b01111001,0b01110000};
byte S[] = {0b01111000,0b01100001,0b11100000};
byte T[] = {0b00100010,0b00100010,0b01110000};
byte U[] = {0b01101001,0b10011001,0b10010000};
byte V[] = {0b00100101,0b01010101,0b01010000};
byte W[] = {0b01010111,0b01010101,0b01010000};
byte X[] = {0b00001001,0b01100110,0b10010000};
byte Y[] = {0b00100010,0b00100010,0b01010000};
byte Z[] = {0b00001111,0b00100100,0b11110000};




void setup() {
  //inicializacion de puertos y puerto serial
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(9600);  
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
}


void loop() {
// recepcion de cadena desde PC

  if ( Serial.available() > 0)
  {   
    cadena = Serial.readString(); 
    //aqui se toma cada caracter individualmente   
    for (int i = 0; i< cadena.length(); i++)
    {
    digitalWrite(11, HIGH); //esto mantiene a cero las salidas de los registros      
    busqueda(cadena[i]);  //funcion swich
    digitalWrite(11, LOW); // deja pasar las salidas   
    delay(700);  
    }
  }
    digitalWrite(11, HIGH);  //pone a cero de nuevo las salidas luego que
                            // termine la secuencia de la matriz
}





// recorre el byte y enciende o no los leds en funcion de si es 1, 0
void recorrido_de_byte(byte num)
{
  for(int i= 0; i<=7; i++)
  {
    ciclo(num & 0b10000000); //puede ser solo 1 o solo 0
    num = num << 1;
  }
}




//funcion de encendido y apagado de los registros
//de desplazamiento 
  void ciclo(byte ar)
  {
    if(ar == 0b10000000)
    {
     digitalWrite(13, HIGH); //datos
     delay(retraso);
     digitalWrite(12, HIGH); //reloj
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
     delay(retraso);    
  }

  

//dependiendo de la cadena que se haya recibido desde el PC
//se ejecutara la cadena de una u otra letra o numero
void busqueda(char caracter)
  {
    switch (caracter)
    { 
    case '0':
    recorrido_de_byte(CERO[0]);
    recorrido_de_byte(CERO[1]);
    recorrido_de_byte(CERO[2]);
    break;
   
    case '1':
    recorrido_de_byte(UNO[0]);
    recorrido_de_byte(UNO[1]);
    recorrido_de_byte(UNO[2]);
    break; 

    case '2':
    recorrido_de_byte(DOS[0]);
    recorrido_de_byte(DOS[1]);
    recorrido_de_byte(DOS[2]);
    break; 

    case '3':
    recorrido_de_byte(TRES[0]);
    recorrido_de_byte(TRES[1]);
    recorrido_de_byte(TRES[2]);
    break; 

    case '4':
    recorrido_de_byte(CUATRO[0]);
    recorrido_de_byte(CUATRO[1]);
    recorrido_de_byte(CUATRO[2]);
    break; 
    
    case '5':
    recorrido_de_byte(CINCO[0]);
    recorrido_de_byte(CINCO[1]);
    recorrido_de_byte(CINCO[2]);
    break; 

    case '6':
    recorrido_de_byte(SEIS[0]);
    recorrido_de_byte(SEIS[1]);
    recorrido_de_byte(SEIS[2]);
    break; 

    case '7':
    recorrido_de_byte(SIETE[0]);
    recorrido_de_byte(SIETE[1]);
    recorrido_de_byte(SIETE[2]);
    break; 

    case '8':
    recorrido_de_byte(OCHO[0]);
    recorrido_de_byte(OCHO[1]);
    recorrido_de_byte(OCHO[2]);
    break; 

    case '9':
    recorrido_de_byte(NUEVE[0]);
    recorrido_de_byte(NUEVE[1]);
    recorrido_de_byte(NUEVE[2]);
    break; 

    case 'A':
    recorrido_de_byte(A[0]);
    recorrido_de_byte(A[1]);
    recorrido_de_byte(A[2]);
    break; 

    case 'B':
    recorrido_de_byte(B[0]);
    recorrido_de_byte(B[1]);
    recorrido_de_byte(B[2]);
    break; 

    case 'C':
    recorrido_de_byte(C[0]);
    recorrido_de_byte(C[1]);
    recorrido_de_byte(C[2]);
    break; 

    case 'D':
    recorrido_de_byte(D[0]);
    recorrido_de_byte(D[1]);
    recorrido_de_byte(D[2]);
    break; 

    case 'E':
    recorrido_de_byte(E[0]);
    recorrido_de_byte(E[1]);
    recorrido_de_byte(E[2]);
    break; 

    case 'F':
    recorrido_de_byte(F[0]);
    recorrido_de_byte(F[1]);
    recorrido_de_byte(F[2]);
    break; 

    case 'G':
    recorrido_de_byte(G[0]);
    recorrido_de_byte(G[1]);
    recorrido_de_byte(G[2]);
    break; 

    case 'H':
    recorrido_de_byte(H[0]);
    recorrido_de_byte(H[1]);
    recorrido_de_byte(H[2]);
    break; 

    case 'I':
    recorrido_de_byte(I[0]);
    recorrido_de_byte(I[1]);
    recorrido_de_byte(I[2]);
    break; 

    case 'J':
    recorrido_de_byte(J[0]);
    recorrido_de_byte(J[1]);
    recorrido_de_byte(J[2]);
    break; 

    case 'K':
    recorrido_de_byte(K[0]);
    recorrido_de_byte(K[1]);
    recorrido_de_byte(K[2]);
    break; 

    case 'L':
    recorrido_de_byte(L[0]);
    recorrido_de_byte(L[1]);
    recorrido_de_byte(L[2]);
    break; 

    case 'M':
    recorrido_de_byte(M[0]);
    recorrido_de_byte(M[1]);
    recorrido_de_byte(M[2]);
    break; 

    case 'N':
    recorrido_de_byte(N[0]);
    recorrido_de_byte(N[1]);
    recorrido_de_byte(N[2]);
    break; 

    case 'O':
    recorrido_de_byte(O[0]);
    recorrido_de_byte(O[1]);
    recorrido_de_byte(O[2]);
    break; 
    
    case 'P':
    recorrido_de_byte(P[0]);
    recorrido_de_byte(P[1]);
    recorrido_de_byte(P[2]);
    break; 

    case 'Q':
    recorrido_de_byte(Q[0]);
    recorrido_de_byte(Q[1]);
    recorrido_de_byte(Q[2]);
    break; 

    case 'R':
    recorrido_de_byte(R[0]);
    recorrido_de_byte(R[1]);
    recorrido_de_byte(R[2]);
    break; 

    case 'S':
    recorrido_de_byte(S[0]);
    recorrido_de_byte(S[1]);
    recorrido_de_byte(S[2]);
    break; 

    case 'T':
    recorrido_de_byte(T[0]);
    recorrido_de_byte(T[1]);
    recorrido_de_byte(T[2]);
    break; 
    
     case 'U':
    recorrido_de_byte(U[0]);
    recorrido_de_byte(U[1]);
    recorrido_de_byte(U[2]);
    break; 

    case 'V':
    recorrido_de_byte(V[0]);
    recorrido_de_byte(V[1]);
    recorrido_de_byte(V[2]);
    break; 

    case 'W':
    recorrido_de_byte(W[0]);
    recorrido_de_byte(W[1]);
    recorrido_de_byte(W[2]);
    break; 

    case 'X':
    recorrido_de_byte(X[0]);
    recorrido_de_byte(X[1]);
    recorrido_de_byte(X[2]);
    break; 

    case 'Y':
    recorrido_de_byte(Y[0]);
    recorrido_de_byte(Y[1]);
    recorrido_de_byte(Y[2]);
    break; 

    case 'Z':
    recorrido_de_byte(Z[0]);
    recorrido_de_byte(Z[1]);
    recorrido_de_byte(Z[2]);
    break; 
    default:
    break;
    }

     digitalWrite(12, HIGH);
     delay(retraso);
     digitalWrite(12, LOW); 
    
  }



  
