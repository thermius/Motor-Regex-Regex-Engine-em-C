/*Função que insere um item na fila*/
int Enfileirar (Fila **fila, void *ptr_valor,  size_t tamanho_do_dado, int tipo_de_dado, size_t tipo_de_enfileramento)
{

	if (fila == NULL ||tipo_de_enfileramento == ENFILEIRAR_DADO && tamanho_do_dado < 1 ) 			return -1;
	if (tipo_de_enfileramento == ENFILEIRAR_PONTEIRO && ptr_valor == NULL ) 						return -2;
	if (tipo_de_enfileramento != ENFILEIRAR_PONTEIRO && tipo_de_enfileramento != ENFILEIRAR_DADO) 	return -3;

	/*ponteiro para a memoria que será alocada caso efileiramento de dado*/
	void *ptr_novo_valor  = NULL;

	/*aloca memoria para o node da fila e testa*/
	NodeFila *ptr_novo_node = malloc (sizeof (NodeFila));
	if (ptr_novo_node == NULL)
	{
		#ifdef DEBUG_FILA
			printf("[ DEBUG ] - Enfileirar(): falha na alocação de memoria o node da fila\n");
		#endif
		return -1;
	}
	
	/*caso o enfileiramento seja do tipo ENFILEIRAR_DADO, aloca memoria para o elemento que será enfileirado e testa */
	if (tipo_de_enfileramento == ENFILEIRAR_DADO)
	{
		ptr_novo_valor = malloc (tamanho_do_dado);
		if (ptr_novo_valor == NULL)
		{
			#ifdef DEBUG_FILA
				printf("[ DEBUG ] - Enfileirar(): falha na alocação de memoria para o primeiro elemento da fila\n");
			#endif
			free (ptr_novo_node);
			return -1;
		}
		/*copia os bytes e preenche alguns campos*/
		memcpy (ptr_novo_valor, 	 	ptr_valor, tamanho_do_dado);
		ptr_novo_node->ptr_valor 		= ptr_novo_valor;
		ptr_novo_node->tamanho_do_dado	= tamanho_do_dado;
	}

	/*Primeira inserção*/
	if ( (*fila) == NULL)
	{

		/*aloca memoria para a nova fila e testa*/
		Fila *nova_fila = malloc (sizeof (Fila));
		if (nova_fila == NULL)
		{
			#ifdef DEBUG_FILA
				printf("[ DEBUG ] - Enfileirar(): falha na alocacao de memoria para a nova fila\n");
			#endif
			free (ptr_novo_node);
			if (tipo_de_enfileramento == ENFILEIRAR_DADO)	free (ptr_novo_valor);

		}

		/*aponta a fila para o inicio*/
		(*fila) = nova_fila;

		#ifdef DEBUG_FILA
			printf("[ DEBUG ] - Enfileirar(): primeira insercao\n");
		#endif

		/*preenche o node da fila*/
		ptr_novo_node->tipo_de_dado 			= tipo_de_dado;
		ptr_novo_node->tipo_de_enfileramento 	= tipo_de_enfileramento;
		ptr_novo_node->ptr_proximo 				= NULL;

		/*se enfileirar o ponteiro*/
		if (tipo_de_enfileramento == ENFILEIRAR_PONTEIRO)
		{
			ptr_novo_node->ptr_valor 		= ptr_valor;
			ptr_novo_node->tamanho_do_dado	= sizeof (void*);
		} 

		/*topo e final da fila são o mesmo*/
		(*fila)->ptr_inicio = ptr_novo_node;
		(*fila)->ptr_fim 	= ptr_novo_node;

		return 0;
	}

	/*Inserindo um elemento N*/
	#ifdef DEBUG_FILA
		printf("[ DEBUG ] - Enfileirar(): inserindo elemento N\n");
	#endif
	(*fila)->ptr_fim->ptr_proximo			= ptr_novo_node;
	ptr_novo_node->ptr_proximo 				= NULL;
	ptr_novo_node->tipo_de_dado 			= tipo_de_dado;
	ptr_novo_node->tipo_de_enfileramento 	= tipo_de_enfileramento;

	/*se enfileirar o ponteiro*/
	if (tipo_de_enfileramento == ENFILEIRAR_PONTEIRO)
	{
		ptr_novo_node->ptr_valor 		= ptr_valor;
		ptr_novo_node->tamanho_do_dado	= sizeof (void*);
	} 

	/*Altera o fim da fila*/
	(*fila)->ptr_fim = ptr_novo_node;
	return 0;

}
/*Remove um item do inicio da fila e libera a memoria. Retorna um ponteiro void para o elemento removido ou NULL*/
void *Desenfileirar (Fila **fila)
{	
	if(fila == NULL || (*fila) == NULL|| (*fila)->ptr_inicio == NULL) return NULL;

	/*salva o proximo inicio da fila*/
	void *proximo_elemento = (void*) (*fila)->ptr_inicio->ptr_proximo;
    
	/*obtem o ponteiro para o dado*/
	void *ptr_valor = (*fila)->ptr_inicio->ptr_valor;
	
	/*libera memoria do node*/
	free ((*fila)->ptr_inicio);

	/*altera o inicio da fila*/
	(*fila)->ptr_inicio = (NodeFila*) proximo_elemento;
	
	/*se vazia, anula o ponteiro para o fim*/
	if ((*fila)->ptr_inicio == NULL)
    {
        (*fila)->ptr_fim = NULL;
    }

	/*retona o ponteiro para o valor*/
	return ptr_valor;
}



/*Destroi a fila liberando memoria dos nodes e dos dados empilhados*/
void DestruirFila (Fila **fila)
{
	if (fila == NULL || (*fila) == NULL) return;

	NodeFila *node_corrente = (*fila)->ptr_inicio;
	NodeFila *temp;
	while (node_corrente)
	{
		temp = node_corrente->ptr_proximo;
		if (node_corrente->tipo_de_enfileramento == ENFILEIRAR_DADO) free (node_corrente->ptr_valor);
		free (node_corrente);
		node_corrente = temp;
	}
	free (*fila);
	(*fila) = NULL;

}

/*Retorna verdadeiro se a fila estiver vazia*/
int VazioFila(Fila *fila)
{
	if (fila == NULL || fila->ptr_inicio == NULL) return 1;
	return 0;
}
