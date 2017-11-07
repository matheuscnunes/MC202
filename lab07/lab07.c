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
  INSTALAR = 1, DESINSTALAR, VELOCIDADE_RESPOSTA, OTIMIZAR_RESPOSTA, BACKUP, RESTAURAR, PRINT_PROGRAMAS
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
	char** programas;
};

typedef struct Pasta Pasta;
typedef struct FilaProgramas FilaProgramas;

//Esqueletos
Pasta* criaPasta(char *nomePrograma, Local lado);
char* geraNomeDaPasta(char *nomeBase, Local lado);
Pasta* recriaArvore(FilaProgramas *preOrdem, FilaProgramas *inOrdem, Local ladoAtual);
FilaProgramas* criaFila(int qtd);
void enfileirar(FilaProgramas *f, char *nomePrograma);
char* desenfileirar(FilaProgramas *f);
Local ladoDoProgramaEmRelacaoDaPasta(Pasta *no, char* nomePrograma);
bool filaVazia(FilaProgramas *f);
void printPreOrdem(Pasta *pasta);
void printInOrdem(Pasta *pasta);
int contaPastas(Pasta *pasta);
void backupPreOrdem(Pasta *pasta, FilaProgramas *pre);
void backupInOrdem(Pasta *pasta, FilaProgramas *in);
Pasta* instalarPrograma(Pasta *raiz, char* nomeNovoPrograma, Pasta *pastaInstalada, Local ladoAtual, Pasta **pasta);
void printProgramas(Pasta *pasta, char *caminho, int tam);

int main() {
	int qtdProgramas;
	scanf("%d", &qtdProgramas);
	FilaProgramas *filaPreOrdem = criaFila(qtdProgramas);
	FilaProgramas *filaInOrdem = criaFila(qtdProgramas);
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

	printPreOrdem(raiz);
	printf("\n");
	printInOrdem(raiz);
	printf("\n");

	int op;
	// Le as operações do sistema
	while (scanf("%d", &op) != EOF) {
		switch (op) {
			case INSTALAR: {
				Pasta **pastaInstalada = malloc(sizeof(Pasta*));
				char *nomeNovoPrograma = malloc(TAM_NOME * sizeof(char));
				scanf("%s", nomeNovoPrograma);

				raiz = instalarPrograma(raiz, nomeNovoPrograma, NULL, RAIZ, pastaInstalada);

				printf("Adicionado com sucesso\n");
				printf("Pasta: %s\n", (*pastaInstalada)->nome);
				printf("IN: ");
				printInOrdem(raiz);
				printf("\nPRE: ");
				printPreOrdem(raiz);
				printf("\n");

				break;
			}
			case BACKUP: {

				free(filaPreOrdem);
				free(filaInOrdem);

				int quantidadeDePastas = contaPastas(raiz);
				FilaProgramas *in = criaFila(quantidadeDePastas);
				FilaProgramas *pre = criaFila(quantidadeDePastas);

				backupInOrdem(raiz, in);
				backupInOrdem(raiz, pre);

				filaPreOrdem = pre;
				filaInOrdem = in;
				printf("[BACKUP] Configuracao atual do sistema salva com sucesso\n");
				break;
			}
			case PRINT_PROGRAMAS: {
				char *caminho = malloc(2 * sizeof(char));
				strcpy(caminho, "C:");
				printProgramas(raiz, caminho, 4);
				break;
			}
			default:
		    	printf("Operação não cadastrada!\n");
			break;
		}
	}

	// Liberar árvore completa
}

/**
 * Função que realiza o backup das semestes geradoras in-ordem
 */
void backupInOrdem(Pasta *pasta, FilaProgramas *in) {
	if (pasta != NULL){
		backupInOrdem(pasta->esq, in);
		enfileirar(in, pasta->nomePrograma);
		backupInOrdem(pasta->dir, in);
	}
}

/**
 * Função que realiza o backup das semestes geradoras pre-ordem
 */
void backupPreOrdem(Pasta *pasta, FilaProgramas *pre) {
	if (pasta != NULL){
		enfileirar(pre, pasta->nomePrograma);
		backupInOrdem(pasta->esq, pre);
		backupInOrdem(pasta->dir, pre);
	}	
}

/**
 * Função que printa o diretórios dos programas instalados
 */
void printProgramas(Pasta *pasta, char *caminho, int tam) {
	if (pasta != NULL) {
		// Cria uma variável auxiliar para ser um diretório a frente do caminho

		caminho = realloc(caminho, (tam + strlen(pasta->nome) + 1) * sizeof(char));
		char *aux = malloc((strlen(caminho) + strlen(pasta->nome) + 1) * sizeof(char));

		strcpy(aux, caminho);
		strcat(aux, "/");
		strcat(aux, pasta->nome);

		printProgramas(pasta->esq, aux, tam + strlen(pasta->nome) + 1);
		printf("%s/%s.exe\n", aux, pasta->nomePrograma);
		printProgramas(pasta->dir, aux, tam + strlen(pasta->nome) + 1);

		// Retorna o diretório anterior para o auxiliar
		strcpy(aux, caminho);
	}
}

/**
 * Função para instalar um novo programa que retorna a raiz
 */
Pasta* instalarPrograma(Pasta *no, char* nomeNovoPrograma, Pasta *anterior, Local ladoAtual, Pasta **pastaInstalada) {
	if (no != NULL) {
		if (strcmp(nomeNovoPrograma, no->nomePrograma) >= 0) {
			no->dir = instalarPrograma(no->dir, nomeNovoPrograma, no, DIREITA, pastaInstalada);
		} else {
			no->esq = instalarPrograma(no->esq, nomeNovoPrograma, no, ESQUERDA, pastaInstalada);
		}
	} else {
		no = malloc(sizeof(Pasta));
		no->nome = geraNomeDaPasta(anterior != NULL ? anterior->nomePrograma : NULL, ladoAtual);
		no->nomePrograma = nomeNovoPrograma;
		*pastaInstalada = no;
	}
	return no;
}

/**
 * Função para recriar uma árvore dado duas semestes geradoras
 */
Pasta* recriaArvore(FilaProgramas *preOrdem, FilaProgramas *inOrdem, Local ladoAtual) {
	if (filaVazia(preOrdem) || filaVazia(inOrdem))
		return NULL;

	char* programaMae = desenfileirar(preOrdem);
	FilaProgramas *arvoreEsqInOrdem = criaFila((inOrdem->fim - inOrdem->inicio) / 2);
	char* nomePrograma = desenfileirar(inOrdem);
	while (nomePrograma != NULL && strcmp(programaMae, nomePrograma) != 0) {
		enfileirar(arvoreEsqInOrdem, nomePrograma);
		nomePrograma = desenfileirar(inOrdem);
	}
	FilaProgramas *arvoreDirInOrdem = inOrdem; //O que sobrou na fila In-Ordem pertence ao lado direito da arvore
	Pasta *p = criaPasta(programaMae, ladoAtual);
	p->esq = recriaArvore(preOrdem, arvoreEsqInOrdem, ESQUERDA);
	p->dir = recriaArvore(preOrdem, arvoreDirInOrdem, DIREITA);
	return p;
}

// INICIALIZERS

Pasta* criaPasta(char *nomePrograma, Local lado) {
	Pasta *novaPasta = malloc(sizeof(Pasta));
	char *nomePasta = geraNomeDaPasta(nomePrograma, lado);
	novaPasta->nome = nomePasta;
	novaPasta->nomePrograma = nomePrograma;
	novaPasta->dir = novaPasta->esq = NULL;

	return novaPasta;
}

FilaProgramas* criaFila(int qtd) {
	FilaProgramas *f = malloc(sizeof(FilaProgramas));
	f->programas = malloc(qtd * sizeof(char*));
	f->inicio = f->fim = 0;
	return f;
}


// HELPERS

int contaPastas(Pasta *pasta) {
	if (pasta != NULL) {
		return contaPastas(pasta->esq) + contaPastas(pasta->dir) + 1;
	} else {
		return 0;
	}
}

void enfileirar(FilaProgramas *f, char *nomePrograma) {
	f->programas[f->fim] = nomePrograma;
	f->fim += 1;
}

char* desenfileirar(FilaProgramas *f) {
	if(filaVazia(f))
		return NULL;
	char* primeiroDaFila = f->programas[f->inicio];
	f->inicio += 1;
	return primeiroDaFila;
}

bool filaVazia(FilaProgramas *f) {
	if (f == NULL)
		return true;
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

void printPreOrdem(Pasta *pasta) {
	if (pasta != NULL) {
		printf("%s", pasta->nomePrograma);
		printPreOrdem(pasta->esq);
		printPreOrdem(pasta->dir);
	}
}

void printInOrdem(Pasta *pasta) {
	if (pasta != NULL) {
		printInOrdem(pasta->esq);
		printf("%s", pasta->nomePrograma);
		printInOrdem(pasta->dir);
	}
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