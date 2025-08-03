CXX		 = g++

EXE		 = PictureCross

SRC		 = ../src
INCLUDE	 = ../include
LIB		 = ../lib

FLAGS	 = -std=c++20 -I$(INCLUDE) -L$(LIB) -O3
CXXFLAGS = -c -Wall $(FLAGS)
LDFLAGS	 = $(FLAGS) -o

OBJECTS	 = main.o Grid.o Solver.o HeaderInfo.o

all: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(EXE)

main.o: $(SRC)/main.cpp $(INCLUDE)/main.h $(INCLUDE)/Defines.h $(INCLUDE)/Array.h
	$(CXX) $(CXXFLAGS) $<

Grid.o: $(SRC)/Grid.cpp $(INCLUDE)/Grid.h $(INCLUDE)/Defines.h
	$(CXX) $(CXXFLAGS) $<

HeaderInfo.o: $(SRC)/HeaderInfo.cpp $(INCLUDE)/HeaderInfo.h $(INCLUDE)/Defines.h
	$(CXX) $(CXXFLAGS) $<

Solver.o: $(SRC)/Solver.cpp $(INCLUDE)/Solver.h $(INCLUDE)/Defines.h
	$(CXX) $(CXXFLAGS) $<

clean:
	del -q *.exe *.o
