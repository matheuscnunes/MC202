#include <stdio.h>
#include <stdlib.h>

/*
 * Nomes: Matheus Cezar Nunes , Mateus Freitas Silveira
 * RA: 203373 e 203263
 *
 */

//Enuns
typedef enum Operacoes {
  
} Operacoes;
typedef enum bool { false, true } bool;

//Structs
struct No {
 	struct No *dir, *esq, *pai;
};

typedef struct No No;
typedef struct Programa Programa;

//Esqueletos

int main() {

	int op;
	// Le as operações do sistema
	while (scanf("%d", &op) != EOF) {
		switch (op) {
		  default:
		    printf("Operação não cadastrada!\n");
		  break;
		}
	}

	// Liberar memoria
}
