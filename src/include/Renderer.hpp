#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include "IRenderer.hpp"
#include <panel.h>

class Renderer : public IRenderer
{
    public:
        Renderer() = default;
        Renderer(WINDOW *w, PANEL *p = nullptr) : win(w), pan(p) {}
        ~Renderer()
        {
            if(win != nullptr)delwin(win);
            if(pan != nullptr)del_panel(pan);
        }
        void setAttribute(Attr_T at) override
        {
            if(win != nullptr)wattron(win, at);
        }
        void unsetAttribute(Attr_T at) override
        {
            if(win != nullptr)wattroff(win, at);
        }
        void setCoordinates(unsigned w, unsigned h, unsigned x, unsigned y) override
        {
            if(win != nullptr)delwin(win);
            if(pan != nullptr)del_panel(pan);

            win = newwin(w, h, x, y);
            pan = new_panel(win);
        }
        virtual void clearWindow() override
        {
            if(win != nullptr)werase(win);
        }
        virtual void renderString(std::string s, Point p) override
        {
            if(win == nullptr || pan == nullptr)return;

            Point maxpos, relativePos, off;
            getmaxyx(win, maxpos.y, maxpos.x);

            if(center.x >= 0) off = (maxpos / 2) - center;
            else off = {0, 0};

            relativePos.x = p.x + off.x;
            relativePos.y = p.y + off.y;
            
            
            mvwprintw(win, relativePos.y, relativePos.x, "%*.*s", 1, maxpos.x - relativePos.x, s.c_str());
            wmove(win, 0, 0);
        }
        virtual void update() override
        {
            update_panels();
            doupdate();
        }
        virtual void setCenter(Point p) override
        {
            center = p;
        }

        virtual void resetCenter() override
        {
            if(win == nullptr)
            {
                center = {-1, -1};
                return;
            }

            Point p;
            getmaxyx(win, p.y, p.x);

            center = p / 2;
        }

    private:
        WINDOW *win{nullptr};
        PANEL *pan{nullptr};

        Point center;
};

#endif