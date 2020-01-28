#ifndef _IACTOR_HPP_
#define _IACTOR_HPP_

#include "IDrawable.hpp"

struct IActor : public IDrawable
{
    bool selected{false};

    virtual Point getCenter() const = 0;
};

struct IXmlAble
{
    virtual std::string dumpDataIntoXmlFormat() const = 0;
};

namespace XML
{
    template <typename T>
    std::string makeTag(std::string tag, T v)
    {
        return "<" + tag + "> " + std::to_string(v) + " </" + tag + ">\n";
    }

    template <>
    std::string makeTag<std::string>(std::string tag, std::string v)
    {
        return "<" + tag + "> " + v + " </" + tag + ">\n";
    }


    std::string openTag(std::string tag)
    {
        return "<" + tag + "> \n";
    }

    std::string closeTag(std::string tag)
    {
        return "</" + tag + "> \n";
    }

    std::string partDelegate(const IXmlAble &ix, std::string name)
    {
        return openTag(name) + ix.dumpDataIntoXmlFormat() + closeTag(name);
    }
}

#endif