#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilha.h"

#ifdef DEBUG_PILHA_H 
static int debug_pilha = 0;
#endif


/*Empilha um elemento. Permite empilhar tanto o ponteiro para o elemento quando uma copia dos bytes do elemento. Retorna 0 se sucesso e -1 se falha*/
int Empilhar ( Pilha **topo, void *ptr_valor,  size_t tamanho_do_dado, int tipo_de_dado, size_t tipo_de_empilhamento)
{
	/*Travas de segurança*/
	if	(ptr_valor == NULL) return -1;
	if	(tipo_de_empilhamento == EMPILHAR_DADO_APONTADO && tamanho_do_dado == 0) return -1;
	
	/*Aloca memoria para o elemento pilha*/
	Pilha *novo_elemento =  ( Pilha*) malloc (sizeof ( Pilha));
	if (!novo_elemento) 
	{
		#ifdef DEBUG_PILHA_H
			if (debug_pilha) printf ("[ DEBUG ] - Empilhar(): falha na alocacao de memoria com malloc para o novo elemento da pilha\n");
		#endif
		return -1;
	}
	

	/*Se empilhar o ponterio, copia direto o ponteiro para o poteiro para valor do node*/
	switch (tipo_de_empilhamento)
	{
		case EMPILHAR_PONTEIRO:
			#ifdef DEBUG_PILHA_H
			if (debug_pilha) printf ("[ DEBUG ] - Empilhar(): Operacao EMPILHAR_PONTEIRO infornada\n");
			#endif
			novo_elemento->ptr_valor = ptr_valor;
		break;

		case EMPILHAR_DADO_APONTADO:
			#ifdef DEBUG_PILHA_H
			if (debug_pilha) printf ("[ DEBUG ] - Empilhar(): Operacao EMPILHAR_DADO_APONTADO infornada\n");
			#endif
			novo_elemento->ptr_valor = malloc (tamanho_do_dado);
		break;

		default:
			#ifdef DEBUG_PILHA_H
			if (debug_pilha) printf ("[ DEBUG ] - Empilhar(): Operacao invalida infornada\n");
			#endif
			free (novo_elemento);
		return -1;

	}

	/*Testa se a alocação foi bem sucedida para o empilhamento de dados apontados*/
	if (!novo_elemento->ptr_valor && tipo_de_empilhamento == EMPILHAR_DADO_APONTADO)
	{
		#ifdef DEBUG_PILHA_H
			if (debug_pilha) printf ("[ DEBUG ] - Empilhar(): falha na alocacao de memoria para copia do dado\n");
		#endif
		free (novo_elemento);
		return -1;
	}
	
	/*Copia os dados que serão empilhados para a memoria alocada*/
	if (tipo_de_empilhamento == EMPILHAR_DADO_APONTADO)	memcpy (novo_elemento->ptr_valor, 	 ptr_valor, tamanho_do_dado);

	/*preenche os demais campos da struct*/
	novo_elemento->tipo_de_dado 		= tipo_de_dado;
	novo_elemento->tamanho_do_dado		= tamanho_do_dado;
	novo_elemento->tipo_de_empilhamento	= tipo_de_empilhamento;
	novo_elemento->ptr_anterior 		= *topo;
	*topo 								= novo_elemento;
	
	#ifdef DEBUG_PILHA_H
		if (debug_pilha) printf ("[ DEBUG ] - Empilhar(): empilhado com sucesso\n");
	#endif

	return 0;
}

/*Desempilha um elemento retornado o ponteiro void para o dado desempilhado*/
void *Desempilhar ( Pilha **topo)
{
	if (topo == NULL || *topo == NULL )  
	{
		#ifdef DEBUG_PILHA_H
			if (debug_pilha)  printf ("[ DEBUG ] - Desempilhar(): pilha vazia\n");
		#endif
		return NULL;
	}
	
	/*aponta o proximo topo*/
	 Pilha *proximo_topo = (*topo)->ptr_anterior;
	
	/*aponta para o dado que será desempilhado*/
	void *p = (*topo)->ptr_valor;
	
	/*libera o topo*/
	free (*topo);
	
	/*aponta para o proximo topo*/
	*topo = proximo_topo;
	
	/*retorna o ponteiro para o valor desempilhado*/
	return p;
}

/*Libera a copia dos dados empilhado. Não destroi ponteiros empilhados nem os nodes que os guardam. Retorna 0 se sucesso e -1 se erro*/
int LiberarDadosEmpilhados (Pilha **topo)
{

	if ((*topo) == NULL) return -1;

	#ifdef DEBUG_PILHA_H
		if (debug_pilha) printf ("[ DEBUG ] - LiberarDadosEmpilhados(): Certifique-se de que a memoria liberada. Sera liberado toda memoria exeto os ponteiros empilhados\n");
	#endif

	Pilha *temp 						= NULL;
	Pilha *elemento_a_ser_destruido		= (*topo);

	while (elemento_a_ser_destruido!= NULL)
	{
		/*Enquanto o elemento empilhado for um  ponteiro, desce na pilha enquanto temp persegue*/
        while (elemento_a_ser_destruido != NULL && elemento_a_ser_destruido->tipo_de_empilhamento == EMPILHAR_PONTEIRO)
		{
			temp = elemento_a_ser_destruido;
			elemento_a_ser_destruido = elemento_a_ser_destruido->ptr_anterior;
		}

		/*Se null, retorna*/
		if (elemento_a_ser_destruido == NULL ) return 0;

		/*Se o elemento a ser desutruido é o topo, então o topo deve descer*/
		if (elemento_a_ser_destruido == (*topo) && (*topo)->tipo_de_empilhamento == EMPILHAR_DADO_APONTADO)
		{
			/*Altera o topo*/
			(*topo) = elemento_a_ser_destruido->ptr_anterior;

			/*Libera as memoria do topo*/
			free (elemento_a_ser_destruido->ptr_valor);
			free (elemento_a_ser_destruido);
			#ifdef DEBUG_PILHA_H
				if (debug_pilha) printf ("[ DEBUG ] - LiberarDadosEmpilhados(): Memoria do topo liberada\n");
			#endif
			/*Aponta para o proximo topo e repete o algoritmo*/
			elemento_a_ser_destruido = (*topo);
			continue;
		}

		/*Se o elemento a ser destruido for um elemento n qualquer diferente do topo*/
		temp->ptr_anterior = elemento_a_ser_destruido->ptr_anterior;

		/*Libera as memoria do elemento n */
		free (elemento_a_ser_destruido->ptr_valor);
		free (elemento_a_ser_destruido);
		#ifdef DEBUG_PILHA_H
			if (debug_pilha) printf ("[ DEBUG ] - LiberarDadosEmpilhados(): Memoria de um elemento N liberada\n");
		#endif
		/*Aponta para o proximo elemento a ser destruido e repete o algoritmo*/
		elemento_a_ser_destruido = temp->ptr_anterior;
	}
		return 0;
}

/* Gerado pelo Gemini;
 * Destroi COMPLETAMENTE a pilha e todas as suas caixinhas de nós (Pilha*).
 *  Garante que os ponteiros puros fiquem intactos. */
void DestruirPilhaTotal (Pilha **topo)
{
	if (topo == NULL || (*topo) == NULL) return;

	#ifdef DEBUG_PILHA_H
	if (debug_pilha) printf ("[ DEBUG ] - DestruirPilhaTotal(): Iniciando destruicao total da estrutura...\n");
	#endif

	Pilha *atual = *topo;
	Pilha *proximo = NULL;

	while (atual != NULL)
	{
		// 1. Salva o link do nó anterior antes de explodir o nó atual
		proximo = atual->ptr_anterior;

		// 2. Se for cópia física de dados (DADO_APONTADO), limpamos o valor clonado!
		if (atual->tipo_de_empilhamento == EMPILHAR_DADO_APONTADO && atual->ptr_valor != NULL)
		{
			free (atual->ptr_valor);
			#ifdef DEBUG_PILHA_H
			if (debug_pilha) printf ("[ DEBUG ] - DestruirPilhaTotal(): Memoria do dado clonado liberada\n");
			#endif
		}
		// Se for EMPILHAR_PONTEIRO, o bloco acima é pulado. O Grafo da main continua vivo!

		// 3. AGORA SIM: Limpamos a caixinha do nó da pilha de QUALQUER JEITO!
		free (atual);
		#ifdef DEBUG_PILHA_H
		if (debug_pilha) printf ("[ DEBUG ] - DestruirPilhaTotal(): Estrutura do node Pilha* liberada\n");
		#endif

		// 4. Move para o próximo nó da descida
		atual = proximo;
	}

	// 5. Zera o topo original na main para evitar ponteiro fantasma (Dangling Pointer)
	*topo = NULL;
}


/*Retorna verdadeiro se a pilha estiver vazia e falso se não estiver*/
int Vazio  (Pilha *topo)
{
	if (topo == NULL)
	{
	#ifdef DEBUG_PILHA_H
	 if (debug_pilha)  printf ("[ DEBUG ] - Vazio(): pilha vazia\n");
	#endif
	return 1;
	}
	return 0;
}

/*Retorna o topo da pilha*/
void *Topo ( Pilha *topo)
{
	#ifdef DEBUG_PILHA_H
		if (topo == 0 && debug_pilha) printf ("[ DEBUG ] - Topo(): pilha vazia\n");
	#endif

	if (topo == 0) return 0;
	return topo->ptr_valor;
}


/*Ativa o debug para a pilha*/
#ifdef DEBUG_PILHA_H
int DebugPilha (int valor)
{
	debug_pilha = valor;
	return 1;
}
#endif
