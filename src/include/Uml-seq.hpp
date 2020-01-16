#ifndef _UML_SEQ_HPP_
#define _UML_SEQ_HPP_

#include <map>
#include "ToolBase.hpp"
#include "BackendBase.hpp"
#include "Logic.hpp"

namespace tools
{

class UmlSeq : public ToolBase
{

std::map<std::string, std::string> entries;
Logic logix;
Renderer rend;

public:
    UmlSeq()
    {
        setEntry("IS_SAVED", "NO");
        logix.setRenderer(&rend);
        rend.resetCenter();
    }
    void setEntry(const std::string &key, const std::string &value) noexcept override
    {
        entries[key] = value;
    }
    std::string getEntry(const std::string &key) noexcept override
    {
        return entries[key];
    }
    void setCoordinates(int width, int height, int startx, int starty) noexcept override
    {
        rend.setCoordinates(width, height, startx, starty);
    }
    void setBackend(backends::BackendBase &bk) override
    {
        ToolBase::setBackend(bk);

        setEntry("IS_SAVED", "YES");

        bk.bind(":newact ${ARG}", [this](void){
            logix.addProcess(getEntry("ARG"));
        }, "Creates a new actor with specified name");

        bk.bind(":rmact ${ARG}", [this](void){
            logix.remProcess(getEntry("ARG"));
        }, "Removes all actors with specified name");

        bk.bind("crs", nullptr, "Throw unhandled exception");
       
    }

};

}

#endif