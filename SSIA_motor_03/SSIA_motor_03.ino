//definir os pinos como entradas e saídas e configurar
#define pinCHA 2
#define pinCHB 3
#define pinPWM1 8
#define pinPWM2 9

volatile long int posicao=0;
bool teste1 = true;
bool teste2 = true;
int leitA,leitB;

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
     analogWrite(pinPWM1, 0);
     analogWrite(pinPWM2, 127);
     Serial.print(127);Serial.println("\n");
     Serial.println(posicao);    
     posicao=0;
     delay(5);
  }


  }
  
  teste1 = false;
  teste2 = false;

  analogWrite(pinPWM1, 0); // (0 a 255)
  analogWrite(pinPWM2, 0);

  delay(200);

  return;
  
}
