CXX = g++
CXXFLAGS = -pthread
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# List of source files excluding Functions.cpp
SRCS = $(filter-out $(SRC_DIR)/Functions.cpp, $(wildcard $(SRC_DIR)/*.cpp))

# List of object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# List of executable files
EXECUTABLES = $(patsubst $(SRC_DIR)/%.cpp, %, $(SRCS))

all: $(EXECUTABLES)

# Rule to build each executable
%: $(OBJS) $(BUILD_DIR)/Functions.o
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(BUILD_DIR)/Functions.o

# Rule to build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

# Rule to build Functions.o separately
$(BUILD_DIR)/Functions.o: $(SRC_DIR)/Functions.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

clean:
	rm -f $(EXECUTABLES) $(BUILD_DIR)/*.o