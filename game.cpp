#include "game.h"
#include <random> // incluido para que seja possível gerar na tela blocos aleatórios

Game::Game()
{
    grid = Grid();
    blocks = GetAllBlocks();                                          // inicializa o vetor de blocos com todos os blocos disponíveis
    currentBlock = GetRandomBlock();                                  // seleciona um bloco aleatório para começar o jogo
    nextBlock = GetRandomBlock();                                     // seleciona o próximo bloco que entrará em jogo
    gameOver = false;                                                 // indica que o jogo não acabou ainda
    score = 0;                                                        // inicializa a pontuação do jogo como 0
    InitAudioDevice();                                                // inicia o áudio do jogo
    music = LoadMusicStream("sounds/minecraft_music.mp3");            // carrega a música de fundo do jogo
    PlayMusicStream(music);                                           // inicia a música de fundo do jogo
    oneTwoRowsClearedSound = LoadSound("sounds/limpou1_2linhas.mp3"); // carrega o som para uma linha limpa
    threeRowsClearedSound = LoadSound("sounds/limpou3linhas.mp3");    // carrega o som para quatro linhas limpas
    menuSelectSound = LoadSound("sounds/menuselecao.mp3");            // carrega o som de seleção de menu
    menuExitSound = LoadSound("sounds/voltarmenu.mp3");               // carrega o som de saída de menu
    gameOverSound = LoadSound("sounds/gameover.mp3");                 // carrega o som de fim de jogo
}

Game::~Game()
{
    UnloadSound(oneTwoRowsClearedSound); // descarrega o som de uma linha limpa
    UnloadSound(threeRowsClearedSound);  // descarrega o som de quatro linhas limpas
    UnloadSound(menuSelectSound);        // descarrega o som de seleção de menu
    UnloadSound(menuExitSound);          // descarrega o som de saída de menu
    UnloadSound(gameOverSound);          // descarrega o som de fim de jogo
    UnloadMusicStream(music);            // descarrega a música de fundo do jogo
    CloseAudioDevice();                  // para o áudio do jogo quando o objeto Game é destruído
}

Block Game::GetRandomBlock()
{
    if (blocks.empty()) // verifica se o vetor de blocos está vazio
    {
        blocks = GetAllBlocks(); // se estiver vazio, chama o método GetAllBlocks para preencher de volta o vetor de blocos
    }
    int randomIndex = rand() % blocks.size();   // gera um número aleatório entre 0 e o tamanho do vetor de blocos
    Block block = blocks[randomIndex];          // seleciona um bloco aleatório do vetor
    blocks.erase(blocks.begin() + randomIndex); // remove o bloco selecionado do vetor para evitar repetições
    return block;                               // retorna o bloco selecionado do vetor
}

std::vector<Block> Game::GetAllBlocks()
{
    return {LBlock(), JBlock(), IBlock(), OBlock(), SBlock(), TBlock(), ZBlock()}; // retorna todos os blocos disponíveis
}

void Game::Draw()
{
    grid.Draw();             // desenha a grade na tela
    if (BlockFits() == true) // verifica se o bloco atual cabe na grade
    {
        DrawRectangleRounded({350, 81, 130, 130}, 0, 4, menuGray); // desenha um retângulo para o proximo bloco
        currentBlock.Draw(31, 81);                                 // desenha o bloco atual na tela
        switch (nextBlock.id)                                      // desenha o próximo bloco na tela
        {
        case 3:
            nextBlock.Draw(266, 128); // bloco I
            break;

        case 4:
            nextBlock.Draw(266, 115); // bloco O
            break;

        default:
            nextBlock.Draw(280, 113); // outros blocos
        }
    }
    else
    {
        gameOver = true;
    }
}

void Game::ControlInput()
{
    int pressedKey = GetKeyPressed(); // obtém a tecla pressionada pelo player
    if (gameOver && pressedKey != 0)
    {
        gameOver = false; // se o jogo estiver acabado e uma tecla for pressionada, reinicia o jogo
    }

    switch (pressedKey)
    {
    case KEY_LEFT:       // se a tecla pressionada for a seta para a esquerda
        MoveBLockLeft(); // move o bloco atual para a esquerda
        break;
    case KEY_RIGHT:       // se a tecla pressionada for a seta para a direita
        MoveBLockRight(); // move o bloco atual para a direita
        break;
    case KEY_DOWN:         // se a tecla pressionada for a seta para baixo
        MoveBLockDown();   // move o bloco atual para baixo
        UpdateScore(0, 1); // atualiza a pontuação do jogo cada vez que o jogador movimenta para baixo
        break;
    case KEY_UP:
        RotateBlock();
        break;
    }
}

void Game::MoveBLockLeft()
{
    if (!gameOver)
    {
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || BlockFits() == false) // verifica se o bloco atual está fora da grade após o movimento
        {
            currentBlock.Move(0, 1); // se o bloco estiver fora da grade ou não couber na grade, desfaz o movimento
        }
    }
}

void Game::MoveBLockRight()
{
    if (!gameOver)
    {
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, -1); // se o bloco estiver fora da grade ou não couber na grade, desfaz o movimento
        }
    }
}

void Game::MoveBLockDown()
{
    if (!gameOver)
    {
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(-1, 0); // se o bloco estiver fora da grade ou não couber na grade, desfaz o movimento
            LockBlock();
        }
    }
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions(); // obtem as posições do bloco atual
    for (Position item : tiles)                                    // percorre todas as posições do bloco atual
    {
        if (grid.IsCellOutside(item.row, item.col)) // verifica se alguma célula do bloco atual está fora da grade
        {
            return true;
        }
    }
    return false;
}

void Game::RotateBlock()
{
    if (!gameOver) // verifica se o jogo não acabou
    {
        currentBlock.Rotate(); // rotaciona o bloco atual

        if (IsBlockOutside() || BlockFits() == false) // verifica se o bloco atual está fora da grade após a rotação
        {
            currentBlock.UndoRotation(); // se o bloco estiver fora da grade, desfaz a rotação
        }
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions(); // obtem as posições do bloco atual
    for (Position item : tiles)                                    // percorre todas as posições do bloco atual
    {
        grid.grid[item.row][item.col] = currentBlock.id; // preenche a grade com o id do bloco atual
    }
    currentBlock = nextBlock; // atualiza o bloco atual para o próximo bloco
    if (BlockFits() == false) // verifica se o novo bloco cabe na grade
    {
        gameOver = true; // se não couber bloco, o jogo acaba
    }
    nextBlock = GetRandomBlock();           // seleciona um novo bloco aleatório para o próximo movimento
    int rowsCleared = grid.ClearFullRows(); // limpa as linhas completas após o bloqueio do bloco atual
    UpdateScore(rowsCleared, 0);            // atualiza a pontuação do jogo com base nas linhas limpadas
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions(); // obtem as posições do bloco atual
    for (Position item : tiles)                                    // percorre todas as posições do bloco atual
    {
        if (grid.IsCellEmpty(item.row, item.col) == false) // se a célula não estiver vazia
        {
            return false; // o bloco não cabe na grade
        }
    }
    return true; // se todas as células estiverem vazias, o bloco cabe na grade
}

void Game::ResetGame()
{

    grid.Initialize();               // reinicializa a grade
    blocks = GetAllBlocks();         // reinicializa o vetor de blocos com todos os blocos disponíveis
    currentBlock = GetRandomBlock(); // seleciona um bloco aleatório para começar o jogo
    nextBlock = GetRandomBlock();    // seleciona o próximo bloco que entrará em jogo
    score = 0;                       // reinicializa a pontuação do jogo
}

void Game::UpdateScore(int linesCleared, int moveDownBlocks)
{
    if (!gameOver)
    {
        switch (linesCleared) // verifica quantas linhas foram limpas
        {
        case 1:
            score += 50;                       // adiciona 50 pontos se uma linha for limpa
            PlaySound(oneTwoRowsClearedSound); // toca o som de uma ou duas linhas limpas
            break;
        case 2:
            score += 100;                      // adiciona 100 pontos se duas linhas forem limpas
            PlaySound(oneTwoRowsClearedSound); // toca o som de uma ou duas linhas limpas
            break;
        case 3:
            score += 200;                     // adiciona 200 pontos se três linhas forem limpas
            PlaySound(threeRowsClearedSound); // toca o som de três ou mais linhas limpas
            break;
        case 4:
            score += 200;                     // adiciona 200 pontos se três linhas forem limpas
            PlaySound(threeRowsClearedSound); // toca o som de três ou mais linhas limpas
            break;
        case 5:
            score += 200;                     // adiciona 200 pontos se três linhas forem limpas
            PlaySound(threeRowsClearedSound); // toca o som de três ou mais linhas limpas
            break;
        default:
            break; // se nenhuma linha for limpa, não faz nada
        }

        score += moveDownBlocks;
    }
}