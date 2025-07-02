#pragma once

class Position // Classe que representa uma posição na grade do jogo
{
public:
    Position(int row, int col); // Construtor que inicializa a posição com linha e coluna
    int row;                    // Atributo que representa a linha da posição
    int col;                    // Atributo que representa a coluna da posição
};