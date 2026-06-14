# Trabalho 3 de AEDII — Análise de Desempenho de Índices

Este projeto prático consiste na implementação, análise e comparação de desempenho entre diferentes estruturas de dados (Árvore Binária de Pesquisa, Tabela Hash e Busca Sequencial) utilizadas para indexação e recuperação de registros em arquivos binários.

---

## Integrantes

* **André Malmsteen Oliveira Amorim** 
* **Benjamim Isaac Ribeiro Lima** 
* **Giovanna Bembom da Silva Bandeira**
* **Mariana Ramos André Simões**

**Disciplina:** Algoritmos e Estruturas de Dados II (AED2)  
**Ano:** 2026

---

## Descrição das Questões

O programa executa de forma sequencial um bloco de 30 buscas de teste para avaliar o tempo médio, desvio padrão e tempo total gasto em cada uma das abordagens abaixo:

### Questão 1: Busca via ABP para o atributo chave (ID)
Realiza a busca de 30 chaves geradas aleatoriamente utilizando uma **Árvore Binária de Pesquisa (ABP)** carregada em memória RAM. Cada nó da árvore guarda o `ID` e a `posição` física do registro no arquivo. Quando o ID é encontrado na árvore, o programa faz um acesso direto (`fseek`) na posição exata do arquivo binário.

### Questão 2: Busca via Tabela Hash para o atributo chave (ID)
Efetua as mesmas buscas da Q1, mas utilizando uma **Tabela Hash** com encadeamento exterior (lista encadeada para tratar colisões). Além das métricas de tempo, essa etapa contabiliza e exibe o número total de colisões geradas durante o processo de busca.

### Questão 3: Busca sequencial no arquivo pelo atributo chave (ID)
Serve como cenário de pior caso (linha de base). O programa percorre o arquivo binário do início ao fim (`O(n)`) registro por registro usando `fread` até encontrar o ID desejado ou atingir o fim do arquivo, sem utilizar qualquer estrutura de índice na memória.

### Questão 4: Busca por faixa (Range Query) via ABP (Atributo não-chave: Ano)
Avalia consultas com operadores de comparação (`>`, `>=`, `<`, `<=`) baseadas no campo `ano` do registro. Utiliza uma árvore binária modificada (`arvAno`) onde cada nó aponta para uma lista encadeada de registros que compartilham aquele mesmo ano. O resultado gera uma lista dinâmica com todos os registros mapeados dentro do intervalo solicitado.

### Questão 5: Busca por faixa sequencial no arquivo (Atributo não-chave: Ano)
Realiza as mesmas consultas por intervalo da Q4, porém varrendo o arquivo binário linearmente de ponta a ponta. O programa lê todos os registros sequencialmente e incrementa um contador sempre que o campo `ano` satisfizer a condição do operador lógico testado.
