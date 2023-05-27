#include <LiquidCrystal.h>


//definir os pinos como entradas e saídas e configurar
#define pinCHA 2
#define pinCHB 3
#define pinPWM1 8
#define pinPWM2 9

bool now_1, last_1, now_2, last_2, now_3, last_3, exit_python;
bool teste1 = true;
bool teste2 = true;
bool stop = false;
int leitA,leitB,degrau;
double integral=0;
double erro,input,output;
double dt=0.005; // 5ms
double setpoint;
volatile long int posicao=0;
//double Kp=58,Ki=50;
double Kp=21.82,Ki=1604;

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

void run_PI(){

Serial.println("controle");

while(true){

  for(int i=0;i<=250;i++){

    valorPotenciometro = analogRead(A0); // Leitura do valor do potenciômetro
    valorMapeado = map(valorPotenciometro, 0, 1023, 0, 20); // Mapeia o valor para o intervalo de 0 a 255
    exit_python = digitalRead(26);

    if(exit_python == LOW){
      Serial.print("SAIR PYTHON");
      Serial.print('\n');
    }


    setpoint = valorMapeado;
    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("SP: ");
    //lcd.print("Running");
    //lcd.print(" ");
    //lcd.print(posicao);
    //lcd.setCursor(0, 1);
    //lcd.print("PV:");
    //lcd.print(posicao);
    
    
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

    Serial.print("output:");
    Serial.print(output);
    Serial.print('\n');
    Serial.print("input:");
    Serial.print(input);
    Serial.print('\n');
    Serial.print("set point:");
    Serial.println(valorMapeado);

    delay(5);
    }
  }
}

void run_ident(){

  Serial.println("modelagem");

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

    degrau = 127;

    for(int i=1;i<=300;i++)
  { 
  
    analogWrite(pinPWM1, 0);
    analogWrite(pinPWM2, degrau);
    Serial.print(degrau);
    Serial.println("\n");
    Serial.println(posicao);    
     
    lcd.clear(); // Limpa o conteúdo do display

    // Escreve o valor da variável no display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Veloc. Atual: ");
    lcd.print(posicao);
    lcd.setCursor(0, 1);
    lcd.print("Degrau: ");
    lcd.print(degrau);
    posicao=0;
    delay(5);
  }

  }
  
  teste1 = true;
  teste2 = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fim Teste");

  analogWrite(pinPWM1, 0); // (0 a 255)
  analogWrite(pinPWM2, 0);


  delay(1000);
}

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115200);
  lcd.begin(16, 2);

  pinMode(pinCHA, INPUT);
  pinMode(pinCHB, INPUT);
  pinMode(22, INPUT_PULLUP);
  pinMode(24, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);

  pinMode(pinPWM1, OUTPUT);
  pinMode(pinPWM2, OUTPUT);

  analogWrite(pinPWM1, 0); // (0 a 255)
  analogWrite(pinPWM2, 0);

  attachInterrupt(digitalPinToInterrupt(pinCHA), interrupt_pinCHA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinCHB), interrupt_pinCHB, CHANGE);

  last_1 = digitalRead(22);
  last_2 = digitalRead(24);
  exit_python = digitalRead(26);

  delay(2000);

}

void loop()
{

  now_1 = digitalRead(22);
  now_2 = digitalRead(24);


  if(now_1 != last_1){
    delay(50);
  }

  if(now_2 != last_2){
    delay(50);
  }

  if (now_1 == digitalRead(22)){
    last_1 = now_1;
  }

  if(now_2 == digitalRead(24)){
    last_2 = now_2;
  }

  if (now_1 == LOW){
    lcd.setCursor(0, 0);
    lcd.print("Selecionado:");
    lcd.setCursor(0, 1);
    lcd.print("Controlador PI");
    delay(5000);
    run_PI();

  } else if (now_2 == LOW){
    lcd.setCursor(0, 0);
    lcd.print("Selecionado:");
    lcd.setCursor(0, 1);
    lcd.print("Indent. Sistema");
    delay(5000);
    run_ident();
  }
  
 
  delay(100);

}
