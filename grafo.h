#ifndef EPSILON
	#define EPSILON -1
#endif

#ifndef GRAFO_H 
	#define GRAFO_H
	#define DEBUG_GRAFO
	#define MAX_ARGUMENTOS 8
	#define	MAX_SEMANTICAS 8
	/*declaracoes*/
	typedef struct _Estado Estado;
	typedef struct _Transicao Transicao;
	typedef int (*AcaoSemantica)(void *);
	struct _Transicao
	{
		int 	condicao;
		Estado *destino;
	};
	struct _Estado
	{
		int id;
		char descricao [16];
		AcaoSemantica acoes[MAX_SEMANTICAS];
		Transicao *transicao1;
		Transicao *transicao2;
	};

	#ifdef DEBUG_GRAFO

	#endif

	/*API*/
Estado *CriarEstado (int sequencia);
Transicao *CriarTransicao (int condicao, Estado *destino);
Estado *AdicionarTransicao1 (Estado *estado, Transicao *transicao);
Estado *AdicionarTransicao2 (Estado *estado, Transicao *transicao);
int CondicaoTransicao1 (Estado *S, int condicao);
int CondicaoTransicao2 (Estado *S, int condicao);
Transicao *ObterPonteiroTransicao1 (Estado *S);
Transicao *ObterPonteiroTransicao2 (Estado *S);
Estado *Destino1 (Estado *S);
Estado *Destino2 (Estado *S);
Transicao *LigarTransicao (Transicao *transicao, Estado *destino);
Estado *AdicionarAcaoSemantica   (Estado *estado,int index, int (*Acao) (void *argumentos));
#endif

