#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. Criacao da struct Item: Define a estrutura de dados para cada item no inventario.
typedef struct {
    char nome[30];      // Nome do item (ex: Faca, Municao Cal. 12)
    char tipo[20];      // Tipo do item (ex: arma, municao, cura, ferramenta)
    int quantidade;     // Quantidade em estoque
} Item;

// Constante para a capacidade maxima da mochila
#define CAPACIDADE_MAXIMA 10

// Variaveis Globais 

Item mochila[CAPACIDADE_MAXIMA]; // O vetor de structs (a lista sequencial de itens)
int total_itens = 0;             // Contador para rastrear quantos slots estao preenchidos

void limpar_buffer();
void limpar_nova_linha(char *str);
void inserirItem();
void removerItem();
void listarItens();
void buscarItem();
void exibirMenu();

// Limpa o buffer de entrada (stdin) apos o uso de scanf para inteiros
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Remove o '\n' (nova linha) que o fgets() adiciona ao final da string
void limpar_nova_linha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Funcao para cadastrar um novo item no vetor de structs (Lista Sequencial)
void inserirItem() {
    if (total_itens >= CAPACIDADE_MAXIMA) {
        printf("\n[ATENCAO] Mochila cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    printf("\n--- CADASTRAR NOVO ITEM ---\n");
    Item novo_item;

    // Leitura do Nome (usando fgets para strings com espacos)
    printf("Nome do Item (max 29 caracteres): ");
    fgets(novo_item.nome, sizeof(novo_item.nome), stdin);
    limpar_nova_linha(novo_item.nome);

    // Leitura do Tipo
    printf("Tipo (arma, municao, cura, ferramenta): ");
    fgets(novo_item.tipo, sizeof(novo_item.tipo), stdin);
    limpar_nova_linha(novo_item.tipo);

    // Leitura da Quantidade (usando scanf para inteiro)
    printf("Quantidade: ");
    if (scanf("%d", &novo_item.quantidade) != 1) {
        printf("[ERRO] Entrada invalida para quantidade.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer(); // Limpa o buffer apos o scanf

    // Adiciona a nova struct a primeira posicao vazia do vetor (Lista Sequencial)
    mochila[total_itens] = novo_item;
    total_itens++;

    printf("\n[SUCESSO] Item '%s' adicionado a mochila.\n", novo_item.nome);
    listarItens(); // Lista os itens apos a operacao
}

// Funcao para remover um item da mochila (Busca e Deslocamento)
void removerItem() {
    if (total_itens == 0) {
        printf("\n[ALERTA] A mochila esta vazia. Nada a remover.\n");
        return;
    }

    char nome_remover[30];
    printf("\n--- REMOVER ITEM ---\n");
    printf("Digite o nome do item a ser removido: ");
    
    // Leitura do nome a ser removido (usando fgets e limpeza)
    fgets(nome_remover, sizeof(nome_remover), stdin);
    limpar_nova_linha(nome_remover);
    
    int encontrado_indice = -1;
    
    // 1. Busca Linear para encontrar o item
    for (int i = 0; i < total_itens; i++) {
        // strcmp retorna 0 se as strings forem iguais
        if (strcmp(mochila[i].nome, nome_remover) == 0) {
            encontrado_indice = i;
            break; // Item encontrado, para a busca
        }
    }

    if (encontrado_indice != -1) {
        // 2. Remocao e Deslocamento (Garantindo a sequencia da lista)
        printf("[SUCESSO] Item '%s' encontrado no slot %d.\n", nome_remover, encontrado_indice + 1);

        // Desloca todos os elementos subsequentes uma posicao para a esquerda
        for (int i = encontrado_indice; i < total_itens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }

        total_itens--; // Diminui o contador de itens
        printf("Item removido e mochila reorganizada.\n");
    } else {
        printf("[ALERTA] Item '%s' nao encontrado na mochila.\n", nome_remover);
    }
    
    listarItens(); // Lista os itens apos a operacao
}

// Funcao para exibir todos os itens na lista sequencial
void listarItens() {
    printf("\n--- INVENTARIO ATUAL (Total: %d/%d) ---\n", total_itens, CAPACIDADE_MAXIMA);
    
    if (total_itens == 0) {
        printf("Slot 01 | (Vazio)\n");
        printf("---------------------------------------------------\n");
        return;
    }

    printf(" Slot | Nome (Tipo)          | Quantidade\n");
    printf("---------------------------------------------------\n");
    
    for (int i = 0; i < total_itens; i++) {
        // Imprime os dados de cada struct
        printf(" %4d | %20s (%-5s) | %10d\n",
               i + 1,
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade);
    }
    printf("---------------------------------------------------\n");
}

// Funcao de busca sequencial: localiza e exibe um item pelo nome
void buscarItem() {
    if (total_itens == 0) {
        printf("\n[ALERTA] A mochila esta vazia. Impossivel buscar.\n");
        return;
    }

    char nome_busca[30];
    printf("\n--- BUSCA SEQUENCIAL ---\n");
    printf("Digite o nome do item que deseja buscar: ");
    
    // Leitura do nome a ser buscado
    fgets(nome_busca, sizeof(nome_busca), stdin);
    limpar_nova_linha(nome_busca);

    int encontrado = 0;
    
    // Busca Linear.
    for (int i = 0; i < total_itens; i++) {
        // Compara o nome buscado com o nome do item no slot 'i'
        if (strcmp(mochila[i].nome, nome_busca) == 0) {
            printf("\n[ITEM ENCONTRADO no Slot %d]\n", i + 1);
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            encontrado = 1;
            break; // Encerra a busca ao encontrar o primeiro item
        }
    }

    if (!encontrado) {
        printf("\n[INFO] Item '%s' nao foi encontrado na mochila.\n", nome_busca);
    }
}

//Funcao para exibir as opcoes do usuario
void exibirMenu() {
    printf("\n\n=============== MOCHILA DO JOGADOR ===============\n");
    printf(" 1 - Cadastrar Novo Item (Inserir)              |\n");
    printf(" 2 - Remover Item pelo Nome                     |\n");
    printf(" 3 - Listar Todos os Itens                      |\n");
    printf(" 4 - Buscar Item pelo Nome (Busca Sequencial)   |\n");
    printf(" 0 - Sair do Jogo                               |\n");
    printf("==================================================\n");
    printf("Escolha uma opcao: ");
}

// --- Funcao Principal ---

int main() {
    int opcao;
    
    // Mensagem de boas vindas
    printf(">>> Iniciando Simulacao de Inventario (Mochila de Loot) <<<\n");
    
    // Loop principal do menu
    do {
        exibirMenu();
        
        if (scanf("%d", &opcao) != 1) {
            printf("\n ERRO: Opcao invalida. Tente novamente.\n");
            limpar_buffer();
            opcao = -1; // Forca a repeticao do loop
            continue;
        }
        limpar_buffer(); // Limpa o buffer após o scanf da opcao
        
        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                buscarItem();
                break;
            case 0:
                printf("\nFIM: Inventario fechado. Jogo encerrado.\n");
                break;
            default:
                printf("\nALERTA: Opcao desconhecida. Tente um numero de 0 a 4.\n");
        }
        
    } while (opcao != 0);

    return 0;
}
