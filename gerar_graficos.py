# Alunos: Matheus Paul Lopuch e Leonardo Augusto Dolvitsch

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def main():
    # Carregar o arquivo Excel
    excel_file = "resultados_pyjava.xlsx"
    print(f"Carregando dados de {excel_file}...")
    
    # Usar pandas para ler o arquivo Excel
    df = pd.read_excel(excel_file)
    
    # Exibir informações sobre os dados carregados
    print(f"Carregados {len(df)} registros.")
    print("Colunas:", df.columns.tolist())
    
    # Obter valores únicos
    algorithms = df['Algoritmo'].unique().tolist()
    languages = df['Linguagem'].unique().tolist()
    cases = df['Caso'].unique().tolist()
    sizes = sorted(df['Tamanho da entrada'].unique().tolist())
    
    print(f"Algoritmos: {algorithms}")
    print(f"Linguagens: {languages}")
    print(f"Casos: {cases}")
    print(f"Tamanhos: {sizes}")
    
    # Gerar gráficos por algoritmo (comparando Python e Java)
    for algo in algorithms:
        for case in cases:
            plt.figure(figsize=(10, 6))
            
            for lang in languages:
                # Filtrar dados
                data = df[(df['Algoritmo'] == algo) & (df['Linguagem'] == lang) & (df['Caso'] == case)]
                data = data.sort_values('Tamanho da entrada')
                
                # Plotar linha
                plt.plot(data['Tamanho da entrada'], data['Tempo de execução (segundos)'], 
                         marker='o', label=lang)
            
            # Adicionar linha de tendência teórica
            if algo in ["Bubble Sort", "Selection Sort", "Insertion Sort"]:
                # O(n²)
                x = np.array(sizes)
                y_max = df[(df['Algoritmo'] == algo) & (df['Caso'] == case)]['Tempo de execução (segundos)'].max()
                scale = y_max / (sizes[-1]**2)
                plt.plot(x, x**2 * scale * 0.8, '--', label='O(n²)', color='black')
            else:
                # O(n log n)
                x = np.array(sizes)
                y_max = df[(df['Algoritmo'] == algo) & (df['Caso'] == case)]['Tempo de execução (segundos)'].max()
                scale = y_max / (sizes[-1] * np.log(sizes[-1]))
                plt.plot(x, x * np.log(x) * scale * 0.8, '--', label='O(n log n)', color='black')
            
            # Configurar gráfico
            plt.title(f"{algo} - Caso {case}")
            plt.xlabel("Tamanho da Entrada")
            plt.ylabel("Tempo de Execução (segundos)")
            plt.grid(True)
            plt.legend()
            
            # Salvar gráfico
            output_file = f"{algo.replace(' ', '_')}_{case}.png"
            plt.savefig(output_file)
            print(f"Gráfico salvo: {output_file}")
    
    # Gerar gráficos por linguagem (comparando todos os algoritmos)
    for lang in languages:
        for case in cases:
            plt.figure(figsize=(10, 6))
            
            for algo in algorithms:
                # Filtrar dados
                data = df[(df['Algoritmo'] == algo) & (df['Linguagem'] == lang) & (df['Caso'] == case)]
                data = data.sort_values('Tamanho da entrada')
                
                # Plotar linha
                plt.plot(data['Tamanho da entrada'], data['Tempo de execução (segundos)'], 
                         marker='o', label=algo)
            
            # Configurar gráfico
            plt.title(f"Algoritmos em {lang} - Caso {case}")
            plt.xlabel("Tamanho da Entrada")
            plt.ylabel("Tempo de Execução (segundos)")
            plt.grid(True)
            plt.legend()
            plt.yscale('log')  # Escala logarítmica
            
            # Salvar gráfico
            output_file = f"comparacao_{lang}_{case}.png"
            plt.savefig(output_file)
            print(f"Gráfico salvo: {output_file}")
    
    print("Todos os gráficos foram gerados com sucesso!")

if __name__ == "__main__":
    main()
