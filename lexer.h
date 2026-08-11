#ifndef LEXER_H
#define LEXER_H

/*Tokens suportados*/
enum Tokens {
    ABERTURA_PARENTESES,
    FECHAMENTO_PARENTESES,
    ABERTURA_COLCHETES,
    FECHAMENTO_COLCHETES,
    OU,
    ESTRELA,
    MAIS,
    INTERROGACAO,
    CARACTERE,
    FIM
};

/*Estrutura que guarda o token*/
typedef struct _Token {
    enum Tokens tipo;
    char valor;
} Token;

/*Estrutura que guarda o estado do lexer*/
typedef struct _Lexer {
    const char *ptr_entrada;
    unsigned int posicao;
    Token token;
} Lexer;

/*Funções*/
int     InicializarLexer (Lexer *, char *);
void    ObterProximoToken(Lexer *);
void    DebugarLexer (Lexer );

#endif
