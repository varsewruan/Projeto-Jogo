#include "grid.h"
#include <iostream> // inclui a biblioteca de entrada e saída padrão
#include "colors.h" // inclui a header de cores



Grid::Grid() // construtor da classe Grid
{
    numRows = 20;             // define o número de linhas da grade
    numCols = 10;             // define o número de linhas e colunas da grade
    cellSize = 30;            // tamanho dos bloquinhos em px
    Initialize();             // chama o método para inicializar a grade
    colors = GetCellColors(); // chama o método para obter as cores das células
}

void Grid::Initialize() // método para inicializar a grade
{
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            grid[i][j] = 0; // inicializa todas as células (todos os blocos) com 0
        }
    }
}

void Grid::Print() // método para imprimir a grade no console
{
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            std::cout << grid[i][j] << " "; // imprime o valor de cada célula
        }
        std::cout << std::endl; // pula para a próxima linha
    }
}

void Grid::Draw() // método para desenhar a grade na tela
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

bool Grid::IsCellOutside(int row, int col) // método para verificar se a célula está fora da grade
{
    if (row >= 0 && row < numRows && col >= 0 && col < numCols)
    {
        return false; // se a célula estiver dentro da grade, retorna falso
    }
    return true; // se estiver fora, a condição "IsCellOutside" é verdadeira
}

int Grid::ClearFullRows() // método para limpar as linhas completas
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

bool Grid::IsCellEmpty(int row, int col) // método para verificar se a célula está vazia
{
    if (grid[row][col] == 0) // verifica se há espaço vazio na célula
    {
        return true;
    }
    return false;
}

bool Grid::IsRowFull(int row) // método para verificar se a linha está completa
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

void Grid::ClearRow(int row) // método para limpar a linha se estiver completa
{
    for (int col = 0; col < numCols; col++)
    {
        grid[row][col] = 0; // limpa a linha, definindo todas as células como 0
    }
}

void Grid::MoveRowDown(int row, int numRows) // método para mover a linha incompleta para baixo
{
    for (int col = 0; col < numCols; col++)
    {
        grid[row + numRows][col] = grid[row][col]; // move a linha para baixo
        grid[row][col] = 0;                        // limpa a linha original, definindo todas as células como 0
    }
}
