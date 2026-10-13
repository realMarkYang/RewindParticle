ifeq ($(OS), Windows_NT)
    PLATFORM = WINDOWS
else
    PLATFORM = LINUX
endif

CC          = gcc
CFLAGS      = -Wall -Wextra -Wpedantic -O1 -std=c99
SRC         = main.c 
BUILD_DIR   = build
TARGET      = $(BUILD_DIR)/particle 

ifeq ($(PLATFORM), WINDOWS)
    TARGET    = $(BUILD_DIR)/particle.exe
    VCPKG     = C:/vcpkg
    CFLAGS   += -I$(VCPKG)/installed/x64-mingw-static/include
    LIBS      = -L$(VCPKG)/installed/x64-mingw-static/lib -lraylib -lglfw3 -lgdi32 -luser32 -lwinmm -lopengl32 -static
    RM_CMD    = rmdir /S /Q $(BUILD_DIR)
    MKDIR_CMD = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
    RC        = windres
    
endif

ifeq ($(PLATFORM), LINUX)
    CFLAGS   += -D_POSIX_C_SOURCE=199309L
    LIBS      = -lraylib -lX11 -lXext -lXrandr -lGL -lm
    RM_CMD    = rm -rf $(BUILD_DIR)
    MKDIR_CMD = mkdir -p $(BUILD_DIR)
endif


all: $(TARGET)

$(TARGET): $(SRC) | $(BUILD_DIR)
ifeq ($(PLATFORM), WINDOWS)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LIBS)
else
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
endif

$(BUILD_DIR):
	$(MKDIR_CMD)


run: all
ifeq ($(PLATFORM), WINDOWS)
	cd $(BUILD_DIR) && .\$(notdir $(TARGET))
else
	cd $(BUILD_DIR) && ./$(notdir $(TARGET))
endif

clean:
	$(RM_CMD)

.PHONY: all run clean