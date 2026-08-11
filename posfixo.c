#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "posfixo.h"
#include "pilha.h"

/*Converte uma string infixa para posfixa. Retorna um ponteiro para string posfixa ou NULL*/
char *ConverterParaPosfixo (char *entrada, unsigned int tamanho_string)
{
	/*Aloca memoria para a saida posfixada e  testa o ponteiro*/
	char *ptr_saida	= malloc (tamanho_string + 1);
	if (ptr_saida == NULL) return NULL;

	/*Pilha que guarda os operadores*/
	Pilha 	*pilha_operadores = NULL;

	/*Index que aponta para a string de entrada e saida*/
	int 	index_entrada 		= 0;
	int 	index_saida 		= 0;

	/*Loop que faz a convesão*/
	while (entrada [index_entrada])
	{
		/*Se for caractere alfanumerico, vai direto para saida*/
		if (isalnum (entrada[index_entrada]))
		{
 			ptr_saida [index_saida++] = entrada [index_entrada++];
 			continue;
		}

		if (entrada[index_entrada] == '*')
		{
		    ptr_saida[index_saida++] = entrada[index_entrada++];
		    continue;
		}
		/*Se abertura, vai direto para a pilha*/
		if (entrada[index_entrada] == '(')
		{
			Empilhar (&pilha_operadores, &entrada[index_entrada], sizeof (char),0, EMPILHAR_DADO_APONTADO);
			index_entrada++;
			continue;
		}

		/*Se fechamento, desempilha tudo e joga na saida enquanto não for vazio ou '('*/
		if (entrada[index_entrada] == ')')
		{
			char elemento = 0;
			while (!Vazio (pilha_operadores) && *(char*)Topo (pilha_operadores)!= '(')
			{
				elemento = *(char*) Desempilhar (&pilha_operadores);
				ptr_saida [index_saida++] = elemento;
			}

			/* Remove o '(' da pilha*/
			Desempilhar (&pilha_operadores);

			/*Avança  a entrada*/
			index_entrada++;
			continue;
		}

		/*Se qualquer operador regex*/
		else
		{	
			/*Enquanto a procedencia do topo da pilha for maior ou igual, desempilha o topo e insere na saida*/
			while ( Topo (pilha_operadores) != NULL && Procedencia( *(char*) Topo (pilha_operadores)) >= Procedencia ( entrada[index_entrada]))
			{
				char elemento = *(char*) Desempilhar (&pilha_operadores);
				ptr_saida [index_saida++] = elemento;

			}
			/* Empilha o operador atual no momento em que sua procedencia for menor que o topo*/
	        Empilhar (&pilha_operadores, &entrada[index_entrada], sizeof(char), 0, EMPILHAR_DADO_APONTADO);
	        index_entrada++;
	        continue;
		}


    }

	/*Ao fim da string, esvazia a pilha de operadores*/
	while (!Vazio(pilha_operadores))
	{
		char elemento = *(char*) Desempilhar (&pilha_operadores);
		ptr_saida [index_saida++] = elemento;
	}

	/*Encerra a string e retorna*/
	ptr_saida[index_saida] = '\0';
	return ptr_saida;
}

/*Retorna um inteiro de procedencia dos operadores regex. Quanto mais alta a procedencia, maior é o valor retornado*/
int Procedencia(char operador)
{
    switch (operador)
    {
    	/*Maiores procedencias*/
        case '*':
            return 2;

       	/*OU*/
        case '|':
            return 1; 

        /*Qualquer outro*/
        default:
            return 0;
    }
}