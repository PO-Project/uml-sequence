#ifndef LOGIC_HPP_
#define LOGIC_HPP_

#include "Arrow.hpp"
#include "Renderer.hpp"
#include "Process.hpp"
#include <memory>
#include <cassert>

class Logic
{
    std::vector<Process> processes;

    static constexpr int minSpace = 12;
    int actPosition{0};
    
    IRenderer *rend{nullptr};

    void refitAll()
    {
        actPosition = 0;
        for(auto& i : processes)
        {
            i.setPosition({actPosition += minSpace, 0});
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
        Point p{actPosition += minSpace, 0};
        processes.push_back({name, p});

        render();
    }

    void remProcess(std::string name)
    {
        auto it = processes.erase(std::remove_if(processes.begin(), processes.end(), 
            [&name](const Process &p)
            {
                return p.getName() == name;
        }), processes.end());

        /*if(it != processes.end())*/refitAll();

        render();
    }

    void render()
    {
        assert(rend != nullptr);

        rend->clearWindow();

        for(auto &i : processes)
        {
            i.draw(*rend);
        }

        rend->update();
    }
};

#endif