#ifndef GAME_2048_CPP
#define GAME_2048_CPP

#include <cstdint>

#include "2048.hpp"

#include "../../widgets/gui.hpp"

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    void flushScreen();
#endif

void Game_2048::main()
{
    Window window("");
    canvas = new Canvas(0, 0, 320, 480);
    window.addChild(canvas);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            {
                matrice1[i][j] = 0;
                matrice2[i][j] = 0;
                matrice3[i][j] = 0;
            }

    score=0;

    while(true)
    {
        uint8_t x, y = 0;
        x = random(0, 4);
        y = random(0, 4);
        if(matrice1[x][y]==0)
        {
            matrice1[x][y] = 1;
            break;
        }
    }

    draw();
    canvas->push();

    while (true)
    {
        uint8_t slide = 0; // 1 right, 2 left, 3 up, 4 down
        while (true)
        {
            if(home_button::isPressed())
            {
                return;
            }
            eventHandler.update();
            touch.update();
            

            if (touch.isSlidingHorizontally() > 30)
                slide = 3;
            if (touch.isSlidingHorizontally() < -30)
                slide = 1;
            if (touch.isSlidingVertically() > 30)
                slide = 4;
            if (touch.isSlidingVertically() < -30)
                slide = 2;
            if (slide)
                break;
            
            #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
                SDL_Delay(20);
            #endif
        }

        touch.resetScrollVertical();
        touch.resetScrollHorizontal();

        move(slide);

        bool c = false;

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if(matrice1[i][j]==0)
                    c = true;

        if(!c)
        {
            canvas->l_tft.deleteSprite();
            return;
        }

        while(true)
        {
            uint8_t x, y = 0;
            x = random(0, 4);
            y = random(0, 4);
            if(matrice1[x][y]==0)
            {
                matrice1[x][y] = 1;
                break;
            }
        }

        draw();
        canvas->push();
        window.updateAll();

        #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
            void flushScreen();
        #endif

        while(touch.isTouch()) touch.update();
    }
}

void Game_2048::move(uint8_t slide)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            matrice2[i][j] = matrice1[i][j];

    for (int r = 0; r < slide - 1; r++)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                matrice3[i][j] = matrice2[4 - j - 1][i];
            }
        }
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                matrice2[i][j] = matrice3[i][j];
    }

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            matrice3[i][j] = 0;

    for (int i = 3; i >= 0; i--)
    {
        for (int j = 0; j < 4; j++)
        {
            if (matrice2[i][j])
            {
                bool add = false;
                for(int b = 1; b < 4 && i-b >= 0; b++)
                {
                    if(matrice2[i][j] == matrice2[i-b][j])
                    {
                        matrice2[i-b][j] = 0;
                        add=true;
                        break;
                    }
                    if(matrice2[i][j] != matrice2[i-b][j] && matrice2[i-b][j] != 0)
                        break;
                }
                if(!add)
                {
                    for(int b = 0; b < 4; b++)
                    {
                        if(matrice3[3-b][j]==0)
                        {
                            matrice3[3-b][j] = matrice2[i][j];
                            matrice2[i][j] = 0;
                            break;
                        }
                    }
                }
                else
                {
                    for(int b = 3; b >= 0; b--)
                    {
                        if(matrice3[b][j]==0)
                        {
                            score+=colors[1][matrice2[i][j]];
                            matrice3[b][j] = matrice2[i][j] + 1;
                            matrice2[i][j] = 0;
                            break;
                        }
                    }
                }
            }
        }
    }

    for (int l = 0; l < 4 - (slide - 1); l++)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                matrice1[i][j] = matrice3[4 - j - 1][i];
            }
        }
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                matrice3[i][j] = matrice1[i][j];
    }
}

void Game_2048::draw()
{
    canvas->l_tft.fillSprite(0xFFFF);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (matrice1[i][j])
            {
                canvas->l_tft.fillRoundRect(15 + i * 75, 50 + j * 75, 65, 65, 5, colors[0][matrice1[i][j]-1]);
                canvas->l_tft.setTextColor(0xFFFF);
                canvas->l_tft.drawCenterString(std::to_string(colors[1][matrice1[i][j]-1]).c_str(), 15 + i * 75 + 65 / 2, 50 + j * 75 + 65 / 4, &fonts::Font4);
            }
        }
    }
    canvas->l_tft.setTextColor(0x0000);
    canvas->l_tft.drawCenterString(std::to_string(score).c_str(), 160, 400, &fonts::Font4);
}

#endif
