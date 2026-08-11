#ifndef BUFFER_INFINITO_H
#define BUFFER_INFINITO_H
#define CAPACIDADE 0xFF
#define DEGUBAR_BUFFER_INFINITO 1
#define LIMITE	0xFFFF
#include <stdio.h>
#include <stdlib.h>

	typedef struct _BufferInfinito
	{
	char	*ptr_buffer;
	unsigned int tamanho;
	unsigned int capacidade;
	} BufferInfinito;

	#ifdef DEGUBAR_BUFFER_INFINITO
		static int debuga_buffer_infinito = 0;
	#endif

	int 			IniciarBuffer			(BufferInfinito *);
	int 			LerEntrada				(BufferInfinito *);
	char			*ObterPonteiroDoBuffer 	(BufferInfinito *);
	unsigned int 	ObterTamanhoDoBuffer 	(BufferInfinito *);
	void 			DestruirBuffer			(BufferInfinito *buffer);

#endif
