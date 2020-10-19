#ifndef Nave_hpp
#define Nave_hpp

#include <iostream>
using namespace std;


#ifdef WIN32
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

    void desenhaQuadradosDaNave();
    void desenhaQuadrado();
public:
    Nave();
    Nave(int ** desenho, Cores cores, int linhas, int colunas);
    Nave(Nave *nave, int vidas, int xInicial, int yInicial, int angInicial);
    void desenhaNave();
    void moveParaFrente(Ponto minP, Ponto maxP);
    void rotacionaAntiHorario();
    void rotacionaHorario();
};

#endif
