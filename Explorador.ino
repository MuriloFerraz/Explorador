/*
------------ Versão 1.02 --------------------
          
       Autor: Murilo Ferraz de Almeida
         murilof.almeida@hotmail.com
         murilo.ferraz125@gmail.com
		 
		   09/2015
---------------------------------------------------


----------------- Requerimentos ------------------------
     Este código foi escrito para controlar
      um robô explorador auto-evasivo

    HARDWARE:
    1 Arduino ou compatível
    1 HC-SR04 - Sensor Ultrasonico
    1 SG90 - Micro servo 9G
    1 TB6612FNG - Dual Motor Carrier / L293D
    2 Motores AC - 3V / TAMIYA TWIN-MOTOR GEARBOX


-----------------------------------------------------
   
   Espero que este código lhe seja útil e seu projeto
  funcione corretamente, não deixe de ler o arquivo de licença.
     
           Se possível site a fonte.
----------------------------------------------------------

----------------- Direção do servo motor -----------------
          (90) Frente
          (180) Esquerda
          (0) Direita
----------------------------------------------------------

*/

// -------- bibliotecas para correto funcionamento ----------

/////////

#include <Servo.h>
#include <Ultrasonic.h>


// delcaração das variaveis e objetos

Servo servo; // servo motor 

int potPin = A0; // pino do potenciometro
int potVal = 0; // leitura do potenciometro

int PWMA = 11;  // L293D -> Enable A
int PWMB = 3; //  L293D -> Enable B
int STBY = 6; // Desabilite se estiver a usar L293D

int AIN1 = 7; // Motor A - 1
int AIN2 = 8; // Motor A - 2

int BIN1 = 5; // Motor B - 1
int BIN2 = 4; // Motor B - 2

int LED = 13;



// ------ Sensor Ultrasonic -----------------
float distanciaES = 0;  // Distancia em CM a esquerda
float distanciaDI = 0;  // Distancia em CM a direita
float distanciaFR = 0;  // Distancia em CM a frente
// float distancia[3]; // testar utilização de vetor

Ultrasonic ultrasonic(10, 12); // (10 - Trig PIN, 12 - Echo PIN)

// -------------------------------------------



void setup() {
  // put your setup code here, to run once:

  servo.attach(9); // Servo conectado ao Pino 9
  
  pinMode(PWMA, OUTPUT); // Velocidade Motor A - desabilite se estiver a usar L293D
  pinMode(PWMB, OUTPUT); // Velocidade Motor B - desabilite se estiver a usar L293D
  pinMode(STBY, OUTPUT); // Parada dos motores e Diver - Desabilite se estiver a usar L293D
  
  pinMode(AIN1, OUTPUT); // Motor A - 1
  pinMode(AIN2, OUTPUT); // Motor A - 2
  pinMode(BIN1, OUTPUT); // Motor B - 1
  pinMode(BIN2, OUTPUT); // Motor B - 2

  pinMode(LED, OUTPUT); // led

  pinMode(potPin, INPUT); // definir como INPUT o pino do potenciometro



}

void loop() {
  // put your main code here, to run repeatedly:


 servo.write(90); // Apontar o servo para frente a cada execução
 potVal = analogRead(potPin);  // ler o valor informado pelo potenciometro
 potVal = map(potVal, 0, 1023, 0, 255); // ajustar intervalo de leitura

// o intervalo de leitura do potencionetro eh 0 a 1023
// a funcao map converte este intervalo de 0 a 1023 para 0 a 255
// intervalo ideal para controle de velocidade do driver dos motores
 
    explora(); // verifique o metodo explora
    
  //frente(); // Teste
  //esquerda(); // teste
  //direita(); // teste
  //parado(); / teste
  
delay (100); // Aumentar este tempo pode causar colisão (robo com obstaculos)
}

// ----- metodos -----------
void frente()  
{
	// o robo anda para frente 
  digitalWrite(STBY, HIGH);  // Desabilite sempre que ocorrer se estiver a usar L293D
  
  analogWrite(PWMA, potVal); // velocidade ajustada pelo potenciometro 
  digitalWrite(AIN1, LOW); 
  digitalWrite(AIN2, HIGH);
  
  analogWrite(PWMB, potVal);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  digitalWrite(LED, HIGH);
}

void parado()
{
	// o robo esta parado
  digitalWrite(STBY, LOW);
  
  analogWrite(PWMA, 0);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  
  analogWrite(PWMB, 0);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  //digitalWrite(LED, LOW);
}

void esquerda()
{
	// um motor gira ao contario do outro
	// fazendo o robo girar
  digitalWrite(STBY, HIGH);
  
  analogWrite(PWMA, potVal);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  
  analogWrite(PWMB, potVal);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  digitalWrite(LED, HIGH);
}

void direita()
{
	// um motor gira ao contario do outro
	// fazendo o robo girar
  digitalWrite(STBY, HIGH);
  
  analogWrite(PWMA, potVal);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  
  analogWrite(PWMB, potVal);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  digitalWrite(LED, HIGH);
}



void explora()
{
  distanciaFR = (ultrasonic.Ranging(CM)); // distancia a frente
  if (distanciaFR > 20) // se distancia a frente > 20
    {
      frente();  // metodo frente
    }
   else
   {
    parado(); // parado 

     servo.write(180); // gira o servo para esquerda
     delay(1000); // tempo para o servo.
     distanciaES = (ultrasonic.Ranging(CM)); // medir e armazenar na variavel esquerda
     delay(100);

     servo.write(0); // gira o servo para direita
     delay(1000);
     distanciaDI = (ultrasonic.Ranging(CM)); // medir e armazenar na var. direita
     delay(100);

     servo.write(90); // girar o servo para frente para não andar se nao houver espaco
     delay(1000);
 
 // -- daqui em diante, comparacoes para decidir em qual direcao seguir
 // -- da pra melhorar, e muito (kkkkk).

      if (distanciaES > distanciaDI)
        {
          esquerda();
          delay(100);
        }

       if (distanciaES < distanciaDI)
        {
          direita();
          delay(100); 
        }

       else
        {
          esquerda();
          delay(100); 
        }

       distanciaES = 0;
       distanciaDI = 0; 
   }

}


// ------------------------------------------------

/*

O robo irá comparar as distancias á esquerda e á direita e irá girar
para onde a distancia for maior, caso as duas distancias forem igual,
o robo irá girar para esquerda.

Mesmo girando para esquerda ou direita, caso encontre obstaculo,
o robo não proseguirá para frente se a distancia for menor que 20cm,
ele continuará giranado até encontar distancia suficiente.

Infelizmente este robo não é capaz de escapar de labirinto.


*/


// finalizado em: 31/10/2014
// revisão em: 20/09/2015
