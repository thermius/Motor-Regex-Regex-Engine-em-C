#ifndef PILHA_H
#define PILHA_H
#define DEBUG_PILHA_H
#define EMPILHAR_PONTEIRO 1
#define EMPILHAR_DADO_APONTADO 2

typedef struct _Pilha 
{
	struct		_Pilha *ptr_anterior;
	int 		tipo_de_dado;
	int 		tamanho_do_dado;
	size_t 		tipo_de_empilhamento;
	void 		*ptr_valor;

} Pilha;

void 		TutorialPilha		();
int			Empilhar 			( Pilha**, void*, size_t, int, size_t);
int 	 	Vazio 				( Pilha* );
void 		InverterPilha		( Pilha**);
void 		*Desempilhar 		( Pilha**);
void	 	*Topo				( Pilha* );
int			LiberarDadosEmpilhados 		(Pilha **);
void 		DestruirPilhaTotal 	(Pilha **topo);



#ifdef DEBUG_PILHA_H
	int 	DebugPilha	(int);
	void 	ExibirPilha	(Pilha*, char *);
#endif
#endif
