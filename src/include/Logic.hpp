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
    std::weak_ptr<Process> procSelected;

    static constexpr int minSpace = 12;
    static constexpr int minYPosition = 2;
    int actPosition{0};
    
    IRenderer *rend{nullptr};

    enum class editionMode
    {
        Actors,
        Signals,
    };

    editionMode currentMode{editionMode::Actors};

    void refitAll()
    {
        actPosition = 0;
        for(auto& i : processes)
        {
            i->setPosition({actPosition += minSpace, minYPosition});
        }
    }

    public:

    Logic() = default;

    void setRenderer(IRenderer * r)
    {
        rend = r;
    }

    void addProcess(std::string name)
    {
        Point p{actPosition += minSpace, minYPosition};
        processes.push_back(std::move(std::make_shared<Process>(name, p)));

        render();
    }

    void remProcess(std::string name)
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

    void selectByName(std::string name)
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

    void deselect(bool reset = false)
    {
        if(auto strong = procSelected.lock())
        {
            strong = procSelected.lock();
            strong->selected = false;
        }
        if(reset)
        {
            rend->resetCenter();
            render();
        }
    }

    void editSelected(std::string name)
    {
        if(auto strong = procSelected.lock())
        {
            auto oldsize = processes.size();

            auto it = std::find_if(processes.begin(), processes.end(), 
                [&strong](const std::shared_ptr<Process> &p)
                {
                    return p.get() == strong.get();
            });

            if(it != processes.end())
            {
                (*it)->setName(name);
                render();
            }
        }
    }

    void deleteSelected()
    {
        if(auto strong = procSelected.lock())
        {
            auto oldsize = processes.size();

            auto it = processes.erase(std::remove_if(processes.begin(), processes.end(), 
                [&strong](const std::shared_ptr<Process> &p)
                {
                    return p.get() == strong.get();
            }), processes.end());

            if(oldsize != processes.size())
            {
                refitAll();
                render();
            }
        }
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