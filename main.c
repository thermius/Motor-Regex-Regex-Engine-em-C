#include <stdio.h>
#include <stdlib.h>
#include "buffer_infinito.c"
#include "pilha.c"
#include "parsing.c"
#include "grafo.c"
#include "lexer.c"
#include "posfixo.c"


int main(int argc, char *argv[])
{
    Pilha           *fechameto = NULL;
    Pilha           *movimento = NULL;
    Estado          *aceitacao = NULL;
    Estado          *partida   = NULL;

    Lexer           lexer;
    BufferInfinito  buffer;
    BufferInfinito  buffer2;

    char *regex_posfixado = NULL;
    char *string = NULL;
    int *conjunto = NULL;
    int quant_estados = 0;

    /*Inicializa o buffer infinito*/
    IniciarBuffer(&buffer);
    IniciarBuffer(&buffer2);

    /*Pede a string que sera analisada e pega o ponteiro*/
    printf ("Informe a string que o regex ira analisar\n");
    LerEntrada(&buffer2);
    string = ObterPonteiroDoBuffer(&buffer2);

    /*Pede o regex*/
    printf ("Informe o regex a ser utilizado (suporta qualquer alfabeto ancii, * e | e concatenacao por meio de '[' ']')\n");
    LerEntrada(&buffer);
    printf("\n");

    /*Converte o regex para posfixo*/
    regex_posfixado = ConverterRegexParaPosfixo(ObterPonteiroDoBuffer(&buffer));

    InicializarLexer(&lexer, regex_posfixado);
    printf("[ NOTA ] - main(): regex postfixado: %s\n",regex_posfixado);

    /*Cria o AFN*/
    partida   = CriarAFN(&lexer, &aceitacao, &quant_estados);

    /*Aloca memoria para o conjunto*/
    conjunto = malloc (quant_estados * sizeof (int));

    if (conjunto == NULL)
    {
    	printf ("[ ERROR ] - main(): Falha na alocacao de memoria\n");
    	DestruirGrafo (&partida,quant_estados);
    	return 0;
    }

    /*Limpa a marcação dos conjunto*/
    for (int j = 0; j < quant_estados; j++) conjunto [j] = 0;

    /*Obtem o fechamento-e de s0*/
    fechameto = FechamentoEpsilon(partida, conjunto);

    /*Motor regex*/
    int  i = 0;
    while (string[i] != '\0' && string[i] != '\n' && string[i] != '\r')    
    {

        /*Limpa a marcação dos conjunto*/
        for (int j = 0; j < quant_estados; j++) conjunto [j] = 0;

        /*Obtem os movimentos atingiveis por meio do alfabeto*/
        movimento = FechamentoMovimento(&fechameto, string[i], conjunto);

        /*Se for possivel alçancar algum estado, da match e exibe*/
        if (movimento != NULL && !Vazio(movimento)) printf("\033[1;32m%c\033[0m", string[i]);

        /*Se não for possivel alçancar algum estado, encerra*/
        else { printf("\033[1;31m\nFalhou no caractere '%c'\033[0m\n", string[i]); DestruirPilhaTotal(&movimento); break; }

        /*Recebe o  conjunto de todos os estados antingidos pelo alfabeto anterior, incluindo estados atingidos por transições epsilon, para buscar pela proximo alfabeto*/
        DestruirPilhaTotal (&fechameto);
        fechameto = movimento;
        movimento = NULL;
        i++;
    }

    /*Verifica se casou. Trecho gerado por IA. 
    Ele desempilha o ultimo conjunto de estados antingido e verifica se o ID é igual ao ID do estado de aceitação*/
    int casou = 0;
    if (string[i] == '\0')
    {
        while (!Vazio(fechameto))
        {
            Estado *S = (Estado*) Desempilhar(&fechameto);
            if (S != NULL && aceitacao != NULL && S->id == aceitacao->id)
            {
                casou = 1;
                break;
            }
        }
    }

    /* 5. Veredito Final */
    printf("\n=================================\n");
    if (casou)
    {
         printf("  ✅ RECOGNIZED (Match Completo!)\n");
    }

    else
    {
        printf("  ❌ REJECTED (String nao casa)\n");
    }
         printf("=================================\n");

    /*Libera todas as memorias*/
    free (conjunto);
    free (regex_posfixado);
    DestruirBuffer (&buffer);
    DestruirBuffer (&buffer2);
    DestruirPilhaTotal (&fechameto);
    DestruirPilhaTotal (&movimento);
    DestruirGrafo (&partida,quant_estados);
    return 0;
}
