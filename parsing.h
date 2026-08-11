#ifndef PARSING_H
#define PARSING_H

/*Estrutura que guarda cada fragmento de grafo criado para um simbolo qualquer*/
typedef struct _Fragmento {
    Estado *inicio;
    Estado *fim;
} Fragmento;


/*API*/
Estado  *CriarAFN (Lexer *lexer, Estado **aceitacao, int *quant_estados);
Pilha   *FechamentoEpsilon (Estado *S, int *conjunto);
Pilha   *FechamentoMovimento ( Pilha **T, int alfabeto, int *conjunto);
int     DestruirGrafo ( Estado **partida, unsigned int quant_estados);
#endif
