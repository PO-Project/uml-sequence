#ifndef _IDRAWABLE_HPP_
#define _IDRAWABLE_HPP_

class IRenderer;

struct IDrawable
{ 
    virtual void draw(IRenderer &) = 0;
};

#endif