/*
------------ Codigo definitivo --------------------
          
       Autor: Murilo Ferraz de Almeida
         murilof.almeida@hotmail.com
         murilo.ferraz125@gmail.com
		 
		          06/2015
---------------------------------------------------


----------------- Requerimentos ------------------------
     Este código foi escrito para controlar
      um robô explorador auto-evasivo

    HARDWARE:
    1 Garagino ou Arduino / Atmega 328P based boards
    1 HC-SR04 - Sensor Ultrasonico
    1 SG90 - Micro servo 9G
    1 TB6612FNG - Dual Motor Carrier / L293D
    2 Motores AC - 3V / TAMIYA TWIN-MOTOR GEARBOX


-----------------------------------------------------
   
   Espero que este código lhe seja útil e seu projeto
  funcione corretamente, caso seja utilizado em projetos
  de estudos, desejo ainda que tire exelentes notas.
     
           Se possível site a fonte.
----------------------------------------------------------

----------------- Direção do servo motor -----------------
          (90) Frente
          (180) Esquerda
          (0) Direita
----------------------------------------------------------

*/

// -------- bibliotecas para correto funcionamento ----------

#include <Servo.h>
#include <Ultrasonic.h>

//----------------------------------------------------------

//----------- Driver POLOLU / L293D ------------------------

 int PWMA = 9;  // Desabilite se estiver a usar L293D
 int PWMB = 10; // Desabilite se estiver a usar L293D
 int STBY = 13; // Desabilite se estiver a usar L293D

int AIN1 = 4;
int AIN2 = 5;

int BIN1 = 6;
int BIN2 = 7;

//---------------------------------------------------------
//--------- SENSOR ULTRASONIC -----------------------------

Ultrasonic sensor (11, 12);   //  11 = TRIG -- 12 = ECHO

long microsec = 0;
float distanciaES = 0;  // Distancia em CM a esquerda
float distanciaDI = 0;  // Distancia em CM a direita
float distanciaFR = 0;  // Distancia em CM a frente

//---------------------------------------------------------
// ---------------- SERVO MOTOR ---------------------------

Servo servo;  // SERVO MOTOR

//--------------------------------------------------------
//-------------------------------------------------------- 

/*

Esta parte do gódigo é executada apenas uma vez

*/

void setup()
{

  
  servo.attach(3); // servo motor ligado à porta 3.
  
  // Controle do Driver de motor
  
  pinMode(PWMA, OUTPUT); // Velocidade Motor A - desabilite se estiver a usar L293D
  pinMode(PWMB, OUTPUT); // Velocidade Motor B - desabilite se estiver a usar L293D
  pinMode(STBY, OUTPUT); // Parada dos motores e Diver - Desabilite se estiver a usar L293D
  
  pinMode(AIN1, OUTPUT); // Motor A - 1
  pinMode(AIN2, OUTPUT); // Motor A - 2
  pinMode(BIN1, OUTPUT); // Motor B - 1
  pinMode(BIN2, OUTPUT); // Motor B - 2
  
  
  
}


//------- Loop Infinito -----------------------------------


void loop()
{
microsec = sensor.timing(); // Medição do tempo de resposta do sensor ultrasonico
distanciaFR = sensor.convert(microsec, Ultrasonic::CM); // converter o tempo de resposta para CM.

explora();
delay(1000);
}


//----------------------------------------------------------
//------------- métodos ------------------------------------

void explora()
{

  if (distanciaFR > 20)
    {
      frente();  
    }
  
  else
    {
      parado();
      
      servo.write(180);
      delay(1000);
      microsec = sensor.timing();
      distanciaES = sensor.convert(microsec, Ultrasonic::CM);
      
      servo.write(0);
      delay(1000);
      microsec = sensor.timing();
      distanciaDI = sensor.convert(microsec, Ultrasonic::CM);
      delay(1000);
      
      servo.write(90);
      delay(1000);
      
      
          if (distanciaES > distanciaDI)
              {
                esquerda();
                delay(1000);              
              }
           
           if (distanciaES < distanciaDI)
                {
                direita();
                delay(1000);
                } 
              
            else
                {
                  esquerda();
                  delay(1000);                
                }    
    }
  
  
  
  
}

void frente()
{
  digitalWrite(STBY, HIGH);
  
  analogWrite(PWMA, 255);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  
  analogWrite(PWMB, 255);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, HIGH);
  

}


void esquerda()
{
  digitalWrite(STBY, HIGH);
  
  analogWrite(PWMA, 255);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  
  analogWrite(PWMB, 255);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  
}

void direita()
{ 
  digitalWrite(STBY, HIGH);
  
  analogWrite(PWMA, 255);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  
  analogWrite(PWMB, 255);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void parado()
{
  digitalWrite(STBY, LOW);
  
  analogWrite(PWMA, 0);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  
  analogWrite(PWMB, 0);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}



// finalizado em: 31/10/2014
// revisão em: 05/06/2015
