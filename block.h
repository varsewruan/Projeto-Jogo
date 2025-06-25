#pragma once
#include <vector>
#include <map>
#include "position.h"
#include "colors.h"

class Block // classe 'pai' para administrar os blocos do jogo
{
public:
    Block();
    void Draw(int offsetX, int offsetY); // método para desenhar o bloco na tela
    void Move(int rows, int cols);       // para  movimento das peças
    void Rotate();                       // método para a rotação das peças
    void UndoRotation();
    std::vector<Position> GetCellPositions();
    int id;                                     // id de cada bloco
    std::map<int, std::vector<Position>> cells; // mapa que relaciona o id do bloco com as posições que ele ocupa

private:
    int cellSize;      // tamanho da célula
    int rotationState; // rotaçao do bloco
    int rowOffset;     // indica a linha da origem da "grade-mae" de cada peça
    int colOffset;     // indica a coluna da origem da "grade-mae" de cada peça
    std::vector<Color> colors;
};