#include "Nave.h"
#include <math.h>
#define velocidadeRotacao 5
#define PI 3.14159265
Nave::Nave(){}

Nave::Nave(int ** desenh, Cores core, int linha, int coluna){
    desenho = desenh;
    cores = core;
    linhas = linha;
    colunas = coluna;
}

Nave::Nave(Nave *nave, int vida, int xInicial, int yInicial, int angInicial, float velo){
    desenho = nave->desenho;
    cores = nave->cores;
    vidas = vida;
    linhas = nave->linhas;
    colunas = nave->colunas;
    x = xInicial;
    y = yInicial;
    ang = angInicial;
    statusCurva = true;
    t = 0.0;
    velocidade = velo;
}

void Nave::desenhaQuadrado(){
    glBegin(GL_QUADS);
        glVertex2d(-1, -1);
        glVertex2d(-1, 0);
        glVertex2d(0, 0);
        glVertex2d(0, -1);
    glEnd();
}

void Nave::desenhaQuadradosDaNave(){

    glPushMatrix();
        glTranslatef(0, 1, 0);
        for(int i = 0; i < linhas; i++){
            glTranslatef(0, -1, 0);
            for(int j = 0; j < colunas; j++){
                glTranslatef(1, 0, 0);
                int idDaCor = desenho[i][j];
                int r, g, b;
                //Se for -1 eh transparente
                if(idDaCor > 0){
                    cores.getCor(idDaCor, &r, &g, &b);
                    glColor3f(r, g, b);
                    desenhaQuadrado();
                }

            }
            glTranslatef(-colunas, 0, 0);
        }
    glPopMatrix();
}

void Nave::desenhaNave(){
    glPushMatrix();
        glTranslatef(x, y, 0);
        glRotatef(ang, 0, 0, 1);
        glTranslatef(-(colunas/2), linhas/2, 0);
        this->desenhaQuadradosDaNave();
    glPopMatrix();
}

void Nave::perdeVida(){
    vidas--;
}

int Nave::getVidas(){
    return vidas;
}

void Nave::rotacionaAntiHorario(){
    ang += velocidadeRotacao;
}

void Nave::rotacionaHorario(){
    ang -= velocidadeRotacao;
}

Ponto Nave::getRotacaoPonto(Ponto p, int ang){
    float xu = p.x * cos(ang * PI / 180.0) - p.y * sin(ang * PI / 180.0);
    float yu = p.y * cos(ang * PI / 180.0) + p.x * sin(ang * PI / 180.0);

    Ponto pRotacao = Ponto(xu, yu);

    return pRotacao;
}

void Nave::moveParaFrente(Ponto minP, Ponto maxP, bool disparo){
    Ponto p = Ponto(0, velocidade);
    Ponto pRotacao = getRotacaoPonto(p, ang);

    if(disparo){
        x += pRotacao.x;
        y += pRotacao.y;
    } else {
        if (x + pRotacao.x >= minP.x + 2 && x + pRotacao.x <= maxP.x - 2)
            x += pRotacao.x;
        if (y + pRotacao.y >= minP.y + 2 && y + pRotacao.y <= maxP.y - 2)
            y += pRotacao.y;
    }
}

void Nave::moveParaTras(Ponto minP, Ponto maxP){
    Ponto p = Ponto(0, -velocidade);
    Ponto pRotacao = getRotacaoPonto(p, ang);

    if (x + pRotacao.x >= minP.x + 2 && x + pRotacao.x <= maxP.x - 2)
        x += pRotacao.x;
    if (y + pRotacao.y >= minP.y + 2 && y + pRotacao.y <= maxP.y - 2)
        y += pRotacao.y;

}

void Nave::getPosicaoComAngulo(float* xP, float* yP, int* angP){
    *xP = x;
    *yP = y;
    *angP = ang;
}

Poligono Nave::getEnvelope(){
    // Mesmo formato do desenha quadrado
    // P2 ------- P3
    // |          |
    // |          |
    // P1 ------- P4
    Ponto p1, p2, p3, p4;

    p1 = Ponto(0, -linhas);
    p2 = Ponto(0, 0);
    p3 = Ponto(colunas, 0);
    p4 = Ponto(colunas, -linhas);

    p1 = Ponto(p1.x - (colunas / 2), p1.y + (linhas / 2));
    p2 = Ponto(p2.x - (colunas / 2), p2.y + (linhas / 2));
    p3 = Ponto(p3.x - (colunas / 2), p3.y + (linhas / 2));
    p4 = Ponto(p4.x - (colunas / 2), p4.y + (linhas / 2));

    p1 = getRotacaoPonto(p1, ang);
    p2 = getRotacaoPonto(p2, ang);
    p3 = getRotacaoPonto(p3, ang);
    p4 = getRotacaoPonto(p4, ang);

    p1 = Ponto(p1.x + x, p1.y + y);
    p2 = Ponto(p2.x + x, p2.y + y);
    p3 = Ponto(p3.x + x, p3.y + y);
    p4 = Ponto(p4.x + x, p4.y + y);

    Poligono envelope;
    envelope.insereVertice(p1);
    envelope.insereVertice(p2);
    envelope.insereVertice(p3);
    envelope.insereVertice(p4);

    return envelope;
}

bool Nave::objetosColidem(Nave nave){
    Poligono envelopeAtual = getEnvelope();
    Poligono envelopeNaveTeste = nave.getEnvelope();

    Ponto p2 = envelopeAtual.getVertice(1);
    Ponto p4 = envelopeAtual.getVertice(3);

    p2 = Ponto(p2.x - x, p2.y - y);
    p4 = Ponto(p4.x - x, p4.y - y);

    float xMin, xMax, yMin, yMax;

    xMin = getRotacaoPonto(p2, -ang).x;
    xMax = getRotacaoPonto(p4, -ang).x;

    yMin = getRotacaoPonto(p4, -ang).y;
    yMax = getRotacaoPonto(p2, -ang).y;

    for(int i = 0; i < 4; i++){
        Ponto pAux = envelopeNaveTeste.getVertice(i);
        pAux = Ponto(pAux.x - x, pAux.y - y);
        pAux = getRotacaoPonto(pAux, -ang);
        if(pAux.x >= xMin && pAux.x <= xMax && pAux.y >= yMin && pAux.y <= yMax){
            return true;
        }
    }

    return false;
}

void Nave::desenhaEnvelope(){
    Poligono envelope = getEnvelope();
    envelope.desenhaPoligono();
}

float Nave::distancia(int xAux, int yAux){
    return sqrt(pow((x - xAux), 2) + pow((y - yAux), 2));
}


//Inimigo

bool Nave::finalizouCurva(){
    return statusCurva;
}

void Nave::novaCurva(Ponto disparadorAtual, Ponto curvaNovo){
    inicial = Ponto(x, y);
    curva = curvaNovo;
    disparador = disparadorAtual;
    statusCurva = false;
    t = 0;
}
void Nave::andaNaCurva(){
    t = t + velocidade;
    float pointX = pow(1 - t, 2) * inicial.x + 2 * t * (1 - t) * curva.x + pow(t, 2) * disparador.x;
    float pointY = pow(1 - t, 2) * inicial.y + 2 * t * (1 - t) * curva.y + pow(t, 2) * disparador.y;

    ang += 1;
    //cout << "Antigo: "<< x << ", " << y << " Novo: " << pointX << ", "<< pointY << endl;

    x = pointX;
    y = pointY;

    if( t >= 1){
        statusCurva = true;
    }
}

