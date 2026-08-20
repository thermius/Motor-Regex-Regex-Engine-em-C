#ifndef FILA_H
#define FILA_H
#define ENFILEIRAR_DADO 1
#define ENFILEIRAR_PONTEIRO 2
//#define DEBUG_FILA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _NodeFila 
{
	int 				tipo_de_dado;
	int 				tamanho_do_dado;
	size_t 				tipo_de_enfileramento;
	void 				*ptr_valor;
	struct _NodeFila 	*ptr_proximo;

} NodeFila;

typedef struct _Fila
{
	NodeFila *ptr_inicio;
	NodeFila *ptr_fim;
} Fila;

/*apis*/
int 	Enfileirar 		(Fila **, void *,  size_t , int , size_t);
void 	*Desenfileirar 	(Fila **);
void 	DestruirFila 	(Fila **);
int 	VazioFila		(Fila*);

#include "fila.c"
#endif