#pragma once        // garante que o arquivo será incluído apenas uma vez na compilação
#include <vector>   //inclui a biblioteca de vetores para administrar a grade com os blocos de dadas cores
#include <raylib.h> //para administrar as cores
class Grid
{
public: // partes públicas do código, acessíveis de fora da classe
    Grid();
    void Initialize();                    // método para inicializar a grade
    void Print();                         // para imprimir a grade no console
    void Draw();                          // para desenhar a grade na tela
    bool IsCellOutside(int row, int col); // para verificar se a célula está fora da grade
    int ClearFullRows();                  // para limpar as linhas completas
    bool IsCellEmpty(int row, int col);   // para verificar se os espaços ao redor estão preenchidos ou nao
    int grid[20][10];                     // dá a estrutura da grade (linhas, colunas)

private:                                    // partes privadas do código, acessíveis apenas dentro da classe
    bool IsRowFull(int row);                // verifica se a linha está completa
    void ClearRow(int row);                 // limpa a linha se estiver completa
    void MoveRowDown(int row, int numRows); // para mover a linha incompleta para baixo
    int numRows;                            // numero de linhas
    int numCols;                            // numero de colunas
    int cellSize;                           // tamanho dos bloquinhos em px
    std::vector<Color> colors;
};