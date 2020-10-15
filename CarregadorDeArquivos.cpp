
#include "CarregadorDeArquivos.h"

void CarregadorDeArquivos::carregaArquivoDisparador(const char *arquivo){
}

void CarregadorDeArquivos::carregaArquivoNave(const char *arquivo){

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

    Cores cores = Cores(qtdCores);

    double id, r, g, b;

    for (int i=0; i < qtdCores; i++)
    {
        // Le cada elemento da linha
        input >> id >> r >> g >> b;
        cores.adicionaCor(id, r, g, b);
    }

    this->cores = &cores;
}

void CarregadorDeArquivos::carregaArquivos(){
    const char *arquivoCores = "cores.txt";
    const char *arquivoDisparador = "disparador.txt";
    const char *arquivoNaveInimiga1 = "naveInimiga1.txt";
    const char *arquivoNaveInimiga2 = "naveInimiga2.txt";
    const char *arquivoNaveInimiga3 = "naveInimiga3.txt";

    carregaArquivoCores(arquivoCores);

}
