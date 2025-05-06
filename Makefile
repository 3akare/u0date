CC = gcc
INCDIR = -Iinclude
CFLAGS = -Wall -Wextra -pedantic -Werror -g -O2 $(INCDIR)
LDFLAGS=-lncurses

SRC_DIR = src
BUILD_DIR = build

C_SOURCE_FILES = $(shell find $(SRC_DIR) -name "*.c")
OBJECT_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCE_FILES))
BINARY = $(BUILD_DIR)/u0date


all: $(BINARY)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINARY): $(OBJECT_FILES)
		$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)