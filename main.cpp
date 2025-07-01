#define RAYGUI_IMPLEMENTATION
#include <iostream>
#include <string.h>
#include "raylib.h"
#include "colors.h"
#include "game.h"
#include "raygui.h"
#include <vector>
#include <algorithm>

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

char scoreText[20];    // cria um array de caracteres para armazenar a pontuação
char nickname[20];     // cria um array de caracteres para armazenar o nickname
char textInput[20];    // cria um array de caracteres para armazenar o texto de entrada
bool editMode = false; // modo de edição para o nickname

void WriteLeaderbord()
{
    FILE *file = fopen("ranking.csv", "a"); // abre o arquivo de ranking em modo de adição

    fprintf(file, "%s, %s\n", nickname, scoreText); // escreve no arquivo
    fclose(file);                                   // fecha o arquivo
}

void SortRanking()
{
    FILE *file = fopen("ranking.csv", "r"); // abre o arquivo de ranking em modo de leitura
    if (file == NULL)
    {
        return; // se o arquivo não existir, retorna
    }

    std::vector<std::pair<std::string, int>> rankings; // vetor para armazenar os rankings

    char line[100];                         // cria um array de caracteres para armazenar a linha lida
    while (fgets(line, sizeof(line), file)) // lê cada linha do arquivo
    {
        char *token = strtok(line, ","); // divide a linha em tokens usando a vírgula como delimitador
        if (token != NULL)
        {
            std::string name = token;  // armazena o nome do jogador
            token = strtok(NULL, ","); // pega o próximo token (pontuação)
            if (token != NULL)
            {
                int score = atoi(token);                         // converte a pontuação para inteiro
                rankings.push_back(std::make_pair(name, score)); // adiciona ao vetor de rankings
            }
        }
    }
    fclose(file); // fecha o arquivo

    std::sort(rankings.begin(), rankings.end(), [](const auto &a, const auto &b)
              { return a.second > b.second; }); // ordena os rankings

    file = fopen("ranking.csv", "w");  // abre o arquivo de ranking em modo de escrita
    for (const auto &entry : rankings) // escreve os rankings ordenados no arquivo
    {
        fprintf(file, "%s, %d\n", entry.first.c_str(), entry.second); // escreve o nome e a pontuação no arquivo
    }
    fclose(file); // fecha o arquivo
}

GameState currentGameState = PRE_MENU;
int selectedGameState = 1;

int main()
{
    InitWindow(840, 760, "Blockathon"); // inicia a janela, dá o nome e tamanho
    SetTargetFPS(60);                   // garante que o jogo rode a no máximo 60 fps sempre
    SetExitKey(KEY_NULL);               // desabilita a tecla de saída padrão (ESC)

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
            SortRanking(); // ordena o ranking

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mouse = GetMousePosition();
                Rectangle textbox = {300, 375, 200, 30};
                if (CheckCollisionPointRec(mouse, textbox))
                {
                    editMode = true;
                }
                else
                {
                    editMode = false;
                }
            }

            DrawText("BLOCKATHON", 164, 290, 75, WHITE);                         // desenha o título do jogo
            GuiTextBox((Rectangle){300, 375, 200, 30}, textInput, 20, editMode); // caixa de texto para o nickname
            if (GuiButton((Rectangle){300, 420, 200, 30}, "CONFIRMAR"))          // botao de confirmar
            {
                strcpy(nickname, textInput); // copia o texto de entrada para o nickname
                currentGameState = MENU;     // muda o estado do jogo para MENU
            }
            break;
        case MENU:
            if (IsKeyPressed(KEY_UP) && selectedGameState > 1) // se a tecla UP for pressionada e o estado selecionado for maior que 1
            {
                selectedGameState -= 1; // diminui o estado selecionado
            }
            else if (IsKeyPressed(KEY_DOWN) && selectedGameState < 4) // se a tecla DOWN for pressionada e o estado selecionado for menor que 4
            {
                selectedGameState += 1; // aumenta o estado selecionado
            }

            DrawText("BLOCKATHON", 164, 120, 75, WHITE);
            switch (selectedGameState)
            {
            case 1:
                if (IsKeyPressed(KEY_ENTER)) // se a tecla ENTER for pressionada
                {
                    currentGameState = JOGAR;
                }
                DrawText("JOGAR", 350, 300, 32, GOLD);       // desenha a opção de jogar
                DrawText("COMO JOGAR", 330, 350, 32, WHITE); // desenha a opção de como jogar
                DrawText("RANKING", 330, 400, 32, WHITE);    // desenha a opção de ranking
                DrawText("METER O PE", 330, 450, 32, WHITE); // desenha a opção de meter o pé
                break;
            case 2:
                if (IsKeyPressed(KEY_ENTER)) // se a tecla ENTER for pressionada
                {
                    currentGameState = COMO_JOGAR;
                }
                DrawText("JOGAR", 330, 300, 32, WHITE);      // desenha a opção de jogar
                DrawText("COMO JOGAR", 350, 350, 32, GOLD);  // desenha a opção de como jogar
                DrawText("RANKING", 330, 400, 32, WHITE);    // desenha a opção de ranking
                DrawText("METER O PE", 330, 450, 32, WHITE); // desenha a opção de meter o pé
                break;
            case 3:
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentGameState = RANKING;
                }
                DrawText("JOGAR", 330, 300, 32, WHITE);      // desenha a opção de jogar
                DrawText("COMO JOGAR", 330, 350, 32, WHITE); // desenha a opção de como jogar
                DrawText("RANKING", 350, 400, 32, GOLD);     // desenha a opção de ranking
                DrawText("METER O PE", 330, 450, 32, WHITE); // desenha a opção de meter o pé
                break;
            case 4:
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentGameState = METER_O_PE; // muda o estado do jogo para METER_O_PE
                }
                DrawText("JOGAR", 330, 300, 32, WHITE);      // desenha a opção de jogar
                DrawText("COMO JOGAR", 330, 350, 32, WHITE); // desenha a opção de como jogar
                DrawText("RANKING", 330, 400, 32, WHITE);    // desenha a opção de ranking
                DrawText("METER O PE", 350, 450, 32, GOLD);  // desenha a opção de meter o pé
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
                DrawText("''Kkkkkkk''", 635, 260, 16, yellow);
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
                DrawText("VOCE", 645, 325, 19, green);   // desenha o texto "VOCE" na tela
                DrawText("PROXIMO", 379, 60, 16, WHITE); // desenha o texto "PROXIMO" na tela
            }
            sprintf(scoreText, "%d", game.score);                                 // formata a pontuação como string
            Vector2 textSize = MeasureTextEx(GetFontDefault(), scoreText, 38, 1); // mede o tamanho do texto da pontuação
            DrawTexture(vilaoTriste, 580, 81, WHITE);                             // desenha o rosto do vilao

            if (game.gameOver) // caso game over
            {
                DrawText("GAME OVER!", 377, 385, 30, RED);               // desenha o texto "GAME OVER!" na tela
                DrawText("[APERTE ENTER PARA SAIR]", 335, 430, 6, GRAY); // desenha o texto "APERTE ENTER PARA SAIR" na tela
                DrawTexture(rostoTriste, 610, 345, WHITE);               // desenha o rosto triste do player
                DrawTexture(vilaoFeliz, 580, 81, WHITE);                 // desenha o rosto feliz do vilao
                if (IsKeyPressed(KEY_ENTER))
                {
                    close_game = true; // se a tecla ENTER for pressionada, fecha o jogo
                }
            }

            DrawRectangleRounded({355, 580, 290, 120}, 0.2, 4, menuGray);                               // desenha um retângulo arredondado para o score
            DrawTextEx(GetFontDefault(), scoreText, {355 + (290 - textSize.x) / 2, 619}, 38, 1, WHITE); // desenha a pontuação no centro do retângulo

            if (game.score < 500) // se os pontos forem menores que 500
            {
                if (EventTime(0.4)) // chama o método EventTime com um intervalo de 0.5 segundos
                {
                    game.MoveBLockDown(); // move o bloco para baixo
                }
            }

            if (game.score >= 500 && game.score < 1000)
            {
                if (EventTime(0.20)) // chama o método EventTime com um intervalo de 0.4 segundos
                {
                    game.MoveBLockDown(); // move o bloco para baixo
                }
            }

            if (game.score >= 1000 && game.score < 1500)
            {
                if (EventTime(0.10)) // chama o método EventTime com um intervalo de 0.3 segundos
                {
                    game.MoveBLockDown(); // move o bloco para baixo
                }
            }

            if (game.score >= 1500)
            {
                if (EventTime(0.07)) // chama o método EventTime com um intervalo de 0.3 segundos
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
            DrawText("COMO JOGAR", 280, 80, 50, WHITE); // desenha o título "como jogar"
            DrawRectangleRounded({150, 160, 500, 5}, 0.2, 4, menuGray);
            break;
        }
        case RANKING:
        {
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE))
            {
                currentGameState = MENU;
            }
            DrawText("RANKING", 280, 80, 50, WHITE); // desenha o título "ranking"
            DrawRectangleRounded({150, 160, 500, 5}, 0.2, 4, menuGray);
            FILE *file = fopen("ranking.csv", "r"); // abre o arquivo de ranking em modo de leitura

            char line[100];                         // cria um array de caracteres para armazenar a linha lida
            int y = 200;                            // posição y inicial para desenhar o ranking
            while (fgets(line, sizeof(line), file)) // lê cada linha do arquivo
            {
                char *token = strtok(line, ","); // divide a linha em tokens usando a vírgula como delimitador
                if (token != NULL)
                {
                    DrawText(token, 160, y, 32, WHITE); // desenha o nome do jogador
                    token = strtok(NULL, ",");          // pega o próximo token (pontuação)
                    if (token != NULL)
                    {
                        DrawText(token, 533, y, 32, WHITE); // desenha a pontuação do jogador
                    }
                }
                y += 56; // incrementa a posição y para a próxima linha
            }
            fclose(file); // fecha o arquivo

            DrawText("APERTE ESC OU BACKSPACE PARA VOLTAR", 180, 35, 20, GRAY); // mensagem para voltar ao menu
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
    WriteLeaderbord(); // escreve o ranking no arquivo
    CloseWindow();
    return 0;
}
