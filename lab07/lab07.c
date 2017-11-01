#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Nomes: Matheus Cezar Nunes , Mateus Freitas Silveira
 * RA: 203373 e 203263
 *
 */

//Enuns
typedef enum Operacoes {
  INSERIR = 1
} Operacoes;
typedef enum bool { false, true } bool;
typedef enum Local { DIREITA, ESQUERDA, RAIZ } Local;

#define TAM_NOME 30

//Structs
struct Pasta {
	char *nome;
	char *nomePrograma;
 	struct Pasta *dir, *esq;
};

struct FilaProgramas {
	int inicio, fim;
	char* programas[];
};

typedef struct Pasta Pasta;
typedef struct FilaProgramas FilaProgramas;

//Esqueletos
Pasta* criaPasta(char *nomePrograma, Local lado);
char* geraNomeDaPasta(char *nomeBase, Local lado);
Pasta* recriaArvore(FilaProgramas *preOrdem, FilaProgramas *inOrdem, Local ladoAtual);
FilaProgramas* criaFila();
void enfileirar(FilaProgramas *f, char *nomePrograma);
char* desenfileirar(FilaProgramas *f);
Local ladoDoProgramaEmRelacaoDaPasta(Pasta *no, char* nomePrograma);
bool filaVazia(FilaProgramas *f);

int main() {
	int qtdProgramas;
	scanf("%d", &qtdProgramas);
	FilaProgramas *filaPreOrdem = criaFila();
	FilaProgramas *filaInOrdem = criaFila();
	//Lê in-ordem
	for(int i = 0; i < qtdProgramas; i++) {
		char *nomePrograma = malloc(TAM_NOME * sizeof(char));
		scanf("%s", nomePrograma);
		enfileirar(filaInOrdem, nomePrograma);
	}
	//Lê pré-ordem
	for(int i = 0; i < qtdProgramas; i++) {
		char *nomePrograma = malloc(TAM_NOME * sizeof(char));
		scanf("%s", nomePrograma);
		enfileirar(filaPreOrdem, nomePrograma);
	}

	Pasta *raiz = recriaArvore(filaPreOrdem, filaInOrdem, RAIZ);

	int op;
	// Le as operações do sistema
	while (scanf("%d", &op) != EOF) {
		switch (op) {
		  default:
		    printf("Operação não cadastrada!\n");
		  break;
		}
	}

	// Liberar árvore completa
}

Pasta* criaPasta(char *nomePrograma, Local lado) {
	Pasta *novaPasta = malloc(sizeof(Pasta));
	char *nomePasta = geraNomeDaPasta(nomePrograma, lado);
	novaPasta->nome = nomePasta;
	novaPasta->nomePrograma = nomePrograma;
	novaPasta->dir = novaPasta->esq = NULL;

	return novaPasta;
}

char* geraNomeDaPasta(char *nomeBase, Local lado) {
	char *nome = malloc((TAM_NOME + 4) * sizeof(char));
	nome[0] = '\0';
	if(lado == RAIZ) {
		strcat(nome, "raiz");
		return nome;
	}
	strcat(nome, nomeBase);
	if(lado == DIREITA)
		strcat(nome, "_dir");
	else if(lado == ESQUERDA)
		strcat(nome, "_esq");

	return nome;
}

Pasta* recriaArvore(FilaProgramas *preOrdem, FilaProgramas *inOrdem, Local ladoAtual) {
	if(filaVazia(preOrdem) || filaVazia(inOrdem))
		return NULL;

	char* programaMae = desenfileirar(preOrdem);
	FilaProgramas *arvoreEsqInOrdem = criaFila();
	char* nomePrograma = desenfileirar(inOrdem);
	while(nomePrograma != NULL && strcmp(programaMae, nomePrograma) != 0) {
		enfileirar(arvoreEsqInOrdem, nomePrograma);
		nomePrograma = desenfileirar(inOrdem);
	}
	FilaProgramas *arvDirInOrdem = inOrdem; //O que sobrou na fila In-Ordem pertence ao lado direito da arvore
	Pasta *p = criaPasta(programaMae, ladoAtual);
	p->esq = recriaArvore(preOrdem, arvoreEsqInOrdem, ESQUERDA);
	p->dir = recriaArvore(preOrdem, arvDirInOrdem, DIREITA);
	return p;
}

FilaProgramas* criaFila() {
	FilaProgramas *f = malloc(sizeof(FilaProgramas));
	f->inicio = f->fim = 0;
	return f;
}

void enfileirar(FilaProgramas *f, char *nomePrograma) {
	f->fim += 1;
	f->programas[f->fim] = nomePrograma;
}

char* desenfileirar(FilaProgramas *f) {
	if(filaVazia(f))
		return NULL;
	char* primeiroDaFila = f->programas[f->inicio];
	f->inicio += 1;
	return primeiroDaFila;
}

bool filaVazia(FilaProgramas *f) {
	return f->fim == f->inicio;
}

Local ladoDoProgramaEmRelacaoDaPasta(Pasta *no, char* nomePrograma) {
	if(no->nomePrograma == NULL)
		return RAIZ;
	if(strcmp(nomePrograma, no->nomePrograma) < 0)
		return ESQUERDA;
	else
		return DIREITA;
}


