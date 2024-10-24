# Set the compiler
CXX = clang++

# Set the compiler flags
CXXFLAGS = -std=gnu++14 -g

# Name of the executable
TARGET = main

# Source files
SRCS = main.cpp imputArguments.cpp ExpenseSheet.cpp


# Object files (generated automatically from source files)
OBJS = $(SRCS:.cpp=.o)

# Rule for building the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule for compiling source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove compiled files
clean:
	rm -f $(OBJS) $(TARGET)