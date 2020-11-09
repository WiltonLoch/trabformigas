#include <random>

using namespace std;

class Formigas{
    private:
        bool carregando;
        int xAtual;
        int yAtual;
        mt19937 gerador;

    public:
        Formigas(int x, int y);

        ~Formigas();

        bool pegar(int **matriz, int tamanho);

        bool largar(int **matriz, int tamanho);

        int qtdVizinhos(int **matriz, int tamanho);

        bool estaCarregando();

        int getX();

        int getY();
        
        void caminha(int **matriz, int tamanho);

        void caminha2(int tamanho);

        void caminha3(int tamanho);
};
