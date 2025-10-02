#\
--------Automatic Variables-------- \
									\
$@ - Target of a rule				\
$< - First dependency of a rule		\
$^ - All dependencies of a rule		\
									\
-----------------------------------

# Output File Name
NAME = main

# Compiler and Flags
CXX = g++
CXXFLAGS = -g -Wall -Wextra -std=c++17

# Define Headers
HEADERS = 

# Source and Objecct Files
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = $(NAME)

# Default Target
all: $(TARGET)

# Link Object Files into Executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

#Compile .cpp files into .o
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run Target
run: $(TARGET)
	./$(TARGET)

# Run Target and Clean
runclean: $(TARGET)
	./$(TARGET)
	$(MAKE) clean

#Clean up Object files and executable
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean run runclean