#include "label.hpp"

Label::Label() : Gui(0, 0, 0, 0)
{
    this->fontHeight = BASE_FONT_SIZE;
    setTextColor(COLOR_BLACK);
    enabledBackground=false;
}

Label::Label(int16_t x, int16_t y, int16_t width, int16_t height, std::string text) : Gui(x, y, width, height)
{
    this->text = text;
    this->fontHeight = BASE_FONT_SIZE;
    setTextColor(COLOR_BLACK);
    
    enabledBackground=false;

    frontName = "mono";
    fontHeight;
    lineSpacing = DEFAULT_LINE_SPACING;
    textWidth = 0;
    bold = false;
    italic = false;

    halfRadius = 0;
    finalHeight = 0;
    totalMarginX = 0;
    totalMarginY = 0;
    totalTextWidth = 0;
    autoMarginYForText = 0;
    textFactor = 1.0;
}

void Label::draw()
{
    if(enabledBackground)
        l_tft.drawRoundRectWithBorder(0, 0, getWidth(), getHeight(), getRadius(), getBorderSize(), getBackgroundColor(), getBorderColor());

    LGFX_Sprite* renderBuffer = selfDetermination();

    // parse text

    ////////////////////

    std::vector<std::string> lines;
    std::string currentLine = "";
    for (char c : text) {
        if (c == '\n') {
            lines.push_back(currentLine);
            currentLine = "";
        } else if (renderBuffer->textWidth((currentLine + c).c_str()) / textFactor <= totalTextWidth) {
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
    if(linked)
        currentLine+="|";
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    uint16_t lineNumber = 0;

    ////////////////////

    std::string line;

    for (int i = 0; i < lines.size(); i++)
    {
        line = lines[i];
        renderBuffer->setPsram(false);
        renderBuffer->setColorDepth(8);
        renderBuffer->createSprite(totalTextWidth*textFactor, fontHeight*textFactor);
        renderBuffer->fillScreen(getBackgroundColor());
        renderBuffer->setTextColor(textColor);

        if(H_alignment == LEFT_ALIGNMENT)
            renderBuffer->setCursor(0, 0);
        else if(H_alignment == CENTER_ALIGNMENT)
            renderBuffer->setCursor(totalTextWidth*textFactor/2 - renderBuffer->textWidth(line.c_str())/2, 0);
        else
            renderBuffer->setCursor(totalTextWidth*textFactor - renderBuffer->textWidth(line.c_str()), 0);

        renderBuffer->print(line.c_str());
        renderBuffer->pushRotateZoomWithAA(&l_tft,
                                        getWidth()/2,
                                        totalMarginY + (renderBuffer->fontHeight() / textFactor)/2 + ((renderBuffer->fontHeight() / textFactor + lineSpacing) * (lineNumber)) + autoMarginYForText,
                                        0,
                                        1/textFactor,
                                        1/textFactor,
                                        getBackgroundColor());
        renderBuffer->deleteSprite();
        lineNumber++;
    }

    delete renderBuffer;
}

LGFX_Sprite* Label::selfDetermination()
{
    LGFX_Sprite* renderBuffer = new LGFX_Sprite(&l_tft);

    halfRadius = radius/2;

    // calcul of font size
    uint8_t frontIndex = 0;

    // calculate sizes
    totalMarginX = halfRadius + marginX;
    totalMarginY = halfRadius + marginY;

    totalTextWidth = width - totalMarginX*2;

    for (int i = 0; i < 3; i++) 
    {
        if(allFrontsNames[i] == frontName)
            frontIndex = i;
    }

    for(int i = 0; i < 4; i++)
    {
        renderBuffer->setFont(allFronts[frontIndex][bold*2+italic][i]);
        if(renderBuffer->fontHeight()>fontHeight)
        {
            front = allFronts[frontIndex][bold*2+italic][i];
            break;
        }
    }

    if(renderBuffer->fontHeight()<fontHeight)
    {
        // error
    }

    textFactor = (float) renderBuffer->fontHeight()/fontHeight; // > 1

    ////////////////////

    std::vector<std::string> lines;
    std::string currentLine = "";
    for (char c : text) {
        if (c == '\n') {
            lines.push_back(currentLine);
            currentLine = "";
        } else if (renderBuffer->textWidth((currentLine + c).c_str()) / textFactor <= totalTextWidth) {
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
    if(linked)
        currentLine+="|";
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    uint16_t lineNumber = lines.size();

    ////////////////////

    finalHeight = totalMarginY * 2 + (renderBuffer->fontHeight() / textFactor) + ((renderBuffer->fontHeight() / textFactor + lineSpacing) * (lineNumber - 1));
    
    if(autoH == true)
    {
        height = finalHeight;
    } else
    if(V_alignment == CENTER_ALIGNMENT)
    {
        autoMarginYForText = height/2 - finalHeight/2;
    }
    return renderBuffer;
}

uint Label::getTextWidth()
{
    LGFX_Sprite* renderBuffer = new LGFX_Sprite(&l_tft);
    delete selfDetermination();
    uint textWidth = renderBuffer->textWidth((getText()).c_str()) / textFactor;
    delete renderBuffer;
    return textWidth;
}
