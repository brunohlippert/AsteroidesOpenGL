#include "Cores.h"
#include<bits/stdc++.h>

Cores::Cores(){}

Cores::Cores(int qtdCores){
    this->qtdCores = qtdCores;

    this->listaDeCores = new int* [qtdCores];

    for (int i=0; i<qtdCores; i++){
       this->listaDeCores[i] = new int[3];
       memset(this->listaDeCores[i], 0, 3*sizeof(int));
    }
}

void Cores::adicionaCor(int id, int r, int g, int b){
    this->listaDeCores[id - 1][0] = r;
    this->listaDeCores[id - 1][1] = g;
    this->listaDeCores[id - 1][2] = b;
}

void Cores::getCor(int id, int* r, int* g, int* b){
    *r = this->listaDeCores[id - 1][0];
    *g = this->listaDeCores[id - 1][1];
    *b = this->listaDeCores[id - 1][2];
}
