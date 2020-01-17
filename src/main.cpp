
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
    std::cout<<"Koniec\n";
}
