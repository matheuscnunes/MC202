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
	int tamanhoMaximo, tamanhoDoPrograma;
	int codigo;
 	struct No *dir, *esq, *pai;
 	Situacao estado;
};

typedef struct No No;

//Esqueletos
No* initNo(int tamanho, No *pai);
int doisElevado(unsigned int);

//Funções

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

		    break;
		  }
		  case FINALIZAR_PROCESSO: {

		    break;
		  }
		  case FRAGMENTACAO: {
		    break;
		  }
		  case RELATORIO: {

		    break;
		  }
		  case IMPRIME_SEMENTES: {

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
}

No* initNo(int tamanho, No *pai) {
  No *no = malloc(sizeof(No));
  no->tamanhoMaximo = tamanho;
  no->tamanhoDoPrograma = 0;
  no->dir = no->esq = NULL;
  no->pai = pai;
  no->estado = LIVRE;
}

// HELPERS
int doisElevado(unsigned int exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= 2;
    return result;
 }
