#include "buffer_infinito.h"

/*Funcão que inicializa o buffer*/
int IniciarBuffer(BufferInfinito *novo_buffer)
{
	/*trava de segurança*/
	if (novo_buffer == NULL) 
	{
		#ifdef DEGUBAR_BUFFER_INFINITO
			if (debuga_buffer_infinito) printf ("[ nota ] - IniciarBuffer(): ponteiro nulo recebido com argumento. Requer ponteiro inicalizado.\n");
		#endif
		return -1;
	}

	novo_buffer->ptr_buffer = malloc (sizeof (char) * CAPACIDADE);
	if (novo_buffer->ptr_buffer == NULL) 
	{
		#ifdef DEGUBAR_BUFFER_INFINITO
			if (debuga_buffer_infinito) printf ("[ nota ] - IniciarBuffer(): falha na alocacao do buffer\n");
		#endif
		return -1;
	}
	novo_buffer->capacidade 		= sizeof (char) * CAPACIDADE;
	novo_buffer->tamanho			= 0;
	return 0;
}

/*Função que inicializa o novo buffer, ler a entrada e extende automaticamente enquanto for possivel. Retorna 0 se sucesso e -1 se erro, -2 se limite execido*/
int LerEntrada(BufferInfinito *buffer)
{
	/*trava de segurança*/
	if (buffer == NULL || buffer->ptr_buffer == NULL) return -1;
	int c = 0;					/*recebe o inteiro de getc*/
	int saida = 0;				/*condição de saida*/

	/*while que aumenta a capacidade*/
	while (saida != 1 && buffer->capacidade < LIMITE/2)
	{

		/*extende o buffer sempre dobrando a capacidade*/
		char *tmp = (char*) realloc ((void*)buffer->ptr_buffer, buffer->capacidade * 2);
 
		/*testa o ponteiro e aumenta a capacidade*/
		if (tmp) 
		{
			buffer->ptr_buffer = tmp;
			buffer->capacidade = buffer->capacidade * 2;
			#ifdef DEGUBAR_BUFFER_INFINITO
				if (debuga_buffer_infinito) printf ("[ nota ] - LerEntrada(): novo buffer alocado com sucesso. Nova capacidade: %u bytes\n", buffer->capacidade);
			#endif
		}
		else 
		{
			#ifdef DEGUBAR_BUFFER_INFINITO
				if (debuga_buffer_infinito) printf ("[ nota ] - LerEntrada(): falha na realocacao. \n");
			#endif
			return -1;
		}

		/*while que ler enquanto houver capacidade*/
		while (buffer->tamanho + 1 < buffer->capacidade && saida != 1)
		{
			c = getchar();
			if (c == EOF) 	
			{
				buffer->ptr_buffer[buffer->tamanho] = 0;
				#ifdef DEGUBAR_BUFFER_INFINITO
					if (debuga_buffer_infinito) printf ("[ nota ] - LerEntrada(): EOF recebido. Saindo do loop\n");
				#endif
				saida = 1; break;
			}
				buffer->ptr_buffer[buffer->tamanho] = (char)c; 						/*o tamanho atual é sempre a proxima posição vazia do buffer*/
				buffer->tamanho++;													/*avança o buffer*/
			if (buffer->ptr_buffer [buffer->tamanho - 1] == '\n') { buffer->tamanho--; saida = 1;}	/*entrada vazia indica fim da inserção (ENTER)*/
		}
		/*finaliza a string e retorna*/
		buffer->ptr_buffer[buffer->tamanho] = 0;
		if (saida) return 0;
	}
	
	/*se chegar aqui, o limite foi exedido e a memoria é liberada*/
	free (buffer->ptr_buffer);
	buffer->ptr_buffer = NULL;
	buffer->capacidade = 0;
	buffer->tamanho = 0;
	#ifdef DEGUBAR_BUFFER_INFINITO
		if (debuga_buffer_infinito) printf ("[ nota ] - LerEntrada(): limite de buffer exedido: %i :liberando memoria  e retornando\n",LIMITE);
	#endif
	return -2;


}

/*Retorna um ponteiro char para o buffer ou NULL*/
char *ObterPonteiroDoBuffer 	(BufferInfinito *buffer)
{
	return buffer->ptr_buffer;
}

/*Retorna o tamanho da string do buffer ou NULL*/
unsigned int ObterTamanhoDoBuffer (BufferInfinito *buffer)
{
	if (buffer == NULL) return 0;
	return buffer->tamanho;
}

/*Liberea memoria do buffer*/
void DestruirBuffer(BufferInfinito *buffer)
{
	if (buffer == NULL) return;
	free (buffer->ptr_buffer);
	buffer->capacidade = 0;
	buffer->tamanho = 0;
}
/*Ativa o debug para o buffer*/
#ifdef DEGUBAR_BUFFER_INFINITO
	int DebugarBufferInfinito (const int d)
	{
		debuga_buffer_infinito = d;	
	}
#endif
