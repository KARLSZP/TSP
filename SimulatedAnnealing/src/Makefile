CXX = g++-7
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -g -fsanitize=address
LDFLAGS =  -fsanitize=address

SRC = simulatedannealing_main.cpp SimulatedAnnealing.cpp TSPbase.cpp LocalSearch.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = ../bin/simulated_annealing

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)