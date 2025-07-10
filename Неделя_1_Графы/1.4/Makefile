CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O1 -g -fsanitize=address,undefined

#консоль
SRC = code.cpp
OBJ = $(SRC:.cpp=.o)
RUN = program

#тесты
TEST_SRC = tests.cpp 
TEST_EXE = tests

all: build
build: $(RUN) 
	./$(RUN)

$(RUN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(RUN)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Сборка и запуск тестов
test: $(TEST_EXE)
	./$(TEST_EXE)

$(TEST_EXE): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $(TEST_EXE)

clean:
	rm -f $(RUN) $(TEST_EXE) *.o

.PHONY: all build test clean
