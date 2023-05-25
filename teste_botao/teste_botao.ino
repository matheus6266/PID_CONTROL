bool teste, now, last;


void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);         // Configura o pino 13 (led interno) como saída;
  pinMode(22, INPUT_PULLUP);    // Configura pino 8 como entrada e habilita pull up interno;

  last = digitalRead(22);
}
void loop() {
  now = digitalRead(22);

  if (now != last){
    delay(10);
  }

  if(now == digitalRead(22)){
    last = now;
  }

  Serial.println(teste); 
  if (now == LOW) { // Botão Pressionado;
    digitalWrite(13, HIGH);    // Liga led.
  }
  else {                       // Botão Não Pressionado
    digitalWrite(13, LOW);     // Desliga led.
  }

  delay(100);
}