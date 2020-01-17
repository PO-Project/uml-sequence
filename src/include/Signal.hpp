#ifndef _SIGNAL_HPP_
#define _SIGNAL_HPP_

#include "IActor.hpp"
#include "Arrow.hpp"
#include <memory>

class Signal : public IActor
{
    protected:
    std::unique_ptr<IDrawable> entity;

    public:

    virtual void draw(IRenderer &r) override
    {
        if(selected)r.setAttribute(A_REVERSE);
            entity->draw(r);
        if(selected)r.unsetAttribute(A_REVERSE);
    }
};

class InformationSignal : public Signal
{
    public:

    InformationSignal(Point s, Point p)
    {
        entity = std::make_unique<Line>(s, p);
    }
};

class ProcessSwitchSignal : public Signal
{
    public:

    ProcessSwitchSignal(Point s, Point p)
    {
        entity = std::make_unique<Arrow>(s, p);
    }
};

#endif