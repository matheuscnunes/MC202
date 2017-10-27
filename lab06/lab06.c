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
Programa initPrograma(int tamanho, int cod);
int doisElevado(unsigned exp);
bool finalizarProcesso(No *no, int cod);
bool iniciarPrograma(No *no, Programa p);
char estadoDoNo(No *no);
void imprimirProcessosPreOrdem(No *no);
void imprimirProcessosPosOrdem(No *no);
void imprimirProcessosInOrdem(No *no);

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
		  	Programa p = initPrograma(tamanho, cod);
		  	bool conseguiuIniciarPrograma = iniciarPrograma(raiz, p);
		  	if(conseguiuIniciarPrograma)
		  		printf("INICIOU PORRAAAA\n");
		  	else
		  		printf("NEM ROLOU HEIN\n");
		  	imprimirProcessosPreOrdem(raiz);
		  	printf("\n");
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

		  	printf("Sim = ");
		  	imprimirProcessosInOrdem(raiz);
		  	printf("\n");
		  	printf("Pre = ");
		  	imprimirProcessosPreOrdem(raiz);
		  	printf("\n");
		  	printf("Pos = ");
		  	imprimirProcessosPosOrdem(raiz);
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

Programa initPrograma(int tamanho, int cod) {
	Programa p;
	p.tamanho = tamanho;
	p.codigo = cod;
	return p;
}


bool iniciarPrograma(No *no, Programa p) {
	if(p.tamanho > no->tamanhoMemoria)
		return false;
	bool estaOcupado = no->p != NULL;
	if(estaOcupado)
		return false;
	bool estaParticionado = no->dir != NULL && no->esq != NULL;
	if(p.tamanho <= no->tamanhoMemoria) {
		if(estaParticionado == false) {
			if(p.tamanho > no->tamanhoMemoria/2) {
				no->p = malloc(sizeof(Programa));
				*(no->p) = p;
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
bool finalizarProcesso(No *no, int cod) {
	char estado = estadoDoNo(no);
	if (estado == OCUPADO) {
		if (no->p->codigo == cod) {
			free(no->p);
			no->p = NULL;
			return true;
		}
	} else if(estado == PARTICIONADO) {
		bool finalizouPrograma = finalizarProcesso(no->esq, cod);
		if(finalizouPrograma == false)
			finalizouPrograma = finalizarProcesso(no->dir, cod);
		bool filhosNaoEstaoParticionados = estadoDoNo(no->esq) != PARTICIONADO && estadoDoNo(no->dir) != PARTICIONADO;
		bool filhosEstaoLivres = no->esq->p == NULL && no->dir->p == NULL;
		if (filhosNaoEstaoParticionados && filhosEstaoLivres) {
			free(no->esq);
			free(no->dir);
			no->dir = NULL;
			no->esq = NULL;
		}
		return finalizouPrograma;
	}
	return false;
}

char estadoDoNo(No *no) {
	if(no->p != NULL)
		return OCUPADO;
	if(no->esq != NULL && no->dir != NULL)
		return PARTICIONADO;
	return LIVRE;
}

/*
 * Função para imprimir processos pre-ordem
 */
void imprimirProcessosPreOrdem(No *no) {
	if (no != NULL) {
		char estado = estadoDoNo(no);
		printf("(%c:", estado);
		if (estado == OCUPADO) {
			printf("%d/%d[%d])", no->p->tamanho, no->tamanhoMemoria, no->p->codigo);
		} else {
			printf("%d)", no->tamanhoMemoria);
		}
		imprimirProcessosPreOrdem(no->esq);
		imprimirProcessosPreOrdem(no->dir);
	}
}

/*
 * Função para imprimir processos pos-ordem
 */
void imprimirProcessosPosOrdem(No *no) {
	char estado = estadoDoNo(no);
	if (estado == PARTICIONADO) {
		imprimirProcessosPosOrdem(no->esq);
		imprimirProcessosPosOrdem(no->dir);
	}
	printf("(%c:", estado);
	if (estado == OCUPADO) {
		printf("%d/%d[%d])", no->p->tamanho, no->tamanhoMemoria, no->p->codigo);
	} else {
		printf("%d)", no->tamanhoMemoria);
	}
}

/*
 * Função para imprimir processos in-ordem
 */
void imprimirProcessosInOrdem(No *no) {
	if (no != NULL) {
		imprimirProcessosInOrdem(no->esq);
		char estado = estadoDoNo(no);
		printf("(%c:", estado);
		if (estado == OCUPADO) {
			printf("%d/%d[%d])", no->p->tamanho, no->tamanhoMemoria, no->p->codigo);
		} else {
			printf("%d)", no->tamanhoMemoria);
		}
		imprimirProcessosInOrdem(no->dir);
	}
}

// HELPERS
int doisElevado(unsigned exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= 2;
    return result;
 }
