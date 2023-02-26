#include "label.hpp"

Label::Label(int16_t x, int16_t y, int16_t width, int16_t height, std::string text)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->text = text;
    this->fontHeight = 12;
    enabledBackground=false;
}

void Label::draw()
{
    if(enabledBackground)
        l_tft.drawRoundRectWithBorder(0, 0, getWidth(), getHeight(), getRadius(), getBorderSize(), getBackgroundColor(), getBorderColor());

    LGFX_Sprite* renderBuffer = selfDetermination();

    print("factor: " + std::to_string(textFactor));

    // parse text

    string world = "";
    string line = "";
    uint16_t lineNumber = 0;
    uint16_t index = 0;

    while (index < text.length())
    {
        if(text[index] == '\n' || renderBuffer->textWidth(const_cast<char*>((line + world).c_str())) / textFactor >= totalTextWidth || index+1 == text.length())
        {
            if(text[index] == '\n')
            {
                line += world;
                world="";
                index++;
            }

            if(renderBuffer->textWidth(const_cast<char*>((line + world).c_str())) / textFactor >= totalTextWidth)
            {
                world += text[index];
                index++;
            }

            if(index+1 == text.length())
            {
                line += world + text[index];
                world="";
                index++;
            }

            // render line
            renderBuffer->setPsram(false);
            renderBuffer->setColorDepth(8);
            renderBuffer->createSprite(totalTextWidth*textFactor, fontHeight*textFactor);
            renderBuffer->fillScreen(getBackgroundColor());
            renderBuffer->setTextColor(textColor);
            renderBuffer->setCursor(0, 0);
            renderBuffer->print(line.c_str());
            renderBuffer->pushRotateZoomWithAA(&l_tft,
                                            getWidth()/2,
                                            totalMarginY + (renderBuffer->fontHeight() / textFactor)/2 + ((renderBuffer->fontHeight() / textFactor + lineSpacing) * (lineNumber)),
                                            0,
                                            1/textFactor,
                                            1/textFactor,
                                            getBackgroundColor());
            renderBuffer->deleteSprite();
            
            line = ""; // erase line
            lineNumber++;
        }
        else if(text[index] == ' ')
        {
            line += world + " ";
            world="";
            index++;
        }
        else
        {
            world += text[index];
            index++;
        }
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
        renderBuffer->setFreeFont(allFronts[frontIndex][bold*2+italic][i]);
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

    textFactor = (float) renderBuffer->fontHeight()/fontHeight;

    // parse text

    string world = "";
    string line = "";
    uint16_t lineNumber = 0;
    uint16_t index = 0;

    while (index < text.length())
    {
        if(text[index] == '\n' || renderBuffer->textWidth(const_cast<char*>((line + world).c_str())) / textFactor >= totalTextWidth || index+1 == text.length())
        {
            if(text[index] == '\n')
            {
                line += world;
                world="";
                index++;
            }

            if(renderBuffer->textWidth(const_cast<char*>((line + world).c_str())) / textFactor >= totalTextWidth)
            {
                world += text[index];
                index++;
            }

            if(index+1 == text.length())
            {
                line += world + text[index];
                world="";
                index++;
            }
            
            line = ""; // erase line
            lineNumber++;
        }
        else if(text[index] == ' ')
        {
            line += world + " ";
            world="";
            index++;
        }
        else
        {
            world += text[index];
            index++;
        }
    }

    print(std::to_string(lineNumber));
    finalHeight = totalMarginY * 2 + (renderBuffer->fontHeight() / textFactor) + ((renderBuffer->fontHeight() / textFactor + lineSpacing) * (lineNumber - 1));
    
    if(height == AUTO)
        height = finalHeight;
    return renderBuffer;
}