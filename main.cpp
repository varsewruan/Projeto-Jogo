#include <stdio.h>
#include <iostream>
#include <raylib.h>
#include "colors.h"
#include "game.h"

double lastUpdateTime = 0; // para armazenar o intervalo em que o jogo foi atualizado

bool EventTime(double interval) // define o intervalo
{
    double currentTime = GetTime();               // pega o tempo atual do jogo
    if (currentTime - lastUpdateTime >= interval) // verifica se o tempo atual menos o tempo do último update é maior ou igual ao intervalo
    {
        lastUpdateTime = currentTime; // atualiza o tempo do último update
        return true;                  // retorna verdadeiro se o intervalo foi atingido
    }
    return false; // retorna falso se o intervalo não foi atingido
}

enum GameState
{
    PRE_MENU, //
    MENU,     // estado do menu
    JOGAR,    // estado de jogo
    COMO_JOGAR,
    RANKING,
    METER_O_PE // estado de saída
};

GameState currentGameState = PRE_MENU;
int selectedGameState = 1;

int main()
{
    char scoreText[20];                 // cria um array de caracteres para armazenar a pontuação
    InitWindow(840, 760, "Blockathon"); // inicia a janela, dá o nome e tamanho
    SetTargetFPS(60);                   // garante que o jogo rode a no máximo 60 fps sempre
    SetExitKey(KEY_NULL);

    Texture2D rostoFeliz = LoadTexture("assets/rostoBlockathonFeliz.png");        // carrega a textura do rosto feliz
    Texture2D rostoTriste = LoadTexture("assets/rostoBlockathonTriste.png");      // carrega a textura do rosto triste
    Texture2D vilaoFeliz = LoadTexture("assets/rostoBlockathonVilaoFeliz.png");   // carrega a textura do rosto triste
    Texture2D vilaoTriste = LoadTexture("assets/rostoBlockathonVilaoTriste.png"); // carrega a textura do rosto triste

    Game game = Game(); // cria uma instância do jogo

    bool close_game = false;
    while (!close_game) // enquanto a janela não for fechada
    {
        BeginDrawing();             // começa a desenhar na tela
        ClearBackground(lightGray); //"limpa" a tela com a cor cinza

        switch (currentGameState)
        {
        case PRE_MENU:
            if (IsKeyPressed(KEY_ENTER)) // se a tecla Enter for pressionada
            {
                currentGameState = MENU; // muda o estado do jogo para MENU
            }
            DrawText("BLOCKATHON", 350, 200, 40, WHITE);                         // desenha o título do jogo
            DrawText("APERTE QUALQUER BOTAO PARA INICIAR", 300, 300, 20, WHITE); // instrução para iniciar
            break;
        case MENU:
            if (IsKeyPressed(KEY_UP) && selectedGameState > 1)
            {
                selectedGameState -= 1;
            }
            else if (IsKeyPressed(KEY_DOWN) && selectedGameState < 4)
            {
                selectedGameState += 1;
            }

            DrawText("BLOCKATHON", 310, 200, 40, WHITE);
            switch (selectedGameState)
            {
            case 1:
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentGameState = JOGAR;
                }
                DrawText("JOGAR", 370, 300, 20, GOLD);       // desenha a opção de jogar
                DrawText("COMO JOGAR", 370, 350, 20, WHITE); // desenha a opção de como jogar
                DrawText("Ranking", 370, 400, 20, WHITE);    // desenha a opção de ranking
                DrawText("METER O PE", 370, 450, 20, WHITE); //
                break;
            case 2:
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentGameState = COMO_JOGAR;
                }
                DrawText("JOGAR", 370, 300, 20, WHITE);     // desenha a opção de jogar
                DrawText("COMO JOGAR", 370, 350, 20, GOLD); // desenha a opção de como jogar
                DrawText("Ranking", 370, 400, 20, WHITE);   // desenha a opção de ranking
                DrawText("METER O PE", 370, 450, 20, WHITE);
                break;
            case 3:
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentGameState = RANKING;
                }
                DrawText("JOGAR", 370, 300, 20, WHITE);      // desenha a opção de jogar
                DrawText("COMO JOGAR", 370, 350, 20, WHITE); // desenha a opção de como jogar
                DrawText("Ranking", 370, 400, 20, GOLD);     // desenha a opção de ranking
                DrawText("METER O PE", 370, 450, 20, WHITE);
                break;
            case 4:
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentGameState = METER_O_PE;
                }
                DrawText("JOGAR", 370, 300, 20, WHITE);      // desenha a opção de jogar
                DrawText("COMO JOGAR", 370, 350, 20, WHITE); // desenha a opção de como jogar
                DrawText("Ranking", 370, 400, 20, WHITE);    // desenha a opção de ranking
                DrawText("METER O PE", 370, 450, 20, GOLD);
                break;
            }
            break;

        case JOGAR:
        {
            game.Draw();         // desenha a grade e o bloco alterado na tela
            game.ControlInput(); // chama o método de entrada para controlar o bloco
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
            sprintf(scoreText, "%d", game.score);                                 // formata a pontuação como string
            Vector2 textSize = MeasureTextEx(GetFontDefault(), scoreText, 38, 1); // mede o tamanho do texto da pontuação
            DrawTexture(vilaoTriste, 580, 81, WHITE);                             // desenha o rosto do vilao

            if (game.gameOver)
            {
                DrawText("GAME OVER!", 377, 385, 30, RED);
                DrawText("[APERTE QUALQUER TECLA PARA REINICIAR]", 350, 430, 6, GRAY);
                DrawTexture(rostoTriste, 610, 345, WHITE); // desenha o rosto triste do player
                DrawTexture(vilaoFeliz, 580, 81, WHITE);   // desenha o rosto feliz do vilao
            }

            DrawRectangleRounded({355, 580, 290, 120}, 0.2, 4, menuGray);                               // desenha um retângulo arredondado para o score
            DrawTextEx(GetFontDefault(), scoreText, {355 + (290 - textSize.x) / 2, 619}, 38, 1, WHITE); // desenha a pontuação no centro do retângulo

            if (game.score < 500)
            {
                if (EventTime(0.5)) // chama o método EventTime com um intervalo de 0.5 segundos
                {
                    game.MoveBLockDown(); // move o bloco para baixo
                }
            }

            if (game.score >= 500 && game.score < 1000)
            {
                if (EventTime(0.35)) // chama o método EventTime com um intervalo de 0.4 segundos
                {
                    game.MoveBLockDown(); // move o bloco para baixo
                }
            }

            if (game.score >= 1000 && game.score < 1500)
            {
                if (EventTime(0.25)) // chama o método EventTime com um intervalo de 0.3 segundos
                {
                    game.MoveBLockDown(); // move o bloco para baixo
                }
            }

            if (game.score >= 1500)
            {
                if (EventTime(0.15)) // chama o método EventTime com um intervalo de 0.3 segundos
                {
                    game.MoveBLockDown(); // move o bloco para baixo
                }
            }
            break;
        }
    case COMO_JOGAR:
    {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE))
        {
            currentGameState = MENU;
        }
        break;
    }
    case RANKING:
    {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE))
        {
            currentGameState = MENU;
        }
        
        break;
    }
    case METER_O_PE:
    {
        close_game = true;
        break;
    }
    }
    EndDrawing();
}

CloseWindow();
return 0;
}
