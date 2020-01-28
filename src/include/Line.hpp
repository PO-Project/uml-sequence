#ifndef _LINE_HPP_
#define _LINE_HPP_

#include "Renderer.hpp"
#include "IDrawable.hpp"

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
                int off = 1;
                if(start.y > end.y)off = -1;

                for(auto i = 0; i < abs(end.y - start.y); i++)
                {
                    r.renderString("|", {start.x, start.y + off*i});
                }
            }
            else if(start.y == end.y)
            {
                int off = 1;
                if(start.x > end.x)off = -1;

                for(auto i = 0; i < abs(end.x - start.x); i++)
                {
                    r.renderString("-", {start.x + off*i, start.y});
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