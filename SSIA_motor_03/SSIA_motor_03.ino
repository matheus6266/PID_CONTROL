#include <LiquidCrystal.h>


//definir os pinos como entradas e saídas e configurar
#define pinCHA 2
#define pinCHB 3
#define pinPWM1 8
#define pinPWM2 9

volatile long int posicao=0;
bool teste1 = true;
bool teste2 = true;
int leitA,leitB;

const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
int valorMapeado, valorPotenciometro;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void interrupt_pinCHA()
{
   leitA=digitalRead(pinCHA);
   leitB=digitalRead(pinCHB);
   if(leitA==HIGH)
   {
      if(leitB==HIGH) posicao--;
      if(leitB==LOW)  posicao++;
   }
   else
   {
      if(leitB==HIGH) posicao++;
      if(leitB==LOW)  posicao--;
   }
}

void interrupt_pinCHB()
{
   leitA=digitalRead(pinCHA);
   leitB=digitalRead(pinCHB);
   if(leitB==HIGH)
   {
      if(leitA==HIGH) posicao++;
      if(leitA==LOW)  posicao--;
   }
   else
   {
      if(leitA==HIGH) posicao--;
      if(leitA==LOW)  posicao++;
   }
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  lcd.begin(16, 2);

  pinMode(pinCHA, INPUT);
  pinMode(pinCHB, INPUT);

  pinMode(pinPWM1, OUTPUT);
  pinMode(pinPWM2, OUTPUT);

  analogWrite(pinPWM1, 0); // (0 a 255)
  analogWrite(pinPWM2, 0);

  attachInterrupt(digitalPinToInterrupt(pinCHA), interrupt_pinCHA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinCHB), interrupt_pinCHB, CHANGE);

  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (teste1 == true) {

    for(int i=1;i<=50;i++)
  {
     analogWrite(pinPWM1, 0);
     analogWrite(pinPWM2, 0);
     Serial.print(0);Serial.println("\n");
     Serial.println(posicao);    
     posicao=0;
     delay(5);
  }


  }
  
  if (teste2 == true) {

    for(int i=1;i<=200;i++)
  { 
    valorPotenciometro = analogRead(A0); // Leitura do valor do potenciômetro
    valorMapeado = map(valorPotenciometro, 0, 1023, 0, 255); // Mapeia o valor para o intervalo de 0 a 255

    analogWrite(pinPWM1, 0);
    analogWrite(pinPWM2, 127);
    Serial.print(127);Serial.println("\n");
    Serial.println(posicao);    
     
    lcd.clear(); // Limpa o conteúdo do display

    // Escreve o valor da variável no display
    lcd.setCursor(0, 0);
    lcd.print("Veloc. Atual: ");
    lcd.print(posicao);
    lcd.setCursor(0, 1);
    lcd.print("Set Point: ");
    lcd.print(valorMapeado);
    posicao=0;
    delay(5);
  }


  }
  
  teste1 = true;
  teste2 = true;

  analogWrite(pinPWM1, 0); // (0 a 255)
  analogWrite(pinPWM2, 0);



  delay(1000);
  
}
