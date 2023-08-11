#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/memory.hpp"

class Snake : public App
{
    public:
    void main()
    {
        mainWindow=nullptr;
        while(1)
        {
            uint16_t snake_color = 0xFFFF;
            uint16_t background_color = 0x0000;

            tft_root.fillScreen(background_color);
            tft_root.drawRect(9, 39, 302, 402, 0xFFFF);
            uint8_t snake[1200][2] = {254};   // create snake board of 200 cubes (254 is out of screen)
            snake[0][1] = 24;           // init position of snake
            snake[0][0] = 16;
            int level = 7;              // init level to 4
            int speed = 300;           // init speed in milliseconds | ex: 100 = 1 cube / second ; 200 = 2 cube / second
            uint8_t pointxy[2];       // init the point at randoms points
            pointxy[0] = random(1, 31);
            pointxy[1] = random(4, 40);
            uint16_t pointColor = tft_root.color565(random(100, 255), random(100, 255), random(100, 255));
            tft_root.fillRoundRect(pointxy[0]*10, pointxy[1]*10, 10, 10, 3, pointColor);
            uint64_t snakeTimer = millis();   // init the timer
            uint8_t direction = 1;         // 1=right 2=under 3=left 4=up
            uint8_t direction_ = 1;
            bool gameover = 0;
            uint8_t interprete = false;

            while(!gameover)
            {
                eventHandler.update();
                if (millis() > snakeTimer + 100000/speed)
                {
                    for (uint8_t i = level; i != 0; i--)
                    {
                        snake[i][1] = snake[i - 1][1];
                        snake[i][0] = snake[i - 1][0];
                    }

                    if(direction != direction_ && interprete == 1)
                    {
                        direction = direction_;
                        interprete = 2;
                    }
                        

                    if (direction == 1)
                        snake[0][0]++;
                    if (direction == 3)
                        snake[0][0]--;
                    if (direction == 2)
                        snake[0][1]++;
                    if (direction == 4)
                        snake[0][1]--;

                    tft_root.fillRect(snake[0][0] * 10, snake[0][1] * 10, 10, 10, snake_color);
                    tft_root.fillRect(snake[level - 1][0] * 10, snake[level - 1][1] * 10, 10, 10, background_color);

                    // detect collision

                    for (int i = 1; i < level; i++)
                    {
                        if (snake[i][0] == snake[0][0]  && snake[i][1] == snake[0][1] && snake) // gameover
                        {
                            gameover = true;
                            tft_root.fillRect(snake[0][0] * 10, snake[0][1] * 10, 10, 10, 0xD820);

                            while(touch.isTouch());
                            while(!touch.isTouch());
                        }
                    }

                    if(snake[0][0] == pointxy[0] && snake[0][1] == pointxy[1]) // get point
                    {
                        pointxy[0] = random(1, 31);
                        pointxy[1] = random(4, 40);
                        
                        level++;
                        speed+=10;
                        snake_color = pointColor;

                        tft_root.setTextColor(0x0000);
                        tft_root.drawCentreString(("score = " + std::to_string(level-8)).c_str(), 160, 460, &fonts::Font4);
                        tft_root.setTextColor(0xFFFF);
                        tft_root.drawCentreString(("score = " + std::to_string(level-7)).c_str(), 160, 460, &fonts::Font4);
                        
                        pointColor = tft_root.color565(random(100, 255), random(100, 255), random(100, 255));
                    }
                    
                    tft_root.fillRoundRect(pointxy[0]*10, pointxy[1]*10, 10, 10, 3, pointColor);


                    if (1 > snake[0][0] || snake[0][0] > 30 || 4 > snake[0][1] || snake[0][1] > 43)
                    {
                        gameover = true;
                        tft_root.fillRect(snake[0][0] * 10, snake[0][1] * 10, 10, 10, 0xD820);
                        while(touch.isTouch())
                        {
                            touch.update();
                            if(home_button.pressed())
                                return;
                        }
                        while(!touch.isTouch()) 
                        {
                            touch.update();
                            if(home_button.pressed())
                                return;
                        }
                    }

                    snakeTimer = millis();
                }

                touch.update();
                if (touch.isTouch())
                {
                    delay(5);
                     // get new pos
                    uint16_t tx, ty;
                    tx = touch.getX();
                    ty = touch.getY();

                    if(interprete == 0 || interprete == 1)
                    {
                        if(direction == 1 || direction == 3)
                        {
                            if(snake[0][1] < ty/10)
                                direction_ = 2;
                            else
                                direction_ = 4;
                        }

                        if(direction == 2 || direction == 4)
                        {
                            if(snake[0][0] < tx/10)
                                direction_ = 1;
                            else
                                direction_ = 3;
                        }
                        interprete = 1;
                    }
                    
                }
                else if(interprete == 2)
                    interprete = 0;
                if(home_button.pressed())
                    return;
                
                #ifdef BUILD_EMU
                    SDL_Delay(20);
                #endif
            }
        }
    }
};

#endif
