#ifndef _ARROW_HPP_
#define _ARROW_HPP_

#include "Line.hpp"
#include <cassert>

class Arrow : public Line
{
    public:
    
    Arrow(Point a, Point b) : Line(a, {b.x - 1, b.y}) {}

    void draw(IRenderer &r)
    {
        if(start.x == end.x)
        {
            assert(1 != 0 && "NOT NEEDED");
        }
        else if(start.y == end.y)
        {
            Line::draw(r);

            if(end.x > start.x)r.renderString(">", end);
            else r.renderString("<", {end.x + 1, end.y});
        }
        else
        {
            assert(1 != 0 && "NOT NEEDED");
        }
            
    }

};

#endif