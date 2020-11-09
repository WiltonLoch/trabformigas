#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "Formigas.hpp"
//#include <SFML/Graphics.hpp>
#include "SFML/Graphics.hpp"
#include <omp.h>
#include <string>

#define TAMANHO_JANELA 500

// using namespace sf;

void desenhar(int **matriz, int tamMatriz, sf::RenderWindow &window, std::vector<Formigas> vFormigas){

    sf::RectangleShape grid[tamMatriz][tamMatriz];
    sf::Vector2f cellSize((TAMANHO_JANELA/tamMatriz), (TAMANHO_JANELA/tamMatriz)); 

    window.clear();

    for(int i=0;i<tamMatriz;i++){
        for(int j=0;j<tamMatriz;j++){
            grid[i][j].setSize(cellSize);
            grid[i][j].setOutlineColor(sf::Color::Black);
            grid[i][j].setOutlineThickness(1.0f);
            if(matriz[i][j] == 0) grid[i][j].setFillColor(sf::Color::White);
            if(matriz[i][j] == 1) grid[i][j].setFillColor(sf::Color::Red);                    

            grid[i][j].setPosition(i*cellSize.x, j*cellSize.y);
            window.draw(grid[i][j]);

        }
    }

    for (int i = 0; i < vFormigas.size(); i++){
        if(vFormigas[i].estaCarregando()){
            grid[vFormigas[i].getX()][vFormigas[i].getY()].setFillColor(sf::Color::Green);
        }else{
            grid[vFormigas[i].getX()][vFormigas[i].getY()].setFillColor(sf::Color::Blue);
        }
        grid[vFormigas[i].getX()][vFormigas[i].getY()].setPosition(vFormigas[i].getX() * cellSize.x, vFormigas[i].getY() * cellSize.y);

        window.draw(grid[vFormigas[i].getX()][vFormigas[i].getY()]);
    }

    window.display();
}

int main(int argc, char **argv){
    if(argc < 4){
        printf("Parâmetros faltando\n");
        printf("Uso: %s <formigas vivas> <formigas mortas> <tabuleiro> <iterações>", argv[0]);
        return 0;
    }
    mt19937 gerador(random_device{}());
    vector<Formigas> vFormigas;
    int i;
    int nVivas = atoi(argv[1]);
    int nMortas = atoi(argv[2]);
    int tamMatriz = atoi(argv[3]);
    long int iteracoes = atoi(argv[4]);
    long int total_iteracoes = iteracoes;
    int x;
    int y;

    int **matriz = (int **)calloc(tamMatriz, sizeof(int *));

    for(i=0; i<tamMatriz; i++){
        matriz[i] = (int *)calloc(tamMatriz, sizeof(int));
    }

    uniform_int_distribution<int> distribution(0, tamMatriz - 1);

// Posiciono as formigas mortas
    i=0;
    while(i<nMortas){
        x = distribution(gerador); //gera um numero randomico entre 0 e tamMatriz
        y = distribution(gerador);
        if(matriz[x][y] == 0){
            matriz[x][y] = 1;
            i++;
        }
    }

// Posiciono as formigas vivas
    i=0;
    while(i<nVivas){
        x = distribution(gerador); //gera um numero randomico entre 0 e tamMatriz
        y = distribution(gerador);
        distribution.reset();
        if(matriz[x][y] == 0){
            Formigas f1(x, y);
            vFormigas.push_back(f1);
            i++;
        }
    }

    sf::RenderWindow window(sf::VideoMode(TAMANHO_JANELA, TAMANHO_JANELA), "Ant Clustering");   
    sf::Image captura;
    
    string nome;
    
    bool ainda_carregando = false;
    
    while (window.isOpen() and (iteracoes > 0 || ainda_carregando))
    {  
		ainda_carregando = false;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        #pragma omp parallel for private(i) 
        for (int i = 0; i < nVivas; i++)
        {
            if (vFormigas[i].estaCarregando()){
               #pragma omp critical
               {
			ainda_carregando = true;
                    if(!vFormigas[i].largar(matriz, tamMatriz))
                       vFormigas[i].caminha3(tamMatriz); 
               }
            }else if(iteracoes > 0){
               #pragma omp critical
               {    					
					if(!vFormigas[i].pegar(matriz, tamMatriz)){
						vFormigas[i].caminha3(tamMatriz);  
					}
				}                               
				
			}  
		}
		if(iteracoes % 30000 == 0){
            desenhar(matriz, tamMatriz, window, vFormigas);
        }
        iteracoes--;
    }  

    free(matriz);

    return 0;
}
