import serial
import time
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Configurar a porta serial
porta = 'COM4'  # Porta serial do Arduino (pode variar dependendo do sistema operacional)
velocidade = 115200  # Velocidade de comunicação serial
iteracao = 0
RespostaDegrau = []
dados = ""
timeout = 20  # Tempo limite de espera em segundos
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
        print(dados)
        start_time = time.time()
    if ((time.time() - start_time) > timeout) or (dados == "SAIR PYTHON"):
        break
        
dados_coletados = [item for item in RespostaDegrau if item]
#print(dados_coletados)   
if dados_coletados[0] == 'modelagem':
    RespostaDegrau = dados_coletados
    RespostaDegrau.pop(0)
    lista_pares, lista_impares = gerar_lista_pares_impares(len(RespostaDegrau))
    resposta = [RespostaDegrau[i] for i in lista_impares]
    degrau = [RespostaDegrau[i] for i in lista_pares]
    df = pd.DataFrame({'Degrau': degrau, 'Resposta': resposta})
    # Salvar o DataFrame em formato .xlsx
    nome_arquivo = 'Resposta ao Degrau.xlsx'
    df.to_excel(nome_arquivo, index=False)
    passo = 0.005
    valor = 0
    vetor_tempo = []
    num_posicoes = int(len(RespostaDegrau)/2)
    print(num_posicoes)
    for _ in range(num_posicoes):
        vetor_tempo.append(valor)
        valor += passo

    #vetor_tempo = np.arange(0, len(RespostaDegrau)/2, 0.005)
    print("DataFrame salvo com sucesso em", nome_arquivo)
    plt.plot(vetor_tempo, resposta)
    # Adicionar rótulos aos eixos
    plt.xlabel('Degrau')
    plt.ylabel('Resposta')
    # Adicionar título ao gráfico
    plt.title('Respsota Motor CC ao Degrau')
    # Mostrar o gráfico

    plt.savefig("Resposta ao Degrau.png")
    plt.show()

if (dados_coletados[0] == 'controle'):
    teste_controlador = dados_coletados
    teste_controlador.pop(0)

    output = []
    input = []
    set_point = []

    for item in teste_controlador:
        if item.startswith("output:"):
            output.append(float(item.split(":")[1]))
        elif item.startswith("input:"):
            input.append(float(item.split(":")[1]))
        elif item.startswith("set point:"):
            set_point.append(float(item.split(":")[1]))
    
    df = pd.DataFrame({"Output": output, "Input": input, "Set Point": set_point})
    nome_arquivo = 'Teste Controlador.xlsx'
    df.to_excel(nome_arquivo, index=False)

    vetor_tempo = np.arange(0, len(output), 1)
    
    # Plotar os dados
    #plt.plot(vetor_tempo, output, label='Output')
    plt.plot(vetor_tempo, input, label='Input')
    plt.plot(vetor_tempo, set_point, label='Set Point')

    # Configurações do gráfico
    plt.xlabel('Tempo')
    plt.ylabel('Valores')
    plt.title('Gráfico de Variáveis')
    plt.legend()

    plt.savefig("Resposta ao Degrau.png")

    # Exibir o gráfico
    plt.show()
    

    print("Output: {}".format(output))
    print("Input: {}".format(input))
    print("Set Point: {}".format(set_point))