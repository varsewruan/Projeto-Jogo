#include "block.h"
#include "position.h"

class LBlock : public Block
{
public:
    LBlock()
    {
        id = 1;
        cells[0] = {Position(0, 2), Position(1, 0), Position(1, 1), Position(1, 2)}; // rotaçao 0 - bloco L
        cells[1] = {Position(0, 1), Position(1, 1), Position(2, 1), Position(2, 2)}; // rotaçao 1 - bloco L
        cells[2] = {Position(1, 0), Position(1, 1), Position(1, 2), Position(2, 0)}; // rotaçao 2 - bloco L
        cells[3] = {Position(0, 0), Position(0, 1), Position(1, 1), Position(2, 1)}; // rotaçao 3 - bloco L
        Move(0, 3);                                                                  // move o bloco para a direita, para que ele fique centralizado na tela
    }
};

class JBlock : public Block
{
public:
    JBlock()
    {
        id = 2;
        cells[0] = {Position(0, 0), Position(1, 0), Position(1, 1), Position(1, 2)}; // rotaçao 0 - bloco J
        cells[1] = {Position(0, 1), Position(0, 2), Position(1, 1), Position(2, 1)}; // rotaçao 1 - bloco J
        cells[2] = {Position(1, 0), Position(1, 1), Position(1, 2), Position(2, 2)}; // rotaçao 2 - bloco J
        cells[3] = {Position(0, 1), Position(1, 1), Position(2, 0), Position(2, 1)}; // rotaçao 3 - bloco J
        Move(0, 3);
    }
};

class IBlock : public Block
{
public:
    IBlock()
    {
        id = 3;
        cells[0] = {Position(1, 0), Position(1, 1), Position(1, 2), Position(1, 3)}; // rotaçao 0 - bloco I
        cells[1] = {Position(0, 2), Position(1, 2), Position(2, 2), Position(3, 2)}; // rotaçao 1 - bloco I
        cells[2] = {Position(2, 0), Position(2, 1), Position(2, 2), Position(2, 3)}; // rotaçao 2 - bloco I
        cells[3] = {Position(0, 1), Position(1, 1), Position(2, 1), Position(3, 1)}; // rotaçao 3 - bloco I
        Move(-1, 3);
    }
};

class OBlock : public Block
{
public:
    OBlock()
    {
        id = 4;
        cells[0] = {Position(0, 0), Position(0, 1), Position(1, 0), Position(1, 1)}; // quadrado, rotação irrelevante
        Move(0, 4);                                                                  // move 4 para a direita para que ELE fique centralizado na tela
    }
};

class SBlock : public Block
{
public:
    SBlock()
    {
        id = 5;
        cells[0] = {Position(0, 1), Position(0, 2), Position(1, 0), Position(1, 1)}; // rotaçao 0 - bloco S
        cells[1] = {Position(0, 1), Position(1, 1), Position(1, 2), Position(2, 2)}; // rotaçao 1 - bloco S
        cells[2] = {Position(1, 1), Position(1, 2), Position(2, 0), Position(2, 1)}; // rotaçao 2 - bloco S
        cells[3] = {Position(0, 0), Position(1, 0), Position(1, 1), Position(2, 1)}; // rotaçao 3 - bloco S
        Move(0, 3);
    }
};

class TBlock : public Block
{
public:
    TBlock()
    {
        id = 6;
        cells[0] = {Position(0, 1), Position(1, 0), Position(1, 1), Position(1, 2)}; // rotaçao 0 - bloco T
        cells[1] = {Position(0, 1), Position(1, 1), Position(1, 2), Position(2, 1)}; // rotaçao 1 - bloco T
        cells[2] = {Position(1, 0), Position(1, 1), Position(1, 2), Position(2, 1)}; // rotaçao 2 - bloco T
        cells[3] = {Position(0, 1), Position(1, 0), Position(1, 1), Position(2, 1)}; // rotaçao 3 - bloco T
        Move(0, 3);
    }
};

class ZBlock : public Block
{
public:
    ZBlock()
    {
        id = 7;
        cells[0] = {Position(0, 0), Position(0, 1), Position(1, 1), Position(1, 2)}; // rotaçao 0 - bloco Z
        cells[1] = {Position(0, 2), Position(1, 1), Position(1, 2), Position(2, 1)}; // rotaçao 1 - bloco Z
        cells[2] = {Position(1, 0), Position(1, 1), Position(2, 1), Position(2, 2)}; // rotaçao 2 - bloco Z
        cells[3] = {Position(0, 1), Position(1, 0), Position(1, 1), Position(2, 0)}; // rotaçao 3 - bloco Z
        Move(0, 3);
    }
};