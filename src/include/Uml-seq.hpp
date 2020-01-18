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

        bk.bind(":act ${ARG}", [this](void){
            logix.addProcess(getEntry("ARG"));
        }, "Creates a new actor with specified name");

        bk.bind(":rma ${ARG}", [this](void){
            logix.remProcess(getEntry("ARG"));
        }, "Removes all actors with specified name");

        bk.bind(":sel ${ARG}", [this](void){
            logix.selectByName(getEntry("ARG"));
        }, "Selects first actor with specified name");

        bk.bind(":unsel", [this](void){
            logix.deselect(true);
        }, "Unselect actors");

        bk.bind(":edit ${ARG}", [this](void){
            logix.editSelected(getEntry("ARG"));
        }, "Edits selected actor.");

        bk.bind(":mode ${ARG}", [this](void){
            logix.setEditionMode(getEntry("ARG"));
        }, "Set edition mode (actors/signals).");

        bk.bind("<DEL>", [this](void){
            logix.deleteSelected();
        }, "Deletes selected actor.");

        bk.bind(":del", [this](void){
            logix.deleteSelected();
        }, "Deletes selected actor.");

        bk.bind(":r", [this](void){
            logix.right();
        }, "Select right/next actor");

        bk.bind(":l", [this](void){
            logix.left();
        }, "Select left/previous actor");

        bk.bind(":crs", nullptr, "Throw unhandled exception");
       
    }

};

}

#endif