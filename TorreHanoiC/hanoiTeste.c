#include <stdio.h>
#include <stdlib.h>

#define FACIL 3
#define MEDIO 5
#define DIFICIL 7

// Estrutura de uma pilha
typedef struct Pilha {
    int *dados;
    int topo;
    int capacidade;
} Pilha;

// Função para criar uma pilha
Pilha* criarPilha(int capacidade) {
    Pilha *pilha = (Pilha*)malloc(sizeof(Pilha));
    pilha->capacidade = capacidade;
    pilha->topo = -1;
    pilha->dados = (int*)malloc(capacidade * sizeof(int));
    return pilha;
}

int isEmpty(Pilha *pilha) {
    return pilha->topo == -1;
}

int isFull(Pilha *pilha) {
    return pilha->topo == pilha->capacidade - 1;
}

void push(Pilha *pilha, int disco) {
    if (!isFull(pilha)) {
        pilha->dados[++pilha->topo] = disco;
    }
}

int pop(Pilha *pilha) {
    if (!isEmpty(pilha)) {
        return pilha->dados[pilha->topo--];
    }
    return -1;
}

int topo(Pilha *pilha) {
    if (!isEmpty(pilha)) {
        return pilha->dados[pilha->topo];
    }
    return -1;
}

void imprimirPilhas(Pilha *pilhaA, Pilha *pilhaB, Pilha *pilhaC) {
    printf("Pilha A: ");
    for (int i = 0; i <= pilhaA->topo; i++) printf("%d ", pilhaA->dados[i]);
    printf("\nPilha B: ");
    for (int i = 0; i <= pilhaB->topo; i++) printf("%d ", pilhaB->dados[i]);
    printf("\nPilha C: ");
    for (int i = 0; i <= pilhaC->topo; i++) printf("%d ", pilhaC->dados[i]);
    printf("\n\n");
}

int movimentoValido(Pilha *origem, Pilha *destino) {
    if (isEmpty(origem)) return 0;
    if (isEmpty(destino)) return 1;
    if (topo(origem) < topo(destino)) return 1;
    return 0;
}

int main() {
    int dificuldade;
    int numDiscos;

    printf("Escolha a dificuldade do jogo:\n");
    printf("1. Fácil (3 discos)\n");
    printf("2. Médio (5 discos)\n");
    printf("3. Difícil (7 discos)\n");
    printf("Digite a opção (1-3): ");
    scanf("%d", &dificuldade);

    switch (dificuldade) {
        case 1: numDiscos = FACIL; break;
        case 2: numDiscos = MEDIO; break;
        case 3: numDiscos = DIFICIL; break;
        default:
            printf("Opção inválida! Usando o nível Fácil.\n");
            numDiscos = FACIL;
    }

    Pilha *pilhaA = criarPilha(numDiscos);
    Pilha *pilhaB = criarPilha(numDiscos);
    Pilha *pilhaC = criarPilha(numDiscos);

    for (int i = numDiscos; i > 0; i--) {
        push(pilhaA, i);
    }

    printf("Estado inicial das pilhas:\n");
    imprimirPilhas(pilhaA, pilhaB, pilhaC);

    char origem, destino;
    while (1) {
        printf("Digite de qual pilha você deseja mover o disco (A, B, C): ");
        scanf(" %c", &origem);
        printf("Digite para qual pilha você deseja mover o disco (A, B, C): ");
        scanf(" %c", &destino);

        Pilha *pilhaOrigem, *pilhaDestino;

        if (origem == 'A') pilhaOrigem = pilhaA;
        else if (origem == 'B') pilhaOrigem = pilhaB;
        else if (origem == 'C') pilhaOrigem = pilhaC;

        if (destino == 'A') pilhaDestino = pilhaA;
        else if (destino == 'B') pilhaDestino = pilhaB;
        else if (destino == 'C') pilhaDestino = pilhaC;

        if (movimentoValido(pilhaOrigem, pilhaDestino)) {
            int disco = pop(pilhaOrigem);
            push(pilhaDestino, disco);
            printf("Movimento realizado de %c para %c:\n", origem, destino);
            imprimirPilhas(pilhaA, pilhaB, pilhaC);
        } else {
            printf("Movimento inválido! Tente novamente.\n");
        }

        if (pilhaC->topo == numDiscos - 1) {
            printf("Você completou a Torre de Hanoi! Parabéns!\n");
            break;
        }
    }

    free(pilhaA->dados); free(pilhaA);
    free(pilhaB->dados); free(pilhaB);
    free(pilhaC->dados); free(pilhaC);

    return 0;
}
