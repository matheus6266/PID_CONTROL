import serial
import time
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Configurar a porta serial
porta = 'COM3'  # Porta serial do Arduino (pode variar dependendo do sistema operacional)
velocidade = 115200  # Velocidade de comunicação serial
iteracao = 0
RespostaDegrau = []

timeout = 5  # Tempo limite de espera em segundos
start_time = time.time()

# Abrir a porta serial
arduino = serial.Serial(porta, velocidade)

def gerar_lista_pares_impares(numero):
    lista_pares = [i for i in range(numero) if i % 2 == 0]
    lista_impares = [i for i in range(numero) if i % 2 != 0]
    return lista_pares, lista_impares

# Ler e exibir os dados da porta serial
while True:
    if arduino.in_waiting > 0:
        dados = arduino.readline().decode('utf-8').rstrip()
        RespostaDegrau.append(dados)
        start_time = time.time()  # Reiniciar o tempo de início

    if (time.time() - start_time) > timeout:
        break

RespostaDegrau = [item for item in RespostaDegrau if item]
lista_pares, lista_impares = gerar_lista_pares_impares(len(RespostaDegrau))


resposta = [RespostaDegrau[i] for i in lista_impares]
degrau = [RespostaDegrau[i] for i in lista_pares]

df = pd.DataFrame({'Degrau': degrau, 'Resposta': resposta})

# Salvar o DataFrame em formato .xlsx
nome_arquivo = 'Resposta ao Degrau.xlsx'

df.to_excel(nome_arquivo, index=False)

passo = 5
vetor_tempo = np.arange(0, len(RespostaDegrau)/2, 1)

print("DataFrame salvo com sucesso em", nome_arquivo)

plt.plot(vetor_tempo, resposta)

# Adicionar rótulos aos eixos
plt.xlabel('Degrau')
plt.ylabel('Resposta')

# Adicionar título ao gráfico
plt.title('Respsota Motor CC ao Degrau')

# Mostrar o gráfico
plt.show()

        