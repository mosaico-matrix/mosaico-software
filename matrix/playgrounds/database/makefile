# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Linker flags
LDFLAGS = -lsqlite3

# Target executable
TARGET = database

# Source file
SRC = database.cpp

# Object file
OBJ = $(SRC:.cpp=.o)

# Default rule
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

# Rule to compile the source file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets
.PHONY: all clean
