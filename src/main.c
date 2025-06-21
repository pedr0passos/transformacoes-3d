#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "objeto.h"
#include "tela.h"
#include "algebra.h"
#include "camera.h"

void projetaPontoPerspectiva(float *ponto3d, float *ponto2d, float d) {
    ponto2d[0] = ponto3d[0] / (ponto3d[2] / d);
    ponto2d[1] = ponto3d[1] / (ponto3d[2] / d);
    ponto2d[2] = 1.0f;
}

void desenhaObjetoTela(SDL_Renderer *renderer, float **matriz, tObjeto3d *objeto) {
    if (renderer == NULL || objeto == NULL || objeto->pontos == NULL || objeto->arestas == NULL) {
        fprintf(stderr, "Erro: Parametros invalidos em: desenhaObjetoTela.\n");
        return;
    }

    float d = 10.0f; // Distância do observador ao plano de projeção

    for (int i=0; i<objeto->nArestas; i++) {
        int indicePonto1 = objeto->arestas[i][0];
        int indicePonto2 = objeto->arestas[i][1];

        float *pontoOriginal1 = objeto->pontos[indicePonto1];
        float *pontoOriginal2 = objeto->pontos[indicePonto2];

        float p1_homogeneo[4] = {pontoOriginal1[0], pontoOriginal1[1], pontoOriginal1[2], 1.0f};
        float p2_homogeneo[4] = {pontoOriginal2[0], pontoOriginal2[1], pontoOriginal2[2], 1.0f};

        float *pontoTransformado1 = multMatriz4dPonto(matriz, p1_homogeneo);
        float *pontoTransformado2 = multMatriz4dPonto(matriz, p2_homogeneo);

        if (pontoTransformado1 == NULL || pontoTransformado2 == NULL) {
            fprintf(stderr, "Erro ao transformar pontos em desenhaObjetoTela. Pulando aresta.\n");
            if (pontoTransformado1) free(pontoTransformado1);
            if (pontoTransformado2) free(pontoTransformado2);
            continue;
        }

        float p1_proj[3], p2_proj[3];
        projetaPontoPerspectiva(pontoTransformado1, p1_proj, d);
        projetaPontoPerspectiva(pontoTransformado2, p2_proj, d);

        desenhaArestaTela(renderer, p1_proj, p2_proj);

        free(pontoTransformado1);
        free(pontoTransformado2);
    }
}

int main (int argc, char * argv[]) {

    if (SDL_Init( SDL_INIT_EVERYTHING) < 0){
        printf("SDL não inicializou! SDL Erro: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = criaTela("Transformações 3D");

    if (window == NULL){
        printf("SDL não criou a janela! SDL Erro: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Event windowEvent;

    tObjeto3d *obj = carregaObjeto("src/objs/cubo.txt");

    // Inicia pequeno e centralizado
    escalaObjeto(obj, 0.1f, 0.1f, 0.1f); // pequeno
    transladaObjeto(obj, 0.0f, 0.0f, 7.0f); // centro da tela e mais distante da câmera

    // Cria e define a câmera
    tCamera3d *cam = criaCamera();
    float camX = 0, camY = 0, camZ = 0;
    float focoX = 0, focoY = 0, focoZ = 5;
    float cimaX = 0, cimaY = 1, cimaZ = 0;
    defineCamera(cam, camX, camY, camZ, focoX, focoY, focoZ, cimaX, cimaY, cimaZ);

    // Matriz composta model * view
    float **modelView = (float **)malloc(4 * sizeof(float *));
    for (int i = 0; i < 4; i++)
        modelView[i] = (float *)malloc(4 * sizeof(float));
    criaIdentidade4d(modelView);

    while (true) { 
        if (SDL_PollEvent(&windowEvent)){
            if(windowEvent.type == SDL_QUIT){
                break;
            }
            // Controle WASD para câmera
            if(windowEvent.type == SDL_KEYDOWN){
                switch(windowEvent.key.keysym.sym){
                    // Câmera
                    case SDLK_w:
                        camZ += 0.5f;
                        focoZ += 0.5f;
                        break;
                    case SDLK_s:
                        camZ -= 0.5f;
                        focoZ -= 0.5f;
                        break;
                    case SDLK_a:
                        camX -= 0.5f;
                        focoX -= 0.5f;
                        break;
                    case SDLK_d:
                        camX += 0.5f;
                        focoX += 0.5f;
                        break;

                    // Translação do objeto (setas)
                    case SDLK_UP:
                        transladaObjeto(obj, 0.0f, 0.2f, 0.0f);
                        break;
                    case SDLK_DOWN:
                        transladaObjeto(obj, 0.0f, 0.2f, 0.0f);
                        break;
                    case SDLK_LEFT:
                        transladaObjeto(obj, 0.2f, 0.0f, 0.0f);
                        break;
                    case SDLK_RIGHT:
                        transladaObjeto(obj, 0.2f, 0.0f, 0.0f);
                        break;

                    // Escala do objeto (Q/E)
                    case SDLK_q:
                        escalaObjeto(obj, 1.1f, 1.1f, 1.1f); // aumenta
                        break;
                    case SDLK_e:
                        escalaObjeto(obj, 0.9f, 0.9f, 0.9f); // diminui
                        break;

                    // Rotação do objeto (IJKL/UO)
                    case SDLK_i:
                        rotacionaObjetoEixoX(obj, -0.1f);
                        break;
                    case SDLK_k:
                        rotacionaObjetoEixoX(obj, 0.1f);
                        break;
                    case SDLK_j:
                        rotacionaObjetoEixoY(obj, -0.1f);
                        break;
                    case SDLK_l:
                        rotacionaObjetoEixoY(obj, 0.1f);
                        break;
                    case SDLK_u:
                        rotacionaObjetoEixoZ(obj, -0.1f);
                        break;
                    case SDLK_o:
                        rotacionaObjetoEixoZ(obj, 0.1f);
                        break;
                }
                defineCamera(cam, camX, camY, camZ, focoX, focoY, focoZ, cimaX, cimaY, cimaZ);
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Combina modelMatrix do objeto com viewMatrix da câmera
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                modelView[i][j] = obj->modelMatrix[i][j];
        multMatriz4d(cam->viewMatrix, modelView); // modelView = view * model

        desenhaObjetoTela(renderer, modelView, obj);

        SDL_RenderPresent(renderer);
    }

    // Libera recursos
    for (int i = 0; i < 4; i++) free(modelView[i]);
    free(modelView);
    desalocaCamera(cam);
    desalocaObjeto(obj);
    desalocaTela(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}