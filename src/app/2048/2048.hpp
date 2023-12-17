#ifndef GAME_2048_HPP
#define GAME_2048_HPP

#include <string>
#include <algorithm>

#include "../../widgets/gui/canvas.hpp"
#include "../CApp.hpp"

class Game_2048 : public CApp
{
public:
    [[nodiscard]] const char* getAppName() const override { return "2048"; }

    void draw();
    void move(uint8_t slide);

    void execute() override;

private:
    unsigned int matrice1[4][4];
    unsigned int matrice2[4][4];
    unsigned int matrice3[4][4];
    int score = 0;
    Canvas* canvas = nullptr;
    uint16_t colors[2][14] = {
        {0xEF7D, 0xD69A, 0xFDCB, 0xF425, 0xCA28, 0xF800, 0xFFEE, 0xE7EE, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0},
        {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384}};
};

#endif
