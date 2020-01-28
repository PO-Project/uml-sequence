#ifndef LOGIC_HPP_
#define LOGIC_HPP_

#include "Arrow.hpp"
#include "Renderer.hpp"
#include "Process.hpp"
#include <memory>
#include <cassert>

class Logic
{
    std::vector<std::shared_ptr<Process>> processes;
    std::vector<std::shared_ptr<Signal>> signals;

    std::weak_ptr<IActor> procSelected; //or signal, name stays for legacy reasons

    static constexpr int minSpace = 12;
    static constexpr int minYPosition = 2;
    int actPosition{0};
    ///////
    static constexpr int minSignalPosition{5};
    static constexpr int minSignalSeparation = 3;
    int actSignalPosition{minSignalPosition};
    ///////

    
    IRenderer *rend{nullptr};

    enum class editionMode
    {
        Procs,
        Signals,
        None = 0xF,
    };

    editionMode currentMode{editionMode::None};

    void refitAll() //legacy, means Processes
    {
        actPosition = 0;
        for(auto& i : processes)
        {
            i->setPosition({actPosition += minSpace, minYPosition});
        }
    }
    void refitAllSigs()
    {
        actSignalPosition = minSignalPosition;
        for(auto& i : signals)
        {
            i->b.y = actSignalPosition += minSignalSeparation;
            i->e.y = actSignalPosition;
        }
        for(auto& i : processes)
        {
            i->findVerticalEnd();
        }
    }

    /*
        Position or end() of respective vector
    */
    template <typename T>
    auto getIteratorFromPointerActual(const std::shared_ptr<T> &sp, const std::vector<std::shared_ptr<T>> &ct) const //pointer jest wazny, bo inne funkcje dbają o zresetowanie go jesli nie jest
    {
        return (std::find_if(ct.begin(), ct.end(), 
            [&sp](const std::shared_ptr<T> &p)
            {
                return p.get() == sp.get();
        }));
    }

    template <typename T>
    auto getIteratorFromPointerActualReversed(const std::shared_ptr<T> &sp, const std::vector<std::shared_ptr<T>> &ct) const //pointer jest wazny, bo inne funkcje dbają o zresetowanie go jesli nie jest
    {
        return (std::find_if(ct.crbegin(), ct.crend(), 
            [&sp](const std::shared_ptr<T> &p)
            {
                return p.get() == sp.get();
        }));
    }

    template <typename T>
    auto findByName(std::string name, const std::vector<std::shared_ptr<T>> &ct) const
    {
        return (std::find_if(ct.begin(), ct.end(), 
        [&name](const std::shared_ptr<T> &p)
            {
                return p->getName() == name;
        }));
    }

    void cullSignals()
    {
        signals.erase(std::remove_if(signals.begin(), signals.end(), 
        [](const std::shared_ptr<Signal> &p)
        {
            return !p->start.lock() || !p->end.lock();
        }), signals.end());
    }

    public:

    Logic() = default;

    void setRenderer(IRenderer * r)
    {
        rend = r;
    }

    void center(bool hard = false)
    {
        if(auto strong = procSelected.lock())rend->setCenter(strong->getCenter());
        if(hard)render();
    }

    void setEditionMode(std::string mode)
    {
        std::for_each(mode.begin(), mode.begin(), [](char &c)
        {
            c = tolower(c);
        });

        if(mode == "actors" || mode == "actor" || mode == "act" || mode == "process" || mode == "processes" || mode == "procs" || mode == "proc" || mode == "a" || mode == "p")
        {
            currentMode = editionMode::Procs;
        }
        else if(mode == "signals" || mode == "signal" || mode == "sign" || mode == "sig" || mode == "s")
        {
            currentMode = editionMode::Signals;
        }
        else
        {
            currentMode = editionMode::None;
        }

        deselect(true);        
    }

    void addProcess(std::string name)
    {
        if(currentMode == editionMode::Procs)
        {
            Point p{actPosition += minSpace, minYPosition};
            processes.push_back(std::move(std::make_shared<Process>(name, p)));

            render();
        }
    }

    void remProcess(std::string name)
    {
        if(currentMode == editionMode::Procs)
        {
            auto oldsize = processes.size();

            processes.erase(std::remove_if(processes.begin(), processes.end(), 
                [&name](const std::shared_ptr<Process> &p)
                {
                    return p->getName() == name;
            }), processes.end());

            if(oldsize != processes.size())
            {
                cullSignals();

                refitAll();
                refitAllSigs();

                render();
            }
        }
    }

    void selectByName(std::string name)
    {
        if(currentMode == editionMode::Procs)
        {
            auto it = findByName(name, processes);

            if(it != processes.end())
            {
                deselect();
                (*it)->selected = true;
                procSelected = *it;

                rend->setCenter((*it)->getCenter());

                render();
            }
        }
    }

    void deselect(bool reset = false)
    {
        if(auto strong = procSelected.lock())
        {
            strong->selected = false;
            procSelected.reset();
        }
        if(reset)
        {
            rend->resetCenter();
            render();
        }
    }

    void editSelected(std::string name)
    {
        if(currentMode == editionMode::Procs)
        {
            if(auto strong = procSelected.lock())
            {
                (std::dynamic_pointer_cast<Process>(strong))->setName(name);
                render();
            }
        }
    }

    void deleteSelected()
    {
        if(currentMode == editionMode::Procs)
        {
            auto & container = processes;
            if(auto strong = procSelected.lock())
            {
                auto oldsize = container.size();

                container.erase(std::remove_if(container.begin(), container.end(), 
                    [&strong](const std::shared_ptr<IActor> &p)
                    {
                        return p.get() == strong.get();
                }), container.end());

                if(oldsize != container.size())
                {
                    strong.reset(); //elimin. silnego dowiązania

                    cullSignals();

                    refitAll();
                    refitAllSigs();

                    render();
                }
            }
        }
        else if(currentMode == editionMode::Signals)
        {
            auto & container = signals;
            if(auto strong = procSelected.lock())
            {
                auto oldsize = container.size();

                container.erase(std::remove_if(container.begin(), container.end(), 
                    [&strong](const std::shared_ptr<IActor> &p)
                    {
                        return p.get() == strong.get();
                }), container.end());

                if(oldsize != container.size())
                {
                    refitAllSigs();
                    render();
                }
            }
        }

    }

    void right(bool loop = true)
    {
        if(currentMode == editionMode::None)
        {
            return;
        }
        else if(currentMode == editionMode::Procs)
        {
            if(processes.size() == 0)return;
        }        
        else if(currentMode == editionMode::Signals)
        {
            if(signals.size() == 0)return;
        }

        auto strong = procSelected.lock();

        deselect(false);

        if(!strong)
        {
            switch(currentMode)
            {
                case editionMode::Procs:
                {
                    procSelected = strong = *processes.begin();
                    strong->selected = true;
                    break;
                }
                case editionMode::Signals:
                {
                    procSelected = strong = *signals.begin();
                    strong->selected = true;
                    break;
                }
                default: //niby jak
                    assert(0 == 1 && "Soft fault");
                    break;
            }
        }
        else
        {
            switch(currentMode)
            {
                case editionMode::Procs:
                {   
                    auto it = getIteratorFromPointerActual<Process>(std::dynamic_pointer_cast<Process>(strong), processes);
                    std::advance(it, 1);
                    if(it != processes.end())
                    {
                        procSelected = strong = *it;
                        strong->selected = true;
                    }
                    else if(loop)
                    {
                        procSelected = strong = *processes.begin();
                        strong->selected = true;
                    }
                    break;
                }
                case editionMode::Signals:
                {
                    auto it = getIteratorFromPointerActual<Signal>(std::dynamic_pointer_cast<Signal>(strong), signals);
                    std::advance(it, 1);
                    if(it != signals.end())
                    {
                        procSelected = strong = *it;
                        strong->selected = true;
                    }
                    else if(loop)
                    {
                        procSelected = strong = *signals.begin();
                        strong->selected = true;
                    }
                    break;
                }
                default: //niby jak
                    assert(0 == 1 && "Soft fault");
                    break;
            }
        }

        if(strong)
        {
            center();
        }

        render();
    }

    void left(bool loop = true)
    {
        if(currentMode == editionMode::None)
        {
            return;
        }
        else if(currentMode == editionMode::Procs)
        {
            if(processes.size() == 0)return;
        }        
        else if(currentMode == editionMode::Signals)
        {
            if(signals.size() == 0)return;
        }

        auto strong = procSelected.lock();

        deselect(false);

        if(!strong)
        {
            switch(currentMode)
            {
                case editionMode::Procs:
                {
                    procSelected = strong = *processes.crbegin();
                    strong->selected = true;
                    break;
                }
                case editionMode::Signals:
                {
                    procSelected = strong = *signals.crbegin();
                    strong->selected = true;
                    break;
                }
                default: //niby jak
                    assert(0 == 1 && "Soft fault");
                    break;
            }
        }
        else
        {
            switch(currentMode)
            {
                case editionMode::Procs:
                {   
                    auto it = getIteratorFromPointerActualReversed<Process>(std::dynamic_pointer_cast<Process>(strong), processes);
                    std::advance(it, 1);
                    if(it != processes.crend())
                    {
                        procSelected = strong = *it;
                        strong->selected = true;
                    }
                    else if(loop)
                    {
                        procSelected = strong = *processes.crbegin();
                        strong->selected = true;
                    }
                    break;
                }
                case editionMode::Signals:
                {
                    auto it = getIteratorFromPointerActualReversed<Signal>(std::dynamic_pointer_cast<Signal>(strong), signals);
                    std::advance(it, 1);
                    if(it != signals.crend())
                    {
                        procSelected = strong = *it;
                        strong->selected = true;
                    }
                    else if(loop)
                    {
                        procSelected = strong = *signals.crbegin();
                        strong->selected = true;
                    }
                    break;
                }
                default: //niby jak
                    assert(0 == 1 && "Soft fault");
                    break;
            }
        }

        if(strong)
        {
            center();
        }

        render();
    }

    template <typename T>
    void insertCreateSignalGeneric(std::string from, std::string to)
    {
        if(currentMode != editionMode::Signals)
        {
            return;
        }
        auto strong = procSelected.lock();
        
        if(!strong)
        {
            return;
        }

        auto pos = getIteratorFromPointerActual(std::dynamic_pointer_cast<Signal>(strong), signals);

        auto it1 = findByName(from, processes);
        auto it2 = findByName(to, processes);

        if(it1 == processes.end() || it2 == processes.end())
        {
            return;
        }

        auto newsignal = std::make_shared<T>();

        newsignal->b = {0 /*to be set by process*/, actSignalPosition += minSignalSeparation};
        newsignal->e = {0 /*a.b.*/, actSignalPosition};

        newsignal->start = *it1;
        newsignal->end = *it2;

        //(*it1)->signalEndPositionVertical = actSignalPosition;
        //(*it2)->signalEndPositionVertical = actSignalPosition;

        (*it1)->pipeSignalFrom(newsignal);
        (*it2)->pipeSignalTo(newsignal);

        signals.insert(pos, std::move(newsignal));

        refitAllSigs();

        render();
    }

    template <typename T>
    void appendCreateSignalGeneric(std::string from, std::string to)
    {
        if(currentMode != editionMode::Signals)
        {
            return;
        }

        auto it1 = findByName(from, processes);
        auto it2 = findByName(to, processes);

        if(it1 == processes.end() || it2 == processes.end())
        {
            return;
        }

        auto newsignal = std::make_shared<T>();

        newsignal->b = {0 /*to be set by process*/, actSignalPosition += minSignalSeparation};
        newsignal->e = {0 /*a.b.*/, actSignalPosition};

        newsignal->start = *it1;
        newsignal->end = *it2;

        (*it1)->signalEndPositionVertical = actSignalPosition;
        (*it2)->signalEndPositionVertical = actSignalPosition;

        (*it1)->pipeSignalFrom(newsignal);
        (*it2)->pipeSignalTo(newsignal);

        signals.push_back(std::move(newsignal));

        render();
    }

    void render()
    {
        assert(rend != nullptr);

        rend->clearWindow();

        for(auto &i : processes)
        {
            i->draw(*rend);
        }

        for(auto &i : signals)
        {            
            i->lateConstructor();
            i->draw(*rend);
        }

        rend->update();
    }
};

#endif