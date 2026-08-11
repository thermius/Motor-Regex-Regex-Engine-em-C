#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

/*Criar um estado com transições nulas e retorna um ponteiro para ele ou NULL com transições nulas.*/
Estado *CriarEstado (int sequencia)
{
    Estado *novo_estado = malloc (sizeof(Estado));
    if (novo_estado == NULL) return NULL;
    novo_estado->id             = sequencia++;
    novo_estado->transicao1     = NULL;
    novo_estado->transicao2     = NULL;
    novo_estado->descricao[0]   = 'n';
    novo_estado->descricao[1]   = '/';
    novo_estado->descricao[2]   = 'a';
    novo_estado->descricao[3]   = 0;

    /*Inicializa as ações semanticas com NULL*/
    for (int i = 0; i < MAX_SEMANTICAS; i++) novo_estado->acoes[i] = NULL;
    return novo_estado;
}

/*Cria uma transição com destino  retorna um ponteiro para ela ou NULL*/
Transicao *CriarTransicao (int condicao, Estado *destino)
{
    Transicao *nova_transicao = malloc (sizeof (Transicao));
    if (nova_transicao == NULL) return NULL;
    nova_transicao->condicao    = condicao;
    nova_transicao->destino     = destino;
    return nova_transicao;
}

/*Adiciona uma saida 1 ao estado. Retorna um ponteiro para o propio estado ou NULL*/
Estado *AdicionarTransicao1 (Estado *estado, Transicao *transicao)
{
    if (estado == NULL || transicao == NULL) return NULL;
    estado->transicao1 = transicao;
    return estado;
}

/*Adiciona uma saida 2 ao estado. Retorna um ponteiro para o propio estado ou NULL*/
Estado *AdicionarTransicao2 (Estado *estado, Transicao *transicao)
{
    if (estado == NULL || transicao == NULL) return NULL;
    estado->transicao2 = transicao;
    return estado;
}


/*Retorna verdadeiro se houver condição de seguir pela transição 1*/
int CondicaoTransicao1 (Estado *S, int condicao)
{
    if (S == NULL|| S->transicao1 == NULL) return 0;
    if (S->transicao1->condicao == condicao) return 1;
    return 0;
}

/*Retorna verdadeiro se houver condição de seguir pela transição 2*/
int CondicaoTransicao2 (Estado *S, int condicao)
{
    if (S == NULL || S->transicao2 == NULL) return 0;
    if (S->transicao2->condicao == condicao) return 1;
    return 0;
}

/*Retorna o ponteiro para transição 1 de um estado S ou NULL*/
Transicao *ObterPonteiroTransicao1 (Estado *S)
{
    if (S == NULL|| S->transicao1 == NULL) return NULL;
    return S->transicao1;
}

/*Retorna o ponteiro para transição 2 de um estado S ou NULL*/
Transicao *ObterPonteiroTransicao2 (Estado *S)
{
    if (S == NULL|| S->transicao2 == NULL) return NULL;
    return S->transicao2;
}

/*Retorna o ponteiro para o destino 1 de uma transicao de um estado S ou NULL*/
Estado *Destino1 (Estado *S)
{
    if (S == NULL || S->transicao1 == NULL ||S->transicao1->destino == NULL) return NULL;
    return S->transicao1->destino;
}

/*Retorna o ponteiro para o destino 2 de uma transicao de um estado S ou NULL*/
Estado *Destino2 (Estado *S)
{
    if (S == NULL || S->transicao2 == NULL ||S->transicao2->destino == NULL) return NULL;
    return S->transicao2->destino;
}

/*Liga uma transição a um estado*/
Transicao *LigarTransicao (Transicao *transicao, Estado *destino)
{
    if (transicao == NULL)  return NULL;
    transicao->destino = destino;
    return transicao;
}

/*Adiciona uma ação semantica ao Estado*/
Estado *AdicionarAcaoSemantica   (Estado *estado,int index, int (*Acao) (void *argumentos))
{
    if (estado == NULL || Acao == NULL) return NULL;
    if (index < 0 || index >= MAX_SEMANTICAS) return NULL;
    estado->acoes[index] = Acao;
    return estado;
}


