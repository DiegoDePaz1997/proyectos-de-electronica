#include <Arduino.h>

#define vel_normal 150
#define vel_giro   100
#define vel_add    40

//motores
#define mot_der_1 3
#define mot_der_2 5
#define mot_izq_1 6
#define mot_izq_2 7

class mov{
private:

    void movimiento(int izq, int der){
        if(izq>=0){ // avance de derecha
            digitalWrite(mot_der_1, der);
            digitalWrite(mot_der_2, 0);
        }
        else{
            der*= -1;
            digitalWrite(mot_der_1, 0);
            digitalWrite(mot_der_2, der);            
        }

        if(der>=0){ // avance de izquierda
            digitalWrite(mot_izq_1, izq);
            digitalWrite(mot_izq_2, 0);
        }
        else{
            izq*= -1;
            digitalWrite(mot_izq_1, 0);
            digitalWrite(mot_izq_2, izq);            
        }        

    }




public:
    void adelante(){
        movimiento(vel_normal, vel_normal);
    }
    void atras(){
        movimiento(-vel_normal, -vel_normal);
    }

    void giro_izq_der(bool cond){ // si cond == tue gira der else gira izquierda
        if(cond){movimiento(-vel_giro, vel_giro);}//der
        else{movimiento(vel_giro, -vel_giro);}
        
    }


    //corrige el lado que se indica
    void correc_der(){
        movimiento(vel_normal, vel_normal + vel_add);
    }
    void correc_izq(){
        movimiento(vel_normal + vel_add, vel_normal);
    }



};