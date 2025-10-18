#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Necessario para o requisito de tempo .

// Definicoes Globais
#define CAPACIDADE_MAXIMA 10

// Structs 

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Struct No: Elemento da Lista Encadeada
typedef struct No {
    Item dados;             // Armazena a struct Item
    struct No *proximo;     // Ponteiro para o proximo No (o encadeamento)
} No;


// VETOR
Item mochila_vetor[CAPACIDADE_MAXIMA];
int total_itens_vetor = 0;

// Lira encadeada
No *cabeca_lista = NULL; // Ponteiro para o inicio da lista

// contador
long long contador_comparacoes = 0;

// fucoes auxiliares
void limpar_buffer();
void limpar_nova_linha(char *str);
void imprimir_item(const Item *item, int slot);
void exibirMenuPrincipal();
void exibirMenuVetor();
void exibirMenuLista();

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

void imprimir_item(const Item *item, int slot) {
    printf(" %4d | %20s | %10s | %10d\n",
           slot,
           item->nome,
           item->tipo,
           item->quantidade);
}

void Vetor_inserirItem(Item novo_item) {
    if (total_itens_vetor >= CAPACIDADE_MAXIMA) {
        printf("\n[ATENCAO] Mochila (Vetor) cheia!\n");
        return;
    }
    mochila_vetor[total_itens_vetor] = novo_item;
    total_itens_vetor++;
    printf("\n[SUCESSO] Item '%s' adicionado ao Vetor.\n", novo_item.nome);
}

void Vetor_listarItens() {
    printf("\n--- INVENTARIO (Vetor - Total: %d/%d) ---\n", total_itens_vetor, CAPACIDADE_MAXIMA);
    if (total_itens_vetor == 0) {
        printf(" (Vazio)\n");
        return;
    }
    printf(" Slot | Nome                 | Tipo       | Quantidade\n");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < total_itens_vetor; i++) {
        imprimir_item(&mochila_vetor[i], i + 1);
    }
    printf("----------------------------------------------------------\n");
}

void Vetor_removerItem(const char *nome_remover) {
    int encontrado_indice = -1;
    
    // 1. Busca Sequencial
    for (int i = 0; i < total_itens_vetor; i++) {
        if (strcmp(mochila_vetor[i].nome, nome_remover) == 0) {
            encontrado_indice = i;
            break;
        }
    }

    if (encontrado_indice != -1) {
        // 2. Remocao e Deslocamento (Operacao custosa em vetores)
        for (int i = encontrado_indice; i < total_itens_vetor - 1; i++) {
            mochila_vetor[i] = mochila_vetor[i + 1];
        }
        total_itens_vetor--;
        printf("[SUCESSO] Item '%s' removido e vetor reorganizado.\n", nome_remover);
    } else {
        printf("[ALERTA] Item '%s' nao encontrado no Vetor.\n", nome_remover);
    }
}

// Ordenacao do vetor por nome (Bubble Sort)
void Vetor_ordenar() {
    if (total_itens_vetor <= 1) return;

    for (int i = 0; i < total_itens_vetor - 1; i++) {
        for (int j = 0; j < total_itens_vetor - i - 1; j++) {
            // Compara os nomes
            if (strcmp(mochila_vetor[j].nome, mochila_vetor[j + 1].nome) > 0) {
                // Troca (Swap) os elementos
                Item temp = mochila_vetor[j];
                mochila_vetor[j] = mochila_vetor[j + 1];
                mochila_vetor[j + 1] = temp;
            }
        }
    }
    printf("\n[SUCESSO] Vetor ordenado alfabeticamente por nome (Bubble Sort).\n");
    Vetor_listarItens();
}

// Busca Sequencial no Vetor
void Vetor_buscarSequencial(const char *nome_busca) {
    contador_comparacoes = 0;
    int encontrado = 0;
    
    for (int i = 0; i < total_itens_vetor; i++) {
        contador_comparacoes++; // Conta a comparacao
        if (strcmp(mochila_vetor[i].nome, nome_busca) == 0) {
            printf("\n[ENCONTRADO - SEQ] Slot %d:\n", i + 1);
            imprimir_item(&mochila_vetor[i], i + 1);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\n[INFO] Item '%s' nao encontrado no Vetor.\n", nome_busca);
    }
    printf(">>> Busca Sequencial - Total de Comparacoes: %lld <<<\n", contador_comparacoes);
}

// Busca Binaria no Vetor (Apos a ordenacao!)
void Vetor_buscarBinaria(const char *nome_busca) {
    if (total_itens_vetor == 0) {
        printf("\n[INFO] O vetor esta vazio.\n");
        return;
    }

    contador_comparacoes = 0;
    int esquerda = 0;
    int direita = total_itens_vetor - 1;
    int encontrado = 0;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        int resultado_comparacao = strcmp(mochila_vetor[meio].nome, nome_busca);
        contador_comparacoes++;

        if (resultado_comparacao == 0) {
            // Encontrado
            printf("\n[ENCONTRADO - BIN] Slot %d (posicao media apos a busca):\n", meio + 1);
            imprimir_item(&mochila_vetor[meio], meio + 1);
            encontrado = 1;
            break;
        } else if (resultado_comparacao < 0) {
            // O item buscado e maior (esta na metade direita)
            esquerda = meio + 1;
        } else {
            // O item buscado e menor (esta na metade esquerda)
            direita = meio - 1;
        }
    }

    if (!encontrado) {
        printf("\n[INFO] Item '%s' nao encontrado no Vetor.\n", nome_busca);
    }
    printf(">>> Busca Binaria - Total de Comparacoes: %lld <<<\n", contador_comparacoes);
}

void Lista_inserirItem(Item novo_item) {
    // Cria o novo No e aloca memoria
    No *novo_no = (No*)malloc(sizeof(No));
    if (novo_no == NULL) {
        printf("[ERRO] Falha na alocacao de memoria para a lista.\n");
        return;
    }
    
    novo_no->dados = novo_item;
    
    // Insercao no inicio.
    novo_no->proximo = cabeca_lista;
    cabeca_lista = novo_no;
    
    printf("\n[SUCESSO] Item '%s' adicionado ao INICIO da Lista Encadeada (O(1)).\n", novo_item.nome);
}

void Lista_listarItens() {
    No *atual = cabeca_lista;
    int slot = 1;

    printf("\n--- INVENTARIO (Lista Encadeada) ---\n");
    if (atual == NULL) {
        printf(" (Vazio)\n");
        return;
    }
    
    printf(" Slot | Nome                 | Tipo       | Quantidade\n");
    printf("----------------------------------------------------------\n");
    while (atual != NULL) {
        imprimir_item(&(atual->dados), slot);
        atual = atual->proximo; // Avanca para o proximo No
        slot++;
    }
    printf("----------------------------------------------------------\n");
}

void Lista_removerItem(const char *nome_remover) {
    No *atual = cabeca_lista;
    No *anterior = NULL;
    int encontrado = 0;

    // Busca o No a ser removido
    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nome_remover) == 0) {
            encontrado = 1;
            break;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    if (encontrado) {
        // Remocao (Apenas ajuste de ponteiros - O(1) se achado)
        if (anterior == NULL) {
            // Remover o primeiro No
            cabeca_lista = atual->proximo;
        } else {
            // Remover No do meio ou fim
            anterior->proximo = atual->proximo;
        }
        
        free(atual); // Libera a memoria do No removido
        printf("[SUCESSO] Item '%s' removido da Lista Encadeada (O(1) apos a busca).\n", nome_remover);
    } else {
        printf("[ALERTA] Item '%s' nao encontrado na Lista Encadeada.\n", nome_remover);
    }
}

// Busca Sequencial na Lista Encadeada
void Lista_buscarSequencial(const char *nome_busca) {
    No *atual = cabeca_lista;
    contador_comparacoes = 0;
    int encontrado = 0;
    int slot = 1;

    while (atual != NULL) {
        contador_comparacoes++; // Conta a comparacao
        if (strcmp(atual->dados.nome, nome_busca) == 0) {
            printf("\n[ENCONTRADO - SEQ] Slot %d:\n", slot);
            imprimir_item(&(atual->dados), slot);
            encontrado = 1;
            break;
        }
        atual = atual->proximo;
        slot++;
    }

    if (!encontrado) {
        printf("\n[INFO] Item '%s' nao encontrado na Lista Encadeada.\n", nome_busca);
    }
    printf(">>> Busca Sequencial - Total de Comparacoes: %lld <<<\n", contador_comparacoes);
}

// Funcao para leitura generica de um Item
Item ler_item() {
    Item novo_item;
    printf("Nome do Item (max 29): ");
    fgets(novo_item.nome, sizeof(novo_item.nome), stdin);
    limpar_nova_linha(novo_item.nome);
    printf("Tipo (arma, municao, cura): ");
    fgets(novo_item.tipo, sizeof(novo_item.tipo), stdin);
    limpar_nova_linha(novo_item.tipo);
    printf("Quantidade: ");
    if (scanf("%d", &novo_item.quantidade) != 1) {
        printf("[ERRO] Entrada invalida. Quantidade setada para 1.\n");
        novo_item.quantidade = 1;
    }
    limpar_buffer();
    return novo_item;
}

// Funcao para leitura do nome de busca/remocao
void ler_nome(char *buffer, const char *acao) {
    printf("Digite o nome do item para %s: ", acao);
    fgets(buffer, 30, stdin);
    limpar_nova_linha(buffer);
}


// Funcao de Controle de Menus
void controlar_mochila(int tipo_estrutura) {
    int opcao;
    char nome_buffer[30];

    do {
        if (tipo_estrutura == 1) exibirMenuVetor();
        else exibirMenuLista();
        
        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        if (tipo_estrutura == 1) { // Vetor
            switch (opcao) {
                case 1: Vetor_inserirItem(ler_item()); break;
                case 2: ler_nome(nome_buffer, "remover"); Vetor_removerItem(nome_buffer); break;
                case 3: Vetor_listarItens(); break;
                case 4: Vetor_ordenar(); break;
                case 5: ler_nome(nome_buffer, "busca Sequencial"); Vetor_buscarSequencial(nome_buffer); break;
                case 6: ler_nome(nome_buffer, "busca Binaria"); Vetor_buscarBinaria(nome_buffer); break;
                case 0: printf("\nVoltando ao menu principal.\n"); break;
                default: printf("\nOpcao invalida. Tente novamente.\n");
            }
        } else { // Lista Encadeada
            switch (opcao) {
                case 1: Lista_inserirItem(ler_item()); break;
                case 2: ler_nome(nome_buffer, "remover"); Lista_removerItem(nome_buffer); break;
                case 3: Lista_listarItens(); break;
                case 4: ler_nome(nome_buffer, "busca Sequencial"); Lista_buscarSequencial(nome_buffer); break;
                case 0: printf("\nVoltando ao menu principal.\n"); break;
                default: printf("\nOpcao invalida. Tente novamente.\n");
            }
        }
    } while (opcao != 0);
}

void exibirMenuPrincipal() {
    printf("\n\n======== SELECAO DA ESTRUTURA ========\n");
    printf("| [1] Mochila com Vetor (Lista Sequencial)     |\n");
    printf("| [2] Mochila com Lista Encadeada (Dinamica)   |\n");
    printf("| [0] Sair do Programa                         |\n");
    printf("==============================================\n");
    printf("Escolha a estrutura: ");
}

void exibirMenuVetor() {
    printf("\n--- MOCHILA: VETOR (SEQUENCIAL) ---\n");
    printf("[1] Inserir Item (Adicionar ao final)\n");
    printf("[2] Remover Item por Nome (O(n))\n");
    printf("[3] Listar Itens\n");
    printf("[4] Ordenar Itens por Nome (REQUISITO PARA BUSCA BINARIA)\n");
    printf("[5] Buscar Sequencial (O(n))\n");
    printf("[6] Buscar Binaria (O(log n) - Apos Ordenacao)\n");
    printf("[0] Voltar\n");
    printf("Opcao: ");
}

void exibirMenuLista() {
    printf("\n--- MOCHILA: LISTA ENCADEADA ---\n");
    printf("[1] Inserir Item (Adicionar no inicio - O(1))\n");
    printf("[2] Remover Item por Nome (O(n) na busca)\n");
    printf("[3] Listar Itens\n");
    printf("[4] Buscar Sequencial (O(n))\n");
    printf("[0] Voltar\n");
    printf("Opcao: ");
}

int main() {
    int escolha_estrutura;
    
    printf(">>> Simulacao de Inventario (Comparacao de Estruturas) <<<\n");
    
    // Seed para rand, embora nao usado aqui, e uma boa pratica.
    srand(time(NULL)); 

    do {
        exibirMenuPrincipal();
        
        if (scanf("%d", &escolha_estrutura) != 1) {
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        if (escolha_estrutura == 1 || escolha_estrutura == 2) {
            controlar_mochila(escolha_estrutura);
        } else if (escolha_estrutura != 0) {
            printf("\nEscolha invalida. Tente novamente.\n");
        }
        
    } while (escolha_estrutura != 0);

    // A lista encadeada (cabeca_lista) deve ser liberada.
    printf("\n[FIM] Liberando memoria dinamica da Lista Encadeada...\n");
    No *atual = cabeca_lista;
    No *proximo_no;
    while (atual != NULL) {
        proximo_no = atual->proximo;
        free(atual); // Libera o No atual
        atual = proximo_no;
    }
    cabeca_lista = NULL; // Garante  que o ponteiro nao e mais um dangling pointer
    printf("Memoria dinamica liberada. Programa encerrado.\n");
    
    return 0;
}
