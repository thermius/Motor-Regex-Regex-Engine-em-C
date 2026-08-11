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
| :---: | :---: | :---: | :--- |
| `*` | Fecho de Kleene | `a*` | Aceita 0 ou mais ocorrências do elemento. |
| `\|` | União (OU) | `a\|b` | Aceita o caractere `a` OU o caractere `b`. |
| `()` | Agrupamento | `(a\|b)*` | Altera a precedência de avaliação dos operadores. |

---
Limitação atual: a implementação não possui um operador explícito de concatenação e não trata concatenação implícita entre elementos. Por exemplo, (ab)* não é interpretado como repetição de ab.

🏗️ Arquitetura do Motor

O pipeline de execução segue as etapas clássicas de processamento de linguagens formais:
Plaintext

Regex Infixo         Regex Pós-fixo              AFN (Thompson)          Simulação
 "(a|b)*c*"   ───►    "ab|*c*"     ───►    [Q0] ──ε──► [Q1] ... ───►  ✅ RECOGNIZED
 


## 💻 Exemplo de Uso no Terminal

```text
thermius@arch: ./a.out 
Informe a string que o regex ira analisar
pedroooeeeiioouu
Informe o regex a ser utilizado (suporta qualquer alfabeto ancii, * e |)
pedr(a|e|i|o|u)*

[ NOTA ] - main(): regex postfixado: pedrae|i|o|u|*
pedroooeeeiioouu
=================================
  ✅ RECOGNIZED (Match Completo!)
=================================

thermius@arch: ./a.out 
Informe a string que o regex ira analisar
pedoeee
Informe o regex a ser utilizado (suporta qualquer alfabeto ancii, * e |)
pedr*oe*

[ NOTA ] - main(): regex postfixado: pedr*oe*
pedoeee
=================================
  ✅ RECOGNIZED (Match Completo!)
=================================

thermius@arch: ./a.out 
Informe a string que o regex ira analisar
aaeeeeeeeiiiiiuuuuuuc
Informe o regex a ser utilizado (suporta qualquer alfabeto ancii, * e |)
a*e*i*o*u*

[ NOTA ] - main(): regex postfixado: a*e*i*o*u*
aaeeeeeeeiiiiiuuuuuu
Falhou no caractere 'c'

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
## 🧹 Gerenciamento eficiente de memória com Valgrind

O projeto foi testado com Valgrind para verificar o gerenciamento dinâmico de memória:

```text
==4439== HEAP SUMMARY:
==4439==     in use at exit: 0 bytes in 0 blocks
==4439==   total heap usage: 3,197 allocs, 3,197 frees, 130,267 bytes allocated
==4439==
==4439== All heap blocks were freed -- no leaks are possible
==4439==
==4439== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

**Resultado:** 3.197 alocações e 3.197 liberações, sem vazamentos de memória ou erros detectados pelo Valgrind no teste realizado.


## 📄 Licença

© 2026. Todos os direitos reservados.

Este projeto é disponibilizado exclusivamente para fins de portfólio e demonstração técnica. O código-fonte não pode ser copiado, redistribuído, modificado ou utilizado, integral ou parcialmente, sem autorização prévia e explícita do autor.

