#ifndef GAME_2048_HPP
#define GAME_2048_HPP

LGFX_Sprite S(&tft_root);

class Game_2048 : public App
{
    public:
    void launch();
    void draw();
    void move(uint8_t slide);

    void main()
    {
        launch();
    }
    
    static std::shared_ptr<App> createInstance() {
        return std::make_shared<Game_2048>();
    }


    private:
    unsigned int matrice1[4][4];
    unsigned int matrice2[4][4];
    unsigned int matrice3[4][4];
    int score = 0;
    uint16_t colors[2][14] = {
        {0xEF7D, 0xD69A, 0xFDCB, 0xF425, 0xCA28, 0xF800, 0xFFEE, 0xE7EE, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0},
        {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384}};
};

void Game_2048::launch()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            {
                matrice1[i][j] = 0;
                matrice2[i][j] = 0;
                matrice3[i][j] = 0;
            }

    S.setPsram(true);
    S.createSprite(320, 480);

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
    mainWindow=nullptr;

    while (true)
    {
        uint8_t slide = 0; // 1 right, 2 left, 3 up, 4 down
        while (true)
        {
            if(home_button.pressed())
            {
                S.deleteSprite();
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
            
            #ifdef BUILD_EMU
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
            S.deleteSprite();
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

        while(touch.isTouch()) touch.update();
    }

    S.deleteSprite();
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
    S.fillSprite(0xFFFF);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (matrice1[i][j])
            {
                S.fillRoundRect(15 + i * 75, 50 + j * 75, 65, 65, 5, colors[0][matrice1[i][j]-1]);
                S.setTextColor(0xFFFF);
                S.drawCenterString(std::to_string(colors[1][matrice1[i][j]-1]).c_str(), 15 + i * 75 + 65 / 2, 50 + j * 75 + 65 / 4, &fonts::Font4);
            }
        }
    }
    S.setTextColor(0x0000);
    S.drawCenterString(std::to_string(score).c_str(), 160, 400, &fonts::Font4);
    S.pushSprite(&tft_root, 0, 25);
    
    *shouldUS = true;
}

#endif
