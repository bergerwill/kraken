CXX := g++
CXXFLAGS := -std=c++14 -Wall -Wextra -pthread
SRC_DIR := ../src
INCLUDE_DIR := ../include
BUILD_DIR := build
BIN_DIR := bin
TEST_SRCS := $(filter-out main.cpp,$(wildcard *.cpp))
TEST_OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))
TARGET := $(BIN_DIR)/test_executable
LIBS := -lgtest -lgtest_main

# Collect all source and header files except main.cpp
SRC_FILES := $(filter-out $(SRC_DIR)/main.cpp,$(wildcard $(SRC_DIR)/*.cpp))
HEADER_FILES := $(wildcard $(INCLUDE_DIR)/*.h)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(TEST_OBJS) $(SRC_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) -I$(INCLUDE_DIR)

$(BUILD_DIR)/%.o: %.cpp $(HEADER_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_DIR) -I$(SRC_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
