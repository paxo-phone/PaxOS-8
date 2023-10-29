#ifndef MINECRAFT_HPP
#define MINECRAFT_HPP

#include <vector>
#include "blocks.hpp"
#include "renderingEngine.hpp"
#include "SimplexNoise.hpp"

class Chunk
{
    public:
    int16_t x, y = 0;
    void chunk(int16_t x, int16_t y)
    {
        this->x = x;
        this->y = y;
        for(int x = 0; x < 16; x++)
            for(int y = 0; y < 16; y++)
                for(int z = 0; z < 16; z++)
                    data[x][y][z] = AIR;
    }
    uint8_t data[16][16][32] = {{{AIR}}}; // [x][y][z]
};

class Minecraft : public CppAppContainer
{
    public:
    void main();
    
    std::vector<Chunk *> chunks;
    void processChunks(LGFX_Sprite* output);
    void renderChunk(int index, LGFX_Sprite* buffer);
    void renderBlock(int type, int x, int y, int z, LGFX_Sprite* buffer);
    uint8_t isBlock(int x, int y, int z);

    double cameraX, cameraY, cameraZ = 0;
    double cameraRX, cameraRY, cameraRZ = 0;
    RenderingEngine* render;
};

uint8_t Minecraft::isBlock(int x, int y, int z)
{
    for (int i = 0; i < chunks.size(); i++)
    {
        Chunk* c = chunks[i];
        if(c->x*16<=x && x<c->x*16+16 && c->y*16<=y && y<c->y*16+16)
        {
            x-=c->x*16;
            y-=c->y*16;

            return c->data[x][y][z];
        }
    }
    return false;
}

void Minecraft::main()
{
    mainWindow=nullptr;
    
    LGFX_Sprite output(&tft_root);
    render = new RenderingEngine();
    output.setPsram(false);
    output.setColorDepth(8);
    
    Box keys(50, 250, 180, 76);
    Label* front = new Label(39, 0, 29, 34, "^");
    Label* back = new Label(39, 42, 29, 34, "v");
    Label* right = new Label(0, 23, 34, 29, "<");
    Label* left = new Label(72, 23, 34, 29, ">");
    Label* up = new Label(147, 0, 29, 34, "+");
    Label* down = new Label(147, 42, 29, 34, "-");
    
    front->setBackgroundColor(0x0000);
    back->setBackgroundColor(0x0000);
    right->setBackgroundColor(0x0000);
    left->setBackgroundColor(0x0000);
    up->setBackgroundColor(0x0000);
    down->setBackgroundColor(0x0000);
    keys.addChild(right);
    keys.addChild(left);
    keys.addChild(front);
    keys.addChild(back);
    keys.addChild(up);
    keys.addChild(down);
    
    tft_root.fillScreen(0xFFFF);
    
    output.createSprite(320, 240);
    
    for(int cx = 0; cx < 2; cx++)
    {
        for(int cy = 0; cy < 2; cy++)
        {
            chunks.push_back((Chunk*) malloc(sizeof(Chunk)));
            chunks[chunks.size()-1]->chunk(cx,cy);
            
            for(int x = 0; x < 16; x++)
            {
                for(int y = 0; y < 16; y++)
                {
                    for(int z = 1; z < 32; z++)
                    {
                        chunks[chunks.size()-1]->data[x][y][z] = AIR;
                    }
                }
            }
            
            for(int x = 0; x < 16; x++)
            {
                for(int y = 0; y < 16; y++)
                {
                    uint16_t noi = (Get2DPerlinNoiseValue(x+(chunks[chunks.size()-1]->x*16), y+(chunks[chunks.size()-1]->y*16), 30)+1)*2;
                    for(int z = 1; z < noi+1; z++)
                    {
                        if(noi+4==z)
                        {
                            chunks[chunks.size()-1]->data[x][y][z] = GRASS;
                            
                            if(rand()%128==1)
                            {
                                for(int z2 = z; z2-z<=3 || rand()%3; z2++)
                                {
                                    chunks[chunks.size()-1]->data[x][y][z2] = WOOD;
                                    chunks[chunks.size()-1]->data[x][y][z2+1] = LEAVES;
                                }
                            }
                        }
                        else
                            chunks[chunks.size()-1]->data[x][y][z] = DIRT;
                    }
                }
            }
        }
    }
    
    
    unsigned long timer = millis();
    unsigned long timer_m = millis();
    
    keys.updateAll();
    
    processChunks(&output);
    processChunks(&output);
            
    while (!home_button.pressed())
    {
        keys.updateAll();
        
        //output.drawString(std::string(float(1000/(millis() - timer))) + " fps", 0, 0); // print fps
        
        timer_m = float(1000/(millis() - timer));
        timer = millis();
        
        double speed = 4.3/timer_m; // calculate speed for tps
        
        if(isBlock(render->cameraX, render->cameraY, render->cameraZ-4)==AIR)   // gravity
        {
            render->cameraZ-=1;
            processChunks(&output);
        } else if(isBlock(render->cameraX, render->cameraY, render->cameraZ-3)!=AIR && isBlock(render->cameraX, render->cameraY, render->cameraZ-3)!=WATER) // up blocks
        {
            render->cameraZ+=1;
            processChunks(&output);
        } else if(isBlock(isBlock(render->cameraX, render->cameraY, render->cameraZ-3-0.2)!=AIR && render->cameraX, render->cameraY, render->cameraZ-3)==WATER) // water
        {
            render->cameraZ-=0.2;
            processChunks(&output);
        } else if (touch.isTouch())
        {
            processChunks(&output);
        }
        
        if(right->isFocuced())
        {
            render->cameraX+=rotate(speed, 0, render->cameraRY, 0);
            render->cameraY+=rotate(speed, 0, render->cameraRY, 1);
        }
        if(left->isFocuced())
        {
            render->cameraX+=rotate(-speed, 0, render->cameraRY, 0);
            render->cameraY+=rotate(-speed, 0, render->cameraRY, 1);
        }
        if(front->isFocuced())
        {
            render->cameraX+=rotate(0, speed, render->cameraRY, 0);
            render->cameraY+=rotate(0, speed, render->cameraRY, 1);
        }
        if(back->isFocuced())
        {
            render->cameraX+=rotate(0, -speed, render->cameraRY, 0);
            render->cameraY+=rotate(0, -speed, render->cameraRY, 1);
        }
        if(up->isFocuced())
            render->cameraZ+=0.4;
        if(down->isFocuced())
            render->cameraZ-=0.4;
        
        if(touch.isSlidingHorizontally())
        {
            render->cameraRY+=radian(touch.isSlidingHorizontally()/3);
            touch.resetScrollHorizontal();
        }
        if(touch.isSlidingVertically())
        {
            render->cameraRX-=radian(touch.isSlidingVertically()/3);
            touch.resetScrollVertical();
        }
                
        #ifdef BUILD_EMU
            SDL_Delay(20);
        #endif
    }
    
//    for(Chunk* ch : chunks)
//    {
//        delete ch;
//    }
//    chunks.clear();
}

void Minecraft::processChunks(LGFX_Sprite* output)
{
    output->pushSprite(0, 0); // show frame
    
    output->fillSprite(0xFFFF); // clear frame
    
    std::vector<int> chunks_indexs; // chunks index from farthest
    
    for (int j = 0; j < chunks.size(); j++)
    {
        double distance_max = 0;
        int index_max = 0;
        
        for (int i = 0; i < chunks.size(); i++)
        {
            Chunk* ch = chunks[i];
            
            bool pass = false;
            
            for(int ii = 0; ii < chunks_indexs.size(); ii++)
                if(i == chunks_indexs[ii])
                    pass=true;
            
            if(!pass && pow((ch->x*16+8)-render->cameraX, 2) + pow((ch->y*16+8)-render->cameraY, 2)>distance_max)
            {
                distance_max = pow((ch->x*16+8)-render->cameraX, 2) + pow((ch->y*16+8)-render->cameraY, 2);
                index_max=i;
            }
        }
        chunks_indexs.push_back(index_max);
    }
    for (int i = 0; i < chunks_indexs.size() && i < 1; i++)
        renderChunk(chunks_indexs[chunks_indexs.size()+i-1], output);
    //renderChunk(chunks_indexs[chunks_indexs.size()-1], &output);
    
    #ifdef BUILD_EMU
        *shouldUS = true;
    #endif
}

void Minecraft::renderChunk(int index, LGFX_Sprite* buffer)
{
    //uint8_t indexs[16][16][32] = {{{0}}};
    uint16_t rex = chunks[index]->x*16, rey = chunks[index]->y*16; // relative coordinates chunk

    int16_t px = render->cameraX-chunks[index]->x;
    int16_t py = render->cameraY-chunks[index]->y;
    int16_t pz = render->cameraZ;
    
    
    for(int z = 0; z < pz; z++)
    {
        for(int y = 0; y < py && y<16 && y>=0; y++)
        {
            for(int x = 0; x < px && x<16 && x>=0; x++)
            {
                renderBlock(chunks[index]->data[x][y][z], rex+x, rey+y, z, buffer);
            }
            for(int x = 15; x >= px && x<16 && x>=0; x--)
            {
                renderBlock(chunks[index]->data[x][y][z], rex+x, rey+y, z, buffer);
            }
        }
        for(int y = 15; y >= py && y<16 && y>=0; y--)
        {
            for(int x = 0; x < px && x<16 && x>=0; x++)
            {
                renderBlock(chunks[index]->data[x][y][z], rex+x, rey+y, z, buffer);
            }
            for(int x = 15; x >= px && x<16 && x>=0; x--)
            {
                renderBlock(chunks[index]->data[x][y][z], rex+x, rey+y, z, buffer);
            }
        }
    }
    for(int z = 15; z >= pz; z--)
    {
        for(int y = 0; y < py && y<16 && y>=0; y++)
        {
            for(int x = 0; x < px && x<16 && x>=0; x++)
            {
                renderBlock(chunks[index]->data[x][y][z], rex+x, rey+y, z, buffer);
            }
            for(int x = 15; x >= px && x<16 && x>=0; x--)
            {
                renderBlock(chunks[index]->data[x][y][z], rex+x, rey+y, z, buffer);
            }
        }
        for(int y = 15; y >= py && y<16 && y>=0; y--)
        {
            for(int x = 0; x < px && x<16 && x>=0; x++)
            {
                renderBlock(chunks[index]->data[x][y][z], rex+x, rey+y, z, buffer);
            }
            for(int x = 15; x >= px && x<16 && x>=0; x--)
            {
                renderBlock(chunks[index]->data[x][y][z], rex+x, rey+y, z, buffer);
            }
        }
    }
}

void Minecraft::renderBlock(int type, int x, int y, int z, LGFX_Sprite* buffer)
{
    if(type == AIR)
        return;

    if(render->cameraZ>z)
    {
        if(!isBlock(x, y, z+1) && textures[type][0])
        {
            render->triangles.push_back(new Triangle(x, y, z+1, x+1, y, z+1, x+1, y+1, z+1, textures[type][0]));
            render->triangles.push_back(new Triangle(x, y, z+1, x, y+1, z+1, x+1, y+1, z+1, textures[type][1]));
        }
    }
    else
    {
        if(!isBlock(x, y, z-1) && textures[type][2])
        {
            render->triangles.push_back(new Triangle(x, y, z, x+1, y, z, x+1, y+1, z, textures[type][2]));
            render->triangles.push_back(new Triangle(x, y, z, x, y+1, z, x+1, y+1, z, textures[type][3]));
        }
    }


    if(render->cameraY<y)
    {
        if(!isBlock(x, y-1, z) && textures[type][2])
        {
            render->triangles.push_back(new Triangle(x, y, z+1, x+1, y, z+1, x+1, y, z, textures[type][2]));
            render->triangles.push_back(new Triangle(x, y, z+1, x, y, z, x+1, y, z, textures[type][3]));
        }
    }
    else
    {
        if(!isBlock(x, y+1, z) && textures[type][2])
        {
            render->triangles.push_back(new Triangle(x, y+1, z+1, x+1, y+1, z+1, x+1, y+1, z, textures[type][2]));
            render->triangles.push_back(new Triangle(x, y+1, z+1, x, y+1, z, x+1, y+1, z, textures[type][3]));
        }
    }

    if(render->cameraX>x)
    {
        if(!isBlock(x+1, y, z) && textures[type][2])
        {
            render->triangles.push_back(new Triangle(x+1, y, z+1, x+1, y+1, z+1, x+1, y, z, textures[type][2]));
            render->triangles.push_back(new Triangle(x+1, y+1, z, x+1, y+1, z+1, x+1, y, z, textures[type][3]));
        }
    }
    else
    {
        if(!isBlock(x-1, y, z) && textures[type][2])
        {
            render->triangles.push_back(new Triangle(x, y, z+1, x, y+1, z+1, x, y, z, textures[type][2]));
            render->triangles.push_back(new Triangle(x, y+1, z, x, y+1, z+1, x, y, z, textures[type][3]));
        }
    }
    
    render->render(buffer);
}

#endif