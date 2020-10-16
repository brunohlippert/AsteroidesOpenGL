#include "Nave.h"
#include <math.h>
#define velocidadeRotacao 5
#define PI 3.14159265
Nave::Nave(){}

Nave::Nave(int ** desenh, Cores core, int linha, int coluna){
    desenho = desenh;
    cores = core;
    linhas = linha;
    colunas = coluna;
}

Nave::Nave(Nave *nave, int vida, int xInicial, int yInicial, int angInicial){
    desenho = nave->desenho;
    cores = nave->cores;
    vidas = vida;
    linhas = nave->linhas;
    colunas = nave->colunas;
    x = xInicial;
    y = yInicial;
    ang = angInicial;
}

void Nave::desenhaQuadrado(){
    glBegin(GL_QUADS);
        glVertex2d(-1, -1);
        glVertex2d(-1, 0);
        glVertex2d(0, 0);
        glVertex2d(0, -1);
    glEnd();
}

void Nave::desenhaQuadradosDaNave(){

    glPushMatrix();
        glTranslatef(0, 1, 0);
        for(int i = 0; i < linhas; i++){
            glTranslatef(0, -1, 0);
            for(int j = 0; j < colunas; j++){
                glTranslatef(1, 0, 0);
                int idDaCor = desenho[i][j];
                int r, g, b;
                //Se for -1 eh transparente
                if(idDaCor > 0){
                    cores.getCor(idDaCor, &r, &g, &b);
                    glColor3f(r, g, b);
                    desenhaQuadrado();
                }

            }
            glTranslatef(-colunas, 0, 0);
        }
    glPopMatrix();
}

void Nave::desenhaNave(){
    glPushMatrix();
        glTranslatef(x, y, 0);
        glRotatef(ang, 0, 0, 1);
        glTranslatef(-(colunas/2), linhas/2, 0);
        this->desenhaQuadradosDaNave();
    glPopMatrix();
}

void Nave::rotacionaAntiHorario(){
    ang += velocidadeRotacao;
}

void Nave::rotacionaHorario(){
    ang -= velocidadeRotacao;
}

void Nave::moveParaFrente(){
    float xu = 0 * cos(ang * PI / 180.0) - 1 * sin(ang * PI / 180.0);
    float yu = 1 * cos(ang * PI / 180.0) + 0 * sin(ang * PI / 180.0);

    x += xu;
    y += yu;
}

