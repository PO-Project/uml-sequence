#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

#include <string>
#include "IActor.hpp"
#include "Box.hpp"
#include "Line.hpp"
#include "Signal.hpp"

class Process : public IActor
{
    std::string name;
    Box box;
    Line line;

    static constexpr unsigned maxNameSize {8};

    void boxUpdateName()
    {
        if(name.size() > maxNameSize)box.content = name.substr(0, maxNameSize - 3) + "...";
        else box.content = name;
    }

    public:
        Process() = default;

        Process(std::string name, Point pos = {0,0}) : name(name)
        {
            box.p = pos;
            boxUpdateName();
        }

        void setName(std::string nn)
        {
            name = nn;
            boxUpdateName();
        }
        std::string getName() const {return name;}

        void setPosition(Point p)
        {
            box.p = p;
        }

        virtual void draw(IRenderer &r)override
        {
            if(selected)r.setAttribute(A_REVERSE);

            box.draw(r);

            auto blc = box.leftUpperCorner();
            auto brc = box.rightLowerCorner();

            auto lineX = blc.x  + ((brc.x - blc.x) / 2);

            line.start = {lineX, brc.y + 1};
            line.end =   {lineX, brc.y + 3};
            
            line.draw(r);

            if(selected)r.unsetAttribute(A_REVERSE);
        }

    bool selected{false};
};

#endif