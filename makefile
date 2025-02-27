CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror

SRC_DIR := src
OBJ_DIR := obj
BIN := test.exe

SRCS := $(wildcard $(SRC_DIR)/*.cpp) test.cpp
OBJS := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/$(SRC_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN)

run: $(BIN)
	./$(BIN)
