#ifndef CarregadorDeArquivos_hpp
#define CarregadorDeArquivos_hpp
#include <ctime>
#include <iostream>
using namespace std;
#include <random>

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
#include "Nave.h"


#include <vector>

#include <iostream>
#include <fstream>
#include <random>

class CarregadorDeArquivos
{
    vector <Nave> naves;
    Cores cores;
    void carregaArquivoCores(const char *arquivo);
    void carregaArquivoNave(const char *arquivo);
    int getRandomInRange(int minimo, int maximo);
public:
    void carregaArquivos();
    Nave getInstanceDisparador(int vidas, int xInicial, int yInicial, int angInicial, float velocidade);
    Nave getInstanceDisparo(int xInicial, int yInicial, int angInicial, float velocidade);
    Nave getInstanceInimigo(int vidas, int xInicial, int yInicial, int angInicial, float velocidade);
};

#endif
