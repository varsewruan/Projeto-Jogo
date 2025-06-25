#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <stdio.h>
#include <iostream>

double lastUpdateTime = 0; // para armazenar o intervalo em que o jogo foi atualizado

bool EventTriggered(double interval) // define o intervalo
{
    double currentTime = GetTime();               // pega o tempo atual do jogo
    if (currentTime - lastUpdateTime >= interval) // verifica se o tempo atual menos o tempo do último update é maior ou igual ao intervalo
    {
        lastUpdateTime = currentTime; // atualiza o tempo do último update
        return true;                  // retorna verdadeiro se o intervalo foi atingido
    }
    return false; // retorna falso se o intervalo não foi atingido
}

int main()
{
    InitWindow(840, 760, "Blockathon"); // inicia a janela, dá o nome e tamanho

    Texture2D rostoFeliz = LoadTexture("assets/rostoBlockathonFeliz.png");        // carrega a textura do rosto feliz
    Texture2D rostoTriste = LoadTexture("assets/rostoBlockathonTriste.png");      // carrega a textura do rosto triste
    Texture2D vilaoFeliz = LoadTexture("assets/rostoBlockathonVilaoFeliz.png");   // carrega a textura do rosto triste
    Texture2D vilaoTriste = LoadTexture("assets/rostoBlockathonVilaoTriste.png"); // carrega a textura do rosto triste

    SetTargetFPS(60); // garante que o jogo rode a no máximo 60 fps sempre

    Game game = Game(); // cria uma instância do jogo

    while (WindowShouldClose() == false) // enquanto a janela não for fechada
    {
        BeginDrawing();             // começa a desenhar na tela
        ClearBackground(lightGray); //"limpa" a tela com a cor cinza
        game.Draw();                // desenha a grade e o bloco alterado na tela
        game.ControlInput();        // chama o método de entrada para controlar o bloco
        DrawText("CASTANHAS: ", 377, 535, 38, WHITE);
        DrawText("VILAO", 638, 60, 22, red);

        if (game.gameOver)
        {
            DrawText("''kkkkkkk''", 635, 260, 16, yellow);
        }
        else
        {
            DrawText("''DEVOLVA MINHAS CASTANHAS''", 540, 260, 16, yellow);
        }
        DrawTexture(rostoFeliz, 610, 345, WHITE); // desenha o rosto feliz do player
        if (game.gameOver)
        {
            DrawText("VOCE", 645, 325, 19, RED);
        }
        else
        {
            DrawText("VOCE", 645, 325, 19, green);
            DrawText("PROXIMO", 379, 60, 16, WHITE);
        }
        DrawTexture(vilaoTriste, 580, 81, WHITE); // desenha o rosto do vilao

        if (game.gameOver)
        {
            DrawText("GAME OVER!", 377, 385, 30, RED);
            DrawTexture(rostoTriste, 610, 345, WHITE); // desenha o rosto triste do player
            DrawTexture(vilaoFeliz, 580, 81, WHITE);   // desenha o rosto feliz do vilao
        }

        char scoreText[20];                                                   // cria um array de caracteres para armazenar a pontuação
        sprintf(scoreText, "%d", game.score);                                 // formata a pontuação como string
        Vector2 textSize = MeasureTextEx(GetFontDefault(), scoreText, 38, 1); // mede o tamanho do texto da pontuação

        DrawRectangleRounded({355, 580, 290, 120}, 0.2, 4, menuGray);                               // desenha um retângulo arredondado para o score
        DrawTextEx(GetFontDefault(), scoreText, {355 + (290 - textSize.x) / 2, 619}, 38, 1, WHITE); // desenha a pontuação no centro do retângulo

        if (EventTriggered(0.2))
        {
            game.MoveBLockDown();
        }
        
        EndDrawing(); // finaliza o desenho na tela
    }

    CloseWindow(); // fecha a janela
}