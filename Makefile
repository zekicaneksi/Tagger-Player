SOURCES=$(shell find src/ -name '*.cpp')
OBJECTS=$(patsubst %.cpp,%.o, $(SOURCES))

all: main

$(OBJECTS): src/%.o : src/%.cpp
	g++ -c src/$*.cpp -o build/$(notdir $*).o `wx-config --cxxflags --libs`

main: $(OBJECTS)
	g++ -o build/main `wx-config --cxxflags --libs` $(wildcard build/*.o)
