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
#include <vector>

#include <iostream>
#include <fstream>

class Nave
{
    int ** desenho;
    Cores *cores;
    int vidas;
public:
    Nave(int ** desenho, Cores cores , int vidas);
};

#endif
