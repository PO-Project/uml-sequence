
// Compile with: g++ main.cpp -lncurses -o example

#include <iostream>
#include "Uml-seq.hpp"
#include "VimBackend.hpp"
#include "ExampleTool.hpp"
#include "include/Logic.hpp"

int main(int argc, char *argv[])
{
    tools::UmlSeq tool;
    backends::VimBackend backend;
    backend.setTool(tool);
    tool.setBackend(backend);
    backend();
    std::cout<<"\n";
/* raw test sect */
        //Line l1({2,2}, {2,7});
    //Line l2({3,3}, {7,3});
    /*Box b1("Box1234", {10,10});
    //Box b2("Box5678", {74,31});
    //Box b3("BoxABCD", {0,0});
    Process p("Niedziewdz123", {0, 0});
    p.selected = true;
initscr();
    auto W = newwin(40, 120, 0, 0);

    Renderer r;//(W, new_panel(W));
    r.setCoordinates(40, 120, 0, 0);
    //r.setCenter({60,20});
    r.resetCenter();

    Logic lg;
    lg.setRenderer(&r);
    lg.addProcess("Proc12");

    //p.draw(r);

    //l2.draw(r);
    //l1.draw(r);
    //b1.draw(r);
    //b2.draw(r);
    //b3.draw(r);

   r.update();


refresh();
getch();
endwin();*/

}
