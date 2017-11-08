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
void printProgramas(Pasta *pasta, char *caminho, int tam);
Pasta* encontraMaximo(Pasta *arvore);
Pasta* desinstalarPrograma(Pasta *no, Pasta *mae, char *nomeRemover, bool *removeuPrograma);
void transfereProgramas(Pasta *p1, Pasta *p2);
void desalocarArvore(Pasta* raiz);
Local ladoDaPasta(Pasta *p);
Pasta* balanceiaArvore(char** programasInOrdem, int tam, Pasta* mae);
void criaVetorBalanceamento(Pasta *pasta, char** vetor, int *index);
int tempoDeExecucao(Pasta *raiz, char* nomePrograma);

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
				char *nomeNovoPrograma = (char*) malloc(TAM_NOME * sizeof(char));
				scanf("%s", nomeNovoPrograma);

				raiz = instalarPrograma(raiz, nomeNovoPrograma, NULL, RAIZ, pastaInstalada);

				printf("[INSTALL] Programa %s.exe instalado com sucesso na pasta %s\n", nomeNovoPrograma, (*pastaInstalada)->nome);
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
      case OTIMIZAR_RESPOSTA: {
      	int numPastas = contaPastas(raiz);
      	int *index = malloc(sizeof(int));
      	*index = 0;
      	char **programasInOrdem = malloc(numPastas * sizeof(char*));
      	criaVetorBalanceamento(raiz, programasInOrdem, index);

      	printf("Programas: ");
      	for (int i = 0; i < numPastas; i++) {
      		printf("%s\n", programasInOrdem[i]);
      	}
        desalocarArvore(raiz);
        raiz = balanceiaArvore(programasInOrdem, numPastas, NULL);
        printf("[OPTIMIZE] O sistema de acesso a programas foi otimizado\n");
      	break;
      }
      case VELOCIDADE_RESPOSTA: {
        char *nomeProgramaPesquisa = malloc(TAM_NOME * sizeof(char));
        int tempo;
				scanf("%s %d", nomeProgramaPesquisa, tempo);
        int tempoResposta = tempoDeExecucao(raiz, nomeProgramaPesquisa);
        int tempoRespostaSemRaiz = tempoResposta - 1;
        if (tempo > tempoRespostaSemRaiz)
          printf("[DELAY][OK] O acesso ao programa %s.exe foi concluido em %d segundos", nomeProgramaPesquisa, tempoRespostaSemRaiz);
        else
          printf("[DELAY][FAIL] O acesso ao programa %s.exe ultrapassou o limite de %d segundo", nomeProgramaPesquisa, tempo);
        printf("%d\n", tempoResposta);
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
      case RESTAURAR: {
        desalocarArvore(raiz);
        raiz = recriaArvore(*filaPreOrdem, *filaInOrdem, NULL);
        printf("[RESTORE] Sistema restaurado para a versao do backup\n");
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

void desalocarArvore(Pasta* raiz) {
  if (raiz == NULL)
    return;
  desalocarArvore(raiz->esq);
  desalocarArvore(raiz->dir);
  free(raiz->nome);
  raiz->nome = NULL;
  raiz->nomePrograma = NULL;
  raiz->esq = raiz->dir = NULL;
  free(raiz);
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
		no->nome = geraNomeDaPasta(anterior, ladoAtual);
		no->nomePrograma = nomeNovoPrograma;
		*pastaInstalada = no;
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

Pasta* balanceiaArvore(char** programasInOrdem, int tam, Pasta* mae) {
  if (tam == 0)
    return NULL;
  int indiceMediana = tam / 2; //Meidana é a nova raiz dessa chamada
  Pasta* novaRaiz = criaPasta(programasInOrdem[indiceMediana], mae);

  //Cria a array que possui a arvore à esquerda de novaRaiz
  int tamProgramasEsq = indiceMediana;
  char** programasEsq = malloc(tamProgramasEsq * sizeof(char*));
  for(int i = 0; i < tamProgramasEsq; i++)
    programasEsq[i] = programasInOrdem[i];

  //Cria a array que possui a arvore à direita de novaRaiz
  int tamProgramasDir = tam - (indiceMediana + 1); // indiceMediana precisa ser incrementado, pois a mediana não entra nesse vetor
  char** programasDir = malloc(tamProgramasDir * sizeof(char*));
  for(int j = 0; j < tamProgramasDir; j++)
    programasEsq[j] = programasInOrdem[j];

  novaRaiz->esq = balanceiaArvore(programasEsq, tamProgramasEsq, novaRaiz);
  novaRaiz->dir = balanceiaArvore(programasDir, tamProgramasDir, novaRaiz);
  free(programasDir); free(programasEsq); //Aqui já posso liberar essas arrays
  return novaRaiz;
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

int tempoDeExecucao(Pasta *raiz, char* nomePrograma) {
  if (raiz == NULL)
    return 0;
  if (strcmp(raiz->nomePrograma, nomePrograma) > 0)
    return tempoDeExecucao(raiz->esq, nomePrograma) + 1;
  if (strcmp(raiz->nomePrograma, nomePrograma) < 0)
    return tempoDeExecucao(raiz->dir, nomePrograma) + 1;
  return 1;
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

void criaVetorBalanceamento(Pasta *pasta, char** vetor, int *index) {
	if (pasta != NULL) {
		criaVetorBalanceamento(pasta->esq, vetor, index);
		vetor[(*index)] = pasta->nomePrograma;
		printf("index: %d - %s\n", (*index), vetor[(*index)]);
		*index += 1;
		criaVetorBalanceamento(pasta->dir, vetor, index);
	}
}

char* geraNomeDaPasta(Pasta *pastaMae, Local lado) {
	char *nome = malloc((TAM_NOME) * sizeof(char));
	nome[0] = '\0';
	if(lado == RAIZ || pastaMae == NULL) {
		strcpy(nome, "raiz");
		return nome;
	}
	strcpy(nome, pastaMae->nomePrograma);
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
