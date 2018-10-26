#ifndef COLORED_POINT_H
#define COLORED_POINT_H

#include "stable.h"
struct colored_point
{
    int x;
    int y;
    QRgb rgb;

    void set(int x,int y,QRgb rgb)
    {
        this->x = x;
        this->y = y;
        this->rgb = rgb;
    }
    colored_point(){}
    colored_point(int x,int y,QRgb rgb)
    {
        this->set(x,y,rgb);
    }
};

#endif // COLORED_POINT_H
