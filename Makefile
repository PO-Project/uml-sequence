CXX=g++
CXXFLAGS=-Wall -Wextra -Wpedantic -g -I./common -I./src/include -I./backends/vim-backend/include -std=c++17 -g
LINKFLAGS= -L./backends/vim-backend/ -lVimBackend -lncurses -lpanel
TARGET=uml-seq.app

BASESOURCES=$(wildcard ./src/*.cpp)
SOURCES= $(BASESOURCES)
TARGETS= $(SOURCES:.cpp=.o)
BUILDSOUP = $(wildcard ./build/*.o)

.PHONY: sources targets all target clean rebuild
.SUFFIXES:

all: vim-app

vim-app: prep target
	cmake ./backends/vim-backend/ -B ./backends/vim-backend/
	cmake --build ./backends/vim-backend/
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(BUILDSOUP) $(LINKFLAGS) 

prep:
	[ -d build ] || mkdir build

target: $(TARGETS)

rebuild: clean all

clean:
	rm -rf $(BUILDSOUP)
	rm -f $(TARGET)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o build/$(notdir $@)

sources:
	@echo $(SOURCES)

targets:
	@echo $(TARGETS)
