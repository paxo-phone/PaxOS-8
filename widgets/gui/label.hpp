#ifndef Label_HPP
#define Label_HPP

#include <string.h>

using namespace std;

#define DEFAULT_LINE_SPACING 5

/*const GFXfont *allFronts[3][4][4] = {
{
    {&FreeSans9pt7b, &FreeSans12pt7b, &FreeSans18pt7b, &FreeSans24pt7b},
    {&FreeSansOblique9pt7b, &FreeSansOblique12pt7b, &FreeSansOblique18pt7b, &FreeSansOblique24pt7b},
    {&FreeSansBold9pt7b, &FreeSansBold12pt7b, &FreeSansBold18pt7b, &FreeSansBold24pt7b},
    {&FreeSansBoldOblique9pt7b, &FreeSansBoldOblique12pt7b, &FreeSansBoldOblique18pt7b, &FreeSansBoldOblique24pt7b}
},
{
    {&FreeMono9pt7b, &FreeMono12pt7b, &FreeMono18pt7b, &FreeMono24pt7b},
    {&FreeMonoOblique9pt7b, &FreeMonoOblique12pt7b, &FreeMonoOblique18pt7b, &FreeMonoOblique24pt7b},
    {&FreeMonoBold9pt7b, &FreeMonoBold12pt7b, &FreeMonoBold18pt7b, &FreeMonoBold24pt7b},
    {&FreeMonoBoldOblique9pt7b, &FreeMonoBoldOblique12pt7b, &FreeMonoBoldOblique18pt7b, &FreeMonoBoldOblique24pt7b}
},
{
    {&FreeSerif9pt7b, &FreeSerif12pt7b, &FreeSerif18pt7b, &FreeSerif24pt7b},
    {&FreeSerifItalic9pt7b, &FreeSerifItalic12pt7b, &FreeSerifItalic18pt7b, &FreeSerifItalic24pt7b},
    {&FreeSerifBold9pt7b, &FreeSerifBold12pt7b, &FreeSerifBold18pt7b, &FreeSerifBold24pt7b},
    {&FreeSerifBoldItalic9pt7b, &FreeSerifBoldItalic12pt7b, &FreeSerifBoldItalic18pt7b, &FreeSerifBoldItalic24pt7b}
}};*/

const lgfx::U8g2font *allFronts[3][4][4] = {
{
    {&lgfxJapanGothicP_8, &lgfxJapanGothicP_12, &lgfxJapanGothicP_16, &lgfxJapanGothicP_24},
    {&lgfxJapanGothicP_8, &lgfxJapanGothicP_12, &lgfxJapanGothicP_16, &lgfxJapanGothicP_24},
    {&lgfxJapanGothicP_8, &lgfxJapanGothicP_12, &lgfxJapanGothicP_16, &lgfxJapanGothicP_24},
    {&lgfxJapanGothicP_8, &lgfxJapanGothicP_12, &lgfxJapanGothicP_16, &lgfxJapanGothicP_24}
},
{
    {&lgfxJapanGothicP_8, &lgfxJapanGothicP_12, &lgfxJapanGothicP_16, &lgfxJapanGothicP_24},
    {&lgfxJapanGothicP_8, &lgfxJapanGothicP_12, &lgfxJapanGothicP_16, &lgfxJapanGothicP_24},
    {&lgfxJapanGothicP_8, &lgfxJapanGothicP_12, &lgfxJapanGothicP_16, &lgfxJapanGothicP_24},
    {&lgfxJapanGothicP_8, &lgfxJapanGothicP_12, &lgfxJapanGothicP_16, &lgfxJapanGothicP_24}
},
{
    {&lgfxJapanGothicP_8, &lgfxJapanGothicP_12, &lgfxJapanGothicP_16, &lgfxJapanGothicP_24},
    {&lgfxJapanGothicP_8, &lgfxJapanGothicP_12, &lgfxJapanGothicP_16, &lgfxJapanGothicP_24},
    {&lgfxJapanGothicP_8, &lgfxJapanGothicP_12, &lgfxJapanGothicP_16, &lgfxJapanGothicP_24},
    {&lgfxJapanGothicP_8, &lgfxJapanGothicP_12, &lgfxJapanGothicP_16, &lgfxJapanGothicP_24}
}};

// /paxo/system/fronts/roboto.vlw

string allFrontsNames[] = {"sans", "mono", "serif"};

class Keyboard;

class Label : public Gui
{
public:
    GUI_TYPE getType(){return LABEL_TYPE;}

    Label();
    Label(int16_t x, int16_t y, int16_t width, int16_t height, std::string text = "");

    void draw();
    void free(){}

    LGFX_Sprite* selfDetermination();
    void updateSizes() { delete selfDetermination(); }

    // text
    string text;
    void setText(string text){this->text = text; rendered=false; }
    string getText(){return this->text;}

    // text color
    color_t textColor;
    void setTextColor(color_t color){textColor = color;}
    color_t getTextColor(){return textColor;}
    uint16_t getTextWidth();

    // font
    string frontName = "mono";
    const lgfx::U8g2font *front;
    uint8_t fontHeight;
    uint8_t lineSpacing = DEFAULT_LINE_SPACING;
    unsigned int textWidth = 0;
    bool bold = false;
    bool italic = false;

        // self determination
        uint16_t halfRadius = 0;
        uint16_t finalHeight = 0;
        uint16_t totalMarginX = 0;
        uint16_t totalMarginY = 0;
        uint16_t totalTextWidth = 0;
        uint16_t autoMarginYForText = 0;
        float textFactor = 1.0; // *factor to convert to virtual screen and /factor to convert to physical screen (>1)

    Keyboard* key = nullptr;
    bool editable = false;
    bool linked = false;
    void link(Keyboard* keyboard){this->key = keyboard;}
};

#endif