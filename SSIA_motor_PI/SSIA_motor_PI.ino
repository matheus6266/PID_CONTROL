//definir os pinos como entradas e saídas e configurar
#define pinCHA 2
#define pinCHB 3
#define pinPWM1 8
#define pinPWM2 9

int leitA,leitB;
double integral=0;
double erro,input,output;
double dt=0.005; // 5ms
double setpoint=10;
volatile long int posicao=0;
double Kp=58,Ki=50;


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

for(int i=0;i<=250;i++)
{
  // leitura do sensor
  input=posicao;
  posicao=0;

  // cálculo da ação de controle
  erro=setpoint-input;
  integral=integral+erro*dt;
  output=Kp*erro + Ki*integral;

  // acionamento da saída
  if(output>=0)
  {
     if(output>255) {output=255;}
     analogWrite(pinPWM1, 0);
     analogWrite(pinPWM2, output);
  }
  else
  {
     if(output<-255) {output=-255;}
     analogWrite(pinPWM1, -output); // 0 a 255
     analogWrite(pinPWM2, 0);    
  }

  Serial.print(output);
  Serial.print('\t');
  Serial.println(input);

  delay(5);
}

}
