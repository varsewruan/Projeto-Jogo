#define RAYGUI_IMPLEMENTATION
#include <iostream>  // para imprimir mensagens no console
#include <string.h>  // para manipulação de strings
#include "raylib.h"  // para usar a biblioteca Raylib
#include "colors.h"  // para usar as cores definidas no jogo
#include "game.h"    // para usar a classe Game e suas funcionalidades
#include "raygui.h"  // para usar a biblioteca Raygui para interface gráfica
#include <vector>    // para usar vetores
#include <algorithm> // para usar algoritmos como sort

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

enum GameState // define os estados do jogo
{
    PRE_MENU,   //  estado do pré-menu
    MENU,       // estado do menu
    JOGAR,      // estado de jogo
    COMO_JOGAR, // estado de como jogar
    RANKING,    // estado do ranking
    METER_O_PE, // estado de saída
    GAME_OVER   // estado de game over
};

char scoreText[20];    // cria um array de caracteres para armazenar a pontuação
char nickname[20];     // cria um array de caracteres para armazenar o nickname
char textInput[20];    // cria um array de caracteres para armazenar o texto de entrada
bool editMode = false; // modo de edição para o nickname

void WriteLeaderbord() // função para escrever o ranking no arquivo CSV
{
    FILE *file = fopen("ranking.csv", "a"); // abre o arquivo de ranking em modo de adição

    fprintf(file, "%s, %s\n", nickname, scoreText); // escreve no arquivo
    fclose(file);                                   // fecha o arquivo
}

void SortRanking() // função para ordenar o ranking
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
              { return a.second > b.second; });                                  // ordena os rankings em ordem decrescente de pontuação

    file = fopen("ranking.csv", "w");  // abre o arquivo de ranking em modo de escrita
    for (const auto &entry : rankings) // escreve os rankings ordenados no arquivo
    {
        fprintf(file, "%s, %d\n", entry.first.c_str(), entry.second); // escreve o nome e a pontuação no arquivo
    }
    fclose(file); // fecha o arquivo
}

bool close_game = false; // variável para controlar o fechamento do jogo

GameState currentGameState = PRE_MENU;
int selectedGameState = 1;

int main()
{
    InitWindow(840, 760, "Blockathon"); // inicia a janela, dá o nome e tamanho
    SetTargetFPS(60);                   // garante que o jogo rode a no máximo 60 fps sempre
    SetExitKey(KEY_NULL);               // desabilita a tecla de saída padrão (ESC)

    Texture2D rostoFeliz = LoadTexture("assets/rostoBlockathonFeliz.png");        // carrega a textura do rosto feliz
    Texture2D vilaoTriste = LoadTexture("assets/rostoBlockathonVilaoTriste.png"); // carrega a textura do rosto triste
    
    Game game = Game(); // cria uma instância do jogo
    
    SetSoundVolume(game.menuExitSound, 0.20f); // define o volume do som
    SetSoundVolume(game.gameOverSound, 0.5f); // define o volume do som
    SetSoundVolume(game.oneTwoRowsClearedSound, 0.4f); // define o volume do som
    SetSoundVolume(game.threeRowsClearedSound, 0.35f); // define o volume do som
    SetSoundVolume(game.menuSelectSound, 0.15f); // define o volume do som
    SetMusicVolume(game.music, 0.3f); // define o volume do som
    
    while (!close_game) // enquanto a janela não for fechada
    {
        UpdateMusicStream(game.music); // atualiza a música de fundo
        BeginDrawing();                  // começa a desenhar na tela
        ClearBackground(lightGray);      //"limpa" a tela com a cor cinza

        switch (currentGameState)
        {
        case PRE_MENU:

            SortRanking(); // ordena o ranking

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) // se o botão esquerdo do mouse for pressionado
            {
                Vector2 mouse = GetMousePosition();         // pega a posição do mouse
                Rectangle textbox = {300, 405, 200, 30};    // define a caixa de texto para o nickname
                if (CheckCollisionPointRec(mouse, textbox)) // verifica se o mouse está dentro da caixa de texto
                {
                    editMode = true;
                }
                else
                {
                    editMode = false;
                }
            }

            DrawText("BLOCKATHON", 164, 240, 75, WHITE);                                           // desenha o título do jogo
            DrawText("DIGITE SEU NICKNAME", 279, 375, 20, WHITE);                                  // mensagem de erro se o nickname for maior que 12 caracteres
            DrawText("MAX. 12 CARACTERES", 332, 440, 12, RED);                                     // mensagem de erro se o nickname for maior que 12 caracteres
            GuiTextBox((Rectangle){300, 405, 200, 30}, textInput, 20, editMode);                   // caixa de texto para o nickname
            if (GuiButton((Rectangle){300, 470, 200, 40}, "CONFIRMAR") || IsKeyPressed(KEY_ENTER)) // botao de confirmar
            {
                if (strlen(textInput) > 0 && strlen(textInput) <= 12) // se o texto de entrada tiver entre 1 e 12 caracteres
                {
                    strcpy(nickname, textInput); // copia o texto de entrada para o nickname
                    currentGameState = MENU;     // muda para o menu
                }
            }

            break;
        case MENU:
            if (IsKeyPressed(KEY_UP) && selectedGameState > 1) // se a tecla UP for pressionada e o estado selecionado for maior que 1
            {
                PlaySound(game.menuSelectSound); // toca o som de seleção de menu
                selectedGameState -= 1;          // diminui o estado selecionado
            }
            else if (IsKeyPressed(KEY_DOWN) && selectedGameState < 4) // se a tecla DOWN for pressionada e o estado selecionado for menor que 4
            {
                PlaySound(game.menuSelectSound); // toca o som de seleção de menu
                selectedGameState += 1;          // aumenta o estado selecionado
            }

            DrawText("BLOCKATHON", 158, 120, 75, WHITE);
            switch (selectedGameState)
            {
            case 1:
                if (IsKeyPressed(KEY_ENTER)) // se a tecla ENTER for pressionada
                {
                    currentGameState = JOGAR;
                }
                DrawText("JOGAR", 310, 300, 32, GOLD);       // desenha a opção de jogar
                DrawText("COMO JOGAR", 290, 350, 32, WHITE); // desenha a opção de como jogar
                DrawText("RANKING", 290, 400, 32, WHITE);    // desenha a opção de ranking
                DrawText("METER O PE", 290, 450, 32, WHITE); // desenha a opção de meter o pé
                break;
            case 2:
                if (IsKeyPressed(KEY_ENTER)) // se a tecla ENTER for pressionada
                {
                    currentGameState = COMO_JOGAR;
                }
                DrawText("JOGAR", 290, 300, 32, WHITE);      // desenha a opção de jogar
                DrawText("COMO JOGAR", 310, 350, 32, GOLD);  // desenha a opção de como jogar
                DrawText("RANKING", 290, 400, 32, WHITE);    // desenha a opção de ranking
                DrawText("METER O PE", 290, 450, 32, WHITE); // desenha a opção de meter o pé
                break;
            case 3:
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentGameState = RANKING;
                }
                DrawText("JOGAR", 290, 300, 32, WHITE);      // desenha a opção de jogar
                DrawText("COMO JOGAR", 290, 350, 32, WHITE); // desenha a opção de como jogar
                DrawText("RANKING", 310, 400, 32, GOLD);     // desenha a opção de ranking
                DrawText("METER O PE", 290, 450, 32, WHITE); // desenha a opção de meter o pé
                break;
            case 4:
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentGameState = METER_O_PE; // muda o estado do jogo para METER_O_PE
                }
                DrawText("JOGAR", 290, 300, 32, WHITE);      // desenha a opção de jogar
                DrawText("COMO JOGAR", 290, 350, 32, WHITE); // desenha a opção de como jogar
                DrawText("RANKING", 290, 400, 32, WHITE);    // desenha a opção de ranking
                DrawText("METER O PE", 310, 450, 32, GOLD);  // desenha a opção de meter o pé
                break;
            }
            break;
        case JOGAR:
        {
            if (!game.gameOver)
            {
                DrawText("''DEVOLVA MINHAS CASTANHAS''", 540, 260, 16, yellow); // desenha o texto "DEVOLVA MINHAS CASTANHAS" na tela
                DrawText("VOCE", 645, 325, 19, green);                          // desenha o texto "VOCE" na tela
                DrawText("PROXIMO", 379, 60, 16, WHITE);                        // desenha o texto "PROXIMO" na tela
                game.Draw();                                                    // desenha a grade e o bloco alterado na tela
                game.ControlInput();                                            // chama o método de entrada para controlar o bloco
                DrawText("CASTANHAS: ", 377, 535, 38, WHITE);                   // desenha o texto "CASTANHAS: " na tela
                DrawText("VILAO", 638, 60, 22, red);                            // desenha o texto "VILAO" na tela

                DrawTexture(rostoFeliz, 610, 345, WHITE); // desenha o rosto feliz do player

                sprintf(scoreText, "%d", game.score);                                 // formata a pontuação como string
                Vector2 textSize = MeasureTextEx(GetFontDefault(), scoreText, 38, 1); // mede o tamanho do texto da pontuação
                DrawTexture(vilaoTriste, 580, 81, WHITE);                             // desenha o rosto do vilao

                DrawRectangleRounded({355, 580, 290, 120}, 0.2, 4, menuGray);                               // desenha um retângulo arredondado para o score
                DrawTextEx(GetFontDefault(), scoreText, {355 + (290 - textSize.x) / 2, 619}, 38, 1, WHITE); // desenha a pontuação no centro do retângulo

                if (game.score < 500) // se os pontos forem menores que 500
                {
                    if (EventTime(0.33)) // chama o método EventTime com um intervalo de 0.5 segundos
                    {
                        game.MoveBLockDown(); // move o bloco para baixo
                    }
                }

                if (game.score >= 500 && game.score < 1000) // se os pontos forem maiores ou iguais a 500 e menores que 1000
                {
                    if (EventTime(0.22)) // chama o método EventTime com um intervalo de 0.4 segundos
                    {
                        game.MoveBLockDown(); // move o bloco para baixo
                    }
                }

                if (game.score >= 1000 && game.score < 1500) // se os pontos forem maiores ou iguais a 1000 e menores que 1500
                {
                    if (EventTime(0.11)) // chama o método EventTime com um intervalo de 0.3 segundos
                    {
                        game.MoveBLockDown(); // move o bloco para baixo
                    }
                }

                if (game.score >= 1500) // se os pontos forem maiores ou iguais a 1500
                {
                    if (EventTime(0.07)) // chama o método EventTime com um intervalo de 0.3 segundos
                    {
                        game.MoveBLockDown(); // move o bloco para baixo
                    }
                }
            }
            else // se o jogo estiver acabado
            {
                PlaySound(game.gameOverSound);                                                              // toca o som de game over
                currentGameState = GAME_OVER; // muda o estado do jogo para GAME_OVER
            }

            break;
        }
        case COMO_JOGAR:
        {
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE))
            {
                PlaySound(game.menuExitSound); // toca o som de saída do menu
                currentGameState = MENU;
            }
            DrawText("APERTE ESC OU BACKSPACE PARA VOLTAR", 180, 700, 20, GRAY); // mensagem para voltar ao menu
            DrawText("COMO JOGAR", 242, 80, 50, WHITE);                          // desenha o título "como jogar"
            DrawRectangleRounded({150, 160, 500, 5}, 0.2, 4, menuGray);          // detalhe para preencher beleza no menu

            DrawRectangleRounded({120, 230, 120, 65}, 0.2, 4, menuGray);               // desenha um retângulo arredondado para o primeiro comando
            DrawText("[RightArrow]", 137, 255, 15, WHITE);                             // desenha o texto "RightArrow" no retângulo
            DrawText("Move a peça para a direita", 300, 255, 20, WHITE);               // desenha o texto "Move a peça para a direita" na tela
            DrawRectangleRounded({120, 330, 120, 65}, 0.2, 4, menuGray);               // desenha um retângulo arredondado para o segundo comando
            DrawText("[LeftArrow]", 138, 355, 15, WHITE);                              // desenha o texto "LeftArrow" no retângulo
            DrawText("Move a peça para a esquerda", 300, 355, 20, WHITE);              // desenha o texto "Move a peça para a esquerda" na tela
            DrawRectangleRounded({120, 430, 120, 65}, 0.2, 4, menuGray);               // desenha um retângulo arredondado para o terceiro comando
            DrawText("[DownArrow]", 137, 455, 15, WHITE);                              // desenha o texto "DownArrow" no retângulo
            DrawText("Acelera a queda da peça e ganha +1 ponto", 300, 455, 20, WHITE); // desenha o texto "Acelera a queda da peça e ganha +1 ponto" na tela
            DrawRectangleRounded({120, 530, 120, 65}, 0.2, 4, menuGray);               // desenha um retângulo arredondado para o quarto comando
            DrawText("[UpArrow]", 145, 555, 15, WHITE);                                // desenha o texto "UpArrow" no retângulo
            DrawText("Rotaciona a peça", 300, 555, 20, WHITE);                         // desenha o texto "Rotaciona a peça" na tela
            break;
        }
        case RANKING:
        {
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) // se a tecla ESC ou BACKSPACE for pressionada
            {
                PlaySound(game.menuExitSound); // toca o som de saída do menu
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

            DrawText("APERTE ESC OU BACKSPACE PARA VOLTAR", 180, 700, 20, GRAY); // mensagem para voltar ao menu
            break;
        }
        case METER_O_PE:
        {
            return 0; // encerra o jogo
            break;
        }
        case GAME_OVER:
        {
            DrawText("GAME OVER!", 177, 200, 75, RED);                                                  // desenha o texto "GAME OVER!" na tela
            DrawText("[APERTE X PARA FUGIR]", 274, 600, 20, GRAY);                                      // desenha o texto "APERTE ENTER PARA SAIR" na tela
            DrawText("CASTANHAS RECUPERADAS", 225, 325, 25, WHITE);                                     // desenha o texto "APERTE ENTER PARA SAIR" na tela
            sprintf(scoreText, "%d", game.score);                                                       // formata a pontuação como string
            Vector2 textSize = MeasureTextEx(GetFontDefault(), scoreText, 38, 1);                       // mede o tamanho do texto da pontuação
            DrawRectangleRounded({255, 360, 290, 120}, 0.2, 4, menuGray);                               // desenha um retângulo arredondado para o score
            DrawTextEx(GetFontDefault(), scoreText, {255 + (290 - textSize.x) / 2, 400}, 38, 1, WHITE); // desenha a pontuação no centro do retângulo
            if (IsKeyPressed(KEY_X))                                                                    // se a tecla X for pressionada
            {
                close_game = true; // fecha o jogo
            }
        }
        }
        EndDrawing(); // termina de desenhar na tela
    }

    if (game.score > 0)
    {
        WriteLeaderbord(); // escreve o ranking no arquivo
        SortRanking();     // ordena o ranking
    }

    UnloadTexture(rostoFeliz);  // descarrega a textura do rosto feliz
    UnloadTexture(vilaoTriste); // descarrega a textura do rosto triste

    return 0; // retorna 0 para indicar que o jogo foi encerrado com sucesso
}
