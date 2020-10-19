
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
    const char *arquivoCores = "arquivosTextoCarregar/cores.txt";
    const char *arquivoDisparador = "arquivosTextoCarregar/disparador.txt";
    const char *arquivoProjetil = "arquivosTextoCarregar/projetil.txt";
    const char *arquivoNaveInimiga1 = "arquivosTextoCarregar/naveInimiga1.txt";
    const char *arquivoNaveInimiga2 = "arquivosTextoCarregar/naveInimiga2.txt";
    const char *arquivoNaveInimiga3 = "arquivosTextoCarregar/naveInimiga3.txt";

    carregaArquivoCores(arquivoCores);
    carregaArquivoNave(arquivoDisparador);
    carregaArquivoNave(arquivoProjetil);

    carregaArquivoNave(arquivoNaveInimiga1);
    carregaArquivoNave(arquivoNaveInimiga2);
    carregaArquivoNave(arquivoNaveInimiga3);

}

int CarregadorDeArquivos::getRandomInRange(int minimo, int maximo){
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(minimo, maximo); // define the range

    return distr(gen);
}

Nave CarregadorDeArquivos::getInstanceDisparador(int vidas, int xInicial, int yInicial, int angInicial, float velocidade){
    Nave disparadorModelo = this->naves.at(0);
    Nave instanciaDisparador = Nave(&disparadorModelo, vidas, xInicial, yInicial, angInicial, velocidade);

    return instanciaDisparador;
}

Nave CarregadorDeArquivos::getInstanceDisparo(int xInicial, int yInicial, int angInicial, float velocidade){
    Nave disparoModelo = this->naves.at(1);
    Nave instanciaDisparo = Nave(&disparoModelo, 1, xInicial, yInicial, angInicial, velocidade);

    return instanciaDisparo;
}

Nave CarregadorDeArquivos::getInstanceInimigo(int vidas, int xInicial, int yInicial, int angInicial, float velocidade){
    int minIndexNavesInimigas = 2;
    int maxIndexNavesInimigas = 4;

    int randNaveIndex = getRandomInRange(minIndexNavesInimigas, maxIndexNavesInimigas);

    Nave inimigoModelo = this->naves.at(randNaveIndex);
    Nave instanciaInimigo = Nave(&inimigoModelo, vidas, xInicial, yInicial, angInicial, velocidade);

    return instanciaInimigo;
}
