# Compiler
CXX = g++

# Compiler flags
# -std=c++20: Use the C++20 standard
# -Wall: Enable all warnings
# -g: Generate debugging information
CXXFLAGS = -std=c++20 -Wall -g

# The name of the executable to be created
TARGET = scheduler

# The source files (.cpp)
SOURCES = main.cpp scheduler.cpp

# The object files (.o) that will be generated from the source files
OBJECTS = $(SOURCES:.cpp=.o)

# The default rule, which is executed when you run `make`
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to compile a .cpp source file into a .o object file
# The $< is the first prerequisite (the .cpp file)
# The $@ is the target (the .o file)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# The "clean" rule is used to remove generated files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets are not actual files. `make` will always run them.
.PHONY: all clean