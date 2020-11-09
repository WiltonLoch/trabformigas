#include "Formigas.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

Formigas::Formigas(int x, int y){
    this->carregando = false;
    this->xAtual = x;
    this->yAtual = y;
    gerador = mt19937(random_device{}());
}

int Formigas::qtdVizinhos(int **matriz, int tamanho){
    int soma_vizinhos = 0;
    for (int i = -1; i < 2; i++){
        for (int j = -1; j < 2; j++){
                soma_vizinhos += matriz[(tamanho + this->xAtual + i) % tamanho][(tamanho + this->yAtual + j) % tamanho];
        }
    }

    return soma_vizinhos;
}

bool Formigas::pegar(int **matriz, int tamanho){
    if(matriz[xAtual][yAtual] != 1) return false;
    
    uniform_real_distribution<float> distribution(0, 1);

    float soma_vizinhos = qtdVizinhos(matriz, tamanho);
    double probabilidade_pegar = 1.0f/soma_vizinhos;
    if(soma_vizinhos == 8) probabilidade_pegar = 0;
    probabilidade_pegar *= probabilidade_pegar;

    float moeda = distribution(gerador);

    if(moeda < probabilidade_pegar){
        this->carregando = true;
        matriz[xAtual][yAtual] = 0;
        return true;
    }
    return false;
}

bool Formigas::largar(int **matriz, int tamanho){
    if(matriz[xAtual][yAtual] != 0) return false;
    
    uniform_real_distribution<float> distribution(0, 1);

    float soma_vizinhos = qtdVizinhos(matriz, tamanho);
    
    double probabilidade_largar = soma_vizinhos/8.0f;
    probabilidade_largar *= probabilidade_largar;

    float moeda = distribution(gerador);

    if(moeda < probabilidade_largar){
        this->carregando = false;
        matriz[xAtual][yAtual] = 1;
        return true;
    }
    // printf("soma vizinhos: %d\n", soma_vizinhos);
    return false;
}

bool Formigas::estaCarregando(){
    return this->carregando;
}

int Formigas::getX(){
    return this->xAtual;
}

int Formigas::getY(){
    return this->yAtual;
}



void Formigas::caminha(int **matriz, int tamanho){
    short int mov_x[] = {-1, -1, -1,  0,  0,  1, 1, 1};
    short int mov_y[] = {-1,  0,  1, -1,  1, -1, 0, 1};
    vector<short int> probabilidades(8, 1);
    int moeda, direcao;

    uniform_int_distribution<int> distribution(0, 7);

    direcao = distribution(gerador);

    xAtual += mov_x[direcao];
    yAtual += mov_y[direcao];
    
    xAtual = (tamanho + xAtual) % tamanho;
    yAtual = (tamanho + yAtual) % tamanho;

        
}

void Formigas::caminha2(int tamanho){
    short int mov_x[3] = {-1, 0, 1};
    short int mov_y[3] = {-1, 0, 1};
    uniform_int_distribution<int> distribution(0, 2);

    int xTemp, yTemp, direcao;

    direcao = distribution(gerador);
    xTemp = this->xAtual + mov_x[direcao];
    distribution.reset();
    direcao = distribution(gerador);
    yTemp = this->yAtual + mov_y[direcao];

    
    //this->xAtual = (tamanho + xTemp) % tamanho;
    //this->yAtual = (tamanho + yTemp) % tamanho;

    this->xAtual = (tamanho - xTemp) % tamanho;
    this->yAtual = (tamanho - yTemp) % tamanho;    
}

void Formigas::caminha3(int tamanho){
    short int opcoes[3] = {-1, 0, 1};
    uniform_int_distribution<int> distribution(0, 2);

    int xTemp, yTemp, direcao;

    direcao = distribution(gerador);
    xTemp = this->xAtual + opcoes[direcao];

    distribution.reset();

    direcao = distribution(gerador);
    yTemp = this->yAtual + opcoes[direcao];

    
    this->xAtual = (tamanho + xTemp) % tamanho;
    this->yAtual = (tamanho + yTemp) % tamanho; 
}


Formigas::~Formigas(){
}

/* Outra forma de caminhar, utilizando o conceito de modulo de vetores
void Formigas::caminha(){
    uniform_int_distribution<int> distribuicaoMovX = uniform_int_distribution<int>(0, 9);
    uniform_int_distribution<int> distribuicaoMovY = uniform_int_distribution<int>(0, 9);

    int xNovo = distribuicaoMovX(this->gerador);
    int yNovo = distribuicaoMovY(this->gerador);
    int xTemp = xNovo - this->xAtual;
    int yTemp = yNovo - this->yAtual;

    xTemp = pow(xTemp, 2);
    yTemp = pow(yTemp, 2);

    if(sqrt(xTemp + yTemp) == 1 || sqrt(xTemp + yTemp) == sqrt(2)){
        this->xAtual = xNovo;
        this->yAtual = yNovo;
    }
    else{
        caminha();
    }

    //cout << "X: " << this->xAtual << " Y: " << this->yAtual << endl;
}

*/
