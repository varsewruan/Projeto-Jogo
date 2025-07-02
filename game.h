#pragma once
#include "grid.h"
#include "blocks.cpp"

class Game // classe principal do jogo, que gerencia a lógica do jogo, os blocos e a grade
{
public:
    Game(); // construtor da classe Game, que inicializa a grade e os blocos
    ~Game();
    Block GetRandomBlock();            // método para obter um bloco aleatório do vetor de blocos
    std::vector<Block> GetAllBlocks(); //
    void Draw();                       // método para desenhar a grade e o bloco atual na tela
    void ControlInput();               // método para controlar a entrada do usuário e movimentar o bloco atual
    void MoveBLockLeft();              // método para mover o bloco atual para a esquerda
    void MoveBLockRight();             // método para mover o bloco atual para a direita
    void MoveBLockDown();              // método para mover o bloco atual para baixo
    Grid grid;                         // grade do jogo, que armazena os blocos já colocados
    bool gameOver;                     // variável que indica se o jogo acabou
    int score;                         // pontuação do jogo
    bool IsBlockOutside();             // verifica se o bloco atual está fora da grade
    bool BlockFits();                  // verifica se o bloco atual cabe na grade
    Music music;                     // música de fundo do jogo
    Sound oneTwoRowsClearedSound;      // som para uma linha limpa
    Sound threeRowsClearedSound;       // som para quatro linhas limpas
    Sound gameOverSound;               // som para o fim do jogo
    Sound menuSelectSound;             // som para seleção de menu
    Sound menuExitSound;               // som para saída de menu

private:
    void
    RotateBlock();                                          //  método para rotacionar o bloco atual
    void LockBlock();                                       // método para travar o bloco atual na grade
    void ResetGame();                                       // reinicializa o jogo, limpando a grade e selecionando novos blocos
    void UpdateScore(int linesCleared, int moveDownBlocks); // atualiza a pontuação do jogo com base nas linhas limpas e blocos movidos para baixo
    std::vector<Block> blocks;                              // vetor que armazena todos os blocos disponíveis no jogo
    Block currentBlock;                                     // bloco atual que está em jogo
    Block nextBlock;                                        // próximo bloco que vai entrar em jogo
};