#ifndef __LABEL_HPP__
#define __LABEL_HPP__

#include <string.h>
#include "../gui_class.hpp"
#include "../../interface/screen.hpp"

#define DEFAULT_LINE_SPACING 5

extern const GFXfont *fontsList[3][4][4];
extern string fontsNamesList[];

class Keyboard;

class Label2 : public Gui
{
    public:

        GUI_TYPE getType(){return LABEL_TYPE;}

        Label2();
        Label2(int16_t x, int16_t y, std::string text = "");
        Label2(int16_t x, int16_t y, int16_t width, int16_t height, std::string text = "");

        ~Label2();

        void draw(void);
        void free(void){}

        uint16_t getUsableWidth(void); // retourne la largeur réelle maximale utilisée par le texte (sans les marge bordure etc...)
        uint16_t getUsableHeight(void);
        LGFX_Sprite* getSprite(void); // retourne un sprite déjà paramètré pour le rendu

        void setTextColor(color_t color);
        void setBold(bool bold);
        void setItalic(bool italic);
        void setFontSize(uint8_t s);

    private:

        std::vector<std::string> parse(void); // découpe le texte en lignes
        uint16_t toSpriteConverter(uint16_t s); // convertir une taille réelle sur l'écran en équivalent dans le buffer de rendu
        uint16_t toRealConverter(uint16_t s); // convertir une taille du buffer de rendu en équivalent sur l'écran physique

        float m_factor; // facteur entre le buffer de rendu et les mesures réelles sur l'écran

        std::string m_fontName;
        uint8_t m_fontHeight;
        uint16_t m_lineHeightSpacing;
        bool m_bold;
        bool m_italic;
        GFXfont *m_font;

        color_t m_textColor;
        bool m_linked;
        std::string m_text;
        LGFX_Sprite* renderBuffer;
};

#endif
