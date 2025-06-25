#include "grid.h"
#include <iostream> // inclui a biblioteca de entrada e saída padrão
#include "colors.h" // inclui a header de cores

Grid::Grid() // construtor da classe Grid
{
    numRows = 20;
    numCols = 10;
    cellSize = 30;
    Initialize();
    colors = GetCellColors(); // chama o método para obter as cores das células
}

void Grid::Initialize()
{
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            grid[i][j] = 0; // inicializa todas as células (todos os blocos) com 0
        }
    }
}

void Grid::Print()
{
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            std::cout << grid[i][j] << " "; // imprime o valor de cada célula
        }
        std::cout << std::endl;
    }
}

void Grid::Draw()
{
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            int cellValue = grid[i][j];                                                                         // obtém o valor da célula
            DrawRectangle(j * cellSize + 31, i * cellSize + 81, cellSize - 1, cellSize - 1, colors[cellValue]); // estilizando a grade
        }
    }
}

bool Grid::IsCellOutside(int row, int col)
{
    if (row >= 0 && row < numRows && col >= 0 && col < numCols)
    {
        return false; // se a célula estiver dentro da grade, retorna falso
    }
    return true; // se estiver fora, a condição "IsCellOutside" é verdadeira
}

int Grid::ClearFullRows()
{
    int completed = 0; // contador de linhas completas
    for (int row = numRows - 1; row >= 0; row--)
    {
        if (IsRowFull(row)) // verifica se a linha está completa
        {
            ClearRow(row); // limpa a linha
            completed++;   // incrementa o contador de linhas completas
        }
        else if (completed > 0) // se a alguma linha estiver completa
        {
            MoveRowDown(row, completed); // move a linha incompleta para baixo
        }
    }
    return completed; // retorna o número de linhas completas
}

bool Grid::IsCellEmpty(int row, int col)
{
    if (grid[row][col] == 0) // verifica se há espaço vazio na célula
    {
        return true;
    }
    return false;
}

bool Grid::IsRowFull(int row)
{
    for (int col = 0; col < numCols; col++) // verifica todas as colunas da linha especificada
    {
        if (grid[row][col] == 0) // se encontrar uma célula vazia, a linha não está completa
        {
            return false; // a linha não está completa
        }
    }
    return true; // se não encontrar células vazias, a linha está completa
}

void Grid::ClearRow(int row)
{
    for (int col = 0; col < numCols; col++)
    {
        grid[row][col] = 0; // limpa a linha, definindo todas as células como 0
    }
}

void Grid::MoveRowDown(int row, int numRows)
{
    for (int col = 0; col < numCols; col++)
    {
        grid[row + numRows][col] = grid[row][col]; // move a linha para baixo
        grid[row][col] = 0;
    }
}
