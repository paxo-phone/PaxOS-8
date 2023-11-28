#ifndef Keyboard_HPP
#define Keyboard_HPP

#define KEY_ERASE 127 // ascii delete char

#define COLOR_PRESS 0x9F1D
#define COLOR_NOT_PRESS 0x29A6
#define COLOR_BACKGROUND 0xFFFF

class Box;
class Label;

class Keyboard : public Gui
{
public:
    GUI_TYPE getType(){return KEYBOARD_TYPE;}
    Keyboard();
    ~Keyboard();

    void clear(color_t color) { tft.fillRect(0, 320, 322, 200, color); }
    void free(){}
    bool update(); // seulement pour les widgets inférieurs

    char getKey();

    void draw();

    bool isEnd();

    void setMode(uint8_t mode){ this->mode = mode;}

    void link(Label* label);

private:
    std::string alphabet[4][3] =
    {
        {
            {"azertyuiop"},
            {"qsdfghjklm"},
            {"  wxcvbn  "}
        },
        {
            {"AZERTYUIOP"},
            {"QSDFGHJKLM"},
            {"  WXCVBN  "}
        },
        {
            {"[]{}#%^*+="},
            {"-\\|~<>*+=/"},
            {"   .,?!'  "}
        },
        {
            {"1234567890"},
            {"-/:;()$&@\""},
            {"   .,?!'  "}
        }
    };

    Label *space_label;
    Label *return_label;

    Box* box_maj;
    Box* box_type;
    Box* box_erase;

    Label* label = nullptr;

    uint8_t mode = 0;
    char lastChar = 0;
    uint64_t timer_last_char = 0;
    uint8_t blocked = 0; // 0=no, 1=used for the first time, 2 = repeat
};

#endif /* Keyboard_HPP */
