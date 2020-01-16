#ifndef _BOX_HPP
#define _BOX_HPP

#include "IDrawable.hpp"
#include "IRenderer.hpp"
#include <string>


class Box : IDrawable
{
    std::string content{""};
    Point p; //lewy górny róg

    Box() = default;

    public:
        Box(std::string s, Point p) : content(s), p(p) {}
        
        void draw(IRenderer &r) override
        {
            r.renderString("+" + std::string(content.size(),'-') + "+", p);
            r.renderString("|" + content + "|", {p.x, p.y + 1} );
            r.renderString("+" + std::string(content.size(),'-') + "+", {p.x, p.y + 2} );
        }

        Point leftUpperCorner() const
        {
            return {p.x, p.y};
        }
        Point rightLowerCorner() const
        {
            return {p.x + 1 + content.size(), p.y + 2};
        }

    friend class Process;
};

#endif