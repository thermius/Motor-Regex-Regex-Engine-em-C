# Motor de Expressões Regulares em C

Motor de expressões regulares desenvolvido em **C**, baseado na **Construção de Thompson para Autômatos Finitos Não-Determinísticos (AFN)**.

O projeto implementa, em baixo nível, o processo de transformação de uma expressão regular em uma estrutura de **AFN composta por estados e transições**, incluindo transições $\epsilon$ e composição de fragmentos através de estruturas de dados dinâmicas.

A arquitetura e os algoritmos foram projetados com base nos conceitos clássicos apresentados no livro **"Compiladores: Princípios, Técnicas e Ferramentas"** (*Dragon Book* - Aho, Sethi, Ullman & Lam).

A implementação foi desenvolvida sem a utilização de bibliotecas externas de expressões regulares, permitindo controlar diretamente a construção do autômato, suas estruturas de dados e o gerenciamento de memória.

O projeto tem como foco a aplicação prática de conceitos de **Teoria de Linguagens, Autômatos, Estruturas de Dados, Algoritmos e Programação de Baixo Nível**.

---

## 📌 Características

- Desenvolvido integralmente em **C**.
- Construção de **Autômato Finito Não-Determinístico (AFN)** baseada na Construção de Thompson.
- Suporte a qualquer caractere do alfabeto representável por `char`.
- Suporte ao operador `*` — Fechamento de Kleene.
- Suporte ao operador `|` — Alternância (OU).
- Suporte a alternância de procedência por meio de parenteses `()`.
- Suporte a concatenação por meio de colchetes `[]`.
- Utilização de **transições $\epsilon$** na construção do autômato.
- Representação explícita de **estados, transições e grafos**.
- Construção dos fragmentos do AFN através de **pilha**.
- Análise léxica da expressão regular através de **Lexer**.
- Análise sintática através de **Parser**.
- Conversão da expressão para **notação pós-fixa** durante o processamento.
- Gerenciamento manual de memória (Criação, travessia e rotina para destruição/liberação do grafo).
- Separação clara de responsabilidades entre análise, construção e representação do autômato.

---

## 🛠️ Operadores Suportados

| Operador | Nome | Exemplo | Descrição |
| :---: | :--- | :---: | :--- |
| `*` | Fecho de Kleene | `a*` | Aceita 0 ou mais ocorrências do elemento. |
| `\|` | União (OU) | `a\|b` | Aceita o caractere `a` OU o caractere `b`. |
| `()` | Agrupamento | `(a\|b)*` | Altera a precedência de avaliação dos operadores. |
| `[]` | Concatenação | `[abc]` | Concatena os elementos do escopo, formando uma sequência que deve ser reconhecida na ordem especificada. |

---
Limitação atual: a implementação não possui um operador explícito de concatenação e não trata concatenação implícita entre elementos. Por exemplo, (ab)* não é interpretado como repetição de ab.

## 🏗️ Arquitetura do Motor

O pipeline de execução segue as etapas clássicas de processamento de linguagens formais:
Plaintext

Regex Infixo         Regex Pós-fixo              AFN (Thompson)          Simulação
 "(a|b)*c*"   ───►    "ab|*c*"     ───►    [Q0] ──ε──► [Q1] ... ───►  ✅ RECOGNIZED
 


## 💻 Exemplo de Uso no Terminal

```text
Informe a string que o regex ira analisar
pedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedroanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanajoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaomariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamaria
Informe o regex a ser utilizado (suporta qualquer alfabeto ancii, * e | e concatenacao por meio de '[' ']')
([pedro]*)([ana]*)([joao]*)([maria]*)

[ NOTA ] - main(): regex postfixado: [pedro]*[ana]*[joao]*[maria]*
pedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedroanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanajoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaomariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamaria
=================================
  ✅ RECOGNIZED (Match Completo!)
=================================

thermius@arch: ./a.out 
Informe a string que o regex ira analisar
pedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedroanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanajoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaomariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariax
Informe o regex a ser utilizado (suporta qualquer alfabeto ancii, * e | e concatenacao por meio de '[' ']')
([pedro]*)([ana]*)([joao]*)([maria]*)

[ NOTA ] - main(): regex postfixado: [pedro]*[ana]*[joao]*[maria]*
pedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedropedroanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanaanajoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaojoaomariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamariamaria
Falhou no caractere 'x'

=================================
  ❌ REJECTED (String nao casa)
=================================
thermius@arch: 

```

## 🛠️ Como Compilar e Executar

Como o projeto possui todas as dependências vinculadas a partir do arquivo principal, a compilação é feita em um único comando:
1. Compilando o Projeto

No terminal, dentro da pasta do projeto, execute:
```bash
gcc main.c -o motor_regex
```
2. Executando o Programa

Após compilar, basta rodar o executável gerado:
```bash
./motor_regex
```

## ⚠️ Limitações

O projeto encontra-se em desenvolvimento e possui algumas limitações deliberadas. O objetivo atual é explorar, de forma educacional, as etapas envolvidas na construção de um motor de expressões regulares, desde a análise da entrada até a construção e execução do autômato.

A abertura e o fechamento dos escopos precisam ser resolvidos antes da construção do AFN. Isso cria uma dependência entre as etapas de análise sintática e construção do autômato que ainda está sendo refinada.


## 🧹 Gerenciamento eficiente de memória com Valgrind

O projeto foi testado com Valgrind para verificar o gerenciamento dinâmico de memória:

```text
==11686== ==11686== HEAP SUMMARY: ==11686== in use at exit: 0 bytes in 0 blocks ==11686== total heap usage: 52,673 allocs, 52,673 frees, 1,705,096 bytes allocated ==11686== ==11686== All heap blocks were freed -- no leaks are possible ==11686== ==11686== For lists of detected and suppressed errors, rerun with: -s ==11686== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

**Resultado:** 52,673 alocações e 52,673 liberações, sem vazamentos de memória ou erros detectados pelo Valgrind no teste realizado.


## 📄 Licença

© 2026. Todos os direitos reservados.

Este projeto é disponibilizado exclusivamente para fins de portfólio e demonstração técnica. O código-fonte não pode ser copiado, redistribuído, modificado ou utilizado, integral ou parcialmente, sem autorização prévia e explícita do autor.

