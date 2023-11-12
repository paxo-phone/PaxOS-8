#ifdef _3D_MINECRAFT_HPP // on purpose
#define _3D_MINECRAFT_HPP

/*
    x = largeur
    y = profondeur
    z = hauteur

    rx = lever ou baisser la camera
    ry = /!\ fait n'importe quoi
    rz = tourner a droite ou a gauche la camera
*/


double rotate(double x, double y, double r, bool xy)
{
    r*=-1;
  double cosR=cos(r);
  double sinR=sin(r);
  double a2[2]={0};
  a2[0]=x*cosR-y*sinR;
  a2[1]=y*cosR+x*sinR;
      
  return a2[xy];
}

class Triangle
{
    public:
    Triangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, uint16_t color)
    {
        this->x1 = x1;
        this->y1 = y1;
        this->z1 = z1;
        this->x2 = x2;
        this->y2 = y2;
        this->z2 = z2;
        this->x3 = x3;
        this->y3 = y3;
        this->z3 = z3;
        this->color = color;
    }
    double x1, y1, z1, x2, y2, z2, x3, y3, z3 = 0;
    uint16_t color = 0;
};

double radian(double degree)
{
    double pi = 3.14159265359;
    return (degree * (pi / 180));
}


class RenderingEngine
{
    public:
    RenderingEngine(){}
    void render(LGFX_Sprite* buffer);
    bool gp2D(double x, double y, double z);

    double cameraX = 8, cameraY = 8, cameraZ = 15;
    double cameraRX = 0, cameraRY = 0, cameraRZ = 15;
    int16_t buffer_out_x, buffer_out_y = 0;

    std::vector<Triangle *> triangles;
};

bool RenderingEngine::gp2D(double x, double y, double z)
{
    double pX = -(x - cameraX);
    double pY = -(z - cameraZ);
    double pZ = y - cameraY;

    double cosX = cos(cameraRX);
    double sinX = sin(cameraRX);
    double cosY = cos(cameraRY);
    double sinY = sin(cameraRY);
    double cosZ = cos(cameraRZ);
    double sinZ = sin(cameraRZ);

    // rotate Y
    double pX2 = pX;
    pX = cosY*pX2+sinY*pZ;
    pZ = -sinY*pX2+cosY*pZ;
    
    double pY2 = pY;
    pX2 = pX;
    
    // rotate X
    pY2 = pY;
    pY = cosX*pY2-sinX*pZ;
    pZ = sinX*pY2+cosX*pZ;
    
    if (pZ <= 0)
        return true;

    pY2 = pY;
    pX2 = pX;

    // Perspective projection
    double f = 320 / pZ;
    pX2 = pX * f;
    pY2 = pY * f;

    pX2 += 320 / 2;
    pY2 += 240 / 2;

    buffer_out_x = pX2;
    buffer_out_y = pY2;

    return false;
}

void RenderingEngine::render(LGFX_Sprite* buffer)
{
    // trier les triagles par distance

    for(int i = 0; i < triangles.size(); i++)
    {
        Triangle* t = triangles[i];
        uint8_t result = 0;
        uint8_t corners = 0;
        result+=gp2D(t->x1, t->y1, t->z1); double co1[2] = {static_cast<double>(buffer_out_x), static_cast<double>(buffer_out_y)};
        corners+=(0<buffer_out_x && buffer_out_x<320 && 0<buffer_out_y && buffer_out_y<240);
        result+=gp2D(t->x2, t->y2, t->z2); double co2[2] = {static_cast<double>(buffer_out_x), static_cast<double>(buffer_out_y)};
        corners+=(0<buffer_out_x && buffer_out_x<320 && 0<buffer_out_y && buffer_out_y<240);
        result+=gp2D(t->x3, t->y3, t->z3); double co3[2] = {static_cast<double>(buffer_out_x), static_cast<double>(buffer_out_y)};
        corners+=(0<buffer_out_x && buffer_out_x<320 && 0<buffer_out_y && buffer_out_y<240);
        if(!result && corners != 0)
        {
            buffer->fillTriangle(co1[0], co1[1], co2[0], co2[1], co3[0], co3[1], t->color);
        }
    }
    for(; triangles.size();)
    {
        delete[] triangles[0];
        triangles.erase(triangles.begin());
    }
}

#endif
