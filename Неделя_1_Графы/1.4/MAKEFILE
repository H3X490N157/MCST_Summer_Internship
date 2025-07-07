SRC = code.cpp
OBJ = code.o
EXE = program

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
all: build
build: $(EXE)

$(EXE): $(SRC) edge.h node.h graph.h dijkstra.h fordfulkerson.h tarjan.h
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXE)

test: $(EXE)
	@echo "Running tests..."
	@# Здесь будут тесты, например: ./program < input.txt
	@echo "No tests defined yet."

clean:
	rm -f $(EXE) *.o

.PHONY: all build test clean
