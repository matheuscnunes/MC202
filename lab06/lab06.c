#include <stdio.h>
#include <stdlib.h>

/*
 * Nomes: Matheus Cezar Nunes , Mateus Freitas Silveira
 * RA: 203373 e 203263
 *
 */

//Enuns
typedef enum Situacao { LIVRE = 'L', PARTICIONADO = 'P', OCUPADO = 'O' } Situacao;
typedef enum Operacoes {
  INICIAR_PROCESSO = 1, FINALIZAR_PROCESSO, FRAGMENTACAO, RELATORIO,
  IMPRIME_SEMENTES, IMPRIME_PROCESSOS
} Operacoes;
typedef enum bool { false, true } bool;

//Structs
struct No {
	int tamanhoMemoria;
 	struct No *dir, *esq, *pai;
 	struct Programa *p;
};

struct Programa {
	int tamanho;
	int codigo;
};

typedef struct No No;
typedef struct Programa Programa;

//Esqueletos
No* initNo(int espacoMemoria, No *pai);
Programa* initPrograma(int tamanho, int cod);
int doisElevado(unsigned exp);
void finalizarProcesso(No *raiz, int cod);
void imprimirProcessosPreOrdem(No *no, char* estados);
bool iniciarPrograma(No *no, Programa *p);
char estadoDoNo(No *no);

int main() {
	int expoenteMemoria;
	scanf("%d", &expoenteMemoria);
	int memoriaTotal = doisElevado(expoenteMemoria);
	No *raiz = initNo(memoriaTotal, NULL);

	// INICIO DO PROCESSO DE POPULAÇÃO DA ARVORE
	// raiz->estado = PARTICIONADO;
	// raiz->tamanhoUtilizado = 0;
	// raiz->codigo = 9;

	// No *n1 = (No*) malloc(sizeof(No));
	// n1->pai = raiz;
	// n1->estado = PARTICIONADO;
	// n1->tamanhoMemoria = n1->pai->tamanhoMemoria/2;
	// n1->tamanhoUtilizado = 0;;
	// n1->codigo = 1;

	// No *n2 = malloc(sizeof(No));
	// n2->estado = LIVRE;
	// n2->pai = n1;
	// n2->tamanhoMemoria = n2->pai->tamanhoMemoria/2;
	// n2->tamanhoUtilizado = 0;
	// n2->codigo = 2;

	// No *n3 = malloc(sizeof(No));
	// n3->pai = n1;
	// n3->estado = OCUPADO;
	// n3->tamanhoMemoria = n3->pai->tamanhoMemoria/2;
	// n3->tamanhoUtilizado = 3;
	// n3->codigo = 3;

	// No *n4 = malloc(sizeof(No));
	// n4->pai = raiz;
	// n4->estado = LIVRE;
	// n4->tamanhoMemoria = n4->pai->tamanhoMemoria/2;
	// n4->tamanhoUtilizado = 0;
	// n4->codigo = 4;

	// raiz->esq = n1;
	// raiz->dir = n4;
	// n1->esq = n2;
	// n1->dir = n3;

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
		  	Programa *p = initPrograma(tamanho, cod);
		  	bool conseguiuIniciarPrograma = iniciarPrograma(raiz, p);
		  	if(conseguiuIniciarPrograma)
		  		printf("INICIOU PORRAAAA\n");
		  	else
		  		printf("NEM ROLOU HEIN\n");
		  	imprimirProcessosPreOrdem(raiz, estados);
		    break;
		  }
		  case FINALIZAR_PROCESSO: {
		  	int cod;
		  	scanf("%d", &cod);
		  	// finalizarProcesso(&(*raiz), cod);
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

No* initNo(int espacoMemoria, No *pai) {
  No *no = malloc(sizeof(No));
  no->tamanhoMemoria = espacoMemoria;
  no->dir = no->esq = NULL;
  no->pai = pai;
  no->p = NULL;
  return no;
}

Programa* initPrograma(int tamanho, int cod) {
	Programa *p = malloc(sizeof(Programa));
	p->tamanho = tamanho;
	p->codigo = cod;
	return p;
}


bool iniciarPrograma(No *no, Programa *p) {
	if(p->tamanho > no->tamanhoMemoria)
		return false;
	bool estaOcupado = no->p != NULL;
	if(estaOcupado)
		return false;
	bool estaParticionado = no->dir != NULL && no->esq != NULL;
	if(p->tamanho <= no->tamanhoMemoria) {
		if(estaParticionado == false) {
			if(p->tamanho > no->tamanhoMemoria/2) {
				no->p = p;
				return true;
			} else {
				no->dir = initNo(no->tamanhoMemoria/2, no);
				no->esq = initNo(no->tamanhoMemoria/2, no);
			}
		}
		bool iniciouNaEsquerda = iniciarPrograma(no->esq, p);
		if(iniciouNaEsquerda)
			return iniciouNaEsquerda;
		else
			return iniciarPrograma(no->dir, p);
	}
	return false;
}

/*
 * Função para finalizar um processo com código cod
 */
void finalizarProcesso(No *no, int cod) {
	// if (no != NULL && no->esq == NULL && no->dir == NULL) {
	// 	if (no->codigo == cod && no->estado == OCUPADO) {
	// 		no->estado = LIVRE;
	// 		no->tamanhoUtilizado = 0;
	// 		no->codigo = 0;
	// 	}
	// } else {
	// 	finalizarProcesso(&(*no->esq), cod);
	// 	finalizarProcesso(&(*no->dir), cod);
	// 	if (no->esq->estado == LIVRE && no->dir->estado == LIVRE) {
	// 		free(no->esq);
	// 		free(no->dir);
	// 		no->dir = NULL;
	// 		no->esq = NULL;
	// 		no->estado = LIVRE;
	// 	}
	// }
}

/*
 * Função para imprimir processos pre-ordem
 */
void imprimirProcessosPreOrdem(No *no, char* estados) {
	if (no != NULL) {
		char estado = estadoDoNo(no);
		printf("(%c:", estado);
		if (estado == OCUPADO) {
			printf("%d/%d[%d])", no->p->tamanho, no->tamanhoMemoria, no->p->codigo);
		} else {
			printf("%d)", no->tamanhoMemoria);
		}
		imprimirProcessosPreOrdem(no->esq, estados);
		imprimirProcessosPreOrdem(no->dir, estados);
	}
}

char estadoDoNo(No *no) {
	if(no->p != NULL)
		return OCUPADO;
	if(no->esq != NULL && no->dir != NULL)
		return PARTICIONADO;
	return LIVRE;
}

// HELPERS
int doisElevado(unsigned exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= 2;
    return result;
 }
