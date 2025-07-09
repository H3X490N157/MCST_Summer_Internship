CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O1 -g -fsanitize=address,undefined

SRC = code.cpp
OBJ = $(SRC:.cpp=.o)
RUN = program

all: build

build: $(RUN)

$(RUN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(RUN)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(RUN)
	./$(RUN) < test_input.txt

clean:
	rm -f $(RUN) *.o

.PHONY: all build test clean
