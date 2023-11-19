#include "label2.hpp"

const GFXfont *fontsList[3][4][4] = {
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
}};

string fontsNamesList[] = {"sans", "mono", "serif"};

Label2::Label2() : Gui(0, 0, 0, 0)
{
    this->m_fontHeight = BASE_FONT_SIZE;
    setTextColor(COLOR_BLACK);
    enabledBackground=false;

    m_fontName = "sans";
    m_fontHeight = 20;
    m_lineHeightSpacing = DEFAULT_LINE_SPACING;
    m_bold = false;
    m_italic = false;
    m_factor = 1.0;
    m_font = nullptr;
    m_linked = false;
}

Label2::Label2(int16_t x, int16_t y, std::string m_text) : Gui(x, y, AUTO, AUTO)
{
    this->m_fontHeight = BASE_FONT_SIZE;
    this->m_text = m_text;
    setTextColor(COLOR_BLACK);
    enabledBackground=false;

    m_fontName = "sans";
    m_fontHeight = 20;
    m_lineHeightSpacing = DEFAULT_LINE_SPACING;
    m_bold = false;
    m_italic = false;
    m_factor = 1.0;
    m_font = nullptr;
    m_linked = false;
}

Label2::Label2(int16_t x, int16_t y, int16_t width, int16_t height, std::string m_text) : Gui(x, y, width, height)
{
    this->m_fontHeight = BASE_FONT_SIZE;
    this->m_text = m_text;
    setTextColor(COLOR_BLACK);
    enabledBackground=false;

    m_fontName = "sans";
    m_fontHeight = 20;
    m_lineHeightSpacing = DEFAULT_LINE_SPACING;
    m_bold = false;
    m_italic = false;
    m_factor = 1.0;
    m_font = nullptr;
    m_linked = false;
}

Label2::~Label2()
{
    if(renderBuffer != nullptr)
        delete renderBuffer;
}

void Label2::setBold(bool bold)
{
    this->m_bold = bold;
    this->rendered = false;
}

void Label2::setItalic(bool italic)
{
    this->m_italic = italic;
    this->rendered = false;
}

void Label2::setFontSize(uint8_t s)
{
    this->m_fontHeight = s;
    this->rendered = false;
}

uint16_t Label2::getUsableWidth(void)
{
    return getWidth()-2*getRadius()-2*getBorderSize();
}

uint16_t Label2::getUsableHeight(void)
{
    return getHeight()-2*getRadius()-2*getBorderSize();
}

uint16_t Label2::toSpriteConverter(uint16_t s)
{
    return (float) s * m_factor;
}

uint16_t Label2::toRealConverter(uint16_t s)
{
    return (float) s / m_factor;
}

std::vector<std::string> Label2::parse()
{
    renderBuffer = getSprite();

    std::vector<std::string> lines;
    std::string currentLine = "";

    for (char c : m_text) {
        if (c == '\n') {
            lines.push_back(currentLine);
            currentLine = "";
        } else if (toRealConverter(renderBuffer->textWidth((currentLine + c).c_str())) <= getUsableWidth()) {
            currentLine += c;
        } else if (c == ' ') {
            lines.push_back(currentLine);
            currentLine = "";
        } else {
            if (currentLine.empty()) {
                currentLine += c;
            } else if (currentLine.back() == ' ') {
                currentLine += c;
            } else {
                std::size_t lastSpace = currentLine.find_last_of(' ');
                if (lastSpace == std::string::npos) {
                    lines.push_back(currentLine);
                    currentLine = "";
                    currentLine += c;
                } else {
                    std::string firstPart = currentLine.substr(0, lastSpace);
                    lines.push_back(firstPart);
                    currentLine = currentLine.substr(lastSpace + 1);
                    currentLine += c;
                }
            }
        }
    }

    if(m_linked)
        currentLine+="|";
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    return lines;
}

void Label2::draw()
{
    if(enabledBackground)
        l_tft.drawRoundRectWithBorder(0, 0, getWidth(), getHeight(), getRadius(), getBorderSize(), getBackgroundColor(), getBorderColor());

    renderBuffer = getSprite();

    std::vector<std::string> lines = parse();

    std::cout << m_factor << " " << int(m_fontHeight) << " " << m_fontName << std::endl;

    for (int i = 0; i < lines.size(); i++)
    {
        std::string line;
        
        line = lines[i];
        renderBuffer->setPsram(false);
        renderBuffer->setColorDepth(8);
        renderBuffer->createSprite(toSpriteConverter(getUsableWidth()), renderBuffer->fontHeight());
        renderBuffer->fillScreen(getBackgroundColor());
        renderBuffer->setTextColor(m_textColor);
        uint16_t textWidth = renderBuffer->textWidth(line.c_str());

        if(H_alignment == LEFT_ALIGNMENT)
            renderBuffer->setCursor(0, 0);
        else if(H_alignment == CENTER_ALIGNMENT)
            renderBuffer->setCursor(toSpriteConverter(getUsableWidth())/2 - textWidth/2, 0);
        else
            renderBuffer->setCursor(toSpriteConverter(getUsableWidth()) - textWidth, 0);

        renderBuffer->print(line.c_str());
        renderBuffer->pushRotateZoomWithAA(&l_tft,
                                        getWidth()/2,
                                        getRadius()/2 + getBorderSize() + renderBuffer->fontHeight()/2 + (renderBuffer->fontHeight() + m_lineHeightSpacing) * i,
                                        0,
                                        1/m_factor,
                                        1/m_factor,
                                        getBackgroundColor());
        renderBuffer->deleteSprite();
    }
}

LGFX_Sprite* Label2::getSprite()
{
    if(renderBuffer!=nullptr)
        delete renderBuffer;
    
    renderBuffer = new LGFX_Sprite(&l_tft);

    int frontIndex = 0;

    for (int i = 0; i < 3; i++) 
    {
        if(fontsNamesList[i] == m_fontName)
            frontIndex = i;
    }

    for(int i = 0; i < 4; i++)
    {
        renderBuffer->setFont(fontsList[frontIndex][m_bold*2+m_italic][i]);
        if(renderBuffer->fontHeight()>m_fontHeight)
        {
            m_font = const_cast<GFXfont *> (fontsList[frontIndex][m_bold*2+m_italic][i]);
            break;
        }
    }

    if(renderBuffer->fontHeight()<m_fontHeight)
    {
        // error
    }

    m_factor = (float) renderBuffer->fontHeight()/m_fontHeight; // > 1

    return renderBuffer;
}

void Label2::setTextColor(color_t color)
{
    this->m_textColor = color;
    this->rendered = false;
}