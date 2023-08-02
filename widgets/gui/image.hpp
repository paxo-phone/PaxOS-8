#ifndef IMAGE_HPP
#define IMAGE_HPP

class Image : public Gui
{
    public:
    Image(std::string filename, int16_t x, int16_t y, int16_t w = AUTO, int16_t h = AUTO);
    ~Image();

    void load();
    void unload();

    void draw();

    static bool parsePngHeader(std::string filename, int16_t* width, int16_t* height);
    static bool parseBmpHeader(std::string filename, int16_t* width, int16_t* height);
    static bool parseJpgHeader(std::string filename, int16_t* width, int16_t* height);

    enum ImageFormat
    {
        BMP,
        PNG,
        JPG
    };

    private:
    std::string filename = "";
    int16_t oW, oH;

    uint8_t imageFormat;
};

struct image_header
{
    std::string path;
    LGFX_Sprite* img = nullptr;
    int16_t width;
    int16_t height;
    std::vector<Image*> usedBy;
};

std::vector<image_header> imagesLoaded;

#endif