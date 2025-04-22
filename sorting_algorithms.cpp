#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>

// 1. Insertion Sort
void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 2. Selection Sort
void selectionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        std::swap(arr[i], arr[min_idx]);
    }
}

// 3. Bubble Sort
void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // Se nenhuma troca ocorreu nesta passagem, o array está ordenado
        if (!swapped) {
            break;
        }
    }
}

// 4. Merge Sort
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // Criar arrays temporários
    std::vector<int> L(n1), R(n2);
    
    // Copiar dados para os arrays temporários
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }
    
    // Mesclar os arrays temporários de volta em arr[left..right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copiar os elementos restantes de L[], se houver algum
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    // Copiar os elementos restantes de R[], se houver algum
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortImpl(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        // Encontrar o ponto médio
        int mid = left + (right - left) / 2;
        
        // Ordenar primeira e segunda metades
        mergeSortImpl(arr, left, mid);
        mergeSortImpl(arr, mid + 1, right);
        
        // Mesclar as metades ordenadas
        merge(arr, left, mid, right);
    }
}

void mergeSort(std::vector<int>& arr) {
    mergeSortImpl(arr, 0, arr.size() - 1);
}

// 5. Quick Sort
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // Pivot (elemento a ser posicionado)
    int i = low - 1; // Índice do menor elemento
    
    for (int j = low; j <= high - 1; j++) {
        // Se o elemento atual for menor que o pivot
        if (arr[j] < pivot) {
            i++; // Incrementar índice do menor elemento
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSortImpl(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        // pi é o índice de particionamento
        int pi = partition(arr, low, high);
        
        // Ordenar elementos separadamente antes e depois da partição
        quickSortImpl(arr, low, pi - 1);
        quickSortImpl(arr, pi + 1, high);
    }
}

void quickSort(std::vector<int>& arr) {
    quickSortImpl(arr, 0, arr.size() - 1);
}

// Função para gerar arrays de teste
std::vector<int> generateTestArray(int size, const std::string& caseType) {
    std::vector<int> arr(size);
    
    if (caseType == "random") {
        // Gerar array aleatório
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 10000);
        
        for (int i = 0; i < size; i++) {
            arr[i] = distrib(gen);
        }
    } else if (caseType == "sorted") {
        // Gerar array já ordenado (melhor caso)
        for (int i = 0; i < size; i++) {
            arr[i] = i;
        }
    } else if (caseType == "reverse") {
        // Gerar array em ordem reversa (pior caso para muitos algoritmos)
        for (int i = 0; i < size; i++) {
            arr[i] = size - i;
        }
    }
    
    return arr;
}

// Função para testar um algoritmo
double testAlgorithm(void (*sortFunction)(std::vector<int>&), std::vector<int> arr) {
    // Criar uma cópia do array original para não modificá-lo
    std::vector<int> testArr = arr;
    
    // Medir o tempo de execução
    auto start = std::chrono::high_resolution_clock::now();
    sortFunction(testArr);
    auto end = std::chrono::high_resolution_clock::now();
    
    // Calcular duração em segundos
    std::chrono::duration<double> duration = end - start;
    return duration.count();
}

// Estrutura para armazenar resultados
struct Result {
    std::string algorithm;
    std::string caseType;
    int size;
    double time;
};

// Função para obter informações do sistema
std::string getSystemInfo() {
    std::string info = "";
    
    #ifdef _WIN32
    info += "Windows ";
    #ifdef _WIN64
    info += "64-bit";
    #else
    info += "32-bit";
    #endif
    #elif __APPLE__
    info += "macOS";
    #elif __linux__
    info += "Linux";
    #elif __unix__
    info += "Unix";
    #else
    info += "Desconhecido";
    #endif
    
    return info;
}

int main() {
    std::cout << "Benchmarking de Algoritmos de Ordenação em C++" << std::endl;
    std::cout << "Sistema: " << getSystemInfo() << std::endl;
    
    // Definir tamanhos dos arrays para teste
    std::vector<int> sizes = {100, 1000, 5000, 10000, 30000};
    
    // Definir tipos de casos
    std::vector<std::string> caseTypes = {"random", "sorted", "reverse"};
    
    // Definir algoritmos
    std::vector<std::pair<std::string, void (*)(std::vector<int>&)>> algorithms = {
        {"Insertion Sort", insertionSort},
        {"Selection Sort", selectionSort},
        {"Bubble Sort", bubbleSort},
        {"Merge Sort", mergeSort},
        {"Quick Sort", quickSort}
    };
    
    // Vetor para armazenar resultados
    std::vector<Result> results;
    
    // Executar testes
    for (int size : sizes) {
        for (const auto& caseType : caseTypes) {
            std::cout << "\nGerando array de tamanho " << size << " para caso " << caseType << "..." << std::endl;
            
            // Gerar array de teste
            std::vector<int> arr = generateTestArray(size, caseType);
            
            for (const auto& [algoName, algoFunc] : algorithms) {
                // Pular algoritmos O(n²) para entradas muito grandes
                if (size > 50000 && (algoName == "Insertion Sort" || 
                                      algoName == "Selection Sort" || 
                                      algoName == "Bubble Sort")) {
                    std::cout << "  Pulando " << algoName << " para n=" << size << " (muito lento)" << std::endl;
                    
                    Result result;
                    result.algorithm = algoName;
                    result.caseType = caseType;
                    result.size = size;
                    result.time = -1; // Indicar que não foi executado
                    
                    results.push_back(result);
                    continue;
                }
                
                std::cout << "  Executando " << algoName << " para n=" << size << ", caso=" << caseType << "..." << std::endl;
                
                // Testar algoritmo
                double executionTime = testAlgorithm(algoFunc, arr);
                
                std::cout << "    Tempo: " << std::fixed << std::setprecision(6) << executionTime << " segundos" << std::endl;
                
                // Armazenar resultado
                Result result;
                result.algorithm = algoName;
                result.caseType = caseType;
                result.size = size;
                result.time = executionTime;
                
                results.push_back(result);
            }
        }
    }
    
    // Gerar relatório
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    struct tm* time_info = localtime(&now_time);
    
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", time_info);
    
    std::string filename = "sorting_benchmark_cpp_" + std::string(timestamp) + ".txt";
    std::ofstream report(filename);
    
    if (report.is_open()) {
        report << "==========================================================" << std::endl;
        report << "RELATÓRIO DE ANÁLISE DE ALGORITMOS DE ORDENAÇÃO EM C++" << std::endl;
        report << "==========================================================" << std::endl << std::endl;
        
        report << "Configuração do Sistema:" << std::endl;
        report << "  Sistema: " << getSystemInfo() << std::endl;
        report << "  Linguagem: C++" << std::endl << std::endl;
        
        // Tabela de resultados para cada caso
        for (const auto& caseType : caseTypes) {
            report << "Caso: " << caseType << std::endl;
            report << "----------------------------------------------------------" << std::endl;
            
            // Cabeçalho da tabela
            report << std::left << std::setw(20) << "Algoritmo" << " | ";
            for (int size : sizes) {
                report << std::setw(12) << size << " | ";
            }
            report << std::endl;
            report << "----------------------------------------------------------" << std::endl;
            
            // Dados da tabela
            for (const auto& [algoName, _] : algorithms) {
                report << std::left << std::setw(20) << algoName << " | ";
                
                for (int size : sizes) {
                    // Encontrar o resultado correspondente
                    auto it = std::find_if(results.begin(), results.end(),
                        [&algoName, &caseType, size](const Result& r) {
                            return r.algorithm == algoName && r.caseType == caseType && r.size == size;
                        });
                    
                    if (it != results.end() && it->time >= 0) {
                        report << std::fixed << std::setprecision(6) << std::setw(12) << it->time << " | ";
                    } else {
                        report << std::setw(12) << "N/A" << " | ";
                    }
                }
                report << std::endl;
            }
            
            report << std::endl << std::endl;
        }
        
        // Conclusões
        report << "CONCLUSÕES" << std::endl;
        report << "----------------------------------------------------------" << std::endl;
        report << "1. Comparação de complexidade teórica vs. resultados empíricos:" << std::endl;
        report << "   - Insertion Sort, Selection Sort, Bubble Sort: O(n²) no pior caso" << std::endl;
        report << "   - Merge Sort: O(n log n) em todos os casos" << std::endl;
        report << "   - Quick Sort: O(n log n) no caso médio, O(n²) no pior caso" << std::endl << std::endl;
        
        report << "2. Observações sobre os resultados obtidos:" << std::endl;
        
        // Encontrar algoritmo mais rápido e mais lento
        auto validResults = std::vector<Result>();
        std::copy_if(results.begin(), results.end(), std::back_inserter(validResults),
            [](const Result& r) { return r.time >= 0; });
        
        if (!validResults.empty()) {
            auto fastestIt = std::min_element(validResults.begin(), validResults.end(),
                [](const Result& a, const Result& b) { return a.time < b.time; });
                
            auto slowestIt = std::max_element(validResults.begin(), validResults.end(),
                [](const Result& a, const Result& b) { return a.time < b.time; });
            
            report << "   - O algoritmo mais rápido foi " << fastestIt->algorithm 
                   << " para n=" << fastestIt->size << ", caso=" << fastestIt->caseType 
                   << ": " << std::fixed << std::setprecision(6) << fastestIt->time << " segundos" << std::endl;
            
            report << "   - O algoritmo mais lento foi " << slowestIt->algorithm 
                   << " para n=" << slowestIt->size << ", caso=" << slowestIt->caseType 
                   << ": " << std::fixed << std::setprecision(6) << slowestIt->time << " segundos" << std::endl;
        }
        
        report.close();
        std::cout << "\nRelatório gerado: " << filename << std::endl;
    } else {
        std::cerr << "Erro ao abrir arquivo para escrita de relatório!" << std::endl;
    }
    
    return 0;
}