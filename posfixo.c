#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "posfixo.h"

/*Converte uma string infixa para posfixa. Retorna um ponteiro para string posfixa ou NULL*/
char *ConverterRegexParaPosfixo (char *entrada)
{
	/*calcula o tamanho da string*/
	int index = 0;
	while (entrada [index++]);

	/*aloca memoria para a string posfixada e zera o index*/	
	char *saida = malloc (index);
	if (saida == NULL) return NULL;
	index = 0;

	/*estrutura da dados utilizadas*/
	Pilha *operadores = NULL;

	/*loop que avalia a entrada*/
	int posicao_saida = 0;
	while (entrada[index])
	{

		switch (entrada [index])
		{

		/*se parenteses vai direto para a pilha*/
		case '(':
			Empilhar (&operadores, &entrada[index], sizeof (char), 0 , EMPILHAR_PONTEIRO);
			break;

		/*se fechamento de paranteses, desempilha tudo e joga na saida até encontrar o parenteses de abertura*/
		case ')':
			char *ptr_elemento = NULL;
			while (!Vazio(operadores))
			{
				ptr_elemento = (char*) Desempilhar (&operadores);
				if (*ptr_elemento == '(') break;
				saida[posicao_saida++] = *ptr_elemento;
			}
			break;

		case '*':
		case '+':
		case '|':
			/*avalia a procedencia de forma a garantir que no topo sempre esteja o elemento de maior procedencia*/
			while (!Vazio(operadores))
			{
				/*obtem o operador do topo da pilha sem desempilhar*/
				char *ptr_elemento = (char*) Topo (operadores);

				/*se o elemento do topo tiver procedencia maior ou igual a entrada, o elemento do topo deve ir primeiro para a saida*/
				if ( AvaliarProcedencia (*ptr_elemento) >= AvaliarProcedencia (entrada[index]) )
				{
					/*pega o elemento do topo e insere na saida*/
					ptr_elemento = (char*) Desempilhar (&operadores);
					saida[posicao_saida++] = *ptr_elemento;
				}
				/*se não houver procedencia, interrompe o loop para evitar o algoritmo zerar a pilha indevidamente*/
				else break;
				
			}
			
			/*se a procedencia não for maior, alarme falso, insere para a pilha*/
			Empilhar (&operadores, &entrada[index], sizeof (char), 0 , EMPILHAR_PONTEIRO);
			break;

			/*qualquer outra coisa é enviada diretamente a saida*/
			default:
				saida[posicao_saida++] = entrada[index];
				break;
		}
		index++;
	}

	/*o que sobra da pilha é enviado diretamente a saida*/
	while (!Vazio (operadores)) 
	{
		/*desempilha um elemento*/
		char *ptr_elemento = (char*) Desempilhar (&operadores);
		/*se null, encerra*/
		if (ptr_elemento == NULL) break;
		/*joga na saida e avança o index*/
		saida[posicao_saida++] = *ptr_elemento;
	}

	/*finaliza a saida com 0*/
	saida[posicao_saida] = 0;
	
	/*Destroi a pilha de operadores*/
	DestruirPilhaTotal(&operadores);

	/*retorna o ponteiro para a saida posfixada*/
	return saida;
}

/*Retorna um inteiro de procedencia dos operadores regex. Quanto mais alta a procedencia, maior é o valor retornado*/
int AvaliarProcedencia(char operador)
{
    switch (operador)
    {
    	/*Maiores procedencias*/
        case '*':
		case '+':
            return 2;

       	/*OU*/
        case '|':
            return 1; 

        /*Qualquer outro*/
        default:
            return 0;
    }
}
