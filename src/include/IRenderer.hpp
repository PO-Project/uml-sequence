#ifndef _IRENDERER_HPP_
#define _IRENDERER_HPP_

#include <ncurses.h>
#include <string>
#include "Utility.hpp"

using Attr_T = NCURSES_ATTR_T;
using Utility::Point;

struct IRenderer
{
    virtual void renderString(std::string, Point) = 0;
    virtual void setAttribute(Attr_T attribute) = 0;
    virtual void unsetAttribute(Attr_T attribute) = 0;
    virtual void setCoordinates(unsigned w, unsigned h, unsigned x, unsigned y) = 0;
    virtual void clearWindow() = 0;
    virtual void update() = 0;
    virtual void setCenter(Point) = 0;
    virtual void resetCenter() = 0;
};

#endif