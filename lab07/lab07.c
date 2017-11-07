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
Pasta* criaPasta(char *nomePrograma, Pasta *mae);
char* geraNomeDaPasta(Pasta *pastaMae, Local lado);
Pasta* recriaArvore(FilaProgramas preOrdem, FilaProgramas inOrdem, Pasta *mae);
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
Pasta* encontraMaximo(Pasta *arvore);
Pasta* desinstalarPrograma(Pasta *no, Pasta *mae, char *nomeRemover, bool *removeuPrograma);
void transfereProgramas(Pasta *p1, Pasta *p2);
void desalocarArvore(Pasta* raiz);
Local ladoDaPasta(Pasta *p);

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
	Pasta *raiz = recriaArvore(*filaPreOrdem, *filaInOrdem, NULL); //The pointer can't be passed, because we can't lose the first backup

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
      case DESINSTALAR: {
        char *nomeProgramaRemover = malloc(TAM_NOME * sizeof(char));
				scanf("%s", nomeProgramaRemover);
        bool *removeuPrograma = malloc(sizeof(bool));
        *removeuPrograma = false;
        raiz = desinstalarPrograma(raiz, NULL, nomeProgramaRemover, removeuPrograma);
        if (*removeuPrograma == true)
          printf("[UNINSTALL] Programa %s.exe desinstalado com sucesso\n", nomeProgramaRemover);
        else
          printf("[UNINSTALL] Nao foi encontrado no sistema nenhum programa com nome %s\n", nomeProgramaRemover);
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
      case RESTAURAR: {
        desalocarArvore(raiz);
        raiz = recriaArvore(*filaPreOrdem, *filaInOrdem, NULL);
        printf("[RESTORE] Sistema restaurado para a versao do backup");
        break;
      }
			default:
		    	printf("Operação não cadastrada!\n");
			break;
		}
	}

	// Liberar árvore completa
}

void backupInOrdem(Pasta *pasta, FilaProgramas *in) {
	if (pasta != NULL){
		backupInOrdem(pasta->esq, in);
		enfileirar(in, pasta->nomePrograma);
		backupInOrdem(pasta->dir, in);
	}
}

void backupPreOrdem(Pasta *pasta, FilaProgramas *pre) {
	if (pasta != NULL){
		enfileirar(pre, pasta->nomePrograma);
		backupInOrdem(pasta->esq, pre);
		backupInOrdem(pasta->dir, pre);
	}
}

void desalocarArvore(Pasta* raiz) {
  if (raiz == NULL)
    return;
  desalocarArvore(raiz->esq);
  desalocarArvore(raiz->dir);
  free(raiz->nome);
  // free(raiz->nomePrograma);
  raiz->esq = raiz->dir = NULL;
  free(raiz);
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
		no->nome = geraNomeDaPasta(anterior, ladoAtual);
		no->nomePrograma = nomeNovoPrograma;
		*pastaInstalada = no;
		return no;
	}
		return no;
}

Pasta* desinstalarPrograma(Pasta *no, Pasta *mae, char *nomeRemover, bool *removeuPrograma) {
  //TODO: FIX THE NAME OF FOLDER
  if(no == NULL)
    return NULL;
  if(strcmp(no->nomePrograma, nomeRemover) == 0) {
    *removeuPrograma = true; //Se entrou aqui, é porque o programa será removido, então já posso assumir que ele foi desinstaldo
    if(no->esq == NULL && no->dir == NULL)
      return NULL;
    if (no->esq == NULL) {
      no->dir->nome = geraNomeDaPasta(mae, ladoDoProgramaEmRelacaoDaPasta(mae, no->dir->nomePrograma));
      return no->dir;
    }
    if (no->dir == NULL) {
      no->esq->nome = geraNomeDaPasta(mae, ladoDoProgramaEmRelacaoDaPasta(mae, no->esq->nomePrograma));
      return no->esq;
    }
    Pasta* extremaDireita = encontraMaximo(no->esq);
    transfereProgramas(no, extremaDireita);
  }
  no->esq = desinstalarPrograma(no->esq, no, nomeRemover, removeuPrograma);
  no->dir = desinstalarPrograma(no->dir, no, nomeRemover, removeuPrograma);
  return no;
}

/**
 * Função para recriar uma árvore dado duas semestes geradoras
 */
Pasta* recriaArvore(FilaProgramas preOrdem, FilaProgramas inOrdem, Pasta *mae) {
	if (filaVazia(&preOrdem) || filaVazia(&inOrdem))
		return NULL;

	char* programaRaiz = desenfileirar(&preOrdem);
	FilaProgramas *arvoreEsqInOrdem = criaFila((inOrdem.fim - inOrdem.inicio) / 2);
	char* nomePrograma = desenfileirar(&inOrdem);
	while (nomePrograma != NULL && strcmp(programaRaiz, nomePrograma) != 0) {
		enfileirar(arvoreEsqInOrdem, nomePrograma);
		nomePrograma = desenfileirar(&inOrdem);
	}
	FilaProgramas arvoreDirInOrdem = inOrdem; //O que sobrou na fila In-Ordem pertence ao lado direito da arvore
	Pasta *p = criaPasta(programaRaiz, mae);
	p->esq = recriaArvore(preOrdem, *arvoreEsqInOrdem, p);
	p->dir = recriaArvore(preOrdem, arvoreDirInOrdem, p);
	return p;
}

// INICIALIZERS

Pasta* criaPasta(char *nomePrograma, Pasta *mae) {
	Pasta *novaPasta = malloc(sizeof(Pasta));
	char *nomePasta = geraNomeDaPasta(mae, ladoDoProgramaEmRelacaoDaPasta(mae, nomePrograma));
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
	if (no == NULL)
		return RAIZ;
  if (no->nomePrograma == NULL)
    return RAIZ;
	if (strcmp(nomePrograma, no->nomePrograma) < 0)
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

char* geraNomeDaPasta(Pasta *pastaMae, Local lado) {
	char *nome = malloc((TAM_NOME) * sizeof(char));
	nome[0] = '\0';
	if(lado == RAIZ || pastaMae == NULL) {
		strcat(nome, "raiz");
		return nome;
	}
	strcat(nome, pastaMae->nomePrograma);
	if(lado == DIREITA)
		strcat(nome, "_dir");
	else if(lado == ESQUERDA)
		strcat(nome, "_esq");

	return nome;
}

// Local ladoDaPasta(Pasta *p) {
//   int ultimoIndice = strlen(p->nome) - 1;
//   if (p->nome[ultimoIndice] == 'q')
//     return ESQUERDA;
//   else if (p->nome[ultimoIndice] == 'r')
//     return DIREITA;
//   return RAIZ;
// }

Pasta* encontraMaximo(Pasta *arvore) {
  if(arvore->dir == NULL)
    return arvore;
  else
    return encontraMaximo(arvore->dir);
}

void transfereProgramas(Pasta *p1, Pasta *p2) {
  char* programaPasta2 = p2->nomePrograma;
  p2->nomePrograma = p1->nomePrograma;
  p1->nomePrograma = programaPasta2;
}
