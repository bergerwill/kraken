OBJ_DIR := build
BIN_DIR := bin
SOURCE := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SOURCE))
CFLAGS := -std=c++17 -Wc++11-extensions -g -O0 -pthread -I/usr/local/include/boost_1_55_0

orderbooksimulator: $(OBJS)
	@mkdir -p $(BIN_DIR)
	g++ -o $(BIN_DIR)/$@ $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(OBJ_DIR)
	g++ $(CFLAGS) -c $< -o $@ -Iinclude

clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/orderbooksimulator
