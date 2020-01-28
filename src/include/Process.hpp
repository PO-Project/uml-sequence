#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

#include <string>
#include "IActor.hpp"
#include "Box.hpp"
#include "Line.hpp"
#include "Signal.hpp"

class Process : public IActor, public IXmlAble
{
    std::string name;
    Box box;
    Line line;

    static constexpr unsigned maxNameSize {8};

    std::vector<std::weak_ptr<Signal>> signalsIn;
    std::vector<std::weak_ptr<Signal>> signalsOut;

    void boxUpdateName()
    {
        if(name.size() > maxNameSize)box.content = name.substr(0, maxNameSize - 3) + "...";
        else box.content = name;
    }

    public:

        int signalEndPositionVertical{0};

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

        std::string dumpDataIntoXmlFormat() const override
        {
            std::string ret;

            ret += XML::makeTag("name", name);

            return ret;
        }

        void setPosition(Point p)
        {
            box.p = p;

            for(auto &i : signalsIn)
            {
                if(auto strong = i.lock())
                {
                    strong->e.x = getCenter().x;
                }
            }

            for(auto &i : signalsOut)
            {
                if(auto strong = i.lock())
                {
                    strong->b.x = getCenter().x;
                }
            }
        }

        Point getCenter() const override
        {
            auto blc = box.leftUpperCorner();
            auto brc = box.rightLowerCorner();

            return {blc.x  + ((brc.x - blc.x) / 2), (blc.y - brc.y)/2 + blc.y};
        }

        void pipeSignalFrom(std::weak_ptr<Signal> sig)
        {
            signalsOut.push_back(sig);

            //enforce x position
            (signalsOut.back().lock())->b.x = getCenter().x;
        }
        void pipeSignalTo(std::weak_ptr<Signal> sig)
        {
            signalsIn.push_back(sig);

            //enforce x position
            (signalsIn.back().lock())->e.x = getCenter().x;
        }

        void cullDeadSignals(int mode) //bit mask bit1 - out, bit0 - in
        {
            if(mode & 1)
            {
                signalsIn.erase(std::remove_if(signalsIn.begin(), signalsIn.end(), 
                    [](const std::weak_ptr<Signal> &p) -> bool
                    {
                        return (!p.lock());
                }), signalsIn.end());
            }
            if (mode & 2)
            {
                signalsOut.erase(std::remove_if(signalsOut.begin(), signalsOut.end(), 
                    [](const std::weak_ptr<Signal> &p) -> bool
                    {
                        return (!p.lock());
                }), signalsOut.end());
            }
        }

        void findVerticalEnd()
        {
            cullDeadSignals(3);

            auto cmp = [](std::weak_ptr<Signal> a, std::weak_ptr<Signal> b)
            {
                return a.lock()->e.y < b.lock()->e.y;
            };

            auto s1 = std::max_element(signalsIn.begin(), signalsIn.end(), cmp);
            auto s2 = std::max_element(signalsOut.begin(), signalsOut.end(), cmp);

            auto a = s1 == signalsIn.end() ? 0 : s1->lock()->e.y;
            auto b = s2 == signalsOut.end() ? 0 : s2->lock()->e.y;

            signalEndPositionVertical = std::max(a, b);
        }

        virtual void draw(IRenderer &r) override
        {
            if(selected)r.setAttribute(A_REVERSE);

            box.draw(r);

            auto blc = box.leftUpperCorner();
            auto brc = box.rightLowerCorner();

            auto lineX = blc.x  + ((brc.x - blc.x) / 2);

            line.start = {lineX, brc.y + 1};
            line.end =   {lineX, std::max(brc.y + 1, signalEndPositionVertical+1)};
            
            line.draw(r);

            if(selected)r.unsetAttribute(A_REVERSE);
        }

};

#endif