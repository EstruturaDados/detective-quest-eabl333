#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// ---------------------------------------------
// Estruturas de dados
// ---------------------------------------------

// Nó da árvore binária de salas
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó da árvore BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Estrutura da tabela hash (pistas -> suspeitos)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *proximo;
} HashNode;

// ---------------------------------------------
// Funções auxiliares
// ---------------------------------------------

// Função hash simples baseada na soma dos caracteres
int funcaoHash(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAM_HASH;
}

// Cria uma nova sala
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (!novaSala) {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Cria um novo nó de pista
PistaNode* criarPistaNode(const char *pista) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (!novo) {
        printf("Erro ao alocar memória para pista!\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Insere pista na BST em ordem alfabética
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL)
        return criarPistaNode(pista);

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

// Exibe as pistas em ordem alfabética
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// Insere um par (pista -> suspeito) na tabela hash
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito) {
    int indice = funcaoHash(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabela[indice];
    tabela[indice] = novo;
}

// Busca o suspeito relacionado a uma pista
char* encontrarSuspeito(HashNode *tabela[], const char *pista) {
    int indice = funcaoHash(pista);
    HashNode *atual = tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->proximo;
    }
    return "Desconhecido";
}

// ---------------------------------------------
// Exploração da mansão
// ---------------------------------------------

void explorarSalas(Sala *atual, PistaNode **arvorePistas, HashNode *tabela[]) {
    char escolha;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Coleta da pista
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);

            // Mostra suspeito associado
            char *suspeito = encontrarSuspeito(tabela, atual->pista);
            printf("Essa pista está relacionada a: %s\n", suspeito);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        // Menu de navegação
        printf("\nEscolha seu próximo passo:\n");
        printf("  (e) Ir para a esquerda\n");
        printf("  (d) Ir para a direita\n");
        printf("  (s) Sair da mansão\n");
        printf("-> ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL)
            atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita != NULL)
            atual = atual->direita;
        else if (escolha == 's') {
            printf("\nVocê decidiu encerrar a investigação.\n");
            break;
        } else {
            printf("Caminho inválido!\n");
        }
    }
}

// ---------------------------------------------
// Julgamento final
// ---------------------------------------------

// Conta quantas pistas apontam para o suspeito acusado
int contarPistasDoSuspeito(PistaNode *raiz, HashNode *tabela[], const char *suspeito) {
    if (raiz == NULL)
        return 0;

    int contador = 0;
    char *s = encontrarSuspeito(tabela, raiz->pista);
    if (strcmp(s, suspeito) == 0)
        contador++;

    contador += contarPistasDoSuspeito(raiz->esquerda, tabela, suspeito);
    contador += contarPistasDoSuspeito(raiz->direita, tabela, suspeito);
    return contador;
}

void verificarSuspeitoFinal(PistaNode *arvorePistas, HashNode *tabela[]) {
    char acusado[50];
    printf("\n=== FASE FINAL ===\n");
    printf("Digite o nome do suspeito que você acredita ser o culpado: ");
    scanf(" %[^\n]", acusado);

    int qtd = contarPistasDoSuspeito(arvorePistas, tabela, acusado);
    printf("\nAnalisando suas pistas...\n");

    if (qtd >= 2) {
        printf("Correto! %s é realmente o culpado, com %d pistas associadas!\n", acusado, qtd);
    } else {
        printf("Incorreto! %s não tem pistas suficientes (%d encontradas).\n", acusado, qtd);
    }
}

// ---------------------------------------------
// Função principal (main)
// ---------------------------------------------

int main() {
    // Montagem da mansão
    Sala *hall = criarSala("Hall de Entrada", "Relógio quebrado");
    Sala *salaEstar = criarSala("Sala de Estar", "Copo de vinho derramado");
    Sala *cozinha = criarSala("Cozinha", "Faca suja");
    Sala *biblioteca = criarSala("Biblioteca", "Luvas rasgadas");
    Sala *jardim = criarSala("Jardim", "Pegada de terra fresca");
    Sala *quarto = criarSala("Quarto Principal", "Perfume caro");

    // Conexões da árvore da mansão
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = quarto;

    // Inicializa a BST de pistas e a tabela hash
    PistaNode *arvorePistas = NULL;
    HashNode *tabela[TAM_HASH] = {NULL};

    // Mapeamento de pistas -> suspeitos
    inserirNaHash(tabela, "Relógio quebrado", "Sr. Black");
    inserirNaHash(tabela, "Copo de vinho derramado", "Sra. Scarlet");
    inserirNaHash(tabela, "Faca suja", "Chef Pierre");
    inserirNaHash(tabela, "Luvas rasgadas", "Sr. Black");
    inserirNaHash(tabela, "Pegada de terra fresca", "Jardineiro Tom");
    inserirNaHash(tabela, "Perfume caro", "Sra. Scarlet");

    printf("Bem-vindo(a) ao capítulo final de Detective Quest!\n");
    explorarSalas(hall, &arvorePistas, tabela);

    // Exibe as pistas coletadas
    printf("\n=== PISTAS COLETADAS ===\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(arvorePistas);

    // Julgamento final
    verificarSuspeitoFinal(arvorePistas, tabela);

    printf("\nObrigado por jogar! Fim da investigação.\n");
    return 0;
}
