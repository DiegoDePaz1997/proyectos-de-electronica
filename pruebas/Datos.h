#include <Arduino.h>



//sensores
#define sensor_derecha 11
#define sensor_izquierda 12
#define sensor_enmedio A0

//colores
#define negro 1
#define blanco 0
#define amarillo 10

//color amarillo
#define valor  250  //ese es aproximado al valor querido
#define mas_menos 10 // se le suma y resta a valor para tener ese rango de deteccion


#define N 5//tamaño de los vectores solo numeros impares

//tiempos
#define t_periodo 1 //periodo entre sensadas    





class Datos{
    private:
    //variables de datos
    bool der [N];
    bool izq [N];
    float vec_med [N];

    //datos tratados
    bool moda_der;
    bool moda_izq;
    byte prom_color; // blanco; amarillo; negro




    void moda(){
        int der_ = 0;
        int izq_ = 0;

        for(byte i =0; i<N; i++){
            if(der[i]){der_+=1;} 
            else{der_-=1;}

            if(izq[i]){izq_+=1;}
            else{izq_-=1;}
        }

        if(der_>0){moda_der = negro;}
        else{moda_der = blanco;}

        if(izq_>0){moda_izq = negro;}
        else{moda_izq = blanco;}


    }

    void promedio(){
        float acum = 0;
        for(byte i =0; i<N; i++){
            acum+= vec_med[i];
        }
        acum = acum/N;

        if(acum> (valor + mas_menos)){prom_color = negro;}

        else if(acum>(valor - mas_menos) and acum<(valor + mas_menos)){
            prom_color = amarillo;
        }

        else{prom_color = blanco;}


    }






    public:


    void sensado(){
        for(byte i= 0; i<N; i++){
            der[i] = digitalRead(sensor_derecha);
            izq[i] = digitalRead(sensor_izquierda);
            vec_med[i] = analogRead(sensor_enmedio);
        delay(t_periodo);
        }

        moda();
        promedio();

    }

    byte datos(byte dato_n){// moda_izq = 0; moda_der = 1; prom_color = 2;
        if(dato_n == 0){return moda_izq;}
        else if(dato_n ==1){return moda_der;}
        else{return prom_color;}
    }






};
