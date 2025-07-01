#pragma once
#include "grid.h"
#include "blocks.cpp"

class Game
{
public:
    Game();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    void Draw();
    void ControlInput();
    void MoveBLockLeft();
    void MoveBLockRight();
    void MoveBLockDown();
    Grid grid;
    bool gameOver;
    int score;
    bool IsBlockOutside();
    bool BlockFits();

private:
    void RotateBlock();
    void LockBlock();
    void ResetGame();
    void UpdateScore(int linesCleared, int moveDownBlocks);
    std::vector<Block> blocks;
    Block currentBlock; // bloco atual que está em jogo
    Block nextBlock;    // próximo bloco que vai entrar em jogo
};