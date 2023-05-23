import serial

# Configurar a porta serial
porta = 'COM3'  # Porta serial do Arduino (pode variar dependendo do sistema operacional)
velocidade = 9600  # Velocidade de comunicação serial

# Abrir a porta serial
arduino = serial.Serial(porta, velocidade)

# Ler e exibir os dados da porta serial
while True:
    if arduino.in_waiting > 0:
        dados = arduino.readline().decode('utf-8').rstrip()
        print('Dados recebidos:', dados)