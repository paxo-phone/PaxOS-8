#include "label.hpp"
#include "../../tasks/tasks.hpp"

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

Label::Label() : Gui(0, 0, 0, 0)
{
    this->m_fontHeight = BASE_FONT_SIZE;
    setTextColor(COLOR_BLACK);
    enabledBackground=false;

    m_fontName = "mono";
    m_fontHeight = 20;
    m_lineHeightSpacing = DEFAULT_LINE_SPACING;
    m_bold = false;
    m_italic = false;
    m_factor = 1.0;
    m_font = nullptr;
    m_linked = false;
    m_canBeEdited = false;

    idEvent = setInterval(new CallbackMethod<Label>(this, &Label::updateBlinkingCursor), 500);
}

Label::Label(int16_t x, int16_t y, std::string m_text) : Gui(x, y, AUTO, AUTO)
{
    this->m_fontHeight = BASE_FONT_SIZE;
    this->m_text = m_text;
    setTextColor(COLOR_BLACK);
    enabledBackground=false;

    m_fontName = "mono";
    m_fontHeight = 20;
    m_lineHeightSpacing = DEFAULT_LINE_SPACING;
    m_bold = false;
    m_italic = false;
    m_factor = 1.0;
    m_font = nullptr;
    m_linked = false;
    m_canBeEdited = false;

    idEvent = setInterval(new CallbackMethod<Label>(this, &Label::updateBlinkingCursor), 500);
}

Label::Label(int16_t x, int16_t y, int16_t width, int16_t height, std::string m_text) : Gui(x, y, width, height)
{
    this->m_fontHeight = BASE_FONT_SIZE;
    this->m_text = m_text;
    setTextColor(COLOR_BLACK);
    enabledBackground=false;

    m_fontName = "mono";
    m_fontHeight = 20;
    m_lineHeightSpacing = DEFAULT_LINE_SPACING;
    m_bold = false;
    m_italic = false;
    m_factor = 1.0;
    m_font = nullptr;
    m_linked = false;
    m_canBeEdited = false;

    idEvent = setInterval(new CallbackMethod<Label>(this, &Label::updateBlinkingCursor), 500);
}

Label::~Label()
{
    removeInterval(idEvent);
}

void Label::setBold(bool bold)
{
    this->m_bold = bold;
    this->rendered = false;
}

void Label::setItalic(bool italic)
{
    this->m_italic = italic;
    this->rendered = false;
}

void Label::setFontSize(uint8_t s)
{
    this->m_fontHeight = s;
    this->rendered = false;
}

uint8_t Label::getFontSize(void)
{
    return this->m_fontHeight;
}

void Label::setText(std::string text)
{
    this->m_text = text;
    this->rendered = false;
}

std::string Label::getText(void)
{
    return this->m_text;
}

void Label::setCanBeEdited(bool canBeEdited)
{
    this->m_canBeEdited = canBeEdited;
}

bool Label::canBeEdited()
{
    return this->m_canBeEdited;
}

void Label::setFontName(std::string fontName)
{
    this->m_fontName = fontName;
}

uint16_t Label::getTextHeight(void)
{
    std::vector<std::string> lines = parse();
    uint16_t textHeight =  (getFontSize() + m_lineHeightSpacing) * (lines.size());
        textHeight += getRadius()-2*getBorderSize();
    return textHeight;
}

uint16_t Label::getUsableWidth(void)
{
    return getWidth()-getRadius()-2*getBorderSize();
}

uint16_t Label::getUsableHeight(void)
{
    return getHeight()-getRadius()-2*getBorderSize();
}

uint16_t Label::toSpriteConverter(uint16_t s)
{
    return (float) s * m_factor;
}

uint16_t Label::toRealConverter(uint16_t s)
{
    return (float) s / m_factor;
}

void Label::updateBlinkingCursor(void)
{
    if(m_linked == false)
        return;
    if(isCursorVisible)
        isCursorVisible = false;
    else
        isCursorVisible = true;
    rendered=false;
}

std::vector<std::string> Label::parse()
{
    initSprite();

    std::vector<std::string> lines;
    std::string currentLine = "";


    for (char c : m_text) {
        if (c == '\n') {
            lines.push_back(currentLine);
            currentLine = "";
        } else if (toRealConverter(renderBuffer.textWidth((currentLine + c).c_str())) <= getUsableWidth()) {
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

    
    if(m_linked && isCursorVisible)
        currentLine+="|";

    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    return lines;
}

void Label::draw()
{
    if(enabledBackground)
        l_tft.drawRoundRectWithBorder(0, 0, getWidth(), getHeight(), getRadius(), getBorderSize(), getBackgroundColor(), getBorderColor());

    initSprite();

    std::vector<std::string> lines = parse();

    std::cout << m_factor << " " << int(m_fontHeight) << " " << m_fontName << std::endl;

    for (int i = 0; i < lines.size(); i++)
    {
        std::string line;
        
        line = lines[i];
        renderBuffer.setPsram(false);
        renderBuffer.setColorDepth(8);
        renderBuffer.createSprite(toSpriteConverter(getUsableWidth()), renderBuffer.fontHeight());
        renderBuffer.fillScreen(getBackgroundColor());
        renderBuffer.setTextColor(m_textColor);
        uint16_t textWidth = renderBuffer.textWidth(line.c_str());

        if(H_alignment == LEFT_ALIGNMENT)
            renderBuffer.setCursor(0, 0);
        else if(H_alignment == CENTER_ALIGNMENT)
            renderBuffer.setCursor(toSpriteConverter(getUsableWidth())/2 - textWidth/2, 0);
        else
            renderBuffer.setCursor(toSpriteConverter(getUsableWidth()) - textWidth, 0);

        renderBuffer.print(line.c_str());
        renderBuffer.pushRotateZoomWithAA(&l_tft,
                                        getWidth()/2,
                                        getRadius()/2 + getBorderSize() + getFontSize()/2 + (getFontSize() + m_lineHeightSpacing) * i
                                            + ((this->getVerticalAlignment() == CENTER_ALIGNMENT) ? (getUsableHeight()/2 - (getFontSize() + m_lineHeightSpacing) * lines.size() /2) : (0))
                                            + ((this->getVerticalAlignment() == DOWN_ALIGNMENT) ? (getUsableHeight() - (getFontSize() + m_lineHeightSpacing) * lines.size()) : (0)),
                                        0,
                                        1/m_factor,
                                        1/m_factor,
                                        getBackgroundColor());
        renderBuffer.deleteSprite();
    }
}

void Label::initSprite()
{
    int frontIndex = 0;

    for (int i = 0; i < 3; i++) 
    {
        if(fontsNamesList[i] == m_fontName)
            frontIndex = i;
    }

    for(int i = 0; i < 4; i++)
    {
        renderBuffer.setFont(fontsList[frontIndex][m_bold*2+m_italic][i]);
        if(renderBuffer.fontHeight()>m_fontHeight)
        {
            m_font = const_cast<GFXfont *> (fontsList[frontIndex][m_bold*2+m_italic][i]);
            break;
        }
    }

    if(renderBuffer.fontHeight()<m_fontHeight)
    {
        // error
    }

    m_factor = (float) renderBuffer.fontHeight()/m_fontHeight; // > 1
}

void Label::setTextColor(color_t color)
{
    this->m_textColor = color;
    this->rendered = false;
}