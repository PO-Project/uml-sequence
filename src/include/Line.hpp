#ifndef _LINE_HPP_
#define _LINE_HPP_

#include "Renderer.hpp"
#include "IDrawable.hpp"
//#include <cmath>

class Line : public IDrawable
{
    protected:
        Point start, end;

        Line() = default;

    public:
        Line(Point a, Point b) : start(a), end(b) {}

        void draw(IRenderer &r)
        {
            if(start.x == end.x)
            {
                for(unsigned i = 0; i < abs(start.y - end.y); i++)
                {
                    r.renderString("|", {start.x, start.y + i});
                }
            }
            else if(start.y == end.y)
            {
                for(unsigned i = 0; i < abs(start.x - end.x); i++)
                {
                    r.renderString("-", {start.x + i, start.y});
                }
            }
            else
            {
                //nie trzeba
                std::terminate();
            }
            
        }

    friend class Process;
};

#endif