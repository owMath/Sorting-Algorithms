import time
import random
import matplotlib.pyplot as plt
import numpy as np

# 1. Insertion Sort
def insertion_sort(arr):
    for i in range(1, len(arr)):
        key = arr[i]
        j = i - 1
        while j >= 0 and arr[j] > key:
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = key
    return arr

# 2. Selection Sort
def selection_sort(arr):
    for i in range(len(arr)):
        min_idx = i
        for j in range(i + 1, len(arr)):
            if arr[j] < arr[min_idx]:
                min_idx = j
        arr[i], arr[min_idx] = arr[min_idx], arr[i]
    return arr

# 3. Bubble Sort
def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        swapped = False
        for j in range(0, n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
                swapped = True
        if not swapped:
            break
    return arr

# 4. Merge Sort
def merge_sort(arr):
    if len(arr) <= 1:
        return arr
    
    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])
    
    return merge(left, right)

def merge(left, right):
    result = []
    i = j = 0
    
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1
    
    result.extend(left[i:])
    result.extend(right[j:])
    return result

# 5. Quick Sort
def quick_sort(arr):
    if len(arr) <= 1:
        return arr
    
    pivot = arr[len(arr) // 2]
    left = [x for x in arr if x < pivot]
    middle = [x for x in arr if x == pivot]
    right = [x for x in arr if x > pivot]
    
    return quick_sort(left) + middle + quick_sort(right)

# Versão in-place do Quick Sort (alternativa)
def quick_sort_inplace(arr, low=0, high=None):
    if high is None:
        high = len(arr) - 1
    
    if low < high:
        pivot_idx = partition(arr, low, high)
        quick_sort_inplace(arr, low, pivot_idx - 1)
        quick_sort_inplace(arr, pivot_idx + 1, high)
    
    return arr

def partition(arr, low, high):
    pivot = arr[high]
    i = low - 1
    
    for j in range(low, high):
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]
    
    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    return i + 1

# Função para testar os algoritmos
def test_algorithm(sort_function, arr):
    # Fazemos uma cópia para não alterar o array original
    test_arr = arr.copy()
    
    # Medimos o tempo de execução
    start_time = time.time()
    sort_function(test_arr)
    end_time = time.time()
    
    # Retornamos o tempo de execução em segundos
    return end_time - start_time

# Função para gerar arrays de teste
def generate_test_array(size, case_type="random"):
    if case_type == "random":
        return [random.randint(0, 10000) for _ in range(size)]
    elif case_type == "sorted":
        return list(range(size))  # Melhor caso
    elif case_type == "reverse":
        return list(range(size, 0, -1))  # Pior caso para muitos algoritmos
    else:
        raise ValueError("Tipo de caso inválido")

# Função para executar todos os testes
def run_tests(sizes, algorithms, case_types=["random"]):
    results = {algo.__name__: {size: {} for size in sizes} for algo in algorithms}
    
    for size in sizes:
        print(f"Testando com tamanho {size}...")
        
        for case_type in case_types:
            test_array = generate_test_array(size, case_type)
            
            for algo in algorithms:
                # Não executa algoritmos quadráticos para entradas muito grandes
                if size > 100000 and algo.__name__ in ["insertion_sort", "selection_sort", "bubble_sort"]:
                    results[algo.__name__][size][case_type] = "N/A (muito lento)"
                    continue
                
                print(f"  Executando {algo.__name__} para caso {case_type}...")
                execution_time = test_algorithm(algo, test_array)
                results[algo.__name__][size][case_type] = execution_time
                print(f"    Tempo: {execution_time:.6f} segundos")
    
    return results

# Função para plotar os resultados
def plot_results(results, sizes, case_type="random"):
    plt.figure(figsize=(12, 8))
    
    for algo_name, size_data in results.items():
        times = []
        valid_sizes = []
        
        for size in sizes:
            if case_type in size_data[size] and size_data[size][case_type] != "N/A (muito lento)":
                times.append(size_data[size][case_type])
                valid_sizes.append(size)
        
        if times:  # Se tiver dados válidos
            plt.plot(valid_sizes, times, marker='o', label=algo_name)
    
    plt.xlabel('Tamanho da Entrada (n)')
    plt.ylabel('Tempo de Execução (segundos)')
    plt.title(f'Desempenho dos Algoritmos de Ordenação - Caso {case_type}')
    plt.grid(True)
    plt.legend()
    plt.xscale('log')  # Escala logarítmica para melhor visualização
    plt.yscale('log')
    
    # Salvar o gráfico
    plt.savefig(f'performance_plot_{case_type}.png')
    plt.show()

# Exemplo de uso:
if __name__ == "__main__":
    # Definição dos algoritmos a serem testados
    algorithms = [insertion_sort, selection_sort, bubble_sort, merge_sort, quick_sort]
    
    # Tamanhos dos arrays para teste
    # Para testes iniciais, usar tamanhos menores
    sizes = [100, 1000, 5000, 10000, 50000, 100000]
    
    # Tipos de casos de teste
    case_types = ["random", "sorted", "reverse"]
    
    # Executar os testes
    results = run_tests(sizes, algorithms, case_types)
    
    # Plotar os resultados para cada tipo de caso
    for case_type in case_types:
        plot_results(results, sizes, case_type)
        
    # Imprimir tabela de resultados
    print("\nResultados (tempo em segundos):")
    print("-" * 80)
    
    for case_type in case_types:
        print(f"\nCaso: {case_type}")
        header = "Algoritmo"
        for size in sizes:
            header += f" | {size}"
        print(header)
        print("-" * len(header))
        
        for algo_name in [algo.__name__ for algo in algorithms]:
            row = algo_name
            for size in sizes:
                if case_type in results[algo_name][size]:
                    value = results[algo_name][size][case_type]
                    if isinstance(value, float):
                        row += f" | {value:.6f}"
                    else:
                        row += f" | {value}"
                else:
                    row += " | N/A"
            print(row)