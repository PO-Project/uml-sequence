#ifndef _ARROW_HPP_
#define _ARROW_HPP_

#include "Line.hpp"

class Arrow : public Line
{
    public:
    
    Arrow(Point a, Point b) : Line(a - 1, b - 1) {}

    void draw(IRenderer *r)
    {
        if(start.x == end.x)
        {
            this->Arrow::draw(r);
            r->renderString(">", end);
        }
        else if(start.y == end.y)
        {
            this->Arrow::draw(r);
            r->renderString("\\/", end);
        }
        else
        {
            std::terminate();
        }
            
    }

};

#endif