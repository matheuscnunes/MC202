#include <stdio.h>
#include <stdlib.h>

/*
 * Nomes: Matheus Cezar Nunes , Mateus Freitas Silveira
 * RA: 203373 e 203263
 *
 */

//Enuns
typedef enum Situacao { LIVRE, PARTICIONADO, OCUPADO };
typedef enum Operacoes {
  INICIAR_PROCESSO = 1, FINALIZAR_PROCESSO, FRAGMENTACAO, RELATORIO,
  IMPRIME_SEMENTES, IMPRIME_PROCESSOS
};
typedef enum bool { false, true };

//Structs

struct No {
 	int tamanho;
 	struct No *dir, *esq, *pai;
 	Situacao estado;
};

typedef struct No No;


//Funções

int main() {
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