#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "registro.h"
#include "TAD_arv.h"
#include "TAD_arv_ano.h"
#include "TAD_hash.h"
#include "estatisticas.h"

#define ARQUIVO_DADOS "dados.bin"
#define N_REGISTROS 100000
#define N_BUSCAS 30

static const int LIMIARES[N_BUSCAS] = {
    2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009,
    2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019,
    2020, 2021, 2022, 2023, 2024, 2025, 2026, 2027, 2028, 2029
};

static void secao(int n, const char *titulo)
{
    printf("\n");
    printf("QUESTÃO %d — %s\n", n, titulo);
}

static void questao1(FILE *arq, arv *raiz_bst, const int chaves[], double tempos_bst[])
{
    secao(1, "Busca via ABP para o atributo chave (ID)");

    registro resultado;
    for (int i = 0; i < N_BUSCAS; i++) {
        double ini = tempoAtual();
        int ok = buscarNoArquivo(raiz_bst, chaves[i], arq, &resultado);
        tempos_bst[i] = tempoAtual() - ini;

        printf("[ABP] Chave = %-6d %s  %.9f s\n", chaves[i], ok ? "Encontrada" : "Não encontrada", tempos_bst[i]);
    }

    double med = media(tempos_bst, N_BUSCAS);
    double dp = desvioPadrao(tempos_bst, N_BUSCAS, med);
    printf("\nMédia ABP: %.9f s\n", med);
    printf("Desvio padrão: %.9f s\n", dp);
    printf("Tempo total: %.6f s\n",  soma(tempos_bst, N_BUSCAS));
}

static void questao2(FILE *arq, const int chaves[], double tempos_hash[])
{
    secao(2, "Busca via tabela Hash para o atributo chave (ID)");
    tabelaHash *tabela = indiceHash(arq, 50021);

    registro resultado;
    int colisoes_totais = 0;

    for (int i = 0; i < N_BUSCAS; i++) {
        int colisoes = 0;
        double ini = tempoAtual();
        int ok = buscarHashArquivo(tabela, chaves[i], arq, &resultado, &colisoes);
        tempos_hash[i] = tempoAtual() - ini;
        colisoes_totais += colisoes;

        printf("[HASH] Chave = %-6d %s  %.9f s | Colisões = %d\n", chaves[i], ok ? "Encontrada" : "Não encontrada", tempos_hash[i], colisoes);
    }

    double med = media(tempos_hash, N_BUSCAS);
    double dp  = desvioPadrao(tempos_hash, N_BUSCAS, med);
    printf("\nMédia Hash: %.9f s\n", med);
    printf("Desvio padrão: %.9f s\n", dp);
    printf("Tempo total: %.6f s\n",  soma(tempos_hash, N_BUSCAS));
    printf("Total de colisões: %d\n", colisoes_totais);

    liberaHash(tabela);
}

static void questao3(FILE *arq, const int chaves[], double tempos_seq[])
{
    secao(3, "Busca sequencial no arquivo pelo atributo chave (ID)");

    registro resultado;
    for (int i = 0; i < N_BUSCAS; i++) {
        double ini = tempoAtual();
        int ok = arquivo_busca_seq_chave(arq, chaves[i], &resultado);
        tempos_seq[i] = tempoAtual() - ini;

        printf("[SEQ] Chave = %-6d %s  %.9f s\n", chaves[i], ok ? "Encontrada" : "Não encontrada", tempos_seq[i]);
    }

    double med = media(tempos_seq, N_BUSCAS);
    double dp = desvioPadrao(tempos_seq, N_BUSCAS, med);
    printf("\nMédia SEQ: %.9f s\n", med);
    printf("Desvio padrão: %.9f s\n", dp);
    printf("Tempo total: %.6f s\n",  soma(tempos_seq, N_BUSCAS));
}

static void questao4(arvAno *raiz_ano, double tempos_q4[][N_BUSCAS])
{
    secao(4, "Busca 'ano OP X' via ABP (atributo não-chave)");

    const int  operadores[4] = { OP_MAIOR, OP_MAIOR_IGUAL, OP_MENOR, OP_MENOR_IGUAL };
    const char *sinais[4] = { ">", ">=", "<", "<=" };

    for (int op = 0; op < 4; op++) {
        printf("\nOperador '%s':\n", sinais[op]);

        for (int i = 0; i < N_BUSCAS; i++) {
            double ini = tempoAtual();
            resLista *res = buscarRangeAno(raiz_ano, LIMIARES[i], operadores[op]);
            int cnt = contarResLista(res);
            tempos_q4[op][i] = tempoAtual() - ini;
            liberaResLista(res);

            printf("[ABP] Ano %s %-4d : %6d registros  %.9f s\n", sinais[op], LIMIARES[i], cnt, tempos_q4[op][i]);
        }

        double med = media(tempos_q4[op], N_BUSCAS);
        double dp = desvioPadrao(tempos_q4[op], N_BUSCAS, med);
        printf("\nMédia (ABP %s): %.9f s\n", sinais[op], med);
        printf("Desvio padrão: %.9f s\n", dp);
        printf("Tempo total: %.6f s\n",  soma(tempos_q4[op], N_BUSCAS));
    }
}

static void questao5(FILE *arq, double tempos_q5[][N_BUSCAS])
{
    secao(5, "Busca 'ano OP X' sequencial no arquivo (atributo não-chave)");

    const int  operadores[4] = { OP_MAIOR, OP_MAIOR_IGUAL, OP_MENOR, OP_MENOR_IGUAL };
    const char *sinais[4] = { ">", ">=", "<", "<=" };

    for (int op = 0; op < 4; op++) {
        printf("\nOperador '%s':\n", sinais[op]);

        for (int i = 0; i < N_BUSCAS; i++) {
            double ini = tempoAtual();
            int cnt = arquivo_busca_seq_ano(arq, LIMIARES[i], operadores[op]);
            tempos_q5[op][i] = tempoAtual() - ini;

            printf("[SEQ] Ano %s %-4d : %6d registros  %.9f s\n", sinais[op], LIMIARES[i], cnt, tempos_q5[op][i]);
        }

        double med = media(tempos_q5[op], N_BUSCAS);
        double dp = desvioPadrao(tempos_q5[op], N_BUSCAS, med);
        printf("\nMédia (SEQ %s): %.9f s\n", sinais[op], med);
        printf("Desvio padrão: %.9f s\n", dp);
        printf("Tempo total: %.6f s\n", soma(tempos_q5[op], N_BUSCAS));
    }
}

static void resumo(double tempos_bst[], double tempos_hash[], double tempos_seq[], double tempos_q4[][N_BUSCAS], double tempos_q5[][N_BUSCAS])
{
    const char *sinais[4] = { ">", ">=", "<", "<=" };

    printf("\n");
    printf("Resumo Final\n");

    double med_bst = media(tempos_bst, N_BUSCAS);
    double med_hash = media(tempos_hash, N_BUSCAS);
    double med_seq = media(tempos_seq, N_BUSCAS);

    printf("\n  [Busca pelo atributo chave (ID)]\n");
    printf("%s: %f s\n", "Média ABP", med_bst);
    printf("%s: %f s\n", "Média Hash", med_hash);
    printf("%s: %f s\n", "Média SEQ", med_seq);

    printf("\n  [Busca pelo atributo não-chave (ano)]\n");
    for (int op = 0; op < 4; op++) {
        double m4 = media(tempos_q4[op], N_BUSCAS);
        double m5 = media(tempos_q5[op], N_BUSCAS);
        printf("Operador %-4s \n ABP: %.9f s | SEQ: %.9f s", sinais[op], m4, m5);
        printf("\n");
    }
}

int main(void)
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    FILE *arq = fopen(ARQUIVO_DADOS, "rb");
    if (arq == NULL) {
        printf("Arquivo não encontrado. Gerando %d registros...\n", N_REGISTROS);
        arquivo_gerar(ARQUIVO_DADOS, N_REGISTROS);
        arq = arquivo_abrir(ARQUIVO_DADOS, "rb");
    } else {
        printf("Arquivo '%s' aberto. Total de registros: %ld\n",
               ARQUIVO_DADOS, arquivo_total_registros(arq));
    }
    if (arq == NULL) {
        fprintf(stderr, "Erro: não foi possível abrir o arquivo.\n");
        return 1;
    }

    printf("\nConstruindo índice ABP (por ID)...\n");
    double t0 = tempoAtual();
    arv *raiz_bst = construirIndice(arq);
    printf("ABP construída em %.6f s\n", tempoAtual() - t0);

    printf("\nConstruindo índice ABP para 'ano'...\n");
    t0 = tempoAtual();
    arvAno *raiz_ano = construirIndiceAno(arq);
    printf("ABP-ano construída em %.6f s\n", tempoAtual() - t0);

    int chaves[N_BUSCAS];
    gerar_chaves(chaves, N_BUSCAS, N_REGISTROS);

    printf("\nChaves de teste (Q1/Q2/Q3):\n");
    for (int i = 0; i < N_BUSCAS; i++) {
        printf("%d", chaves[i]);
        if (i < N_BUSCAS - 1) printf(", ");
    }
    printf("\n");

    double tempos_bst[N_BUSCAS];
    double tempos_hash[N_BUSCAS];
    double tempos_seq[N_BUSCAS];
    double tempos_q4[4][N_BUSCAS];
    double tempos_q5[4][N_BUSCAS];

    questao1(arq, raiz_bst, chaves, tempos_bst);
    questao2(arq, chaves, tempos_hash);
    questao3(arq, chaves, tempos_seq);
    questao4(raiz_ano, tempos_q4);
    questao5(arq, tempos_q5);
    resumo(tempos_bst, tempos_hash, tempos_seq, tempos_q4, tempos_q5);

    raiz_bst = podaArv(raiz_bst);
    podaArvAno(raiz_ano);
    arquivo_fechar(arq);

    return 0;
}