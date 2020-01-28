#ifndef _SIGNAL_HPP_
#define _SIGNAL_HPP_

#include "IActor.hpp"
#include "Arrow.hpp"
#include <memory>

class Process;

class Signal : public IActor, public IXmlAble
{
    protected:
    
    std::unique_ptr<IDrawable> entity;
    Point b,e;

    public:

    inline virtual int identify() const
    {
        return 0;
    }

    std::weak_ptr<Process> start, end;

    Signal() = default;

    virtual void lateConstructor() = 0;

    virtual void draw(IRenderer &r) override
    {
        if(selected)r.setAttribute(A_REVERSE);
            entity->draw(r);
        if(selected)r.unsetAttribute(A_REVERSE);
    }

    std::string dumpDataIntoXmlFormat() const override
    {
        std::string ret;

        ret+=XML::makeTag("starts", reinterpret_cast<long unsigned int>(start.lock().get()));
        ret+=XML::makeTag("ends", reinterpret_cast<long unsigned int>(end.lock().get()));
        ret+=XML::makeTag("type", identify());

        return ret;
    }

    Point getCenter() const override
    {
        return {(b.x + e.x) / 2, (b.y + e.y) / 2};
    }

    friend class Process;
    friend class Logic;
};

class InformationSignal : public Signal
{
    public:

    inline virtual int identify() const override
    {
        return 1;
    }

    InformationSignal() = default;

    void lateConstructor()
    {
        entity = std::make_unique<Line>(b, e);
    }

    friend class Process;
    friend class Logic;
};

class ProcessSwitchSignal : public Signal
{
    public:

    inline virtual int identify() const override
    {
        return 2;
    }

    ProcessSwitchSignal() = default;

    void lateConstructor()
    {
        entity = std::make_unique<Arrow>(b, e);
    }

    friend class Process;
    friend class Logic;
};


#endif