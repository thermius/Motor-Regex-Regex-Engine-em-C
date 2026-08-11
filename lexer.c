    #include <stdio.h>
#include "lexer.h"
/*Função que inicializa o laxer. Retorna 0 se sucesso e -1 se erro*/
int InicializarLexer (Lexer * lexer, char *ptr_entrada)
{
    if (lexer == NULL || ptr_entrada == NULL) return -1;

    lexer->ptr_entrada  = ptr_entrada;
    lexer->posicao      = 0;
    lexer->token.valor  = 0;
    lexer->token.tipo   = FIM;
    return  0;

}

/* Função que identifica o próximo token */
void ObterProximoToken(Lexer *lexer)
{
    switch (lexer->ptr_entrada[lexer->posicao])
    {
        case '(':
            lexer->token.tipo  = ABERTURA_PARENTESES;
            lexer->token.valor = '(';
            break;

        case ')':
            lexer->token.tipo  = FECHAMENTO_PARENTESES;
            lexer->token.valor = ')';
            break;

        case '[':
            lexer->token.tipo  = ABERTURA_COLCHETES;
            lexer->token.valor = '[';
            break;

        case ']':
            lexer->token.tipo  = FECHAMENTO_COLCHETES;
            lexer->token.valor = ']';
            break;

        case '|':
            lexer->token.tipo  = OU;
            lexer->token.valor = '|';
            break;

        case '*':
            lexer->token.tipo  = ESTRELA;
            lexer->token.valor = '*';
            break;

        case '+':
            lexer->token.tipo  = MAIS;
            lexer->token.valor = '+';
            break;

        case '?':
            lexer->token.tipo  = INTERROGACAO;
            lexer->token.valor = '?';
            break;

        case '\0':
            lexer->token.tipo  = FIM;
            lexer->token.valor = '\0';
            break;

        default:
            lexer->token.tipo  = CARACTERE;
            lexer->token.valor = lexer->ptr_entrada[lexer->posicao];
            break;
    }

    lexer->posicao++;
}

/*Função que debuga o lexer*/
void DebugarLexer (Lexer lexer)
{
    int temp = lexer.posicao;
    do
    {
        ObterProximoToken (&lexer);
        switch (lexer.token.tipo)
        {
            case ABERTURA_PARENTESES:
                printf("ABERTURA_PARENTESES\t'%c'\n", lexer.token.valor);
                break;

            case FECHAMENTO_PARENTESES:
                printf("FECHAMENTO_PARENTESES\t'%c'\n", lexer.token.valor);
                break;

            case ABERTURA_COLCHETES:
                printf("ABERTURA_COLCHETES\t'%c'\n", lexer.token.valor);
                break;

            case FECHAMENTO_COLCHETES:
                printf("FECHAMENTO_COLCHETES\t'%c'\n", lexer.token.valor);
                break;

            case OU:
                printf("OU\t\t\t'%c'\n", lexer.token.valor);
                break;

            case ESTRELA:
                printf("ESTRELA\t\t\t'%c'\n", lexer.token.valor);
                break;

            case MAIS:
                printf("MAIS\t\t\t'%c'\n", lexer.token.valor);
                break;

            case INTERROGACAO:
                printf("INTERROGACAO\t\t'%c'\n", lexer.token.valor);
                break;

            case CARACTERE:
                printf("CARACTERE\t\t'%c'\n", lexer.token.valor);
                break;

            case FIM:
                printf("FIM\n");
                break;

            default:
                printf("TOKEN DESCONHECIDO\t'%c'\n", lexer.token.valor);
                break;
        }
    }while (lexer.token.tipo != FIM);

    lexer.posicao = temp;

}
