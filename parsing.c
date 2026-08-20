#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "grafo.h"
#include "pilha.h"
#include "lexer.h"
#include "parsing.h"

/*Guarda o numero da sequencia do estado*/
int seq = 0;

/*Função que cria um AFN a partir de uma expressão regular. Retorna um ponteiro para o estado de partida ou null. REQUER TRATAR RETORNO DE PONTEIRO NULO*/
Estado *CriarAFN (Lexer *lexer, Estado **aceitacao, int *quant_estados)
{
    if (lexer == NULL) return NULL;

    /*Estrutura de dados utilizadas*/
    Pilha       *pilha_fragmentos   = NULL;

    /*Construção de thompson*/
    ObterProximoToken(lexer);
    while (lexer->token.tipo != FIM)
    {
        switch (lexer->token.tipo)
        {
            /*Cria o grafo para concatenção*/
            case ABERTURA_COLCHETES:
            {
                /*Obtem o proximo token*/
                ObterProximoToken(lexer);

                /*Estrutura de dados utilizadas*/
                Elo     elo;
                Fila    *fila_de_elos = NULL;

                /*concatena enquanto não houver ']'. a abertura e fechamento de escopos DEVE ser tratada antes de constuir o AFN*/
                while (lexer->token.tipo != FECHAMENTO_COLCHETES)
                {   
                    /*cria o grafo de forma na forma 'q0----a----NULL' que será chamado de elo*/
                    Estado      *q0     = CriarEstado(seq++);
                    Transicao   *t0     = CriarTransicao(lexer->token.valor, NULL);
                    AdicionarTransicao1 (q0, t0);
                    elo.estado          = q0;

                    /*enfileira o elo*/
                    Enfileirar (&fila_de_elos,&elo,sizeof(elo),0, ENFILEIRAR_DADO);

                    /*Obtem o proximo token*/
                    ObterProximoToken(lexer);

                }

                /*obtem o estado de partida da concatenação*/
                Elo     *elo_de_partida         = (Elo*) Desenfileirar (&fila_de_elos);
                Estado  *concatenacao_corrente  = elo_de_partida->estado;
                Elo     *elo_corrente           = NULL;

                /*enquanto a fila não estiver vazia, remove os elos e liga por meio de transições diretas, sem necessidade de EPSILON*/
                while (!VazioFila (fila_de_elos))
                {

                    /*remove um elo da fila*/
                    elo_corrente = (Elo*) Desenfileirar (&fila_de_elos);

                    /*liga a o estado de concatenação corrente ao elo corrente no formato 'q0----a----q1----b----q2---NULL'*/
                    LigarTransicao (concatenacao_corrente->transicao1, elo_corrente->estado);
                    concatenacao_corrente = elo_corrente->estado;
                    free (elo_corrente);
                }

                /*o ultimo estado sempre teŕa uma transição para NULL, criamos então qn para fechar o ciclo e formar o fragmento: 'q0----a----q1----b----q2---qn'*/
                Estado  *qn  = CriarEstado(seq++);
                LigarTransicao (concatenacao_corrente->transicao1, qn);

                /*Cria o fragmento e empilha*/
                Fragmento novo_fragmento;
                novo_fragmento.inicio   = elo_de_partida->estado;
                novo_fragmento.fim      = qn;
                Empilhar (&pilha_fragmentos, &novo_fragmento, sizeof(novo_fragmento), 0, EMPILHAR_DADO_APONTADO);

                /*libera a memoria*/
                DestruirFila (&fila_de_elos);
                free (elo_de_partida);

            } 
                break;

            /*Cria o grafo para caracteres*/
            case CARACTERE:
            {
                Estado      *q0 = NULL;
                Estado      *q1 = NULL;
                Transicao   *t0 = NULL;

                /*Cria os novos estados*/
                q0  = CriarEstado(seq++);
                q1  = CriarEstado(seq++);
                t0  = CriarTransicao(lexer->token.valor, q1);
                AdicionarTransicao1 (q0, t0);

                /*Gera o fragmento e empilha*/
                Fragmento fragmento;
                fragmento.inicio    =   q0;
                fragmento.fim       =   q1;
                Empilhar (&pilha_fragmentos,&fragmento, sizeof(fragmento),0, EMPILHAR_DADO_APONTADO);
                break;
            }

            /*Cria o grafo para estrela*/
            case ESTRELA:
            {
                Estado      *q0 = NULL;
                Estado      *q1 = NULL;
                Estado      *q2 = NULL;
                Transicao   *t0 = NULL;
                Transicao   *t1 = NULL;
                Transicao   *t2 = NULL;
                Transicao   *t3 = NULL;
                Fragmento   *f0 = (Fragmento*) Desempilhar  (&pilha_fragmentos);    if (f0 == NULL) return NULL;

                /*Cria os novos estados*/
                q0  = CriarEstado(seq++);
                q1  = CriarEstado(seq++);

                /*Cria as transições EPSILON orfãs*/
                t0  = CriarTransicao(EPSILON, NULL);
                t1  = CriarTransicao(EPSILON, NULL);
                t2  = CriarTransicao(EPSILON, NULL);
                t3  = CriarTransicao(EPSILON, NULL);

                /*Insere as transições nos estados*/
                AdicionarTransicao1(q0,t0);
                AdicionarTransicao2(q0,t1);
                AdicionarTransicao1(f0->fim,t2);
                AdicionarTransicao2(f0->fim,t3);

                /*Aponta as transições ao destino*/
                LigarTransicao (t0,f0->inicio);
                LigarTransicao (t1,q1);
                LigarTransicao (t2,f0->inicio);
                LigarTransicao (t3,q1);

                /*Cria o novo fragmento e empilha*/
                Fragmento novo_fragmento;
                novo_fragmento.inicio   = q0;
                novo_fragmento.fim      = q1;
                Empilhar (&pilha_fragmentos, &novo_fragmento, sizeof(novo_fragmento),0, EMPILHAR_DADO_APONTADO);
                free (f0);
                break;
            }

            /*Cria o grafo para OU*/
            case OU:
                Estado      *q0 = NULL;
                Estado      *q1 = NULL;
                Transicao   *t0 = NULL;
                Transicao   *t1 = NULL;
                Transicao   *t2 = NULL;
                Transicao   *t3 = NULL;
                Fragmento   *f0 = (Fragmento*) Desempilhar  (&pilha_fragmentos);    if (f0 == NULL) return NULL;
                Fragmento   *f1 = (Fragmento*) Desempilhar  (&pilha_fragmentos);    if (f1 == NULL) { free (f0); return NULL;}

                /*Cria os novos estados*/
                q0  = CriarEstado(seq++);
                q1  = CriarEstado(seq++);

                /*Cria as transições EPSILON orfãs*/
                t0  = CriarTransicao(EPSILON, NULL);
                t1  = CriarTransicao(EPSILON, NULL);
                t2  = CriarTransicao(EPSILON, NULL);
                t3  = CriarTransicao(EPSILON, NULL);

                /*Insere as transições no estado q0*/
                AdicionarTransicao1(q0, t0);
                AdicionarTransicao2(q0, t1);

                /*Aponta as transições de q0 ao destino*/
                LigarTransicao  (t0, f0->inicio);
                LigarTransicao  (t1, f1->inicio);

                AdicionarTransicao1(f0->fim, t2);
                AdicionarTransicao2(f1->fim, t3);

                /*Aponta as transições de f0 e f1 ao destino*/
                LigarTransicao  (t2, q1);
                LigarTransicao  (t3, q1);

                /*Cria o novo fragmento e empilha*/
                Fragmento novo_fragmento;
                novo_fragmento.inicio   = q0;
                novo_fragmento.fim      = q1;
                Empilhar (&pilha_fragmentos, &novo_fragmento, sizeof(novo_fragmento), 0, EMPILHAR_DADO_APONTADO);
                
                free (f0);
                free (f1);
                break;          
        }

        /*Obtem o proximo token*/
        ObterProximoToken(lexer);
    }

    /*Obtem o estado de aceitação*/
    Estado      *q0 = NULL;
    Transicao   *t0 = NULL;
    Fragmento   *f0 = (Fragmento*) Desempilhar  (&pilha_fragmentos);    if (f0 == NULL) return NULL;
    (*aceitacao)    = f0->fim;

    /*Obtem o estado de inicio do fragmento desempilhado*/
    Estado *partida = f0->inicio;

    /*Libera o fragmneto*/
    free(f0);

    /*Trata o que sobrar da pilha*/
    while (!Vazio (pilha_fragmentos))
    {
        /*Obtem um novo fragmento de estados*/
        f0  = (Fragmento*) Desempilhar  (&pilha_fragmentos);    if (f0 == NULL) return NULL;

        /*Cria uma transição epsilon nula*/
        t0  = CriarTransicao(EPSILON, NULL);                    if (t0 == NULL) { free(f0); return NULL; }

        /*Adciona a transiçao epslion ao estado fim de f0*/
        AdicionarTransicao1 (f0->fim,t0);

        /*Liga a transição epslion ao inicio do estado de partida correte de forma a ligar os estados restante por meio de transições epslion*/
        LigarTransicao      (t0,partida);

        /*O inicio do fragmento desempilhado se torda o estado de partida do proximo*/
        partida = f0->inicio;

        /*Libera o fragmento*/
        free(f0);
    }

    /*Destoi a pilha de fragmento*/
    DestruirPilhaTotal (&pilha_fragmentos);

    /*Escreve a quantidade de estados*/
    *quant_estados = seq;
    /*Retorna o estado de partida*/
    return partida;
}

/*Calcula o fechamento EPSILON de um estado S e retora uma pilha contendo o fecho ou NULL*/
Pilha *FechamentoEpsilon (Estado *S, int *conjunto)
{
    Pilha   *pilha_transicoes   = NULL;
    Pilha   *pilha_fechamento   = NULL;
    Estado  *t                  = NULL;
    Empilhar (&pilha_transicoes, S, sizeof(Estado),0, EMPILHAR_PONTEIRO);
    Empilhar (&pilha_fechamento, S, sizeof(Estado),0, EMPILHAR_PONTEIRO);

    /*Marca S no conjuto*/
    conjunto[S->id] = 1;

    while (!Vazio (pilha_transicoes))
    {
        /*Pega elemento para buscar por transições EPSILON*/
        t = Desempilhar (&pilha_transicoes);

        /*Verifica se a transição 1 é EPSILON*/
        if (CondicaoTransicao1 (t,EPSILON))
        {
            /*Pega o destino da transicao 1 de t*/
            Estado *destino1 = Destino1 (t);

            /*Verifica se o destino já está dentro do conjunto fechamento-e*/
            if (destino1 != NULL && conjunto [destino1->id] == 0)
            {
                /*Se não existir, insere no conjunto, marca e devolve para a pilha de transições*/
                Empilhar (&pilha_fechamento, destino1, sizeof(Estado),0, EMPILHAR_PONTEIRO);
                Empilhar (&pilha_transicoes, destino1, sizeof(Estado),0, EMPILHAR_PONTEIRO);
                conjunto [destino1->id] = 1;
            }
        }

        /*Verifica se a transição 2 é EPSILON*/
        if (CondicaoTransicao2 (t,EPSILON))
        {
            /*Pega o destino da transicao 1 de t*/
            Estado *destino2 = Destino2 (t);

            /*Verifica se o destino já está dentro do conjunto fechamento-e*/
            if (destino2 != NULL && conjunto [destino2->id] == 0)
            {
                /*Se não existir, insere no conjunto, marca e devolve para a pilha de transições*/
                Empilhar (&pilha_fechamento, destino2, sizeof(Estado),0, EMPILHAR_PONTEIRO);
                Empilhar (&pilha_transicoes, destino2, sizeof(Estado),0, EMPILHAR_PONTEIRO);
                conjunto [destino2->id] = 1;
            }
        }
    }

    /*Destroi a pilha*/
    DestruirPilhaTotal (&pilha_transicoes);

    /*Retorna uma pilha contendo todas as transição EPSILON atingiveis a partir de S*/
    return pilha_fechamento;
}

/*Calcular todos movimento possiveis para um conjunto T para um albabeto*/
Pilha *FechamentoMovimento ( Pilha **T, int alfabeto, int *conjunto)
{
    if (T == NULL || *(T) == NULL ) return NULL;
    
    Pilha   *pilha_copia_T    = NULL;
    Pilha   *pilha_movimento  = NULL;
    Estado  *S                = NULL;

    while (!Vazio (*T))
    {
        /*Pega um estado para buscar por transições do alfabeto*/
        S = Desempilhar (T);

        /*Verifica se a transição 1 é alfabeto*/
        if (CondicaoTransicao1 (S,alfabeto))
        {
            /*Pega o destino da transicao 1 de S*/
            Estado *destino1 = Destino1 (S);

            /*Verifica se o destino já está dentro do conjunto fechamento-movimento*/
            if (destino1 != NULL && conjunto [destino1->id] == 0)
            {
                /*Obtem o fechamento-e de destino 1*/
                Pilha *fecho = FechamentoEpsilon (destino1, conjunto);

                /*Despeja todos os estados do fecho-epsilon na pilha_movimento*/
                while (!Vazio(fecho))
                {
                    Estado *temp = (Estado*) Desempilhar(&fecho);
                    Empilhar(&pilha_movimento, temp, sizeof(Estado), 0, EMPILHAR_PONTEIRO);
                }
            }
        }

        /*Verifica se a transição 2 é alfabeto*/
        if (CondicaoTransicao2 (S,alfabeto))
        {
            /*Pega o destino da transicao 2 de S*/
            Estado *destino2 = Destino2 (S);

            /*Verifica se o destino já está dentro do conjunto fechamento-movimento*/
            if (destino2 != NULL && conjunto [destino2->id] == 0)
            {
                /*Obtem o fechamento-e de destino 2*/
                Pilha *fecho = FechamentoEpsilon (destino2, conjunto);

                /*Despeja todos os estados do fecho-epsilon na pilha_movimento*/
                while (!Vazio(fecho))
                {
                    Estado *temp = (Estado*) Desempilhar(&fecho);
                    Empilhar(&pilha_movimento, temp, sizeof(Estado), 0, EMPILHAR_PONTEIRO);
                }
            }
        }
        /*Empilha S na pilha de copia de T*/
        Empilhar (&pilha_copia_T, S , sizeof(Estado),0, EMPILHAR_PONTEIRO);
    }

    /*Aponta T para a copia*/
    (*T) = pilha_copia_T;
    return pilha_movimento;
}


/*Destroi o grafo*/
int DestruirGrafo ( Estado **partida, unsigned int quant_estados)
{
    if (partida == NULL || (*partida) == NULL) return -1;

    /*Estruturas utilizadas*/
    Pilha   *pilha_destruicao   = NULL;      
    Pilha   *pilha_visita       = NULL;
    int     conjunto_visitados[quant_estados];

    /*Limpa o conjunto*/
    for (int i = 0; i < quant_estados; i++) conjunto_visitados[i] = 0;

    /*Empilha o estado de partida*/
    Empilhar (&pilha_visita, *partida, sizeof(void*), 0, EMPILHAR_PONTEIRO);

    /*Loop que percorre o grafo*/
    while (!Vazio(pilha_visita))
    {

        /*Obtem um estado s para visita*/
        Estado *s = (Estado*) Desempilhar(&pilha_visita);
        if (s == NULL) continue;

        /*Empilha o estado atual para destruiçao*/
        Empilhar (&pilha_destruicao, s, sizeof(void*), 0, EMPILHAR_PONTEIRO);

        /*Verifica se há transição 1 em s e empilha*/
        if (s->transicao1 != NULL) 
        {
                /*Empilha a transição para destruiçao*/
                Empilhar (&pilha_destruicao, s->transicao1, sizeof(void*), 0, EMPILHAR_PONTEIRO);

               /*Verifica se há destino 1 em s e empilha se for novo */
                if (s->transicao1->destino != NULL && conjunto_visitados[s->transicao1->destino->id] == 0)
                { 
                    conjunto_visitados[s->transicao1->destino->id] = 1;
                    Empilhar (&pilha_visita, s->transicao1->destino, sizeof(void*), 0, EMPILHAR_PONTEIRO);
                }
        }
        /*Verifica se há transição 2 em s e empilha*/
        if (s->transicao2 != NULL) 
        {
                /*Empilha a transição para destruiçao*/
                Empilhar (&pilha_destruicao, s->transicao2, sizeof(void*), 0, EMPILHAR_PONTEIRO);

               /*Verifica se há destino 1 em s e empilha se for novo */
                if (s->transicao2->destino != NULL && conjunto_visitados[s->id] == 0)
                { 
                    conjunto_visitados[s->transicao2->destino->id] = 1;
                    Empilhar (&pilha_visita, s->transicao2->destino, sizeof(void*), 0, EMPILHAR_PONTEIRO);
                }
        }
  
    }

    /*Libera a memoria*/
    while (!Vazio(pilha_destruicao))
    {
        void *ptr = Desempilhar(&pilha_destruicao);
        if ( ptr != NULL) 
        {
            free(ptr);
        }
    }

    *partida = NULL;
    return 1;
}

/* Função auxiliar para formatar a condição de transição */
static const char* FormatarCondicao(int cond, char *buffer)
{
    if (cond == EPSILON)
    {
        return "EPSILON";
    }
    if (cond >= 32 && cond <= 126) // Caracteres ASCII imprimíveis
    {
        sprintf(buffer, "'%c'", (char)cond);
        return buffer;
    }
    sprintf(buffer, "0x%02X", cond);
    return buffer;
}

/* Worker recursivo de busca em profundidade */
static void ImprimirGrafoRecursivo(Estado *estado, int *visitados)
{
    if (estado == NULL || visitados[estado->id])
    {
        return;
    }

    // Marca estado como visitado para evitar loops infinitos no fecho de Kleene
    visitados[estado->id] = 1;

    char buf[16];

    printf("Estado ID: %d | Descricao: \"%s\"\n", 
           estado->id, 
           estado->descricao[0] ? estado->descricao : "N/A");

    // Transição 1
    if (estado->transicao1 != NULL && estado->transicao1->destino != NULL)
    {
        printf("  ├── Transicao 1: %-8s ──> Estado %d\n",
               FormatarCondicao(estado->transicao1->condicao, buf),
               estado->transicao1->destino->id);
    }

    // Transição 2
    if (estado->transicao2 != NULL && estado->transicao2->destino != NULL)
    {
        printf("  └── Transicao 2: %-8s ──> Estado %d\n",
               FormatarCondicao(estado->transicao2->condicao, buf),
               estado->transicao2->destino->id);
    }

    if (estado->transicao1 == NULL && estado->transicao2 == NULL)
    {
        printf("  └── (Estado Final / Sem Saidas)\n");
    }

    printf("\n");

    // Recurso para os estados adjacentes
    if (estado->transicao1 != NULL && estado->transicao1->destino != NULL)
    {
        ImprimirGrafoRecursivo(estado->transicao1->destino, visitados);
    }
    if (estado->transicao2 != NULL && estado->transicao2->destino != NULL)
    {
        ImprimirGrafoRecursivo(estado->transicao2->destino, visitados);
    }
}

/* Função principal a ser chamada no seu código */
void ExibirGrafoAFN(Estado *inicio, int total_estados)
{
    if (inicio == NULL)
    {
        printf("Grafo vazio (Estado inicial nulo).\n");
        return;
    }

    // Aloca vetor para controlar visitação e evitar recursão infinita
    int *visitados = (int *) calloc(total_estados, sizeof(int));
    if (visitados == NULL)
    {
        fprintf(stderr, "Erro de alocacao de memoria no ExibirGrafoAFN.\n");
        return;
    }

    printf("========================================\n");
    printf("         ESTRUTURA DO GRAFO AFN         \n");
    printf("========================================\n\n");

    ImprimirGrafoRecursivo(inicio, visitados);

    printf("========================================\n");

    free(visitados);
}
