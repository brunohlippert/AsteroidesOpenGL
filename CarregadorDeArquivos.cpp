
#include "CarregadorDeArquivos.h"
#include <typeinfo>
#include <cstring>

void CarregadorDeArquivos::carregaArquivoNave(const char *arquivo){
    ifstream input;
    input.open(arquivo, ios::in);
    if (!input)
    {
        cout << "Erro ao abrir " << arquivo << ". " << endl;
        exit(0);
    }

    unsigned int linhas, colunas, corAtual;

    input >> linhas >> colunas;

    int ** matrixObjetoNave = new int* [linhas];

    for (int i=0; i<linhas; i++){
        matrixObjetoNave[i] = new int[colunas];
        memset(matrixObjetoNave[i], 0, colunas*sizeof(int));
    }

    for (int i=0; i < linhas; i++){
        for (int j=0; j < colunas; j++){
            // Le cada elemento da linha
            input >> corAtual;
            matrixObjetoNave[i][j] = corAtual;
        }
    }
    Nave naveObj = Nave(matrixObjetoNave, cores, linhas, colunas);
    naves.push_back(naveObj);

}

void CarregadorDeArquivos::carregaArquivoCores(const char *arquivo){
    ifstream input;
    input.open(arquivo, ios::in);
    if (!input)
    {
        cout << "Erro ao abrir " << arquivo << ". " << endl;
        exit(0);
    }

    unsigned int qtdCores;

    input >> qtdCores;

    Cores core = Cores(qtdCores);

    double id, r, g, b;

    for (int i=0; i < qtdCores; i++)
    {
        // Le cada elemento da linha
        input >> id >> r >> g >> b;
        core.adicionaCor(id, r, g, b);
    }

    cores = core;
}

void CarregadorDeArquivos::carregaArquivos(){
    const char *arquivoCores = "cores.txt";
    const char *arquivoDisparador = "disparador.txt";
    const char *arquivoNaveInimiga1 = "naveInimiga1.txt";
    const char *arquivoNaveInimiga2 = "naveInimiga2.txt";
    const char *arquivoNaveInimiga3 = "naveInimiga3.txt";

    carregaArquivoCores(arquivoCores);
    carregaArquivoNave(arquivoDisparador);
}

Nave CarregadorDeArquivos::getInstanceDisparador(int vidas, int xInicial, int yInicial, int angInicial){
    Nave disparadorModelo = this->naves.at(0);
    Nave instanciaDisparador = Nave(&disparadorModelo, vidas, xInicial, yInicial, angInicial);

    return instanciaDisparador;

}
