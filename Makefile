SOURCES=$(shell find src/ -name '*.cpp')
OBJECTS=$(patsubst %.cpp,%.o, $(SOURCES))

all: main

LIBS=`wx-config --cxxflags --libs` -lsqlite3

$(OBJECTS): src/%.o : src/%.cpp
	g++ -c src/$*.cpp -o build/$(notdir $*).o $(LIBS)

main: $(OBJECTS)
	g++ -o build/main $(LIBS) $(wildcard build/*.o)
