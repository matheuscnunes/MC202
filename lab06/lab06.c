#include <stdio.h>
#include <stdlib.h>

/*
 * Nomes: Matheus Cezar Nunes , Mateus Freitas Silveira
 * RA: 203373 e 203263
 *
 */

//Enuns
typedef enum Situacao { LIVRE, PARTICIONADO, OCUPADO } Situacao;
typedef enum Operacoes {
  INICIAR_PROCESSO = 1, FINALIZAR_PROCESSO, FRAGMENTACAO, RELATORIO,
  IMPRIME_SEMENTES, IMPRIME_PROCESSOS
} Operacoes;
typedef enum bool { false, true } bool;

//Structs
struct No {
	int tamanhoMemoria;
	int codigo;
	int tamanhoUtilizado;
	Situacao estado;
 	struct No *dir, *esq, *pai;
 	// struct Programa p;
};

// struct Programa {
// 	int tamanho;
// 	int cod;
// }

typedef struct No No;
// typedef struct Programa Programa;

//Esqueletos
No* initNo(int tamanho, No *pai);
int doisElevado(unsigned exp);
void finalizarProcesso(No *raiz, int cod);
void imprimirProcessosPreOrdem(No *no, char* estados);
// void iniciarPrograma(No *raiz, Programa *p);

int main() {
	int expoenteMemoria;
	scanf("%d", &expoenteMemoria);
	int memoriaTotal = doisElevado(expoenteMemoria);
	No *raiz = initNo(memoriaTotal, NULL);

	// INICIO DO PROCESSO DE POPULAÇÃO DA ARVORE
	raiz->estado = PARTICIONADO;
	raiz->tamanhoUtilizado = 0;
	raiz->codigo = 9;

	No *n1 = (No*) malloc(sizeof(No));
	n1->pai = raiz;
	n1->estado = PARTICIONADO;
	n1->tamanhoMemoria = n1->pai->tamanhoMemoria/2;
	n1->tamanhoUtilizado = 0;;
	n1->codigo = 1;

	No *n2 = malloc(sizeof(No));
	n2->estado = LIVRE;
	n2->pai = n1;
	n2->tamanhoMemoria = n2->pai->tamanhoMemoria/2;
	n2->tamanhoUtilizado = 0;
	n2->codigo = 2;

	No *n3 = malloc(sizeof(No));
	n3->pai = n1;
	n3->estado = OCUPADO;
	n3->tamanhoMemoria = n3->pai->tamanhoMemoria/2;
	n3->tamanhoUtilizado = 3;
	n3->codigo = 3;

	No *n4 = malloc(sizeof(No));
	n4->pai = raiz;
	n4->estado = LIVRE;
	n4->tamanhoMemoria = n4->pai->tamanhoMemoria/2;
	n4->tamanhoUtilizado = 0;
	n4->codigo = 4;

	raiz->esq = n1;
	raiz->dir = n4;
	n1->esq = n2;
	n1->dir = n3;

	// FIM DO PROCESSO DE POPULAÇÃO DA ARVORE

	// Vetor para facilitar impressões de estado
	char *estados = malloc(sizeof(char));
	estados[0] = 'L'; // Livre
	estados[1] = 'P'; // Particionado
	estados[2] = 'O'; // Ocupado

	int op;
	// Le as operações do sistema
	while (scanf("%d", &op) != EOF) {
		switch (op) {
		  case INICIAR_PROCESSO: {
		  	int cod, tamanho;
		  	scanf("%d %d", &cod, &tamanho);
		    break;
		  }
		  case FINALIZAR_PROCESSO: {
		  	int cod;
		  	scanf("%d", &cod);
		  	finalizarProcesso(&(*raiz), cod);
		    break;
		  }
		  case FRAGMENTACAO: {
		    break;
		  }
		  case RELATORIO: {

		    break;
		  }
		  case IMPRIME_SEMENTES: {

		  	printf("Pre = ");
		  	imprimirProcessosPreOrdem(raiz, estados);
		  	printf("\n");
		    break;
		  }
		  case IMPRIME_PROCESSOS: {
		  	
		    break;
		  }
		  default:
		    printf("Operação não cadastrada!\n");
		  break;
		}
	}

	free(estados);
	// Liberar árvore completa
}

No* initNo(int tamanho, No *pai) {
  // Freitas, favor identar essa porcaria de 2 espaços. Att, Matheus <3
  No *no = malloc(sizeof(No));
  no->tamanhoMemoria = tamanho;
  no->dir = no->esq = NULL;
  no->pai = pai;
  no->estado = LIVRE;
}

/*
 * Função para finalizar um processo com código cod
 */
void finalizarProcesso(No *no, int cod) {
	if (no != NULL && no->esq == NULL && no->dir == NULL) {
		if (no->codigo == cod && no->estado == OCUPADO) {
			no->estado = LIVRE;
			no->tamanhoUtilizado = 0;
			no->codigo = 0;
		}
	} else {
		finalizarProcesso(&(*no->esq), cod);
		finalizarProcesso(&(*no->dir), cod);
		if (no->esq->estado == LIVRE && no->dir->estado == LIVRE) {
			free(no->esq);
			free(no->dir);
			no->dir = NULL;
			no->esq = NULL;
			no->estado = LIVRE;
		}
	}
}

/*
 * Função para imprimir processos pre-ordem
 */
void imprimirProcessosPreOrdem(No *no, char* estados) {
	if (no != NULL) {
		printf("(%c:", estados[no->estado]);
		if (no->estado == OCUPADO) {
			printf("%d/%d[%d])", no->tamanhoUtilizado, no->tamanhoMemoria, no->codigo);
		} else {
			printf("%d)", no->tamanhoMemoria);
		}
		imprimirProcessosPreOrdem(no->esq, estados);
		imprimirProcessosPreOrdem(no->dir, estados);
	}
}

// HELPERS
int doisElevado(unsigned exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= 2;
    return result;
 }
