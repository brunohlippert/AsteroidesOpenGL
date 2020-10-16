#ifndef CarregadorDeArquivos_hpp
#define CarregadorDeArquivos_hpp

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
#include "Nave.h"


#include <vector>

#include <iostream>
#include <fstream>

class CarregadorDeArquivos
{
    vector <Nave> naves;
    Cores cores;
    void carregaArquivoCores(const char *arquivo);
    void carregaArquivoNave(const char *arquivo);
public:
    void carregaArquivos();
    Nave getInstanceDisparador(int vidas, int xInicial, int yInicial, int angInicial);
};

#endif
