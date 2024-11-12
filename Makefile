# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Target executable
TARGET = main

# Source files
SRCS = main.cpp MatrixMath.cpp Game.cpp NeuralNetwork.cpp Agent.cpp Trainer.cpp  

# Object files
OBJS = $(SRCS:.cpp=.o)

# Header files (for dependency tracking)
HEADERS = MatrixMath.h Game.h NeuralNetwork.h Agent.h Trainer.h  

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile each .cpp file into .o file
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run
