CXX = g++-7
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -g -fsanitize=address
LDFLAGS =  -fsanitize=address

SRC = geneticalgorithm_main.cpp GeneticAlgorithm.cpp TSPbase.cpp LocalSearch.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = ../bin/genetic_algorithm

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)