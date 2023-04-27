#include "image.hpp"
#include "../../interface/memory.hpp"
#include "../../interface/shell.hpp"

#include <string>
#include "stdint.h"

using namespace std;

bool Image::parsePngHeader(std::string filename, int16_t* width, int16_t* height)
{
    storage::LFile file(filename, storage::READ);
    
    if (!file.is_open()) // error during oppening
    {
        print("can't open the file: " + filename);
        return false;
    }

    uint8_t table[32];

    for (int i = 0; i < 32; i++) // add available later !!!
    {
        table[i] = file.readChr();
    }

    *width=table[16]*256*256*256+table[17]*256*256+table[18]*256+table[19];
    *height=table[20]*256*256*256+table[21]*256*256+table[22]*256+table[23];

    file.close();

    // print(to_string(*width) + "; " + to_string(*height)); // debug ok

    return true;
}

bool Image::parseBmpHeader(std::string filename, int16_t* width, int16_t* height)
{
    storage::LFile file(filename, storage::READ);
    
    if (!file.is_open()) // error during oppening
    {
        print("can't open the file: " + filename);
        return false;
    }

    uint8_t table[32];

    for (int i = 0; i < 32; i++)
    {
        table[i] = file.readChr();
    }

    *width=table[15]*256*256*256+table[16]*256*256+table[17]*256+table[18];
    *height=table[19]*256*256*256+table[20]*256*256+table[21]*256+table[22];

    file.close();

    return true;
}

bool Image::parseJpgHeader(std::string filename, int16_t *width, int16_t *height)
{
    storage::LFile file(filename, storage::READ);
    
    if (!file.is_open()) // error during oppening
    {
        print("can't open the file: " + filename);
        return false;
    }

    file.read();
    file.read();

    for(;;) {
        int marker;
        while((marker = file.readChr()) != 0xFF);
        while((marker = file.readChr()) == 0xFF);

        if (marker ==  0xC0 || marker == 0xC2) {
            file.read();
            file.read();
            file.read();
            *height = (file.readChr() << 8) + file.readChr();
            *width = (file.readChr() << 8) + file.readChr();
            return true;
        }
    }

    file.close();

    return true;
}

void drawImageFromStorage(LGFX_Sprite* sprite, std::string filename, Image::ImageFormat format)
{
    print("drawing image from storage...");
    #ifdef BUILD_EMU
        filename = "storage/" + filename;
    #endif
    #ifdef BUILD_PAXO
        filename = "/storage/" + filename;
    #endif

    lgfx::FileWrapper file;
    switch (format) // image size with right format
    {
        case Image::ImageFormat::BMP:
            sprite->drawBmpFile(&file, filename.c_str(), 0, 0);
        break;
        case Image::ImageFormat::PNG:
            sprite->drawPngFile(&file, filename.c_str(), 0, 0);
        break;
        case Image::ImageFormat::JPG:
            sprite->drawJpgFile(&file, filename.c_str(), 0, 0);
        break;
    };
    print("ok");
}

Image::Image(std::string filename, int16_t x, int16_t y, int16_t w, int16_t h)
{
    this->filename = filename;

    if(filename.find(".bmp") != std::string::npos) // parsing image format
        imageFormat = ImageFormat::BMP;
    if(filename.find(".png") != std::string::npos)
        imageFormat = ImageFormat::PNG;
    if(filename.find(".jpg") != std::string::npos || filename.find(".jpeg") != std::string::npos)
        imageFormat = ImageFormat::JPG;

    switch (imageFormat) // image size with right format
    {
        case BMP:
            parseBmpHeader(filename, &oW, &oH);
        break;
        case PNG:
            parsePngHeader(filename, &oW, &oH);
        break;
        case JPG:
            parseJpgHeader(filename, &oW, &oH);
        break;
    }

    if(w == AUTO)
        w = oW;
    if(h == AUTO)
        h = oH;

    init(x, y, w, h);

    print (to_string(oW) + " " + to_string(oH) + " " + to_string(width) + " " + to_string(height));
}

Image::~Image()
{
    print("Image::~Image");
    unload();
}

void Image::unload()
{
    for (int i = 0; i < imagesLoaded.size(); i++)
    {
        if (imagesLoaded[i].path == this->filename && imagesLoaded[i].width == this->width && imagesLoaded[i].height == this->height)
        {
            if(imagesLoaded[i].usedBy.size() == 1)
            {
                imagesLoaded[i].img->deleteSprite();
                imagesLoaded.erase(imagesLoaded.begin() + i);
                return;
            }
            for (int j = 0; j < imagesLoaded[i].usedBy.size(); j++)
            {
                if (imagesLoaded[i].usedBy[j] == this)
                {
                    imagesLoaded[i].usedBy.erase(imagesLoaded[i].usedBy.begin() + j);
                    return;
                }
            }
        }
    }
}

void Image::draw()
{
    for (int i = 0; i < imagesLoaded.size(); i++)
    {
        if (imagesLoaded[i].path == this->filename && imagesLoaded[i].width == this->width && imagesLoaded[i].height == this->height)
        {
            imagesLoaded[i].img->pushSprite(&l_tft, 0, 0);
        }
    }
}

void Image::load()
{
    print("Image::load");
    for (int i = 0; i < imagesLoaded.size(); i++)
    {
        if (imagesLoaded[i].path == this->filename && imagesLoaded[i].width == this->width && imagesLoaded[i].height == this->height)
        {
            imagesLoaded[i].usedBy.push_back(this);
            return;
        }
    }

    image_header img_pre;
    img_pre.path = this->filename;
    img_pre.img = new LGFX_Sprite;
    img_pre.width = this->width;
    img_pre.height = this->height;
    img_pre.usedBy = {this};
    imagesLoaded.push_back(img_pre);
    image_header* img = &imagesLoaded[imagesLoaded.size()-1];

    // if the image is not already loaded by another Widget load it

    if(oW == this->width && oH == this->height) // don't need to be resized
    {
        img->img->setPsram(true);
        img->img->createSprite(this->width, this->height);
        img->img->fillScreen(ALPHA_16B);

        drawImageFromStorage(img->img, filename, ImageFormat(imageFormat));

    }
    else // need to be resized
    {
        LGFX_Sprite imgPre;
        imgPre.setPsram(true);
        imgPre.createSprite(oW, oH);
        imgPre.fillScreen(ALPHA_16B);

        drawImageFromStorage(&imgPre, filename, ImageFormat(imageFormat));

        img->img->setPsram(true);
        img->img->createSprite(this->width, this->height);
        img->img->fillScreen(ALPHA_16B);

        imgPre.pushRotateZoomWithAA(img->img,
                                    getWidth()/2,
                                    getHeight()/2,
                                    0,
                                    float(float(this->width)/float(oW)),
                                    float(float(this->height)/float(oH)),
                                    getBackgroundColor());
        
        imgPre.deleteSprite();

        LGFX_Sprite mask(img->img); // a modifier plutard car modification du sprite chargé et généralisé pour toutes les images utilsant ce fichier

        mask.setPsram(true);
        mask.setColorDepth(16);
        mask.createSprite(getWidth(), getHeight());
        mask.fillSprite(ALPHA_16B);

        mask.drawRoundRectWithBorder(0, 0,getWidth(),getHeight(),this->radius, getBorderSize(), 0xFFFF, getBorderColor());
        mask.pushSprite(img->img, 0, 0, 0xFFFF);

        mask.deleteSprite();
    }

    print("image loaded! ");
}