#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

//Definicoes Globais
#define CAPACIDADE_MAXIMA 20
#define CHAVE_ATIVACAO "Chip Central" // Componente-chave para a Busca Binaria


// Componente: Estrutura de dados basica para o item da torre
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade; // De 1 (mais baixa) a 10 (mais alta)
} Componente;

// --- Variaveis Globais (Representam o Inventario de Componentes) ---
Componente torre_componentes[CAPACIDADE_MAXIMA]; // O vetor estatico de structs
int total_componentes = 0;                     // Contador de itens
long long contador_comparacoes = 0;            // Contador global de desempenho

// --- Protótipos das Funções Auxiliares ---
void limpar_buffer();
void limpar_nova_linha(char *str);
void mostrarComponentes(Componente componentes[], int tamanho);

// --- Protótipos dos Algoritmos de Ordenação ---
void bubbleSortNome(Componente componentes[], int tamanho);
void insertionSortTipo(Componente componentes[], int tamanho);
void selectionSortPrioridade(Componente componentes[], int tamanho);
void buscaBinariaPorNome(Componente componentes[], int tamanho, const char *chave_busca);

// --- Funções Auxiliares de E/S e Utilitários ---

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void limpar_nova_linha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Funcao para exibir os dados dos componentes de forma formatada
void mostrarComponentes(Componente componentes[], int tamanho) {
    printf("\n--- Componentes da Torre (Total: %d) ---\n", tamanho);
    if (tamanho == 0) {
        printf(" (Inventario Vazio)\n");
        return;
    }
    printf(" Indice | Prioridade | Tipo                 | Nome\n");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < tamanho; i++) {
        printf(" %6d | %10d | %20s | %s\n",
               i,
               componentes[i].prioridade,
               componentes[i].tipo,
               componentes[i].nome);
    }
    printf("----------------------------------------------------------\n");
}

// --- Funcao de Cadastro ---

void cadastrarComponentes() {
    int num_cadastrar;
    
    printf("\n--- CADASTRO INICIAL ---\n");
    printf("Quantos componentes serao cadastrados (Max %d)? ", CAPACIDADE_MAXIMA);
    if (scanf("%d", &num_cadastrar) != 1 || num_cadastrar <= 0 || num_cadastrar > CAPACIDADE_MAXIMA) {
        printf("[ERRO] Numero de componentes invalido. Usando 0.\n");
        num_cadastrar = 0;
    }
    limpar_buffer();
    
    total_componentes = num_cadastrar; // Define o tamanho ativo do vetor
    
    // Cadastro dos dados
    for (int i = 0; i < total_componentes; i++) {
        printf("\nComponente %d:\n", i + 1);
        
        printf("Nome (ex: Capacitor): ");
        fgets(torre_componentes[i].nome, sizeof(torre_componentes[i].nome), stdin);
        limpar_nova_linha(torre_componentes[i].nome);
        
        printf("Tipo (ex: Suporte, Controle): ");
        fgets(torre_componentes[i].tipo, sizeof(torre_componentes[i].tipo), stdin);
        limpar_nova_linha(torre_componentes[i].tipo);
        
        printf("Prioridade (1-10): ");
        if (scanf("%d", &torre_componentes[i].prioridade) != 1) {
             torre_componentes[i].prioridade = 1; // Default
             printf("[ALERTA] Prioridade invalida. Definida para 1.\n");
        }
        limpar_buffer();
    }
    printf("\n[SUCESSO] %d componentes cadastrados.\n", total_componentes);
}


// =================================================================
// ALGORITMOS DE ORDENAÇÃO (com Contagem e Tempo)
// =================================================================

// Bubble Sort: Ordena por NOME (string)
void bubbleSortNome(Componente componentes[], int tamanho) {
    if (tamanho <= 1) return;
    
    contador_comparacoes = 0;
    clock_t inicio = clock();
    
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            // Comparacao de Strings: strcmp
            contador_comparacoes++;
            if (strcmp(componentes[j].nome, componentes[j + 1].nome) > 0) {
                // Troca (Swap)
                Componente temp = componentes[j];
                componentes[j] = componentes[j + 1];
                componentes[j + 1] = temp;
            }
        }
    }
    
    clock_t fim = clock();
    double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\n--- ORDENACAO CONCLUIDA (Bubble Sort por Nome) ---\n");
    printf("Tempo de Execucao: %.6f segundos\n", tempo_gasto);
    printf("Total de Comparacoes: %lld\n", contador_comparacoes);
    mostrarComponentes(componentes, tamanho);
}


// Insertion Sort: Ordena por TIPO (string)
void insertionSortTipo(Componente componentes[], int tamanho) {
    if (tamanho <= 1) return;
    
    contador_comparacoes = 0;
    clock_t inicio = clock();

    for (int i = 1; i < tamanho; i++) {
        Componente chave = componentes[i];
        int j = i - 1;
        
        // Move os elementos maiores que a chave para a direita
        while (j >= 0 && (contador_comparacoes++, strcmp(componentes[j].tipo, chave.tipo) > 0)) {
            componentes[j + 1] = componentes[j];
            j--;
        }
        componentes[j + 1] = chave;
    }
    
    clock_t fim = clock();
    double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    
    printf("\n--- ORDENACAO CONCLUIDA (Insertion Sort por Tipo) ---\n");
    printf("Tempo de Execucao: %.6f segundos\n", tempo_gasto);
    printf("Total de Comparacoes: %lld\n", contador_comparacoes);
    mostrarComponentes(componentes, tamanho);
}


// Selection Sort: Ordena por PRIORIDADE (int)
void selectionSortPrioridade(Componente componentes[], int tamanho) {
    if (tamanho <= 1) return;
    
    contador_comparacoes = 0;
    clock_t inicio = clock();
    
    for (int i = 0; i < tamanho - 1; i++) {
        int indice_minimo = i;
        
        // Encontra o componente com a MAIOR prioridade (maior valor de int)
        for (int j = i + 1; j < tamanho; j++) {
            contador_comparacoes++;
            // Note o ">" para ordenar em ordem decrescente de prioridade (10 no topo)
            if (componentes[j].prioridade > componentes[indice_minimo].prioridade) {
                indice_minimo = j;
            }
        }
        
        // Troca o componente mais prioritario encontrado com o elemento na posicao 'i'
        Componente temp = componentes[indice_minimo];
        componentes[indice_minimo] = componentes[i];
        componentes[i] = temp;
    }
    
    clock_t fim = clock();
    double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\n--- ORDENACAO CONCLUIDA (Selection Sort por Prioridade) ---\n");
    printf("Tempo de Execucao: %.6f segundos\n", tempo_gasto);
    printf("Total de Comparacoes: %lld\n", contador_comparacoes);
    mostrarComponentes(componentes, tamanho);
}


// --- Funcao de Busca ---

// Busca Binaria: Localiza a chave de ativacao (funciona SOMENTE se ordenado por NOME)
void buscaBinariaPorNome(Componente componentes[], int tamanho, const char *chave_busca) {
    if (tamanho == 0) return;
    
    printf("\n--- BUSCA BINARIA: CHAVE DE ATIVACAO (%s) ---\n", chave_busca);

    contador_comparacoes = 0;
    int esquerda = 0;
    int direita = tamanho - 1;
    int encontrado = 0;
    
    clock_t inicio = clock();

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        int resultado_comparacao = strcmp(componentes[meio].nome, chave_busca);
        contador_comparacoes++; // Conta a comparacao da string

        if (resultado_comparacao == 0) {
            // Encontrado!
            printf("[ATIVACAO CONCLUIDA] Chave '%s' encontrada no indice %d.\n", chave_busca, meio);
            printf("Prioridade: %d, Tipo: %s.\n", componentes[meio].prioridade, componentes[meio].tipo);
            encontrado = 1;
            break;
        } else if (resultado_comparacao < 0) {
            // O item buscado e 'maior' (vem depois alfabeticamente)
            esquerda = meio + 1;
        } else {
            // O item buscado e 'menor' (vem antes alfabeticamente)
            direita = meio - 1;
        }
    }
    
    clock_t fim = clock();
    double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

    if (!encontrado) {
        printf("[ERRO CRITICO] A chave de ativacao '%s' nao foi encontrada no inventario.\n", chave_busca);
    }
    
    printf(">>> Busca Binaria - Tempo: %.6f segundos | Comparacoes: %lld <<<\n", tempo_gasto, contador_comparacoes);
}



void exibirMenu() {
    printf("\n\n=============== ESTRATEGIA FINAL ===============\n");
    printf("| 1 - Cadastrar Componentes Iniciais             |\n");
    printf("| 2 - Ordenar por NOME (Bubble Sort)             |\n");
    printf("| 3 - Ordenar por TIPO (Insertion Sort)          |\n");
    printf("| 4 - Ordenar por PRIORIDADE (Selection Sort)    |\n");
    printf("| 5 - Buscar CHAVE DE ATIVACAO (Busca Binaria)   |\n");
    printf("| 0 - Sair                                       |\n");
    printf("==================================================\n");
    printf("Escolha uma opcao: ");
}

int main() {
    int opcao;

    printf(">>> MODO ESTRATEGIA: Montagem da Torre de Fuga <<<\n");
    
    do {
        exibirMenu();
        
        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Opcao invalida. Tente novamente.\n");
            limpar_buffer();
            opcao = -1; 
            continue;
        }
        limpar_buffer();
        
        if (total_componentes == 0 && opcao > 1) {
            printf("\n[ALERTA] Cadastre os componentes (Opcao 1) antes de ordenar ou buscar.\n");
            continue;
        }

        switch (opcao) {
            case 1:
                cadastrarComponentes();
                mostrarComponentes(torre_componentes, total_componentes);
                break;
            case 2:
                bubbleSortNome(torre_componentes, total_componentes);
                break;
            case 3:
                insertionSortTipo(torre_componentes, total_componentes);
                break;
            case 4:
                selectionSortPrioridade(torre_componentes, total_componentes);
                break;
            case 5:
                buscaBinariaPorNome(torre_componentes, total_componentes, CHAVE_ATIVACAO);
                break;
            case 0:
                printf("\n[FIM] Plano de Fuga Encerrado. Boa sorte na proxima rodada!\n");
                break;
            default:
                printf("\n[ALERTA] Opcao desconhecida.\n");
        }
        
    } while (opcao != 0);

    return 0;
}
