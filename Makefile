# Detect OS
ifeq ($(OS),Windows_NT)
    # Windows settings
    SRCS := $(shell cmd /c "dir /s /b *.cpp")
    OUTPUT := Syringa.exe
    RM := del /F /Q
else
    # Unix-like settings (Linux, macOS, etc.)
    SRCS := $(shell find . -name "*.cpp")
    OUTPUT := Syringa
    RM := rm -f
endif

# Set the compiler and compiler flags
CC := g++
CFLAGS := -std=c++20 -Iinclude -Iobject -Iinterpreter

# Add DEBUG macro to CFLAGS
ifdef DEBUG
CFLAGS += -DDEBUG -Wall -Wextra -g
CFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-reorder
endif

# Build rule
build: $(OUTPUT)

# Linking rule
$(OUTPUT): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(OUTPUT)

# Clean rule
clean:
	$(RM) $(OUTPUT)
