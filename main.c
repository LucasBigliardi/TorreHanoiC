#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>

#define FACIL 3
#define MEDIO 5
#define DIFICIL 7

#define MAX 10

typedef struct {
    int dados[MAX];
    int topo;
} Pilha;

int isEmpty(Pilha *p);
int isFull(Pilha *p);
int top(Pilha *p);
int push(Pilha *p, int dado);
int pop(Pilha *p);
void printPilha(Pilha *p);
int telaDificuldade();
void telaMovimento(int numDiscos, int movimentos, int segundos, Pilha *p1, Pilha *p2, Pilha *p3);
void limparTela();
void clearPilha(Pilha *p);

void main() {

    // encoding padrão: Windows-1252
    setlocale(LC_ALL, "Portuguese");

    Pilha p1, p2, p3;
    // iniciando em -1 para indicar pilha vazia
    p1.topo = -1;
    p2.topo = -1;
    p3.topo = -1;

    int discos = telaDificuldade();

    if (discos == 0) {
        limparTela();
        printf("Opção inválida: Tente novamente");
        return;
    }

    // preenchendo a primeira pilha com os discos
    for (int i = discos; i > 0; i--) {
        push(&p1, i);
    }

    int jogarNovamente = 1;
    int novoJogo = 0;
    int movimentos = 0;
    // varialveis para o timer
    time_t inicio = time(NULL);
    int segundos = 0;

    // loop principal, toda lógica a partir do inicio
    // passa por aqui, e não volta mais ao escopo da main
    // até o fim do programa
    while (jogarNovamente) {

        // checa se é um novo jogo após uma vitória e reseta
        // os dados da partida anterior
        if (novoJogo) {
            clearPilha(&p1);
            clearPilha(&p2);
            clearPilha(&p3);
            for (int i = discos; i > 0; i--) {
                push(&p1, i);
            }
            movimentos = 0;
            inicio = time(NULL);
            novoJogo = 0;
        }

        // variaveis que receberão movimentos e pilhas equivalentes
        int mov1 = 0;
        int mov2 = 0;
        Pilha *origem = NULL;
        Pilha *destino = NULL;

        // alterando o tempo em base da diferença entre
        // tempo de inicio do jogo e o tempo passado
        segundos = (int)difftime(time(NULL), inicio);

        // interface visual do hanoi
        telaMovimento(discos, movimentos, segundos, &p1, &p2, &p3);

        printf("\nEscolha a pilha de origem (1, 2, 3): ");
        scanf("%d", &mov1);
        printf("Escolha a pilha de destino (1, 2, 3): ");
        scanf("%d", &mov2);

        // validar entrada
        if (mov1 < 1 || mov1 > 3 || mov2 < 1 || mov2 > 3 || mov1 == mov2) {
            printf("\nMovimento inválido! Pressione Enter para continuar...");
            // serve para clicar enter e continuar
            getchar(); getchar();
            continue;
        }

        // mapear pilhas em base da jogada
        switch(mov1) {
            case 1: origem = &p1; break;
            case 2: origem = &p2; break;
            case 3: origem = &p3; break;
        }
        switch(mov2) {
            case 1: destino = &p1; break;
            case 2: destino = &p2; break;
            case 3: destino = &p3; break;
        }

        // verificar se origem está vazia
        if (isEmpty(origem)) {
            printf("\nPilha de origem vazia! Pressione Enter para continuar...");
            getchar(); getchar();
            continue;
        }

        // verificar se o disco maior está acima do menor
        if (!isEmpty(destino) && top(origem) > top(destino)) {
            printf("\nMovimento inválido: disco maior sobre menor! Pressione Enter para continuar...");
            getchar(); getchar();
            continue;
        }

        // fazer o movimento
        int disco = pop(origem);
        push(destino, disco);
        movimentos++;

        // verificar vitória (todos os discos na última pilha)
        if (p3.topo == discos - 1) {
            limparTela();
            telaMovimento(discos, movimentos, segundos, &p1, &p2, &p3);
            printf("\nParabéns! Você venceu em %d movimentos!\n", movimentos);
            printf("Tempo total: %02d:%02d\n", segundos / 60, segundos % 60);

            char opcao;
            do {
                printf("\nDeseja jogar novamente? (S/N): ");
                scanf(" %c", &opcao);
                // lowercase de S e N para s e n
                opcao = tolower(opcao);
            } while (opcao != 's' && opcao != 'n');

            if (opcao == 's') {
                jogarNovamente = 1;
                novoJogo = 1;
            } else {
                jogarNovamente = 0;
                novoJogo = 0;

                printf("Pressione Enter para sair...");
                getchar(); getchar();
            }
        }
    }
}

// funcoes do hanoi | interface visual
int telaDificuldade() {

    int opcao;

    printf("||| TORRE DE HANOI |||\n\n");
    printf("Selecione a dificuldade:\n\n");
    printf("1. Fácil (3 torres):\n");
    printf("2. Médio (5 torres):\n");
    printf("3. Díficil (7 torres):\n");

    printf("\n");
    
    scanf("%i", &opcao);

    switch (opcao) {
        case 1:
            return FACIL;
        case 2:
            return MEDIO;
        case 3:
            return DIFICIL;
        default:
            return 0;
    }
}

void telaMovimento(int numDiscos, int movimentos, int segundos, Pilha *p1, Pilha *p2, Pilha *p3) {
    limparTela();

    printf("\t\t\t\t\t ||| TORRE DE HANOI |||\t\n\n");
    printf("\t\t\t\t\t     Movimentos: %d  |  Tempo: %02d:%02d\t\n\n", movimentos, segundos / 60, segundos % 60);

    // loop do topo das torres
    for (int i = 0; i < 3; i++) {
        printf("            |            \t\t");
    }
    printf("\n");

    Pilha *pilhas[] = {p1, p2, p3};

    // loop da quantidade de pratos (de cima para baixo)
    for (int i = numDiscos - 1; i >= 0; i--) {
        // loop para desenhar as fileiras
        for (int j = 0; j <= 2; j++) {
            // calcula posição do disco na pilha começando da base
            int pos = i;
            if (pos <= pilhas[j]->topo) {
                switch (pilhas[j]->dados[pos]) {
                    case 1:
                        printf("           ===           \t\t");
                        break;
                    case 2:
                        printf("          =====          \t\t");
                        break;
                    case 3:
                        printf("        =========        \t\t");
                        break;
                    case 4:
                        printf("      =============      \t\t");
                        break;
                    case 5:
                        printf("    =================    \t\t");
                        break;
                    case 6:
                        printf("  =====================  \t\t");
                        break;
                    case 7:
                        printf("=========================\t\t");
                        break;
                    default:
                        printf("            |            \t\t");
                        break;
                }
            } else {
                printf("            |            \t\t");
            }
        }
        printf("\n");
    }

    // loop para desenhar a base
    for (int i = 0; i < 3; i++) {
        printf("[-----------------------]\t\t");
    }
    printf("\n");
}

void limparTela() {
    system("cls");
}

// funcoes da pilha | logica interna

int isEmpty(Pilha *p) {
    return p->topo == -1 ? 1 : 0;
}

int isFull(Pilha *p) {
    return p->topo == MAX - 1 ? 1 : 0;
}

int push(Pilha *p, int valor) {
    if (isFull(p)) {
        printf("Overload\n");
        return 0;
    }

    p->topo++;
    p->dados[p->topo] = valor;
    return 1;
}

int pop(Pilha *p) {
    if (isEmpty(p)) {
        printf("Underload\n");
    }
    int valor = p->dados[p->topo];
    p->topo--;
    return valor;
}

int top(Pilha *p) {
    return p->dados[p->topo];
}

void clearPilha(Pilha *p) {
    p->topo = -1;
}

void printPilha(Pilha *p) {
    if (isEmpty(p)) {
        printf("Underload\n");
    }
    for (int i = p->topo; i > 0; i--) {
        printf("\n%i", p->dados[i]);
    }
    printf("\n");
}
