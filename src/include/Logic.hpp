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
    
    IRenderer *rend{nullptr};

    enum class editionMode
    {
        Procs,
        Signals,
        None = 0xF,
    };

    editionMode currentMode{editionMode::None};

    void refitAll()
    {
        actPosition = 0;
        for(auto& i : processes)
        {
            i->setPosition({actPosition += minSpace, minYPosition});
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

    public:

    Logic() = default;

    void setRenderer(IRenderer * r)
    {
        rend = r;
    }

    void setEditionMode(std::string mode)
    {
        std::for_each(mode.begin(), mode.begin(), [](char &c)
        {
            c = tolower(c);
        });

        if(mode == "actors" || mode == "actor" || mode == "process" || mode == "processes" || mode == "procs" || mode == "proc")
        {
            currentMode = editionMode::Procs;
        }
        else if(mode == "signals" || mode == "signal" || mode == "sign")
        {
            currentMode = editionMode::Signals;
        }
        else
        {
            currentMode = editionMode::None;
        }

        procSelected.reset();
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

            auto it = processes.erase(std::remove_if(processes.begin(), processes.end(), 
                [&name](const std::shared_ptr<Process> &p)
                {
                    return p->getName() == name;
            }), processes.end());

            if(oldsize != processes.size())
            {
                refitAll();
                render();
            }
        }
    }

    void selectByName(std::string name)
    {
        if(currentMode == editionMode::Procs)
        {
            auto it = (std::find_if(processes.begin(), processes.end(), 
                [&name](const std::shared_ptr<Process> &p)
                {
                    return p->getName() == name;
            }));

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

    void deleteSelected() //bad design
    {
        if(currentMode == editionMode::Procs)
        {
            auto & container = processes;
            if(auto strong = procSelected.lock())
            {
                auto oldsize = container.size();

                auto it = container.erase(std::remove_if(container.begin(), container.end(), 
                    [&strong](const std::shared_ptr<IActor> &p)
                    {
                        return p.get() == strong.get();
                }), container.end());

                if(oldsize != container.size())
                {
                    refitAll();
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

                auto it = container.erase(std::remove_if(container.begin(), container.end(), 
                    [&strong](const std::shared_ptr<IActor> &p)
                    {
                        return p.get() == strong.get();
                }), container.end());

                if(oldsize != container.size())
                {
                    refitAll();
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

        rend->update();
    }
};

#endif