CC := g++
FLAGS := -std=c++11 -w
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
TEST_DIR := test
INCLUDE := -I./$(INC_DIR)
GMP_DIR := gmp-build
LIB := -L./lib -lgmp
ROOT:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

$(BIN_DIR)/main: $(BUILD_DIR)/main.o $(BUILD_DIR)/Int.o $(BUILD_DIR)/Rsa.o $(BUILD_DIR)/OctetString.o $(BUILD_DIR)/KeyProducing.o $(BUILD_DIR)/Encryption.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE) $(LIB) $^ -o $@
	./bin/main

test: $(BUILD_DIR)/test.o $(BUILD_DIR)/Rsa.o $(BUILD_DIR)/KeyProducing.o $(BUILD_DIR)/Encryption.o $(BUILD_DIR)/OctetString.o $(BUILD_DIR)/Int.o
	$(CC) $(FLAGS) $(INCLUDE) $(LIB) $^ -o $@
	./test
	@rm test


gmp:
	@mkdir -p $(GMP_DIR)
	@apt-get install m4 -y
	@apt-get install texinfo
	@cd gmp-6.2.0 && ./configure -prefix=$(ROOT)/$(GMP_DIR) && make && make install && cd -
	@cp -r $(GMP_DIR)/lib . && cp -r $(GMP_DIR)/include .

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) $(LIB) -c -o $@ $<

.PHONY: clean


clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)