#include <stdio.h>
#include <SDL2/SDL.h>

#include "objeto.h"
#include "algebra.h"

//Lê as informações de um arquivo e as carrega num novo objeto alocado
tObjeto3d *carregaObjeto(char *nomeArquivo){

    FILE *arquivo = fopen(nomeArquivo, "r");

    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return NULL;
    }

    tObjeto3d *objeto = (tObjeto3d *)malloc(sizeof(tObjeto3d));

    if (!objeto) {
        printf("Erro ao alocar memória para o objeto\n");
        fclose(arquivo);
        return NULL;
    }

    int i;

    if (fscanf(arquivo, "%d", &objeto->nPontos) != 1) {
        fprintf(stderr, "Erro ao ler o número de pontos\n");
        fclose(arquivo);
        free(objeto);
        return NULL;
    }

    objeto->pontos = (float **)malloc(objeto->nPontos * sizeof(float *));

    // aloca vetor de pontos
    for (i = 0; i < objeto->nPontos; i++) {
        objeto->pontos[i] = (float *)malloc(3 * sizeof(float));
        if (fscanf(arquivo, "%f %f %f", 
            &objeto->pontos[i][0], 
            &objeto->pontos[i][1], 
            &objeto->pontos[i][2]) != 3) {
            fprintf(stderr, "Erro ao ler ponto %d\n", i);
            fclose(arquivo);
        }
        objeto->pontos[i][3] = 1.0f; // Adiciona coordenada homogênea
    }

    // leitura número de arestas
    if (fscanf(arquivo, "%d", &objeto->nArestas) != 1) {
        fprintf(stderr, "Erro ao ler o número de arestas\n");
        fclose(arquivo);
        free(objeto->pontos);
        free(objeto);
        return NULL;
    }

    objeto->arestas = (int **)malloc(objeto->nArestas * sizeof(int *));

    // aloca vetor de arestas
    for (i = 0; i < objeto->nArestas; i++) {
        objeto->arestas[i] = (int *)malloc(2 * sizeof(int));
        if (fscanf(arquivo, "%d %d", 
            &objeto->arestas[i][0], 
            &objeto->arestas[i][1]) != 2) {
            fprintf(stderr, "Erro ao ler aresta %d\n", i);
            fclose(arquivo);
        }
    }

    objeto->transl = (float *)malloc(3 * sizeof(float));
    objeto->escala = (float *)malloc(3 * sizeof(float));
    
    for (i = 0; i < 3; i++) {
        objeto->transl[i] = 0.0f; // Inicializa a translação
        objeto->escala[i] = 1.0f; // Inicializa a escala
    }

    objeto->Rx = 0.0f; // Inicializa a rotação em torno do eixo X
    objeto->Ry = 0.0f; // Inicializa a rotação em torno do eixo Y
    objeto->Rz = 0.0f; // Inicializa a rotação em torno do eixo Z   

    objeto->modelMatrix = (float **)malloc(4 * sizeof(float *));
    
    for (i = 0; i < 4; i++) {
        objeto->modelMatrix[i] = (float *)malloc(4 * sizeof(float));
    }

    criaIdentidade4d(objeto->modelMatrix);

    fclose(arquivo);
    return objeto;
}

//Altera a modelMatrix de um objeto para redimenciona-lo segundo os parâmetros escalaX, escalaY e escalaZ
void escalaObjeto(tObjeto3d *objeto, float escalaX, float escalaY, float escalaZ){

}

//Altera a modelMatrix de um objeto para translada-lo segundo os parâmetros transX, transY e transZ
void transladaObjeto(tObjeto3d *objeto, float transX, float transY, float transZ){

}

//Altera a modelMatrix de um objeto para rotaciona-lo ao redor do eixo X segundo o angulo informado
void rotacionaObjetoEixoX(tObjeto3d *objeto, float angulo){

}

//Altera a modelMatrix de um objeto para rotaciona-lo ao redor do eixo Y segundo o angulo informado
void rotacionaObjetoEixoY(tObjeto3d *objeto, float angulo){

}

//Altera a modelMatrix de um objeto para rotaciona-lo ao redor do eixo Z segundo o angulo informado
void rotacionaObjetoEixoZ(tObjeto3d *objeto, float angulo){

}

//Imprime um objeto no terminal
void imprimeObjetoDBG(tObjeto3d *objeto){
    int i, j;
    if (!objeto) {
        printf("Objeto nulo\n");
        return;
    }
    printf("Objeto3d: %d pontos, %d arestas\n", objeto->nPontos, objeto->nArestas);

    // Pontos
    printf("Pontos (x, y, z, w):\n");
    for (i = 0; i < objeto->nPontos; i++) {
        printf("  [%d] %.2f, %.2f, %.2f, %.2f\n",
               i,
               objeto->pontos[i][0],
               objeto->pontos[i][1],
               objeto->pontos[i][2],
               objeto->pontos[i][3]);
    }

    // Arestas
    printf("Arestas (v1 -> v2):\n");
    for (i = 0; i < objeto->nArestas; i++) {
        printf("  [%d] %d -> %d\n",
               i,
               objeto->arestas[i][0],
               objeto->arestas[i][1]);
    }

    // Transformações
    printf("Translação: x=%.2f, y=%.2f, z=%.2f\n",
           objeto->transl[0],
           objeto->transl[1],
           objeto->transl[2]);
    printf("Escala: x=%.2f, y=%.2f, z=%.2f\n",
           objeto->escala[0],
           objeto->escala[1],
           objeto->escala[2]);
    printf("Rotação: Rx=%.2f, Ry=%.2f, Rz=%.2f\n",
           objeto->Rx,
           objeto->Ry,
           objeto->Rz);

    // Matriz de modelo
    printf("Model Matrix (4x4):\n");
    for (i = 0; i < 4; i++) {
        printf("  ");
        for (j = 0; j < 4; j++) {
            printf("%.2f ", objeto->modelMatrix[i][j]);
        }
        printf("\n");
    }
}

//Desaloca o objeto
void desalocaObjeto(tObjeto3d *objeto){

}
