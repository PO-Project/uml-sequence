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
            setEntry("IS_SAVED", "NO");
        }, "Creates a new actor with specified name");

        bk.bind(":rma ${ARG}", [this](void){
            logix.remProcess(getEntry("ARG"));
            setEntry("IS_SAVED", "NO");
        }, "Removes all actors with specified name");

        bk.bind(":sel ${ARG}", [this](void){
            logix.selectByName(getEntry("ARG"));
        }, "Selects first actor with specified name");

        bk.bind(":unsel", [this](void){
            logix.deselect(true);
        }, "Unselect actors");

        bk.bind(":edit ${ARG}", [this](void){
            logix.editSelected(getEntry("ARG"));
            setEntry("IS_SAVED", "NO");
        }, "Edits selected actor.");

        bk.bind(":mode ${ARG}", [this](void){
            logix.setEditionMode(getEntry("ARG"));
        }, "Set edition mode (actors/signals).");

        bk.bind(":ins ${ARG}", [this](void){
            logix.insertProcess(getEntry("ARG"));
        }, "Insert process before selected actor.");

        bk.bind("<DEL>", [this](void){
            logix.deleteSelected();
            setEntry("IS_SAVED", "NO");
        }, "Deletes selected actor.");

        bk.bind(":del", [this](void){
            logix.deleteSelected();
            setEntry("IS_SAVED", "NO");
        }, "Deletes selected actor.");

        bk.bind(":r", [this](void){
            logix.right();
        }, "Select right/next actor");

        bk.bind("<RARROW>", [this](void){
            logix.right();
        }, "Select right/next actor");

        bk.bind(":l", [this](void){
            logix.left();
        }, "Select left/previous actor");

        bk.bind("<LARROW>", [this](void){
            logix.left();
        }, "Select left/previous actor");
        
        bk.bind(":c", [this](void){
            logix.center(true);
        }, "Center on selected actor");

        bk.bind(":afs ${ARG} ${AARG}", [this](void){
            logix.appendCreateSignalGeneric<InformationSignal>(getEntry("ARG"), getEntry("AARG"));
            setEntry("IS_SAVED", "NO");
        }, "Add information signal between two processes.");

        bk.bind(":ajs ${ARG} ${AARG}", [this](void){
            logix.appendCreateSignalGeneric<ProcessSwitchSignal>(getEntry("ARG"), getEntry("AARG"));
            setEntry("IS_SAVED", "NO");
        }, "Add jump/switch signal between two processes.");

        bk.bind(":fs ${ARG} ${AARG}", [this](void){
            logix.insertCreateSignalGeneric<InformationSignal>(getEntry("ARG"), getEntry("AARG"));
            setEntry("IS_SAVED", "NO");
        }, "Insert information signal between two processes before selected position.");

        bk.bind(":js ${ARG} ${AARG}", [this](void){
            logix.insertCreateSignalGeneric<ProcessSwitchSignal>(getEntry("ARG"), getEntry("AARG"));
            setEntry("IS_SAVED", "NO");
        }, "Insert jump/switch signal between two processes before selected position.");        

        bk.bind(":save ${ARG}", [this](void){
            logix.save(getEntry("ARG"));
            setEntry("IS_SAVED", "YES");
        }, "Export to xml format");

        bk.bind(":forget", [this](void){
            setEntry("IS_SAVED", "YES");
        }, "Forces backend to think, that file has been saved.");

        bk.bind(":load ${ARG}", [this](void){
            logix.load(getEntry("ARG"));
            setEntry("IS_SAVED", "YES");
        }, "Load from xml file");
       
    }

};

}

#endif