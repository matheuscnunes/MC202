#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
	Situacao estado;
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
int memoriaFragmentada(No *no);
void relatorioDoSistema(No *no, int *ocupados, int *livres, int *particionados, int *memoriaUsada);
void imprimirProcessosAlocados(No *no, bool *encontrouPrograma);
void desalocaMemoria(No *no);

int main() {
	int expoenteMemoria;
	scanf("%d", &expoenteMemoria);
	int memoriaTotal = doisElevado(expoenteMemoria);
	No *raiz = initNo(memoriaTotal, NULL);

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
		  		printf("Processo (%d) de tamanho %d inicializado com sucesso\n", cod, tamanho);
		  	else
		  		printf("Memoria insuficiente\n");
		    break;
		  }
		  case FINALIZAR_PROCESSO: {
		  	int cod;
		  	scanf("%d", &cod);
		  	if (finalizarProcesso(&(*raiz), cod)) {
		  		printf("Processo (%d) finalizado com sucesso\n", cod);
		  	} else {
		  		printf("Nao existe processo de codigo %d inicializado no sistema\n", cod);
		  	}
		    break;
		  }
		  case FRAGMENTACAO: {
		  	int fragmentacao = memoriaFragmentada(raiz);
		  	printf("Quantidade total de memoria desperdicada por fragmentacao: %d\n", fragmentacao);
		    break;
		  }
		  case RELATORIO: {
		  	int *ocupados = malloc(sizeof(int));
		  	int *livres = malloc(sizeof(int));
		  	int *memoriaUsada = malloc(sizeof(int));
		  	int *particionados = malloc(sizeof(int));
		  	*ocupados = *livres = *memoriaUsada = *particionados = 0; 
		  	relatorioDoSistema(raiz, ocupados, livres, particionados, memoriaUsada);
		  	float mem = *memoriaUsada;
		  	float percentual = mem / memoriaTotal * 100;
		  	double res = floor(percentual);
		  	printf("[RELATORIO DE SISTEMA]\n");
		  	printf("%d Ocupados\n", *ocupados);
		  	printf("%d Livres\n", *livres);
		  	printf("%d Particionados\n", *particionados);
		  	printf("Memoria utilizada = %.0lf / 100\n", res);

		  	free(ocupados);
		  	free(livres);
		  	free(memoriaUsada);
		  	free(particionados);
		    break;
		  }
		  case IMPRIME_SEMENTES: {
		  	printf("[SEMENTES GERADORAS]\n");
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
		  	printf("[PROCESSOS PRESENTES NA MEMORIA]\n");
		  	bool encontrouPrograma = false;
		  	imprimirProcessosAlocados(raiz, &encontrouPrograma);
		  	if (!encontrouPrograma) {
		  		printf("Nenhum processo presente\n");
		  	}
		    break;
		  }
		  default:
		    printf("Operação não cadastrada!\n");
		  break;
		}
	}

	desalocaMemoria(raiz);
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
		int finalizouPrograma = finalizarProcesso(no->esq, cod);
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

void relatorioDoSistema(No *no, int *ocupados, int *livres, int *particionados, int *memoriaUsada) {
	char estado = estadoDoNo(no);
	if (estado == PARTICIONADO) {
		*particionados += 1;

		relatorioDoSistema(no->esq, &(*ocupados), &(*livres), &(*particionados), &(*memoriaUsada));
		relatorioDoSistema(no->dir, &(*ocupados), &(*livres), &(*particionados), &(*memoriaUsada));
	} else if (estado == OCUPADO) {
		*ocupados += 1;
		*memoriaUsada += no->tamanhoMemoria;		
	} else {
		*livres += 1;
	}
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

int memoriaFragmentada(No *no) {
	char estado = estadoDoNo(no);
	if(estado == LIVRE)
		return 0;
	if(estado == OCUPADO)
		return no->tamanhoMemoria - no->p->tamanho;
	return memoriaFragmentada(no->esq) + memoriaFragmentada(no->dir);
}

void imprimirProcessosAlocados(No *no, bool *encontrouPrograma) {
	char estado = estadoDoNo(no);
	if(estado == OCUPADO) {
		printf("%d [Processo: %d]\n", no->tamanhoMemoria, no->p->codigo);
		*encontrouPrograma = true;
	} else if(estado == PARTICIONADO) {
		imprimirProcessosAlocados(no->esq, &(*encontrouPrograma));
		imprimirProcessosAlocados(no->dir, &(*encontrouPrograma));
	}
}

void desalocaMemoria(No *no) {
	if(no != NULL) {
		No *esq = no->esq;
		No *dir = no->dir;
		free(no);
		desalocaMemoria(&(*esq));
		desalocaMemoria(&(*dir));
	}	
}

// HELPERS
int doisElevado(unsigned exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= 2;
    return result;
 }
