#ifndef Cores_hpp
#define Cores_hpp

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
#include <vector>

#include <iostream>
#include <fstream>

class Cores
{
    int qtdCores;
    int ** listaDeCores;
public:
    Cores();
    Cores(int qtdCores);
    void adicionaCor(int id, int r, int g, int b);
    void getCor(int id, int* r, int* g, int* b);
};

#endif
