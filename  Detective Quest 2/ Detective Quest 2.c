#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------------------
// Estruturas de dados
// -------------------------------

// Estrutura de uma sala (nó da árvore da mansão)
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura de um nó de pista (para a BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// -------------------------------
// Funções da árvore da mansão
// -------------------------------

// Cria uma sala dinamicamente
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// -------------------------------
// Funções da árvore de pistas (BST)
// -------------------------------

// Cria um novo nó de pista
PistaNode* criarPistaNode(const char *pista) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (novo == NULL) {
        printf("Erro ao alocar memória para a pista!\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Insere uma pista na árvore BST em ordem alfabética
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }

    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }

    return raiz;
}

// Exibe as pistas em ordem alfabética (in-order traversal)
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// -------------------------------
// Função principal de exploração
// -------------------------------

void explorarSalasComPistas(Sala *atual, PistaNode **arvorePistas) {
    char escolha;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Verifica se há pista no cômodo
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        // Mostra opções de navegação
        printf("Escolha seu próximo passo:\n");
        printf("  (e) Ir para a esquerda\n");
        printf("  (d) Ir para a direita\n");
        printf("  (s) Sair da mansão\n");
        printf("-> ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (escolha == 's') {
            printf("\nVocê decidiu encerrar a investigação.\n");
            break;
        } else {
            printf("Caminho inválido! Tente novamente.\n");
        }
    }
}

// -------------------------------
// Função principal
// -------------------------------

int main() {
    // Montagem fixa da mansão (árvore binária)
    Sala *hall = criarSala("Hall de Entrada", "Pegada de sapato molhado");
    Sala *salaEstar = criarSala("Sala de Estar", "Relógio parado às 3h");
    Sala *cozinha = criarSala("Cozinha", "Faca com resquícios de tinta");
    Sala *biblioteca = criarSala("Biblioteca", "Livro antigo com página rasgada");
    Sala *jardim = criarSala("Jardim", "");
    Sala *quarto = criarSala("Quarto Principal", "Carta rasgada sobre a cama");

    // Estrutura da mansão
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = quarto;

    // Criação da árvore de pistas (inicialmente vazia)
    PistaNode *arvorePistas = NULL;

    printf("Bem-vindo(a) ao Detective Quest – Coleta de Pistas!\n");
    explorarSalasComPistas(hall, &arvorePistas);

    // Exibe as pistas coletadas
    printf("\n=== PISTAS COLETADAS ===\n");
    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(arvorePistas);
    }

    // Libera memória (básico)
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(quarto);

    // Fim do jogo
    printf("\nFim da investigação. Até a próxima, detetive!\n");
    return 0;
}
