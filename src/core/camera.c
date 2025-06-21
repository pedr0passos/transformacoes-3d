#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "camera.h"
#include "algebra.h"

// Aloca a estrutura de memória para representar a câmera
tCamera3d *criaCamera() {
    tCamera3d *cam = (tCamera3d *)malloc(sizeof(tCamera3d));
    cam->posicao = (float *)malloc(3 * sizeof(float));
    cam->foco = (float *)malloc(3 * sizeof(float));
    cam->cima = (float *)malloc(3 * sizeof(float));
    cam->viewMatrix = (float **)malloc(4 * sizeof(float *));
    for (int i = 0; i < 4; i++)
        cam->viewMatrix[i] = (float *)malloc(4 * sizeof(float));
    criaIdentidade4d(cam->viewMatrix);
    return cam;
}

// Recebe os parâmetros específicos da câmera e calcula viewMatrix
void defineCamera(tCamera3d *camera, float posX, float posY, float posZ, float focX, float focY, float focZ, float cimX, float cimY, float cimZ) {
    // Guarda os vetores
    camera->posicao[0] = posX; camera->posicao[1] = posY; camera->posicao[2] = posZ;
    camera->foco[0] = focX; camera->foco[1] = focY; camera->foco[2] = focZ;
    camera->cima[0] = cimX; camera->cima[1] = cimY; camera->cima[2] = cimZ;

    // Calcula os vetores da base da câmera (view matrix estilo LookAt)
    float f[3], s[3], u[3];
    // f = foco - posicao
    for (int i = 0; i < 3; i++) f[i] = camera->foco[i] - camera->posicao[i];
    // Normaliza f
    float flen = sqrtf(f[0]*f[0] + f[1]*f[1] + f[2]*f[2]);
    for (int i = 0; i < 3; i++) f[i] /= flen;
    // s = f x cima
    s[0] = f[1]*camera->cima[2] - f[2]*camera->cima[1];
    s[1] = f[2]*camera->cima[0] - f[0]*camera->cima[2];
    s[2] = f[0]*camera->cima[1] - f[1]*camera->cima[0];
    // Normaliza s
    float slen = sqrtf(s[0]*s[0] + s[1]*s[1] + s[2]*s[2]);
    for (int i = 0; i < 3; i++) s[i] /= slen;
    // u = s x f
    u[0] = s[1]*f[2] - s[2]*f[1];
    u[1] = s[2]*f[0] - s[0]*f[2];
    u[2] = s[0]*f[1] - s[1]*f[0];

    // Monta a viewMatrix (OpenGL style)
    float **m = camera->viewMatrix;
    m[0][0] = s[0]; m[0][1] = s[1]; m[0][2] = s[2]; m[0][3] = - (s[0]*posX + s[1]*posY + s[2]*posZ);
    m[1][0] = u[0]; m[1][1] = u[1]; m[1][2] = u[2]; m[1][3] = - (u[0]*posX + u[1]*posY + u[2]*posZ);
    m[2][0] = -f[0]; m[2][1] = -f[1]; m[2][2] = -f[2]; m[2][3] = (f[0]*posX + f[1]*posY + f[2]*posZ);
    m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}

// Desaloca a câmera
void desalocaCamera(tCamera3d *camera) {
    if (!camera) return;
    if (camera->posicao) free(camera->posicao);
    if (camera->foco) free(camera->foco);
    if (camera->cima) free(camera->cima);
    if (camera->viewMatrix) {
        for (int i = 0; i < 4; i++) free(camera->viewMatrix[i]);
        free(camera->viewMatrix);
    }
    free(camera);
}