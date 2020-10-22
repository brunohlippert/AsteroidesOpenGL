//Bruno Lippert e Pedro Chem

#include <iostream>
#include <cmath>
#include <math.h>
#include <ctime>
#include <fstream>
#include <random>
#include <cstring>

using namespace std;

#ifdef _WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Ponto.h"
#include "Nave.h"
#include "Poligono.h"

#include "Temporizador.h"

#include "CarregadorDeArquivos.h"
#define MAX_DISPAROS 10
#define VELOCIDADE_DISPARADOR 2
#define VELOCIDADE_DISPARO_INIMIGOS 1
#define VELOCIDADE_DISPARO 4
#define VIDAS_DISPARADOR 3
#define QTD_INIMIGOS 3
#define PCTG_CHANCE_INIMIGO_ATIRAR 0.1

#define TAMANHO_TELA 100
#define DISTANCIA_MINIMA_DO_DISPARADOR_INICIAR TAMANHO_TELA/2

#define DEBUG_MODE false

CarregadorDeArquivos carregador;

Nave disparador;

vector<Nave> disparos;
vector<Nave> disparosInimigos;

vector<Nave> inimigos;

Temporizador T;
double AccumDeltaT=0;

static Ponto Min, Max;


//INSTANCIA OBJETOS

void disparar(){
    float x, y;
    int ang;

    disparador.getPosicaoComAngulo(&x, &y, &ang);

    if(disparos.size() >= MAX_DISPAROS)
        return;

    disparos.push_back(carregador.getInstanceDisparo(x, y, ang, VELOCIDADE_DISPARO));
}

void dispararInimigo(Nave inimigo){
    float x, y;
    int ang;

    inimigo.getPosicaoComAngulo(&x, &y, &ang);

    disparosInimigos.push_back(carregador.getInstanceDisparo(x, y, ang, VELOCIDADE_DISPARO_INIMIGOS));
}

int getRandomInRange(int minimo, int maximo){
    static bool first = true;
    if (first)
    {
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
    }
    return minimo + rand() % (( maximo + 1 ) - minimo);
}

Ponto getRandomPontoDentroDaTela(){
    return Ponto(getRandomInRange(-TAMANHO_TELA, TAMANHO_TELA), getRandomInRange(-TAMANHO_TELA, TAMANHO_TELA));
}

void carregarInimigos(){
    float x, y;
    int ang;

    disparador.getPosicaoComAngulo(&x, &y, &ang);

    int i = 0;
    while(i < QTD_INIMIGOS){
        int xAux = getRandomInRange(Min.x, Max.x);
        int yAux = getRandomInRange(Min.y, Max.y);

        if(disparador.distancia(xAux, yAux) > DISTANCIA_MINIMA_DO_DISPARADOR_INICIAR){
            Nave inimigo = carregador.getInstanceInimigo(1, xAux, yAux, 0, float(getRandomInRange(1, 5)) / 1000);
            inimigos.push_back(inimigo);
            i++;
        }
    }
}

void init()
{
    // Define a cor do fundo da tela (BRANCO)
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    Min = Ponto(-TAMANHO_TELA, -TAMANHO_TELA);
    Max = Ponto(TAMANHO_TELA, TAMANHO_TELA);

    carregador.carregaArquivos();
    disparador = carregador.getInstanceDisparador(VIDAS_DISPARADOR, 0, 0, 0, VELOCIDADE_DISPARADOR);
    carregarInimigos();
}

double nFrames=0;
double TempoTotal=0;

//ATUALIZA POSICOES

bool disparoSaiuDaTela(Nave disparo){
    float x, y;
    int ang;

    disparo.getPosicaoComAngulo(&x, &y, &ang);

    return (y < Min.y || y > Max.y || x < Min.x || x > Max.x);

}

bool verificaDisparoAtingiuInimigo(Nave disparo){
    vector<Nave> auxInimigos;
    bool colidem = false;
    for(int i = 0; i < inimigos.size(); i++){
        if(inimigos.at(i).objetosColidem(disparo)){
            colidem = true;
            continue;
        }

        auxInimigos.push_back(inimigos.at(i));
    }

    inimigos = auxInimigos;
    return colidem;
}

bool verificaColidiuComDisparador(Nave inimigo){
    if(disparador.objetosColidem(inimigo)){
        disparador.perdeVida();
        return true;
    }
    return false;
}

void atualizaPosicaoDisparos(){
    vector<Nave> novosDisparos;
    for(int i = 0; i < disparos.size(); i++){
        disparos.at(i).moveParaFrente(Min, Max, true);

        if(!disparoSaiuDaTela(disparos.at(i)) && !verificaDisparoAtingiuInimigo(disparos.at(i))){
            novosDisparos.push_back(disparos.at(i));
        }
    }

    disparos = novosDisparos;

    vector<Nave> auxDisparosInimigos;
    for(int i = 0; i < disparosInimigos.size(); i++){
        disparosInimigos.at(i).moveParaFrente(Min, Max, true);
        if(verificaColidiuComDisparador(disparosInimigos.at(i))){
            continue;
        }

        auxDisparosInimigos.push_back(disparosInimigos.at(i));
    }

    disparosInimigos = auxDisparosInimigos;
}

void moveInimigos(){
    vector<Nave> auxInimigos;

    float x, y;
    int ang;

    disparador.getPosicaoComAngulo(&x, &y, &ang);
    Ponto pDisparador = Ponto(x, y);

    for(int i = 0; i < inimigos.size(); i++){

        if(inimigos.at(i).finalizouCurva()){
            inimigos.at(i).novaCurva(pDisparador, getRandomPontoDentroDaTela());
        }

        inimigos.at(i).andaNaCurva();

        if(verificaColidiuComDisparador(inimigos.at(i))){
            continue;
        }
        auxInimigos.push_back(inimigos.at(i));
    }

    inimigos = auxInimigos;
}

void fazDisparosInimigos(){
    for(int i = 0; i < inimigos.size(); i++){
        if(getRandomInRange(0, 100) <= PCTG_CHANCE_INIMIGO_ATIRAR){
            cout << "bang" << endl;
            dispararInimigo(inimigos.at(i));
        }
    }

}


//DESENHOS

void DesenhaEixos()
{
    Ponto Meio;
    Meio.x = (Max.x+Min.x)/2;
    Meio.y = (Max.y+Min.y)/2;
    Meio.z = (Max.z+Min.z)/2;

    glBegin(GL_LINES);
    //  eixo horizontal
        glVertex2f(Min.x,Meio.y);
        glVertex2f(Max.x,Meio.y);
    //  eixo vertical
        glVertex2f(Meio.x,Min.y);
        glVertex2f(Meio.x,Max.y);
    glEnd();
}

void desenhaDisparos(){
    for(int i = 0; i < disparos.size(); i++){
        disparos.at(i).desenhaNave();
    }

    for(int i = 0; i < disparosInimigos.size(); i++){
        disparosInimigos.at(i).desenhaNave();
    }
}

void desenhaInimigos(){
    for(int i = 0; i < inimigos.size(); i++){
        inimigos.at(i).desenhaNave();

        if(DEBUG_MODE){
            inimigos.at(i).desenhaEnvelope();
        }
    }
}

void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualiza‹o da tela em 30
    {
        AccumDeltaT = 0;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        //cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        //cout << "Nros de Frames sem desenho: " << nFrames << endl;
        //cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}
// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(Min.x,Max.x,
            Min.y,Max.y,
            0,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void output(int x, int y, float r, float g, float b, const char *txt)
{
  glColor3f( r, g, b );
  glRasterPos2f(x, y);
  int len, i;
  len = (int)strlen(txt);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, txt[i]);
  }
}

void printaVidas(){
    std::string str = "Vidas: "+std::to_string(disparador.getVidas());
    const char * c = str.c_str();
    output(0, TAMANHO_TELA - 15, 0, 0, 0, c);
}


// **********************************************************************
//  void display( void )
//
// **********************************************************************
void display( void )
{

    // Limpa a tela coma cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites lógicos da área OpenGL dentro da Janela
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLineWidth(1);
	glColor3f(1,1,1); // R, G, B  [0..1]
    DesenhaEixos();
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // Coloque aqui as chamadas das rotinas que desenham os objetos
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    if(disparador.getVidas() > 0 && inimigos.size() > 0){
        //Calculos
        atualizaPosicaoDisparos();
        moveInimigos();
        fazDisparosInimigos();
        //Desenha
        disparador.desenhaNave();
        if(DEBUG_MODE){
            disparador.desenhaEnvelope();
        }
        desenhaDisparos();
        desenhaInimigos();
        printaVidas();
    } else {
        if(disparador.getVidas() <= 0){
            output(-20, 0, 0, 0, 0, "Voce perdeu");
        } else {
            output(-20, 0, 0, 0, 0, "Voce ganhou");
        }

    }


    glutSwapBuffers();
}
// **********************************************************************
// ContaTempo(double tempo)
//      conta um certo nœmero de segundos e informa quanto frames
// se passaram neste per’odo.
// **********************************************************************
void ContaTempo(double tempo)
{
    Temporizador T;

    unsigned long cont = 0;
    cout << "Inicio contagem de " << tempo << "segundos ..." << flush;
    while(true)
    {
        tempo -= T.getDeltaT();
        cont++;
        if (tempo <= 0.0)
        {
            cout << "fim! - Passaram-se " << cont << " frames." << endl;
            break;
        }
    }

}
// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
// **********************************************************************

void keyboard ( unsigned char key, int x, int y )
{

    switch ( key )
    {
    case 27:        // Termina o programa qdo
        exit ( 0 );   // a tecla ESC for pressionada
        break;
    case 32:
        disparar();
        break;
    default:
        break;
    }
}
// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
    switch ( a_keys )
    {
    case GLUT_KEY_UP:       // Se pressionar UP
        disparador.moveParaFrente(Min, Max, false);
        break;
    case GLUT_KEY_LEFT:
        disparador.rotacionaAntiHorario();
        break;
    case GLUT_KEY_RIGHT:
        disparador.rotacionaHorario();
        break;
    case GLUT_KEY_DOWN:
        disparador.moveParaTras(Min, Max);
        break;
    default:
        break;
    }
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
// **********************************************************************
int  main ( int argc, char** argv )
{
    cout << "Programa OpenGL" << endl;

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 650, 500);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de título da janela.
    glutCreateWindow    ( "Primeiro Programa em OpenGL" );

    // executa algumas inicializações
    init ();

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // será chamada automaticamente quando
    // for necessário redesenhar a janela
    glutDisplayFunc ( display );

    // Define que o tratador de evento para
    // o invalida‹o da tela. A funcao "display"
    // será chamada automaticamente sempre que a
    // m‡quina estiver ociosa (idle)
    glutIdleFunc(animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // será chamada automaticamente quando
    // o usuário alterar o tamanho da janela
    glutReshapeFunc ( reshape );

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // será chamada automaticamente sempre
    // o usuário pressionar uma tecla comum
    glutKeyboardFunc ( keyboard );

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" será chamada
    // automaticamente sempre o usuário
    // pressionar uma tecla especial
    glutSpecialFunc ( arrow_keys );

    // inicia o tratamento dos eventos
    glutMainLoop ( );

    return 0;
}
