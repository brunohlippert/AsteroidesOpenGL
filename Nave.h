#ifndef Nave_hpp
#define Nave_hpp

#include <iostream>
using namespace std;


#ifdef _WIN32
#include <windows.h>
#include <glut.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif
#include "Cores.h"
#include "Ponto.h"
#include "Poligono.h"
#include <vector>

#include <iostream>
#include <fstream>

class Nave
{
private:
    int ** desenho;
    Cores cores;
    int vidas;
    int linhas;
    int colunas;
    float x;
    float y;
    int ang;
    float velocidade;

    //Inimigos
    Ponto inicial, curva, disparador;
    float t;
    bool statusCurva;

    void desenhaQuadradosDaNave();
    void desenhaQuadrado();
public:
    Nave();
    Nave(int ** desenho, Cores cores, int linhas, int colunas);
    Nave(Nave *nave, int vidas, int xInicial, int yInicial, int angInicial, float velo);
    void desenhaNave();
    void moveParaFrente(Ponto minP, Ponto maxP, bool disparo);
    void moveParaTras(Ponto minP, Ponto maxP);
    void rotacionaAntiHorario();
    void rotacionaHorario();
    Ponto getRotacaoPonto(Ponto p, int ang);
    void getPosicaoComAngulo(float* xP, float* yP, int* angP);
    Poligono getEnvelope();
    bool objetosColidem(Nave nave);
    void desenhaEnvelope();
    void perdeVida();
    int getVidas();
    float distancia(int xAux, int yAux);
    bool finalizouCurva();

    void novaCurva(Ponto disparadorAtual, Ponto curvaNovo);
    void andaNaCurva();
};

#endif
