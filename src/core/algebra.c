#include <stdio.h>
#include <stdlib.h>

//Ajusta valores de um matriz 4d para uma matriz identidade
void criaIdentidade4d(float **novaMatriz) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            novaMatriz[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

//Imprime uma matriz 4d no terminal
void imprimeMatriz4dDBG(float **matriz){
    for (int i = 0; i < 4; i++) {
        printf("  ");
        for (int j = 0; j < 4; j++) {
            printf("%.2f ", matriz[i][j]);
        }
        printf("\n");
    }
}

//Multiplica uma matriz 4d por um ponto
float *multMatriz4dPonto(float **matriz, float *ponto){
    if (matriz == NULL || ponto == NULL) {
        fprintf(stderr, "Erro: Matrizes ou ponto nulos em multMatriz4dPonto.\n");
        return NULL;
    }

    float *resultado = (float *)malloc(4 * sizeof(float));
    if (resultado == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o resultado em multMatriz4dPonto.\n");
        return NULL;
    }

    for (int i = 0; i < 4; i++) {
        resultado[i] = 0.0f;
        for (int j = 0; j < 4; j++) {
            resultado[i] += matriz[i][j] * ponto[j];
        }
    }

    return resultado;
}

//Multiplica duas matrizes 4d salvando o resultado na segunda matriz
void multMatriz4d(float **matrizA, float **matrizB){
    float temp[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            temp[i][j] = 0.0f;
            for (int k = 0; k < 4; k++)
                temp[i][j] += matrizA[i][k] * matrizB[k][j];
        }
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            matrizB[i][j] = temp[i][j];
}