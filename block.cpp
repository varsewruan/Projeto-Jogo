#include <block.h>

Block::Block()
{
    cellSize = 30; // define o tamanho da célula do bloco
    rotationState = 0; // inicializa o estado de rotação do bloco
    colors = GetCellColors(); // obtem as cores das células do bloco
    rowOffset = 0; // inicializa o "deslocador" da linha do ponto origem da "grade-mae" de cada peça
    colOffset = 0; // inicializa o "deslocador" da coluna do ponto origem da "grade-mae" de cada peça
}

void Block::Draw(int offsetX, int offsetY) // método para desenhar o bloco na tela
{
    std::vector<Position> tiles = GetCellPositions(); // obtem as posições seguindo a rotação e armazena em um vetor "tiles"
    for (Position item : tiles) // percorre todas as posições do vetor "tiles"
    {
        DrawRectangle(item.col * cellSize + offsetX, item.row * cellSize + offsetY, cellSize - 1, cellSize - 1, colors[id]); // desenha o bloco na tela
    }
}

void Block::Move(int rows, int cols)
{
    rowOffset += rows; // atualiza o deslocamento da linha
    colOffset += cols; // atualiza o deslocamento da coluna
}

void Block::Rotate()
{
    
    rotationState++;
    if (rotationState == (int)cells.size()) // verifica se a rotação é maior que o número de rotações possíveis
    {
        rotationState = 0;
    }
}

void Block::UndoRotation() // desfazer a rotação
{
    rotationState--; // diminui o estado de rotação
}

std::vector<Position> Block::GetCellPositions() // método para obter as posições das células do bloco
{
    std::vector<Position> tiles = cells[rotationState]; // obtem as posições seguindo a rotação e aramazena em um vetor "tiles"
    std::vector<Position> movedTiles;                   // cria um vetor para armazenar as posições alteradas

    for (Position item : tiles) // percorre todas as posições do vetor "tiles"
    {
        Position newPos = Position(item.row + rowOffset, item.col + colOffset); // define a posição pós deslocamento
        movedTiles.push_back(newPos);                                           // adiciona as posições alteradas ao vetor de posições movidas
    }
    return movedTiles; // retorna o vetor de posições alteradas
}
